/**
*	@file		alsaMidiSequencerEventsHandler.h
*	@author		Nahum Budin
*	@date		12-May-2024
*	@version	1.4
*					1. Code refactoring and notaion.
*					2. Adding Rx Q as a parameter
*					3. Remove commands registration by passing parent instrument as a parameter
*
*	@brief		Recives data from a alsa midi by pulling it out of the midi in rx data queue.
*
*	History:
*	version 1.3	???
*	version 1.2 26-Nov-2020
*					1. Add support in Serial Port MIDI input.
*					2. Add ALSA client to support serial port input stream..*
*	version	1.1 13-12-2019(revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Sep 2018)
*
*/

#pragma once

#include <stdint.h>
//#include <alsa/asoundlib.h> 

#include "../utils/safeQueues.h"
#include "../MIDI/midiStream.h"
#include "../Instrument/instrument.h"

class Instrument;

class AlsaMidiSeqencerEventsHandler : public MidiStream {

public:
	AlsaMidiSeqencerEventsHandler(uint8_t stage = 0, SafeQueue<snd_seq_event_t *> *rxq = NULL);

	 void set_instrument(Instrument *inst);
	
	void set_active_midi_channels(uint16_t act_chans);
	uint16_t get_active_midi_channels();

	virtual void update(void);
	virtual void channel_is_active_updater(int chan);
	
	bool update_active_midi_channels_on_channel_change;
	
private:
  SafeQueue<snd_seq_event_t *> *alsa_seq_client_rx_queue;

  Instrument *instrument;

  uint16_t active_midi_channels;
};

