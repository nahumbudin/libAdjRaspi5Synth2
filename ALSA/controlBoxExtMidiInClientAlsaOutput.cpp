/**
*	@file		controlBoxExtMidiInClientAlsaOutput.cpp
*	@author		Nahum Budin
*	@date		24-6-2024
*	@version	1.1 
*					1. Customizing to Control Box external MIDI interface
*
*	@brief		Serial Port input based alsa output client (serial->alsa)
*	
*	History:
*				Ver 1.0 27-11-2020   serialPortClientAlsaOutput.h
*				
*/

#include "controlBoxExtMidiInClientAlsaOutput.h"
#include "../Serial/serialPort.h"
#include "../Misc/priorities.h"

bool ControlBoxExtMidiInClientAlsaOutput::alsa_out_thread_is_running = false;
pthread_t ControlBoxExtMidiInClientAlsaOutput::alsa_out_thread_id;
snd_rawmidi_t *ControlBoxExtMidiInClientAlsaOutput::midiout = NULL;
ControlBoxExtMidiInClientAlsaOutput *ControlBoxExtMidiInClientAlsaOutput::control_box_ext_midi_in_client_alsa_output = NULL;

/* Will hold data received from Serial port */
serialPortData_t *serial_port_rx_alsa_channel_data = NULL;

ControlBoxExtMidiInClientAlsaOutput::ControlBoxExtMidiInClientAlsaOutput()
{
	const char *portname = "virtual";
	int status;

	mode = SND_RAWMIDI_SYNC;

	if ((status = snd_rawmidi_open(NULL, &midiout, portname, mode)) < 0)
	{
		printf("Problem opening Ext MIDI Int Alsa output: %s", snd_strerror(status));
		//		exit(1);

	}

	snd_rawmidi_info_t *info;	
	snd_rawmidi_info_malloc(&info);
	fprintf(stderr, "id %u\n", snd_rawmidi_info_get_subdevice(info));
	snd_rawmidi_info_free(info);
}

ControlBoxExtMidiInClientAlsaOutput::~ControlBoxExtMidiInClientAlsaOutput()
{
	snd_rawmidi_close(midiout);
	midiout = NULL; // snd_rawmidi_close() does not clear invalid pointer,
}

/**
*   @brief  retruns the single serial port client alsa output instance
*
*   @param  none
*   @return the single serial port client alsa output
*/
ControlBoxExtMidiInClientAlsaOutput *ControlBoxExtMidiInClientAlsaOutput::get_instance()
{
	if (control_box_ext_midi_in_client_alsa_output == NULL)
		control_box_ext_midi_in_client_alsa_output = new(ControlBoxExtMidiInClientAlsaOutput);

	return control_box_ext_midi_in_client_alsa_output;
}

/**
 * Starts the serial port client thread looking for new bt data
 */
void ControlBoxExtMidiInClientAlsaOutput::startAlsaOutThread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the BT thread is not allready running
	if (alsa_out_thread_is_running)
		return;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_CTLB;
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
		fprintf(stderr, "Unsuccessful in setting serial port alsa out thread realtime prio\n");
	}
	// Set the control flag
	alsa_out_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&alsa_out_thread_id, &tattr, alsaOutThread, (void *)100);
	char thname[] = { "controlextmidialsaoutthread" };
	pthread_setname_np(alsa_out_thread_id, &thname[0]);
}

/**
 * Stop the serial port client thread.
 */
void ControlBoxExtMidiInClientAlsaOutput::stopAlsaOutThread()
{
	// Reset the control flag to terminate the thread
	alsa_out_thread_is_running = false;
}

/**
* Serial alsa output thread.
*
* 1. Polls for a new data in the serial port Rx alsa data queue every 5msc.
* 2. Sends new data through the alsa output
*/
void *ControlBoxExtMidiInClientAlsaOutput::alsaOutThread(void *threadid)
{
	bool timeout = false, blocking = true, non_blocking = false;
	int count, last_count, status, timeouts = 0;
	uint8_t bytes[512];

	fprintf(stderr, "Serial Prt alsa out thread started\n");

	while (alsa_out_thread_is_running)
	{
		// Wait for new data - non blocking
		serial_port_rx_alsa_channel_data = SerialPort::alsa_control_box_ext_midi_in_port_serial_port_rx_queue.dequeue(0, &timeout, &non_blocking);
		// Get all new data
		count = 0;
		last_count = 0;
		while (serial_port_rx_alsa_channel_data != NULL)
		{
			// Collect all available data
			for (count; count < (last_count + serial_port_rx_alsa_channel_data->mssg_len) &&
						count < sizeof(serial_port_rx_alsa_channel_data->message) &&
						count < sizeof(bytes);
				 count++)
			{
				bytes[count] = serial_port_rx_alsa_channel_data->message[count - last_count];
			}
			last_count = count;
			delete serial_port_rx_alsa_channel_data;
			serial_port_rx_alsa_channel_data = SerialPort::alsa_control_box_ext_midi_in_port_serial_port_rx_queue.dequeue(0, &timeout, &non_blocking);
			//			printf("Serial port in data alsa thread while data\n");
		}

		if (count > 0)
		{
			// New data is available
			//			printf("\nSerial Port in data alsa thread: count: %i  data: ", count);
			//			for (int i = 0; i < count; i++)
			//				printf("%x ", bytes[i]);
			//			printf("\n");

			if ((status = snd_rawmidi_write(midiout, bytes, count)) < 0)
			{
				printf("Problem writing serial port data to MIDI output: %s", snd_strerror(status));
				//					exit(1);
			}
		}

		usleep(1000);
	}

	return 0;
}
