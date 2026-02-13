/**
*	@file		modSynthDefaultPresetParams.cpp
*	@author		Nahum Budin
*	@date		18-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*
*	@brief		Set default preset parameters
*
*	History:\n
*				version	1.1	6-02-2021	Code refactoring and notaion.
*				version 1.0	02-05-2020	First version
*
*/

#include "modSynth.h"
#include "modSynthPatch.h"
#include "./AdjSynth/adjSynth.h"
#include "./FluidSynth/fluidSynthInterface.h"
#include <string>

int ModSynth::set_default_preset_parameters(_settings_params_t *params) {

	int res;

	for (int i = 0; i < 16; i++)
	{
		
		// Note - we are using the block callback option to target each 
		//        midi-mixer channel
		res = ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1)+ ".synth_type",
			_MIDI_CHAN_ASSIGNED_SYNTH_NONE,
			_MIDI_CHAN_ASSIGNED_SYNTH_ADJ,
			_MIDI_CHAN_ASSIGNED_SYNTH_NONE,
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_synth_type_cb,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_TYPE | _SET_BLOCK_CALLBACK,
			-1); // No prog

		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_program",
			0,
			127,
			0,
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_midi_program_cb,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_TYPE | _SET_BLOCK_CALLBACK,
			-1);
		
		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_string_param(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_program_name",
			"",
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_midi_program_name_cb,
			_SET_VALUE | _SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_TYPE | _SET_BLOCK_CALLBACK,
			-1);

		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(	
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_bank",
			0,
			256,
			0,
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_midi_bank_cb,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_TYPE | _SET_BLOCK_CALLBACK,
			-1);

		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_string_param(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".midi_sound_font",
			"FluidR3_GM.sf2",
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_midi_sound_font_cb,
			_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_BLOCK_CALLBACK,
			
			-1);

		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_string_param(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".adj_synth_patch_name",
			"default_patch",
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_adj_synth_patch_cb,
			_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_BLOCK_CALLBACK,
			-1);

		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
			params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".level",
			0,
			100,
			0,
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_level_cb,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_TYPE | _SET_BLOCK_CALLBACK,
			-1);

		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
				params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".pan",
			50,
			100,
			0,
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_pan_cb,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_TYPE | _SET_BLOCK_CALLBACK,
			-1);

		res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
				params,
			"modsynth.mixer_ch" + std::to_string(i + 1) + ".send",
			0,
			100,
			0,
			_MOD_SYNTH_PRESET_PARAMS,
			NULL,
			i,
			i,
			set_mixer_channel_send_cb,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_BLOCK_START_INDEX | _SET_BLOCK_STOP_INDEX |
			_SET_TYPE | _SET_BLOCK_CALLBACK,
			-1);
	}

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"fluid_synth.volume",
		0,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_volume_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_bool_param(
		params,
		"fluid_synth.reverb.enabled",
		false,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_reverb_enable_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_float_param(
		params,
		"fluid_synth.reverb.room-size",
		0,
		1.0,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_reverb_room_size_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_float_param(
		params,
		"fluid_synth.reverb.damp",
		0,
		1.0,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_reverb_damp_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_float_param(
		params,
		"fluid_synth.reverb.width",
		0,
		1.0,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_reverb_width_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_float_param(
		params,
		"fluid_synth.reverb.level",
		0,
		1.0,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_reverb_level_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_bool_param(
		params,
		"fluid_synth.chorus.enabled",
		false,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_chorus_enable_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"fluid_synth.chorus.N",
		0,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_chorus_n_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_float_param(
		params,
		"fluid_synth.chorus.level",
		0,
		10.0,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_chorus_level_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_float_param(
		params,
		"fluid_synth.chorus.speed",
		0.29,
		5.0,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_chorus_speed_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_float_param(
		params,
		"fluid_synth.chorus.depth",
		0,
		21.0,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_chorus_depth_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"fluid_synth.chorus.waveform",
		0,
		1,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_fluid_synth_chorus_waveform_callback,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.reverb3m.enable_state",
		false,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb3m_enable_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1); // no program (common resource)

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.reverb.enable_state",
		false,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb_enable_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.reverb3m.preset",
		_SF_REVERB_PRESET_DEFAULT,
		_SF_REVERB_PRESET_LONGREVERB2,
		_SF_REVERB_PRESET_DEFAULT,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb3m_preset_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.reverb.room_size",
		50,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb_room_size_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.reverb.damp",
		50,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb_damp_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.reverb.wet",
		50,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb_wet_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.reverb.dry",
		50,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb_dry_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.reverb.width",
		0,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb_width_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.reverb.mode",
		0,
		100,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_reverb_mode_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res = ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_31_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_31_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1); // -1: no program (common resource)

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_62_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_62_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_125_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_125_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_250_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_250_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_500_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_500_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_1k_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_1k_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_2k_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_2k_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_4k_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_4k_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_8k_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_8k_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.band_16k_level",
		0,
		50,
		-50,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_band_16k_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= ModSynth::get_instance()->adj_synth->adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.equilizer.preset",
		0,
		10,
		0,
		_MOD_SYNTH_PRESET_PARAMS,
		set_equilizer_preset_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);


	return res;

}