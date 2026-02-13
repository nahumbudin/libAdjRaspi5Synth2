/**
*	@file		LibAPI_getMSOparams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active MSO settings parameters
*
	@History: 
*				version 1.2	15-Oct-2024	Rename patch to preset parameters.
*				version 1.1	9-Feb-2021
*				version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_mso;
_settings_int_param_t int_param_mso;
_settings_bool_param_t bool_param_mso;

bool mod_synth_get_active_mso_enable_state()
{
	res_mso = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.enabled",
		&bool_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return bool_param_mso.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_mso_table_segment_point(int point)
{
	switch (point)
	{
	case en_positions::pos_a:
		res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.mso_synth.segment_position_a",
			&int_param_mso);
		if (res_mso == _SETTINGS_KEY_FOUND)
		{
			return int_param_mso.value;
		}
		else
		{
			return 0;
		}

	case en_positions::pos_b:
		res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.mso_synth.segment_position_b",
			&int_param_mso);
		if (res_mso == _SETTINGS_KEY_FOUND)
		{
			return int_param_mso.value;
		}
		else
		{
			return 0;
		}

	case en_positions::pos_c:
		res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.mso_synth.segment_position_c",
			&int_param_mso);
		if (res_mso == _SETTINGS_KEY_FOUND)
		{
			return int_param_mso.value;
		}
		else
		{
			return 0;
		}

	case en_positions::pos_d:
		res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.mso_synth.segment_position_d",
			&int_param_mso);
		if (res_mso == _SETTINGS_KEY_FOUND)
		{
			return int_param_mso.value;
		}
		else
		{
			return 0;
		}

	case en_positions::pos_e:
		res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.mso_synth.segment_position_e",
			&int_param_mso);
		if (res_mso == _SETTINGS_KEY_FOUND)
		{
			return int_param_mso.value;
		}
		else
		{
			return 0;
		}

	case en_positions::pos_f:
		res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.mso_synth.segment_position_f",
			&int_param_mso);
		if (res_mso == _SETTINGS_KEY_FOUND)
		{
			return int_param_mso.value;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int mod_synth_get_active_mso_symetry()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.symmetry",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_mso_pwm_percents()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.symmetry",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_detune_octave()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.tune_offset_oct",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_detune_semitones()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.tune_offset_semitones",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_detune_cents()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.tune_offset_cents",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_send_filter_1()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.send_filter_1",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_send_filter_2()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.send_filter_2",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_mso_freq_mod_lfo()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.freq_modulation_lfo_num",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_freq_mod_lfo_level()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.freq_modulation_lfo_level",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_freq_mod_env()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.freq_modulation_env_num",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_freq_mod_env_level()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.freq_modulation_env_lev",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_pwm_mod_lfo()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.pwm_modulation_lfo_num",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_pwm_mod_lfo_level()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.pwm_modulation_lfo_level",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_pwm_mod_env()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.pwm_modulation_env_num",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_pwm_mod_env_level()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.pwm_modulation_env_level",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_amp_mod_lfo()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.amp_modulation_lfo_num",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_amp_mod_lfo_level()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.amp_modulation_lfo_level",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_amp_mod_env()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.amp_modulation_env_num",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_mso_amp_mod_env_level()
{
	res_mso = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.mso_synth.amp_modulation_env_level",
		&int_param_mso);
	if (res_mso == _SETTINGS_KEY_FOUND)
	{
		return int_param_mso.value;
	}
	else
	{
		return 0;
	}
}
