/**
 *	@file		adjSynthSettingsCallbacksProgramPAD.cpp
 *	@author		Nahum Budin
 *	@date		12-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program PAD Synth Settings
 *	settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoicePAD.cpp ver1.2
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_pad_synth_enabled_cb(bool enable, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_bool_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.enabled",
			enable,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_detune_octave_cb(int oct, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.tune_offset_octave",
			oct,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_detune_semitones_cb(int semt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.tune_offset_semitones",
			semt,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_detune_cents_cb(int cnts, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.tune_offset_cents",
			cnts,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_send_filter_1_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.send_filter_1",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_send_filter_2_cb(int snd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.send_filter_2",
			snd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_freq_modulation_lfo_num_cb(int lfo, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.freq_modulation_lfo_num",
			lfo,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_freq_modulation_lfo_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.freq_modulation_lfo_level",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_freq_modulation_env_num_cb(int env, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.freq_modulation_env_num",
			env,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_freq_modulation_env_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.freq_modulation_env_level",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_amp_modulation_lfo_num_cb(int lfo, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.amp_modulation_lfo_num",
			lfo,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_amp_modulation_lfo_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.amp_modulation_lfo_level",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_amp_modulation_env_num_cb(int env, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.amp_modulation_env_num",
			env,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_amp_modulation_env_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.amp_modulation_env_level",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_pad_synth_quality_cb(int qlt, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.quality",
			qlt,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		// Don't just set the params that will be assign to new voices.
		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_wavetable_length(
			AdjSynth::get_instance()->synth_program[prog]->program_wavetable,
			qlt);
	}

	return res;
}

int set_program_pad_synth_base_note_cb(int bnot, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.base_note",
			bnot,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_base_note(
			AdjSynth::get_instance()->synth_program[prog]->program_wavetable,
			bnot);
	}

	return res;
}

int set_program_pad_synth_base_width_cb(int bwd, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.base_width",
			bwd,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_base_harmony_width(bwd);
	}

	return res;
}

int set_program_pad_synth_shape_cb(int shp, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.shape",
			shp,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_shape(shp);
	}

	return res;
}

int set_program_pad_synth_shape_cutoff_cb(int shcut, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.shape_cutoff",
			shcut,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_shape_cutoff(shcut);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_0_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_0",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			0, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_1_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_1",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			1, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_2_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_2",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			2, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_3_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_3",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			3, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_4_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_4",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			4, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_5_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_5",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			5, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_6_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_6",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			6, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_7_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_7",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			7, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_8_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_8",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			8, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_level_9_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_level_9",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(
			9, (float)lev / 100.f);
	}

	return res;
}

int set_program_pad_synth_harmonies_detune_cb(int hdet, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.pad_synth.harmonies_detune",
			hdet,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmonies_detune(
			(float)hdet / 100.f);
	}

	return res;
}
