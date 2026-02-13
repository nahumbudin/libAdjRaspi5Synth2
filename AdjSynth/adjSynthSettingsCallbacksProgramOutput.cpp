/**
 *	@file		adjSynthSettingsCallbacksProgramVCO.cpp
 *	@author		Nahum Budin
 *	@date		10-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program DSP Output Amp Settings
 *	settings
 *
 *	History:\n
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_out_gain_1_cb(int gain, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.gain_1",
			gain,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;	
}

int set_program_out_pan_1_cb(int pan, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.pan_1",
			pan,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_send_1_cb(int send, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.send_1",
			send,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_pan_lfo_1_cb(int lfo, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.lfo_1",
			lfo,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_pan_lfo_level_1_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.lfo_level_1",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_gain_2_cb(int gain, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.gain_2",
			gain,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_pan_2_cb(int pan, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.pan_2",
			pan,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_send_2_cb(int send, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.send_2",
			send,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_pan_lfo_2_cb(int lfo, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_out.lfo_2",
			lfo,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_out_pan_lfo_level_2_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.voice_lfo_out.level_2",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}
