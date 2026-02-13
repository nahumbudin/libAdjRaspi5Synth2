/**
*	@file		dspTriangleWaveGenerator.h
*	@author		Nahum Budin
*	@date		13-Sep2024
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	24_Jan-2021	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate settings
*				31-Oct-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Triangle Wave Oscilator.
*/

#pragma once

class DSP_TriangleWaveGenerator
{
public:	
	DSP_TriangleWaveGenerator(int samp_rate, float asym);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	float set_asymetry(float asy);
	float get_asymetry();
	float get_triangle_val();
	void sync();
	bool get_cycle_restarted_sync_state();
	float get_next_triangle_gen_out_val(float freq);
	
private:
	// Triangle heads
	float pt1, pt2;
	// Trinagle slopes
	float dy1, dy2;
	// Triangle wave cycle step / index 0 to 1 
	float pos;
	// Indicates a new cycle has just restarted 
	bool cycle_restarted;
	//asymetric value 0.5-0.95
	float asym;
	
	int sample_rate;
};


