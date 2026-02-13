/**
*	@file		alsaMidi.cpp
*	@author		Nahum Budin
*	@date		11-May-2024
*	@version	1.2
*					1. Updating includes
*
*	@brief		Scan for midi devices and handle input and output connections
*				(may be done instead of by Jack control pannel)
*				Recives data from a alsa midi device and push it into the midi-in rx data queue.
*				https://ccrma.stanford.edu/~craig/articles/linuxmidi/alsa-1.0/
*
*	History:\n
*
*		version	1.2		6-Feb-2021
*					1. Code refactoring and notaion.
*					2. Adding support in multiple devices
*					3. Seperating into input and output devices
*
*		version	1.1		6-Feb-2021
*					1. Code refactoring and notaion.
*					2. Adding support in multiple devices
*					3. Seperating into input and output devices
*
*		version 1.0		9-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Dec 2017)
*/

#include "alsaMidi.h"
#include "../MIDI/midiStream.h"

#include "../Misc/priorities.h"

#include "../LibAPI/libAdjRaspi5SynthAPI.h"
#include "../commonDefs.h"
#include "../utils/utils.h"

AlsaMidi* AlsaMidi::alsa_midi_instance = NULL;

/* True when midi in thread is running (in thread inputs data from output devices) */
bool AlsaMidi::midi_rx_thread_is_running[_MAX_NUM_OF_MIDI_CONNECTIONS];
/* True when midi out thread is running  (out thread outputs data to input devices) */
bool AlsaMidi::midi_tx_thread_is_running[_MAX_NUM_OF_MIDI_CONNECTIONS];
/* True when a  midi device is connected */
bool AlsaMidi::device_connected[_MAX_NUM_OF_MIDI_DEVICES];
// Holds device-number of each connection
int AlsaMidi::connection_device[_MAX_NUM_OF_MIDI_CONNECTIONS];

pthread_t AlsaMidi::th_midi_rx_thread[_MAX_NUM_OF_MIDI_CONNECTIONS];
pthread_t AlsaMidi::th_midi_tx_thread[_MAX_NUM_OF_MIDI_CONNECTIONS];

snd_rawmidi_t* AlsaMidi::midi_in[_MAX_NUM_OF_MIDI_DEVICES];
snd_rawmidi_t* AlsaMidi::midi_out[_MAX_NUM_OF_MIDI_DEVICES];

/* Queue for holding midi input data */
SafeQueue<alsa_data_t*> AlsaMidi::alsa_rx_queue[_MAX_NUM_OF_MIDI_CONNECTIONS];
/* Queue for holding midi output data */
SafeQueue<alsa_data_t*> AlsaMidi::alsa_tx_queue[_MAX_NUM_OF_MIDI_CONNECTIONS];

AlsaMidi* AlsaMidi::get_instance()
{
	if (alsa_midi_instance == NULL)
	{
		alsa_midi_instance = new AlsaMidi();
	}

	return alsa_midi_instance;
}

/**
*   @brief  Create a AlsaMidi object instance and scan for ports.
*   @param	none
*   @return none
*/
AlsaMidi::AlsaMidi()
{
	for (int con = 0; con < _MAX_NUM_OF_MIDI_CONNECTIONS; con++)
	{
		midi_rx_thread_is_running[con] = false;
		midi_tx_thread_is_running[con] = false;
		connection_device[con] = -1;
	}

	for (int dev = 0; dev < _MAX_NUM_OF_MIDI_DEVICES; dev++)
	{
		midi_in[dev] = nullptr;
		midi_out[dev] = nullptr;
		device_connected[dev] = false;
	}

	scan_midi_ports();
}

AlsaMidi::~AlsaMidi()
{
	for (int con = 0; con < _MAX_NUM_OF_MIDI_CONNECTIONS; con++)
	{
		midi_rx_thread_is_running[con] = false;
		midi_tx_thread_is_running[con] = false;
	}
}

