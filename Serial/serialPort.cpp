/**
* @file		serialPort.h
*	@author		Nahum Budin
*	@date		13-May-2024
*	@version	1.1
*					1. Add Tx thread
*					2. Add a state machine to parse the source and length bytes.
*
*	@brief		Handle the serial port full duplext external MIDI interface streams
*	
*	History:\n
*	
*	version	1.0		21-Oct-2020
*	
*port nr.	Linux	
*	0		ttyS0   | 1		ttyS1   | 2		ttyS2   | 3		ttyS3	
*	4		ttyS4   | 5		ttyS5   | 6		ttyS6   | 7		ttyS7	
*	8		ttyS8   | 9		ttyS9   | 10	ttyS10  | 11	ttyS11	
*	12		ttyS12  | 13	ttyS13  | 14	ttyS14  | 15	ttyS15
*	16		ttyUSB0 | 17	ttyUSB1 | 18	ttyUSB2 | 19	ttyUSB3
*	20		ttyUSB4 | 21	ttyUSB5 | 22	ttyAMA0 | 23	ttyAMA1
*	24		ttyACM0 | 25	ttyACM1 | 26	rfcomm0 | 27	rfcomm1
*	28		ircomm0 | 29	ircomm1 | 30	cuau0   | 31	cuau1
*	32		cuau2   | 33	cuau3   | 34	cuaU0   | 35	cuaU1
*	36		cuaU2   | 37	cuaU3
*	
*	Mode	8N1	8O1	8E1	8N2	8O2	8E2	7N1	7O1	7E1	7N2	7O2	7E2	6N1	6O1	6E1	6N2	6O2	6E2	5N1	5O1	5E1	5N2	5O2	5E2
*/

#include "serialPort.h"
#include "../misc/priorities.h"

// #define _SERIAL_DBG_PRINT_IS_ON

bool SerialPort::serial_port_rx_thread_is_running = false;
bool SerialPort::serial_port_tx_thread_is_running = false;
pthread_t SerialPort::serial_port_id; // = NULL;
bool SerialPort::serial_port_is_opened = false;
int SerialPort::port_number = 1;
char SerialPort::default_mode[3] = { '8', 'N', '1' };
SerialPort *SerialPort::serial_port_instance = NULL;
ADJRS232 *SerialPort::port;
SafeQueue<serialPortData_t *> SerialPort::alsa_control_box_control_serial_port_rx_queue;
SafeQueue<serialPortData_t *> SerialPort::alsa_control_box_ext_midi_in_port_serial_port_rx_queue;
SafeQueue<unsigned char *> SerialPort::serial_port_tx_queue;

SerialPort *SerialPort::get_serial_port_instance(int port_num)
{
	if (serial_port_instance == NULL)
	{
		serial_port_instance = new SerialPort(port_num);
	}

	return serial_port_instance;
}

SerialPort::SerialPort(int serNum)
{
	serial_port_instance = this;
	port_number = serNum;
	port = ADJRS232::getAdjRS232instance();
}

void SerialPort::start_serial_port_RX_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the Serial Port thread is not allready running
	if (serial_port_rx_thread_is_running)
		return;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_SERIAL_PORT;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	//	err = errno; // for debug
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting Serial Port Rx thread realtime prio\n");
	}
	// Set the control flag
	serial_port_rx_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&serial_port_id, &tattr, serial_port_rx_thread, (void *)500);
	char thname[] = { "serial_portr_thread" };
	pthread_setname_np(serial_port_id, &thname[0]);
}

void SerialPort::stop_serial_port_RX_thread()
{
	if (port)
	{
		close_port();
		serial_port_is_opened = false;
	}

	// Reset the control flag to terminate the thread
	serial_port_rx_thread_is_running = false;
}

void SerialPort::start_serial_port_TX_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the Serial Port thread is not allready running
	if (serial_port_tx_thread_is_running)
		return;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_SERIAL_PORT;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	//	err = errno; // for debug
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting Serial Port Tx thread realtime prio\n");
	}
	// Set the control flag
	serial_port_tx_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&serial_port_id, &tattr, serial_port_tx_thread, (void *)500);
	char thname[] = { "serial_portt_thread" };
	pthread_setname_np(serial_port_id, &thname[0]);
}

void SerialPort::stop_serial_port_TX_thread()
{
	if (port)
	{
		close_port();
		serial_port_is_opened = false;
	}

	// Reset the control flag to terminate the thread
	serial_port_tx_thread_is_running = false;
}

int SerialPort::open_port(int portNum, int baud, const char *mode, int flowCtl)
{
	if (port == NULL)
	{
		printf("Comport not initiated\n");

		return -1;
	}

	if (port->open_comport(portNum, baud, mode, flowCtl))
	{
		printf("Can not open comport\n");

		return (-2);
	}

	port_number = portNum;
	serial_port_is_opened = true;

	start_serial_port_RX_thread();

	return 0;
}

