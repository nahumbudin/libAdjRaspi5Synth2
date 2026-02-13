/**
 *	@file		adjSynthSettingsCallbacksVoiceFilter.cpp
 *	@author		Nahum Budin
 *	@date		2-Oct-2025
 *	@version	1.3
 *					1. Adding support in both old and new MIDI program objects.
 *
 *	@brief		Callback to handle voice block Filters Settings
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


int set_voice_block_filter_1_frequency_cb(int freq, int voice, int prog)
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

	synth_voice->dsp_voice->filter_1->set_frequency(freq);
	return 0;
}

int set_voice_block_filter_1_octave_cb(int oct, int voice, int prog)
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

	synth_voice->dsp_voice->filter_1->set_octave(oct);
	
	return 0;
}

int set_voice_block_filter_1_q_cb(int q, int voice, int prog)
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

	synth_voice->dsp_voice->filter_1->set_resonance(q);
	
	return 0;
}

int set_voice_block_filter_1_kbd_track_cb(int kbdt, int voice, int prog)
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

	synth_voice->dsp_voice->filter_1->set_kbd_track(kbdt);
	return 0;
}

int set_voice_block_filter_1_band_cb(int bnd, int voice, int prog)
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

	synth_voice->dsp_voice->filter_1->set_band(bnd);
	
	return 0;
}

int set_voice_block_filter_1_freq_modulation_lfo_num_cb(int lfo, int voice, int prog)
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
	
	synth_voice->dsp_voice->set_filter_1_freq_mod_lfo(lfo);
	
	return 0;
}

int set_voice_block_filter_1_freq_modulation_lfo_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_filter_1_freq_mod_lfo_level(lev);
	return 0;
}

int set_voice_block_filter_1_freq_modulation_env_num_cb(int env, int voice, int prog)
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

	synth_voice->dsp_voice->set_filter_1_freq_mod_env(env);
	return 0;
}

int set_voice_block_filter_1_freq_modulation_env_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_filter_1_freq_mod_env_level(lev);
	return 0;
}


int set_voice_block_filter_2_frequency_cb(int freq, int voice, int prog)
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

	synth_voice->dsp_voice->filter_2->set_frequency(freq);
	return 0;
}

int set_voice_block_filter_2_octave_cb(int oct, int voice, int prog)
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

	synth_voice->dsp_voice->filter_2->set_octave(oct);
	return 0;
}

int set_voice_block_filter_2_q_cb(int q, int voice, int prog)
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

	synth_voice->dsp_voice->filter_2->set_resonance(q);
	return 0;
}

int set_voice_block_filter_2_kbd_track_cb(int kbdt, int voice, int prog)
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

	synth_voice->dsp_voice->filter_2->set_kbd_track(kbdt);
	
	return 0;
}

int set_voice_block_filter_2_band_cb(int bnd, int voice, int prog)
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

	synth_voice->dsp_voice->filter_2->set_band(bnd);
	
	return 0;
}

int set_voice_block_filter_2_freq_modulation_lfo_num_cb(int lfo, int voice, int prog)
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

	synth_voice->dsp_voice->set_filter_2_freq_mod_lfo(lfo);
	
	return 0;
}

int set_voice_block_filter_2_freq_modulation_lfo_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_filter_2_freq_mod_lfo_level(lev);
	return 0;
}

int set_voice_block_filter_2_freq_modulation_env_num_cb(int env, int voice, int prog)
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

	synth_voice->dsp_voice->set_filter_2_freq_mod_env(env);
	return 0;
}

int set_voice_block_filter_2_freq_modulation_env_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_filter_2_freq_mod_env_level(lev);
	return 0;
}

