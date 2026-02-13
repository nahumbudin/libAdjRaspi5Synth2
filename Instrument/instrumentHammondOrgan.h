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
	InstrumentHammondOrgan();

	~InstrumentHammondOrgan();

	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);

	int set_default_settings_parameters(_settings_params_t *params = NULL, int prog = -1);

	int events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, bool, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, string, _settings_params_t *params, int program = -1);
	
};