/**
*   @brief  Connect to a midi device.
*   @param	dev		device number.
*   @param  in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT.
*   @return connection number if connected; -1 if params out of range;
*			-2 if no free connection is available
*/
int AlsaMidi::connect_device(int dev, int in_out)
{
	int status, connection;
	char portname[32];

	int mode = 0;

	if (((in_out == _MIDI_DEVICE_INPUT) && (dev >= num_of_input_devices)) ||
		((in_out == _MIDI_DEVICE_OUTPUT) && (dev >= num_of_output_devices)) ||
		((in_out != _MIDI_DEVICE_INPUT) && (in_out != _MIDI_DEVICE_OUTPUT)) ||
		(dev < 0))
	{
		return -1;
	}

	// Get a free connection
	connection = get_free_connection(dev);
	if (connection < 0)
	{
		// no free connection
		return -2;
	}

	// Connect new device
	if ((in_out == _MIDI_DEVICE_INPUT) && (devices[dev].input == 1))
	{
		/*
				sprintf(portname,
					"hw:%d,%d,%d",
					devices[dev].card,
					devices[dev].device,
					devices[dev].subs);

				if ((status = snd_rawmidi_open(&midi_in[dev], NULL, portname, mode)) < 0)
				{
					fprintf(stderr, "Problem opening MIDI output: %s\n", snd_strerror(status));
					return -1;
				}

				device_connected[dev] = true;

				start_midi_thread(connection, dev, _MIDI_DEVICE_INPUT);
		*/
	}
	else
	{
		if (devices[dev].output == 1)
		{
			sprintf(portname,
				"hw:%d,%d,%d",
				devices[dev].card,
				devices[dev].device,
				devices[dev].subs);

			if ((status = snd_rawmidi_open(&midi_out[dev], NULL, portname, mode)) < 0)
			{
				fprintf(stderr, "Problem opening MIDI input: %s\n", snd_strerror(status));
				release_connection(connection);
				return -1;
			}

			device_connected[dev] = true;

			start_midi_thread(connection, dev, _MIDI_DEVICE_OUTPUT);
		}
	}

	return 0;
}

/**
*   @brief  Disconnect a midi device.
*   @param	dev		device number.
*   @param  in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT.
*   @return 0 if disconnected; -1 if params out of range
*			-2 if device is not connected
*			-3 if device connection was not found
*/
int AlsaMidi::disconnect_device(int dev, int in_out)
{
	int connection;

	if (((in_out == _MIDI_DEVICE_INPUT) && (dev >= num_of_input_devices)) ||
		((in_out == _MIDI_DEVICE_OUTPUT) && (dev >= num_of_output_devices)) ||
		((in_out != _MIDI_DEVICE_INPUT) && (in_out != _MIDI_DEVICE_OUTPUT)) ||
		(dev < 0))
	{
		return -1;
	}
	else
	{
		// Get device connection id
		connection = get_device_connection_num(dev);
		if (connection < 0)
		{
			//connection not found
			return -3;
		}
		else if (device_connected[dev] == false)
		{
			// Device is not connected
			return -2;
		}

		if (in_out == _MIDI_DEVICE_INPUT)
		{
			stop_midi_thread(connection, dev, _MIDI_DEVICE_INPUT);
			if (midi_in[dev])
			{
				snd_rawmidi_close(midi_in[dev]);
				midi_in[dev] = NULL; 				// snd_rawmidi_close() does not clear invalid pointer,
			}

			device_connected[dev] = false;
			release_connection(connection);
		}
		else
		{
			stop_midi_thread(connection, dev, _MIDI_DEVICE_OUTPUT);
			if (midi_out[dev])
			{
				snd_rawmidi_close(midi_out[dev]);
				midi_out[dev] = NULL; 				// snd_rawmidi_close() does not clear invalid pointer,
			}

			device_connected[dev] = false;
			release_connection(connection);
		}
	}

	return 0;
}

/**
*   @brief  Return the connection state.
*   @param	dev		device number.
*   @return true if connected false otherwise
*/
bool AlsaMidi::is_device_connected(int dev)
{
	if ((dev < 0) || (dev >= num_of_devices))
	{
		return false;
	}

	return device_connected[dev];
}

