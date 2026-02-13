/**
 *	@file		adjSynthSettingsCallbacksVoiceKPS.cpp
 *	@author		Nahum Budin
 *	@date		2-Oct-2025
 *	@version	1.3
 *					1. Adding support in both old and new MIDI program objects.
 *
 *	@brief		Callback to handle voice block KarplusStrong String Generator Settings
 *	settings
 *
 *	History:\n
 *
 *	version 1.2		5-Oct-2024 Code refactoring and notaion.
 *	version	1.1	5-Feb-2021
 *					1. Code refactoring and notaion.
 *	version 1.0	15_Nov-2019	First version
 *
 */

#include "adjSynth.h"

int set_voice_block_karplus_synth_enabled_cb(bool enable, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}
	
	if (enable)
	{
		synth_voice->dsp_voice->enable_karplus_1();
	}
	else
	{
		synth_voice->dsp_voice->disable_karplus_1();
	}
	
	return 0;
}

int set_voice_block_karplus_synth_excitation_waveform_type_cb(int type, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_excitation_waveform_type(type);
	
	return 0;
}

int set_voice_block_karplus_synth_excitation_waveform_variations_cb(int var, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_excitation_waveform_variations(var);
	
	return 0;
}

int set_voice_block_karplus_synth_decay_cb(int dec, int voice, int prog)
{
	// Repalced with on decay and off decay below
	return 0;
}

int set_voice_block_karplus_synth_pluck_damping_cb(int dump, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_pluck_damping(dump);
	
	return 0;
}

int set_voice_block_karplus_synth_pluck_damping_variations_cb(int dump, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_pluck_damping_variation(dump);
	
	return 0;
}

int set_voice_block_karplus_synth_string_damping_cb(int dump, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_string_damping(dump);
	
	return 0;
}

int set_voice_block_karplus_synth_string_damping_variations_cb(int dump, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_pluck_damping_variation(dump);
	
	return 0;
}

int set_voice_block_karplus_synth_string_damping_calculation_mode_cb(int mode, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_string_dumping_calculation_mode(mode);
	return 0;
}

int set_voice_block_karplus_synth_send_filter_1_cb(int snd, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->set_karplus_1_send_filter_1_level(snd);
	
	return 0;
}

int set_voice_block_karplus_synth_send_filter_2_cb(int snd, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->set_karplus_1_send_filter_2_level(snd);
	
	return 0;
}

int set_voice_block_karplus_synth_on_decay_cb(int dec, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_on_decay(dec);
	
	return 0;
}

int set_voice_block_karplus_synth_off_decay_cb(int dec, int voice, int prog)
{
	SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
	synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
	synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
	if (synth_voice == NULL)
	{
		return -1;
	}

	synth_voice->dsp_voice->karplus_1->set_off_decay(dec);
	
	return 0;
}
