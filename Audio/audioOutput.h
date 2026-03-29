/**
 *	@file		audioOutput.cpp
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.3
 *					1. Code refactoring and notaion.
 *					2. Add id number (for debug).
 *					3. change LEFT/RIGHT defines to _SYNTH_VOICE_OUT_1/2
 *					4. Adding audio limiter in update() to avoid clipping 
 *
 *
 *	@brief		Audio system output block.
 *				This is the output stage of all analog synthesizer audio blocks chain.
 *				It receives audio data from previous block(s) in the chain and sends it to the audio driver via shared memory.
 *				Audio data is received from 2 pathes: main output and send output (for effects processing).
 *				Each path has 2 input channels : left and right.
 *
 *	@History	Ver1.2	Code refactoring and notaion.
 *				29-Jan-2021	1.1
 *					1. Code refactoring and notaion.
 *					2. Adding bloc-size settings
 *				11-Nov-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 17, 2017.
 *
*/

#pragma once

#include "audioBlock.h"
#include "audioMasterLimiter.h"
#include "../LibAPI/audio.h"
#include "../DSP/dspAmp.h"

class AudioOutputFloat : public AudioBlockFloat 
{
public:
	AudioOutputFloat(
		uint8_t stage = 0,
		int block_size = _DEFAULT_BLOCK_SIZE,	
		shared_memory_audio_block_float_stereo_struct_t *shared_memory = NULL,
		AudioBlockFloat **audio_first_update_ptr = NULL, 
		int vid = -1,
		bool use_limiter = false);

	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	void set_master_volume(int vol);
	int get_master_volume();
	
	void set_use_limiter(bool use);
	bool get_use_limiter();
	
		
	virtual void update(void);
				
private:
	// 2 input queues L and R sterams 
	audio_block_float_mono_t *input_queue_array[2];
	// Shared memory for transfering data to audio driver
	shared_memory_audio_block_float_stereo_struct_t *audio_block_stereo_float_shared_memory;
	
	MasterLimiter *master_limiter_left;
	MasterLimiter *master_limiter_right;
	
	int audio_block_size;
	
	bool use_the_limiter;

	int voice_id;
	
	int i_master_volume; // 0-100
	float master_volume; // 0.0 - 1.0
	
};
