/**
*	@ file		jackAudioClients.h
*	@ author		Nahum Budin
*	@ date		27-Sep-2025
*	@ version	1.4 
*						1. Code refactoring and notaion.
*
*	@ brief		Handle JACK input and output audio streaming
*	
*	History:\n
*		2-Oct-2024 1.3 Code refactoring and notaion.
*		18-Jan-2021 1.2 
*						1. Code refactoring and notaion.
*						2. Adding jack setting mode manual: app sets JACK params; Auto: apps get params from JACK
*						3. Seperating auto connection of audio and midi
*		17-May-2020	1.1 
*						Define output client - output AdjSynth signal, and input client to enable signal\n
*						level and signal waveform display
*		11-Nov-2019 1.0
*						Revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October 18, 2017.
*		
*/

#include <pthread.h>
#include <math.h>
#include <jack/jack.h>

#include "../libAdjRaspi5Synth2.h"

#include "jackAudioClients.h"
#include "../Audio/audioManager.h"
#include "../Audio/audioCommons.h"
#include "../LibAPI/audio.h"

extern pthread_cond_t update_thread_cv;
extern pthread_mutex_t update_thread_mutex;

AudioManager *audio_manager = NULL;

jack_client_t *client_out = NULL, *client_in = NULL;
jack_port_t *output_port[_MAX_NUM_OF_JACK_AUDIO_IN_PORTS] = { NULL };
jack_port_t *input_port[_MAX_NUM_OF_JACK_AUDIO_IN_PORTS] = { NULL };

int transport_aware_out = 0, transport_aware_in = 0;
char *client_name_out = 0, *client_name_in = 0;
char *server_name_out = 0, *server_name_in = 0;
jack_status_t status_out, status_in;
jack_options_t options_out = (jack_options_t)(JackNullOption | JackServerName);
jack_options_t options_in = (jack_options_t)(JackNullOption | JackServerName);
int error;
char client_name_string_out_L[] = { "AdjHeartSynthL_out" };
char client_name_string_out_R[] = { "AdjHeartSynthR_out" };
char client_name_string_in_L[] = { "AdjHeartSynthL_in" };
char client_name_string_in_R[] = { "AdjHeartSynthR_in" };

//char clientNameStringMidiIn[_MAX_NUM_OF_MIDI_IN_PORTS][32]; // = { "AdjHeartSynthMidiIn" };

bool initiated_out = false, initiated_in = false;
const char **ports_audio_out, **ports_audio_in;
//const char **portsMidi;

int num_of_system_output_audio_ports = 0, num_of_system_input_audio_ports = 0;
//int numOfSystemInputMidiPorts = 0;
int num_of_connected_audio_output_ports = 0, num_of_connected_audio_input_ports = 0;
//int numOfConnectedMidiInputPorts = 0;

//int connectedMidiPortNum = 0;

int jack_mode = _DEFAULT_JACK_MODE;
int jack_sample_rate = _DEFAULT_SAMPLE_RATE;
int jack_block_size = _DEFAULT_BLOCK_SIZE;
bool jack_auto_start_mode = _DEFAULT_JACK_AUTO_START;
bool jack_auto_connect_audio_mode = _DEFAULT_JACK_AUTO_CONNECT_AUDIO;
bool jack_auto_connect_midi_mode = _DEFAULT_JACK_AUTO_CONNECT_MIDI;

// Osciloscope samples buffers NOTE MAX SIZE 
sample_t osc_left_signal[_AUDIO_MAX_BUF_SIZE];
sample_t osc_right_signal[_AUDIO_MAX_BUF_SIZE];

//char clientNameStringMidi[] = { "musicopenlabMidiIn" };

/**
*   @brief  Initialize the Jack server interface
*   @param  none		
*   @return 0 if OK
*/
int initialize_jack_server_interface()
{
	audio_manager = AudioManager::get_instance();
	jack_sample_rate = audio_manager->get_sample_rate();
	jack_block_size = audio_manager->get_audio_block_size();
	
	return 0;
}

