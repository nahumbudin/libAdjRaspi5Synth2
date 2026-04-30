/**
*	@file		hammondOrganPresets.cpp
*	@author		Nahum Budin
*	@date		25-Apr-2026
*	@version	1.0
*					
*
*	@brief		Used for handling the hammond organ instrument preset params
*
*	History:\n
*
*/

#include "instrumentHammondOrgan.h"

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
	
	res |= instrument_settings_manager->set_int_param_value(
		params,
		"adjsynth.hammond.organ_leslie_speed",
		50,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	res |= instrument_settings_manager->set_int_param_value(
		params,
		"adjsynth.hammond.organ_leslie_level",
		9,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	res |= instrument_settings_manager->set_int_param_value(
			params,
		"adjsynth.hammond.organ_leslie_speed",
		50,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	res |= instrument_settings_manager->set_int_param_value(
			params,
		"adjsynth.hammond.organ_leslie_level",
		0,
		_SET_VALUE,
		_HAMMOND_ORGAN_PROGRAM_20);
	
	return res;
}

int InstrumentHammondOrgan::set_instrument_settings(_settings_params_t *preset)
{
	int res = 0;
	
	return_val_if_true(active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	
	
	return res;
}

int InstrumentHammondOrgan::get_drawbar_level(int drawbar_num)
{
	int res = 0;
	char keyString[128];
	_settings_int_param_t level;
	
	if (drawbar_num < 1 || drawbar_num > 9)
	{
		return -1;
	}
	
	sprintf(keyString, "adjsynth.osc1.unison_level_%i", drawbar_num);
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		keyString,
		&level);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return (int)(((float)level.value / 100.f * _HAMMOND_ORGAN_DRAWBAR_LEVEL_MAX)); // 0-100 to 0-8
	}

	return res;
}

int InstrumentHammondOrgan::get_octave_detune()
{
	int res = 0;
	_settings_int_param_t detune;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.osc1.tune_offset_oct",
		&detune);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return detune.value;
	}

	return res;
}

int InstrumentHammondOrgan::get_semitones_detune()
{
	int res = 0;
	_settings_int_param_t detune;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.osc1.tune_offset_semitones",
		&detune);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return detune.value;
	}

	return res;
}

int InstrumentHammondOrgan::get_cents_detune()
{
	int res = 0;
	_settings_int_param_t detune;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.osc1.tune_offset_cents",
		&detune);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return detune.value;
	}

	return res;
}

bool InstrumentHammondOrgan::get_square_wave_enable()
{
	int res = 0;
	_settings_bool_param_t square_wave_enable;
	
	res = instrument_settings_manager->get_bool_param(
		active_settings_params,
		"adjsynth.osc1.unison_square_wave",
		&square_wave_enable);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return square_wave_enable.value;
	}
	
	return false;
}

int InstrumentHammondOrgan::get_percussion_mode()
{
	int res = 0;
	_settings_int_param_t percussion_mode;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.osc1.hammond_percussion_mode",
		&percussion_mode);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return percussion_mode.value;
	}
	
	return -1;
}

int InstrumentHammondOrgan::get_leslie_level()
{
	int res = 0;
	_settings_int_param_t leslie_level;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.hammond.organ_leslie_level",
		&leslie_level);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return leslie_level.value;
	}	
	
	return res;
}

int InstrumentHammondOrgan::get_leslie_speed()
{
	int res = 0;
	_settings_int_param_t leslie_speed;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.hammond.organ_leslie_speed",
		&leslie_speed);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return leslie_speed.value;
	}	
	
	return res;
}

int InstrumentHammondOrgan::get_send_filter_1()
{
	int res = 0;
	_settings_int_param_t send_filter_1;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.osc1.send_filter_1",
		&send_filter_1);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return send_filter_1.value;
	}
	
	return res;
}

int InstrumentHammondOrgan::get_send_filter_2()
{
	int res = 0;
	_settings_int_param_t send_filter_2;
	
	res = instrument_settings_manager->get_int_param(
		active_settings_params,
		"adjsynth.osc1.send_filter_2",
		&send_filter_2);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		return send_filter_2.value;
	}
	
	return res;
}
