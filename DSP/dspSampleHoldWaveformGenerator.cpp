/**
*	@file		dspSampleHoldWaveformGenerator.cpp
*	@author		Nahum Budin
*	@date		13-Sep2024
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	25_Jan-2021	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate settings
*				31-Oct-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Generates random samples -0.8 to +0.8.
*/

#include <stdlib.h> // rand

#include "dspSampleHoldWaveformGenerator.h"
#include "../libAdjRaspi5Synth2.h"
#include "../utils/utils.h"

/**
 *	@brief	Creates a Sample and Hold generator object instance
 *	@param	sample rate in Hz _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
 *	@return	none
*/
DSP_SampleHoldWaveGenerator::DSP_SampleHoldWaveGenerator(int samp_rate)
{
	set_sample_rate(samp_rate);
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_SampleHoldWaveGenerator::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	pos = 0;
	cycle_restarted = false;
	sample = 0;
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_SampleHoldWaveGenerator::get_sample_rate() { return sample_rate; }

/**
*	@brief	Get next sample and hold generator output value based on pos/index
*	@param	freq	frequency (Hz)
*	@return	float output value -0.8 to +0.8
*/
float DSP_SampleHoldWaveGenerator::get_next_sample_hold_gen_out_val(float freq)
{
	float freq_step = freq / (float)sample_rate;
	
	cycle_restarted = false;
	
	if (pos > 1)
	{
		// pos only 0 to 1
		pos -= 1;
		if (pos < freq_step)
		{
			sample = -0.8f + 1.6f * (float)(rand() % 99999) / 100000.0f;
		}
		cycle_restarted = true;
	}
	else
	{
		if (pos < 0)
		{
			pos += 1;
		}
	}
	
	pos += freq_step;
	
	return sample;
}

/**
*	@brief	Sync S&H generator by zeroing the phase
*	@param	none
*	@return	void
*/
void DSP_SampleHoldWaveGenerator::sync()
{
	pos = 0;
}

/**
*	@brief	Return the S&H generator zero crossing sync state event
*	@param	none
*	@return	true when cycle restarted.
*/
bool DSP_SampleHoldWaveGenerator::get_cycle_restarted_sync_state() { return cycle_restarted; }
