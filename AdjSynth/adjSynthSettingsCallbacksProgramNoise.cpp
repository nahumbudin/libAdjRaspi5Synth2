/**
 *	@file		adjSynthSettingsCallbacksProgramNoise.cpp
 *	@author		Nahum Budin
 *	@date		8-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program Noise Settings
 *	settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceNoise.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_noise_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
		int res = -1;

		if (synth_program != NULL)
		{
			ModSynth::settings_handler_mutex.unlock();

			res = synth_program->program_settings_manager->set_bool_param_value(
				synth_program->get_active_program_preset_params(),
				"adjsynth.noise.enabled",
				enable,
				_SET_VALUE,
				prog);

			ModSynth::settings_handler_mutex.lock();
		}		
	}

	return res;
}

int set_program_noise_color_cb(int col, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.noise.color",
			col,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();		
	}

	return res;
}

int set_program_noise_send_filter_1_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.noise.send_filter_1",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_noise_send_filter_2_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.noise.send_filter_2",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_noise_amp_modulation_lfo_num_cb(int lfon, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.noise.amp_modulation_lfo_num",
			lfon,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_noise_amp_modulation_lfo_level_cb(int lfolev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.noise.amp_modulation_lfo_level",
			lfolev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_noise_amp_modulation_env_num_cb(int envn, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.noise.amp_modulation_env_num",
			envn,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_noise_amp_modulation_env_level_cb(int envlev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.noise.amp_modulation_env_level",
			envlev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}
