/**
 *	@file		adjSynthSettingsCallbacksProgramVCO.cpp
 *	@author		Nahum Budin
 *	@date		7-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program VCOs Settings
 *	settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceVCO.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_osc_1_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.enabled",
			enable,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_waveform_cb(int wvf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.waveform",
			wvf,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_pwm_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.symmetry",
			sym,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_send_filter_1_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		//ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.send_filter_1",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_send_filter_2_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.send_filter_2",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_tune_offset_oct_cb(int oct, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.tune_offset_oct",
			oct,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_tune_offset_semitones_cb(int smt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.tune_offset_semitones",
			smt,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_tune_offset_cents_cb(int cnt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.tune_offset_cents",
			cnt,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_freq_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.freq_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_freq_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.freq_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_freq_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.freq_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_freq_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.freq_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_pwm_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.pwm_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_pwm_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.pwm_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_pwm_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.pwm_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_pwm_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.pwm_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_amp_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.amp_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_amp_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.amp_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_amp_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.amp_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_amp_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.amp_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_mod_cb(int unimod, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_mode",
			unimod,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		mod_synth_callback_set_osc_1_unison_mode(unimod);
	}

	return res;
}

int set_program_osc_1_hammond_percussion_mode_cb(int pmode, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.hammond_percussion_mode",
			pmode,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_1_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_1",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_2_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_2",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_3_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_3",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_4_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_4",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_5_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_5",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_6_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_6",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_7_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_7",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_8_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_8",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_level_9_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_level_9",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_distortion_cb(int dist, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_distortion",
			dist,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_detune_cb(int det, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_detune",
			det,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_1_unison_set_square_cb(bool sqr, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc1.unison_square_wave",
			sqr,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.enabled",
			enable,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_waveform_cb(int wvf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.waveform",
			wvf,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_pwm_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_send_filter_1_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		//ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.send_filter_1",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_send_filter_2_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.send_filter_2",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_tune_offset_oct_cb(int oct, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.tune_offset_oct",
			oct,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_tune_offset_semitones_cb(int smt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.tune_offset_semitones",
			smt,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_tune_offset_cents_cb(int cnt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.tune_offset_cents",
			cnt,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_freq_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.freq_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_freq_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.freq_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_freq_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.freq_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_freq_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.freq_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_pwm_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.pwm_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_pwm_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.pwm_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_pwm_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.pwm_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_pwm_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.pwm_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_amp_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.amp_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_amp_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.amp_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_amp_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.amp_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_amp_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.amp_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_osc_2_sync_on_osc1_cb(bool sync, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.osc2.sync_on_osc_1",
			sync,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}
