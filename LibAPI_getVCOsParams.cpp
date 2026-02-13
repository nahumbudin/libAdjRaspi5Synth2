/**
*	@file		LibAPI_getVCOsParams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active OSC1 and OSC2 settings parameters
*
*	History: 
*				version 1.2	15-Oct-2024	Code refactoring and notaion
*				version	1.1	9-Feb-2021	Code refactoring and notaion
*				version 1.0	5-Oct-2019	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int res_osc;
_settings_int_param_t int_param_osc;
_settings_bool_param_t bool_param_osc;

bool mod_synth_get_active_osc1_enable_state()
{
	res_osc = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.enabled",
		&bool_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return bool_param_osc.value;
	}
	else
	{
		return false;
	}
}

bool mod_synth_get_active_osc2_enable_state()
{
	res_osc = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.enabled",
		&bool_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return bool_param_osc.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_osc1_waveform()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.waveform",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_pwm_percents()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.symmetry",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_detune_octave()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.tune_offset_oct",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_detune_semitones()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.tune_offset_semitones",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_detune_cents()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.tune_offset_cents",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_send_filter_1()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.send_filter_1",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_send_filter_2()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.send_filter_2",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_unison_mode()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.unison_mode",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_hammond_mode()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.hammond_percussion_mode",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_harmonies_levels(int harnum)
{
	switch (harnum)
	{
	case 0:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_1",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 1:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_2",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 2:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_3",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 3:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_4",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 4:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_5",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 5:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_6",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 6:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_7",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 7:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_8",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	case 8:
		res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
			"adjsynth.osc1.unison_level_9",
			&int_param_osc);
		if (res_osc == _SETTINGS_KEY_FOUND)
		{
			return int_param_osc.value;
		}
		else
		{
			return 0;
		}

	default:
		return 0;
	}
}

int mod_synth_get_active_osc1_harmonies_detune_factor()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.unison_detune",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_harmonies_dist_factor()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.unison_distortion",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_osc1_freq_mod_lfo()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.freq_modulation_lfo_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_freq_mod_lfo_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.freq_modulation_lfo_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_freq_mod_env()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.freq_modulation_env_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_freq_mod_env_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.freq_modulation_env_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_pwm_mod_lfo()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.pwm_modulation_lfo_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_pwm_mod_lfo_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.pwm_modulation_lfo_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_pwm_mod_env()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.pwm_modulation_env_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}
int mod_synth_get_active_osc1_pwm_mod_env_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.pwm_modulation_env_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_amp_mod_lfo()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.amp_modulation_lfo_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_amp_mod_lfo_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.amp_modulation_lfo_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_amp_mod_env()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.amp_modulation_env_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc1_amp_mod_env_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.amp_modulation_env_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_waveform() {
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.waveform",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_pwm_percents()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.symmetry",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_detune_octave()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.tune_offset_oct",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_detune_semitones() {
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.tune_offset_semitones",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_detune_cents()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.tune_offset_cents",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_send_filter_1()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.send_filter_1",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_send_filter_2()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.send_filter_2",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

bool mod_synth_get_active_osc2_sync_is_on_state()
{
	res_osc = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.sync_on_osc_1",
		&bool_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return false;
	}
}
bool mod_synth_get_active_osc2_fix_tone_is_on_state() { return false; /* Synthesizer::getInstance()->getActivePatchParams()->osc2FixToneIsOn;*/ }

int mod_synth_get_active_osc2_freq_mod_lfo()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.freq_modulation_lfo_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_freq_mod_lfo_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.freq_modulation_lfo_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_freq_mod_env()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.freq_modulation_env_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_freq_mod_env_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.freq_modulation_env_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_pwm_mod_lfo()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.pwm_modulation_lfo_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_pwm_mod_lfo_level() {
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc1.pwm_modulation_lfo_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_pwm_mod_env()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.pwm_modulation_env_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_pwm_mod_env_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.pwm_modulation_env_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_amp_mod_lfo()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.amp_modulation_lfo_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_amp_mod_lfo_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.amp_modulation_lfo_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_amp_mod_env()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.amp_modulation_env_num",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_osc2_amp_mod_env_level()
{
	res_osc = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_preset_params(),
		"adjsynth.osc2.amp_modulation_env_level",
		&int_param_osc);
	if (res_osc == _SETTINGS_KEY_FOUND)
	{
		return int_param_osc.value;
	}
	else
	{
		return 0;
	}
}