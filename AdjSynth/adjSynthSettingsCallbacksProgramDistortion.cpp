/**
 *	@file		adjSynthSettingsCallbacksProgramDistortion.cpp
 *	@author		Nahum Budin
 *	@date		19-Jan-2026
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program Distortion Settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceAmp.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_distortion_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion.enabled",
			enable,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_distortion_auto_gain_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion.auto_gain_enabled",
			enable,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_distortion_1_drive_cb(int drv, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion_1.drive",
			drv,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_distortion_1_range_cb(int rng, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion_1.range",
			rng,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_distortion_1_blend_cb(int blnd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion_1.blend",
			blnd,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_distortion_2_drive_cb(int drv, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion_2.drive",
			drv,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_distortion_2_range_cb(int rng, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion_2.range",
			rng,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_distortion_2_blend_cb(int blnd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.distortion_2.blend",
			blnd,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}
