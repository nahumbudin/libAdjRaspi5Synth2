/**
*	@file		alsaMidiSequencerClient.cpp
*	@author		Nahum Budin
*	@date		12-May-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*
*	@brief		ALSA MIDI input client. Pushes ALSA midi input data into alsa_seq_client_rx_queue
*				https://ccrma.stanford.edu/~craig/articles/linuxmidi/alsa-1.0/
*
*	History:\n
*
*			version	1.1		6-Feb-2021
*					1. Code refactoring and notaion.
*					2. Adding Rx Q as a parameter
*
*			version 1.0		13-Dec--2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Dec 2017)
*/

#include <pthread.h>

#include "alsaMidiSequencerClient.h"
#include "../Misc/priorities.h"

AlsaMidiSequencerInputClient::AlsaMidiSequencerInputClient(std::string name, SafeQueue<snd_seq_event_t*> *rxq)
{
	client_name = name;
	start_midi_in_seq_client_thread();
	alsa_seq_client_rx_queue = rxq;
}

AlsaMidiSequencerInputClient::~AlsaMidiSequencerInputClient()
{
	close_seq();
	stop_midi_in_seq_client_thread();
}

void AlsaMidiSequencerInputClient::start_midi_in_seq_client_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_IN;
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
		fprintf(stderr, "ALSA MIDI sequencer: Unsuccessful in setting Midi In Seq Client %s thread realtime prio\n", client_name.c_str());
	}

	midi_in_client_thread_is_running = true;
	ret = pthread_create(&midi_client_in_thread, &tattr, &AlsaMidiSequencerInputClient::thread_wraper, this); // open in hread
	if (ret == -1) {
		fprintf(stderr, "ALSA MIDI sequencer: Unable to create MIDI input Seq Client %s thread.", client_name.c_str());
		exit(1);
	}
	pthread_setname_np(midi_client_in_thread, "midi_in_seq_client_thread ");
}

void AlsaMidiSequencerInputClient::stop_midi_in_seq_client_thread()
{
	midi_in_client_thread_is_running = false;
}

snd_seq_t* AlsaMidiSequencerInputClient::open_seq()
{

	snd_seq_t* seq_handle;
	snd_seq_port_info_t* port_info;
	snd_seq_client_info_t* client_info;
	int port_id;

	int ret = snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0);
	if (ret != 0)
	{
		fprintf(stderr, "ALSA MIDI sequencer: Error opening ALSA sequencer %s.\n", client_name.c_str());
		return NULL;
	}

	snd_seq_set_client_name(seq_handle, client_name.c_str());
	if ((port_id = snd_seq_create_simple_port(seq_handle,
		client_name.c_str(),
		SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
		SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
			fprintf(stderr, "ALSA MIDI sequencer: Error creating sequencer port %s.\n", client_name.c_str());
		return NULL;
	}
	
	client_id = snd_seq_client_id(seq_handle);

	return(seq_handle);
}


void AlsaMidiSequencerInputClient::close_seq()
{
	if (midi_in_seq)
	{
		snd_seq_close(midi_in_seq);
	}
}

std::string AlsaMidiSequencerInputClient::get_client_name()
{
	return client_name;
}
int AlsaMidiSequencerInputClient::get_client_id()
{
	return client_id;
}

void* AlsaMidiSequencerInputClient::thread_wraper(void* object)
{
	reinterpret_cast<AlsaMidiSequencerInputClient*>(object)->midi_in_seq_client_thread();
	return 0;
}

void* AlsaMidiSequencerInputClient::midi_in_seq_client_thread()
{
	snd_seq_t* midi_in = open_seq();  
	int npfd;
	struct pollfd* pfd;
	snd_seq_event_t* ev, * qev;

	npfd = snd_seq_poll_descriptors_count(midi_in, POLLIN);
	pfd = (struct pollfd*)alloca(npfd * sizeof(struct pollfd));
	snd_seq_poll_descriptors(midi_in, pfd, npfd, POLLIN);

	while (midi_in_client_thread_is_running)
	{
		if (poll(pfd, npfd, 1000) > 0) { // 100000
			do {
				snd_seq_event_input(midi_in, &ev);

				qev = new snd_seq_event_t();
				memcpy(qev, ev, sizeof(snd_seq_event_t));
				alsa_seq_client_rx_queue->enqueue(qev);
				/*
								switch (ev->type) {
									case SND_SEQ_EVENT_CONTROLLER:
										fprintf(stderr, "Control event on Channel %2d: %5d       \r",
											ev->data.control.channel, ev->data.control.value);
										break;
									case SND_SEQ_EVENT_PITCHBEND:
										fprintf(stderr, "Pitchbender event on Channel %2d: %5d   \r",
											ev->data.control.channel, ev->data.control.value);
										break;
									case SND_SEQ_EVENT_NOTEON:
										fprintf(stderr, "Note On event on Channel %2d: %5d       \r",
											ev->data.control.channel, ev->data.note.note);
										break;
									case SND_SEQ_EVENT_NOTEOFF:
										fprintf(stderr, "Note Off event on Channel %2d: %5d      \r",
											ev->data.control.channel, ev->data.note.note);
										break;
								}
				*/
				snd_seq_free_event(ev);
			} while (snd_seq_event_input_pending(midi_in, 0) > 0);
		}
	}

	return NULL;
}
