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

#include <mutex>

#include "audioReverb.h"
#include "../utils/utils.h"

extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

/**
*   @brief  Create an AudioReverb object instance.
*   @param	audio_first_update_ptr  a pointer to an audio block object instance
*								    that is the first in the update chain
*   @return none
*/
AudioReverb::AudioReverb(
	uint8_t stage,
	int samp_rate,
	int block_size,
	AudioBlockFloat **audio_first_update_ptr)
	: AudioBlockFloat(
		2, // audio inputs
		audio_input_queue_array, // audio input queue
		audio_first_update_ptr,
		stage)
{
	// Initiate 2 reverb models objects
	reverb = new DSP_RevModel();

	sf_presetreverb(&rv3m, samp_rate, _SF_REVERB_PRESET_DEFAULT);
}

int AudioReverb::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int AudioReverb::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int AudioReverb::set_audio_block_size(int size)
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
int AudioReverb::get_audio_block_size() {	return audio_block_size; }

void AudioReverb::rev_enable() 
{ 
	rev_enabled = true;
}
	
void AudioReverb::rev_disable() { rev_enabled = false; }

void AudioReverb::rev3m_enable() 
{ 
	rev3m_enabled = true; 
}
	
void AudioReverb::rev3m_disable() { rev3m_enabled = false; }

/**
*   @brief  Execute an update cycle - get input samples, process and send to
*			next audio block stage..
*   @param  none
*   @return void
*/
void AudioReverb::update(void)
{
	audio_block_float_mono_t *in_block_L, *in_block_R, *out_block_L, *out_block_R;
	int i;

	in_block_L = receive_audio_block_read_only(_LEFT);
	if (!in_block_L)
	{	
		// Input block not received
		return;
	}
	
	in_block_R = receive_audio_block_read_only(_RIGHT);
	if (!in_block_R)
	{
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		release_audio_block(in_block_L);
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		return;
	}

	if (!rev_enabled && !rev3m_enabled)
	{
		// Both reverb models are disabled - Pass through
		transmit_audio_block(in_block_L, _LEFT);
		transmit_audio_block(in_block_R, _RIGHT);
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		release_audio_block(in_block_L);
		release_audio_block(in_block_R);
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
	}
	else
	{
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		out_block_L = allocate_audio_block();
		out_block_R = allocate_audio_block();
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		if (!out_block_L || !out_block_R)
		{
			// one or all output blocks not allocated
			pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
			release_audio_block(in_block_L);
			release_audio_block(in_block_R);
			pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
			if (out_block_L)
			{
				pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
				release_audio_block(out_block_L);
				pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
			}
			if (out_block_R)
			{
				pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
				release_audio_block(out_block_R);
				pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
			}
			return;
		}

		// Only one reverb model can be enabled at a time.
		if (rev3m_enabled)
		{
			sf_sample_st input[audio_block_size], output[audio_block_size];

			// Process inplace - create input and output buffers
			for (i = 0; i < audio_block_size; i++)
			{
				input[i].L = in_block_L->data[i];
				input[i].R = in_block_R->data[i];
			}

			sf_reverb_process(&rv3m, audio_block_size, input, output);

			for (i = 0; i < audio_block_size; i++)
			{
				out_block_L->data[i] = output[i].L;
				out_block_R->data[i] = output[i].R;
			}
		}
		else if (rev_enabled)
		{
			reverb->process_replace(
				in_block_L->data,
				in_block_R->data,
				out_block_L->data,
				out_block_R->data,
				audio_block_size,
				1);
		}

		transmit_audio_block(out_block_L, _LEFT);
		transmit_audio_block(out_block_R, _RIGHT);
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		release_audio_block(out_block_L);
		release_audio_block(out_block_R);
		release_audio_block(in_block_L);
		release_audio_block(in_block_R);
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
	}
}
