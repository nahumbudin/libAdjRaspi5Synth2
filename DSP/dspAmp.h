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

#pragma once

class DSP_Amp
{
public:
	DSP_Amp();
	
	void set_ch1_gain(float gn);
	void set_ch1_gain(int gn);
	void set_ch1_pan(float pn);
	void set_ch1_pan(int pn);
	void set_ch2_gain(float gn);
	void set_ch2_gain(int gn);
	void set_ch2_pan(float pn);
	void set_ch2_pan(int pn);
	
	float get_ch1_gain();
	float get_ch1_pan();
	float get_ch2_gain();
	float get_ch2_pan();
	
	float get_left_output(float in1, float in2, float ch1_lfo_pan_mod = 0.0f, float ch2_lfo_pan_mod = 0.0f);
	float get_right_output(float in1, float in2, float ch1_lfo_pan_mod = 0.0f, float ch2_lfo_pan_mod = 0.0f);

	void enable_fixed_levels();
	void disable_fixed_levels();
	bool levels_are_fixed();
	
private:
	float ch1_gain, ch1_pan, ch2_gain, ch2_pan;

	/* When false, patch values may change gain level; when true - levels will not change. */
	bool fixed_levels;
	
};


