/**
 *	@file		audioVoice.cpp
 *	@author		Nahum Budin
 *	@date		29-Jan-2026
 *	@version	1.3
 *					1. Change magnitude to level.
 *
 *	@History	1.2	1-Oct-2024
 *						1. Code refactoring and notaion.
 *						2. Adding callbacks to indicate activity changes (replacing direct objects calls)
 *						3. Adding option to use global LFOs for modulation calculations.
 *				1.1	29-Jan-2021
 *				1.0	11-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October 18, 2017)
 *
 *	@brief		Audio-block polyphonic voice
 */

#include <mutex>
#include <functional>

#include "audioVoice.h"
// #include "audioPoliphonyMixer.h"
#include "../commonDefs.h"
// #include "../synthesizer/adjSynth.h"

// Mutex to controll audio memory blocks allocation 
extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

/**
*   @brief  Create an AudioVoiceFloat object instance.
*   @param  stage	audio update sequence stage number
*   @param	voice	voice number
*   @param	dsp_voc	a pointer to the associated DSP_Voice object
*   @param	audio_first_update_ptr  a pointer to an audio block object instance
*								    that is the first in the update chain
*   @return none
*/
AudioVoiceFloat::AudioVoiceFloat(
	int stage,
	int samp_rate,
	int block_size,
	int voice,
	int outs,
	DSP_Voice *dsp_voc, 
	AudioBlockFloat **audio_first_update_ptr,
	func_ptr_void_int_t set_voice_active_clbk_ptr,
	func_ptr_void_int_t set_voice_not_active_clbk_ptr,
	func_ptr_void_int_t set_voice_wait_for_not_active_clbk,
	func_ptr_void_int_t reset_voice_wait_for_not_active_clbk_ptr,
	func_ptr_void_int_bool_t free_voice_clbk_ptr)
	: AudioBlockFloat(0, // no audio input
					  NULL, // no audio input quew
					  audio_first_update_ptr,
					  stage)
{
	voice_num = voice;
	dsp_voice = dsp_voc;
	set_voice_active_callback_ptr = set_voice_active_clbk_ptr;
	set_voice_not_active_callback_ptr = set_voice_not_active_clbk_ptr;
	set_voice_wait_for_not_active_callback_ptr = set_voice_wait_for_not_active_clbk;
	reset_voice_wait_for_not_active_callback_ptr = reset_voice_wait_for_not_active_clbk_ptr;
	free_voice_callback_ptr = free_voice_clbk_ptr;
	
	//	dsp_voice->register_voice_end_event_callback(std::mem_fn(&AudioVoiceFloat::set_inactive));
	
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);

	level = 1.0f; // default voice level

	// Initialize polyphonic
	init_poly();
}

