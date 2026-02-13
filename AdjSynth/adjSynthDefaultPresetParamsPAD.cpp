/**
 *	@file		adjSynthDefaultPresetParamsPAD.cpp
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.1
 *					1. Rename patch to preset parameters.
 *					2. Adding default program settings callbacks
*	
*	@brief		Set default patch PAD Synthesizer parameters
*
*	History:\n
*	
*		version 1.1		4-Oct-2024:	Code refactoring and notaion.
*		version 1.0		15_Nov-2019:	First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_preset_parameters_pad(_settings_params_t *params, int prog)
{
	int res;

	res = adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.pad_synth.enabled",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_enabled_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_enabled_cb,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.tune_offset_octave",
		0,
		_OSC_DETUNE_MAX_OCTAVE,
		_OSC_DETUNE_MIN_OCTAVE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_detune_octave_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_detune_octave_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.tune_offset_semitones",
		0,
		_OSC_DETUNE_MAX_SEMITONES,
		_OSC_DETUNE_MIN_SEMITONES,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_detune_semitones_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_detune_semitones_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.tune_offset_cents",
		0,
		_OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_detune_cents_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_detune_cents_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.send_filter_1",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_send_filter_1_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_send_filter_1_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.send_filter_2",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_send_filter_2_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_send_filter_2_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.freq_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_freq_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_freq_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.freq_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_freq_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_freq_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.freq_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_freq_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_freq_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.freq_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_freq_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_freq_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.amp_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_amp_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_amp_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.amp_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_amp_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_amp_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.amp_modulation_env_num",
		_ENV_1,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_amp_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_amp_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.amp_modulation_env_level",
		100,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_amp_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_pad_synth_amp_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.quality",
		_PAD_QUALITY_128K,
		_PAD_QUALITY_1024K,
		_PAD_QUALITY_32K,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_quality_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.base_note",
		_PAD_BASE_NOTE_C3,
		_PAD_BASE_NOTE_G6,
		_PAD_BASE_NOTE_C2,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_base_note_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.base_width",
		30,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_base_width_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.shape",
		_PAD_SHAPE_RECTANGULAR,
		_PAD_SHAPE_DOUBLE_EXP,
		_PAD_SHAPE_RECTANGULAR,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_shape_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.shape_cutoff",
		_PAD_SHAPE_CUTOFF_FULL,
		_PAD_SHAPE_CUTOFF_LOWER,
		_PAD_SHAPE_CUTOFF_FULL,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_shape_cutoff_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_0",
		100,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_0_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_1",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_1_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_2",
		30,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_2_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_3",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_3_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_4",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_4_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_5",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_5_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_6",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_6_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_7",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_7_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_8",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_8_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_level_9",
		0,
		100,
		0, _ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_level_9_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.pad_synth.harmonies_detune",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_pad_synth_harmonies_detune_cb,
		0,
		num_of_voices - 1,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);
	
	return res;
}
