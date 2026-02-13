/**
 *	@file		adjSynthSettingsCallbacksProgramMSO.cpp
 *	@author		Nahum Budin
 *	@date		1-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program MSO Synth Settings
 *	settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceMSO.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_mso_synth_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.enabled",
			enable,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		synth_program->mso_wtab->set_morphing_symetry(sym);
	}

	return res;
}

int set_program_mso_synth_tune_offset_oct_cb(int oct, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.tune_offset_oct",
			oct,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_tune_offset_semitones_cb(int semi, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.tune_offset_semitones",
			semi,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_tune_offset_cents_cb(int cnt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.tune_offset_cents",
			cnt,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_send_filter_1_cb(int send, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.send_filter_1",
			send,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_send_filter_2_cb(int send, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.send_filter_2",
			send,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_freq_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.freq_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_freq_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.freq_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_freq_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.freq_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_freq_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.freq_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_pwm_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.pwm_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_pwm_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.pwm_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_pwm_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.pwm_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_pwm_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.pwm_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_amp_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.amp_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_amp_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.amp_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_amp_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.amp_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_amp_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.amp_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_mso_synth_segment_position_a_cb(int pos, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.segment_position_a",
			pos,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		synth_program->mso_wtab->set_segment_position(
			pos_a, pos,
			&synth_program->mso_wtab->base_segment_positions);
	}

	return res;
}

int set_program_mso_synth_segment_position_b_cb(int pos, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.segment_position_b",
			pos,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		synth_program->mso_wtab->set_segment_position(
			pos_b, pos,
			&synth_program->mso_wtab->base_segment_positions);
	}

	return res;
}

int set_program_mso_synth_segment_position_c_cb(int pos, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.segment_position_c",
			pos,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		synth_program->mso_wtab->set_segment_position(
			pos_c, pos,
			&synth_program->mso_wtab->base_segment_positions);
	}

	return res;
}

int set_program_mso_synth_segment_position_d_cb(int pos, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.segment_position_d",
			pos,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		synth_program->mso_wtab->set_segment_position(
			pos_d, pos,
			&synth_program->mso_wtab->base_segment_positions);
	}

	return res;
}

int set_program_mso_synth_segment_position_e_cb(int pos, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.segment_position_e",
			pos,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		synth_program->mso_wtab->set_segment_position(
			pos_e, pos,
			&synth_program->mso_wtab->base_segment_positions);
	}

	return res;
}

int set_program_mso_synth_segment_position_f_cb(int pos, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.mso_synth.segment_position_f",
			pos,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		synth_program->mso_wtab->set_segment_position(
			pos_f, pos,
			&synth_program->mso_wtab->base_segment_positions);
	}

	return res;
}