/**
*   @brief  scan_midi_ports - go through the list of available "soundcards"
*			checking them to see if there are devices/subdevices on them  sort them
			based on which can read/write MIDI data.
*   @param	print	if true, scan results will be printed
*   @return 0 if OK; -1 if fault
*/
int AlsaMidi::scan_midi_ports(bool print)
{
	int status;
	int card = -1;   // use -1 to prime the pump of iterating through card list
	num_of_devices = 0;
	num_of_input_devices = 0;
	num_of_output_devices = 0;

	if ((status = snd_card_next(&card)) < 0)
	{
		fprintf(stderr, "cannot determine card number: %s\n", snd_strerror(status));
		return -1;
	}
	if (card < 0)
	{
		fprintf(stderr, "no sound cards found\n");
		return -1;
	}

	if (print)
	{
		printf("\nDir Device    Name\n");
		fprintf(stderr, "====================================\n");
	}
	while (card >= 0)
	{
		list_midi_devices_on_card(card, false);
		if ((status = snd_card_next(&card)) < 0)
		{
			fprintf(stderr, "cannot determine card number: %s\n", snd_strerror(status));
			break;
		}
	}

	if (print)
	{
		fprintf(stderr, "\n");
	}

	std::string data_i, data_o;

	return 0;
}


/**
*   @brief  print_card_list - go through the list of available "soundcards"
*			in the ALSA system while printing their associated numbers and names.
*			Cards may or may not have any MIDI ports available on them (for
*			example, a card might only have an audio interface).
*   @param	void
*   @return void
*/
void AlsaMidi::print_card_list(void)
{
	int status;
	int card = -1;   // use -1 to prime the pump of iterating through card list
	char* long_name = NULL;
	char* short_name = NULL;

	if ((status = snd_card_next(&card)) < 0)
	{
		fprintf(stderr, "cannot determine card number: %s\n", snd_strerror(status));
		return;
	}
	if (card < 0)
	{
		fprintf(stderr, "no sound cards found.\n");
		return;
	}
	while (card >= 0)
	{
		printf("Card %d:", card);
		if ((status = snd_card_get_name(card, &short_name)) < 0)
		{
			fprintf(stderr, "cannot determine card shortname: %s\n", snd_strerror(status));
			break;
		}
		if ((status = snd_card_get_longname(card, &long_name)) < 0)
		{
			fprintf(stderr, "cannot determine card longname: %s\n", snd_strerror(status));
			break;
		}
		fprintf(stderr, "\tLONG NAME:  %s\n", long_name);
		fprintf(stderr, "\tSHORT NAME: %s\n", short_name);
		if ((status = snd_card_next(&card)) < 0)
		{
			fprintf(stderr, "cannot determine card number: %s\n", snd_strerror(status));
			break;
		}
	}
}

/**
*   @brief  list_midi_devices_on_card - For a particular "card" look at all
*			of the "devices/subdevices" on it and print information about it
*			if it can handle MIDI input and/or output.
*   @param	card	card number.
*	@param	print	if true, scan results will be printed
*   @return void
*/
void AlsaMidi::list_midi_devices_on_card(int card, bool print)
{
	snd_ctl_t* ctl;
	char name[32];
	int device = -1;
	int status;

	sprintf(name, "hw:%d", card);
	if ((status = snd_ctl_open(&ctl, name, 0)) < 0)
	{
		fprintf(stderr, "cannot open control for card %d: %s\n", card, snd_strerror(status));
		return;
	}
	do
	{
		status = snd_ctl_rawmidi_next_device(ctl, &device);
		if (status < 0)
		{
			fprintf(stderr, "cannot determine device number: %s\n", snd_strerror(status));
			break;
		}
		if (device >= 0)
		{
			list_subdevice_info(ctl, card, device, print);
		}
	} while (device >= 0);

	snd_ctl_close(ctl);
}

