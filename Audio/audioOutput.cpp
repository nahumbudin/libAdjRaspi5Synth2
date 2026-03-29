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
*	@version	Ver1.2	Code refactoring and notaion.
*				29-Jan-2021	1.1 
*					1. Code refactoring and notaion.
*					2. Adding bloc-size settings
*				11-Nov-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 17, 2017.
*
*	@brief		Audio system output block
*/

#include "audioOutput.h"
#include "audioManager.h"
#include "../commonDefs.h"

extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

//AudioManager *_oaudio_manager = NULL;

/**
*   @brief  Create an AudioOutputFloat object instance.
*   @param  stage	audio update sequence stage number
*   @param	bloc_size  audio block-size
*   @param	shared_memory a pointer to a shared_memory_audio_block_float_stereo_struct_t block
*   @param	audio_first_update_ptr  a pointer to an audio block object instance
*								    that is the first in the update chain
*   @return none
*/
AudioOutputFloat::AudioOutputFloat(
				uint8_t stage,
	int block_size,
	shared_memory_audio_block_float_stereo_struct_t *shared_memory,
	AudioBlockFloat **audio_first_update_ptr, int vid, bool use_limiter)
	: AudioBlockFloat(
			2, // 2 audio inputs 
			input_queue_array, // audio block input queue
			audio_first_update_ptr,
			stage) 
{
	audio_block_stereo_float_shared_memory = shared_memory; 
	set_audio_block_size(block_size);
	voice_id = vid;
	use_the_limiter = use_limiter;
	
	i_master_volume = 100; // Currently we do not change it
	master_volume = 1.0;
	
	master_limiter_left = new MasterLimiter(5.0f, 44100.0f);
	master_limiter_right = new MasterLimiter(5.0f, 44100.0f);	
}

/**
*   @brief  sets the audio block size  
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int AudioOutputFloat::set_audio_block_size(int size)
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
int AudioOutputFloat::get_audio_block_size() 
{	
	return audio_block_size; 
}

/**
*   @brief  set the volume
*   @param  vol	volume level 0-1.0
*   @return void
*/
void AudioOutputFloat::set_master_volume(int vol)
{
	if ((vol >= 0) && (vol <= 100))
	{
		i_master_volume = vol;
		master_volume = (float)vol / 100.0f;
	}
}

/**
*   @brief  get the volume
*   @param	none
*   @return volume level 0-100
*/
int AudioOutputFloat::get_master_volume()
{
	return master_volume;
}

void AudioOutputFloat::set_use_limiter(bool use)
{
	use_the_limiter = use;
}
bool AudioOutputFloat::get_use_limiter()
{
	return use_the_limiter;
}

/**
*   @brief  Execute an update cycle - get input samples, process and 
*			write it into the shared memory.
*   @param  none
*   @return void
*/
void AudioOutputFloat::update(void) {
	
	audio_block_float_mono_t *in[2];
	volatile  unsigned int i;
	static unsigned int block_id = 0;

	//	fprintf(stderr, "Update Output\n");
	
	// Get input samples
	in[_SYNTH_VOICE_OUT_1] = receive_audio_block_read_only(_SYNTH_VOICE_OUT_1);
	
	if (in[_SYNTH_VOICE_OUT_1])
	{
		if (use_the_limiter)
		{
			master_limiter_left->processBlock(in[_SYNTH_VOICE_OUT_1]->data, nullptr, audio_block_size);
		}
		
		for (i = 0; i < audio_block_size; i++) 
		{						
			audio_block_stereo_float_shared_memory->data[_SYNTH_VOICE_OUT_1][i] = 
				in[_SYNTH_VOICE_OUT_1]->data[i] * master_volume;
		}
	}
	else 
	{
		printf("Audio Output L %i no input\n", voice_id);
		for (i = 0; i < audio_block_size; i++) 
		{
			// No input - fill with silence.
			audio_block_stereo_float_shared_memory->data[_SYNTH_VOICE_OUT_1][i] = 0;
		}
	}
	
	in[_SYNTH_VOICE_OUT_2] = receive_audio_block_read_only(_SYNTH_VOICE_OUT_2);
	
	if (in[_SYNTH_VOICE_OUT_2])
	{
		if (use_the_limiter)
		{
			master_limiter_right->processBlock(nullptr, in[_SYNTH_VOICE_OUT_2]->data, audio_block_size);
		}
		
		for (i = 0; i < audio_block_size; i++) 
		{
			audio_block_stereo_float_shared_memory->data[_SYNTH_VOICE_OUT_2][i] = 
				in[_SYNTH_VOICE_OUT_2]->data[i] * master_volume;
		}
	}
	else 
	{
		printf("Audio Output R %i no input\n", voice_id);
		for (i = 0; i < audio_block_size; i++) 
		{
			// No input - fill with silence.
			audio_block_stereo_float_shared_memory->data[_SYNTH_VOICE_OUT_2][i] = 0;
		}
	}
	

	// printf("audio out  %x %x\n", (long)audio_block_stereo_float_shared_memory, id); // <<<<<

	audio_block_stereo_float_shared_memory->id = block_id;
	block_id++;
	//	if ((id % (int)(10000000/_PERIOD_TIME_USEC)) == 0)
	//		printf("#transfers: %u  %i sec \n\r", id, (id / (1000000/_PERIOD_TIME_USEC)));

	pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);	
	if (in[_SYNTH_VOICE_OUT_1])
	{		
		release_audio_block(in[_SYNTH_VOICE_OUT_1]);
	}
	
	if (in[_SYNTH_VOICE_OUT_2])
	{
		release_audio_block(in[_SYNTH_VOICE_OUT_2]);
	}
	
	pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
	//	fprintf(stderr, "RO %i\n", in[channel]->memory_pool_index);	
}
