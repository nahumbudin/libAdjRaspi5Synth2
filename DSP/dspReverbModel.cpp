/**
*	@file		dspReverbModel.cpp
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

#include "dspReverbModel.h"

DSP_RevModel::DSP_RevModel()
{
	// Tie the components to their buffers
	comb_L[0].set_buffer(buf_comb_L1, comb_tuning_L1);
	comb_R[0].set_buffer(buf_comb_R1, comb_tuning_R1);
	comb_L[1].set_buffer(buf_comb_L2, comb_tuning_L2);
	comb_R[1].set_buffer(buf_comb_R2, comb_tuning_R2);
	comb_L[2].set_buffer(buf_comb_L3, comb_tuning_L3);
	comb_R[2].set_buffer(buf_comb_R3, comb_tuning_R3);
	comb_L[3].set_buffer(buf_comb_L4, comb_tuning_L4);
	comb_R[3].set_buffer(buf_comb_R4, comb_tuning_R4);
	comb_L[4].set_buffer(buf_comb_L5, comb_tuning_L5);
	comb_R[4].set_buffer(buf_comb_R5, comb_tuning_R5);
	comb_L[5].set_buffer(buf_comb_L6, comb_tuning_L6);
	comb_R[5].set_buffer(buf_comb_R6, comb_tuning_R6);
	comb_L[6].set_buffer(buf_comb_L7, comb_tuning_L7);
	comb_R[6].set_buffer(buf_comb_R7, comb_tuning_R7);
	comb_L[7].set_buffer(buf_comb_L8, comb_tuning_L8);
	comb_R[7].set_buffer(buf_comb_R8, comb_tuning_R8);
	allpass_L[0].set_buffer(buf_allpass_L1, allpass_tuning_L1);
	allpass_R[0].set_buffer(buf_allpass_R1, allpass_tuning_R1);
	allpass_L[1].set_buffer(buf_allpass_L2, allpass_tuning_L2);
	allpass_R[1].set_buffer(buf_allpass_R2, allpass_tuning_R2);
	allpass_L[2].set_buffer(buf_allpass_L3, allpass_tuning_L3);
	allpass_R[2].set_buffer(buf_allpass_R3, allpass_tuning_R3);
	allpass_L[3].set_buffer(buf_allpass_L4, allpass_tuning_L4);
	allpass_R[3].set_buffer(buf_allpass_R4, allpass_tuning_R4);

	// Set default values
	allpass_L[0].set_feedback(0.5f);
	allpass_R[0].set_feedback(0.5f);
	allpass_L[1].set_feedback(0.5f);
	allpass_R[1].set_feedback(0.5f);
	allpass_L[2].set_feedback(0.5f);
	allpass_R[2].set_feedback(0.5f);
	allpass_L[3].set_feedback(0.5f);
	allpass_R[3].set_feedback(0.5f);
	set_wet(initial_wet);
	set_room_size(initial_room);
	set_dry(initial_dry);
	set_damp(initial_damp);
	set_width(initial_width);
	set_mode(initial_mode);

	// Buffer will be full of rubbish - so we MUST mute them
	mute();
}

/**
*	@brief	Mute reverb - zero all buffers samples
*	@param	none
*	@return void
*/
void DSP_RevModel::mute()
{
	if (get_mode() >= freeze_mode)
	{
		return;
	}

	for (int i = 0; i < num_combs; i++)
	{
		comb_L[i].mute();
		comb_R[i].mute();
	}

	for (int i = 0; i < num_allpasses; i++)
	{
		allpass_L[i].mute();
		allpass_R[i].mute();
	}
}

/**
*	@brief	Process while replacing output data
*	@param	inputL	pointer to float left input buffer
*	@param	inputR	pointer to float right input buffer
*	@param	outputL	pointer to float left output buffer
*	@param	outputR	pointer to float right output buffer
*	@param	numsamples number of sample (buffers length)
*	@param	skip samples pointers increment
*	@return void
*/
void DSP_RevModel::process_replace(float *input_L, float *input_R, float *output_L, float *output_R, long num_samples, int skip)
{
	float out_L, out_R, input;

	while (num_samples-- > 0)
	{
		out_L = out_R = 0;
		input = (*input_L + *input_R) * gain;

		// Accumulate comb filters in parallel
		for (int i = 0; i < num_combs; i++)
		{
			out_L += comb_L[i].process(input);
			out_R += comb_R[i].process(input);
		}

		// Feed through allpasses in series
		for (int i = 0; i < num_allpasses; i++)
		{
			out_L = allpass_L[i].process(out_L);
			out_R = allpass_R[i].process(out_R);
		}

		// Calculate output REPLACING anything already there
		*output_L = out_L * wet_1 + out_R * wet_2 + *input_L * dry;
		*output_R = out_R * wet_1 + out_L * wet_2 + *input_R * dry;

		// Increment sample pointers, allowing for interleave (if any)
		input_L += skip;
		input_R += skip;
		output_L += skip;
		output_R += skip;
	}
}

