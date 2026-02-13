/**
*	@file		dspSquareWaveGenerator.cpp
*	@author		Nahum Budin
*	@date		13-Sep2024
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	24_Jan-2021	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate settings
*				31-Oct-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Squar Wave Oscilator.
*/

#include "dspSquareWaveGenerator.h"
#include "../libAdjRaspi5Synth2.h"
#include "../utils/utils.h"

/**
 *	@brief	Creates and returns a Square/Pulse Wave Oscilator object instance
 *	@param sample rate in Hz _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
 *	@param asym	Square/PWM asymetry 0.05 - 0.95 (use PWM dutycycle)
 *	@return		a Square Wave Generator instance
*/
DSP_SquareWaveGenerator::DSP_SquareWaveGenerator(int samp_rate, float asym)
{
	set_sample_rate(samp_rate);
	set_pwm(asym);
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_SquareWaveGenerator::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_SquareWaveGenerator::get_sample_rate() { return sample_rate; }

/**
 *	@brief	Set the Asymetry/PWM of the  Square/Pulse Wave Oscilator
 *	@param asym	Square/PWM asymetry 0.05 - 0.95 (use PWM d cycle)
 *	@return	set pwm value
*/
float DSP_SquareWaveGenerator::set_pwm(float asym)
{
	pt1 = asym;
	
	if (pt1 > 0.95)
	{
		pt1 = 0.95;
	}
	else if (pt1 < 0.05)
	{
		pt1 = 0.05;
	}
	
	cycle_restarted = false;
	pt1 = asym;	
	pos = 0;
	
	return pt1;
}

/**
 *	@brief	Return the Asymetry/PWM of the  Square/Pulse Wave Oscilator
 *	@param	none
 *	@return	asym	Square/PWM asymetry 0.05 - 0.95 (use PWM d cycle)
*/
float DSP_SquareWaveGenerator::get_pwm() { return asym; }

/** 
*	@brief	Get next PWM osc output value based on pos/index
*	@param	none 
*	@return	output value -0.7 to +0.7
*/
float DSP_SquareWaveGenerator::get_square_val()
{
	// Avoid zero slope clicks
	float edge = 0.01f;
	float dy = 1.4f / 0.01f;
    	
	if (pos < edge)
	{
		// Rising edge slope 
		return -0.7f + pos * dy;
	}
	else if (pos < pt1)
	{
		// High level 
		return 0.7f;
	}
	else if (pos < pt1 + edge)
	{
		// Faling edge slope
		return 0.7f - (pos - pt1)*dy;
	}
	else
	{
		// Low level 
		return -0.7f;
	}
}

/**
*	@brief	Sync PWM osc by zeroing the phase
*	@param	none
*	@return	none
*/
void DSP_SquareWaveGenerator::sync()
{
	pos = 0;
}

/**
*	@brief	Return the PWM osc zero crossing sync state event
*	@param	none
*	@return	true when cycle restarted.
*/
bool DSP_SquareWaveGenerator::get_cycle_restarted_sync_state() { return cycle_restarted; }


/** 
 *	@brief	Calculate next PWM Oscillator output value and updates index 
 *	@param freq	Oscillator frequency [Hz]
 *	@return		output val 0.7 or -0.7
 */
float DSP_SquareWaveGenerator::get_next_square_gen_out_val(float freq)
{
	float freq_step = freq / (float)sample_rate;
	float res = 0;
	// clear after 1 cycle
	cycle_restarted = false;
	// Clip pos to 0 upto +1 
	if (pos > 1) 
	{
		pos -= 1; 
		res = get_square_val();
		cycle_restarted = true;
	}
	else 
	{
		if (pos < 0)
		{
			pos += 1;
		}
		res = get_square_val();
	}
	pos += freq_step;
	
	return res;
}
