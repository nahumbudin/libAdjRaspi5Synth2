/**
*	@ file		jackAudioClients.h
*	@ author		Nahum Budin
*	@ date		2-Oct-2024
*	@ version	1.3 
*						1. Code refactoring and notaion.
*
*	@ brief		Handle JACK input and output audio streaming
*	
*	History:\n
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

#pragma once

#include <jack/jack.h>
#include <jack/transport.h>
#include <jack/midiport.h>

typedef float sample_t;

int initialize_jack_server_interface();

int intilize_jack_server_connection_out(const char* client_name, const char* server_name);
int jack_callback_process_out(jack_nframes_t nun_of_frames, void *arg);
void connect_jack_audio_ports_out();
void disconnect_jack_audio_ports_out();
int get_num_of_system_audio_output_ports();
bool jack_audio_is_connected_out();
void process_out(jack_nframes_t nun_of_frames);
void process_silence_out(jack_nframes_t nun_of_frames);

int intilize_jack_server_connection_in(const char* client_name, const char* server_name);
int jack_callback_process_in(jack_nframes_t nun_of_frames, void *arg);
void connect_jack_audio_ports_in();
void disconnect_jack_audio_ports_in();
int get_num_of_system_audio_input_ports();
bool jack_audio_is_connected_in();
void process_in(jack_nframes_t nun_of_frames);
void process_silence_in(jack_nframes_t nun_of_frames);


void jack_shutdown(void *arg);
void jack_exit();

int set_jack_mode(int mode);
int get_jack_mode();
int enable_jack_auto_start();
int disable_jack_auto_start();
bool get_jack_auto_start_state();
int enable_jack_auto_connect_audio();
int disable_jack_auto_connect_audio();
int enable_jack_auto_connect_midi();
int disable_jack_auto_connect_midi();
bool get_jack_auto_connect_audio_state();
bool get_jack_auto_connect_midi_state();

// see iln lib..._1
//int connectJackMidiPort(int portNum = 0);
//int disconnectJackMidiPort(int portNum);
//int getNumOfSystemMidiInputPorts();
//void assign_osciloscope_data_buffers(float *left_buf, float *right_buf);
