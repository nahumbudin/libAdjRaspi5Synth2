/**
*	@file		dspAdjSynthVoiceAmp.cpp
*	@author		Nahum Budin
*	@date		27-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	28-Jan-2021	1.1	Code refactoring and notaion.
*				9-Jun-2018	1.0 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Amp Handling.
*				
*/

#include "dspAdjSynthVoice.h"

/**
*	@brief	Set Amp_1 ch_1 active LFO pan modulator and mode (delayed or not)
*	@param	lfo Amp_1 ch_1 active LFO pan modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_amp_1_ch_1_pan_mod_lfo(int lfo) {
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		amp_1_ch_1_pan_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		amp_1_ch_1_pan_mod_lfo_delay = lfo_delays[lfo];
	} 
}

/**
*	@brief	Set Amp_1 ch_1 active LFO pan modulator level
*	@param	lev Amp_1 ch_1 active LFO pan modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_amp_1_ch_1_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		amp_1_ch_1_pan_mod_lfo_level = (float)lev / 100.0; 
	}
}

/**
*	@brief	Return Amp_1 ch_1 active LFO pan modulator number
*	@param	none
*	@return Amp_1 ch_1 active LFO pan modulator number
*/
int DSP_AdjSynthVoice::get_amp_1_ch_1_pan_lfo_modulation() 
{ 
	return amp_1_ch_1_pan_mod_lfo; 
}

/**
*	@brief	Return Amp_1 ch_1 active LFO pan modulator level
*	@param	none
*	@return Amp_1 ch_1 active LFO pan modulator level
*/
float DSP_AdjSynthVoice::get_amp_1_ch_1_pan_lfo_modulation_level() 
{ 
	return amp_1_ch_1_pan_mod_lfo_level; 
}

/**
*	@brief	Set Amp_1 ch_1 active LFO pan modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_amp_1_ch_1_pan_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < amp_1_ch_1_pan_mod_lfo_delay)
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
	
	amp_1_ch_1_pan_lfo_modulation_value = mod_factor * value + out_amp_1->get_ch1_pan();
	if (amp_1_ch_1_pan_lfo_modulation_value < -1.0f)
	{
		amp_1_ch_1_pan_lfo_modulation_value = 1.0f;
	}
	else if (amp_1_ch_1_pan_lfo_modulation_value > 1.0f)
	{
		amp_1_ch_1_pan_lfo_modulation_value = 1.0f;
	}
}


/**
*	@brief	Set Amp_1 ch_2 active LFO pan modulator and mode (delayed or not)
*	@param	lfo Amp_1 ch_2 active LFO pan modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_amp_1_ch_2_pan_mod_lfo(int lfo) {
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		amp_1_ch_2_pan_lfo_modulation_value = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		amp_1_ch_2_pan_mod_lfo_delay = lfo_delays[lfo];
	} 
}

/**
*	@brief	Set Amp_1 ch_2  active LFO pan modulator level
*	@param	lev Amp_1 ch_2  active LFO pan modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_amp_1_ch_2_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		amp_1_ch_2_pan_mod_lfo_level = (float)lev / 100.0; 
	}
}

/**
*	@brief	Return Amp_1 ch_2  active LFO pan modulator number
*	@param	none
*	@return Amp_1 ch_2  active LFO pan modulator number
*/
int DSP_AdjSynthVoice::get_amp_1_ch_2_pan_lfo_modulation() 
{ 
	return amp_1_ch_2_pan_mod_lfo; 
}

/**
*	@brief	Return Amp_1 ch_2  active LFO pan modulator level
*	@param	none
*	@return Amp_1 ch_2  active LFO pan modulator level
*/
float DSP_AdjSynthVoice::get_amp_1_ch_2_pan_lfo_modulation_level() 
{ 
	return amp_1_ch_2_pan_mod_lfo_level; 
}

/**
*	@brief	Set Amp_1 ch_2  active LFO pan modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_amp_1_ch_2_pan_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < amp_1_ch_2_pan_mod_lfo_delay)
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
	
	amp_1_ch_2_pan_lfo_modulation_value = mod_factor * value + out_amp_1->get_ch2_pan();
	if (amp_1_ch_2_pan_lfo_modulation_value < -1.0f)
	{
		amp_1_ch_2_pan_lfo_modulation_value = 1.0f;
	}
	else if (amp_1_ch_2_pan_lfo_modulation_value > 1.0f)
	{
		amp_1_ch_2_pan_lfo_modulation_value = 1.0f;
	}
}


