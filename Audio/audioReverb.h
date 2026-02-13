/**
*	@file		audioReverb.h
*	@author		Nahum Budin
*	@date		2-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	2-Feb-2021	1.1
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*				9-Nov-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 15, 2018)
*
*	@brief		Audio Reverb
*/

#pragma once

//#include "../headers.h"

#include "audioBlock.h"
#include "../DSP/dspReverbModel.h"
#include "../DSP/dspFreeverb3mod2.h"
#include "../LibAPI/audio.h"

class AudioReverb : public AudioBlockFloat
{
public:
	AudioReverb(
		uint8_t stage = 0, 
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE,
		AudioBlockFloat **audio_first_update_ptr = NULL);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	
	void rev_enable(); 
	void rev_disable();

	void rev3m_enable();
	void rev3m_disable();
		
	virtual void update(void);
	
	DSP_RevModel *reverb;
	sf_reverb_state_st rv3m;

private:
	audio_block_float_mono *audio_input_queue_array[2];

	bool rev_enabled, rev3m_enabled;
	
	int sample_rate, audio_block_size;
};
