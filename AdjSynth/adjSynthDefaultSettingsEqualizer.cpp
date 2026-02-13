/**
*	@file		adjSynthDefaultSettingsEqualizer.cpp
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion. (equi->equa)
*	
*	@brief		Set default settings Equilizer parameters
*
*	History:\n
*	
*	version 1.1	5-Feb-2021
*					1. Code refactoring and notaion. (equi->equa)
*			1.0	15_Nov-2019	First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Set the settings Equilizer parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_settings_parameters_equalizer(_settings_params_t *params)
{
	int res = 0;
	
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_31_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_31_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1); // -1: no program (common resource)
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_62_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_62_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_125_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_125_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_250_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_250_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_500_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_500_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_1k_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_1K_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_2k_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_2K_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_4k_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_4K_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_8k_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_8K_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.band_16k_level",
		0,
		50,
		-50,
		"instrument_settings_param",
		set_band_equalizer_band_16K_level_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
		"adjsynth.equalizer.preset",
		0,
		10,
		0,
		"instrument_settings_param",
		set_band_equalizer_preset_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	return res;
}
