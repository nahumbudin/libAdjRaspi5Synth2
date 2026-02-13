/**
*	@file		dspVoicePad.cpp
*	@author		Nahum Budin
*	@date		17-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion. 
*					
*	@History	
*				version	1.1	25_Jan-2021
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate and bloc-size settings
*				version	1.0	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - PAD Wavetable Generator Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Pad_1 send level to channel 1
*	@param lev Pad1 send level to channel 1 0.0 to 1.0
*	@return void
*/
void DSP_Voice::set_pad_1_send_filter_1_level(float lev)
{
	wavetable_1_send_filter_1_level = lev;
	if (wavetable_1_send_filter_1_level < 0.0f)
	{
		wavetable_1_send_filter_1_level = 0.0f;
	}
	else if (wavetable_1_send_filter_1_level > 1.0f)
	{
		wavetable_1_send_filter_1_level = 1.0f;
	}
}

/**
*	@brief	Set Pad_1 send level to channel 1 (int)
*	@param lev Pad_1 send level to channel 1 0-100
*	@return void
*/
void DSP_Voice::set_pad_1_send_filter_1_level(int lev)
{
	set_pad_1_send_filter_1_level((float)lev / 100.0f);
}

/**
*	@brief	return Pad_1 send level to channel 1
*	@param none
*	@return Pad1 send level to channel 1
*/
float DSP_Voice::get_pad_1_send_filter_1_level() { return wavetable_1_send_filter_1_level; }

/**
*	@brief	Set Pad_1 send level to channel 2
*	@param lev Pad_1 send level to channel 2 0.0 to 1.0
*	@return void
*/
void DSP_Voice::set_pad_1_send_filter_2_level(float lev)
{
	wavetable_1_send_filter_2_level = lev;
	if (wavetable_1_send_filter_2_level < 0.0f)
	{
		wavetable_1_send_filter_2_level = 0.0f;
	}
	else if (wavetable_1_send_filter_2_level > 1.0f)
	{
		wavetable_1_send_filter_2_level = 1.0f;
	}
}

/**
*	@brief	Set Pad_1 send level to channel 2 (int)
*	@param lev Pad_1 send level to channel 2 (int) 0 to 100
*	@return void
*/
void DSP_Voice::set_pad_1_send_filter_2_level(int lev)
{
	set_pad_1_send_filter_2_level((float)lev / 100.0f);
}

/**
*	@brief	return Pad_1 send level to channel 2
*	@param none
*	@return Pad1 send level to channel 2
*/
float DSP_Voice::get_pad_1_send_filter_2_level() { return wavetable_1_send_filter_2_level; }

/**
*	@brief	Set PAD_1 active LFO frequency modulator and mode (delayed or not)
*	@param	lfo PAD_1 active LFO frequency modulator and mode _LFO_NONE to LFO_6_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_pad_1_freq_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_6_DELAYED_2000MS))
	{
		wavetable_1_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		wavetable_1_freq_mod_lfo_delay = AdjSynth::lfo_delays[lfo];
	}
}

/**
*	@brief	Set PAD_1 active LFO frequency modulator level
*	@param	lev PAD_1 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_pad_1_freq_mod_lfo_level(int lev)
{
	if ((lev >= 0) && (lev <= 100))
	{
		wavetable_1_freq_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); 
	}
}

/**
*	@brief	Set PAD_1 active ENV frequency modulator
*	@param	env PAD_1 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_Voice::set_pad_1_freq_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_3))
	{
		wavetable_1_freq_mod_env = env;
	}
}

/**
*	@brief	Set PAD_1 active ENV frequency modulator level
*	@param	lev PAD_1 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_pad_1_freq_mod_env_level(int lev)
{
	if ((lev >= 0) && (lev <= 100))
	{
		wavetable_1_freq_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev);  
	}
}

/**
*	@brief	Set PAD_1 active LFO amplitude modulator and mode (delayed or not)
*	@param	lfo PAD_1 active LFO amplitude modulator and mode _LFO_NONE to LFO_6_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_pad_1_amp_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_6_DELAYED_2000MS))
	{
		wavetable_1_amp_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		wavetable_1_amp_mod_lfo_delay = AdjSynth::lfo_delays[lfo];
	}
}

/**
*	@brief	Set PAD_1 active LFO amplitude modulator level
*	@param	lev PAD_1 active LFO amplitude modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_pad_1_amp_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		wavetable_1_amp_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set PAD_1 active ENV amplitude modulator
*	@param	env PAD_1 active ENV amplitude modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_Voice::set_pad_1_amp_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_3))
	{
		wavetable_1_amp_mod_env = env;
	}
}

/**
*	@brief	Set PAD_1 active ENV amplitude modulator level
*	@param	lev PAD_1 active ENV amplitude modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_pad_1_amp_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		wavetable_1_amp_mod_env_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Return PAD_1 active LFO frequency modulator number
*	@param	none
*	@return PAD1 active LFO frequency modulator number
*/
int DSP_Voice::get_pad_1_freq_mod_lfo() 
{ 
	return wavetable_1_freq_mod_lfo; 
}

