/**
*	@file		dspVoiceDistortion.cpp
*	@author		Nahum Budin
*	@date		17-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	
*				version	1.1	28_Jan-2021	Code refactoring and notaion
*				version	1.0	9-Jun-2018	(modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Distortion Effect Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Distortion_1 Drive level
*	@param drv drive level _DISTORTION_MIN_DRIVE to _DISTORTION_MAX_DRIVE
*	@return void
*/
void DSP_Voice::set_distortion_1_drive(float drv) 
{ 
	distortion_1->set_drive(drv); 
}

/**
*	@brief	Set Distortion_1 Range level
*	@param rng range level _DISTORTION_MIN_RANGE to _DISTORTION_MAX_RANGE
*	@return void
*/
void DSP_Voice::set_distortion_1_range(float rng) 
{ 
	distortion_1->set_range(rng); 
}

/**
*	@brief	Set Distortion_1 Blend level
*	@param drv bln level _DISTORTION_MIN_BLEND to _DISTORTION_MAX_BLEND
*	@return void
*/
void DSP_Voice::set_distortion_1_blend(float bln) 
{ 
	distortion_1->set_blend(bln); 
}

/**
*	@brief	Enable Distortion_1 auto gain.
*	@param none
*	@return void
*/
void DSP_Voice::enable_distortion_1_auto_gain() 
{ 
	distortion_1->auto_gain = true; 
}

/**
*	@brief	Disable Distortion_1 auto gain.
*	@param none
*	@return void
*/
void DSP_Voice::disable_distortion_1_auto_gain() 
{ 
	distortion_1->auto_gain = false; 
}

/**
*	@brief	Set Distortion_2 Drive level
*	@param drv drive level _DISTORTION_MIN_DRIVE to _DISTORTION_MAX_DRIVE
*	@return void
*/
void DSP_Voice::set_distortion_2_drive(float drv) 
{ 
	distortion_2->set_drive(drv); 
}

/**
*	@brief	Set Distortion_2 Range level
*	@param rng range level _DISTORTION_MIN_RANGE to _DISTORTION_MAX_RANGE
*	@return void
*/
void DSP_Voice::set_distortion_2_range(float rng) 
{ 
	distortion_2->set_range(rng); 
}

/**
*	@brief	Set Distortion_2 Blend level
*	@param drv bln level _DISTORTION_MIN_BLEND to _DISTORTION_MAX_BLEND
*	@return void
*/
void DSP_Voice::set_distortion_2_blend(float bln) 
{ 
	distortion_2->set_blend(bln); 
}

/**
*	@brief	Enable Distortion2 auto gain. 
*	@param none
*	@return void
*/
void DSP_Voice::enable_distortion_2_auto_gain() 
{ 
	distortion_2->auto_gain = true; 
}

/**
*	@brief	Disable Distortion_2 auto gain.
*	@param none
*	@return void
*/
void DSP_Voice::disable_distortion_2_auto_gain() 
{ 
	distortion_2->auto_gain = false; 
}

