/**
*	@file		dspTriangleWaveGenerator.cpp
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
*	@brief		Triangle Wave Oscilator.
*/

#include "dspTriangleWaveGenerator.h"
#include "../libAdjRaspi5Synth2.h"
#include "../utils/utils.h"


/**
 *	@brief	Creates a Triangle Wave Oscillator object instance
 *	@param asym		Triangle asymetry 0.05 - 0.95 (use PWM dutycycle)
 *	@return			none
*/
DSP_TriangleWaveGenerator::DSP_TriangleWaveGenerator(int samp_rate, float asym)
{
	set_sample_rate(samp_rate);
	set_asymetry(asym);
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_TriangleWaveGenerator::set_sample_rate(int samp_rate)
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
int DSP_TriangleWaveGenerator::get_sample_rate() { return sample_rate; }

/** 
 *	@brief	Set asymetry level
 *	@param asym		Triangle asymetry 0.05 - 0.95 (use PWM d cycle) 
 *	@return			set asymetric value
 */
float DSP_TriangleWaveGenerator::set_asymetry(float asy)
{
	asym = asy;
	
	if (asym > 0.95)
	{
		asym = 0.95;
	}
	else
	{
		if (asym < 0.05)
		{
			asym = 0.05;
		}
	}	
	
	pt1 = asym / 2;
	pt2 = 1 - pt1;
	dy1 = 1 / pt1;
	dy2 = 2 / (pt2 - pt1);
	pos = 0;
	cycle_restarted = false;
	
	return asym;
}

/**
 *	@brief	Return the Asymetry of the  Triangle Wave Oscilator
 *	@param	none
 *	@return	asym	Square/PWM asymetry 0.05 - 0.95 (use PWM d cycle)
*/
float DSP_TriangleWaveGenerator::get_asymetry() { return asym; }

/** 
*	@brief	Get next triangle wave value based on pos/index
*	@param	none 
*	@return	float triangle value -1 to +1
*/
float DSP_TriangleWaveGenerator::get_triangle_val()
{
	if (pos < pt1)
	{
		// 1st up slope
		return pos * dy1;
	}
	else if (pos > pt2)
	{
		// 2nd up slope
		return -1 + (pos - pt2)*dy1;
	}
	else
	{
		// Down slope
		return 1 - (pos - pt1)*dy2;
	}
}

/**
*	@brief	Sync Triangle osc by zeroing the phase
*	@param	none
*	@return	none
*/
void DSP_TriangleWaveGenerator::sync()
{
	pos = 0;
}

/**
*	@brief	Return the Triangle osc zero crossing sync state event
*	@param	none
*	@return	true when cycle restarted.
*/
bool DSP_TriangleWaveGenerator::get_cycle_restarted_sync_state() { return cycle_restarted; }


/** 
*	@Calculate next Triangle Oscillator output value and updates index
*	@param freq		Oscillator frequency [Hz]
*	@return output val -1.0 or +1.0
*/
float DSP_TriangleWaveGenerator::get_next_triangle_gen_out_val(float freq)
{
	float freq_step = freq / (float)sample_rate;		
	float res = 0;
	// clear after 1 cycle 
	cycle_restarted = false;
	// Clip pos to 0 to +1 
	if (pos > 1) 
	{
		pos -= 1; 
		res = get_triangle_val();
		cycle_restarted = true;
	}
	else 
	{
		if (pos < 0) 
		{
			pos += 1;
		}
		res = get_triangle_val();
	}
	pos += freq_step;
	
	return res;
}

