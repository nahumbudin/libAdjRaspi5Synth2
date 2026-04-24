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
#include "../LibAPI/synthesizer.h"

InstrumentHammondOrgan::InstrumentHammondOrgan(AdjSynth *adj_synth)
	: Instrument(_INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY, true, true, false,
					NULL, NULL, adj_synth)
{
	adjheart_synth = adj_synth;
	
	for (int i = 1; i < 9; i++)
	{
		drawbar_levels[i] = 0;
	}
	
	drawbar_levels[0] =  8;
	square_wave_enable = false;
	percussion_mode = _HAMMOND_ORGAN_PERCUSION_MODE_OFF;
	leslie_speed = 50;
	leslie_level = 0;
	send_filter_1 = 40;
	send_filter_2 = 40;
	
	alsa_midi_sequencer_events_handler->set_instrument(this);
	
	init_analog_synth_default_settings(active_settings_params);
	instrument_settings_manager->set_all_params_type(active_settings_params, "hammond-organ-preset");
	// Adjust the specific settings parameters that are relevant to the Hammond Organ instrument
	init_specific_hammond_organ_default_settings(active_settings_params);
}

InstrumentHammondOrgan::~InstrumentHammondOrgan()
{
}

void InstrumentHammondOrgan::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	AdjSynth::get_instance()->midi_play_note_on(channel, note, velocity, 0, _HAMMOND_ORGAN_PROGRAM_20);
}