int SerialPort::close_port()
{
	if (port == NULL)
	{
		printf("Comport not initiated\n");

		return -1;
	}

	serial_port_is_opened = false;
	port->close_comport(port_number);

	return 0;
}

//#define _SERIAL_DBG_PRINT_IS_ON

void *SerialPort::serial_port_rx_thread(void *threadid)
{
	int num_of_RX_bytes = 0, i;
	unsigned char rx_buf[1024];
	
	uint8_t source, len;
	uint state = _SER_STATE_WAIT_FOR_MIDI_COMMAND_START;
	uint8_t new_char = 0, prev_char = 0;

	serialPortData_t *serialPortRxData;

	fprintf(stderr, "Serial Port rx thread started\n");

	while (serial_port_rx_thread_is_running)
	{
		if (serial_port_is_opened)
		{
			num_of_RX_bytes = port->poll_comport(port_number, rx_buf, 1 /*sizeof(rx_buf)*/); // byte by byte

			if (num_of_RX_bytes > 0)
			{
				// port->flush_RX(port_number);
				if (state == _SER_STATE_WAIT_FOR_MIDI_COMMAND_START)
				{					
					if (rx_buf[0] < 0x80)
					{
						prev_char = new_char;
						new_char = rx_buf[0];
					}
					else
					{
						source = prev_char;   // 1st byte of message is the source
						len = new_char; // 2nd byte is the length
						
						
						/* 1st byte of new data received - 
						 * Create a new rx data object (Don't forget to free after processing!!!) */
						serialPortRxData = new serialPortData_t;
						serialPortRxData->port_num = port_number;
						serialPortRxData->mssg_len = num_of_RX_bytes;
						serialPortRxData->message[0] = rx_buf[0];
						
						if (source == _CONTROL_BOX_EXT_MIDI_INTERFACE)
						{
							alsa_control_box_ext_midi_in_port_serial_port_rx_queue.enqueue(serialPortRxData);							
							
						}
						else if (source == _CONTROL_BOX_CONTROL_INTREFACE)
						{
							alsa_control_box_control_serial_port_rx_queue.enqueue(serialPortRxData);
						}
						
#ifdef _SERIAL_DBG_PRINT_IS_ON
						printf("Ser port rx bytes: n=%i  ", num_of_RX_bytes);
						for (i = 0; i < num_of_RX_bytes; i++)
						{
							printf("%x ", rx_buf[i]);
						}
						printf("\n");
#endif

						state = _SER_STATE_MIDI_COMMAND_IN_PROGRESS;
					}
				}
				else if (state == _SER_STATE_MIDI_COMMAND_IN_PROGRESS)
				{
					/* Next bytes of data received - 
					 * Create a new rx data object (Don't forget to free after processing!!!) */
					serialPortRxData = new serialPortData_t;
					serialPortRxData->port_num = port_number;
					serialPortRxData->mssg_len = num_of_RX_bytes;
									
					for (i = 0; (i < num_of_RX_bytes) && (i < sizeof(serialPortRxData->message)); i++)
					{
						serialPortRxData->message[i] = rx_buf[i];
					}

					if (serialPortRxData->mssg_len > 100)
					{
						printf("ser data big");
					}
					
					if (source == _CONTROL_BOX_EXT_MIDI_INTERFACE)
					{
						alsa_control_box_ext_midi_in_port_serial_port_rx_queue.enqueue(serialPortRxData);							
					}
					else if (source == _CONTROL_BOX_CONTROL_INTREFACE)
					{
						alsa_control_box_control_serial_port_rx_queue.enqueue(serialPortRxData);
					}
					
					len--;
					
					if (len <= 1)
					{
						state = _SER_STATE_WAIT_FOR_MIDI_COMMAND_START;
					}

#ifdef _SERIAL_DBG_PRINT_IS_ON
					printf("Ser port rx bytes: n=%i  ", num_of_RX_bytes);
					for (i = 0; i < num_of_RX_bytes; i++)
					{
						printf("%x ", rx_buf[i]);
					}
					printf("\n");
#endif
				}
				else
				{
					printf("ser data seq error");
				}

			}
		}

		usleep(1000);
	}

	return NULL;
}

void *SerialPort::serial_port_tx_thread(void *threadid)
{
	int num_of_TX_bytes = 0, i;
	unsigned char tx_buf[1024];
	unsigned char* tx_data;
	bool timeout, blocking = false;
	int count;
	
	fprintf(stderr, "Serial Port rx thread started\n");

	while (serial_port_tx_thread_is_running)
	{
		if (serial_port_is_opened)
		{
			// Get all data from Q
			count = 0;
			tx_data = serial_port_tx_queue.dequeue(0, &timeout, &blocking); //// Non blocking get from Q
			while (tx_data != NULL)
			{
				tx_buf[count] = *tx_data;
				count++;
				tx_data = serial_port_tx_queue.dequeue(0, &timeout, &blocking);
			}
			if (count > 0)
			{
				port->send_buf(port_number, tx_buf, count);
			}
		}
		
		usleep(1000);
	}
	
	return NULL;
}
