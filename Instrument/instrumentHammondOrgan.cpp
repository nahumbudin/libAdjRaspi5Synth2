/**
* @file		instrumentHammondOrgan.cpp
*	@author		Nahum Budin
*	@date		26-06-2024
*	@version	1.0	Initial release
*					
*	@brief		Implements an Hammond Organ instrument.
*	
*	History:\n
*	
*/

#include "instrumentHammondOrgan.h"

InstrumentHammondOrgan::InstrumentHammondOrgan()
	: Instrument(_INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY, true, true, false)
{

	alsa_midi_sequencer_events_handler->set_instrument(this);
}

InstrumentHammondOrgan::~InstrumentHammondOrgan()
{
}

void InstrumentHammondOrgan::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void InstrumentHammondOrgan::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void InstrumentHammondOrgan::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentHammondOrgan::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentHammondOrgan::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
}

void InstrumentHammondOrgan::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentHammondOrgan::sysex_handler(uint8_t *message, int len)
{
}

int InstrumentHammondOrgan::set_default_settings_parameters(_settings_params_t *params, int prog)
{

	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{

	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{

	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{

	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{

	return 0;
}

