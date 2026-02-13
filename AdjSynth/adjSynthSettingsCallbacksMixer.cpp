/**
*	@file		adjSynthSettingsCallbacksMixer.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

// Callbacs to set mixer settings

int set_mixer_channel_level_cb(int lev, int chan)
{
#ifdef _USE_NEW_POLY_MIXER_
	
#else
	AdjSynth::get_instance()->audio_polyphony_mixer->set_program_level_1(chan, lev);
	AdjSynth::get_instance()->audio_polyphony_mixer->set_program_level_2(chan, lev);
#endif
	return 0;
}

int set_mixer_channel_pan_cb(int pan, int chan)
{
#ifdef _USE_NEW_POLY_MIXER_

#else
	AdjSynth::get_instance()->audio_polyphony_mixer->set_program_pan_1(chan, pan);
	AdjSynth::get_instance()->audio_polyphony_mixer->set_program_pan_2(chan, pan);
#endif
	return 0;
}

int set_mixer_channel_send_cb(int snd, int chan)
{
#ifdef _USE_NEW_POLY_MIXER_

#else
	AdjSynth::get_instance()->audio_polyphony_mixer->set_program_send_1(chan, snd);
	AdjSynth::get_instance()->audio_polyphony_mixer->set_program_send_2(chan, snd);
#endif
	return 0;
}


int set_mixer_lfo_1_waveform_cb(int wavf, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_1_waveform(wavf);
	return 0;
}

int set_mixer_lfo_2_waveform_cb(int wavf, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_2_frequency(wavf);
	return 0;
}

int set_mixer_lfo_3_waveform_cb(int wavf, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_3_waveform(wavf);
	return 0;
}

int set_mixer_lfo_4_waveform_cb(int wavf, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_4_waveform(wavf);
	return 0;
}

int set_mixer_lfo_5_waveform_cb(int wavf, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_5_waveform(wavf);
	return 0;
}

int set_mixer_lfo_6_waveform_cb(int wavf, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_6_waveform(wavf);
	return 0;
}


int set_mixer_lfo_1_rate_cb(int rate, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_1_frequency(rate);
	return 0;
}

int set_mixer_lfo_2_rate_cb(int rate, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_2_frequency(rate);
	return 0;
}

int set_mixer_lfo_3_rate_cb(int rate, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_3_frequency(rate);
	return 0;
}

int set_mixer_lfo_4_rate_cb(int rate, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_4_frequency(rate);
	return 0;
}

int set_mixer_lfo_5_rate_cb(int rate, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_5_frequency(rate);
	return 0;
}

int set_mixer_lfo_6_rate_cb(int rate, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_6_frequency(rate);
	return 0;
}


int setmixer_lfo_1_symmetry_cb(int sym, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_1_stmmetry(sym);
	return 0;
}

int setmixer_lfo_2_symmetry_cb(int sym, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_2_stmmetry(sym);
	return 0;
}

int setmixer_lfo_3_symmetry_cb(int sym, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_3_stmmetry(sym);
	return 0;
}

int setmixer_lfo_4_symmetry_cb(int sym, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_4_stmmetry(sym);
	return 0;
}

int setmixer_lfo_5_symmetry_cb(int sym, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_5_stmmetry(sym);
	return 0;
}

int setmixer_lfo_6_symmetry_cb(int sym, int chan)
{
//	AdjSynth::get_instance()->audio_polyphony_mixer->set_lfo_6_stmmetry(sym);
	return 0;
}






