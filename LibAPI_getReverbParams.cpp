/**
*	@file		LibAPI_getReverbParams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active Reverb settings parameters
*
*	History: 
*				version 1.2	15-Oct-2024 Code refactoring and notaion.
*				version	1.1	9-Feb-2021
*				version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_rev;
_settings_int_param_t int_param_rev;
_settings_bool_param_t bool_param_rev;

bool mod_synth_get_active_reverb_enable_state() 
{
	res_rev = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb.enable_state",
		&bool_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return bool_param_rev.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_reverb_room_size() 
{
	res_rev = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb.room_size",
		&int_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return int_param_rev.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_reverb_damp() 
{
	res_rev = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb.damp",
		&int_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return int_param_rev.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_reverb_wet() 
{
	res_rev = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb.wet",
		&int_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return int_param_rev.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_reverb_dry() 
{
	res_rev = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb.dry",
		&int_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return int_param_rev.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_reverb_width() 
{
	res_rev = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb.width",
		&int_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return int_param_rev.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_reverb_mode() 
{
	res_rev = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb.mode",
		&int_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return int_param_rev.value;
	}
	else
	{
		return 0;
	}
}

bool mod_synth_get_active_reverb_3m_enable_state() 
{
	res_rev = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb3m.enable_state",
		&bool_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return bool_param_rev.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_reverb_3m_preset() 
{
	res_rev = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.reverb3m.preset",
		&int_param_rev);
	if (res_rev == _SETTINGS_KEY_FOUND)
	{
		return int_param_rev.value;
	}
	else
	{
		return 0;
	}
}

