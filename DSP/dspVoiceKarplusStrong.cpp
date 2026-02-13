/**
*	@file		dspVoiceKarplusStrong.cpp
*	@author		Nahum Budin
*	@date		17-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	
*				version	1.1	
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate and bloc-size settings
*				version	1.0	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Karplus Strong Generator Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Kps_1 send level to channel 1
*	@param lev Kps1 send level to channel 1 0.0 to 1.0
*	@return void
*/
void DSP_Voice::set_karplus_1_send_filter_1_level(float lev)
{
	karplus_1_send_filter_1_level = lev;
	if (karplus_1_send_filter_1_level < 0.0f)
	{
		karplus_1_send_filter_1_level = 0.0f;
	}
	else if (karplus_1_send_filter_1_level > 1.0f)
	{
		karplus_1_send_filter_1_level = 1.0f;
	}
}

/**
*	@brief	Set Kps_1 send level to channel 1 (int)
*	@param lev Kps_1 send level to channel 1 0-100
*	@return void
*/
void DSP_Voice::set_karplus_1_send_filter_1_level(int lev)
{
	set_karplus_1_send_filter_1_level((float)lev / 100.0f);
}

/**
*	@brief	return Kps_1 send level to channel 1
*	@param none
*	@return Kps_1 send level to channel 1
*/
float DSP_Voice::get_karplus_1_send_filter_1_level() { return karplus_1_send_filter_1_level; }

/**
*	@brief	Set Kps_1 send level to channel 2
*	@param lev Kps_1 send level to channel 2 0.0 to 1.0
*	@return void
*/
void DSP_Voice::set_karplus_1_send_filter_2_level(float lev)
{
	karplus_1_send_filter_2_level = lev;
	if (karplus_1_send_filter_2_level < 0.0f)
	{
		karplus_1_send_filter_2_level = 0.0f;
	}
	else if (karplus_1_send_filter_2_level > 1.0f)
	{
		karplus_1_send_filter_2_level = 1.0f;
	}
}

/**
*	@brief	Set Kps_1 send level to channel 2 (int)
*	@param lev Kps_1 send level to channel 2 (int) 0 to 100
*	@return void
*/
void DSP_Voice::set_karplus_1_send_filter_2_level(int lev)
{
	set_karplus_1_send_filter_2_level((float)lev / 100.0f);
}

/**
*	@brief	return Kps_1 send level to channel 2
*	@param none
*	@return Kps_1 send level to channel 2
*/
float DSP_Voice::get_karplus_1_send_filter_2_level() { return karplus_1_send_filter_2_level; }
	
	
	