void InstrumentHammondOrgan::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	AdjSynth::get_instance()->midi_play_note_off(channel, note, 0, 0, _HAMMOND_ORGAN_PROGRAM_20);
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
	if (moduleid != _HAMMOND_ORGAN_EVENT)
	{
		return -1;
	}
	
	int res = 0;
	int level;
	char keyString[128];
	
	switch (paramid)
	{
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_1:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_2:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_3:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_4:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_5:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_6:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_7:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_8:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_9:
		if (val < _HAMMOND_ORGAN_DRAWBAR_LEVEL_MIN || val > _HAMMOND_ORGAN_DRAWBAR_LEVEL_MAX)
		{
			return -1;
		}
		
		drawbar_levels[paramid - _HAMMOND_ORGAN_DRAWBAR_LEVEL_1] = val;
		
		// Update active settings parameters
		level = int(float(val) * 100.0 / _HAMMOND_ORGAN_DRAWBAR_LEVEL_MAX); // 0-8 to 0-100
		if (level < 0)
		{
			level = 0;
		}
		else if (level > 100)
		{
			level = 100;
		}
		
		sprintf(keyString, "adjsynth.osc1.unison_level_%i", paramid - _HAMMOND_ORGAN_DRAWBAR_LEVEL_1 + 1);
		res |= instrument_settings_manager->set_int_param_value(
			params,
			string(keyString),
			level,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	case _HAMMOND_ORGAN_DETUNE_OCTAVE:
		
		if (val < _OSC_DETUNE_MIN_OCTAVE || val > _OSC_DETUNE_MAX_OCTAVE)
		{
			return -1;
		}
		
		detune_octave = val;
		
		res |= instrument_settings_manager->set_int_param_value(
		params,
			"adjsynth.osc1.tune_offset_oct",
			detune_octave,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	case _HAMMOND_ORGAN_DETUNE_SEMITONES:
		
		if (val < _OSC_DETUNE_MIN_SEMITONES || val > _OSC_DETUNE_MAX_SEMITONES)
		{
			return -1;
		}
		
		detune_semitones = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.tune_offset_semitones",
			detune_semitones,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	case _HAMMOND_ORGAN_DETUNE_CENTS:
		
		if (val < _OSC_DETUNE_MIN_CENTS || val > _OSC_DETUNE_MAX_CENTS)
		{
			return -1;
		}
		
		detune_cents = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.tune_offset_cents",
			detune_cents,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
		
	case _HAMMOND_ORGAN_PERCUSSION_MODE:
		
		if (val < _HAMMOND_ORGAN_PERCUSION_MODE_OFF || val > _HAMMOND_ORGAN_PERCUSION_MODE_3RD_NORM_FAST)
		{
			return -1;
		}
		
		percussion_mode = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.hammond_percussion_mode",
			percussion_mode,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		// Call the callback to update the percussion mode in the synth engine
		// Set OSC2 etc. 
		
		break;
		
	case _HAMMOND_ORGAN_LESLIE_SPEED:
		
			if (val < 0 || val > 100)
			{
				return -1;
			}
			
			leslie_speed = val;
			
		// Update active settings parameters - LFO frequency. 
		// TODO:
		
		break;
		
	case _HAMMOND_ORGAN_LESLIE_LEVEL:
		
		if (val < 0 || val > 100)
		{
			return -1;
		}
			
		leslie_level = val;
			
		// Update active settings parameters - PAN modulation level. 
		// TODO:
		break;
		
	case _HAMMOND_ORGAN_SEND_FILTER_1:
		
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		send_filter_1 = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.send_filter_1",
			send_filter_1,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		break;
		
	case _HAMMOND_ORGAN_SEND_FILTER_2:
		
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		send_filter_2 = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.send_filter_2",
			send_filter_2,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	default:
		return -1;
		
	}
	
	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{

	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	if (moduleid != _HAMMOND_ORGAN_EVENT)
	{
		return -1;
	}
	
	int res = 0;
	
	switch (paramid)
	{
		
	case _HAMMOND_ORGAN_SQUARE_WAVE_ENABLE:
		
		square_wave_enable = val;
		
		res |= instrument_settings_manager->set_bool_param_value(
			params,
			"adjsynth.osc1.unison_square_wave",
			square_wave_enable,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	default:
		return -1;
		
	}
	
	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{

	return 0;
}

int InstrumentHammondOrgan::set_drawbar_level(int drawbar_num, int level)
{
	if (drawbar_num < 1 || drawbar_num > 9 || 
		level < _HAMMOND_ORGAN_DRAWBAR_LEVEL_MIN || 
		level > _HAMMOND_ORGAN_DRAWBAR_LEVEL_MAX)
	{
		return -1;
	}
	
	drawbar_levels[drawbar_num - 1] = level;
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_drawbar_level(int drawbar_num)
{
	if (drawbar_num < 1 || drawbar_num > 9)
	{
		return -1;
	}

	return drawbar_levels[drawbar_num - 1];
}
	
int InstrumentHammondOrgan::set_octave_detune(int detune)
{
	if (detune < _OSC_DETUNE_MIN_OCTAVE || detune > _OSC_DETUNE_MAX_OCTAVE)
	{
		return -1;
	}
	
	detune_octave = detune;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::get_octave_detune()
{
	return detune_octave;
}
	
int InstrumentHammondOrgan::set_semitones_detune(int detune)
{
	if (detune < _OSC_DETUNE_MIN_SEMITONES || detune > _OSC_DETUNE_MAX_SEMITONES)
	{
		return -1;
	}
	
	detune_semitones = detune;	
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_semitones_detune()
{
	return detune_semitones;
}
	
int InstrumentHammondOrgan::set_cents_detune(int detune)
{
	if (detune < _OSC_DETUNE_MIN_CENTS || detune > _OSC_DETUNE_MAX_CENTS)
	{
		return -1;
	}
	
	detune_cents = detune;
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_cents_detune()
{
	return detune_cents;
}
	
int InstrumentHammondOrgan::set_square_wave_enable(bool state)
{
	square_wave_enable = state;
	
	// Update active settings parameters
	
	return 0;
}

bool InstrumentHammondOrgan::get_square_wave_enable()
{
	return square_wave_enable;
}
	
int InstrumentHammondOrgan::set_percussion_mode(int mode)
{
	if (mode < _HAMMOND_ORGAN_PERCUSION_MODE_OFF || mode > _HAMMOND_ORGAN_PERCUSION_MODE_3RD_NORM_FAST)
	{
		return -1;
	}
	
	percussion_mode = mode;
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_percussion_mode()
{
	return percussion_mode;
}
	
int InstrumentHammondOrgan::set_leslie_speed(int speed)
{
	if (speed < 0 || speed > 100)
	{
		return -1;
	}
	
	leslie_speed = speed;
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_leslie_speed()
{
	return leslie_speed;
}
	
int InstrumentHammondOrgan::set_leslie_level(int level)
{
	if (level < 0 || level > 100)
	{
		return -1;
	}
	
	leslie_level = level;
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_leslie_level()
{
	return leslie_level;
}
	
int InstrumentHammondOrgan::set_send_filter_1(int level)
{
	if (level < 0 || level > 100)
	{
		return -1;
	}
	
	send_filter_1 = level;
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_send_filter_1()
{
	return send_filter_1;
}
	
int InstrumentHammondOrgan::set_send_filter_2(int level)
{
	if (level < 0 || level > 100)
	{
		return -1;
	}
	
	send_filter_2 = level;
	
	// Update active settings parameters
	
	return 0;
}

int InstrumentHammondOrgan::get_send_filter_2()
{
	return send_filter_2;
}

// Set all analog synth parameters to default values for the Hammond Organ program.
int InstrumentHammondOrgan::init_analog_synth_default_settings(_settings_params_t *params)
{
	int res = 0;
	
	res = adjheart_synth->set_default_preset_parameters_amp(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_distortion(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_filter(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_kps(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_modulators(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_mso(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_noise(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_pad(params, _HAMMOND_ORGAN_PROGRAM_20);
	res |= adjheart_synth->set_default_preset_parameters_vco(params, _HAMMOND_ORGAN_PROGRAM_20);
	
	return res;
}

// Set and modify all the specific Hammond Organ parameters that differs from the default analog synth values.
int InstrumentHammondOrgan::init_specific_hammond_organ_default_settings(_settings_params_t *params)
{
	int res = 0;
	
	// "adjsynth.osc1.enabled" is set to true by default in the analog synth default settings.
	// "adjsynth.osc1.waveform" is set to sine wave by default in the analog synth default settings.
	// "adjsynth.osc1.symmetry" is set to 50% by default in the analog synth default settings.
	// "adjsynth.osc1.tune_offset_oct" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.tune_offset_semitones" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.tune_offset_cents" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.freq_modulation_lfo_num" is set to 0 (no mod) by default in the analog synth default settings.
	// "adjsynth.osc1.freq_modulation_lfo_level" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.freq_modulation_env_num" is set to 0 (no mod) by default in the analog synth default settings.
	// "adjsynth.osc1.freq_modulation_env_level" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.pwm_modulation_lfo_num" is set to 0 (no mod) by default in the analog synth default settings.
	// "adjsynth.osc1.pwm_modulation_lfo_level" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.pwm_modulation_env_num" is set to 0 (no mod) by default in the analog synth default settings.
	// "adjsynth.osc1.pwm_modulation_env_level" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.amp_modulation_lfo_num" is set to 0 (no mod) by default in the analog synth default settings.
	// "adjsynth.osc1.amp_modulation_lfo_level" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.hammond_percussion_mode" is set to 0 (off) by default in the analog synth default settings.
	// "adjsynth.osc1.unison_level_1" is set to 100 by default in the analog synth default settings.
	// "adjsynth.osc1.unison_level_2" - level_9 are set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.unison_distortion" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.unison_detune" is set to 0 by default in the analog synth default settings.
	// "adjsynth.osc1.unison_square_wave" is set to false by default in the analog synth default settings.
	// All OSC2 parameters are set to default values in the analog synth default settings.
	
	
	
	res |= instrument_settings_manager->set_int_param_value(
		params,
		"adjsynth.osc1.send_filter_1",
		40,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	res |= instrument_settings_manager->set_int_param_value(
		params,
		"adjsynth.osc1.send_filter_2",
		40,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	// Use ADSR 1 for amplitude modulation envelope
	res |= instrument_settings_manager->set_int_param_value(
		params,
		"adjsynth.osc1.amp_modulation_env_num",
		_ENV_1,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);

	// Set the amplitude modulation envelope level to 100%
	res |= instrument_settings_manager->set_int_param_value(
		params,
		"adjsynth.osc1.amp_modulation_env_level",
		100,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	// Set the unison mode to Hammond Organ specific unison mode (octave and fifths)
	res |= instrument_settings_manager->set_int_param_value(
		params,
		"adjsynth.osc1.unison_mode",
		_OSC_UNISON_MODE_HAMMOND,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	
	
	
	return res;
}

