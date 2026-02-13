/**
 *	@file		adjSynthDefaultPresetParamsOut.cpp
 *	@author		Nahum Budin
 *	@date		10-Oct-2025
 *	@version	1.0
 *					1. Initial version.
 *
 *	@brief		Set default patch DSP Output Amp parameters
 *
 *	History:\n
 *		First version
 *
 */

#include "../Settings/settings.h"
#include "adjSynth.h"                                                                                  
	
int AdjSynth::set_default_preset_parameters_output(_settings_params_t *params, int prog)
{
	int res;
	
	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_gain_1",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_gain_1_cb,
		0, 	
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);
	
	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_gain_2",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_gain_2_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_pan_1",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_pan_1_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_pan_2",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_pan_2_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_send_1",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_send_1_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_send_2",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_send_2_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);
	
	return res;

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_pan_lfo_1",
		0,
		_NUM_OF_LFOS,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_pan_lfo_1_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_pan_lfo_2",
		0,
		_NUM_OF_LFOS,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_pan_lfo_2_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_pan_lfo_level_1",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_pan_lfo_level_1_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.voice_pan_lfo_level_2",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_out_pan_lfo_level_2_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		prog);
}

