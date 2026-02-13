/**
*	@file		dspReverbComb.h
*	@author		Nahum Budin
*	@date		2-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	25-Jan-2021	1.1	Code refactoring and notaion.
*				2-Nov-2019	1.0	revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Reverb comb filter.
*				Written by Jezar at Dreampoint, June 2000
*/

#pragma once

#include "dspReverbTuning.h"
	
class DSP_ReverbComb
{
public:
	DSP_ReverbComb();
	void	set_buffer(float *buf, int size);
	inline  float	process(float inp);
	void	mute();
	void	set_damp(float val);
	float	get_damp();
	void	set_feedback(float val);
	float	get_feedback();

private:
	float	feedback;
	float	filter_store;
	float	damp1;
	float	damp2;
	float	*buffer;
	int		buf_size;
	int		buf_idx;
}
;


// Big to inline - but crucial for speed

inline float DSP_ReverbComb::process(float input)
{
	float output;

	output = buffer[buf_idx];
	undenormalise(output);

	filter_store = (output * damp2) + (filter_store * damp1);
	undenormalise(filter_store);

	buffer[buf_idx] = input + (filter_store * feedback);

	if (++buf_idx >= buf_size) buf_idx = 0;

	return output;
}
