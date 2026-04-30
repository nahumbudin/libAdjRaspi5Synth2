/**
* @file		instrument.h
*	@author		Nahum Budin
*	@date		11-Apr-2026
*	@version	2.0	
*					1. Specific instruments settings are handled by each instrument.
*					2. No need to split to voice and common - decide per case which settings parameters are needed, 
*					   and use only them.
*					3. Instrument settings params may be local or refernced to, for example, 
*					    a program settings params in the AdjSynth
*					4. Add instrument type: playing, effect, control.
*					   
*					
*	@brief		The basic music generating object, e.g., analog-synthesizer,
*				FluidSynth SoundFont synthesizer, organ, etc.
*	
*	History:\n
*				versio 1.1	  24-09-2025
*					1. Adding a pointer to a AdjSynth object
*					2. Splitting the settings parameters to two groups: active_preset_settings_params and 
*					   active_common_settings_params,
*					3. Splitting active_preset_settings_params to two groups: active_program_settings_params and 
*					    active_common_settings_params, used for the AdjSynth setting params and adding a 
*					    active_fluid_synth_settings_params for the FluidSynth settings params
*					    
*				version 1.0		24-06-2024: First version
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
			 AdjSynth *adj_synth = NULL,
			 _settings_params_t *external_settings = NULL, 
			 int inst_type = _INSTRUMENT_TYPE_PLAYING);
	~Instrument();
	
	// Use instrument's own local settings
	void use_local_settings();	
	// Use external settings (e.g., from a Program)
	void use_external_settings(_settings_params_t* external_params);
	// Get current active settings
	_settings_params_t* get_active_settings_params();

	void set_active_midi_channels(uint16_t act_chans);
	uint16_t get_active_midi_channels();
	
	virtual int init();
	
	void set_instrument_type(int type);
	int get_instrument_type();
	
	
	// Settings parameters management
	
	// Set the default settings parameters (init the active_settings_params)
	// (note - each instrument is assigned with a specific single program, so the default settings parameters are set for this program).
	virtual int set_default_settings_parameters();
	
	// Set all the instrument parameters based on the settings parameters in the active_settings_params structure.
	virtual int set_instrument_settings();
	
	// Saves the active settings parameters to an XML file.
	int save_instrument_active_settings(string path, string type = "type-none");
	
	// Reads the active settings parameters from an XML file into the active_settings_params.
	int read_instrument_settings(_settings_params_t *preset, string path, string type = "type-none");
	
	
	void register_ui_update_callback(func_ptr_void_void_t ptr);
	void activate_ui_update_callback();
	
	string get_instrument_name();

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

	

	AlsaMidiSeqencerEventsHandler *alsa_midi_sequencer_events_handler;

	AdjSynth *adjheart_synth;

	// Settings managers of the instrument, used to manage the settings parameters of the instrument.
	Settings *instrument_settings_manager;
	
	_settings_params_t local_settings_params; // Optional local storage
	_settings_params_t *active_settings_params; // Pointer to active settings		

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
	
	// Playing, effect, control.
	int instrument_type;

	SafeQueue<snd_seq_event_t*> alsa_rx_queue;

	string instrument_name;

	int alsa_input_client_id;

	func_ptr_void_void_t update_ui_callback_ptr;

	bool midi_in_enable;
	bool midi_out_enable;
	bool audio_out_enable;
};

