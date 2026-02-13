/**
* @file		alsaBtClientOutput.cpp
*	@author		Nahum Budin
*	@date		15-May-2024
*	@version	1.2
*					1. Update includes.
*
*	@brief		Handles the BT RFCOMM (SPP) midi input streams and
*				uses ALSA output (source) to enable connecting them to any alsa input.
*
*
*	History:\n
*	
*	version	1.1		6-Feb-2021
*					1. Code refactoring and notaion.
*
*	version 1.0		11-Oct-2019:
*	
	First version
*/

#include "alsaBtClientOutput.h"
#include "../Bluetooth/rspiBluetoothServicesQueuesVer.h"
#include "../Misc/priorities.h"

bool AlsaBtClientOutput::bt_alsa_out_thread_is_running = false;
pthread_t AlsaBtClientOutput::bt_alsa_out_thread_id;
snd_rawmidi_t* AlsaBtClientOutput::midi_out = NULL;
AlsaBtClientOutput* AlsaBtClientOutput::alsa_bt_client_output = NULL;

/* Will hold data received from BT */
bt_chan_data_t* bt_rx_alsa_channel_data = NULL;

AlsaBtClientOutput::AlsaBtClientOutput()
{
	const char* port_name = "virtual";
	int status;
	snd_seq_client_info_t *info;
	snd_seq_client_info_alloca(&info);
	// snd_seq_t *handler;

	const char *name;
	
	int card;

	mode = SND_RAWMIDI_SYNC;


	if ((status = snd_rawmidi_open(NULL, &midi_out, port_name, mode)) < 0)
	{
		printf("ALSA BT client: Problem opening MIDI output: %s", snd_strerror(status));
		//		exit(1);
	}

	//status = snd_seq_get_client_info(handler, info);
	//card = snd_seq_client_info_get_client(info);

	status = 0;
}

AlsaBtClientOutput::~AlsaBtClientOutput()
{
	snd_rawmidi_close(midi_out);
	midi_out = NULL;    // snd_rawmidi_close() does not clear invalid pointer,
}

/**
*   @brief  retruns the single bt client alsa output instance
*
*   @param  none
*   @return the single bt client alsa output
*/
AlsaBtClientOutput* AlsaBtClientOutput::get_instance()
{
	if (alsa_bt_client_output == NULL)
	{
		alsa_bt_client_output = new(AlsaBtClientOutput);
	}

	return alsa_bt_client_output;
}

/**
 * Starts the BT client thread looking for new bt data
 */
void AlsaBtClientOutput::start_bt_alsa_out_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the BT thread is not allready running
	if (bt_alsa_out_thread_is_running)
	{
		return;
	}

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
		fprintf(stderr, "ALSA BT client: Unsuccessful in setting BT alsa out thread realtime prio\n");
	}
	// Set the control flag
	bt_alsa_out_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&bt_alsa_out_thread_id, &tattr, bt_alsa_out_thread, (void*)100);
	char thname[] = { "bt_alsa_out_thread" };
	pthread_setname_np(bt_alsa_out_thread_id, &thname[0]);
}

/**
 * Stop the BT client thread.
 */
void AlsaBtClientOutput::stop_bt_alsa_out_thread()
{
	// Reset the control flag to terminate the thread
	bt_alsa_out_thread_is_running = false;
}



/**
* BT alsa output thread.
*
* 1. Polls for a new data in the BR Rx alsa data queue every 5msc.
* 2. Sends new data through the alsa output
*/
void* AlsaBtClientOutput::bt_alsa_out_thread(void* thread_id)
{
	bool timeout = false, blocking = true, non_blocking = false;
	int count, last_count, status, timeouts = 0;
	uint8_t bytes[512];


	fprintf(stderr, "ALSA BT client: BT alsa out thread started\n");

	while (bt_alsa_out_thread_is_running)
	{
		// Wait for new data - non blocking
		bt_rx_alsa_channel_data = Raspi3BluetoothQ::bt_rx_queue_alsa.dequeue(0, &timeout, &non_blocking);
		// Get all new data
		count = 0;
		last_count = 0;
		while (bt_rx_alsa_channel_data != NULL)
		{
			// Collect all available data
			for (count; count < (last_count + bt_rx_alsa_channel_data->message->mssg_len) &&
				count < sizeof(bt_rx_alsa_channel_data->message->data) &&
				count < sizeof(bytes); count++)
			{
				bytes[count] = bt_rx_alsa_channel_data->message->data[count - last_count];
			}
			last_count = count;
			delete bt_rx_alsa_channel_data;
			bt_rx_alsa_channel_data = Raspi3BluetoothQ::bt_rx_queue_alsa.dequeue(0, &timeout, &non_blocking);
			//			printf("ALSA BT client: BT in data alsa thread while data\n");
		}

		if (count > 0)
		{
			// New data is available
//			printf("\nBT in data alsa thread: count: %i  data: ", count);
//			for (int i = 0; i < count; i++)
//				printf("%x ", bytes[i]);
//			printf("\n");

			if ((status = snd_rawmidi_write(midi_out, bytes, count)) < 0)
			{
				printf("ALSA BT client: Problem writing bt data to MIDI output: %s", snd_strerror(status));
				//					exit(1);
			}

		}

		usleep(1000);
	}

	return 0;
}