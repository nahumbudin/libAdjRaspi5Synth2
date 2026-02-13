/**
*	@file		audioPolyphonyMixer.cpp
*	@author		Nahum Budin
*	@date		24-Sep-2025
*	@version	1.2 
*					1. Bugs fix.
*	
*	History
*		version 1.1		1-Feb-2021	1. Code refactoring and notaion.
*									2. Adding sample-rate and bloc-size settings
*									3. Adding midi maping mode settings (midi/sketch)
*									4. Adding voice-status settings (null, not-active, wait-for-not-active, active)
*		version	1.0		11-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 17, 2017)
*
*	@brief		Mix audio ch1 and ch2 of all voices into stereo Left and Right output signals 
*/

#include "audioPolyphonyMixer.h"
#include "audioManager.h"

#include "../commonDefs.h"
#include "../utils/utils.h"

extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

AudioManager *polyphony_mixer_manager; // = AudioManager::get_instance();

/* Used to set individual output level/pan for non program operation*/
 float master_level_1, master_level_2, master_pan_1, master_pan_2, master_send_1, master_send_2;

AudioPolyphonyMixerFloat *AudioPolyphonyMixerFloat::audio_poly_mixer_instance = NULL;
int AudioPolyphonyMixerFloat::inputs = 1;

bool AudioPolyphonyMixerFloat::voice_active[_SYNTH_MAX_NUM_OF_VOICES];
bool AudioPolyphonyMixerFloat::voice_wait_for_not_active[_SYNTH_MAX_NUM_OF_VOICES];

/* Used to set individual output level/pan for each program */
float program_level_1[_SYNTH_MAX_NUM_OF_PROGRAMS], program_level_2[_SYNTH_MAX_NUM_OF_PROGRAMS];
float program_pan_1[_SYNTH_MAX_NUM_OF_PROGRAMS], program_pan_2[_SYNTH_MAX_NUM_OF_PROGRAMS];
float program_send_1[_SYNTH_MAX_NUM_OF_PROGRAMS], program_send_2[_SYNTH_MAX_NUM_OF_PROGRAMS];
float program_idle_level_1, program_idle_level_2;
float program_idle_pan_1, program_idle_pan_2;
float program_idle_send_1, program_idle_send_2;

