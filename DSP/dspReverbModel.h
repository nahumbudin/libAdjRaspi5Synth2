/**
*	@file		dspReverbModel.h
*	@author		Nahum Budin
*	@date		2-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	25_Jan-2021	1.1	Code refactoring and notaion. 
*				2-Nov-2019	1.0	revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018.
*
*	@brief		Reverb model.
*				Written by Jezar at Dreampoint, June 2000
*/

#pragma once

#include "dspReverbComb.h"
#include "dspReverbAllpass.h"
#include "dspReverbTuning.h"

class DSP_RevModel
{
public:
	DSP_RevModel();
	void	mute();
	void	process_mix(float *input_L, float *input_R, float *output_L, float *output_R, long num_samples, int skip);
	void	process_replace(float *input_L, float *input_R, float *output_L, float *output_R, long num_samples, int skip);
	void	set_room_size(float value);
	void	set_room_size(int value);
	float	get_room_size();
	void	set_damp(float value);
	void	set_damp(int value);
	float	get_damp();
	void	set_wet(float value);
	void	set_wet(int value);
	float	get_wet();
	void	set_dry(float value);
	void	set_dry(int value);
	float	get_dry();
	void	set_width(float value);
	void	set_width(int value);
	float	get_width();
	void	set_mode(float value);
	void	set_mode(int value);
	float	get_mode();

private:
	void	update();

	float	gain;
	float	room_size, room_size_1;
	float	damp, damp_1;
	float	wet, wet_1, wet_2;
	float	dry;
	float	width;
	float	mode;

	// The following are all declared inline 
	// to remove the need for dynamic allocation
	// with its subsequent error-checking messiness

	// Comb filters
	DSP_ReverbComb	comb_L[num_combs];
	DSP_ReverbComb	comb_R[num_combs];

	// Allpass filters
	DSP_ReverbAllpass	allpass_L[num_allpasses];
	DSP_ReverbAllpass	allpass_R[num_allpasses];

	// Buffers for the combs
	float	buf_comb_L1[comb_tuning_L1];
	float	buf_comb_R1[comb_tuning_R1];
	float	buf_comb_L2[comb_tuning_L2];
	float	buf_comb_R2[comb_tuning_R2];
	float	buf_comb_L3[comb_tuning_L3];
	float	buf_comb_R3[comb_tuning_R3];
	float	buf_comb_L4[comb_tuning_L4];
	float	buf_comb_R4[comb_tuning_R4];
	float	buf_comb_L5[comb_tuning_L5];
	float	buf_comb_R5[comb_tuning_R5];
	float	buf_comb_L6[comb_tuning_L6];
	float	buf_comb_R6[comb_tuning_R6];
	float	buf_comb_L7[comb_tuning_L7];
	float	buf_comb_R7[comb_tuning_R7];
	float	buf_comb_L8[comb_tuning_L8];
	float	buf_comb_R8[comb_tuning_R8];

	// Buffers for the allpasses
	float	buf_allpass_L1[allpass_tuning_L1];
	float	buf_allpass_R1[allpass_tuning_R1];
	float	buf_allpass_L2[allpass_tuning_L2];
	float	buf_allpass_R2[allpass_tuning_R2];
	float	buf_allpass_L3[allpass_tuning_L3];
	float	buf_allpass_R3[allpass_tuning_R3];
	float	buf_allpass_L4[allpass_tuning_L4];
	float	buf_allpass_R4[allpass_tuning_R4];
};

