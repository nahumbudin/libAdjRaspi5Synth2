/**
*	@file		dspSquareWaveGenerator.h
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
*	@brief		Squar Wave Oscilator.
*/

#pragma once

class DSP_SquareWaveGenerator
{
public:	
	DSP_SquareWaveGenerator(int samp_rate, float asym);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	float set_pwm(float asym);
	float get_pwm();
	float get_square_val();
	void sync();
	bool get_cycle_restarted_sync_state();
	float get_next_square_gen_out_val(float freq);
	
private:
	// Pulse change level point
	float pt1;
	// Square wave cycle step/index 0 to 1
	float pos;
	// Indicates a new cycle has just restarted 
	bool cycle_restarted;
	// pwm/asymetric value 0.5-0.95
	float asym;
	
	int sample_rate;
};

