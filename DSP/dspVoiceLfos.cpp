/**
*	@file		dspVoiceLfos.cpp
*	@author		Nahum Budin
*	@date		17-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	
*				versio	1.1	28_Jan-2021	Code refactoring and notaion
*				version	1.0	9-Jun-2018	(modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - LFOs Handling.
*				
*/

#include "dspVoice.h"
#include "../commonDefs.h"

/**
*	@brief	Set LFO_1 frequency
*	@param	freq	LFO_1 frequncy 0-100 
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo_1_frequency(float freq)
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
*	@brief	Set LFO_2 frequency
*	@param	freq	LFO_2 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo_2_frequency(float freq)
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
*	@brief	Set LFO_3 frequency
*	@param	freq	LFO_3 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo_3_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));
	
	// Set lfo frequencies due to subsampling rate
	lfo_3_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_3_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_3_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*	@brief	Set LFO_4 frequency
*	@param	freq	LFO_4 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo_4_frequency(float freq)
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
*	@brief	Set LFO_5 frequency
*	@param	freq	LFO_5 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo_5_frequency(float freq)
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
*	@brief	Set LFO_6 frequency
*	@param	freq	LFO_6 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo_6_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_6_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_6_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_6_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

