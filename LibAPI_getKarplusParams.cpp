/**
*	@file		LibAPI_getKarplusParams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active Karplus String Generator settings parameters
*
*	History: 
*				version 1.2	15-Oct-2024	Code refactoring and rename patch to preset parameters.
*				version	1.1	Code refactoring and notaion.
*				version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_kps;
_settings_int_param_t int_param_kps;
_settings_bool_param_t bool_param_kps;

bool mod_synth_get_active_karplus_enable_state()
{
	res_kps = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.enabled",
		&bool_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return bool_param_kps.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_karplus_excitation_waveform_type()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.excitation_waveform_type",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_excitation_waveform_variations()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.excitation_waveform_variations",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_string_damping_calculation_mode()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.string_damping_calculation_mode",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_string_damping()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.string_damping",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_pluck_damping()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.pluck_damping",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}

}

int mod_synth_get_active_karplus_string_on_decay()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.on_decay",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_string_off_decay()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.off_decay",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_string_damping_variation()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.string_damping_variations",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_pluck_damping_variation()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.pluck_damping_variations",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_send_filter_1()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.send_filter_1",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_karplus_send_filter_2()
{
	res_kps = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.karplus_synth.send_filter_2",
		&int_param_kps);
	if (res_kps == _SETTINGS_KEY_FOUND)
	{
		return int_param_kps.value;
	}
	else
	{
		return 0;
	}
}
