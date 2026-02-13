/**
 *	@file		adjSynthDefaultPresetParamsDistortion.cpp
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.2
 *					1. Rename patch to preset parameters.
 *					2. Adding default program settings callbacks
 *
 *	@brief		Set default patch Distortion parameters
 *
 *	History:\n
 *
 *	version 1.1		4-Oct-2024:	Code refactoring and notaion.
 *	version 1.0		15-Nov-2019:
 *		First version
 *
 */

#include "adjSynth.h"

int AdjSynth::set_default_preset_parameters_distortion(_settings_params_t *params, int prog)
{
	int res;

	res = adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.distortion.enabled",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_enabled_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_enabled_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.distortion.auto_gain_enabled",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_auto_gain_enabled_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_auto_gain_enabled_cb,
		_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.distortion_1.drive",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_1_drive_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_1_drive_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.distortion_1.range",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_1_range_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_1_range_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.distortion_1.blend",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_1_blend_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_1_blend_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.distortion_2.drive",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_2_drive_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_2_drive_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.distortion_2.range",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_2_range_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_2_range_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.distortion_2.blend",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_distortion_2_blend_cb,
		0,
		num_of_voices - 1,
		set_voice_block_distortion_2_blend_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);
	
	return res;
}