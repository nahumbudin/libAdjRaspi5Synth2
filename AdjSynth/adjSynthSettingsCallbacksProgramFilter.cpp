/**
 *	@file		adjSynthSettingsCallbacksProgramFilter.cpp
 *	@author		Nahum Budin
 *	@date		28-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program Filters Settings
 *	settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceFilter.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_filter_1_frequency_cb(int freq, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.frequency",
			freq,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_filter_1_octave_cb(int oct, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.octave",
			oct,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_filter_1_q_cb(int q, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.q",
			q,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	return res;
}

int set_program_filter_1_kbd_track_cb(int kbdt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.keyboard_track",
			kbdt,
			_SET_VALUE,
			prog);
		ModSynth::settings_handler_mutex.lock();
	}
	return res;
}

int set_program_filter_1_band_cb(int bnd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.band",
			bnd,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_1_freq_modulation_lfo_num_cb(int lfo, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.freq_modulation_lfo_num",
			lfo,
			_SET_VALUE,
			prog);
		ModSynth::settings_handler_mutex.lock();
	}
	return res;
}

int set_program_filter_1_freq_modulation_lfo_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.freq_modulation_lfo_level",
			lev,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_1_freq_modulation_env_num_cb(int env, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.freq_modulation_env_num",
			env,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_1_freq_modulation_env_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter1.freq_modulation_env_level",
			lev,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_frequency_cb(int freq, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.frequency",
			freq,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_filter_2_octave_cb(int oct, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.octave",
			oct,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_q_cb(int q, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.q",
			q,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_kbd_track_cb(int kbdt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.keyboard_track",
			kbdt,
			_SET_VALUE,
			prog);
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_band_cb(int bnd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.band",
			bnd,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_freq_modulation_lfo_num_cb(int lfo, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.freq_modulation_lfo_num",
			lfo,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_freq_modulation_lfo_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.freq_modulation_lfo_level",
			lev,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_freq_modulation_env_num_cb(int env, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.freq_modulation_env_num",
			env,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}

int set_program_filter_2_freq_modulation_env_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	
	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.filter2.freq_modulation_env_level",
			lev,
			_SET_VALUE,
			prog);
		
		ModSynth::settings_handler_mutex.lock();
	}
	
	return res;
}


