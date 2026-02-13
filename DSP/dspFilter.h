/**
*	@file		dspFilter.h
*	@author		Nahum Budin
*	@date		14-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@History	23_Jan-2021	1.1 
*								1. Code refactoring and notaion.
*								2. Adding sample-rate settings
*				31-Oct-201991.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		State variable filter.
*/

#pragma once

#include <math.h>

class DSP_Filter
{
public:
	DSP_Filter(int iD, int samp_rate);
	
	void clear();
	void set_frequency(float fr);
	void set_frequency_relative(int fr);
	void set_octave(float oct);
	void set_octave_relative(int oct);
	void set_resonance(float res);
	void set_resonance_relative(int res);
	void set_band(int bnd);
	void set_kbd_freq(float frq);
	void set_kbd_freq_relative(int frq);
	void set_kbd_track(float kbdt);
	void set_kbd_track_relative(int kbdt);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	float get_filter_max_center_frequency();
	
	float filter_output(float input, float fmod = 1.0f);
		
private:
	int filter_band;
	int filters_balance;
	
	int id;
	float setting_fcenter;
	float setting_fmult;
	float setting_octave_mult;
	float setting_damp;
	float state_input_prev;
	float state_lowpass;
	float state_highpass;
	float state_bandpass;
	float setting_kbd_fcenter;
	float setting_kbd_fmult;
	float kbd_track;
	float lowpass_tmp, bandpass_tmp, highpass_tmp;
	
	int sample_rate;
	
	static float max_setting_fcenter;
	static float max_setting_fmult;
};