/**
*   @brief  list_subdevice_info - Collect and print information about a subdevice
*			of a device of a card if it can handle MIDI input and/or output.
*   @param	ctl		a pointer to a snd_ctl_t structure
*	@param	card	card number
*   @param  in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT.
*	@param	print	if true, scan results will be printed
*   @return void
*/
void AlsaMidi::list_subdevice_info(snd_ctl_t* ctl, int card, int device, bool print)
{
	snd_rawmidi_info_t* info;
	const char* name;
	const char* sub_name;
	int subs, subs_in, subs_out;
	int sub, in, out;
	int status;

	snd_rawmidi_info_alloca(&info);
	snd_rawmidi_info_set_device(info, device);

	snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_INPUT);
	snd_ctl_rawmidi_info(ctl, info);
	subs_in = snd_rawmidi_info_get_subdevices_count(info);
	snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_OUTPUT);
	snd_ctl_rawmidi_info(ctl, info);
	subs_out = snd_rawmidi_info_get_subdevices_count(info);
	subs = subs_in > subs_out ? subs_in : subs_out;

	sub = 0;
	in = out = 0;
	if ((status = is_output(ctl, card, device, sub)) < 0)
	{
		fprintf(stderr,
			"cannot get rawmidi information %d:%d: %s\n",
			card,
			device,
			snd_strerror(status));
		return;
	}
	else if (status)
	{
		out = 1;
	}

	if (status == 0)
	{
		if ((status = is_input(ctl, card, device, sub)) < 0)
		{
			fprintf(stderr,
				"cannot get rawmidi information %d:%d: %s\n",
				card,
				device,
				snd_strerror(status));
			return;
		}
		else if (status)
		{
			in = 1;
		}
	}
	//	else if (status) 
	//	{	
	//		in = 1;
	//	}

	if (status == 0)
	{
		return;
	}

	name = snd_rawmidi_info_get_name(info);
	sub_name = snd_rawmidi_info_get_subdevice_name(info);
	if (sub_name[0] == '\0')
	{
		if (subs == 1)
		{
			if (print)
			{
				printf(
					"%c%c  hw:%d,%d    %s\n",
					in ? 'I' : ' ',
					out ? 'O' : ' ',
					card,
					device,
					name);
			}

			devices[num_of_devices].input = in;
			devices[num_of_devices].output = out;
			devices[num_of_devices].card = card;
			devices[num_of_devices].device = device;
			strcpy(devices[num_of_devices++].name, name);

			if (out)
			{
				num_of_output_devices++;
			}
			else if (in)
			{
				num_of_input_devices++;
			}
		}
		else
		{
			if (print)
			{
				printf("%c%c  hw:%d,%d    %s (%d subdevices)\n",
					in ? 'I' : ' ',
					out ? 'O' : ' ',
					card,
					device,
					name,
					subs);
			}

			devices[num_of_devices].input = in;
			devices[num_of_devices].output = out;
			devices[num_of_devices].card = card;
			devices[num_of_devices].device = device;
			devices[num_of_devices].subs = subs;
			strcpy(devices[num_of_devices++].name, name);

			if (out)
			{
				num_of_output_devices++;
			}
			else if (in)
			{
				num_of_input_devices++;
			}
		}
	}
	else {
		sub = 0;
		for (;;)
		{
			if (print)
			{
				printf("%c%c  hw:%d,%d,%d  %s\n",
					in ? 'I' : ' ',
					out ? 'O' : ' ',
					card,
					device,
					sub,
					sub_name);
			}

			devices[num_of_devices].input = in;
			devices[num_of_devices].output = out;
			devices[num_of_devices].card = card;
			devices[num_of_devices].device = device;
			devices[num_of_devices].subs = sub;
			strcpy(devices[num_of_devices++].sub_name, sub_name);

			if (out)
			{
				num_of_output_devices++;
			}
			else if (in)
			{
				num_of_input_devices++;
			}

			if (++sub >= subs)
			{
				break;
			}

			in = is_input(ctl, card, device, sub);
			out = is_output(ctl, card, device, sub);
			snd_rawmidi_info_set_subdevice(info, sub);
			if (out)
			{
				snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_OUTPUT);
				if ((status = snd_ctl_rawmidi_info(ctl, info)) < 0)
				{
					fprintf(stderr, "cannot get rawmidi information %d:%d:%d: %s\n",
						card, device, sub, snd_strerror(status));
					num_of_output_devices--;
					num_of_devices--;
					break;
				}
			}
			else {
				snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_INPUT);
				if ((status = snd_ctl_rawmidi_info(ctl, info)) < 0)
				{
					fprintf(stderr, "cannot get rawmidi information %d:%d:%d: %s\n",
						card,
						device,
						sub,
						snd_strerror(status));
					num_of_input_devices--;
					num_of_devices--;
					break;
				}
			}
			sub_name = snd_rawmidi_info_get_subdevice_name(info);
		}
	}
}

