/**
*	@file		adjSynthSettingsCallbacksEqualizer.cpp
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion. (equi->equa)
*	
*	@brief		Callback to handle Bands Equilizer settings
*	settings
*
*	History:\n
*	
*	version	1.1	5-Feb-2021
*			1. Code refactoring and notaion. (equi->equa)
*	version 1.0	30_Nov-2019	First version
*		
*/

#include "adjSynth.h"

int set_band_equalizer_band_31_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_31_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_31_hz, lvl);
	return 0;
}

int set_band_equalizer_band_62_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_62_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_62_hz, lvl);
	return 0;
}

int set_band_equalizer_band_125_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_125_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_125_hz, lvl);
	return 0;
}

int set_band_equalizer_band_250_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_250_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_250_hz, lvl);
	return 0;
}

int set_band_equalizer_band_500_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_500_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_500_hz, lvl);
	return 0;
}

int set_band_equalizer_band_1K_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_1000_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_1000_hz, lvl);
	return 0;
}

int set_band_equalizer_band_2K_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_2000_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_2000_hz, lvl);
	return 0;
}

int set_band_equalizer_band_4K_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_4000_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_4000_hz, lvl);
	return 0;
}

int set_band_equalizer_band_8K_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_8000_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_8000_hz, lvl);
	return 0;
}

int set_band_equalizer_band_16K_level_cb(int lvl, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_L->set_band_level(_band_16000_hz, lvl);
	AdjSynth::get_instance()->audio_equalizer->band_equalizer_R->set_band_level(_band_16000_hz, lvl);
	return 0;
}

int set_band_equalizer_preset_cb(int pres, int prog)
{
	AdjSynth::get_instance()->audio_equalizer->set_preset(pres);
	return 0;
}



