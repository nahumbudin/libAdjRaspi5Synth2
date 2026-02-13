/**
*	@file		dspNoise.h
*	@author		Nahum Budin
*	@date		21-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	24_Jan-2021 1.1 Code refactoring and notaion.
*				02-Nov-2019 1.0 (dspWaveformLUT) (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Noise generator.
*
*	// https://noisehack.com/generate-noise-web-audio-api/	
*/

#pragma once

#include <stdint.h>

class DSP_Noise
{
public:
	DSP_Noise(int voice = 0);
	
	float set_amplitude(float amp);
	float get_amplitude();
	int set_noise_type(int nt);
	int get_noise_type();
	
	float get_next_white_noise_val();
	float get_next_pink_noise_val();
	float get_next_brown_noise_val();
	float get_next_noise_val();
	
private:
	int voice_id;
	float level;
	uint32_t seed; // must start at 1
	static uint16_t instance_count;
	int noise_type;
};

