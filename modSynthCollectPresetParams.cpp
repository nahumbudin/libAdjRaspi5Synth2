/**
 *	@file		modSynthCollectPresetParams.cpp
 *	@author		Nahum Budin
 *	@date		2-Oct-2025
 *	@version	1.3
 *					1. Adding support in both old and new MIDI program objects..
 *
 *	@brief		Used for collecting preset params
 *
 *	History:\n
 *
 *		version 1.2	14-Oct-2024	Code refactoring and notaion.
 *		version	1.1	7-Feb-2021	Code refactoring and notaion
 *		version 1.0	30-4-2020: 	First version
 *
 */

#include "modSynth.h"
#include "./AdjSynth/adjSynth.h"
#include "Instrument/instrumentFluidSynth.h"

int ModSynth::collect_mod_synth_preset_parms(_settings_params_t *params) {

	int res = 0;
	int result = 0;

	_settings_int_param_t int_param;
	_settings_float_param_t float_param;
	_settings_bool_param_t bool_param;

	int int_val;
	float float_val;
	bool bool_val;

	for (int i = 0; i < 16; i++)
	{
		res = AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".synth_type",
			midi_channel_synth[i],
			_SET_VALUE,
			-1); // no program (common resource)

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_program",
			mod_synth_get_fluid_synth_channel_program(i),
			_SET_VALUE,
			-1);

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_string_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_program_name",
			fluid_synth->get_fluid_synth_interface()->get_fluid_synth_channel_preset_name(i),
			_SET_VALUE,
			-1);

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_bank",
			mod_synth_get_fluid_synth_channel_bank(i),
			_SET_VALUE,
			-1);

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_string_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_sound_font",
			mod_synth_get_fluid_synth_channel_preset_soundfont_name(i),
			_SET_VALUE,
			-1);

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_string_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".adj_synth_preset_name",
#ifdef _USE_NEW_MIDI_PROGRAM
			AdjSynth::get_instance()->synth_program[i]->active_preset_params->name,
#else
			AdjSynth::get_instance()->synth_program[i]->active_preset_params.name,
#endif
			_SET_VALUE,
			-1);

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".level",
			mod_synth_get_active_midi_mixer_setting_param(i + _MIXER_CHAN_1_LEVEL),
			_SET_VALUE,
			-1);

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".pan",
			mod_synth_get_active_midi_mixer_setting_param(i + _MIXER_CHAN_1_PAN),
			_SET_VALUE,
			-1);

		res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".send",
			mod_synth_get_active_midi_mixer_setting_param(i + _MIXER_CHAN_1_SEND),
			_SET_VALUE,
			-1);
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"fluid_synth.volume",
		get_fluid_synth_volume(),
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_bool_param(
		&mod_synth->active_fluid_synth_settings_params, "fluid.synth.reverb.active", &bool_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		bool_val = bool_param.value;
	}
	else
	{
		bool_val = false;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_bool_param_value(
		params,
		"fluid_synth.reverb.enabled",
		bool_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_float_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.reverb.room-size",
		&float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_float_param_value(
		params,
		"fluid_synth.reverb.room-size",
		float_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_float_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.reverb.damp",
		&float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_float_param_value(
		params,
		"fluid_synth.reverb.damp",
		float_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_float_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.reverb.width",
		&float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_float_param_value(
		params,
		"fluid_synth.reverb.width",
		float_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_float_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.reverb.level",
		&float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_float_param_value(
		params,
		"fluid_synth.reverb.level",
		float_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_bool_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.chorus.active",
		&bool_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		bool_val = bool_param.value;
	}
	else
	{
		bool_val = false;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_bool_param_value(
		params,
		"fluid_synth.chorus.enabled",
		bool_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_int_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.chorus.nr",
		&int_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		int_val = int_param.value;
	}
	else
	{
		int_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"fluid_synth.chorus.N",
		int_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_float_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.chorus.level",
		&float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_float_param_value(
		params,
		"fluid_synth.chorus.level",
		float_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_float_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.chorus.speed",
		&float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_float_param_value(
		params,
		"fluid_synth.chorus.speed",
		float_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_float_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.chorus.depth",
		&float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_float_param_value(
		params,
		"fluid_synth.chorus.depth",
		float_val,
		_SET_VALUE,
		-1);

	result = mod_synth->fluid_synth_settings_manager->get_int_param(
		&mod_synth->active_fluid_synth_settings_params,
		"fluid.synth.chorus.waveform",
		&int_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		int_val = int_param.value;
	}
	else
	{
		int_val = 0;
	}

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"fluid_synth.chorus.waveform",
		int_val,
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_bool_param_value(
		params,
		"adjsynth.reverb3m.enable_state",
		mod_synth_get_active_reverb_3m_enable_state(),
		_SET_VALUE,
		-1); // no program (common resource)

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_bool_param_value(
		params,
		"adjsynth.reverb.enable_state",
		mod_synth_get_active_reverb_enable_state(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.reverb3m.preset",
		mod_synth_get_active_reverb_3m_preset(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.reverb.room_size",
		mod_synth_get_active_reverb_room_size(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.reverb.damp",
		mod_synth_get_active_reverb_damp(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.reverb.wet",
		mod_synth_get_active_reverb_wet(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.reverb.dry",
		mod_synth_get_active_reverb_dry(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.reverb.width",
		mod_synth_get_active_reverb_width(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.reverb.mode",
		mod_synth_get_active_reverb_mode(),
		_SET_VALUE,
		-1);

	res = AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_31_level",
		mod_synth_get_active_equilizer_band31_level(),
		_SET_VALUE,
		-1); // -1: no program (common resource)

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_62_level",
		mod_synth_get_active_equilizer_band62_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_125_level",
		mod_synth_get_active_equilizer_band125_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_250_level",
		mod_synth_get_active_equilizer_band250_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_500_level",
		mod_synth_get_active_equilizer_band500_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_1k_level",
		mod_synth_get_active_equilizer_band1k_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_2k_level",
		mod_synth_get_active_equilizer_band2k_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_4k_level",
		mod_synth_get_active_equilizer_band4k_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_8k_level",
		mod_synth_get_active_equilizer_band8k_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.band_16k_level",
		mod_synth_get_active_equilizer_band16k_level(),
		_SET_VALUE,
		-1);

	res |= AdjSynth::get_instance()->adj_synth_settings_manager->set_int_param_value(
		params,
		"adjsynth.equilizer.preset",
		mod_synth_get_active_equilizer_preset(),
		_SET_VALUE,
		-1);

	return res;
}
