/**
* @file		instrumentStringSynth.h
*	@author		Nahum Budin
*	@date		1-May-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements a Karplus-Strong String Synth instrument.
*	
*	History:\n
*	
*/

#pragma once

#include "instrument.h"

class InstrumentStringSynth : public Instrument
{
public:
	InstrumentStringSynth(AdjSynth *adj_synth, _settings_params_t *external_settings);

	~InstrumentStringSynth();

	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);

	int set_default_settings_parameters(_settings_params_t *params = NULL, int prog = -1);
	
	int set_instrument_settings(_settings_params_t *preset);
	
	void set_tune_offset_octave(int octave);
	void set_tune_offset_semitones(int semitones);
	
	int get_tune_offset_octave();
	int get_tune_offset_semitones();
	
	int get_param_value(int paramid);

	int events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, bool, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, string, _settings_params_t *params, int program = -1);
	
	int init_string_synth_default_settings(_settings_params_t *params);
	
private:
	
	// We use these varaibles to prevent frequent access to the settings parameters 
	// when handling note-on and note-off MIDI events, which may cause performance issues.
	int detune_octave = 0;
	int detune_semitones = 0;
	
	
};
