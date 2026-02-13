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

#include "dspReverbAllpass.h"

DSP_ReverbAllpass::DSP_ReverbAllpass()
{
	bufidx = 0;
}

/**
*	@brief	Set a buffer
*	@param	buf a pointer to a float buffer
*	@param	size buffer size
*	@return void
*/
void DSP_ReverbAllpass::set_buffer(float *buf, int size) 
{
	buffer = buf; 
	bufsize = size;
}

/**
*	@brief	Mute reverb - zero all buffer samples
*	@param	none
*	@return void
*/
void DSP_ReverbAllpass::mute()
{
	for (int i = 0; i < bufsize; i++)
	{
		buffer[i] = 0;
	}
}

/**
*	@brief	Set feedback level
*	@param	val feedback value
*	@return void
*/
void DSP_ReverbAllpass::set_feedback(float val) 
{
	feedback = val;
}

/**
*	@brief	Return feedback level
*	@param	none
*	@return feedback value
*/
float DSP_ReverbAllpass::get_feedback() 
{
	return feedback;
}