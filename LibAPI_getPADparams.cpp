/**
*	@file		LibAPI_getPADparams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters..
*
*	@brief		Get active PAD Synthesizer settings parameters
*
*	History: 
*				version 1.2	15-Oct-2024 Code refactoring and notaion.
*				version	1.1	9-Feb-2021
*				version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_pad;
_settings_int_param_t int_param_pad;
_settings_bool_param_t bool_param_pad;

bool mod_synth_get_active_pad_enable_state()
{
	res_pad = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.enabled",
		&bool_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return bool_param_pad.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_pad_detune_octave()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.tune_offset_octave",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_detune_semitones()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.tune_offset_semitones",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_detune_cents()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.tune_offset_cents",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_send_filter_1()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.send_filter_1",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_send_filter_2()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.send_filter_2",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_freq_mod_lfo()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.freq_modulation_lfo_num",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_freq_mod_lfo_level()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.freq_modulation_lfo_level",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_freq_mod_env()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.freq_modulation_env_num",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_freq_mod_env_level()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.freq_modulation_env_level",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_amp_mod_lfo()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.amp_modulation_lfo_num",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_amp_mod_lfo_level()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.amp_modulation_lfo_level",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_amp_mod_env()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.amp_modulation_env_num",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_amp_mod_env_level()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.amp_modulation_env_level",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_quality()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.quality",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_shape()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.shape",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_shape_cutoff()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.shape_cutoff",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_base_note()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.base_note",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_pad_base_width()
{
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.base_width",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_pad_harmony_level(int harmony)
{
	switch (harmony)
	{
	case 0:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_0",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 1:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_1",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 2:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_2",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 3:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_3",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 4:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_4",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 5:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_5",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 6:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_6",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 7:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_7",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 8:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_8",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	case 9:
		res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.pad_synth.harmonies_level_9",
			&int_param_pad);
		if (res_pad == _SETTINGS_KEY_FOUND)
		{
			return int_param_pad.value;
		}
		else
		{
			return 0;
		}

	default:
		return 0;
	}

	return 0;
}

int mod_synth_get_active_pad_harmony_detune() {
	res_pad = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.pad_synth.harmonies_detune",
		&int_param_pad);
	if (res_pad == _SETTINGS_KEY_FOUND)
	{
		return int_param_pad.value;
	}
	else
	{
		return 0;
	}
}
