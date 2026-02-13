/**
*	@file		adjSynthDefaultSettingsReverb.cpp
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.3
*					1. Code refactoring and notaion.
*	
*	@brief		Set default settings Reverb parameters
*
*	History:\n
*	
*	version 1.2		5-Feb-2021	Code refactoring and notaion
*	version 1.1		5-Dec-2019
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Set the settings Reverb parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_settings_parameters_reverb(_settings_params_t *params)
{
	int res = 0;
	
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = adj_synth_settings_manager->set_bool_param
				(params,
		"adjsynth.reverb3m.enable_state",
		false,
		"instrument_settings_param",
		set_reverb3m_enable_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1); // no program (common resource)
	
	res |= adj_synth_settings_manager->set_bool_param
				(params,
		"adjsynth.reverb.enable_state",
		false,
		"instrument_settings_param",
		set_reverb_enable_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.reverb3m.preset",
		_SF_REVERB_PRESET_DEFAULT,
		_SF_REVERB_PRESET_LONGREVERB2,
		_SF_REVERB_PRESET_DEFAULT,
		"instrument_settings_param",
		set_reverb3m_preset_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.reverb.room_size",
		50,
		100,
		0,
		"instrument_settings_param",
		set_reverb_room_size_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.reverb.damp",
		50,
		100,
		0,
		"instrument_settings_param",
		set_reverb_damp_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.reverb.wet",
		50,
		100,
		0,
		"instrument_settings_param",
		set_reverb_wet_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.reverb.dry",
		50,
		100,
		0,
		"instrument_settings_param",
		set_reverb_dry_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.reverb.mode",
		0,
		100,
		0,
		"instrument_settings_param",
		set_reverb_mode_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
		
	return res;
}
