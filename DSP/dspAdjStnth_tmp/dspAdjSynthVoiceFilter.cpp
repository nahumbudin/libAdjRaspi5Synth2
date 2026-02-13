/**
*	@file		dspAdjSynthVoiceFilter.cpp
*	@author		Nahum Budin
*	@date		27--Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	28-Jan-2021	1.1	Code refactoring and notaion.
*				9-Jun-2018	1.0	(modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Filters Handling.
*				
*/

#include "dspAdjSynthVoice.h"

/**
*	@brief	Set Filter_1 active LFO frequency modulator and mode (delayed or not)
*	@param	lfo Filter_1 active LFO frequency modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_1_freq_mod_lfo(int lfo) 
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		filter_1_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		filter_1_freq_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set Filter_1 active LFO frequency modulator level
*	@param	lev Filter_1 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_1_freq_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		filter_1_freq_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set Filter_1 active ENV frequency modulator
*	@param	env Filter_1 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_1_freq_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		filter_1_freq_mod_env = env;
	}
}

/**
*	@brief	Set Filter_1 active ENV frequency modulator level
*	@param	lev Filter_1 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_1_freq_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100)) 
	{ 
		filter_1_freq_mod_env_level = (float)lev / 100.0; 
	}
}

/**
*	@brief	Return Filter_1 active LFO frequency modulator number
*	@param	none
*	@return Filter_1 active LFO frequency modulator number
*/
int DSP_AdjSynthVoice::get_filter_1_freq_mod_lfo() 
{ 
	return filter_1_freq_mod_lfo; 
}

/**
*	@brief	Return Filter_1 active LFO frequency modulator level
*	@param	none
*	@return Filter_1 active LFO frequency modulator level
*/
float DSP_AdjSynthVoice::get_filter_1_freq_mod_lfo_level() 
{ 
	return filter_1_freq_mod_lfo_level; 
}

/**
*	@brief	Return Filter_1 active ENV frequency modulator number
*	@param	none
*	@return Filter_1 active ENV frequency modulator number
*/
int DSP_AdjSynthVoice::get_filter_1_freq_mod_env() 
{ 
	return filter_1_freq_mod_env; 
}

/**
*	@brief	Return Filter_1 active ENV frequency modulator level
*	@param	none
*	@return Filter1 active ENV frequency modulator level
*/
float DSP_AdjSynthVoice::get_filter_1_freq_mod_env_level() 
{ 
	return filter_1_freq_mod_env_level; 
}

/**
*	@brief	Set Filter_1 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_1_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < filter_1_freq_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}
	
	float factor = mod_factor;
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
	
	filter_1_freq_lfo_modulation_value = factor * value / 0.8f; 
}

/**
*	@brief	Set Filter_1 active ENV frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_1_freq_env_modulation(float mod_factor, float mod_val)
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
	
	filter_1_freq_env_modulation_value = factor * value; 
}

/**
*	@brief	Set Filter_2 active LFO frequency modulator and mode (delayed or not)
*	@param	lfo Filter_2 active LFO frequency modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_2_freq_mod_lfo(int lfo) {
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		filter_2_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		filter_2_freq_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set Filter_2 active LFO frequency modulator level
*	@param	lev Filter_2 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_2_freq_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		filter_2_freq_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set Filter_2 active ENV frequency modulator
*	@param	env Filter_2 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_2_freq_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		filter_2_freq_mod_env = env;
	}
}

/**
*	@brief	Set Filter_2 active ENV frequency modulator level
*	@param	lev Filter_2 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_2_freq_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100)) 
	{
		filter_2_freq_mod_env_level = (float)lev / 100.0; 
	} 
}

/**
*	@brief	Return Filter_2 active LFO frequency modulator number
*	@param	none
*	@return Filter_2 active LFO frequency modulator number
*/
int DSP_AdjSynthVoice::get_filter_2_freq_mod_lfo() 
{ 
	return filter_2_freq_mod_lfo;
}

/**
*	@brief	Return Filter_2 active LFO frequency modulator level
*	@param	none
*	@return Filter_2 active LFO frequency modulator level
*/
float DSP_AdjSynthVoice::get_filter_2_freq_mod_lfo_level() 
{ 
	return filter_2_freq_mod_lfo_level; 
}

/**
*	@brief	Return Filter_2 active ENV frequency modulator number
*	@param	none
*	@return Filter_2 active ENV frequency modulator number
*/
int DSP_AdjSynthVoice::get_filter_2_freq_mod_env() 
{ 
	return filter_2_freq_mod_env; 
}

/**
*	@brief	Return Filter_2 active ENV frequency modulator level
*	@param	none
*	@return Filter_2 active ENV frequency modulator level
*/
float DSP_AdjSynthVoice::get_filter_2_freq_mod_env_level() 
{ 
	return filter_2_freq_mod_env_level; 
}

/**
*	@brief	Set Filter_2 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_2_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < filter_2_freq_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}
	
	float factor = mod_factor;
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
	
	filter_2_freq_lfo_modulation_value = factor * value / 0.8f; 
}

/**
*	@brief	Set Filter_2 active ENV frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_filter_2_freq_env_modulation(float mod_factor, float mod_val)
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
	
	filter_2_freq_env_modulation_value = factor * value; 
}

