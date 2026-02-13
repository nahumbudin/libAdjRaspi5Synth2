/**
*	@file		LibAPI_getKeyboardParams.cpp
*	@author		Nahum Budin
*	@date		29-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active Keyboard settings parameters
*
*	History:	
*				version 1.2	15-Oct-2024	Rename patch to preset parameters.
*				version 1.1	9-Feb-2021	Code refactoring and notaion.
*				version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_kbd;
_settings_int_param_t int_param_kbd;
_settings_bool_param_t bool_param_kbd;


bool mod_synth_get_active_keyboard_portamento_enabled_state() 
{
	res_kbd = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.keyboard.portamento_state",
		&bool_param_kbd);
	if (res_kbd == _SETTINGS_KEY_FOUND)
	{
		return bool_param_kbd.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_keyboard_portamento_level() 
{
	res_kbd = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.keyboard.portamento",
		&int_param_kbd);
	if (res_kbd == _SETTINGS_KEY_FOUND)
	{
		return int_param_kbd.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_keyboard_sensitivity() 
{
	res_kbd = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.keyboard.sensetivity",
		&int_param_kbd);
	if (res_kbd == _SETTINGS_KEY_FOUND)
	{
		return int_param_kbd.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_keyboard_sensitivity_low() 
{
	res_kbd = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.keyboard.sensetivity_low",
		&int_param_kbd);
	if (res_kbd == _SETTINGS_KEY_FOUND)
	{
		return int_param_kbd.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_keyboard_poly_mode() 
{
	res_kbd = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.keyboard.polyphonic_mode",
		&int_param_kbd);
	if (res_kbd == _SETTINGS_KEY_FOUND)
	{
		return int_param_kbd.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_keyboard_split_point()
{
	res_kbd = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.keyboard.split_point",
		&int_param_kbd);
	if (res_kbd == _SETTINGS_KEY_FOUND)
	{
		return int_param_kbd.value;
	}
	else
	{
		return 0;
	}
}