/**
*   @brief  Initialize the Jack server output connection
*   @param  clientname		a pointer to a null terminated chars string holding the client name
*   @param  servername		a pointer to a null terminated chars string holding the server name
*   @return 0 if connection established, non-zero otherwise
*/
int intilize_jack_server_connection_out(const char* client_name, const char* server_name)
{
	initiated_out = false;

	if (client_out)
	{	
		jack_client_close(client_out);
	}
	
	client_name_out = (char *) malloc(32 * sizeof(char));
	strncpy(client_name_out, client_name, 31);
	server_name_out = (char *) malloc(32 * sizeof(char));
	strncpy(server_name_out, server_name, 31);
	// open a client connection to the JACK server
	options_out = (jack_options_t)(JackNullOption | JackNoStartServer);
	if ((client_out = jack_client_open(client_name_out, options_out, &status_out)) == 0) 
	{
		error = errno;
		fprintf(stderr, "jack server (output) not running?\n");
		callback_message_id(_MESSAGE_JACK_SERV_OUTPUT_NOT_RUNNING);
		sleep(3);
		return -11;
	}
	
	if (status_out & JackServerStarted) 
	{
		fprintf(stderr, "JACK server (output) started\n");
		callback_message_id(_MESSAGE_JACK_SERV_OUTPUT_NOT_RUNNING);
	}
	
	if (status_out & JackNameNotUnique) 
	{
		client_name_out = jack_get_client_name(client_out);
		error = errno;
		fprintf(stderr, "JACK: unique name `%s' assigned\n", client_name_out);
		sleep(3);
	}
	
	// tell the JACK server to call `jackCallbackProcess_out()' whenever there is work to be done.
	jack_set_process_callback(client_out, jack_callback_process_out, 0);
	// tell the JACK server to call `jack_shutdown()' if
	//   it ever shuts down, either entirely, or if it
	//   just decides to stop calling us.
	jack_on_shutdown(client_out, jack_shutdown, 0);
	
	// Look for 2 audio output ports
	output_port[_RIGHT] = jack_port_register(client_out, client_name_string_out_R, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_port[_LEFT] = jack_port_register(client_out, client_name_string_out_L, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	if ((output_port[_RIGHT] == NULL) || (output_port[_LEFT] == NULL))
	{
		fprintf(stderr, "JACK: no more available audio output ports\n");
		sleep(3);
		return -1;
	}
	
	num_of_system_output_audio_ports = 2;
	
	//	jack_sample_rate = jack_get_sample_rate(client_out);	
	
	//	jack_set_buffer_size(client_out, audio_manager->get_audio_block_size());	

	if (jack_mode == _JACK_MODE_SERVER_CONTROL) 
	{
		// Get params from jack server
		jack_sample_rate = jack_get_sample_rate(client_out);
		jack_block_size = jack_get_buffer_size(client_out);		
	}
	else
	{
		// set jack server params
		jack_sample_rate = audio_manager->get_sample_rate();
		if (jack_sample_rate != jack_get_sample_rate(client_out))
		{
			// TODO: set jack server sample rate using system call to QjackCtl and restart the server
			
		}
		
		jack_block_size = audio_manager->get_audio_block_size();
		jack_set_buffer_size(client_out, jack_block_size);
	}
		
	
	// We are ready
	if (jack_activate(client_out)) 
	{
		fprintf(stderr, "Jack: cannot activate out client");
		sleep(3);
		return -2;
	}

	if (jack_auto_connect_audio_mode == _JACK_AUTO_CONNECT_AUDIO_EN)
	{		
		// Connect the audio ports.  You can't do this before the client is
		// activated, because we can't make connections to clients
		// that aren't running.  Note the confusing (but necessary)
		// orientation of the driver backend ports: playback ports are
		// "input" to the backend, and capture ports are "output" from
		// it.
		ports_audio_out = jack_get_ports(client_out, NULL, NULL, JackPortIsPhysical | JackPortIsInput); // JackPortIsPhysical  isinput or isoutput
		if (ports_audio_out == NULL) 
		{
			fprintf(stderr, "Jack: no more playback ports\n");
			sleep(3);
			return -3;
		}
		// Connect Audio

		if (jack_connect(client_out, jack_port_name(output_port[_LEFT]), ports_audio_out[_LEFT])) 
		{
			fprintf(stderr, "cannot connect output ports\n");
		}
		
		if (jack_connect(client_out, jack_port_name(output_port[_RIGHT]), ports_audio_out[_RIGHT])) 
		{
			fprintf(stderr, "Jack: cannot connect output ports\n");
		}
		// Free the memory returned by jack_get_ports
//		free(ports_audio_out);
		
		initiated_out = true;
	}	
	
	return 0;
}

/**
*   @brief  Connect Jack output audio ports
*   @param  none
*   @return void
*/
void connect_jack_audio_ports_out() 
{
	if (initiated_out && client_out && output_port[_LEFT] && ports_audio_out && output_port[_RIGHT])
	{
		if (jack_connect(client_out, jack_port_name(output_port[_LEFT]), ports_audio_out[_LEFT])) 
		{
			fprintf(stderr, "Jack: cannot connect Audio output ports\n");
		}

		if (jack_connect(client_out, jack_port_name(output_port[_RIGHT]), ports_audio_out[_RIGHT])) 
		{
			fprintf(stderr, "Jack: cannot connect Audio output ports\n");
		}
		
		// Free the memory returned by jack_get_ports
		free(ports_audio_out);
	}
}

/**
*   @brief  Disconnect Jack output audio ports
*   @param  none
*   @return void
*/
void disconnect_jack_audio_ports_out()
{
	if (initiated_out && client_out && output_port[_LEFT] && *ports_audio_out && output_port[_RIGHT])
	{
		if (jack_disconnect(client_out, jack_port_name(output_port[_LEFT]), ports_audio_out[_LEFT])) 
		{
			fprintf(stderr, "Jack: cannot dissconnect Audio output ports\n");
		}

		if (jack_disconnect(client_out, jack_port_name(output_port[_RIGHT]), ports_audio_out[_RIGHT])) 
		{
			fprintf(stderr, "Jack: cannot dissconnect Audio output ports\n");
		}
	}
}

bool jack_audio_is_connected_out() { return initiated_out; }

int get_num_of_system_audio_output_ports() { return num_of_system_output_audio_ports;  }

/**
*   @brief  Jack output transfer callback
*   @param  nframes	number of frames
*   @param	arg		a pointer to a (void) srgument data (not in use)
*   @return 0 if connected; non-zero otherwise
*/
int jack_callback_process_out(jack_nframes_t num_of_frames, void *arg)
{
	//	fprintf(stderr, "Jack out callback frames: %i\n", nframes);
	int err, core;
	//	static volatile bool oneOfTwo = true;

	if (transport_aware_out) 
	{
		jack_position_t pos;
		if (jack_transport_query(client_out, &pos) != JackTransportRolling) 
		{
			process_silence_out(num_of_frames);
			return 0;
		}
	}

	if (num_of_frames != jack_block_size)
	{
		fprintf(stderr, "jack callback small buffer\n");
	}

	process_out(num_of_frames);
	

	// Triger update process	
	if (pthread_mutex_trylock(&update_thread_mutex) == 0)
	{
		// Signal update thread
		pthread_cond_signal(&update_thread_cv);
		pthread_mutex_unlock(&update_thread_mutex);
	}
	
	return 0;
}

/**
*   @brief  Process an output  silence audio blocks
*   @param  nframes	number of frames
*   @return void
*/
void process_silence_out(jack_nframes_t nun_of_frames) 
{
	sample_t *buffer_L = (sample_t *) jack_port_get_buffer(output_port[_LEFT], nun_of_frames);
	memset(buffer_L, 0, sizeof(jack_default_audio_sample_t) * nun_of_frames);
	
	sample_t *buffer_R = (sample_t *) jack_port_get_buffer(output_port[_RIGHT], nun_of_frames);
	memset(buffer_R, 0, sizeof(jack_default_audio_sample_t) * nun_of_frames);
}

/**
*   @brief  Process an output sound audio blocks
*   @param  nframes	number of frames
*   @return void
*/
void process_out(jack_nframes_t nun_of_frames) 
{
	volatile int i, voice;
	struct timeval start_ts;
	static long prev_ts_sec, prev_ts_usec;

		//static sample_t *prevBuffer_L = NULL, *prevBuffer_R = NULL;

	sample_t *buffer_L = (sample_t *) jack_port_get_buffer(output_port[_LEFT], nun_of_frames);	
	sample_t *buffer_R = (sample_t *) jack_port_get_buffer(output_port[_RIGHT], nun_of_frames);

	/*
		if (buffer_L == prevBuffer_L)
		{
			fprintf(stderr, "jack server duplicate L buffer\n");
		}

		if (buffer_R == prevBuffer_R)
		{
			fprintf(stderr, "jack server duplicate R buffer\n");
		}

		prevBuffer_R = buffer_R;
		prevBuffer_L = buffer_L;
		
	*/

	if (audio_manager != NULL)
	{
		for (i = 0; i < nun_of_frames /*_PERIOD_SIZE*/; i++) 
		{
			*(buffer_L + i) = (sample_t)audio_manager->audio_block_stereo_float_shared_memory_outputs->data[_LEFT][i]; // / 32767.0;
			*(buffer_R + i) = (sample_t)audio_manager->audio_block_stereo_float_shared_memory_outputs->data[_RIGHT][i]; // / 32767.0;
		}
	}

	/*
	gettimeofday(&start_ts, NULL);
	printf("jack out  %x %ld\n", (long)audio_manager->audio_block_stereo_float_shared_memory_outputs, 
		(long)((start_ts.tv_sec - prev_ts_sec) * 1000000L + (start_ts.tv_usec - prev_ts_usec))); // <<<<<
	prev_ts_sec = start_ts.tv_sec;
	prev_ts_usec = start_ts.tv_usec;
	*/
	
}

/**
*   @brief  Initialize the Jack server input connection
*   @param  clientname		a pointer to a null terminated chars string holding the client name
*   @param  servername		a pointer to a null terminated chars string holding the server name
*   @return 0 if connection established, non-zero otherwise
*/
int intilize_jack_server_connection_in(const char* client_name, const char* server_name)
{
	initiated_in = false;

	if (client_in)
	{	
		jack_client_close(client_in);
	}
	
	client_name_in = (char *) malloc(32 * sizeof(char));
	strncpy(client_name_in, client_name, 31);
	server_name_in = (char *) malloc(32 * sizeof(char));
	strncpy(server_name_in, server_name, 31);
	// open a client connection to the JACK server
	options_in = (jack_options_t)(JackNullOption | JackNoStartServer);
	if ((client_in = jack_client_open(client_name_in, options_in, &status_in)) == 0) 
	{
		error = errno;
		fprintf(stderr, "jack server (input) not running?\n");
		callback_message_id(_MESSAGE_JACK_SERV_INPUT_NOT_RUNNING);
		sleep(3);
		return -11;
	}
	
	if (status_in & JackServerStarted) 
	{
		fprintf(stderr, "JACK server (input) started\n");
		callback_message_id(_MESSAGE_JACK_SERV_INPUT_RUNNING);
	}
	
	if (status_in & JackNameNotUnique) 
	{
		client_name_in = jack_get_client_name(client_in);
		error = errno;
		fprintf(stderr, "unique name `%s' assigned\n", client_name_in);
		sleep(3);
	}
	
	// tell the JACK server to call `jack_callback_process_in()' whenever there is work to be done.
	jack_set_process_callback(client_in, jack_callback_process_in, 0);
	// tell the JACK server to call `jack_shutdown()' if
	//   it ever shuts down, either entirely, or if it
	//   just decides to stop calling us.
	jack_on_shutdown(client_in, jack_shutdown, 0);
	
	// Look for 2 audio output ports
	input_port[_RIGHT] = jack_port_register(client_in, client_name_string_in_R, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	input_port[_LEFT] = jack_port_register(client_in, client_name_string_in_L, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	if ((input_port[_RIGHT] == NULL) || (input_port[_LEFT] == NULL))
	{
		fprintf(stderr, "no more available audio input ports\n");
		sleep(3);
		return -1;
	}
	num_of_system_input_audio_ports = 2;

	
	if (jack_mode == _JACK_MODE_SERVER_CONTROL) 
	{
		// Get params from jack server
		jack_sample_rate = jack_get_sample_rate(client_out);
		jack_block_size = jack_get_buffer_size(client_out);		
	}
	else
	{
		// set jack server params
		jack_sample_rate = audio_manager->get_sample_rate();
		if (jack_sample_rate != jack_get_sample_rate(client_out))
		{
			// TODO: set jack server sample rate using system call to QjackCtl and restart the server
			
		}
		
		jack_block_size = audio_manager->get_audio_block_size();
		jack_set_buffer_size(client_out, jack_block_size);
	}
	
	// We are ready
	if (jack_activate(client_in)) 
	{
		fprintf(stderr, "Jack: cannot activate client");
		sleep(3);
		return -2;
	}

	initiated_in = true;
	
	return 0;
}

/**
*   @brief  Connect Jack input audio ports
*   @param  none
*   @return void
*/
void connect_jack_audio_ports_in() 
{
	if (initiated_in && client_in && input_port[_LEFT] && ports_audio_in && input_port[_RIGHT])
	{
		if (jack_connect(client_in, jack_port_name(input_port[_LEFT]), ports_audio_in[_LEFT])) 
		{
			fprintf(stderr, "Jack: cannot connect Audio input ports\n");
		}

		if (jack_connect(client_in, jack_port_name(input_port[_RIGHT]), ports_audio_in[_RIGHT])) 
		{
			fprintf(stderr, "Jack: cannot connect Audio input ports\n");
		}
	}
}

/**
*   @brief  Disconnect Jack input audio ports
*   @param  none
*   @return void
*/
void disconnect_jack_audio_ports_in()
{
	if (initiated_in && client_in && input_port[_LEFT] && *ports_audio_in && input_port[_RIGHT])
	{
		if (jack_disconnect(client_in, jack_port_name(input_port[_LEFT]), ports_audio_in[_LEFT])) 
		{
			fprintf(stderr, "Jack: cannot dissconnect Audio input ports\n");
		}

		if (jack_disconnect(client_in, jack_port_name(input_port[_RIGHT]), ports_audio_in[_RIGHT])) 
		{
			fprintf(stderr, "Jack: cannot dissconnect Audio input ports\n");
		}
	}
}

bool jack_audio_is_connected_in() { return initiated_in; }

int get_num_of_system_audio_input_ports() { return num_of_system_input_audio_ports;  }

/**
*   @brief  Jack input transfer callback
*   @param  nframes	number of frames
*   @param	arg		a pointer to a (void) srgument data (not in use)
*   @return 0 if connected; non-zero otherwise
*/
int jack_callback_process_in(jack_nframes_t num_of_frames, void *arg)
{
	//	fprintf(stderr, "Jack in callback frames: %i\n", nframes);

	process_in(num_of_frames);
	
	return 0;
}

/**
*   @brief  Process an intput  silence audio blocks
*   @param  nframes	number of frames
*   @return void
*/
void process_silence_in(jack_nframes_t num_of_frames) 
{
	//	sample_t *bufferL = (sample_t *) jack_port_get_buffer(inputPort[LEFT], nframes);
	//	memset(bufferL, 0, sizeof(jack_default_audio_sample_t) * nframes);
	
	//	sample_t *bufferR = (sample_t *) jack_port_get_buffer(outputPort[RIGHT], nframes);
	//	memset(bufferR, 0, sizeof(jack_default_audio_sample_t) * nframes);
}

/**
*   @brief  Process an input sound audio blocks
*   @param  nframes	number of frames
*   @return void
*/
void process_in(jack_nframes_t num_of_frames) 
{
	volatile int i;
	float enrg_L = 0.0f, enrg_R = 0.0f;
	static int count = 0;
	int level_L, level_R;
	
	sample_t *buffer_L = (sample_t *)jack_port_get_buffer(input_port[_LEFT], num_of_frames);
	sample_t *buffer_R = (sample_t *)jack_port_get_buffer(input_port[_RIGHT], num_of_frames);
	
#define _SKIP_ENRG		4	// Energy measuring skip factor
	
	for (i = 0; i < audio_manager->get_audio_block_size(); i++)
	{
		if (i % _SKIP_ENRG == 0)
		{
			enrg_L += (sample_t)buffer_L[i] * (sample_t)buffer_L[i];
			enrg_R += (sample_t)buffer_R[i] * (sample_t)buffer_R[i];
		}
		
		osc_left_signal[i] = (sample_t)buffer_L[i];
		osc_right_signal[i] = (sample_t)buffer_R[i];
	}
	
	count++;
	if (count >= 25)
	{
		if (enrg_L < 0)
		{	
			enrg_L = 0.0f;
		}
			
		if (enrg_R < 0)
		{	
			enrg_R = 0.0f;
		}
			
		level_L = (int)(log10(enrg_L + 1.0f) * 800); // TODO: calibrate
		level_R = (int)(log10(enrg_R + 1.0f) * 800);
	
		//callback_update_left_level(level_L);
		//callback_update_right_level(level_R);
		
		//callback_update_signal_display(osc_left_signal, osc_right_signal, audio_manager->get_audio_block_size());		
		
		count = 0;
	}
}


/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown(void *arg)
{
	error = errno;
	fprintf(stderr, "jack Shutdown. Err %s\n", snd_strerror(error));
	sleep(3);
	exit(1);
}

void jack_exit()
{
	jack_client_close(client_out);
	fprintf(stderr, "jack exit\n");
	sleep(2);
}

int set_jack_mode(int mode)
{
	if ((mode == _JACK_MODE_SERVER_CONTROL) || (mode == _JACK_MODE_APP_CONTROL))
	{
		jack_mode = mode;
		return jack_mode;
	}
	else
	{
		return -1;
	}
}

int get_jack_mode() { return jack_mode; }

int enable_jack_auto_start()
{
	jack_auto_start_mode = true;

	return 0;
}

int disable_jack_auto_start()
{
	jack_auto_start_mode = false;
	
	return 0;
}

bool get_jack_auto_start_state() { return jack_auto_start_mode; }

int enable_jack_auto_connect_audio()
{
	jack_auto_connect_audio_mode = true;
	
	return 0;
}

int disable_jack_auto_connect_audio()
{
	jack_auto_connect_audio_mode = false;
	
	return 0;
}

int enable_jack_auto_connect_midi()
{
	jack_auto_connect_midi_mode = true;
	
	return 0;
}

int disable_jack_auto_connect_midi()
{
	jack_auto_connect_midi_mode = false;
	
	return 0;
}

bool get_jack_auto_connect_audio_state() { return jack_auto_connect_audio_mode; }

bool get_jack_auto_connect_midi_state() { return jack_auto_connect_midi_mode; }
