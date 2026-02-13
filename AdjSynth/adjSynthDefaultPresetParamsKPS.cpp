/**
 *	@file		adjSynthDefaultPresetParamsKPS.cpp
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.1
 *					1. Rename patch to preset parameters.
 *					2. Adding default program settings callbacks
*	
*	@brief		Set default patch Karplus Strong String Generator parameters
*
*	History:\n
*	
*	version 1.1		4-Oct-2024	Code refactoring and notaion.
*	version 1.0		15-Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_preset_parameters_kps(_settings_params_t *params, int prog)
{
	int res;

	res = adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.karplus_synth.enabled",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_enabled_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_enabled_cb,
		_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.excitation_waveform_type",
		_KARPLUS_STRONG_EXCITATION_WHITE_NOISE,
		_KARPLUS_STRONG_EXCITATION_SQUARE_WAVE,
		_KARPLUS_STRONG_EXCITATION_WHITE_NOISE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_excitation_waveform_type_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_excitation_waveform_type_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.excitation_waveform_variations",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_excitation_waveform_variations_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_excitation_waveform_variations_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.decay",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_decay_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.pluck_damping",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_pluck_damping_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_pluck_damping_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.pluck_damping_variations",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_pluck_damping_variations_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_pluck_damping_variations_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.string_damping",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_string_damping_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_string_damping_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.string_damping_variations",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_string_damping_variations_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_string_damping_variations_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.string_damping_calculation_mode",
		_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT,
		_KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC,
		_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_string_damping_calculation_mode_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_string_damping_calculation_mode_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.send_filter_1",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_send_filter_1_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_send_filter_1_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.send_filter_2",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_send_filter_2_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_send_filter_2_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.on_decay",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_on_decay_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_on_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.karplus_synth.off_decay",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_karplus_synth_off_decay_cb,
		0,
		num_of_voices - 1,
		set_voice_block_karplus_synth_off_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);
	
	return res;
}
