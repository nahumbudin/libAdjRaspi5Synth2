/**
 *	@file		audioPolyMixer.cpp
 *	@author		Nahum Budin
 *	@date		29-Sep-2025
 *	@version	1.0
 *
 *
 *	Mix audio ch1 and ch2 of all voices into a master stereo Left and Right output signals, and Left and Right send signals.
 *
 *	Unlike the previous version (audioPolyphonyMixer.h), this version does not handle the voice
 *   allocations to programs but only directlly the audio mixing of all the voices.
 *	The voice allocation to programs is handled by AdjSynthPrograms object.
 *	
 *	In addition, this version uses global LFOs for pan modulation.
 *
 */

#include "audioPolyMixer.h"

/* Mutex to controll audio memory blocks allocation (adjSynyh.h) */
extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

/* Global used to set individual output level/pan/send for non program operation */
float master_level_1, master_level_2, master_pan_1, master_pan_2, master_send_1, master_send_2;

AudioManager *poly_mixer_manager;

AudioPolyMixer *AudioPolyMixer::audio_poly_mixer_instance = NULL;

int AudioPolyMixer::num_of_inputs = _SYNTH_MAX_NUM_OF_VOICES;

AudioPolyMixer::AudioPolyMixer(
	int stage,
	int num_of_voices,
	int block_size,
	int num_of_programs, // TODO: currently not used
	int mapping_mode,
	AudioBlockFloat **audio_first_update_ptr)
	: AudioBlockFloat(
		  0,	// no audio input - input data comes from AudioManager shared memory
		  NULL, // no audio input queue
		  audio_first_update_ptr,
		  stage)
{
	set_audio_block_size(block_size);
	set_midi_mapping_mode(mapping_mode);

	poly_mixer_manager = AudioManager::get_instance();

	num_of_inputs = num_of_voices;
	if (num_of_inputs > _SYNTH_MAX_NUM_OF_VOICES)
	{
		num_of_inputs = _SYNTH_MAX_NUM_OF_VOICES;
	}
	
	for (int i = 0; i < num_of_inputs; i++)
	{
		// Individual voices settings
		voice_level[i] = 0.8f;
		voice_pan[i] = 0.0f;
		voice_send[i] = 0.0f;
		voice_pan_lfo[i] = _LFO_NONE;
		voice_pan_lfo_level[i] = 0.0f;
	}
}
	
AudioPolyMixer *AudioPolyMixer::get_instance(
	int stage,
	int num_of_voices,
	int num_of_programs,
	int block_size,
	int mapping_mode,
	AudioBlockFloat **audio_first_update_ptr)
{
	if (audio_poly_mixer_instance == NULL)
	{
		audio_poly_mixer_instance = new AudioPolyMixer(
			stage,
			num_of_voices,
			num_of_programs,
			block_size,
			mapping_mode,
			audio_first_update_ptr);
	}
	
	return audio_poly_mixer_instance;
}

AudioPolyMixer::~AudioPolyMixer()
{
	
}

/**
 *   @brief  Set mixer active state
 *   @param  active	when true mixer will become active and not active when set to false
 *   @return void
 */
void AudioPolyMixer::set_active()
{
	active = true;
}

/**
 *   @brief  Set mixer Master level 1
 *   @param  lev	level 0-100
 *   @return void
 */

/**
 *   @brief  sets the midi mapping mode
 *   @param  int mode: _MIDI_MAPPING_MODE_SKETCH, _MIDI_MAPPING_MODE_MAPPING
 *   @return mode if OK; -1 param out of range
 */
int AudioPolyMixer::set_midi_mapping_mode(int mode)
{
	int res = 0;

	if (is_valid_midi_mapping_mode(mode))
	{
		midi_mapping_mode = mode;
		res = midi_mapping_mode;
	}
	else
	{
		res = -1;
	}

	return res;
}

/**
 *   @brief  sets the audio block size
 *   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
 *   @return bloc size OK; -1 param out of range
 */
int AudioPolyMixer::set_audio_block_size(int size)
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
int AudioPolyMixer::get_audio_block_size()
{
	return audio_block_size;
}

int AudioPolyMixer::set_voice_level(int voice, float level)
{
	float val = level;
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (val < 0.f)
		{
			val = 0.f;
		}
		if (val > 1.f)
		{
			val = 1.f;
		}

		voice_level[voice] = val;
		
		return 0;
	}
	
	return -1;
}

