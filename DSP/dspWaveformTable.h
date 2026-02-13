/**
*	@file		dspWaveformTable.h
*	@author		Nahum Budin
*	@date		21_sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	23_Jan-2021 1.1 Code refactoring and notaion. 
*				31-Oct-2019	1.0 (dspWaveformLUT) (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		A Table used for generating sampled floating point waveforsm.
*	
*/

#pragma once

#include <math.h>

#define _WTAB_MAX_LEN		262144 // 2^18
#define _WTAB_MIN_LEN		512

class DSP_WaveformTab
{
public:
	DSP_WaveformTab(int len = 512);
	~DSP_WaveformTab();

	int set_wtab_length(int len);
	int get_wtab_length();

	int set_wtab_val(int index, float val);
	float get_wtab_val(int index);
	float *get_wtab_ptr();

private:
	int init_waveform_table();
	
	// Waveform Table 
	float *waveform_tab;
	// WTAB length
	int wtab_length;
	// WTAB fundemental frequency 
	float wtab_fundemental_frequency;
};

