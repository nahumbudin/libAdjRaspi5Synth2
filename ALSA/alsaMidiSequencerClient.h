/**
*	@file		alsaMidiSequencerClient.h
*	@author		Nahum Budin
*	@date		12-May-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*					2. Adding Rx Q as a parameter
*
*	@brief		ALSA MIDI input client. Pushes ALSA midi input data into alsa_seq_client_rx_queue
*				https://ccrma.stanford.edu/~craig/articles/linuxmidi/alsa-1.0/
*
*	History:\n
*
*			version	1.1		6-Feb-2021
*					1. Code refactoring and notaion.
*		
*			version 1.0		13-Dec--2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Dec 2017)
*/

#pragma once

#include <pthread.h>
#include <alsa/asoundlib.h> 

#include "../utils/safeQueues.h"

using namespace std;

class AlsaMidiSequencerInputClient
{
public:
  AlsaMidiSequencerInputClient(std::string name, SafeQueue<snd_seq_event_t*> *rxq);
	~AlsaMidiSequencerInputClient();

	snd_seq_t* open_seq();
	void close_seq();

	string get_client_name();
	int get_client_id();

	static void* thread_wraper(void* object);

	bool midi_in_client_thread_is_running;
	pthread_t midi_client_in_thread;

	snd_seq_t* midi_in_seq;

	SafeQueue<snd_seq_event_t*> *alsa_seq_client_rx_queue;

private:
	
	string client_name;
	int client_id;

	void start_midi_in_seq_client_thread();
	void stop_midi_in_seq_client_thread();
	
	void* midi_in_seq_client_thread();

};



