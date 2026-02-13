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

#pragma once

#define	_FLOW_CONTROL_ENABLED	1
#define	_FLOW_CONTROL_DISABLED	0

#define _NUM_OF_RX_TX_BYTES	128

#define _SER_STATE_WAIT_FOR_MIDI_COMMAND_START 0
#define _SER_STATE_MIDI_COMMAND_IN_PROGRESS 1

#define _CONTROL_BOX_EXT_MIDI_INTERFACE 0
#define _CONTROL_BOX_CONTROL_INTREFACE 1

#include <stdint.h>

#include "../utils/safeQueues.h"

#include "adjRS232.h"

typedef struct serialPortData
{
	serialPortData()
		: port_num(16)
		, mssg_len(0) {}

	int port_num;
	uint8_t message[_NUM_OF_RX_TX_BYTES];
	int mssg_len;
} serialPortData_t;

class SerialPort
{
public:
	static SerialPort *get_serial_port_instance(int port_num);
	~SerialPort();

	void start_serial_port_RX_thread();
	void stop_serial_port_RX_thread();
	
	void start_serial_port_TX_thread();
	void stop_serial_port_TX_thread();
	
	int open_port(int portNum = 1, int baud = 115200, const char *mode = default_mode, int flowCtl = _FLOW_CONTROL_DISABLED);
	int close_port();

	/* A queue used to send into the system the incoming control box data as ALSA midi events */
	static SafeQueue<serialPortData_t *> alsa_control_box_control_serial_port_rx_queue;
	/* A queue used to send into the system the incoming control box external MIDI input port data as ALSA midi events */
	static SafeQueue<serialPortData_t *> alsa_control_box_ext_midi_in_port_serial_port_rx_queue;
	
	
	/* A queue used to send the outgoing serial port data raw data */
	static SafeQueue<unsigned char *> serial_port_tx_queue;

private:
	SerialPort(int portNum = 1);
	static ADJRS232 *port;

	static void *serial_port_rx_thread(void *thread_id);
	static void *serial_port_tx_thread(void *thread_id);

	static SerialPort *serial_port_instance;
	static bool serial_port_rx_thread_is_running;
	static bool serial_port_tx_thread_is_running;
	static pthread_t serial_port_id;
	static bool serial_port_is_opened;

	static int port_number; // See table above

	static char default_mode[3];
};