/**
*   @brief  Returns if specified card/device/sub can input MIDI data.
*   @param	ctl		a pointer to a snd_ctl_t struct
*   @param	card	card number
*   @param	device	device number
*   @param	sub		sbdevice number
*   @return 1 if input 0 otherwise; < 0 on error
*/
int AlsaMidi::is_input(snd_ctl_t* ctl, int card, int device, int sub)
{
	snd_rawmidi_info_t* info;
	int status;

	snd_rawmidi_info_alloca(&info);
	snd_rawmidi_info_set_device(info, device);
	snd_rawmidi_info_set_subdevice(info, sub);
	snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_INPUT);

	if ((status = snd_ctl_rawmidi_info(ctl, info)) < 0 && status != -ENXIO)
	{
		return status;
	}
	else if (status == 0)
	{
		return 1;
	}

	return 0;
}

/**
*   @brief  Returns if specified card/device/sub can output MIDI data.
*   @param	ctl		a pointer to a snd_ctl_t struct
*   @param	card	card number
*   @param	device	device number
*   @param	sub		sbdevice number
*   @return 1 if output 0 otherwise
*/
/**
 is_output -- returns true if specified card/device/sub can output MIDI data.
*/
int AlsaMidi::is_output(snd_ctl_t* ctl, int card, int device, int sub)
{
	snd_rawmidi_info_t* info;
	int status;

	snd_rawmidi_info_alloca(&info);
	snd_rawmidi_info_set_device(info, device);
	snd_rawmidi_info_set_subdevice(info, sub);
	snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_OUTPUT);

	if ((status = snd_ctl_rawmidi_info(ctl, info)) < 0 && status != -ENXIO)
	{
		return status;
	}
	else if (status == 0)
	{
		return 1;
	}

	return 0;
}

/**
*   @brief  Returns an unused connection id and assigned it to device.
*   @param	device	device number
*   @return Free connection id; -1 if param out of range; -2 if no free connection is available
*/
int AlsaMidi::get_free_connection(int device)
{
	int connection = 0;

	if ((device < 0) || (device >= _MAX_NUM_OF_MIDI_DEVICES))
	{
		return -1;
	}

	while ((connection < _MAX_NUM_OF_MIDI_CONNECTIONS) &&
		(connection_device[connection] != -1))	// -1 indicates a free connection
	{
		connection++;
	}

	if (connection >= _MAX_NUM_OF_MIDI_CONNECTIONS)
	{
		// no free connection
		return -2;
	}
	else
	{
		connection_device[connection] = device;
	}

	return connection;
}

/**
*   @brief  Free a used connection id
*   @param	connection	connection number
*   @return 0 if ok; -1 if param out of range; -2 if connection was not in use
*/
int AlsaMidi::release_connection(int connection)
{
	if ((connection < 0) || (connection >= _MAX_NUM_OF_MIDI_CONNECTIONS))
	{
		return -1;
	}
	else if (connection_device[connection] == -1)
	{
		// already not in use
		return -2;
	}
	else
	{
		connection_device[connection] = -1;
		return 0;
	}
}

/**
*   @brief  Returns the device number assigned to a connection.
*   @param	connection	connection number
*   @return device number; -1 if params out of range; -2 if connection is not assigned
*/
int AlsaMidi::get_connection_device_num(int connection)
{
	if ((connection < 0) || (connection >= _MAX_NUM_OF_MIDI_CONNECTIONS))
	{
		return -1;
	}
	else
	{
		return connection_device[connection];
	}
}

