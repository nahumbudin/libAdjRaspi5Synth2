/**
*	@file		dspReverbAllpass.h
*	@author		Nahum Budin
*	@date		2-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	25-Jan-2021	1.1	Code refactoring and notaion. 
*				2-Nov-2019	1.0	revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018
*
*	@brief		Reverb all pass filter.
*				Written by Jezar at Dreampoint, June 2000
*/


#pragma once

#include "dspReverbTuning.h"

class DSP_ReverbAllpass
{
public:
	DSP_ReverbAllpass();
	void	set_buffer(float *buf, int size);
	inline  float	process(float inp);
	void	mute();
	void	set_feedback(float val);
	float	get_feedback();
	// private:
	float	feedback;
	float	*buffer;
	int		bufsize;
	int		bufidx;
};


// Big to inline - but crucial for speed

inline float DSP_ReverbAllpass::process(float input)
{
	float output;
	float bufout;
	
	bufout = buffer[bufidx];
	undenormalise(bufout);
	
	output = -input + bufout;
	buffer[bufidx] = input + (bufout*feedback);

	if (++bufidx >= bufsize) bufidx = 0;

	return output;
}


