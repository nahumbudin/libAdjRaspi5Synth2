/**
 *	@file		adjSynthPresetPatchParamsMSO.cpp
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.2
 *					1. Code refactoring rename modules to instruments.
 *					2. Adding default program settings callbacks
*	
*	@brief		Set default patch Morphed Sine Generator parameters
*
*	History:\n
*	
*	version 1.1		4-Oct-2024	Code refactoring and notaion.
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_preset_parameters_mso(_settings_params_t *params, int prog)
{
	int res;

	res = adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.mso_synth.enabled",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_enabled_cb,
		0,
		0,
		set_voice_block_mso_synth_enabled_cb,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		prog);

	res = adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.symmetry",
		70,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_symmetry_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.send_filter_1",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_send_filter_1_cb,
		0,
		0,
		set_voice_block_mso_synth_send_filter_1_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.send_filter_2",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_send_filter_2_cb,
		0,
		0,
		set_voice_block_mso_synth_send_filter_2_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.tune_offset_oct",
		0,
		_OSC_DETUNE_MAX_OCTAVE,
		_OSC_DETUNE_MIN_OCTAVE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_tune_offset_oct_cb,
		0,
		0,
		set_voice_block_mso_synth_tune_offset_oct_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.tune_offset_semitones",
		0,
		_OSC_DETUNE_MAX_SEMITONES,
		_OSC_DETUNE_MIN_SEMITONES,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_tune_offset_semitones_cb,
		0,
		0,
		set_voice_block_mso_synth_tune_offset_semitones_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.tune_offset_cents",
		0,
		_OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_tune_offset_cents_cb,
		0,
		0,
		set_voice_block_mso_synth_tune_offset_cents_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.freq_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_freq_modulation_lfo_num_cb,
		0,
		0,
		set_voice_block_mso_synth_freq_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.freq_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_freq_modulation_lfo_level_cb,
		0,
		0,
		set_voice_block_mso_synth_freq_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.freq_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_freq_modulation_env_num_cb,
		0,
		0,
		set_voice_block_mso_synth_freq_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.freq_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_freq_modulation_env_level_cb,
		0,
		0,
		set_voice_block_mso_synth_freq_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.pwm_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_pwm_modulation_lfo_num_cb,
		0,
		0,
		set_voice_block_mso_synth_pwm_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.pwm_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_pwm_modulation_lfo_level_cb,
		0,
		0,
		set_voice_block_mso_synth_pwm_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.pwm_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_pwm_modulation_env_num_cb,
		0,
		0,
		set_voice_block_mso_synth_pwm_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.pwm_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_pwm_modulation_env_level_cb,
		0,
		0,
		set_voice_block_mso_synth_pwm_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.amp_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_amp_modulation_lfo_num_cb,
		0,
		0,
		set_voice_block_mso_synth_amp_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.amp_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_amp_modulation_lfo_level_cb,
		0,
		0,
		set_voice_block_mso_synth_amp_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.amp_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_amp_modulation_env_num_cb,
		0,
		0,
		set_voice_block_mso_synth_amp_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.amp_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_amp_modulation_env_level_cb,
		0,
		0,
		set_voice_block_mso_synth_amp_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.segment_position_a",
		512,
		2047,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_segment_position_a_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.segment_position_b",
		513,
		2047,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_segment_position_b_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.segment_position_c",
		1024,
		2047,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_segment_position_c_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.segment_position_d",
		1025,
		2047,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_segment_position_d_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.segment_position_e",
		1536,
		2047,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_segment_position_e_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.mso_synth.segment_position_f",
		1537,
		2047,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_mso_synth_segment_position_f_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	/*	res |= adj_synth_settings_manager->set_int_param
		(params,
			"adjsynth.mso_synth.preset",
			1, // sine
			4, // saw TODO:
			0,
			_ADJ_SYNTH_PATCH_PARAMS,
			set_mso_synth_segment_preset,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
			prog);
			*/
	
	return res;
}