/**
*	@brief	Sets the sample-rate
*	@param	int sample rate (_SAMPLE_RATE_44 or _SAMPLE_RATE_48)
*	@return set sample-rate
*/	
int AudioVoiceFloat::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	if (dsp_voice)
	{
		dsp_voice->set_sample_rate(samp_rate);
	}
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int AudioVoiceFloat::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int AudioVoiceFloat::set_audio_block_size(int size)
{
	int res = 0;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;		
		
		if (dsp_voice)
		{
			dsp_voice->set_audio_block_size(size);
		}
		
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
int AudioVoiceFloat::get_audio_block_size() 
{	
	return audio_block_size; 
}

/**
*   @brief  Initialize polyphonic
*   @param  none
*   @return void
*/
void AudioVoiceFloat::init_poly()
{
	active = false;
	wait_for_not_active = false;
	timestamp = 0;
	note = -1;
	if (dsp_voice)
	{
		dsp_voice->reset_wait_for_not_active();
	}
}

/**
*   @brief  Return the voice number
*   @param  none
*   @return voice number
*/
int AudioVoiceFloat::get_voice_num() 
{ 
	return voice_num; 
}

/**
*   @brief  Set the voice number
*   @param  vnum	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return void
*/
void AudioVoiceFloat::set_voice_num(int v_num)
{
	if ((v_num >= 0) && (v_num < _SYNTH_MAX_NUM_OF_VOICES))
	{
		voice_num = v_num;
	}
	else
	{
		fprintf(stderr, "audioVoice: seting voice num out of range\n");
	}
}

/**
*   @brief  Set the voice active
*   @param  none
*   @return void
*/
void AudioVoiceFloat::set_active() 
{ 
	active = true; 
	if (dsp_voice)
	{
		dsp_voice->set_voice_active();
	}

	//AudioPolyMixerFloat::set_voice_active(voice_num);
	if (set_voice_active_callback_ptr)
	{
		set_voice_active_callback_ptr(voice_num);
	}
}

/**
*   @brief  Set the voice not-active
*   @param  none
*   @return void
*/
void AudioVoiceFloat::set_inactive() 
{ 	
	active = false;
	if (dsp_voice)
	{
		dsp_voice->set_voice_inactive();
	}

	//AudioPolyMixerFloat::set_voice_not_active(voice_num);
	if (set_voice_not_active_callback_ptr)
	{
		set_voice_not_active_callback_ptr(voice_num);
	}
	
	//AdjSynth::get_instance()->synth_polyphony->free_voice(voice_num, false); // not pending
	if (free_voice_callback_ptr)
	{
		free_voice_callback_ptr(voice_num, false); // not pending
	}
}

/**
*   @brief  Return the voice active state
*   @param  none
*   @return true if voice is active, false if not active.
*/
bool AudioVoiceFloat::is_voice_active() 
{ 
	return active; 
}

/**
*   @brief  Set the voice state to wait for not active
*			(used to wait until the Release phase is over 
*			and then set the voice state to not active)
*   @param  none
*   @return void
*/
void AudioVoiceFloat::set_wait_for_not_active() 
{ 
	wait_for_not_active = true;
	
	if (dsp_voice)
	{
		dsp_voice->set_voice_waits_for_not_active();
	}

	//AudioPolyMixerFloat::set_voice_wait_for_not_active(voice_num);
	if (set_voice_wait_for_not_active_callback_ptr)
	{
		set_voice_wait_for_not_active_callback_ptr(voice_num);
	}
}

/**
*   @brief  Set the voice state to NOT wait for not active
*			(used to indicate not wait until the Release phase is over 
*			and set the voice state to not active imidiatlly when note is off)
*   @param  none
*   @return void
*/
void AudioVoiceFloat::reset_wait_for_not_active() 
{ 
	wait_for_not_active = false; 
	if (dsp_voice)
	{
		dsp_voice->reset_wait_for_not_active();
	}

	//AudioPolyMixerFloat::reset_voice_wait_for_not_active(voice_num);
	if (reset_voice_wait_for_not_active_callback_ptr)
	{
		reset_voice_wait_for_not_active_callback_ptr(voice_num);
	}
}

/**
*   @brief  Return the voice wait for not active state
*   @param  none
*   @return true if voice set to not active is set, false otherwise
*/
bool AudioVoiceFloat::is_voice_wait_for_not_active() 
{ 
	return wait_for_not_active; 
}

/**
*   @brief  Set the voice timestamp [us]
*   @param  timestamp	timestamp value
*   @return void
*/
void AudioVoiceFloat::set_timestamp(uint64_t ts) 
{ 
	timestamp = ts; 
}

/**
*   @brief  Return the voice timestamp
*   @param  void
*   @return voice timestamp
*/
uint64_t AudioVoiceFloat::get_timestamp() 
{ 
	return timestamp;
}

/**
*   @brief  Set the voice note
*   @param  note	note MIDI number
*   @return void
*/
void AudioVoiceFloat::set_note(int nt) 
{
	note = nt; 
}

/**
*   @brief  Return the voice note
*   @param  none	
*   @return voice note MIDI number
*/
int AudioVoiceFloat::get_note() 
{ 
	return note; 
}

/**
 *  @brief  Set the use of global LFOs for modulation calculations
 *  @param  use_global	true to use global LFOs for modulation calculations
 *  @return void
 */
void AudioVoiceFloat::set_use_global_lfos_for_modulations(bool use_global)
{
	use_global_lfos_for_modulations = use_global;
}

/**
 *  @brief  Return the use of global LFOs for modulation calculations
 *  @param  none
 *  @return true if global LFOs are used for modulation calculations, false otherwise
 */
bool AudioVoiceFloat::get_use_global_lfos_for_modulations()
{
	return use_global_lfos_for_modulations;
}

/**
*   @brief  Return a pointer to the voice DSP_Voice object
*   @param  none
*   @return a pointer to the voice DSP_Voice object
*/
DSP_Voice *AudioVoiceFloat::get_dsp_voice() 
{ 
	return dsp_voice; 
}

/**
*   @brief  Set the voice output magnitude (volume)
*   @param  mag	voice output magnitude (volume) 0.0 to 1.0
*   @return void
*/
void AudioVoiceFloat::set_output_level(float lev)
{
	level = lev;

	if (level < 0)
	{
		level = 0;
	}
	else if (level > 1.0f)
	{
		level = 1.0f;
	}
}

/**
*   @brief  Return the voice output level (volume)
*   @param  none
*   @return the voice output level (volume) 0.0 to 1.0
*/
float AudioVoiceFloat::get_output_level() 
{ 
	return level; 
}

/**
*   @brief  Execute an update cycle - generate the voice audio block and 
*			send it to next audio block stage.
*   @param  none
*   @return void
*/
void AudioVoiceFloat::update()
{
	audio_block_float_mono_t *block_out1, *block_out2; 
	volatile int i, j = 0;
	float samp1, samp2;
	int global_lfos_sample_num = 0;

	// Verify
	if (!dsp_voice)
	{	
		return;
	}
	
	if (!active)
	{	
		return;
	}
	
	// Get audio out blocks
	pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
	block_out1 = allocate_audio_block();
	block_out2 = allocate_audio_block();
	
	pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
	if (!block_out1 || !block_out2) 
	{
		// unable to allocate memory, so we'll send nothing
		if (block_out1)
		{
			// release if the only one
			pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
			release_audio_block(block_out1);
			pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		}
		if (block_out2)
		{
			// release if the only one
			pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
			release_audio_block(block_out2);
			pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		}
		// Unfortunately, that's it
		return;
	}

	//dsp_voice->calc_next_modulation_values(global_lfos_sample_num, use_global_lfos_for_modulations);
	//global_lfos_sample_num++;

	for (i = 0; i < audio_block_size; i++) 
	{
		// Update modulation factors - updated only every _CONTROL_SUB_SAMPLING samples
		if ((i % _CONTROL_SUB_SAMPLING) == 0)
		{
			dsp_voice->calc_next_modulation_values(global_lfos_sample_num, use_global_lfos_for_modulations);
			dsp_voice->update_voice_modulation(voice_num);
			global_lfos_sample_num++;
		}

		dsp_voice->calc_next_oscilators_output_value();

		// level - Note scaled volume (see kbd)
		block_out1->data[i] = dsp_voice->get_next_output_value_ch_1() * level;
		block_out2->data[i] = dsp_voice->get_next_output_value_ch_2() * level;
	}
		
	transmit_audio_block(block_out1, _SYNTH_VOICE_OUT_1);	
	transmit_audio_block(block_out2, _SYNTH_VOICE_OUT_2);
	
	pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
	release_audio_block(block_out1);
	release_audio_block(block_out2);
	pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
}

