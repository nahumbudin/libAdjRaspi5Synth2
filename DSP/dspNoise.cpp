/**
*	@file		dspNoise.cpp
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

#include <stdlib.h> // srand

#include "dspNoise.h"
#include "../LibAPI/synthesizer.h"

uint16_t DSP_Noise::instance_count = 0;

/**
*	@brief	Creates a noise generator object instance
*	@param voice unique noise generator id number
*	@return void
*/
DSP_Noise::DSP_Noise(int voice)
{
	voice_id = voice;
	level = 0.0f;	
	seed = 1 + instance_count++;
	srand(seed);
	noise_type = _WHITE_NOISE;
}

/**
*	@brief	Set noise generator output volume
*	@param amp noise generator output volume 0.0-1.0
*	@return set volume
*/
float DSP_Noise::set_amplitude(float amp) {
	level = amp;
	if (level < 0.0)
	{
		level = 0.0;
	}
	else if (level > 1.0)
	{
		level = 1.0;
	}
	
	return level;
}

/**
*	@brief	Return noise generator output volume
*	@param none
*	@return noise generator output volume 0.0-1.0
*/
float DSP_Noise::get_amplitude() 
{ 
	return level; 
}

/**
*	@brief	Set noise generator noise type
*	@param nt noise type _WHITE_NOISE, _PINK_NOISE, _BROWN_NOISE
*	@return noise type if OK; -1 if param out of range
*/
int DSP_Noise::set_noise_type(int nt)
{
	if ((nt == _WHITE_NOISE) || (nt == _PINK_NOISE) || (nt == _BROWN_NOISE))
	{
		noise_type = nt;
		return noise_type;
	}
	else
	{
		return -1;
	}
}

/**
*	@brief	Return noise generator noise type
*	@param none
*	@return nt noise type _WHITE_NOISE, _PINK_NOISE, _BROWN_NOISE
*/
int DSP_Noise::get_noise_type() 
{ 
	return noise_type; 
}

/**
*	@brief	Return next white noise generator output sample
*	@param none
*	@return next white noise generator output sample
*/
float DSP_Noise::get_next_white_noise_val()
{
	return (float)rand() / (float)RAND_MAX * 2.0 - 1.0;
}

/**
*	@brief	Return next pink noise generator output sample
*	@param none
*	@return next pink noise generator output sample
*/
float DSP_Noise::get_next_pink_noise_val()
{
	float out;
	
	static float b0 = 0;
	static float b1 = 0;
	static float b2 = 0;
	static float b3 = 0;
	static float b4 = 0;
	static float b5 = 0;
	static float b6 = 0;
	
	float white = get_next_white_noise_val();
	
	b0 = 0.99886 * b0 + white * 0.0555179;
	b1 = 0.99332 * b1 + white * 0.0750759;
	b2 = 0.96900 * b2 + white * 0.1538520;
	b3 = 0.86650 * b3 + white * 0.3104856;
	b4 = 0.55000 * b4 + white * 0.5329522;
	b5 = -0.7616 * b5 - white * 0.0168980;
	
	out =  b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362;
	out *= 0.11; // (roughly) compensate for gain
	
	b6 = white * 0.115926;
	
	return out;
}

/**
*	@brief	Return next brown noise generator output sample
*	@param none
*	@return next brown noise generator output sample
*/
float DSP_Noise::get_next_brown_noise_val()
{
	float out;
	static float lastOut = 0.0f;	
	float white = get_next_white_noise_val();
	
	out = (lastOut + (0.02 * white)) / 1.02;
	lastOut = out;
	out *= 3.5; // (roughly) compensate for gain
	
	return out;
}

/**
*	@brief	Return next noise generator output sample
*	@param none
*	@return next white generator output sample
*/
float DSP_Noise::get_next_noise_val()
{	
	float val = 0.0f;
	
	switch (noise_type)
	{		
		case _WHITE_NOISE:
			val =  get_next_white_noise_val();
			break;
					
		case _PINK_NOISE:
			val = get_next_pink_noise_val();
			break;
					
		case _BROWN_NOISE:
			val = get_next_brown_noise_val();
			break;
	}
	
	return val * level;
}

