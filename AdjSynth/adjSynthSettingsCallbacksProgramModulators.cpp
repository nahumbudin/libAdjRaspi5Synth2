/**
 *	@file		adjSynthSettingsCallbacksOrogramModulators.cpp
 *	@author		Nahum Budin
 *	@date		7-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program Modulators Settings
 *	settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceModulators.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_lfo_1_waveform_cb(int wavf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_1.waveform",
			wavf,
			_SET_VALUE, // Only set - activation will be done whem assigned to program
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_1_rate_cb(int rate, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_1.rate",
			rate,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_1_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_1.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_2_waveform_cb(int wavf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_2.waveform",
			wavf,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_2_rate_cb(int rate, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_2.rate",
			rate,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_2_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_2.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_3_waveform_cb(int wavf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_3.waveform",
			wavf,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_3_rate_cb(int rate, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_3.rate",
			rate,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_3_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_3.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_4_waveform_cb(int wavf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_4.waveform",
			wavf,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_4_rate_cb(int rate, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_4.rate",
			rate,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_4_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_4.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_5_waveform_cb(int wavf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_5.waveform",
			wavf,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_5_rate_cb(int rate, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_5.rate",
			rate,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_5_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_5.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_6_waveform_cb(int wavf, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_6.waveform",
			wavf,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_6_rate_cb(int rate, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_6.rate",
			rate,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_lfo_6_symmetry_cb(int sym, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.lfo_6.symmetry",
			sym,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}


int set_program_env_1_attack_cb(int attck, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_1.attack",
			attck,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_1_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_1.decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_1_sustain_cb(int sus, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_1.sustain",
			sus,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_1_release_cb(int rel, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();
		
		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_1.release",
			rel,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_2_attack_cb(int attck, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_2.attack",
			attck,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_2_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_2.decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_2_sustain_cb(int sus, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_2.sustain",
			sus,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_2_release_cb(int rel, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_2.release",
			rel,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_3_attack_cb(int attck, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_3.attack",
			attck,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_3_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_3.decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_3_sustain_cb(int sus, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_3.sustain",
			sus,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_3_release_cb(int rel, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_3.release",
			rel,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_4_attack_cb(int attck, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_4.attack",
			attck,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_4_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_4.decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_4_sustain_cb(int sus, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_4.sustain",
			sus,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_4_release_cb(int rel, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_4.release",
			rel,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_5_attack_cb(int attck, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_5.attack",
			attck,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_5_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_5.decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_5_sustain_cb(int sus, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_5.sustain",
			sus,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_5_release_cb(int rel, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_5.release",
			rel,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_6_attack_cb(int attck, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_6.attack",
			attck,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_6_decay_cb(int dec, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_6.decay",
			dec,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_6_sustain_cb(int sus, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_6.sustain",
			sus,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_env_6_release_cb(int rel, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.env_6.release",
			rel,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}


