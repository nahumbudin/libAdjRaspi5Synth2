/**
*	@file		dspDistortion.h
*	@author		Nahum Budin
*	@date		14-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@History	23_Jan-2021	1.1 
*								1. Code refactoring and notaion.
*				2-Nov-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Distortion effect.
*				Based on: https://github.com/marcoalkema/cpp-guitar_effects
*/

#include <math.h>

#include "dspDistortion.h"

#include "../LibAPI/synthesizer.h"

DSP_Distortion::DSP_Distortion()
{
	drive = 0.0f;
	range = 0.0f;
	blend = 0.0f;	
	auto_gain = false;
}

/**
*	@brief	Sets Drive level
*	@param drv drive level _DISTORTION_MIN_DRIVE to _DISTORTION_MAX_DRIVE
*	@return void
*/
void DSP_Distortion::set_drive(float drv)
{
	drive = drv;
	
	if (drive < _DISTORTION_MIN_DRIVE)
	{
		drive = _DISTORTION_MIN_DRIVE;
	}
	else if (drive > _DISTORTION_MAX_DRIVE)
	{
		drive = _DISTORTION_MAX_DRIVE;
	}
}

/**
*	@brief	Sets Range level
*	@param rng range level _DISTORTION_MIN_RANGE to _DISTORTION_MAX_RANGE
*	@return void
*/
void DSP_Distortion::set_range(float rng)
{
	range = rng;
	
	if (range < _DISTORTION_MIN_RANGE) 
	{
		range = _DISTORTION_MIN_RANGE;
	}
	else if (range > _DISTORTION_MAX_RANGE) 
	{
		range = _DISTORTION_MAX_RANGE;
	}
}

/**
*	@brief	Sets Blend level
*	@param drv bln level _DISTORTION_MIN_BLEND to _DISTORTION_MAX_BLEND
*	@return void
*/
void DSP_Distortion::set_blend(float bln)
{
	blend = bln;
	
	if (blend < _DISTORTION_MIN_BLEND) 
	{
		blend = _DISTORTION_MIN_BLEND;
	}
	else if (blend > _DISTORTION_MAX_BLEND) 
	{
		blend = _DISTORTION_MAX_BLEND;
	}
}

/**
*	@brief	Returns calculated next output sample
*	@param	in input sample
*	@return void
*/
float DSP_Distortion::get_next_output_val(float in)
{
	
	float input, output, clean_sig = in;
	
	float amp = drive * range;

	input = in * amp;
	
	output = ((((2.f / M_PI) * atan(input)) * blend) + (clean_sig * (1.f - blend)));
	
	if (auto_gain && (amp > 4.f))
	{
		output /= 4.f;
	}

	return output;
}
