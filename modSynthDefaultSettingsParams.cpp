/**
*	@file		modSynthDefaultSettingsParams.cpp
*	@author		Nahum Budin
*	@date		13_oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		Set default ModSynth settings parameters
*
*	History:\n
*	
*		version	1.1	7-Dec-2019
*		version 1.0	15_Nov-2019	First version
*		
*/


#include "modSynth.h"

/**
*   @brief  Set default ModSynth settings parameters
*   @param	params	a pointer to a _setting_params_t struct holding the paramaters
*   @return 0 if done OK.
*/
int ModSynth::set_adj_synth_default_settings(_settings_params_t *params)
{
	int res = 0;
	
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);

	res = AdjSynth::get_instance()->set_default_settings_parameters_mixer(params);
	res |= AdjSynth::get_instance()->set_default_settings_parameters_keyboard(params);
	res |= AdjSynth::get_instance()->set_default_settings_parameters_reverb(params);
	res |= AdjSynth::get_instance()->set_default_settings_parameters_equalizer(params);
	
	return res;
}

/**
*   @brief  Set default ModSynth general settings parameters
*   @param	params	a pointer to a _setting_params_t struct holding the paramaters
*   @return 0 if done OK.
*/
int ModSynth::set_default_general_settings_parameters(_settings_params_t *params)
{
	int res = 0;

	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);

	res = set_default_settings_parameters_audio(params);
	
	return res;
}