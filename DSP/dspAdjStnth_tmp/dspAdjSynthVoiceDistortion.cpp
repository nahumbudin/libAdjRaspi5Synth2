/**
*	@file		dspAdjSynthVoiceDistortion.cpp
*	@author		Nahum Budin
*	@date		27-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	28-Jan-2021	1.1	Code refactoring and notaion.
*				9-Jun-2018	1.0 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Distortion Effect Handling.
*				
*/

#include "dspAdjSynthVoice.h"

/**
*	@brief	Set Distortion_1 Drive level
*	@param drv drive level _DISTORTION_MIN_DRIVE to _DISTORTION_MAX_DRIVE
*	@return void
*/
void DSP_AdjSynthVoice::set_distortion_1_drive(float drv) 
{ 
	distortion_1->set_drive(drv); 
}

/**
*	@brief	Set Distortion_1 Range level
*	@param rng range level _DISTORTION_MIN_RANGE to _DISTORTION_MAX_RANGE
*	@return void
*/
void DSP_AdjSynthVoice::set_distortion_1_range(float rng) 
{ 
	distortion_1->set_range(rng); 
}

/**
*	@brief	Set Distortion_1 Blend level
*	@param drv bln level _DISTORTION_MIN_BLEND to _DISTORTION_MAX_BLEND
*	@return void
*/
void DSP_AdjSynthVoice::set_distortion_1_blend(float bln) 
{ 
	distortion_1->set_blend(bln); 
}

/**
*	@brief	Enable Distortion_1 auto gain.
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_distortion_1_auto_gain() 
{ 
	distortion_1->auto_gain = true; 
}

/**
*	@brief	Disable Distortion_1 auto gain.
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_distortion_1_auto_gain() 
{ 
	distortion_1->auto_gain = false; 
}

/**
*	@brief	Set Distortion_2 Drive level
*	@param drv drive level _DISTORTION_MIN_DRIVE to _DISTORTION_MAX_DRIVE
*	@return void
*/
void DSP_AdjSynthVoice::set_distortion_2_drive(float drv) 
{ 
	distortion_2->set_drive(drv); 
}

/**
*	@brief	Set Distortion_2 Range level
*	@param rng range level _DISTORTION_MIN_RANGE to _DISTORTION_MAX_RANGE
*	@return void
*/
void DSP_AdjSynthVoice::set_distortion_2_range(float rng) 
{ 
	distortion_2->set_range(rng); 
}

/**
*	@brief	Set Distortion_2 Blend level
*	@param drv bln level _DISTORTION_MIN_BLEND to _DISTORTION_MAX_BLEND
*	@return void
*/
void DSP_AdjSynthVoice::set_distortion_2_blend(float bln) 
{ 
	distortion_2->set_blend(bln); 
}

/**
*	@brief	Enable Distortion_2 auto gain. 
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_distortion_2_auto_gain() 
{ 
	distortion_2->auto_gain = true; 
}

/**
*	@brief	Disable Distortion_2 auto gain.
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_distortion_2_auto_gain() 
{ 
	distortion_2->auto_gain = false; 
}

