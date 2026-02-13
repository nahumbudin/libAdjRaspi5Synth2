/**
*	@file		dspAdjSynthVoiceNoise.cpp
*	@author		Nahum Budin
*	@date		21_Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	1.1 25_Jan-2021 Code refactoring and notaion.
*				1.0 9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Noise Handling (dspAdjSynthVoice.h).
*				
*/

#include "dspAdjSynthVoice.h"

/**
*	@brief	Set Noise_1 send level to channel 1
*	@param lev Noise_1 send level to channel 1 0.0 to 1.0
*	@return void
*/
void DSP_AdjSynthVoice::set_noise_1_send_filter_1_level(float lev)
{
	noise_1_send_filter_1_level = lev;
	
	if (noise_1_send_filter_1_level < 0.0f)
	{
		noise_1_send_filter_1_level = 0.0f;
	}
	else if (noise_1_send_filter_1_level > 1.0f)
	{
		noise_1_send_filter_1_level = 1.0f;
	}
}

/**
*	@brief	Set Noise_1 send level to channel 1 (int)
*	@param lev Noise_1 send level to channel 1 0-100
*	@return void
*/
void DSP_AdjSynthVoice::set_noise_1_send_filter_1_level(int lev)
{
	set_noise_1_send_filter_1_level((float)lev / 100.0f);
}

/**
*	@brief	return ?Noise_1 send level to channel 1
*	@param none
*	@return Noise_1 send level to channel 1
*/
float DSP_AdjSynthVoice::get_noise_1_send_filter_1_level() 
{ 
	return noise_1_send_filter_1_level; 
}

/**
*	@brief	Set Noise_1 send level to channel 2
*	@param lev Noise_1 send level to channel 2 0.0 to 1.0
*	@return void
*/
void DSP_AdjSynthVoice::set_noise_1_send_filter_2_level(float lev)
{
	noise_1_send_filter_2_level = lev;
	
	if (noise_1_send_filter_2_level < 0.0f)
	{
		noise_1_send_filter_2_level = 0.0f;
	}
	else if (noise_1_send_filter_2_level > 1.0f)
	{
		noise_1_send_filter_2_level = 1.0f;
	}
}

/**
*	@brief	Set Noise_1 send level to channel 2 (int)
*	@param lev Noise_1 send level to channel 2 (int) 0 to 100
*	@return void
*/
void DSP_AdjSynthVoice::set_noise_1_send_filter_2_level(int lev)
{
	set_noise_1_send_filter_2_level((float)lev / 100.0f);
}

/**
*	@brief	return Noise_1 send level to channel 2
*	@param none
*	@return Noise_1 send level to channel 2
*/
float DSP_AdjSynthVoice::get_noise_1_send_filter_2_level() 
{ 
	return noise_1_send_filter_2_level; 
}

/**
*	@brief	Set Noise_1 active LFO amplitude modulator and mode (delayed or not)
*	@param	lfo Noise_1 active LFO amplitude modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_noise_1_amp_mod_lfo(int lfo) 
{ 
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		noise_1_amp_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		noise_1_amp_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set Noise_1 active LFO amplitude modulator level
*	@param	lev Noise_1 active LFO amplitude modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_noise_1_amp_mod_lfo_level(int lev) 
{
	if ((lev >= 0) && (lev <= 100))
	{
		noise_1_amp_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set Noise_1 active ENV amplitude modulator
*	@param	env Noise_1 active ENV amplitude modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_noise_1_amp_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		noise_1_amp_mod_env = env;
	}
}

/**
*	@brief	Set Noise_1 active ENV amplitude modulator level
*	@param	lev Noise_1 active ENV amplitude modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_noise_1_amp_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		noise_1_amp_mod_env_level = (float)lev / 100.0;
	}

}

/**
*	@brief	Return Noise_1 active LFO amplitude modulator number
*	@param	none
*	@return Noise1 active LFO amplitude modulator number
*/
int DSP_AdjSynthVoice::get_noise_1_amp_mod_lfo() 
{ 
	return noise_1_amp_mod_lfo; 
}

/**
*	@brief	Return Noise_1 active LFO amplitude modulator level
*	@param	none
*	@return Noise_1 active LFO amplitude modulator level
*/
float DSP_AdjSynthVoice::get_noise_1_amp_mod_lfo_level() 
{ 
	return noise_1_amp_mod_lfo_level; 
}

/**
*	@brief	Return Noise_1 active ENV amplitude modulator number
*	@param	none
*	@return Noise_1 active ENV amplitude modulator number
*/
int DSP_AdjSynthVoice::get_noise_1_amp_mod_env() 
{ 
	return noise_1_amp_mod_env; 
}

/**
*	@brief	Return Noise_1 active ENV amplitude modulator level
*	@param	none
*	@return Noise_1 active ENV amplitude modulator level
*/
float DSP_AdjSynthVoice::get_noise_1_amp_mod_env_level() 
{ 
	return noise_1_amp_mod_env_level; 
}

/**
*	@brief	Set Noise_1 active LFO amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_noise_1_amp_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val; // Modulation value (LFO: -1.0 to 1.0; Env: 0 to 1.0)
	
	if (adsr_1 != NULL)
	{
		if (adsr_1->note_on_elapsed_time < noise_1_amp_mod_lfo_delay) // All ADSRs get starting together
		{
			// Delayed activation
			value = 0.5f;
		}
	}
	
	float factor = mod_factor; // 0 to 1.0 : indicates the modulator send set-level
	
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}
	
	noise_1_amp_lfo_modulation_value = 1 - (1.0f + value) * factor / 2.0f;
}

/**
*	@brief	Set Noise_1 active ENV amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_noise_1_amp_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	float value = mod_val;  
	
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}
	
	noise_1_amp_env_modulation_value = factor * value; 
}

