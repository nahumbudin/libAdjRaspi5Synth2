/**
*	@file		adjSynthSettingsCallbacksVoiceAmp.cpp
*	@author		Nahum Budin
*	@date		5-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block Amps Settings
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
#include "../LibAPI/synthesizer.h"

int set_amp_ch_1_send_cb(int lev, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
//		AdjSynth::get_instance()->audio_polyphony_mixer->set_master_send_1(lev);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_amp_ch_2_send_cb(int lev, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_master_send_2(lev);
		return 0;
	}
	else
	{
		return -1;
	}
}


int set_voice_block_amp_ch_1_level_cb(int lev, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS)) // ???
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_master_level_1(lev);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_voice_block_amp_ch_1_pan_cb(int pan, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_master_pan_1(pan);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_voice_block_amp_ch_1_pan_modulation_lfo_num_cb(int num, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_amp_1_pan_mod_lfo(num);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_voice_block_amp_ch_1_pan_modulation_lfo_level_cb(int lev, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_amp_1_pan_mod_lfo_level(lev);
		return 0;
	}
	else
	{
		return -1;
	}
}


int set_voice_block_amp_ch_2_level_cb(int lev, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_master_level_2(lev);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_voice_block_amp_ch_2_pan_cb(int pan, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_master_pan_2(pan);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_voice_block_amp_ch_2_pan_modulation_lfo_num_cb(int num, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_amp_2_pan_mod_lfo(num);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_voice_block_amp_ch_2_pan_modulation_lfo_level_cb(int lev, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
	//	AdjSynth::get_instance()->audio_polyphony_mixer->set_amp_2_pan_mod_lfo_level(lev);
		return 0;
	}
	else
	{
		return -1;
	}
}

int set_voice_block_amp_fixed_levels_state_cb(bool en, int voice, int prog)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		SynthVoice *synth_voice;

#ifdef _USE_NEW_MIDI_PROGRAM
		synth_voice = AdjSynth::get_instance()->synth_program[prog]->get_voice(voice);
#else
		synth_voice = AdjSynth::get_instance()->synth_voice[voice];
#endif
		if (synth_voice == NULL)
		{
			return -1;
		}

		if (en == _AMP_FIXED_LEVELS_ENABLE)
		{
			synth_voice->dsp_voice->out_amp_1->enable_fixed_levels();
		}
		else if (en == _AMP_FIXED_LEVELS_DISABLE)
		{
			synth_voice->dsp_voice->out_amp_1->disable_fixed_levels();
		}

		return 0;
	}
	else
	{
		return -1;
	}
}
