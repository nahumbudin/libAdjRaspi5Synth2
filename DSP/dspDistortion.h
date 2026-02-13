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

#pragma once

class DSP_Distortion
{
public:
	
	DSP_Distortion();
	
	void set_drive(float drv);
	void set_range(float rng);
	void set_blend(float bln);
	float get_next_output_val(float in);
	void enable_auto_gain() { auto_gain = true; }	
	void disable_auto_gain() { auto_gain = false; }

	float drive;
	float range;
	float blend;

	bool auto_gain;
};


