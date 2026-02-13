/**
*	@file		dspWavetable.h
*	@author		Nahum Budin
*	@date		21-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	23_Jan-2021 1.1 Code refactoring and notaion.
*				2-Nov-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Wavetable of float samples handling.
*				Samples may be output as 2 values using 2 pointers.
*	
*/

#include <math.h>

#include "dspWavetable.h"
#include "../LibAPI/synthesizer.h"
#include "../utils/utils.h"

/**
*	@brief	Creates a new wavetable object instance
*	@param	i_d a unique identifier number
*	@param	table a pointer to a Wavetable structure
*	@return none
*/
DSP_Wavetable::DSP_Wavetable(int i_d, Wavetable *table)
{
	id = i_d;
	
	wavetable = table;
	wt_sample_freq = wavetable->base_freq;
		
	init();
}

DSP_Wavetable::~DSP_Wavetable()
{

}

/**
*	@brief	initialize a wavetable object instance
*	@param	none
*	@return void
*/
void DSP_Wavetable::init()
{
	pos_h1 = 0;
	pos_h2 = wavetable->size / 2;
	pos_l = 0;
}

/**
*	@brief	Randomaly select and set pointer_1 position.
*			pointer_2 position is set half the wavetable size away.
*	@param	none
*	@return void
*/
void DSP_Wavetable::randomize_play()
{
	pos_h1 = (int)(RND * (wavetable->size - 1)); 
	pos_h2 = (pos_h1 + wavetable->size / 2) % wavetable->size;
	pos_l = 0;
}

/**
*	@brief	Set base frequency (Hz)
*	@param	out_freq base frequency
*	@param	init_pointers if true pointers are initialized.
*	@return void
*/
void DSP_Wavetable::set_output_frequency(float out_freq, bool init_pointers)
{
	gen_freq = out_freq;
	wt_sample_freq = wavetable->base_freq;
	
	// Actual output freq relative to the wavetable sampled freq.
	wt_step = out_freq / wt_sample_freq;

	wt_step_hi = (int)floor(wt_step);
	wt_step_lo = wt_step - floor(wt_step);

	if (init_pointers)
	{
		init();
	}
}

/**
*	@brief	Get next wavetable output value
*	@param	out1 a pointer to a float variable that will hold output 1
*	@param	out2 a pointer to a float variable that will hold output 2
*	@return void
*/
void DSP_Wavetable::get_next_wavetable_value(float *out1, float *out2)
{
	int nexti;

	pos_h1 += wt_step_hi;
	pos_h2 += wt_step_hi;
	pos_l += wt_step_lo;

	if (pos_l >= 1.f)
	{
		pos_h1 += 1;
		pos_h2 += 1;
		pos_l -= 1.f;
	}

	if (pos_h1 >= wavetable->size)
	{
		pos_h1 %= wavetable->size;
	}

	if (pos_h2 >= wavetable->size)
	{
		pos_h2 %= wavetable->size;
	}

	// Linear interpolation
	nexti = pos_h1 + 1;
	if (nexti >= wavetable->size)
	{
		nexti %= wavetable->size;
	}
	*out1 = wavetable->samples[pos_h1] * (1.f - pos_l) + wavetable->samples[nexti] * pos_l;

	nexti = pos_h2 + 1;
	if (nexti >= wavetable->size)
	{
		nexti %= wavetable->size;
	}
	*out2 = wavetable->samples[pos_h2] * (1.f - pos_l) + wavetable->samples[nexti] * pos_l;
}

/**
*	@brief	Return a pointer to the wavetable samples
*	@param	none
*	@return a pointer to the wavetable samples
*/
float *DSP_Wavetable::get_wavetable() 
{ 	
	return wavetable->samples; 
} 

/**
*	@brief	Return wavetable table size.
*	@param	none
*	@return wavetable table size
*/
int DSP_Wavetable::get_wavetable_size() 
{ 
	return wavetable->size; 
}

/**
*	@brief	Return wavetable object id.
*	@param	none
*	@return wavetable object id.
*/
int DSP_Wavetable::get_id() 
{ 
	return id; 
}

/**
*	@brief	Set frequenct detune octave
*	@param	oct frequency detune octave _OSC_DETUNE_MIN_OCTAVE to _OSC_DETUNE_MAX_OCTAVE
*	@return void
*/
void DSP_Wavetable::set_freq_detune_oct(int oct)
{
	detune_octave = oct;

	if (detune_octave < _OSC_DETUNE_MIN_OCTAVE)
	{
		detune_octave = _OSC_DETUNE_MIN_OCTAVE;
	}
	else if (detune_octave > _OSC_DETUNE_MAX_OCTAVE)
	{
		detune_octave = _OSC_DETUNE_MAX_OCTAVE;
	}
}

/**
*	@brief	Return frequency detune octave
*	@param	none
*	@return frequency detune octave _OSC_DETUNE_MIN_OCTAVE to _OSC_DETUNE_MAX_OCTAVE
*/
int DSP_Wavetable::get_freq_detune_oct() 
{ 
	return detune_octave; 
}

