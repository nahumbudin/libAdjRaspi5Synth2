/**
 *	@file		adjSynthSettingsCallbacksProgramKPS.cpp
 *	@author		Nahum Budin
 *	@date		8-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program Karplus String Synth Settings
 *	settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceKPS.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_karplus_synth_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.enabled",
			enable,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_excitation_waveform_type_cb(int type, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.excitation_waveform_type",
			type,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_excitation_waveform_variations_cb(int var, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.excitation_waveform_variations",
			var,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_pluck_damping_cb(int dump, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.pluck_damping",
			dump,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_pluck_damping_variations_cb(int dump, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.pluck_damping_variations",
			dump,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_string_damping_cb(int dump, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.string_damping",
			dump,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_string_damping_variations_cb(int dump, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.string_damping_variations",
			dump,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_string_damping_calculation_mode_cb(int mode, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.string_damping_calculation_mode",
			mode,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_send_filter_1_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.send_filter_1",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_send_filter_2_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.send_filter_2",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_on_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.on_decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_karplus_synth_off_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.karplus_synth.off_decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}
