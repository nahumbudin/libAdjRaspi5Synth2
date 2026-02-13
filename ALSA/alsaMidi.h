/**
*	@file		alsaMidi.h
*	@author		Nahum Budin
*	@date		11-May-2024
*	@version	1.2
*					1. Updating includes
*
*	@brief		Scan for midi devices and handle input and output connections
*				(may be done instead of by Jack control pannel)
*				Recives data from a alsa midi device and push it into the midi in rx data queue.
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

#pragma once

#include <cstdint>
#include <pthread.h>

#include <./alsa/asoundlib.h>

#include "../commonDefs.h"

#include "../libAdjRaspi5Synth2.h"


#include "../MIDI/midiStream.h"
#include "../utils/safeQueues.h"

/* Defines the number of bytes in received buffer */
#define _NUM_OF_RX_BYTES		256
#define _MAX_NUM_OF_DEVICES		_MAX_NUM_OF_MIDI_DEVICES

/* A structure used for receiving data from connected clients */
typedef struct alsa_midi_rx_data
{
	alsa_midi_rx_data() :
		new_data(false), mssg_len(0) {}

	bool new_data;
	int mssg_len;
	char rx_data[_NUM_OF_RX_BYTES];
} alsa_midi_rx_data_t;

/* A structure used for holding connected clients data */
typedef struct midi_dev
{
	midi_dev() :
		input(0), output(0), card(-1), device(-1), subs(-1) {}

	int input;
	int output;
	int card;
	int device;
	int subs;
	char name[32];
	char sub_name[32];

} mididev_t;

typedef struct alsa_data
{
	alsa_data() : length(0) { for (int i = 0; i < _RAWDATA_MSSG_MAX_LEN; i++) data[i] = 0; }

	int length;
	uint8_t data[_RAWDATA_MSSG_MAX_LEN];
} alsa_data_t;

typedef struct alsa_midi_thread_params
{
	int connection;
	int device;
	snd_rawmidi_t* midi_in_out;
} alsa_midi_thread_params_t;

class AlsaMidi
{
public:
	static AlsaMidi* get_instance();

	~AlsaMidi();

	int connect_device(int dev, int in_out);
	int disconnect_device(int dev, int in_out); // (const char* port_name
	bool is_device_connected(int dev);

	int get_num_of_midi_devices(int in_out);
	int get_total_num_of_midi_devices();
	bool is_midi_device_input(int dev_id);
	bool is_midi_device_output(int dev_id);
	int get_midi_device_card_num(int dev_id);
	int get_midi_device_device_num(int dev_id);
	int get_midi_device_sub_dev_num(int dev_id);
	void get_midi_device_name(int dev_id, char* name);
	void get_midi_device_sub_name(int dev_id, char* sub_name);
	void get_midi_device_string(int dev_id, char* dev_str);

	int get_free_connection(int device);
	int release_connection(int connection);

	//	int get_connected_device(int in_out);

	int  scan_midi_ports(bool print = false);

	static bool midi_rx_thread_is_running[_MAX_NUM_OF_MIDI_CONNECTIONS];
	static bool midi_tx_thread_is_running[_MAX_NUM_OF_MIDI_CONNECTIONS];
	static pthread_t th_midi_rx_thread[_MAX_NUM_OF_MIDI_CONNECTIONS];
	static pthread_t th_midi_tx_thread[_MAX_NUM_OF_MIDI_CONNECTIONS];
	static bool device_connected[_MAX_NUM_OF_MIDI_DEVICES];

	static snd_rawmidi_t* midi_in[_MAX_NUM_OF_MIDI_DEVICES], * midi_out[_MAX_NUM_OF_MIDI_DEVICES];

	static SafeQueue<alsa_data_t*> alsa_rx_queue[_MAX_NUM_OF_MIDI_CONNECTIONS];
	static SafeQueue<alsa_data_t*> alsa_tx_queue[_MAX_NUM_OF_MIDI_CONNECTIONS];

private:
	AlsaMidi();
	void  print_card_list();
	void  list_midi_devices_on_card(int card, bool print = false);
	void  list_subdevice_info(snd_ctl_t* ctl, int card, int device, bool print = false);
	int is_input(snd_ctl_t* ctl, int card, int device, int sub);
	int is_output(snd_ctl_t* ctl, int card, int device, int sub);

	int get_connection_device_num(int connection);
	int get_device_connection_num(int device);

	void start_midi_thread(int connection, int device, int in_out);
	void stop_midi_thread(int connection, int device, int in_out);

	mididev_t devices[_MAX_NUM_OF_DEVICES];
	// Holds the device number of each connection; -1 if connection is disconnected
	static int connection_device[_MAX_NUM_OF_MIDI_CONNECTIONS];

	int num_of_devices, num_of_input_devices = 0, num_of_output_devices = 0;

	static AlsaMidi* alsa_midi_instance;
};

// Threads for inputing data from midi output device
void* midi_rx_thread(void* arg);
void* midi_tx_thread(void* arg);
