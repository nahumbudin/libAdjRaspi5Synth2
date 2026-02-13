/**
*	@file		LibAPI_getEqualizerParams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active Equalizer settings parameters
*
*	History: 
*				version	1.2	15-Oct-2024	Code refactoring and notaion.
*				version	1.1	9-Feb-2021	Code refactoring and notaion.
*				version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_equi;
_settings_int_param_t int_param_equi;

int mod_synth_get_active_equilizer_band31_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_31_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band62_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_62_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band125_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_125_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band250_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_250_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band500_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_500_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band1k_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_1k_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band2k_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_2k_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band4k_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_4k_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band8k_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_8k_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_band16k_level() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.band_16k_level",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_equilizer_preset() 
{
	res_equi = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_settings_params(),
		"adjsynth.equilizer.preset",
		&int_param_equi);
	if (res_equi == _SETTINGS_KEY_FOUND)
	{
		return int_param_equi.value;
	}
	else
	{
		return 0;
	}
}

