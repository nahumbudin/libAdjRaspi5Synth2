/**
*	@file		dspAmp.h
*	@author		Nahum Budin
*	@date		14-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*	
*	@History	23_Jan-2021 1.1 
*					1. Code refactoring and notaion.
*				30-Oct-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Stereo Amplifier ch1 and ch2 with pan level setting and pan LFO modulation input.
*/

#include <stdio.h>

#include "dspAmp.h"


DSP_Amp::DSP_Amp()
{
	ch1_gain = 1.0f;
	ch1_pan = 0.0f;
	ch2_gain = 1.0f;
	ch2_pan = 0.0f;

	fixed_levels = true;
}

/**
*	@brief	Set channel 1 gain 0-1.0
*	@param gn 0.0-1.0
*	@return void
*/
void DSP_Amp::set_ch1_gain(float gn)
{
	float gain = gn;
	
	if (gain < 0.0f) 
	{
		gain = 0.0f;
	}
	else if (gain > 1.0f) 
	{
		gain = 1.0f;
	}
		
	ch1_gain = gain;
}

/**
*	@brief	Set channel 1 gain 0-100
*	@param gn 0.0-100
*	@return void
*/
void DSP_Amp::set_ch1_gain(int gn)
{
	set_ch1_gain((float)gn / 100.0f);
}

/**
*	@brief	Set channel 1 pan -1.0 (left) - +1.0 (right)
*	@param pan 0.0-1.0
*	@return void
*/
void DSP_Amp::set_ch1_pan(float pn)
{
	float pan = pn;
	
	if (pan < -1.0f) 
	{
		pan = -1.0f;
	}
	else if (pan > 1.0f) 
	{
		pan = 1.0f;
	}
		
	ch1_pan = pan;
}

/**
*	@brief	Set channel 1 pan 0-100 (Left: 0-49 mid:50 Right 51-100)
*	@param gn 0-100
*	@return void
*/
void DSP_Amp::set_ch1_pan(int pn)
{
	set_ch1_pan((float)(pn - 50) / 50.0f);
}

/**
*	@brief	Set channel 2 gain 0-1.0
*	@param gn 0.0-1.0
*	@return void
*/
void DSP_Amp::set_ch2_gain(float gn)
{
	float gain = gn;
		
	if (gain < 0.0f) 
	{
		gain = 0.0f;
	}
	else if (gain > 1.0f) 
	{
		gain = 1.0f;
	}
			
	ch2_gain = gain;
}

/**
*	@brief	Set channel 2 gain 0-100
*	@param gn 0.0-100
*	@return void
*/
void DSP_Amp::set_ch2_gain(int gn)
{
	set_ch2_gain((float)gn / 100.0f);
}

/**
*	@brief	Set channel 2 pan -1.0 (Left) - +1.0 )Right)
*	@param pn 0.0-1.0
*	@return void
*/
void DSP_Amp::set_ch2_pan(float pn)
{
	float pan = pn;
	
	if (pan < -1.0f)
	{
		pan = -1.0f;
	}
	else if (pan > 1.0f)
	{
		pan = 1.0f;
	}
		
	ch2_pan = pan;
}

/**
*	@brief	Set channel 1 pan 0-100 (Left: 0-49 mid:50 Right 51-100)
*	@param pn 000-100
*	@return void
*/
void DSP_Amp::set_ch2_pan(int pn)
{
	set_ch2_pan((float)(pn - 50) / 50.0f);
}

/**
*	@brief	Get channel 1 gain
*	@param none
*	@return channel 1 gain 0.0-1.0
*/
float DSP_Amp::get_ch1_gain() 
{ 
	return ch1_gain; 
}

/**
*	@brief	Get channel 1 pan
*	@param none
*	@return channel 1 pan -1.0 - +1.0
*/
float DSP_Amp::get_ch1_pan() 
{ 
	return ch1_pan; 
}

/**
*	@brief	Get channe2 1 gain
*	@param none
*	@return channel 1 gain 0.0-1.0
*/
float DSP_Amp::get_ch2_gain() 
{ 
	return ch2_gain; 
}

/**
*	@brief	Get channel 2 pan
*	@param none
*	@return channel 1 pan -1.0 - +1.0
*/
float DSP_Amp::get_ch2_pan() 
{ 
	return ch2_pan; 
}

/**
*	@brief	Get Left output value
*	@param in1 channel 1 input
*	@param in2 channel 2 input
*	@param ch1lfoPanMod channel 1 lfo pan modulation
*	@param ch2lfoPanMod channel 2 lfo pan modulation
*	@return left output
*/
float DSP_Amp::get_left_output(float in1, float in2, float ch1_lfo_pan_mod, float ch2_lfo_pan_mod)
{
	return (in1 * ch1_gain * (1 - ch1_lfo_pan_mod) + in2 * ch2_gain * (1 - ch2_lfo_pan_mod)) * 0.2f ;
}

/**
*	@brief	Get right output value
*	@param in1 channel 1 input
*	@param in2 channel 2 input
*	@param ch1lfoPanMod channel 1 lfo pan modulation
*	@param ch2lfoPanMod channel 2 lfo pan modulation
*	@return right output
*/
float DSP_Amp::get_right_output(float in1, float in2, float ch1_lfo_pan_mod, float ch2_lfo_pan_mod)
{	
	return (in1 * ch1_gain * (1 + ch1_lfo_pan_mod) + in2 * ch2_gain * (1 + ch2_lfo_pan_mod)) * 0.2f ;
}

/**
*	@brief	Enable fixed level
*	@param none
*	@return none
*/
void DSP_Amp::enable_fixed_levels() 
{ 
	fixed_levels = true; 
}

/**
*	@brief	Disable fixed level
*	@param none
*	@return none
*/
void DSP_Amp::disable_fixed_levels() 
{ 
	fixed_levels = false; 
}

/**
*	@brief	Get fixed level state value
*	@param none
*	@return fixed level state value
*/
bool DSP_Amp::levels_are_fixed() 
{ 
	return fixed_levels; 
}
