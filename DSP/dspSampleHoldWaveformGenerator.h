/**
*	@file		dspSampleHoldWaveformGenerator.h
*	@author		Nahum Budin
*	@date		13-Sep2024
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	25_Jan-2021	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate settings
*				31-Oct-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Generates random samples -0.8 to +0.8.
*/

#pragma once
class DSP_SampleHoldWaveGenerator
{
public:	
	DSP_SampleHoldWaveGenerator(int samp_rate);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	void sync();
	bool get_cycle_restarted_sync_state();
	
	float get_next_sample_hold_gen_out_val(float freq);	
	
private:
	
	int init();
	
	
	float pos;
	float sample;
	bool cycle_restarted;
	
	int sample_rate;
};


