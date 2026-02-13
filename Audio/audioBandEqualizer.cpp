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

#include <mutex>

#include "audioBandEqualizer.h"
#include "../LibAPI/synthesizer.h"
#include "../utils/utils.h"

extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

/**
*   @brief  Creates and initializes a band equilizer audio-block object instance.
*   @param  stage	update stage number
*	@param	audio_first_update_ptr  a pointer to an audio block object instance
*								    that is the first in the update chain
*   @return none
*/
AudioBandEqualizer::AudioBandEqualizer(
					uint8_t stage,	
	int block_size,				
	AudioBlockFloat **audio_first_update_ptr)
	: AudioBlockFloat(4, // audio inputs
					  audio_input_queue_array, // audio input queue
					  audio_first_update_ptr,
					  stage)
{
	equalizer_enabled = true;
	preset = _BAND_EQUALIZER_SET_ALL_ZERO;
	set_audio_block_size(block_size);
	
	band_equalizer_L = new DSP_BandEqualizer();
	band_equalizer_R = new DSP_BandEqualizer();
}

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int AudioBandEqualizer::set_audio_block_size(int size)
{
	int res = 0;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;				
		res = audio_block_size;
	}
	else
	{
		res = -1;
	}
	
	return res;
}
	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int AudioBandEqualizer::get_audio_block_size() {	return audio_block_size; }

/**
*   @brief  Set active preset.
*   @param  prst	active preset nember
*   @return void
*/
void AudioBandEqualizer::set_preset(int prst)
{
	preset = prst;
}

/**
*   @brief  Enable equilizer.
*   @param  prst	active preset nember
*   @return void
*/
void AudioBandEqualizer::enable() 
{ 
	equalizer_enabled = true; 
}

/**
*   @brief  Disable equilizer.
*   @param  prst	active preset nember
*   @return void
*/
void AudioBandEqualizer::disable() 
{ 
	equalizer_enabled = false; 
}

/**
*   @brief  Execute an update cycle - get input samples, process and send to
*			next audio block stage..
*   @param  none
*   @return void
*/
void AudioBandEqualizer::update(void)
{
	audio_block_float_mono_t *in_block_L1, *in_block_R1, *in_block_L2, *in_block_R2, *out_block_L, *out_block_R;
	int i;
	float inL = 0.f, inR = 0.f;

	// Get input samples
	in_block_L1 = receive_audio_block_read_only(_LEFT);
	in_block_R1 = receive_audio_block_read_only(_RIGHT);
	in_block_L2 = receive_audio_block_read_only(_SEND_LEFT);
	in_block_R2 = receive_audio_block_read_only(_SEND_RIGHT);

	if (!in_block_L1 || !in_block_R1 || !in_block_L2 || !in_block_R2)
	{
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		if (in_block_L1)
			release_audio_block(in_block_L1);
		if (in_block_R1)
			release_audio_block(in_block_R1);
		if (in_block_L2)
			release_audio_block(in_block_L2);
		if (in_block_R2)
			release_audio_block(in_block_R2);
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		return;
	}
	/*
		if (!equi_enabled)
		{
			// Equilizer disabled - pass through
			transmit_audio_block(in_block_L1, _LEFT);
			transmit_audio_block(in_block_R1, _RIGHT);
			pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
			release_audio_block(in_block_L1);
			release_audio_block(in_block_R1);
			release_audio_block(in_block_L2);
			release_audio_block(in_block_R2);
			pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		}
		else
		{*/
	// Equilizer enabled

	pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
	out_block_L = allocate_audio_block();
	out_block_R = allocate_audio_block();
	pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
	if (!out_block_L || !out_block_R)
	{
		// Can't allocate 2audio blocks
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		if (in_block_L1)
			release_audio_block(in_block_L1);
		if (in_block_R1)
			release_audio_block(in_block_R1);
		if (in_block_L2)
			release_audio_block(in_block_L2);
		if (in_block_R2)
			release_audio_block(in_block_R2);
		if (out_block_L)
			release_audio_block(out_block_L);
		if (out_block_R)
			release_audio_block(out_block_R);
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);

		return;
	}

	// Process inpur samples
	for (i = 0; i < audio_block_size; i++)
	{
		inL = in_block_L1->data[i] + in_block_L2->data[i];
		inR = in_block_R1->data[i] + in_block_R2->data[i];

		if (!equalizer_enabled)
		{
			out_block_L->data[i] = inL;
			out_block_R->data[i] = inR;
		}
		else
		{
			out_block_L->data[i] = band_equalizer_L->get_equalizer_next_output(inL);
			out_block_R->data[i] = band_equalizer_R->get_equalizer_next_output(inR);
		}
	}

	transmit_audio_block(out_block_L, _LEFT);
	transmit_audio_block(out_block_R, _RIGHT);
	pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
	release_audio_block(out_block_L);
	release_audio_block(out_block_R);
	release_audio_block(in_block_L1);
	release_audio_block(in_block_R1);
	release_audio_block(in_block_L2);
	release_audio_block(in_block_R2);
	pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
	//	}
}
