/**
*	@file		LibAPI_getFiltersParams.cpp
*	@author		Nahum Budin
*	@date		29-Sep-2025
*	@version	1.2
*					1. Rename patch to preset parameters.
*
*	@brief		Get active Filters settings parameters
*
*	History: 
*			version 1.1		9-Feb-2021	Code refactoring and notaion.	
*			version 1.0		5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_filter;
_settings_int_param_t int_param_filter;


int mod_synth_get_active_filter_1_freq() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.frequency",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_oct() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.octave",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_q() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.q",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_kbd_track() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.keyboard_track",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_band() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.band",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_Freq_mod_lfo() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.freq_modulation_lfo_num",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_Freq_mod_lfo_level() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.freq_modulation_lfo_level",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_Freq_mod_env() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.freq_modulation_env_num",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_1_Freq_mod_env_level() 
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter1.freq_modulation_env_level",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_filter_2_freq()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.frequency",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_oct()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.octave",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_q()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.q",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_kbd_track()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.keyboard_track",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_band()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.band",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_Freq_mod_lfo()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.freq_modulation_lfo_num",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_Freq_mod_lfo_level()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.freq_modulation_lfo_level",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_Freq_mod_env()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.freq_modulation_env_num",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_filter_2_Freq_mod_env_level()
{
	res_filter = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.filter2.freq_modulation_env_level",
		&int_param_filter);
	if (res_filter == _SETTINGS_KEY_FOUND)
	{
		return int_param_filter.value;
	}
	else
	{
		return 0;
	}
}