/**
*	@brief	Return PAD_1 active LFO frequency modulator level
*	@param	none
*	@return PAD1 active LFO frequency modulator level
*/
float DSP_Voice::get_pad_1_freq_mod_lfo_lLevel() 
{ 
	return wavetable_1_freq_mod_lfo_level; 
}

/**
*	@brief	Return PAD_1 active ENV frequency modulator number
*	@param	none
*	@return PAD_1 active ENV frequency modulator number
*/
int DSP_Voice::get_pad_1_freq_mod_env() 
{ 
	return wavetable_1_freq_mod_env; 
}

/**
*	@brief	Return PAD_1 active ENV frequency modulator level
*	@param	none
*	@return PAD_1 active ENV frequency modulator number
*/
float DSP_Voice::get_pad_1_freq_mod_env_level() { return wavetable_1_freq_mod_env_level; }

/**
*	@brief	Return PAD_1 active LFO frequency modulator number
*	@param	none
*	@return PAD_1 active ENV frequency modulator number
*/
int DSP_Voice::get_pad_1_amp_mod_lfo() 
{ 
	return wavetable_1_freq_mod_lfo; 
}

/**
*	@brief	Return PAD_1 active LFO frequency modulator level
*	@param	none
*	@return PAD1 active ENV frequency modulator level
*/
float DSP_Voice::get_pad_1_amp_mod_lfo_level() 
{ 
	return wavetable_1_amp_mod_lfo_level; 
}

/**
*	@brief	Return PAD_1 active LFO pwm modulator number
*	@param	none
*	@return PAD_1 active LFO pwm modulator number
*/
int DSP_Voice::get_pad_1_amp_mod_env() 
{ 
	return wavetable_1_amp_mod_env; 
}

/**
*	@brief	Return PAD_1 active LFO pwm modulator level
*	@param	none
*	@return PAD_1 active LFO pwm modulator level
*/
float DSP_Voice::get_pad_1_amp_mod_env_level() { return wavetable_1_amp_mod_env_level; }

/**
*	@brief	Set PAD_1 active LFO amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_pad_1_amp_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val; // Modulation value (LFO: -1.0 to 1.0; Env: 0 to 1.0)
	
	if (adsr_1->note_on_elapsed_time < wavetable_1_amp_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
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

	wavetable_1_amp_lfo_modulation = 1 - (1.0f + value) * factor / 2.0f;
}

/**
*	@brief	Set PAD_1 active ENV amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_Voice::set_pad_1_amp_env_modulation(float mod_factor, float mod_val)
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

	wavetable_1_amp_env_modulation = factor * value;
}

/**
*	@brief	Set PAD_1 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_pad_1_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < wavetable_1_freq_mod_lfo_delay)
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

	wavetable_1_freq_lfo_modulation = factor * value;
}

/**
*	@brief	Set PAD_1 active ENV frequency modulator modulation value
*	@param	modFactor	modulation factor (depth) 0.0 to 1.0
*	@param	modValue	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_Voice::set_pad_1_freq_env_modulation(float mod_factor, float mod_val)
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

	wavetable_1_freq_env_modulation = factor * value;
}