/**
*   @brief  Returns the connection number assigned to a device.
*   @param	device	device number
*   @return connection number; -1 if params out of range;
*			-2 if device is not assigned with a connection
*/
int AlsaMidi::get_device_connection_num(int device)
{
	int connection = 0;

	if ((device < 0) || (device >= _MAX_NUM_OF_MIDI_DEVICES))
	{
		return -1;
	}
	else
	{
		while ((connection < _MAX_NUM_OF_MIDI_DEVICES) && connection_device[connection] != device)
		{
			connection++;
		}

		if (connection >= _MAX_NUM_OF_MIDI_DEVICES)
		{
			// not found
			return -2;
		}
		else
		{
			return connection;
		}
	}
}


/**
*   @brief  Start the midi thread.
*   @param	connection	connection number
*   @param  device number
*   @param  in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT.
*   @return void
*/
void AlsaMidi::start_midi_thread(int connection, int device, int in_out)
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	char thread_name[64];
	alsa_midi_thread_params_t thread_params;

	if ((connection < 0) || (connection >= _MAX_NUM_OF_MIDI_CONNECTIONS) ||
		((in_out != _MIDI_DEVICE_INPUT) && (in_out != _MIDI_DEVICE_OUTPUT)) ||
		(device < 0) || (device >= _MAX_NUM_OF_MIDI_DEVICES))
	{
		return;
	}

	// TODO: check if thread is already running

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	if (in_out == _MIDI_DEVICE_INPUT)
	{
		params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_IN;
	}
	else
	{
		params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_CTLB;
	}
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// setting the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	err = errno;
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		// Print the error
		if (in_out == _MIDI_DEVICE_INPUT)
		{
			fprintf(stderr, "Unsuccessful in setting Midi Out thread realtime prio\n");
		}
		else
		{
			fprintf(stderr, "Unsuccessful in setting Midi In thread realtime prio\n");
		}
	}

	if (in_out == _MIDI_DEVICE_INPUT)
	{
		// Input device is connected to tx (output) thread
		midi_tx_thread_is_running[connection] = true;
		thread_params.connection = connection;
		thread_params.device = device;
		thread_params.midi_in_out = midi_in[device];
		ret = pthread_create(&AlsaMidi::th_midi_tx_thread[connection], &tattr, midi_tx_thread, &thread_params);
		if (ret == -1)
		{
			fprintf(stderr, "Unable to create MIDI tx (output) thread.");
			exit(1);
		}

		sprintf(thread_name, "%s_%i\0", "midi_tx_thread", connection);
		pthread_setname_np(AlsaMidi::th_midi_tx_thread[connection], thread_name);
	}
	else
	{
		// Output device is connected to rx (input) thread
		midi_rx_thread_is_running[connection] = true;
		thread_params.connection = connection;
		thread_params.device = device;
		thread_params.midi_in_out = midi_out[device];
		ret = pthread_create(&AlsaMidi::th_midi_rx_thread[connection], &tattr, midi_rx_thread, &thread_params);
		if (ret == -1)
		{
			fprintf(stderr, "Unable to create MIDI rx (input) thread.");
			exit(1);
		}

		sprintf(thread_name, "%s_%i\0", "midi_rx_thread", connection);
		pthread_setname_np(AlsaMidi::th_midi_rx_thread[connection], "thread_name");
	}
}

/**
*   @brief  Stop the midi in thread.
*   @param  device number
*   @param  in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT.
*   @return void
*/
void AlsaMidi::stop_midi_thread(int connection, int device, int in_out)
{
	if ((connection < 0) || (connection >= _MAX_NUM_OF_MIDI_CONNECTIONS) ||
		(device < 0) || (device >= _MAX_NUM_OF_MIDI_DEVICES) ||
		((in_out != _MIDI_DEVICE_INPUT) && (in_out != _MIDI_DEVICE_OUTPUT)))
	{
		return;
	}

	if (in_out == _MIDI_DEVICE_INPUT)
	{
		// Input device is connected to output thread
		midi_tx_thread_is_running[connection] = false;
	}
	else
	{
		// Output device is connected to input thread
		midi_rx_thread_is_running[connection] = false;
	}
}

