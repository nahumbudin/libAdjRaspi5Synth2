/**
*	@file		adjSynthDefaultSettingsPolyphonicMixer.cpp
*	@author		Nahum Budin
*	@date		31-Jan-2026
*	@version	1.0
*					1st release.
*	
*	@brief		Set default settings Polyphonic Mixer parameters
*
*	History:\n
*		
*/

#include "adjSynth.h"
#include "../Settings/settings.h"

int AdjSynth::set_default_preset_parameters_polyphonic_mixer(_settings_params_t *params, int prog)
{
	int res;
	
	res = adj_synth_settings_manager->set_int_param(params,
		"adjsynth.poly_mixer.level",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_poly_mixer_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);
	
	res |= adj_synth_settings_manager->set_int_param(params,
		"adjsynth.poly_mixer.send",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_poly_mixer_send_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);
	
	res |= adj_synth_settings_manager->set_int_param(params,
		"adjsynth.poly_mixer.pan",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_poly_mixer_pan_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);
	
	res |= adj_synth_settings_manager->set_int_param(params,
		"adjsynth.poly_mixer.pan_lfo_num",
		_LFO_NONE,
		_NUM_OF_LFOS,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_poly_mixer_pan_lfo_num_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);
	
	res |= adj_synth_settings_manager->set_int_param(params,
		"adjsynth.poly_mixer.pan_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_poly_mixer_pan_lfo_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		prog);
	
	return res;
}