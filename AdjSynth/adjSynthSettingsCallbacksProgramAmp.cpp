/**
 *	@file		adjSynthSettingsCallbacksProgramAmp.cpp
 *	@author		Nahum Budin
 *	@date		9-Oct-2025
 *	@version	1.0 - Initial
 *
 *	@brief		Callback to handle a program Amp Settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceAmp.cpp ver1.3
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_amp_ch_1_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_amp_ch_1_pan_cb(int pan, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_amp_ch_1_pan_modulation_lfo_num_cb(int num, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_amp_ch_1_pan_modulation_lfo_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}

int set_program_amp_fixed_levels_state_cb(bool en, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		ModSynth::settings_handler_mutex.lock();
	}

	return res;
}
