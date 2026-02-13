/**
* @file		instrument.h
*	@author		Nahum Budin
*	@date		24-09-2025
*	@version	1.1	
*					1. Adding a pointer to a AdjSynth object
*					
*	@brief		The basic music generating object, e.g., analog-synthesizer,
*				FluidSynth SoundFont synthesizer, organ, etc.
*	
*	History:\n
*		version 1.0		24-06-2024: First version
*	
*/

#pragma once

#include "../AdjSynth/adjSynth.h"

#include "../ALSA/alsaMidi.h"
#include "../ALSA/alsaMidiSequencerClient.h"
#include "../ALSA/alsaMidiSequencerEventsHandler.h"

#include "../Settings/settings.h"

#include "instrumentConnectionsControl.h"
#include "../Jack/jackConnections.h"
#include "instrumentControlApi.h"

using namespace std;

class AlsaMidiSeqencerEventsHandler;

class Instrument
{
public:
  Instrument(string name = "", bool with_midi_in = false,
			 bool with_audio_out = true, bool with_midi_out = false,
			 AlsaMidiSysControl *alsa_control = NULL,
			 string *alsa_client_in_name = NULL,
			 AdjSynth *adj_synth = NULL);
	~Instrument();

	void set_active_midi_channels(uint16_t act_chans);
	uint16_t get_active_midi_channels();
	
	virtual int init();

	void register_ui_update_callback(func_ptr_void_void_t ptr);
	void activate_ui_update_callback();

	virtual int api_settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program = -1);
	virtual int api_settings_events_handler(int moduleid, int paramid, float val, _settings_params_t *params, int program = -1);
	virtual int api_settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program = -1);
	virtual int api_settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program = -1);

	virtual void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	virtual void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	virtual void change_program_handler(uint8_t channel, uint8_t program);
	virtual void channel_pressure_handler(uint8_t channel, uint8_t val);
	virtual void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	virtual void pitch_bend_handler(uint8_t channel, int pitch);
	virtual void sysex_handler(uint8_t *message, int len);
	virtual void control_box_events_handler(int event_id, uint16_t event_value);

	virtual int set_default_settings_parameters(_settings_params_t *params, int prog = -1);

	AlsaMidiSeqencerEventsHandler *alsa_midi_sequencer_events_handler;

	AdjSynth *adjheart_synth;

	Settings *instrument_settings;
	_settings_params_t *active_settings_params;

//	InstrumentConnectionsControl *instrument_connections_control;
	AlsaMidiSysControl *alsa_connections;
	JackConnections *jack_connections;

	snd_rawmidi_t *midiout;

  private:
	/* Receives input midi data stream from ALSA midi source.
	 * Connect to it using, for example, a JACK Audio Connection Kit.
	 * Received data is pushed into the alsa_seq_client_rx_queue */ 
	AlsaMidiSequencerInputClient* alsa_midi_sequencer_input_client;

	/** RawMidi handle to send out MIDI */

	int mode;

	SafeQueue<snd_seq_event_t*> alsa_rx_queue;

	string instrument_name;

	int alsa_input_client_id;

	func_ptr_void_void_t update_ui_callback_ptr;

	bool midi_in_enable;
	bool midi_out_enable;
	bool audio_out_enable;
};

