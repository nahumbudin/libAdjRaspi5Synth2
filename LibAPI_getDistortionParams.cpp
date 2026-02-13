/**
*	@file		LibAPI_getDistortionParams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active Distortion settings parameters
*
*	History: 
*				version 1.2	14-Oct-2024	Code refactoring and notaion.
*				version	1.1	9-Feb-2021	Code refactoring and notaion.
*	`			version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_dist;
_settings_int_param_t int_param_dist;
_settings_bool_param_t bool_param_dist;

bool mod_synth_get_active_distortion_enable_state() 
{
	res_dist = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion.enabled",
		&bool_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return bool_param_dist.value;
	}
	else
	{
		return false;
	}
}

bool mod_synth_get_active_distortion_auto_gain_state() 
{
	res_dist = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion.auto_gain_enabled",
		&bool_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return bool_param_dist.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_distortion_1_drive() 
{
	res_dist = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion_1.drive",
		&int_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return int_param_dist.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_distortion_1_range() 
{
	res_dist = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion_1.range",
		&int_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return int_param_dist.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_distortion_1_blend() 
{
	res_dist = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion_1.blend",
		&int_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return int_param_dist.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_distortion_2_drive()
{
	res_dist = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion_2.drive",
		&int_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return int_param_dist.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_distortion_2_range()
{
	res_dist = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion_2.range",
		&int_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return int_param_dist.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_distortion_2_blend()
{
	res_dist = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.distortion_2.blend",
		&int_param_dist);
	if (res_dist == _SETTINGS_KEY_FOUND)
	{
		return int_param_dist.value;
	}
	else
	{
		return 0;
	}
}
