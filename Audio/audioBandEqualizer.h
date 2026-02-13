/**
*	@file		audioBandEqualizer.h
*	@author		Nahum Budin
*	@date		3-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	2-Feb-2021	1.1 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*				10-Nov-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 July 6, 2018
*
*	@brief		Audio Band Equalizer Block 
*/

#pragma once

#include "audioBlock.h"
#include "../DSP/dspBandEqualizer.h"
#include "../LibAPI/audio.h"

class AudioBandEqualizer : public AudioBlockFloat
{
public:
	AudioBandEqualizer(
		uint8_t stage = 0, 
		int block_size = _DEFAULT_BLOCK_SIZE,
		AudioBlockFloat **audio_first_update_ptr = NULL);
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	void set_preset(int prst);

	virtual void update(void);

	void enable();
	void disable();

	DSP_BandEqualizer *band_equalizer_L, *band_equalizer_R;

private:
	audio_block_float_mono * audio_input_queue_array[4];

	bool equalizer_enabled;
	int preset;
	
	int audio_block_size;
};