/**
*	@brief	Set frequency detune semitones
*	@param	sem frequency detune semitones _OSC_DETUNE_MIN_SEMITONES to _OSC_DETUNE_MAX_SEMITONES
*	@return void
*/
void DSP_Wavetable::set_freq_detune_semitones(int sem)
{
	detune_semitones = sem;

	if (detune_semitones < _OSC_DETUNE_MIN_SEMITONES)
	{
		detune_semitones = _OSC_DETUNE_MIN_SEMITONES;
	}
	else if (detune_semitones > _OSC_DETUNE_MAX_SEMITONES)
	{
		detune_semitones = _OSC_DETUNE_MAX_SEMITONES;
	}
}

/**
*	@brief	Return frequency detune semitones
*	@param	none
*	@return frequenct detune semitones _OSC_DETUNE_MIN_SEMITONES to _OSC_DETUNE_MAX_SEMITONES
*/
int DSP_Wavetable::get_freq_detune_semitones() 
{ 
	return detune_semitones; 
}

/**
*	@brief	Set frequenct detune cents (int)
*	@param	cnt frequenct detune cents (int) (_OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS)/_OSC_DETUNE_CENTS_FACTORIAL
*	@return void
*/
void DSP_Wavetable::set_freq_detune_cents_set_value(int cnt)
{
	detune_cents_set_value = cnt;

	if (detune_cents_set_value < _OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL)
	{
		detune_cents_set_value = _OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL;
	}
	else if (detune_cents_set_value > _OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL)
	{
		detune_cents_set_value = _OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL;
	}

	float val = (float)detune_cents_set_value * _OSC_DETUNE_CENTS_FACTORIAL;

	set_freq_detune_cents(val);
}

/**
*	@brief	Return  frequency detune cents (int)
*	@param	none
*	@return frequenct detune cents (_OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS)/_OSC_DETUNE_CENTS_FACTORIAL
*/
int DSP_Wavetable::get_freq_detune_cents_set_value() 
{ 
	return detune_cents_set_value; 
}

/**
*	@brief	Set frequency detune cents (float)
*	@param	cnt frequency detune cents (float) _OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS
*	@return void
*/
void DSP_Wavetable::set_freq_detune_cents(float cnt)
{
	detune_cents = cnt;

	if (detune_cents < _OSC_DETUNE_MIN_CENTS)
	{
		detune_cents < _OSC_DETUNE_MIN_CENTS;
	}
	else if (detune_cents > _OSC_DETUNE_MAX_CENTS)
	{
		detune_cents < _OSC_DETUNE_MAX_CENTS;
	}
}

/**
*	@brief	Return frequenct detune cents (float)
*	@param	none
*	@return frequenct detune cents (float) _OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS
*/
float DSP_Wavetable::get_freq_detune_cents() 
{ 
	return detune_cents; 
}

/**
*	@brief	Set magnitude (volume)
*	@param	mag output level magnitude 0.0-1.0
*	@return void
*/
void DSP_Wavetable::set_magnitude(float mag)
{
	magnitude = mag;
	if (magnitude < 0)
	{
		magnitude = 0.0f;
	}
	else if (magnitude > 1.0f)
	{
		magnitude = 1.0f;
	}
}

/**
*	@brief	Return  magnitude (volume)
*	@param	none
*	@return  output level magnitude 0.0-1.0
*/
float DSP_Wavetable::get_magnitde() 
{ 
	return magnitude; 
}

/**
*	@brief	Set send level to channel 1
*	@param	snd send level to channel 1 0.0-1.0
*	@return void
*/
float DSP_Wavetable::set_send_level_1(float snd)
{
	send_level_1 = snd;
	
	if (send_level_1 < 0.0f)
	{	
		send_level_1 = 0.0f;
	}
	else if (send_level_1 > 1.0f)
	{	
		send_level_1 = 1.0f;
	}

	return send_level_1;
}

/**
*	@brief	Set send level to channel 1 (int)
*	@param	snd send level to channel 1 0-100
*	@return MSO send level to channel 1 0.0-1.0
*/
float DSP_Wavetable::set_send_level_1(int snd)
{
	float send = (float)snd / 100.0f;

	set_send_level_1(send);
	return send;
}

/**
*	@brief	Set send level to channel 2
*	@param	snd send level to channel 2 0.0-1.0
*	@return void
*/
float DSP_Wavetable::set_send_level_2(float snd)
{
	send_level_2 = snd;
	
	if (send_level_2 < 0.0f)
	{
		send_level_2 = 0.0f;
	}
	else if (send_level_2 > 1.0f)
	{
		send_level_2 = 1.0f;
	}

	return send_level_2;
}

/**
*	@brief	Set send level to channel 2 (int)
*	@param	snd send level to channel 2 0-100
*	@return send level to channel 2 0.0-1.0
*/
float DSP_Wavetable::set_send_level_2(int snd)
{
	float send = (float)snd / 100.0f;
	
	set_send_level_2(send);
	return send;
}

/**
*	@brief	Return send level to channel 1
*	@param	none
*	@return MSO send level to channel 1 0.0-1.0
*/
float DSP_Wavetable::get_send_level_1() 
{ 
	return send_level_1; 
}

/**
*	@brief	Return send level to channel 2
*	@param	none
*	@return MSO send level to channel 2 0.0-1.0
*/
float DSP_Wavetable::get_send_level_2() 
{ 
	return send_level_2; 
}