int AudioPolyMixer::set_voice_pan(int voice, float pan)
{
	float val = pan;
	
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (val < -1.f)
		{
			val = -1.f;
		}
		if (val > 1.f)
		{
			val = 1.f;
		}
		voice_pan[voice] = val;
		
		return 0;
	}
	
	return -1;
}
int AudioPolyMixer::set_voice_send(int voice, float send)
{
	float val = send;
	
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (val < 0.f)
		{
			val = 0.f;
		}
		if (val > 1.f)
		{
			val = 1.f;
		}
		voice_send[voice] = val;
		
		return 0;
	}
	
	return -1;
}

int AudioPolyMixer::set_voice_pan_lfo(int voice, int lfo)
{
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if ((lfo >= _LFO_NONE) && (lfo <= _LFO_6))
		{
			voice_pan_lfo[voice] = lfo;
			return 0;
		}
	}
	
	return -1;
}

int AudioPolyMixer::set_voice_pan_lfo_level(int voice, float level)
{
	float val = level;
	
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (val < 0.f)
		{
			val = 0.f;
		}
		if (val > 1.f)
		{
			val = 1.f;
		}
		voice_pan_lfo_level[voice] = val;
		
		return 0;
	}
	
	return -1;
}

/**
 *  @brief  Set pan lfo modulation value.
 *	@param	modFactor	modulation factor (depth/level)
 *	@param	modVal		modulation LFO output signal value
 *	@param	value		pointer to a float variable to hold new modulation value
 *  @return void
 */
