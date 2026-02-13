/**
*	@file		adjSynthDefaultPresetParamsAmp.cpp
*	@author		Nahum Budin
*	@date		24-Sep-2025
*	@version	1.2
*					1. Rename patch to preset parameters.
*					2. Bug fix: set amp1 level key "adjsynth.amp_ch1.level_cb" to "adjsynth.amp_ch1.level"
*					3. Amp is a common resource*: set only voice 0 (Dummy)
*						* Set fix Amp is program related
*					4. Level Pan, and Send are Master settings and not program associated - not a block callback
*						
*	@brief		Set default patch Amps parameters
*
*	History:\n
*	
*		version 1.1		4-Oct-2024:
*		version 1.0		15-Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"
#include "../Settings/settings.h"

int AdjSynth::set_default_preset_parameters_amp(_settings_params_t *params, int prog)
{
	int res;
	res = adj_synth_settings_manager->set_int_param(params,
		"adjsynth.amp_ch1.level",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL, //set_amp_gain_1_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK, // _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch1.pan",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		0,
		set_voice_block_amp_ch_1_pan_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch1.send",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_amp_ch_1_send_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch1.pan_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		0,
		set_voice_block_amp_ch_1_pan_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch1.pan_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		0,
		set_voice_block_amp_ch_1_pan_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch2.level",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_amp_gain_2_cb,
		0,
		0,
		set_voice_block_amp_ch_2_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch2.pan",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		0,
		set_voice_block_amp_ch_2_pan_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch2.send",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_amp_ch_2_send_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch2.pan_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		0,
		set_voice_block_amp_ch_2_pan_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.amp_ch2.pan_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		0,
		set_voice_block_amp_ch_2_pan_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.amp.fixed_levels_enabled",
		true,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		0,
		set_voice_block_amp_fixed_levels_state_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);
	
	return res;
}