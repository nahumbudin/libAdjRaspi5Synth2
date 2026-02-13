/**
 *	@file		adjSynthSettingsCallbacksAmp.cpp
 *	@author		Nahum Budin
 *	@date		10-Oct-2025
 *	@version	1.0
 *					1. Initial.
 *
 *	@brief		AdjHeart Synthesizer Amplifiers Master Gain and Send parameters
 *
 *	History:\n
 *
 *		First version
 *
 */

#include "adjSynth.h"

// The dummy_prog paramter is required for the int fun(int, int) callback template 
int set_amp_gain_1_cb(int gain, int dummy_prog)
{
	//AdjSynth::get_instance()->audio_polyphony_mixer->set_master_level_1(gain);
	
	return 0;
}

int set_amp_gain_2_cb(int gain, int dummy_prog)
{
	//AdjSynth::get_instance()->audio_polyphony_mixer->set_master_level_2(gain);
	
	return 0;
}

int set_amp_send_1_cb(int send, int dummy_prog)
{
	//AdjSynth::get_instance()->audio_polyphony_mixer->set_master_send_1(send);
	
	return 0;
}

int set_amp_send_2_cb(int send, int dummy_prog)
{
	//AdjSynth::get_instance()->audio_polyphony_mixer->set_master_send_2(send);
	
	return 0;
}
