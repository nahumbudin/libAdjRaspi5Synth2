/**
*	@file		dspReverbTuning.h
*	@author		Nahum Budin
*	@date		2-Oct-2023
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	25_Jan-2021	1.1 Code refactoring and notaion.
*				2-Nov-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018
*
*	@brief		Reverb coefficients.
*				Written by Jezar at Dreampoint, June 2000
*/

#pragma once

#define undenormalise(sample) if((sample > -0.0000001f) && (sample < 0.0000001f)) sample=0.0f

const int	num_combs		= 8;
const int	num_allpasses	= 4;
const float	muted			= 0;
const float	fixed_gain		= 0.015f;
const float scale_wet		= 3;
const float scale_dry		= 2;
const float scale_damp		= 0.4f;
const float scale_room		= 0.28f;
const float offset_room		= 0.7f;
const float initial_room	= 0.99f; //0.5f;
const float initial_damp	= 0.5f;
const float initial_wet		= 1 / scale_wet;
const float initial_dry		= 0;
const float initial_width	= 1;
const float initial_mode		= 0;
const float freeze_mode		= 0.5f;
const int	stereo_spread	= 23;

// These values assume 44.1KHz sample rate
// they will probably be OK for 48KHz sample rate
// but would need scaling for 96KHz (or other) sample rates.
// The values were obtained by listening tests.
const int comb_tuning_L1		= 1116;
const int comb_tuning_R1		= 1116 + stereo_spread;
const int comb_tuning_L2		= 1188;
const int comb_tuning_R2		= 1188 + stereo_spread;
const int comb_tuning_L3		= 1277;
const int comb_tuning_R3		= 1277 + stereo_spread;
const int comb_tuning_L4		= 1356;
const int comb_tuning_R4		= 1356 + stereo_spread;
const int comb_tuning_L5		= 1422;
const int comb_tuning_R5		= 1422 + stereo_spread;
const int comb_tuning_L6		= 1491;
const int comb_tuning_R6		= 1491 + stereo_spread;
const int comb_tuning_L7		= 1557;
const int comb_tuning_R7		= 1557 + stereo_spread;
const int comb_tuning_L8		= 1617;
const int comb_tuning_R8		= 1617 + stereo_spread;
const int allpass_tuning_L1	= 556;
const int allpass_tuning_R1	= 556 + stereo_spread;
const int allpass_tuning_L2	= 441;
const int allpass_tuning_R2	= 441 + stereo_spread;
const int allpass_tuning_L3	= 341;
const int allpass_tuning_R3	= 341 + stereo_spread;
const int allpass_tuning_L4	= 225;
const int allpass_tuning_R4	= 225 + stereo_spread;