/**
*   @brief  Return the number of detected in/out midi devices.
*   @param  in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT.
*   @return the number of detected midi devices. -1 if error
*/
int AlsaMidi::get_num_of_midi_devices(int in_out)
{
	if (in_out == _MIDI_DEVICE_INPUT)
	{
		return num_of_input_devices;
	}
	else if (in_out == _MIDI_DEVICE_OUTPUT)
	{
		return num_of_output_devices;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Return the total number of detected midi devices (input + output).
*   @param  in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT.
*   @return the number of detected midi devices. -1 if error
*/
int AlsaMidi::get_total_num_of_midi_devices()
{
	return num_of_devices;
}

/**
*   @brief  Return true if a midi device is an input device.
*   @param	devid	device number
*   @return true if device is an input device, false otherwise.
*/
bool AlsaMidi::is_midi_device_input(int devid)
{
	bool res = false;

	if ((num_of_devices > 0) && (devid >= 0) && (devid < num_of_devices))
	{
		if (devices[devid].input == 1)
		{
			res = true;
		}
		else
		{
			res = false;
		}
	}

	return res;
}

/**
*   @brief  Return true if a midi device is an output device.
*   @param	devid	device number
*   @return true if device is an output device, false otherwise.
*/
bool AlsaMidi::is_midi_device_output(int devid)
{
	bool res = false;

	if ((num_of_devices > 0) && (devid >= 0) && (devid < num_of_devices))
	{
		if (devices[devid].output == 1)
		{
			res = true;
		}
		else
		{
			res = false;
		}
	}

	return res;
}

/**
*   @brief  Return true if a midi device is an output device.
*   @param	devid	device number
*   @return true if device is an output device, false otherwise.
*/
int AlsaMidi::get_midi_device_card_num(int dev_id)
{
	if ((num_of_devices > 0) && (dev_id >= 0) && (dev_id < num_of_devices))
	{
		return devices[dev_id].card;
	}
	else
		return false;
}

/**
*   @brief  Return the midi device number.
*   @param	devid	device id
*   @return device number.
*/
int AlsaMidi::get_midi_device_device_num(int dev_id)
{
	if ((num_of_devices > 0) && (dev_id >= 0) && (dev_id < num_of_devices))
	{
		return devices[dev_id].device;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Return the midi sub-device number.
*   @param	devid	device id
*   @return sub-device number.
*/
int AlsaMidi::get_midi_device_sub_dev_num(int dev_id)
{
	if ((num_of_devices > 0) && (dev_id >= 0) && (dev_id < num_of_devices))
	{
		return devices[dev_id].subs;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Return the midi device name.
*   @param	dev_id	device id
*   @parm	name	a pointer to a null terminated char string
*					that will hold the returned name
*   @return void.
*/
void AlsaMidi::get_midi_device_name(int dev_id, char* name)
{
	if ((num_of_devices > 0) && (dev_id >= 0) && (dev_id < num_of_devices))
	{
		name = &devices[dev_id].name[0];
	}
}

/**
*   @brief  Return the midi device sub-name.
*   @param	dev_id	device id
*   @param	sub_name	a pointer to a null terminated char string
*					that will hold the returned sub-name
*   @return void.
*/
void AlsaMidi::get_midi_device_sub_name(int dev_id, char* sub_name)
{
	if ((num_of_devices > 0) && (dev_id >= 0) && (dev_id < num_of_devices))
	{
		sub_name = &devices[dev_id].sub_name[0];
	}
}

/**
*   @brief  Return the midi device details string.
*   @param	dev_id	device id
*   @param	dev_str	a pointer to a null terminated char string
*					that will hold the returned string
*   @return void.
*/
void AlsaMidi::get_midi_device_string(int dev_id, char* dev_str)
{
	if ((num_of_devices > 0) && (dev_id >= 0) && (dev_id < num_of_devices))
	{
		if (devices[dev_id].sub_name[0] == '\0')
		{
			if (devices[dev_id].subs == 1)
			{
				sprintf(
					dev_str,
					"%c%c  hw:%d,%d    %s",
					devices[dev_id].input ? 'I' : ' ',
					devices[dev_id].output ? 'O' : ' ',
					devices[dev_id].card,
					devices[dev_id].device,
					devices[dev_id].name);
			}
			else
			{
				sprintf(
					dev_str,
					"%c%c  hw:%d,%d    %s (%d subdevices)",
					devices[dev_id].input ? 'I' : ' ',
					devices[dev_id].output ? 'O' : ' ',
					devices[dev_id].card,
					devices[dev_id].device,
					devices[dev_id].name,
					devices[dev_id].subs);
			}
		}
		else
		{
			sprintf(
				dev_str,
				"%c%c  hw:%d,%d,%d  %s",
				devices[dev_id].input ? 'I' : ' ',
				devices[dev_id].output ? 'O' : ' ',
				devices[dev_id].card,
				devices[dev_id].device,
				devices[dev_id].subs,
				devices[dev_id].sub_name);

		}
	}
	else if (dev_id == num_of_devices)
	{
		// Bluetooth
		sprintf(dev_str, "Rescan Midi Devices");
	}
}

/**
*   @brief  Midi input thread.
*   @param	arg		a pointer to a alsa_midi_thread_params_t struct.
*   @return void.
*/
void* midi_rx_thread(void* arg)
{
	if (arg == NULL)
	{
		return NULL;
	}

	// these are the parameters passed via last argument of pthread_create():
	alsa_midi_thread_params_t* thread_params = (alsa_midi_thread_params_t*)arg;
	snd_rawmidi_t* midi_out = thread_params->midi_in_out; // out device
	int connection = thread_params->connection;
	int device = thread_params->device;
	int bytes_read = -1;
	int err;

	alsa_data_t* rx_data;

	if (midi_out == NULL)
	{
		AlsaMidi::midi_rx_thread_is_running[connection] = false;
	}

	while (AlsaMidi::midi_rx_thread_is_running[connection])
	{
		rx_data = new alsa_data_t();
		if ((bytes_read = snd_rawmidi_read(midi_out, rx_data->data, sizeof(rx_data->data))) < 0)
		{
			err = errno;
			fprintf(stderr, "ALSA MIDI rx thread %i: Problem reading MIDI input: %s\n", connection, snd_strerror(bytes_read));
			//	AlsaMidi::midi_rx_thread_is_running[connection] = false;   // Exit
		}

		if ((bytes_read >= 0) && AlsaMidi::device_connected[device])
		{
			printf("ALSA MIDI in thread %i: alsa in bytes: %i\n", connection, bytes_read);
			rx_data->length = bytes_read;
			// Push data into the midi in receive data queue
			AlsaMidi::alsa_rx_queue[connection].enqueue(rx_data);
			for (int i = 0; i < bytes_read; i++)
			{
				printf("%x ", rx_data->data[i]);
			}
			printf("\n");
		}
		else
		{
			delete rx_data;
		}

	}

	AlsaMidi::get_instance()->release_connection(connection);
	return NULL;
}

/**
*   @brief  Midi output thread.
*   @param	arg		a pointer to a alsa_midi_thread_params_t struct.
*   @return void.
*/
void* midi_tx_thread(void* arg)
{
	if (arg == NULL)
	{
		return NULL;
	}

	// these are the parameters passed via last argument of pthread_create():
	alsa_midi_thread_params_t* thread_params = (alsa_midi_thread_params_t*)arg;
	snd_rawmidi_t* midi_in = thread_params->midi_in_out;  // Input device
	int connection = thread_params->connection;
	int device = thread_params->device;
	int bytes_read = -1;
	int err;

	alsa_data_t* tx_data;

	while (AlsaMidi::midi_tx_thread_is_running[connection])
	{
		usleep(10000); // TODO

	}

	return NULL;
}