/**
*   @brief  Create an AudioPolyMixerFloat object instance.
*   @param  stage	audio update sequence stage number
*   @param	num_of_voices	number of audio voices
*   @param	audio_first_update_ptr  a pointer to an audio block object instance
*								    that is the first in the update chain
*   @return none
*/
AudioPolyphonyMixerFloat::AudioPolyphonyMixerFloat(
	int stage,
	int num_of_voices,
	int block_size,
	int num_of_programs,
	int mapping_mode,
	AudioBlockFloat **audio_first_update_ptr)
	: AudioBlockFloat(
	0, // no audio input
	NULL, // no audio input queue
	audio_first_update_ptr,
	stage)
{
	set_audio_block_size(block_size);

	polyphony_mixer_manager = AudioManager::get_instance();
	
	inputs = num_of_voices;
	if (inputs > _SYNTH_MAX_NUM_OF_VOICES)
	{	
		inputs = _SYNTH_MAX_NUM_OF_VOICES;
	}
	
	programs = num_of_programs;
	if (programs > _SYNTH_MAX_NUM_OF_PROGRAMS)
	{
		programs = _SYNTH_MAX_NUM_OF_PROGRAMS;
	}

	for (int i = 0; i < _SYNTH_MAX_NUM_OF_VOICES; i++)
	{
		gain1[i] = &program_idle_level_1;
		gain2[i] = &program_idle_level_2;
		pan1[i] =  &program_idle_pan_1;
		pan2[i] =  &program_idle_pan_2;
		send1[i] = &program_idle_send_1;
		send2[i] = &program_idle_send_2;

		preserved_gain_1[i] = gain1[i];
		preserved_gain_2[i] = gain2[i];
		preserved_pan_1[i] = pan1[i];
		preserved_pan_2[i] = pan2[i];
		preserved_send_1[i] = send1[i];
		preserved_send_2[i] = send2[i];
		
		voice_active[i] = false;
		voice_wait_for_not_active[i] = false;
	}

	midi_mapping_mode = mapping_mode;

	master_level_1 = master_level_2 = 0.5f;
	master_pan_1 = master_pan_2 = 0.f;
	master_send_1 = master_send_2 = 0.f;
	program_idle_level_1 = program_idle_level_2 = 0.5f;
	program_idle_pan_1 = program_idle_pan_2 = 0.f;
	program_idle_send_1 = program_idle_send_2 = 0.f;

	for (int prog = 0; prog < programs; prog++)
	{
		program_level_1[prog] = program_level_2[prog] = 0.5f;
		program_pan_1[prog] = program_pan_2[prog] = 0.0f;
		program_send_1[prog] = program_send_2[prog] = 0.0f;
	}

	/* LFOs for pan/amp modulations */
	lfo1 = new DSP_Osc(5000,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo_1_frequency((float)10);

	lfo2 = new DSP_Osc(5001,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo_2_frequency((float)10);

	lfo3 = new DSP_Osc(5002,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo_3_frequency((float)10);
	
	lfo4 = new DSP_Osc(5003,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo_4_frequency((float)10);
	
	lfo5 = new DSP_Osc(5004,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo_5_frequency((float)10);
	
	lfo6 = new DSP_Osc(5005,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo_6_frequency((float)10);
}

AudioPolyphonyMixerFloat *AudioPolyphonyMixerFloat::get_instance(
	int stage,
	int num_of_voices,
	int num_of_programs,
	int block_size,
	int mapping_mode,
	AudioBlockFloat** audio_first_update_ptr)
{
	if (audio_poly_mixer_instance == NULL)
	{
		audio_poly_mixer_instance = new AudioPolyphonyMixerFloat(
			stage,
			num_of_voices,
			num_of_programs,
			block_size,
			mapping_mode,
			audio_first_update_ptr);
	}

	return audio_poly_mixer_instance;
}

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return bloc size OK; -1 param out of range
*/
int AudioPolyphonyMixerFloat::set_audio_block_size(int size)
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
int AudioPolyphonyMixerFloat::get_audio_block_size() 
{	
	return audio_block_size; 
}

/**
*   @brief  sets the midi mapping mode  
*   @param  int mode: _MIDI_MAPPING_MODE_SKETCH, _MIDI_MAPPING_MODE_MAPPING
*   @return mode if OK; -1 param out of range
*/
int AudioPolyphonyMixerFloat::set_midi_maping_mode(int mode)
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
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/
int AudioPolyphonyMixerFloat::get_midi_maping_mode() 
{	
	return midi_mapping_mode; 
}

/**
*   @brief  Set voice state to active
*	@param	voice_num	voice number
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_active(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		voice_active[voice] = true;
	}
}

/**
*   @brief  Set voice state to not active
*	@param	voice_num	voice number
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_not_active(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		voice_active[voice] = false;
	}
}

/**
*   @brief  Returns voice active state
*	@param	voice_num	voice number
*   @return voice active state; false if voice out of range
*/
bool AudioPolyphonyMixerFloat::voice_is_active(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		return voice_active[voice];
	}
	else
	{
		return false;
	}
}

/**
*   @brief  Set voice state to wait for not active
*	@param	voice_num	voice number
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_wait_for_not_active(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		voice_wait_for_not_active[voice] = true;
	}
}

/**
*   @brief  Set voice state to not wait for not active
*	@param	voice_num	voice number
*   @return void
*/
void AudioPolyphonyMixerFloat::reset_voice_wait_for_not_active(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		voice_wait_for_not_active[voice] = false;
	}
}

/**
*   @brief  Returns voice wait for not active state
*	@param	voice_num	voice number
*   @return voice wait for not active state; false if voice out of range
*/
bool AudioPolyphonyMixerFloat::voice_waits_for_not_active(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		return voice_wait_for_not_active[voice];
	}
	else
	{
		return false;
	}
}

/**
*   @brief  Set mixer active state
*   @param  active	when true mixer will become active and not active when set to false
*   @return void
*/
void AudioPolyphonyMixerFloat::set_active() 
{ 
	active = true; 
}

/**
*   @brief  Set mixer Master level 1
*   @param  lev	level 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_master_level_1(int lev)
{
	master_level_1 = (float)lev / 100.f;
	
	if (master_level_1 < 0)
	{
		master_level_1 = 0.f;
	}
	else if (master_level_1 > 1.f)
	{
		master_level_1 = 1.f;
	}
}

/**
*   @brief  Set mixer Master level 2
*   @param  lev	level 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_master_level_2(int lev)
{
	master_level_2 = (float)lev / 100.f;
	if (master_level_2 < 0)
	{
		master_level_2 = 0.f;
	}
	else if (master_level_2 > 1.f)
	{
		master_level_2 = 1.f;
	}
}

/**
*   @brief  Set mixer Master pan 1
*   @param  pan	level 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_master_pan_1(int pan)
{
	master_pan_1 = (float)(pan - 50) / 50.f;
	if (master_pan_1 < -1.f)
	{
		master_pan_1 = -1.f;
	}
	else if (master_pan_1 > 1.f)
	{
		master_pan_1 = 1.f;
	}
}

/**
*   @brief  Set mixer Master pan 2
*   @param  pan	level 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_master_pan_2(int pan)
{
	master_pan_2 = (float)(pan - 50) / 50.f;
	if (master_pan_2 < -1.f)
	{
		master_pan_2 = -1.f;
	}
	else if (master_pan_2 > 1.f)
	{
		master_pan_2 = 1.f;
	}
}

/**
*   @brief  Set mixer Master send 1
*   @param  pan	level 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_master_send_1(int lev)
{
	master_send_1 = (float)lev / 100.f;
	if (master_send_1 < 0)
	{
		master_send_1 = 0.f;
	}
	else if (master_send_1 > 1.f)
	{
		master_send_1 = 1.f;
	}
}

/**
*   @brief  Set mixer Master send 2
*   @param  pan	level 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_master_send_2(int lev)
{
	master_send_2 = (float)lev / 100.f;
	if (master_send_2 < 0)
	{
		master_send_2 = 0.f;
	}
	else if (master_send_2 > 1.f)
	{
		master_send_2 = 1.f;
	}
}

/**
*   @brief  Set mixer Program level 1
*   @param	prog	Program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @param  lev		level 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_program_level_1(int prog, int lev)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		program_level_1[prog] = (float)lev / 100.f;
		
		if (program_level_1[prog] < 0)
		{
			program_level_1[prog] = 0.f;
		}
		else if (program_level_1[prog] > 1.f)
		{
			program_level_1[prog] = 1.f;
		}
	}
}

/**
*   @brief  Set mixer Program level 2
*   @param	prog	Program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @param  lev		level 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_program_level_2(int prog, int lev)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		program_level_2[prog] = (float)lev / 100.f;
		
		if (program_level_2[prog] < 0)
		{
			program_level_2[prog] = 0.f;
		}
		else if (program_level_2[prog] > 1.f)
		{
			program_level_2[prog] = 1.f;
		}
	}
}

/**
*   @brief  Set mixer Program pan 1
*   @param	prog	Program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @param  pan		pan 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_program_pan_1(int prog, int pan)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		program_pan_1[prog] = (float)(pan - 50) / 50.f;
		
		if (program_pan_1[prog] < -1.f)
		{
			program_pan_1[prog] = -1.f;
		}
		else if (program_pan_1[prog] > 1.f)
		{
			program_pan_1[prog] = 1.f;
		}
	}
}

/**
*   @brief  Set mixer Program pan 2
*   @param	prog	Program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @param  pan		pan 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_program_pan_2(int prog, int pan)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		program_pan_2[prog] = (float)(pan - 50) / 50.f;
		
		if (program_pan_2[prog] < -1.f)
		{
			program_pan_2[prog] = -1.f;
		}
		else if (program_pan_2[prog] > 1.f)
		{
			program_pan_2[prog] = 1.f;
		}
	}
}

/**
*   @brief  Set mixer Program send 1
*   @param	prog	Program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @param  pan		send 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_program_send_1(int prog, int snd)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		program_send_1[prog] = (float)snd / 100.f;
		
		if (program_send_1[prog] < 0)
		{
			program_send_1[prog] = 0.f;
		}
		else if (program_send_1[prog] > 1.f)
		{
			program_send_1[prog] = 1.f;
		}
	}
}

/**
*   @brief  Set mixer Program send 2
*   @param	prog	Program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @param  pan		send 0-100 (50 - center)
*   @return void
*/
void AudioPolyphonyMixerFloat::set_program_send_2(int prog, int snd)
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		program_send_2[prog] = (float)snd / 100.f;
		
		if (program_send_2[prog] < 0)
		{
			program_send_2[prog] = 0.f;
		}
		else if (program_send_2[prog] > 1.f)
		{
			program_send_2[prog] = 1.f;
		}
	}
}

/**
*   @brief  Get a pointer to mixer Master level 1 float variable
*   @param  none
*   @return a pointer to mixer Master level 1 float variable
*/
float *AudioPolyphonyMixerFloat::get_master_level_1_ptr() 
{ 
	return &master_level_1; 
}

/**
*   @brief  Get a pointer to mixer Master level 2 float variable
*   @param  none
*   @return a pointer to mixer Master level 2 float variable
*/
float *AudioPolyphonyMixerFloat::get_master_level_2_ptr() 
{
	return &master_level_2; 
}

/**
*   @brief  Get a pointer to mixer Master pan 1 float variable
*   @param  none
*   @return a pointer to mixer Master pan 1 float variable
*/
float *AudioPolyphonyMixerFloat::get_master_pan_1_ptr() 
{ 
	return &master_pan_1;
}

/**
*   @brief  Get a pointer to mixer Master pan 2 float variable
*   @param  none
*   @return a pointer to mixer Master pan 2 float variable
*/
float *AudioPolyphonyMixerFloat::get_master_pan_2_ptr() 
{ 
	return &master_pan_2; 
}

/**
*   @brief  Get a pointer to mixer Master send 1 float variable
*   @param  none
*   @return a pointer to mixer Master send 1 float variable
*/
float *AudioPolyphonyMixerFloat::get_master_send_1_ptr() 
{ 
	return &master_send_1; 
}

/**
*   @brief  Get a pointer to mixer Master send 2 float variable
*   @param  none
*   @return a pointer to mixer Master send 2 float variable
*/
float *AudioPolyphonyMixerFloat::get_master_send_2_ptr() 
{ 
	return &master_send_2; 
}

/**
*   @brief  Get a pointer to mixer Program level 1 float variable
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return a pointer to mixer Master level 1 float variable
*/
float *AudioPolyphonyMixerFloat::get_program_level_1_ptr(int prog) 
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		return &program_level_1[prog];
	}	
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get a pointer to mixer Program level 2 float variable
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return a pointer to mixer Master level 2 float variable
*/
float *AudioPolyphonyMixerFloat::get_program_level_2_ptr(int prog) 
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		return &program_level_2[prog];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get a pointer to mixer Program pan 1 float variable
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return a pointer to mixer Master pan 1 float variable
*/
float *AudioPolyphonyMixerFloat::get_program_pan_1_ptr(int prog) 
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		return &program_pan_1[prog];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get a pointer to mixer Program pan 2 float variable
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return a pointer to mixer Master oan 2 float variable
*/
float *AudioPolyphonyMixerFloat::get_program_pan_2_ptr(int prog) 
{
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		return &program_pan_2[prog];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get a pointer to mixer Program send 1 float variable
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return a pointer to mixer Master send 1 float variable
*/
float *AudioPolyphonyMixerFloat::get_program_send_1_ptr(int prog) {
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		return &program_send_1[prog];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get a pointer to mixer Program send 2 float variable
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return a pointer to mixer Master send 2 float variable
*/
float *AudioPolyphonyMixerFloat::get_program_send_2_ptr(int prog) {
	if ((prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		return &program_send_2[prog];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Set the poinetr of a specific voice gain 1 variable to point 
*			at a selected program level 1 variable pointer
*			(This enables changing all voices gains by changing the (single) program level).
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_gain_1_ptr(int voice, int prog)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES) &&
		(prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		gain1[voice] = &program_level_1[prog];
	}
}

/**
*   @brief  Set the poinetr of a specific voice gain 2 variable to point 
*			at a selected program level 2 variable pointer
*			(This enables changing all voices gain by changing the program level).
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_gain_2_ptr(int voice, int prog)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES) &&
		(prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		gain2[voice] = &program_level_2[prog];
	}
}

/**
*   @brief  Set the poinetr of a specific voice pan 1 variable to point 
*			at a selected program pan 1 variable pointer
*			(This enables changing all voices pan by changing the program pan).
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_pan_1_ptr(int voice, int prog)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES) &&
		(prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		pan1[voice] = &program_pan_1[prog];
	}
}

/**
*   @brief  Set the poinetr of a specific voice pan 2 variable to point 
*			at a selected program pan 2 variable pointer
*			(This enables changing all voices pan by changing the program pan).
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_pan_2_ptr(int voice, int prog)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES) &&
		(prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		pan2[voice] = &program_pan_2[prog];
	}
}

/**
*   @brief  Set the poinetr of a specific voice send 1 variable to point
*			at a selected program send 1 variable pointer
*			(This enables changing all voices pan by changing the program send).
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_send_1_ptr(int voice, int prog)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES) &&
		(prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		send1[voice] = &program_send_1[prog];
	}
}

/**
*   @brief  Set the poinetr of a specific voice send 2 variable to point
*			at a selected program send 2 variable pointer
*			(This enables changing all voices pan by changing the program send).
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @param  prog	program number 0 to _SYNTH_NUM_OF_PROGRAMS
*   @return void
*/
void AudioPolyphonyMixerFloat::set_voice_send_2_ptr(int voice, int prog)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES) &&
		(prog >= 0) && (prog < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		send1[voice] = &program_send_2[prog];
	}
}

/**
*   @brief  Get the poinetr of a specific voice gain 1 variable 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return a poinetr of specific voice gain 1 variable 
*/
float *AudioPolyphonyMixerFloat::get_voice_gain_1_ptr(int voice) 
{ 
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{	
		return gain1[voice]; 
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get the poinetr of a specific voice gain 2 variable 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return a poinetr of specific voice gain 2 variable 
*/
float *AudioPolyphonyMixerFloat::get_voice_gain_2_ptr(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{	
		return gain2[voice];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get the poinetr of specific voice pan 1 variable 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return a poinetr of specific voice pan 1 variable 
*/
float *AudioPolyphonyMixerFloat::get_voice_pan_1_ptr(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{	
		return pan1[voice];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get the poinetr of specific voice pan 2 variable 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return a poinetr of specific voice pan 2 variable 
*/
float *AudioPolyphonyMixerFloat::get_voice_pan_2_ptr(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{	
		return pan2[voice];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Get the poinetr of specific voice send 1 variable
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return a poinetr of specific voice send 1 variable
*/
float *AudioPolyphonyMixerFloat::get_voice_send_1_ptr(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
		return send1[voice];
	else
		return NULL;
}

/**
*   @brief  Get the poinetr of specific voice send 2 variable
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return a poinetr of specific voice send 2 variable
*/
float *AudioPolyphonyMixerFloat::get_voice_send_2_ptr(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
		return send2[voice];
	else
		return NULL;
}

/**
*   @brief  Calculate and return specific voice stereo left output 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*	@param	in1		chan 1 signal
*	@param	in2		chan 2 signal
*   @return voice stereo left output
*/
float AudioPolyphonyMixerFloat::get_voice_left_output(int voice, float in1, float in2)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{	
		return (in1 * *gain1[voice] * (1 - *pan1[voice]) + in2 * *gain2[voice] * (1 - *pan2[voice])) * 0.20f;
	}
	else
	{
		return .0f;
	}
}

/**
*   @brief  Calculate and return specific voice stereo right output 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*	@param	in1		chan 1 signal
*	@param	in2		chan 2 signal
*   @return voice stereo right output
*/
float AudioPolyphonyMixerFloat::get_voice_right_output(int voice, float in1, float in2)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{	
		return (in1 * *gain1[voice] * (1 + *pan1[voice]) + in2 * *gain2[voice] * (1 + *pan2[voice])) * 0.20f;
	}
	else
	{
		return 0.f;
	}
}

/**
*   @brief  Save a voice current gain and pan values 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return void
*/
void AudioPolyphonyMixerFloat::preserve_gain_pan(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		preserved_gain_1[voice] = gain1[voice];
		preserved_gain_2[voice] = gain2[voice];
		preserved_pan_1[voice] = pan1[voice];
		preserved_pan_2[voice] = pan2[voice];
		preserved_send_1[voice] = send1[voice];
		preserved_send_2[voice] = send2[voice];
	}
}

/**
*   @brief  Restore a voice current gain and pan values 
*	@param	voice	voice number 0 to _SYNTH_MAX_NUM_OF_VOICES
*   @return void
*/
void AudioPolyphonyMixerFloat::restore_gain_pan(int voice)
{
	if ((voice >= 0) && (voice < _SYNTH_MAX_NUM_OF_VOICES))
	{
		gain1[voice] = preserved_gain_1[voice];
		gain2[voice] = preserved_gain_2[voice];
		pan1[voice] = preserved_pan_1[voice];
		pan2[voice] = preserved_pan_2[voice];
		send1[voice] = preserved_send_1[voice];
		send2[voice] = preserved_send_2[voice];
	}
}

/**
*   @brief  Set LFO 1 frequency 0-100
*			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
*	@param	freq	frequency 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_lfo_1_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate	
	lfo_1_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_1_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{	
		lfo_1_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*   @brief  Set LFO 2 frequency 0-100
*			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
*	@param	freq	frequency 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_lfo_2_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_2_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_2_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_2_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*   @brief  Set LFO 3 frequency 0-100
*			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
*	@param	freq	frequency 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_lfo_3_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_3_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_3_actual_freq > (float)_OSC_MAX_FREQUENCY)
		lfo_3_actual_freq = (float)_OSC_MAX_FREQUENCY;
}

/**
*   @brief  Set LFO 4 frequency 0-100
*			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
*	@param	freq	frequency 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_lfo_4_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_4_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_4_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_4_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*   @brief  Set LFO 5 frequency 0-100
*			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
*	@param	freq	frequency 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_lfo_5_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_5_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_5_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_5_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*   @brief  Set LFO 6 frequency 0-100
*			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
*	@param	freq	frequency 0-100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_lfo_6_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_6_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_6_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_6_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*	@brief	Set LFO 1 waveform
*	@param wform waveform
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_1_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo1->set_waveform(wf);
	}
}

/**
*	@brief	Set LFO 2 waveform
*	@param wform waveform
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_2_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo2->set_waveform(wf);
	}
}

/**
*	@brief	Set LFO 3 waveform
*	@param wform waveform
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_3_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo3->set_waveform(wf);
	}
}

/**
*	@brief	Set LFO 4 waveform
*	@param wform waveform
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_4_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo4->set_waveform(wf);
	}
}

/**
*	@brief	Set LFO 5 waveform
*	@param wform waveform
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_5_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo5->set_waveform(wf);
	}
}

/**
*	@brief	Set LFO 6 waveform
*	@param wform waveform
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_6_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo6->set_waveform(wf);
	}
}

/**
*	@brief	Set LFO 1 symmetry
*	@param sym	symmetry
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_1_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}
	
	lfo1->set_pwm_dcycle(symmetry);
}

/**
*	@brief	Set LFO 2 symmetry
*	@param sym	symmetry
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_2_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}
	
	lfo2->set_pwm_dcycle(symmetry);
}

/**
*	@brief	Set LFO 3 symmetry
*	@param sym	symmetry
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_3_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}
	
	lfo3->set_pwm_dcycle(symmetry);
}

/**
*	@brief	Set LFO 4 symmetry
*	@param sym	symmetry
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_4_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}
	
	lfo4->set_pwm_dcycle(symmetry);
}

/**
*	@brief	Set LFO 5 symmetry
*	@param sym	symmetry
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_5_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}
	
	lfo5->set_pwm_dcycle(symmetry);
}
/**
*	@brief	Set LFO 6 symmetry
*	@param sym	symmetry
*	@return void
*/
void AudioPolyphonyMixerFloat::set_lfo_6_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}

	lfo6->set_pwm_dcycle(symmetry);
}

/**
*   @brief  Calculate next LFO modulation values.
*	@param	none
*   @return void
*/
void AudioPolyphonyMixerFloat::calc_next_modulation_values()
{
	lfo_out[0] = lfo1->get_next_output_val(lfo_1_actual_freq);
	lfo_out[1] = lfo2->get_next_output_val(lfo_2_actual_freq);
	lfo_out[2] = lfo3->get_next_output_val(lfo_3_actual_freq);
	lfo_out[3] = lfo4->get_next_output_val(lfo_4_actual_freq);
	lfo_out[4] = lfo5->get_next_output_val(lfo_5_actual_freq);
	lfo_out[5] = lfo6->get_next_output_val(lfo_6_actual_freq);

	if (amp_1_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_1_pan_lfo_modulation(amp_1_pan_mod_lfo_level, lfo_out[amp_1_pan_mod_lfo - 1]);
	}
	else
	{
		set_amp_1_pan_lfo_modulation(0.0f, 0.0f);
	}

	if (amp_2_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_2_pan_lfo_modulation(amp_2_pan_mod_lfo_level, lfo_out[amp_2_pan_mod_lfo - 1]);
	}
	else
	{
		set_amp_2_pan_lfo_modulation(0.0f, 0.0f);
	}
}

/**
*   @brief  Set amp 1 pan lfo modulation modulation.
*	@param	modFactor	modulation factor (depth)
*	@param	modVal		modulation signal value
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_1_pan_lfo_modulation(float mod_factor, float mod_val)
{
	amp_1_pan_mod = mod_factor * mod_val + master_pan_1;
	if (amp_1_pan_mod < -1.0f)
	{	
		amp_1_pan_mod = -1.0f; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	else if (amp_1_pan_mod > 1.0f)
	{	
		amp_1_pan_mod = 1.0f;
	}

	//	printf("fac1 %f  lev1 %f mod1 %f\n", modFactor, modVal, amp1panMod);
}

/**
*   @brief  Set amp 2 pan lfo modulation modulation.
*	@param	modFactor	modulation factor (depth)
*	@param	modVal		modulation signal value
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_2_pan_lfo_modulation(float mod_factor, float mod_val)
{
	amp_2_pan_mod = mod_factor * mod_val + master_pan_2;
	if (amp_2_pan_mod < -1.0f)
	{	
		amp_2_pan_mod = -1.0f; // <<<<<<<<<<<<<<<<<<<<<
	}
	else if (amp_2_pan_mod > 1.0f)
	{	
		amp_2_pan_mod = 1.0f;
	}
}

/**
*   @brief  Calculate next LFO modulation values.
*	@param	valueCh1	pointer to a float variable to hold ch1 modulation value
*	@param	valueCh2	pointer to a float variable to hold ch2 modulation value
*	@param	none
*   @return void
*/
void AudioPolyphonyMixerFloat::calc_next_modulation_values(float *value_ch1, float *value_ch2)
{	
	lfo_out[0] = lfo1->get_next_output_val(lfo_1_actual_freq);
	lfo_out[1] = lfo2->get_next_output_val(lfo_2_actual_freq);
	lfo_out[2] = lfo3->get_next_output_val(lfo_3_actual_freq);
	lfo_out[3] = lfo4->get_next_output_val(lfo_4_actual_freq);
	lfo_out[4] = lfo5->get_next_output_val(lfo_5_actual_freq);
	lfo_out[5] = lfo6->get_next_output_val(lfo_6_actual_freq);

	if (amp_1_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_1_pan_lfo_modulation_value(amp_1_pan_mod_lfo_level, lfo_out[amp_1_pan_mod_lfo - 1], value_ch1);
	}
	else
	{
		set_amp_1_pan_lfo_modulation_value(0.0f, 0.0f, value_ch1);
	}

	if (amp_2_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_2_pan_lfo_modulation_value(amp_2_pan_mod_lfo_level, lfo_out[amp_2_pan_mod_lfo - 1], value_ch2);
	}
	else
	{
		set_amp_2_pan_lfo_modulation_value(0.0f, 0.0f, value_ch2);
	}
}

/**
*   @brief  Set amp 1 pan lfo modulation value.
*	@param	modFactor	modulation factor (depth)
*	@param	modVal		modulation signal value
*	@param	value		pointer to a float variable to hold new modulation value
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_1_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value)
{
	*(value) = mod_factor * mod_val + master_pan_1;
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
*   @brief  Set amp 2 pan lfo modulation value.
*	@param	modFactor	modulation factor (depth)
*	@param	modVal		modulation signal value
*	@param	value		pointer to a float variable to hold modulation valu
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_2_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value)
{
	*(value) = mod_factor * mod_val + master_pan_1;
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
*   @brief  Set amp 1 modulation LFO number.
*	@param	Lfo		LFO number _LFO_NONE to  _LFO_6
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_1_pan_mod_lfo(int lfo) 
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_6))
	{
		amp_1_pan_mod_lfo = lfo;
	}
}

/**
*   @brief  Set amp 1 modulation LFO level.
*	@param	Lev		Modulation level 0 to 100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_1_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100)) 
	{
		amp_1_pan_mod_lfo_level = (float)lev / 100.0;
	} 
}

/**
*   @brief  Set amp 2 modulation LFO number.
*	@param	Lfo		LFO number _LFO_NONE to  _LFO_5
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_2_pan_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_3))
	{
		amp_2_pan_mod_lfo = lfo;
	}
}

/**
*   @brief  Set amp 2 modulation LFO level.
*	@param	Lev		Modulation level 0 to 100
*   @return void
*/
void AudioPolyphonyMixerFloat::set_amp_2_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		amp_2_pan_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*   @brief  Execute an update cycle - get input samples, process and send to
*			next audio block stage..
*   @param  none
*   @return void
*/
void AudioPolyphonyMixerFloat::update()
{
	audio_block_float_mono_t *block_out_L, *block_out_R, *block_send_L, *block_send_R;
	int voice, i;

	/* Holds pan modulation values at sub sampling rate */
	float amp_1_pan_mod_samp[_AUDIO_MAX_BUF_SIZE / _CONTROL_SUB_SAMPLING + 1];
	float amp_2_pan_mod_samp[_AUDIO_MAX_BUF_SIZE / _CONTROL_SUB_SAMPLING + 1];
	
	int j;
	// Fill PAN modulation arrays with subsampled modulation values.
	for (j = 0; j < audio_block_size / _CONTROL_SUB_SAMPLING + 1; j++)
	{
		calc_next_modulation_values(&amp_1_pan_mod_samp[j], &amp_2_pan_mod_samp[j]);
	}

	if (active)
	{
		// Allocate output blocks
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		block_out_L = allocate_audio_block();
		block_out_R = allocate_audio_block();
		block_send_L = allocate_audio_block();
		block_send_R = allocate_audio_block();
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);

		float left_gain_1, left_gain_2, right_gain_1, right_gain_2;
		float left_send_1, left_send_2, right_send_1, right_send_2;


		if (!block_out_L || !block_out_R || !block_send_L || !block_send_R)
		{
			// unable to allocate memory, so we'll release what we could get, send nothing, and return.
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

		j = 0;

		// voice 0 - it is the 1st voice, so its samples will be used as base for all other voices.
		if (voice_is_active(0) || voice_waits_for_not_active(0))
		{
			for (i = 0; i < audio_block_size; i++) 
			{
				// Update modulation factors at sub sampling rate.
				if ((i % _CONTROL_SUB_SAMPLING) == 0)
				{
					left_gain_1 = *gain1[0] * (1 - *pan1[0]) * (1 - amp_1_pan_mod_samp[j]) * master_level_1 * 0.1f;
					left_gain_2 = *gain2[0] * (1 - *pan2[0]) * (1 - amp_2_pan_mod_samp[j]) * master_level_2 * 0.1f;
					right_gain_1 = *gain1[0] * (1 + *pan1[0]) * (1 + amp_1_pan_mod_samp[j]) * master_level_1 * 0.1f;
					right_gain_2 = *gain2[0] * (1 + *pan2[0]) * (1 + amp_2_pan_mod_samp[j]) * master_level_2 * 0.1f;

					if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
					{
						// In mapping mode use each voice send value for send calculation.
						left_send_1 = *send1[0] * (1 - *pan1[0]) * (1 - amp_1_pan_mod) * master_level_1 * 0.1f;
						left_send_2 = *send2[0] * (1 - *pan2[0]) * (1 - amp_2_pan_mod) * master_level_2 * 0.1f;
						right_send_1 = *send1[0] * (1 + *pan1[0]) * (1 + amp_1_pan_mod) * master_send_1 * 0.1f;
						right_send_2 = *send2[0] * (1 + *pan2[0]) * (1 + amp_2_pan_mod) * master_send_2 * 0.1f;
					}
					else
					{
						// In non-mapping mode use master send value for send calculation.
						left_send_1 = master_send_1 * (1 - master_pan_1) * (1 - amp_1_pan_mod) * 0.1f;
						left_send_2 = master_send_2 * (1 - master_pan_2) * (1 - amp_2_pan_mod) * 0.1f;
						right_send_1 = master_send_1 * (1 + master_pan_1) * (1 + amp_1_pan_mod) * 0.1f;
						right_send_2 = master_send_2 * (1 + master_pan_2) * (1 + amp_2_pan_mod) * 0.1f;
					}
					
					j++;
				}

				// Calculate all block output samples for voice 0
				block_out_L->data[i] =
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][i] * left_gain_1 +
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][i] * left_gain_2;
				block_out_R->data[i] =
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][i] * right_gain_1 +
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][i] * right_gain_2;

				block_send_L->data[i] =
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][i] * left_send_1 +
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][i] * left_send_2;
				block_send_R->data[i] =
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][i] * right_send_1 +
					polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][i] * right_send_2;
			}
		}
		else
		{
			// voice not active - zero values output
			for (i = 0; i < audio_block_size; i++)
			{
				block_out_L->data[i] = 0;
				block_out_R->data[i] = 0;
				block_send_L->data[i] = 0;
				block_send_R->data[i] = 0;
			}
		}
		
		// All other voices: accumulate samples values to voice[0] samples or to 0 if voice[0] was not active.
		for (voice = 1; voice < inputs; voice++)
		{
			j = 0;
			if (voice_is_active(voice) || voice_waits_for_not_active(voice))
			{
				for (i = 0; i < audio_block_size; i++)
				{
					// Update modulation factors at sub sampling rate.
					if ((i % _CONTROL_SUB_SAMPLING) == 0)
					{
						left_gain_1 = *gain1[voice] * (1 - *pan1[voice]) * (1 - amp_1_pan_mod_samp[j]) * master_level_1 * 0.2f;
						left_gain_2 = *gain2[voice] * (1 - *pan2[voice]) * (1 - amp_2_pan_mod_samp[j]) * master_level_2 * 0.2f;
						right_gain_1 = *gain1[voice] * (1 + *pan1[voice]) * (1 + amp_1_pan_mod_samp[j]) * master_level_1 * 0.2f;
						right_gain_2 = *gain2[voice] * (1 + *pan2[voice]) * (1 + amp_2_pan_mod_samp[j]) * master_level_2 * 0.2f;

						if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
						{
							// In mapping mode use each voice send value for send calculation.
							left_send_1 = *send1[0] * (1 - *pan1[0]) * (1 - amp_1_pan_mod) * master_level_1 * 0.1f;
							left_send_2 = *send2[0] * (1 - *pan2[0]) * (1 - amp_2_pan_mod) * master_level_2 * 0.1f;
							right_send_1 = *send1[0] * (1 + *pan1[0]) * (1 + amp_1_pan_mod) * master_send_1 * 0.1f;
							right_send_2 = *send2[0] * (1 + *pan2[0]) * (1 + amp_2_pan_mod) * master_send_2 * 0.1f;
						}
						else
						{
							// In non-mapping mode use master send value for send calculation.
							left_send_1 = master_send_1 * (1 - master_pan_1) * (1 - amp_1_pan_mod) * 0.1f;
							left_send_2 = master_send_2 * (1 - master_pan_2) * (1 - amp_2_pan_mod) * 0.1f;
							right_send_1 = master_send_1 * (1 + master_pan_1) * (1 + amp_1_pan_mod) * 0.1f;
							right_send_2 = master_send_2 * (1 + master_pan_2) * (1 + amp_2_pan_mod) * 0.1f;
						}

						j++;
					}

					// Accumulate all block output samples for all other voices
					block_out_L->data[i] +=
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][i] * left_gain_1 +
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][i] * left_gain_2;
					block_out_R->data[i] +=
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][i] * right_gain_1 +
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][i] * right_gain_2;

					block_send_L->data[i] +=
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][i] * left_send_1 +
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][i] * left_send_2;
					block_send_R->data[i] +=
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][i] * right_send_1 +
						polyphony_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][i] * right_send_2;
				}
			}
		}
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

//		if (RiffWave::getInstance()->getRecordingLength() >= 441000)
//			RiffWave::getInstance()->stopRecording();
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
	}
}

