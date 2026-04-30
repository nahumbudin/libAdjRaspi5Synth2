/**
* @file		instrumentHammondOrgan.h
*	@author		Nahum Budin
*	@date		26-06-2024
*	@version	1.0	Initial release
*					
*	@brief		Implements an Hammond Organ instrument.
*	
*	History:\n
*	
*/

#pragma once

#include "instrument.h"


class InstrumentHammondOrgan : public Instrument
{
  public:
	InstrumentHammondOrgan(AdjSynth *adj_synth, _settings_params_t *external_settings);

	~InstrumentHammondOrgan();

	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);

	int set_default_settings_parameters(_settings_params_t *params = NULL, int prog = -1);
	
	int set_instrument_settings(_settings_params_t *preset);

	int events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, bool, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, string, _settings_params_t *params, int program = -1);
	
	int set_drawbar_level(int drawbar_num, int level);
	int get_drawbar_level(int drawbar_num);
	
	int set_octave_detune(int detune);
	int get_octave_detune();
	
	int set_semitones_detune(int detune);
	int get_semitones_detune();
	
	int set_cents_detune(int detune);
	int get_cents_detune();
	
	int set_square_wave_enable(bool state);
	bool get_square_wave_enable();
	
	int set_percussion_mode(int mode);
	int get_percussion_mode();
	
	int set_leslie_speed(int speed);
	int get_leslie_speed();
	
	int set_leslie_level(int level);
	int get_leslie_level();
	
	int set_send_filter_1(int level);
	int get_send_filter_1();
	
	int set_send_filter_2(int level);
	int get_send_filter_2();
	
	int init_analog_synth_default_settings(_settings_params_t *params);
	int init_specific_hammond_organ_default_settings(_settings_params_t *params);
	
private:
	
	int drawbar_levels[9];
	bool square_wave_enable;
	int percussion_mode;
	bool percussion_soft;
	int leslie_speed;
	int leslie_level;
	int send_filter_1;
	int send_filter_2;
	int detune_octave;
	int detune_semitones;
	int detune_cents;
	
	
};