/**
*	@brief	Process while mixing new and old output data
*	@param	inputL	pointer to float left input buffer
*	@param	inputR	pointer to float right input buffer
*	@param	outputL	pointer to float left output buffer
*	@param	outputR	pointer to float right output buffer
*	@param	numsamples number of sample (buffers length)
*	@param	skip samples pointers increment
*	@return void
*/
void DSP_RevModel::process_mix(float *input_L, float *input_R, float *output_L, float *output_R, long num_samples, int skip)
{
	float out_L, out_R, input;

	while (num_samples-- > 0)
	{
		out_L = out_R = 0;
		input = (*input_L + *input_R) * gain;

		// Accumulate comb filters in parallel
		for (int i = 0; i < num_combs; i++)
		{
			out_L += comb_L[i].process(input);
			out_R += comb_R[i].process(input);
		}

		// Feed through allpasses in series
		for (int i = 0; i < num_allpasses; i++)
		{
			out_L = allpass_L[i].process(out_L);
			out_R = allpass_R[i].process(out_R);
		}

		// Calculate output MIXING with anything already there
		*output_L += out_L * wet_1 + out_R * wet_2 + *input_L * dry;
		*output_R += out_R * wet_1 + out_L * wet_2 + *input_R * dry;

		// Increment sample pointers, allowing for interleave (if any)
		input_L += skip;
		input_R += skip;
		output_L += skip;
		output_R += skip;
	}
}

/**
*	@brief	Recalculate internal values after parameter change
*	@param	none
*	@return void
*/
void DSP_RevModel::update()
{
	int i;

	wet_1 = wet * (width / 2 + 0.5f);
	wet_2 = wet * ((1 - width) / 2);

	if (mode >= freeze_mode)
	{
		room_size_1 = 1;
		damp_1 = 0;
		gain = muted;
	}
	else
	{
		room_size_1 = room_size;
		damp_1 = damp;
		gain = fixed_gain;
	}

	for (i = 0; i < num_combs; i++)
	{
		comb_L[i].set_feedback(room_size_1);
		comb_R[i].set_feedback(room_size_1);
	}

	for (i = 0; i < num_combs; i++)
	{
		comb_L[i].set_damp(damp_1);
		comb_R[i].set_damp(damp_1);
	}
}

// The following get/set functions are not inlined, because
// speed is never an issue when calling them, and also
// because as you develop the reverb model, you may
// wish to take dynamic action when they are called.

/**
*	@brief	Set room size 
*	@param	value room size 0.05-0.98
*	@return void
*/
void DSP_RevModel::set_room_size(float value)
{
	room_size = (value * scale_room) + offset_room;
	update();
}

/**
*	@brief	Set room size (int)
*	@param	value room size 5-98
*	@return void
*/
void DSP_RevModel::set_room_size(int value)
{
	float room = (float)value / 100.0f;

	if (room < 0.05f)
	{
		room = 0.05f;
	}
	else if (room > 0.98f)
	{
		room = 0.98f;
	}

	set_room_size(room);
}

/**
*	@brief	Return room size
*	@param	none
*	@return room size
*/
float DSP_RevModel::get_room_size()
{
	return (room_size - offset_room) / scale_room;
}

/**
*	@brief	Set damp factor
*	@param	value damp factor 0.0-1.0
*	@return void
*/
void DSP_RevModel::set_damp(float value)
{
	damp = value *scale_damp;
	update();
}

/**
*	@brief	Set damp factor (int)
*	@param	value damp factor 0-100
*	@return void
*/
void DSP_RevModel::set_damp(int value)
{
	float damp = (float)value / 100.0f;
	set_damp(damp);
}

/**
*	@brief	Return damp factor
*	@param	none
*	@return scaled damp factor
*/
float DSP_RevModel::get_damp()
{
	return damp / scale_damp;
}

/**
*	@brief	Set wet level
*	@param	value wet level 0.0-1.0
*	@return void
*/
void DSP_RevModel::set_wet(float value)
{
	wet = value * scale_wet;
	update();
}

/**
*	@brief	Set wet level (int)
*	@param	value wet level 0-100
*	@return void
*/
void DSP_RevModel::set_wet(int value)
{
	float wet = (float)value / 100.0f;
	set_wet(wet);
}

/**
*	@brief	Return wet level
*	@param	none
*	@return scaled wet level
*/
float DSP_RevModel::get_wet()
{
	return wet / scale_wet;
}

/**
*	@brief	Set dry level
*	@param	value dry level 0.0-1.0
*	@return void
*/
void DSP_RevModel::set_dry(float value)
{
	dry = value * scale_dry;
}

/**
*	@brief	Set dry level (int)
*	@param	value dry level 0-100
*	@return void
*/
void DSP_RevModel::set_dry(int value)
{
	float dry = (float)value / 100.0f;
	set_dry(dry);
}

/**
*	@brief	Return dry level
*	@param	none
*	@return scaled dry level
*/
float DSP_RevModel::get_dry()
{
	return dry / scale_dry;
}

/**
*	@brief	Set width
*	@param	value dry level 0.0-1.0
*	@return void
*/
void DSP_RevModel::set_width(float value)
{
	width = value;
	update();
}

/**
*	@brief	Set width (int)
*	@param	value dry level 0-100
*	@return void
*/
void DSP_RevModel::set_width(int value)
{
	float width = (float)value / 100.0f;
	set_width(width);
}

/**
*	@brief	Return width
*	@param	none
*	@return width
*/
float DSP_RevModel::get_width()
{
	return width;
}

/**
*	@brief	Set mode
*	@param	value mode 0.0-0.5
*	@return void
*/
void DSP_RevModel::set_mode(float value)
{
	mode = value;
	if (mode > 0.5f)
	{
		mode = 0.5f;
	}
	else if (mode < 0)
	{
		mode = 0;
	}

	update();
}

/**
*	@brief	Set mode (int)
*	@param	value mode level 0-50
*	@return void
*/
void DSP_RevModel::set_mode(int value)
{
	float mode = (float)value / 100.0f;
	set_mode(mode);
}

/**
*	@brief	Return mode
*	@param	none
*	@return 1 if mode >= freezemode 0 otherwise
*/
float DSP_RevModel::get_mode()
{
	if (mode >= freeze_mode)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