void AudioPolyMixer::set_pan_lfo_modulation_value(float mod_factor, float mod_val, float pan_value ,float *value)
{
	*(value) = mod_factor * mod_val + pan_value;
	
	if (*(value) < -1.0f)
	{
		*(value) = -1.0f; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	else if (*(value) > 1.0f)
	{
		*(value) = 1.0f;
	}
}

/**
 *  @brief  Calculate next LFO modulation values.
 *	@param	none
 *  @return modulation value
 */
void AudioPolyMixer::calc_next_pan_modulation_value(int voice)
{
	lfo_out[0] = AdjSynth::get_instance()->global_lfo_1->get_next_output_val(AdjSynth::get_instance()->global_lfo_1_actual_freq);
	lfo_out[1] = AdjSynth::get_instance()->global_lfo_2->get_next_output_val(AdjSynth::get_instance()->global_lfo_2_actual_freq);	
	lfo_out[2] = AdjSynth::get_instance()->global_lfo_3->get_next_output_val(AdjSynth::get_instance()->global_lfo_3_actual_freq);
	lfo_out[3] = AdjSynth::get_instance()->global_lfo_4->get_next_output_val(AdjSynth::get_instance()->global_lfo_4_actual_freq);
	lfo_out[4] = AdjSynth::get_instance()->global_lfo_5->get_next_output_val(AdjSynth::get_instance()->global_lfo_5_actual_freq);
	lfo_out[5] = AdjSynth::get_instance()->global_lfo_6->get_next_output_val(AdjSynth::get_instance()->global_lfo_6_actual_freq);
	
	if (voice_pan_lfo[voice] > _LFO_NONE)
	{
		set_pan_lfo_modulation_value(voice_pan_lfo_level[voice], 
			lfo_out[voice_pan_lfo[voice] - 1], 
			voice_pan[voice],
			&voice_pan_mod_level[voice]);
	}
	else
	{
		voice_pan_mod_level[voice] = voice_pan[voice];
	}
}

/**
 *   @brief  Execute an update cycle - get input samples, process and send to
 *				next audio block stage..
 *   @param  none
 *   @return void
 */
void AudioPolyMixer::update()
{
	audio_block_float_mono_t *block_out_L, *block_out_R, *block_send_L, *block_send_R;
	int voice, samp;

	// Counts modulation subsample index. 
	int subsamp;

	if (active)
	{		
		// Allocate output blocks
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		block_out_L = allocate_audio_block();
		block_out_R = allocate_audio_block();
		block_send_L = allocate_audio_block();
		block_send_R = allocate_audio_block();
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);

		if (!block_out_L || !block_out_R || !block_send_L || !block_send_R)
		{
			// unable to allocate any memory block, so we'll release what we could get, send nothing, and return.
			printf("Audio Poly Mixer - Unable to Allocate Memory/n");
			pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);

			if (block_out_L)
			{
				release_audio_block(block_out_L);
			}

			if (block_out_R)
			{
				release_audio_block(block_out_R);
			}

			if (block_send_L)
			{
				release_audio_block(block_send_L);
			}

			if (block_send_R)
			{
				release_audio_block(block_send_R);
			}
			pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);

			return;
		}

		subsamp = 0;
		
		// voice 0 - it is the 1st voice, so its samples will be used as base for all other voices.
		if (AdjSynth::get_instance()->synth_voice[0]->audio_voice->is_voice_active() ||
			AdjSynth::get_instance()->synth_voice[0]->audio_voice->is_voice_wait_for_not_active())
		{
			for (samp = 0; samp < audio_block_size; samp++)
			{
				if ((samp % _CONTROL_SUB_SAMPLING) == 0)
				{
					calc_next_pan_modulation_value(0);
				}

				// Calculate all block output samples for voice 0
				// Note: the Send Filters levels of each source sets the balance between the voice signa 1 and signal 2
				block_out_L->data[samp] = 
					(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_1][samp] +
					 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_2][samp]) *
						voice_level[0] * (1 - voice_pan_mod_level[0]) * 0.5f; // pan: -1..1 -> Left:0..1
					
				block_out_R->data[samp] = 
					(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_1][samp] +
					 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_2][samp]) *
						voice_level[0] * (1 + voice_pan_mod_level[0]) * 0.5f; // pan: -1..1 -> Right:0..1
				
				block_send_L->data[samp] =
					(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_1][samp] +
					 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_2][samp]) *
						voice_level[0] * voice_send[0] * (1 - voice_pan_mod_level[0]) * 0.5f; // pan: -1..1 -> Left:0..1
				
				block_send_R->data[samp] =
					(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_1][samp] +
					 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_SYNTH_VOICE_OUT_2][samp]) *
						voice_level[0] * voice_send[0] * (1 + voice_pan_mod_level[0]) * 0.5f; // pan: -1..1 -> Right:0..1
			}
		}
		else
		{
			// voice not active - zero values output
			for (samp = 0; samp < audio_block_size; samp++)
			{
				block_out_L->data[samp] = 0;
				block_out_R->data[samp] = 0;
				block_send_L->data[samp] = 0;
				block_send_R->data[samp] = 0;
			}
		} // if (voice 0 active

		// All other voices: accumulate samples values to voice[0] samples or to 0 if voice[0] was not active.
		for (voice = 1; voice < num_of_inputs; voice++)
		{
			subsamp = 0;

			if (AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_active() ||
				AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_wait_for_not_active())
			{
				for (samp = 0; samp < audio_block_size; samp++)
				{
					if ((samp % _CONTROL_SUB_SAMPLING) == 0)
					{
						calc_next_pan_modulation_value(voice);
					}

					// Accumulate all block output samples for all other voices
					block_out_L->data[samp] += 
					(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_1][samp] +
					 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_2][samp]) *
						voice_level[voice] * (1 - voice_pan_mod_level[voice]) * 0.5f; // pan: -1..1 -> Left:0..1

					block_out_R->data[samp] +=
						(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_1][samp] +
						 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_2][samp]) *
							voice_level[voice] * (1 + voice_pan_mod_level[voice]) * 0.5f; // pan: -1..1 -> Right:0..1
				
					block_send_L->data[samp] +=
						(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_1][samp] +
						 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_2][samp]) *
							voice_level[voice] * voice_send[voice] * (1 - voice_pan_mod_level[voice]) * 0.5f; // pan: -1..1 -> Left:0..1
				
					block_send_R->data[samp] +=
						(poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_1][samp] +
						 poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_SYNTH_VOICE_OUT_2][samp]) *
							voice_level[voice] * voice_send[voice] * (1 + voice_pan_mod_level[voice]) * 0.5f; // pan: -1..1 -> Right:0..1
				} // for (samp
			} // if (voice active
		} // for (voice

		// Recording TODO:
		/*
		if (RiffWave::getInstance()->isRecording())
		{
			for (i = 0; i < _PERIOD_SIZE; i++)
			{
				RiffWave::getInstance()->addLeftSample((int16_t)(blockOutL->data[i] * 32767.f));
				RiffWave::getInstance()->addRightSample((int16_t)(blockOutR->data[i] * 32767.f));
			}

			RiffWave::getInstance()->incBlockCount();
			callbackUpdateRecordingTimeDisplay(RiffWave::getInstance()->getBlockCount() * (uint32_t)_PERIOD_TIME_USEC / (uint32_t)1000);
		}

			//	if (RiffWave::getInstance()->getRecordingLength() >= 441000)
			//		RiffWave::getInstance()->stopRecording();
		*/

		// Send output blocks to next stage.
		transmit_audio_block(block_out_L, _LEFT);
		transmit_audio_block(block_out_R, _RIGHT);
		transmit_audio_block(block_send_L, _SEND_LEFT);
		transmit_audio_block(block_send_R, _SEND_RIGHT);

		// Release all blocks.
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		release_audio_block(block_out_L);
		release_audio_block(block_out_R);
		release_audio_block(block_send_L);
		release_audio_block(block_send_R);
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		
	} // if (active
}

