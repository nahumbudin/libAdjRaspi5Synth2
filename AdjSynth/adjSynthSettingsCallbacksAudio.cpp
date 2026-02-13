/**
*	@file		adjSynthSettingsCallbacksAudio.cpp
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.

*	
*	@brief		Callback to handle audio settings
*	settings
*
*	History:\n
*	
*		version	1.1	5-Feb-2021
*					1. Code refactoring and notaion.
*					2. Adding support in sample-rate and audio block-size settings of all modules
*		version 1.0	18-Jan-2021	First version
*		
*/

#include "adjSynth.h"
#include "../Jack/jackAudioClients.h"

int set_audio_sample_rate_cb(int rate, int prog)
{
	AdjSynth::get_instance()->set_sample_rate(rate);
	return 0;
}

int set_audio_block_size_cb(int size, int prog)
{
	AdjSynth::get_instance()->set_audio_block_size(size);
	return 0;
}

int set_audio_driver_type_cb(int driver, int prog)
{
	AdjSynth::get_instance()->set_audio_driver_type(driver);
	return 0;
}

int set_audio_jack_mode_cb(int mode, int prog)
{
	set_jack_mode(mode);
	
	return 0;
}

int set_audio_jack_auto_start_state_cb(bool state, int prog)
{
	if (state)
	{
		enable_jack_auto_start();
	}
	else
	{
		disable_jack_auto_start();
	}

	return 0;
}

int set_audio_jack_auto_connect_state_cb(bool state, int prog)
{
	if (state)
	{
		enable_jack_auto_connect_audio();
	}
	else
	{
		disable_jack_auto_connect_audio();
	}

	return 0;
}
