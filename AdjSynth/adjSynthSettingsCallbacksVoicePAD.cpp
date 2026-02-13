/**
 *	@file		adjSynthSettingsCallbacksVoicePAD.cpp
 *	@author		Nahum Budin
 *	@date		5-Oct-2024
 *	@version	1.2
 *					1. Code refactoring and notaion.
 *					2. Adding support in both old and new MIDI program objects.
 *
 *	@brief		Callback to handle voice block PAD Synthesizer Settings
 *	settings
 *
 *	History:\n
 *
 *	version	1.1	5-Feb-2021
 *					1. Code refactoring and notaion.
 *	version 1.0	15_Nov-2019	First version
 *
 */

#include "adjSynth.h"

int set_voice_block_pad_synth_enabled_cb(bool enable, int voice, int prog)
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
		synth_voice->dsp_voice->enable_pad_synth_1();
	}
	else
	{
		synth_voice->dsp_voice->disable_pad_synth_1();
		
	}
	return 0;
}

int set_voice_block_pad_synth_detune_octave_cb(int oct, int voice, int prog)
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

	synth_voice->dsp_voice->wavetable_1->set_freq_detune_oct(oct);
	
	return 0;
}

int set_voice_block_pad_synth_detune_semitones_cb(int semt, int voice, int prog)
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

	synth_voice->dsp_voice->wavetable_1->set_freq_detune_semitones(semt);
	
	return 0;
}

int set_voice_block_pad_synth_detune_cents_cb(int cnts, int voice, int prog)
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

	synth_voice->dsp_voice->wavetable_1->set_freq_detune_cents(cnts);
	
	return 0;
}

int set_voice_block_pad_synth_send_filter_1_cb(int snd, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_send_filter_1_level(snd);
	
	return 0;
}

int set_voice_block_pad_synth_send_filter_2_cb(int snd, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_send_filter_2_level(snd);
	
	return 0;
}

int set_voice_block_pad_synth_freq_modulation_lfo_num_cb(int lfo, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_freq_mod_lfo(lfo);
	
	return 0;
}

int set_voice_block_pad_synth_freq_modulation_lfo_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_freq_mod_lfo_level(lev);
	
	return 0;
}

int set_voice_block_pad_synth_freq_modulation_env_num_cb(int env, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_freq_mod_env(env);
	
	return 0;
}

int set_voice_block_pad_synth_freq_modulation_env_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_freq_mod_env_level(lev);
	
	return 0;
}

int set_voice_block_pad_synth_amp_modulation_lfo_num_cb(int lfo, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_amp_mod_lfo(lfo);
	
	return 0;
}

int set_voice_block_pad_synth_amp_modulation_lfo_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_amp_mod_lfo_level(lev);
	
	return 0;
}

int set_voice_block_pad_synth_amp_modulation_env_num_cb(int env, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_amp_mod_env(env);
	
	return 0;
}

int set_voice_block_pad_synth_amp_modulation_env_level_cb(int lev, int voice, int prog)
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

	synth_voice->dsp_voice->set_pad_1_amp_mod_env_level(lev);
	
	return 0;
}

int set_voice_block_pad_synth_quality_cb(int qlt, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_wavetable_length(
		AdjSynth::get_instance()->synth_program[prog]->program_wavetable,
		qlt);
	return 0;
}

int set_voice_block_pad_synth_base_note_cb(int bnot, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_base_note(
		AdjSynth::get_instance()->synth_program[prog]->program_wavetable,
		bnot);
	return 0;
}

int set_voice_block_pad_synth_base_width_cb(int bwd, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_base_harmony_width(bwd);
	return 0;
}

int set_voice_block_pad_synth_shape_cb(int shp, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_shape(shp);
	return 0;
}

int set_voice_block_pad_synth_shape_cutoff_cb(int shcut, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_shape_cutoff(shcut);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_0_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(0, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_1_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(1, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_2_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(2, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_3_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(3, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_4_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(4, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_5_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(5, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_6_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(6, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_7_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(7, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_8_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(8, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_level_9_cb(int lev, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmony_level(9, (float)lev / 100.f);
	return 0;
}

int set_voice_block_pad_synth_harmonies_detune_cb(int hdet, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_pad_creator->set_harmonies_detune((float)hdet / 100.f);
	return 0;
}
