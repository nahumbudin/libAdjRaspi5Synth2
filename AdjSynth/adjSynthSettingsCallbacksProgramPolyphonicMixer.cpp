/**
 *	@file		adjSynthSettingsCallbacksProgramPolyphonicMixer.cpp
 *	@author		Nahum Budin
 *	@date		9-Oct-2025
 *	@version	1.0 - Initial
 *						1. Changes in the settings structure and settings handling, 
 *						to support program presets and online editing of program presets.
 *
 *	@brief		Callback to handle a program Polyphonic Mixer Settings
 *
 *	History:\n
 *
 *	Based on adjSynthSettingsBlockCallbacksVoiceAmp.cpp ver1.3
 *	
 *	Note: actual settings are set when a voice is allocated to the program!
 *		  Also update the active voices of the program!
 *
 */

#include "../AdjSynth/adjSynth.h"

int set_program_poly_mixer_level_cb(int level, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	int v;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.poly_mixer.level",
			level,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();
		
		// Update active voices of the program
		for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			if (synth_program->assigned_voices[v] == _VOICE_ASSIGNED)
			{
				AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_level(v, level / 100.0f);				
			}
		}
	}

	return res;
}

int set_program_poly_mixer_pan_cb(int pan, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	int v;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.poly_mixer.pan",
			pan,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		// Update active voices of the program
		for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			if (synth_program->assigned_voices[v] == _VOICE_ASSIGNED)
			{
				AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_pan(v, (float)(pan - 50) / 50.0f);
			}
		}
	}

	return res;
}

int set_program_poly_mixer_send_cb(int send, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	int v;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.poly_mixer.send",
			send,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		// Update active voices of the program
		for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			if (synth_program->assigned_voices[v] == _VOICE_ASSIGNED)
			{
				AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_send(v, send / 100.0f);
			}
		}
	}

	return res;
}

int set_program_poly_mixer_pan_lfo_num_cb(int lfo, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	int v;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.poly_mixer.pan_lfo_num",
			lfo,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		// Update active voices of the program
		for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			if (synth_program->assigned_voices[v] == _VOICE_ASSIGNED)
			{
				AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_pan_lfo(v, lfo);
			}
		}
	}

	return res;
}

int set_program_poly_mixer_pan_lfo_level_cb(int lev, int prog)
{
	AdjSynthPrograms *synth_program = AdjSynth::get_instance()->synth_program[prog];
	int res = -1;
	int v;

	if (synth_program != NULL)
	{
		ModSynth::settings_handler_mutex.unlock();

		res = synth_program->program_settings_manager->set_int_param_value(
			synth_program->get_active_program_preset_params(),
			"adjsynth.poly_mixer.pan_lfo_level",
			lev,
			_SET_VALUE,
			prog);

		ModSynth::settings_handler_mutex.lock();

		// Update active voices of the program
		for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			if (synth_program->assigned_voices[v] == _VOICE_ASSIGNED)
			{
				AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_pan_lfo_level(v, lev / 100.0f);
			}
		}
	}

	return res;
}
