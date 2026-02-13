/**
*	@file		dspVoiceAmp.cpp
*	@author		Nahum Budin
*	@date		17-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	
*				version	1.1	28_Jan-2021	Code refactoring and notaion
*				version	1.0	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Amp Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Amp1 ch1 active LFO pan modulator and mode (delayed or not)
*	@param	lfo Amp1 ch1 active LFO pan modulator and mode _LFO_NONE to LFO_6_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_amp_1_ch_1_pan_mod_lfo(int lfo) {
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_6_DELAYED_2000MS))
	{
		amp_1_pan_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		amp_1_pan_mod_lfo_delay = AdjSynth::lfo_delays[lfo];
	} 
}

/**
*	@brief	Set Amp1 ch1 active LFO pan modulator level
*	@param	lev Amp1 ch1 active LFO pan modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_amp_1_ch_1_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		amp_1_pan_mod_lfo_level = (float)lev / 100.0; 
	}
}

/**
*	@brief	Set Amp1 ch2 active LFO pan modulator and mode (delayed or not)
*	@param	lfo Amp1 ch2 active LFO pan modulator and mode _LFO_NONE to LFO_6_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_amp_1_ch_2_pan_mod_lfo(int lfo) 
{ 
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_6_DELAYED_2000MS))
	{
		amp_2_pan_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		amp_2_pan_mod_lfo_delay = AdjSynth::lfo_delays[lfo];
	}
}

/**
*	@brief	Set Amp_1 ch2 active LFO pan modulator level
*	@param	lev Amp_1 ch2 active LFO pan modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_amp_1_ch_2_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		amp_2_pan_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Return Amp_1 ch1 active LFO pan modulator number
*	@param	none
*	@return Amp_1 ch1 active LFO pan modulator number
*/
int DSP_Voice::get_amp_1_ch_1_pan_lfo_modulation() 
{ 
	return amp_1_pan_mod_lfo; 
}

/**
*	@brief	Return Amp_1 ch1 active LFO pan modulator level
*	@param	none
*	@return Amp_1 ch1 active LFO pan modulator level
*/
float DSP_Voice::get_amp_1_ch_1_pan_lfo_modulation_level() 
{ 
	return amp_1_pan_mod_lfo_level; 
}

/**
*	@brief	Return Amp_1 ch2 active LFO pan modulator number
*	@param	none
*	@return Amp_1 ch2 active LFO pan modulator number
*/
int DSP_Voice::get_amp_1_ch_2_pan_lfo_modulation() 
{ 
	return amp_2_pan_mod_lfo; 
}

/**
*	@brief	Return Amp_1 ch2 active LFO pan modulator level
*	@param	none
*	@return Amp1 ch2 active LFO pan modulator level
*/
float DSP_Voice::get_amp_1_ch_2_pan_lfo_modulation_level() 
{ 
	return amp_2_pan_mod_lfo_level; 
}

/**
*	@brief	Set Amp_1 ch1active LFO pan modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_amp_1_ch_1_pan_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < amp_1_pan_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}
	
	if (value > 1.0)
	{
		value = 1.0;
	}
	else if (value < -1.0)
	{
		value = 1.0;
	}
	
	amp_1_pan_mod = mod_factor * value + out_amp_1->get_ch1_pan();
	if (amp_1_pan_mod < -1.0f)
	{
		amp_1_pan_mod = 1.0f;
	}
	else if (amp_1_pan_mod > 1.0f)
	{
		amp_1_pan_mod = 1.0f;
	}
}

/**
*	@brief	Set Amp_1 ch2 active LFO pan modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_amp_1_ch_2_pan_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < amp_2_pan_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	} 
	
	if (value > 1.0)
	{
		value = 1.0;
	}
	else if (value < -1.0)
	{
		value = 1.0;
	}
	
	amp_2_pan_mod = mod_factor * value + out_amp_1->get_ch2_pan();
	if (amp_2_pan_mod < -1.0f)
	{
		amp_2_pan_mod = 1.0f;
	}
	else if (amp_2_pan_mod > 1.0f)
	{
		amp_2_pan_mod = 1.0f;
	}
}

