/**
*	@file		dspKarplusStrong.h
*	@author		Nahum Budin
*	@date		21-Seo-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	23-Jan-2021 1.1
*					1. Code refactoring and notaion. 
*					2. Add audio sample-rate settings
*				2-Nov-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Karpuls Strong String simulation generator.
*	
*	http://amid.fish/javascript-karplus-strong
*/

#include <math.h>
#include <stdlib.h>

#include "dspKarplusStrong.h"
#include "../utils/utils.h"

uint16_t DSP_KarplusStrong::instance_count = 0;

/**
*	@brief	Creates a Karplus Strong generator instance
*	@param	voice unique id number
*	@return void
*/
DSP_KarplusStrong::DSP_KarplusStrong(int voice, int samp_rate)
{
	buffer = NULL;
	
	set_sample_rate(samp_rate); // Also initiates the buffer, max_buff_size, buffer_index and buffer_len 
	
	voice_id = voice;
	
	seed = 1 + instance_count++;
	srand(seed);
	
	excitation_waveform_type = _KARPLUS_STRONG_EXCITATION_WHITE_NOISE;
	string_dumping_calculation_mode = _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT;
	
	state = _KARPLUS_STRONG_STATE_STEADY_OUTPUT;
	
	string_damping = (float)(_KARPLUS_STRONG_MIN_STRING_DAMPING + 
		(_KARPLUS_STRONG_MAX_STRING_DAMPING - _KARPLUS_STRONG_MIN_STRING_DAMPING) / 2.0f);		
	string_damping_variation = (float)(_KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION + 
		(_KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION - _KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION) / 2.0f);
	
	lpf_smoothing_factor = string_damping;
	
	on_decay = (float)(_KARPLUS_STRONG_MIN_DECAY + 
		(_KARPLUS_STRONG_MAX_DECAY - _KARPLUS_STRONG_MIN_DECAY) / 2.0f);
	off_decay = on_decay;
	
	prior_samp = 0;
	active_decay = on_decay;
	buffer_index = 0;
	excitation_waveform_variations = 50;
	
	pluck_damping = (_KARPLUS_STRONG_MAX_PLUCK_DAMPING + _KARPLUS_STRONG_MIN_PLUCK_DAMPING) / 2.0;
	pluck_damping_variation = (_KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION + 
								_KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION) / 2.0f;
	character_variation = (_KARPLUS_STRONG_MAX_CHARACTER_VARIATION + 
							_KARPLUS_STRONG_MIN_CHARACTER_VARIATION) / 2.0f;
	
	magnitude = 0.5f;
	freq = 440.0f;
	
	for (int i = 0; i < max_buf_size; i++)
	{
		buffer[i] = 0.f;
	}
	
	pluck_damping_variation_max = _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION;
	pluck_damping_variation_difference = 0;
	energy = 0;
}

/**
*	@brief	Sets sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100)
*					Update max_buffer_size, buffer_len, buffer_index and initiate the buffer .
*	@return set sample-rate
*/	
int DSP_KarplusStrong::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	max_buf_size = get_buffer_max_size();
	buffer_len = max_buf_size;
	buffer_index = 0;
	
	init_buffer();
	
	return sample_rate;
}

/**
*	@brief	Return set sample rate
*	@param	none
*	@return excitation type _KARPLUS_STRONG_EXCITATION_WHITE_NOISE.... see libAdjHeartModSynth_1.h
*/
int DSP_KarplusStrong::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*	@brief	Allocates buffer with size set according to sample rate
*			Update also max_buffer_size, buffer_len, buffer_index and initiate the buffer
*	@param	none
*	@return buffer size
*/
int DSP_KarplusStrong::init_buffer()
{
	if (buffer != NULL)
	{
		delete[] buffer;
	}
	
	max_buf_size = get_buffer_max_size();
	buffer_len = max_buf_size;
	buffer_index = 0;
	
	buffer = new float[max_buf_size];
	
	return max_buf_size;
}

/**
*	@brief	Set excitation waveform type
*	@param	en excitation type _KARPLUS_STRONG_EXCITATION_WHITE_NOISE.... see libAdjHeartModSynth_1.h
*	@return void
*/
void DSP_KarplusStrong::set_excitation_waveform_type(int et)
{
	excitation_waveform_type = et;
	
	if (excitation_waveform_type < _KARPLUS_STRONG_EXCITATION_WHITE_NOISE)
	{
		excitation_waveform_type = _KARPLUS_STRONG_EXCITATION_WHITE_NOISE;
	}
	else if (excitation_waveform_type > _KARPLUS_STRONG_EXCITATION_SQUARE_WAVE)
	{
		excitation_waveform_type = _KARPLUS_STRONG_EXCITATION_SQUARE_WAVE;
	}
}

/**
*	@brief	Return excitation waveform type
*	@param	none
*	@return excitation type _KARPLUS_STRONG_EXCITATION_WHITE_NOISE.... see LibAPI/synthesizer.h
*/
int DSP_KarplusStrong::get_excitation_waveform_type() 
{ 
	return 
		excitation_waveform_type; 
}

/**
*	@brief	Return output energy level
*	@param	none
*	@return output energy level
*/
float DSP_KarplusStrong::get_energy() 
{ 
	return energy; 
}

/**
*	@brief	Set excitation waveform variations level
*	@param	wv excitation waveform variations level (0-100)
*	@return void
*/
void DSP_KarplusStrong::set_excitation_waveform_variations(int wv)
{
	excitation_waveform_variations = wv;
	
	if (excitation_waveform_variations < 0)
	{
		excitation_waveform_variations = 0;
	}
	else if (excitation_waveform_variations > 100)
	{
		excitation_waveform_variations = 100;
	}
}

/**
*	@brief	Return excitation waveform variations level
*	@param	none
*	@return excitation waveform variations level (0-100)
*/
int DSP_KarplusStrong::get_excitation_waveform_variations() 
{ 
	return excitation_waveform_variations; 
}

/**
*	@brief	Set pluck damping level
*	@param	pd pluck damping level _KARPLUS_STRONG_MIN_PLUCK_DAMPING to _KARPLUS_STRONG_MAX_PLUCK_DAMPING
*	@return void
*/
void DSP_KarplusStrong::set_pluck_damping(float pd)
{
	pluck_damping = pd;
	
	if (pluck_damping > _KARPLUS_STRONG_MAX_PLUCK_DAMPING)
	{
		pluck_damping = _KARPLUS_STRONG_MAX_PLUCK_DAMPING;
	}
	else if (pluck_damping < _KARPLUS_STRONG_MIN_PLUCK_DAMPING)
	{
		pluck_damping = _KARPLUS_STRONG_MIN_PLUCK_DAMPING;
	}
}

void DSP_KarplusStrong::set_pluck_damping(int pd) {} // TODO: ?

/**
*	@brief	Return pluck damping level
*	@param	none
*	@return pluck damping level _KARPLUS_STRONG_MIN_PLUCK_DAMPING to _KARPLUS_STRONG_MAX_PLUCK_DAMPING
*/
float DSP_KarplusStrong::get_pluck_damping() 
{ 
	return pluck_damping; 
}

/**
*	@brief	Set pluck damping variation level
*	@param	pdv pluck damping variation level 
*			_KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION to _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION
*	@return void
*/
void DSP_KarplusStrong::set_pluck_damping_variation(float pdv)
{
	pluck_damping_variation = pdv;
	
	if (pluck_damping_variation > _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION)
	{
		pluck_damping_variation = _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION;
	}
	else if (pluck_damping_variation < _KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION)
	{
		pluck_damping_variation = _KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION;
	}
}

void DSP_KarplusStrong::set_pluck_damping_variation(int pdv) {} // TODO: ?

/**
*	@brief	Return pluck damping variation level
*	@param	none
*	@returnpluck damping variation level
*			_KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION to _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION
*/
float DSP_KarplusStrong::get_pluck_damping_variation() 
{ 
	return pluck_damping_variation; 
}

/**
*	@brief	Set string damping  level
*	@param	sd string damping level
*			_KARPLUS_STRONG_MIN_STRING_DAMPING to _KARPLUS_STRONG_MAX_STRING_DAMPING
*	@return void
*/
void DSP_KarplusStrong::set_string_damping(float sd)
{
	string_damping = sd;
	
	if (string_damping > _KARPLUS_STRONG_MAX_STRING_DAMPING)
	{
		string_damping = _KARPLUS_STRONG_MAX_STRING_DAMPING;
	}
	else if (string_damping < _KARPLUS_STRONG_MIN_STRING_DAMPING)
	{
		string_damping = _KARPLUS_STRONG_MIN_STRING_DAMPING;
	}
}

/**
*	@brief	Set string damping level (int)
*	@param	sd string damping level 0-100
*	@return void
*/
void DSP_KarplusStrong::set_string_damping(int sd)
{
	set_string_damping(_KARPLUS_STRONG_MIN_STRING_DAMPING +
						(float)sd / 100.0f * (float)(_KARPLUS_STRONG_MAX_STRING_DAMPING - 
							_KARPLUS_STRONG_MIN_STRING_DAMPING));
}

/**
*	@brief	return string damping  level
*	@param	none
*	@returnstring damping level
*			_KARPLUS_STRONG_MIN_STRING_DAMPING to _KARPLUS_STRONG_MAX_STRING_DAMPING
*/
float DSP_KarplusStrong::get_string_damping() 
{ 
	return string_damping;
}

/**
*	@brief	Set string damping variation level
*	@param	sd string damping variation level
*			_KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION to _KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION
*	@return void
*/
void DSP_KarplusStrong::set_string_damping_variation(float sdv)
{
	string_damping_variation = sdv;
	
	if (string_damping_variation > _KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION)
	{
		string_damping_variation = _KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION;
	}
	else if (string_damping_variation < _KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION)
	{
		string_damping_variation = _KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION;
	}
}

/**
*	@brief	Set string damping variation level (int)
*	@param	sd string damping variation level 0-100
*	@return void
*/
void DSP_KarplusStrong::set_string_damping_variation(int sdv)
{
	set_string_damping_variation(_KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION +
		(float)sdv / 100.0f * 
		(float)(_KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION -
			_KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION));
}

/**
*	@brief	Return string damping variation level
*	@param	none
*	@return string damping variation level
*			_KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION to _KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION
*/
float DSP_KarplusStrong::get_string_damping_variation() 
{ 
	return string_damping_variation; 
}

/**
*	@brief	Set characteristics variation level
*	@param	cv characteristics variation level
*			_KARPLUS_STRONG_MIN_CHARACTER_VARIATION to _KARPLUS_STRONG_MAX_CHARACTER_VARIATION
*	@return void
*/
void DSP_KarplusStrong::set_character_variation(float cv)
{
	character_variation = cv;

	if (character_variation > _KARPLUS_STRONG_MAX_CHARACTER_VARIATION)
	{
		character_variation = _KARPLUS_STRONG_MAX_CHARACTER_VARIATION;
	}
	else if (character_variation < _KARPLUS_STRONG_MIN_CHARACTER_VARIATION)
	{
		character_variation = _KARPLUS_STRONG_MIN_CHARACTER_VARIATION;
	}
}

void DSP_KarplusStrong::set_character_variation(int cv) {}	// TODO: ?

/**
*	@brief	Return characteristics variation level
*	@param	none
*	@return characteristics variation level
*			_KARPLUS_STRONG_MIN_CHARACTER_VARIATION to _KARPLUS_STRONG_MAX_CHARACTER_VARIATION
*/
float DSP_KarplusStrong::get_character_variation() 
{ 
	return character_variation; 
}

/**
*	@brief	Set note on decay time
*	@param	dec note on decay time _KARPLUS_STRONG_MIN_DECAY to _KARPLUS_STRONG_MAX_DECAY
*	@return void
*/
void DSP_KarplusStrong::set_on_decay(float dec)
{
	on_decay = dec;
	
	if (on_decay < _KARPLUS_STRONG_MIN_DECAY)
	{
		on_decay = _KARPLUS_STRONG_MIN_DECAY;
	}
	else if (on_decay > _KARPLUS_STRONG_MAX_DECAY)
	{
		on_decay = _KARPLUS_STRONG_MAX_DECAY;
	}
}

/**
*	@brief	Set note on decay time int
*	@param	dec note on decay time 0-100
*	@return void
*/
void DSP_KarplusStrong::set_on_decay(int dec)
{
	int decay = dec;
	
	if ((decay < 0) || (decay > 100))
	{
		return;
	}
	
	set_on_decay(_KARPLUS_STRONG_MIN_DECAY +
		(float)decay / 100.0f *
		(float)(_KARPLUS_STRONG_MAX_DECAY - _KARPLUS_STRONG_MIN_DECAY));
}

/**
*	@brief	Return note on decay time
*	@param	none
*	@return note on decay time _KARPLUS_STRONG_MIN_DECAY to _KARPLUS_STRONG_MAX_DECAY
*/
float DSP_KarplusStrong::get_on_decay() 
{ 
	return on_decay; 
}

/**
*	@brief	Set note off decay time
*	@param	dec note off decay time _KARPLUS_STRONG_MIN_DECAY to _KARPLUS_STRONG_MAX_DECAY
*	@return void
*/
void DSP_KarplusStrong::set_off_decay(float dec)
{
	off_decay = dec;
	
	if (off_decay < _KARPLUS_STRONG_MIN_DECAY) 
	{
		off_decay = _KARPLUS_STRONG_MIN_DECAY;
	}
	else if (off_decay > _KARPLUS_STRONG_MAX_DECAY) 
	{
		off_decay = _KARPLUS_STRONG_MAX_DECAY;
	}
}

/**
*	@brief	Set note off decay time int
*	@param	dec note off decay time 0-100
*	@return void
*/
void DSP_KarplusStrong::set_off_decay(int dec)
{
	int decay = dec;
	
	if ((decay < 0) || (decay > 100))
	{
		return;
	}
	
	set_off_decay(_KARPLUS_STRONG_MIN_DECAY +
		(float)decay / 100.0f *
		(float)(_KARPLUS_STRONG_MAX_DECAY - _KARPLUS_STRONG_MIN_DECAY));
}

/**
*	@brief	Return note off decay time
*	@param	none
*	@return note off decay time _KARPLUS_STRONG_MIN_DECAY to _KARPLUS_STRONG_MAX_DECAY
*/
float DSP_KarplusStrong::get_off_decay() 
{ 
	return 
		off_decay; 
}

/**
*	@brief	Set string dumping calculation mode
*	@param	dcm string dumping calculation mode _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT or _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC
*	@return void
*/
void DSP_KarplusStrong::set_string_dumping_calculation_mode(int dcm)
{
	string_dumping_calculation_mode = dcm;
	
	if (string_dumping_calculation_mode < _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT)
	{
		string_dumping_calculation_mode = _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT;
	}
	else if (string_dumping_calculation_mode > _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC)
	{
		string_dumping_calculation_mode = _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC;
	}
}

/**
*	@brief	Return string dumping calculation mode
*	@param	none
*	@return string dumping calculation mode _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT or _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC
*/
int DSP_KarplusStrong::get_string_dumping_calculation_mode() 
{ 
	return string_dumping_calculation_mode; 
}

/**
*	@brief	Set output level (volume)
*	@param	amp output level (volume) 0.0-1.0
*	@return void
*/
void DSP_KarplusStrong::set_amplitude(float amp) {
	
	magnitude = amp;
	
	if (magnitude < 0.0) 
	{
		magnitude = 0.0;
	}
	else if (magnitude > 1.0) 
	{
		magnitude = 1.0;
	}
}

/**
*	@brief	Return output level (volume)
*	@param	none
*	@return output level (volume) 0.0-1.0
*/
float DSP_KarplusStrong::get_amplitude() 
{
	return magnitude; 
}

/**
*	@brief	Initiates a note-on generation
*	@param	notenum not MIDI number A0(21) to C8(108)
*	@param	velocity note volume
*	@return void
*/
void DSP_KarplusStrong::note_on(int notenum, float velocity)
{
	int note = notenum;
	
	if (note < 21)
	{
		note = 21; // A0
	}
	else if (note > 108)
	{
		note = 108; // C8
	}
	
	calc_lpf_smoothing_factor(notenum);
	calc_pluck_damping_coefficient();
	
	freq = 27.5 * pow(2.0f, (notenum - 21) / 12.0f);
	
	if (freq < _KARPLUS_STRONG_MIN_FREQ)
	{
		freq = _KARPLUS_STRONG_MIN_FREQ;
	}
	else if (freq > _KARPLUS_STRONG_MAX_FREQ)
	{
		freq = _KARPLUS_STRONG_MAX_FREQ;
	}
	
	if (velocity > 1.0f)
	{
		velocity = 0.0f;
	}
	else if (velocity <= 0.0f)
	{
		note_off();
		return;
	}

	magnitude = velocity;
	
	int len = (int)((float)sample_rate / freq + 0.5f);
	
	if (len > max_buf_size)
	{
		len = max_buf_size;
	}

	buffer_len = len;
	buffer_index = 0;	
	active_decay = on_decay;
	
	init_excitation_samples();
	
	state = _KARPLUS_STRONG_STATE_STEADY_BEGIN_NEXT;
}	
	
/**
*	@brief	Initiates a note-off of current playing note
*	@param	none
*	@return void
*/
void DSP_KarplusStrong::note_off()
{
	state = _KARPLUS_STRONG_STATE_STEADY_OUTPUT;
	active_decay = off_decay;
}

/**
*	@brief	Return the buffer maximum length based on sample-rate
*	@param	none
*	@return the next white noise output sample
*/
int DSP_KarplusStrong::get_buffer_max_size()
{
	return (int)((float)sample_rate / _KARPLUS_STRONG_MIN_FREQ + 0.5f);
}

/**
*	@brief	Return the next white noise output sample
*	@param	none
*	@return the next white noise output sample
*/
float DSP_KarplusStrong::get_next_white_noise_val()
{
	return (float)rand() / (float)RAND_MAX * 1.95 - 1.0;
}

/**
*	@brief	Return the next pink noise output sample
*	@param	none
*	@return the next pink noise output sample
*/
float DSP_KarplusStrong::get_next_pink_noise_val()
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
	out *= 0.2; //0.11;     // (roughly) compensate for gain

	b6 = white * 0.115926;

	return out;
}

/**
*	@brief	Return the next brown noise output sample
*	@param	none
*	@return the next brown noise output sample
*/
float DSP_KarplusStrong::get_next_brown_noise_val()
{
	float out;
	static float lastOut = 0.0f;	
	float white = get_next_white_noise_val();

	out = (lastOut + (0.02 * white)) / 1.02;
	lastOut = out;
	out *= 5.5; //3.5;     // (roughly) compensate for gain

	return out;
}

/**
*	@brief	Return the next sawtooth generator output sample
*	@param	none
*	@return the next sawtooth generator output sample
*/
float DSP_KarplusStrong::get_next_sawtooth_wave_val(int pos)
{
	float out;
	int peak = (int)(((float)excitation_waveform_variations / 2.0 + 1.0f) / 100.0f * buffer_len + 0.5f);
	
	if (pos <= peak)
	{
		out = (float)pos / (float)peak * 2.0f - 1.0f;
	}
	else
	{
		out = (float)(buffer_len - pos) / (float)(buffer_len - peak) * 2.0 - 1.0;
	}
	
	if (out < -1.0f)
	{
		return -1.0f;
	}
	else if (out > 1.0f)
	{
		return 1.0f;
	}
	else
	{
		return out;
	}
}

/**
*	@brief	Return the next square wave generator output sample
*	@param	none
*	@return the next square wave generator output sample
*/
float DSP_KarplusStrong::get_next_square_wave_val(int pos)
{
	int peak = (int)(((float)excitation_waveform_variations / 2.0 + 5.0f) / 100.0f * buffer_len + 0.5f);
	
	if (pos < peak)
	{
		return 1.0f;
	}
	else
	{
		return 0;
	}
}

/**
*	@brief	Return the next sine chirp generator output sample
*	@param	none
*	@return the next sine chirp generator output sample
*/
float DSP_KarplusStrong::get_next_sine_chirp_val(int pos)
{
	return (float)sin((double)pos*((float)excitation_waveform_variations / 15.0f + 1.5f) * M_PI  / buffer_len);
}

/**
*	@brief	Return the next decayed sine generator output sample
*	@param	none
*	@return the next sawtodecayed sineoth generator output sample
*/
float DSP_KarplusStrong::get_next_decayed_sine_val(int pos)
{
	float time = (float)pos / (float)sample_rate;
	float out = magnitude * pow(2.0, -active_decay * time) * sin(2.0 * M_PI * 
				freq * time * ((float)excitation_waveform_variations / 150.0f + 1.0f));
	
	buffer[buffer_index] = out;
	
	return out;
}

/**
*	@brief	Return the next excitation output sample
*	@param	none
*	@return the next excitation output sample
*/
float DSP_KarplusStrong::get_next_excitation_val(int pos)
{
	float outputVal;

	switch (excitation_waveform_type)
	{		
		case _KARPLUS_STRONG_EXCITATION_WHITE_NOISE:
			outputVal = get_next_white_noise_val();
			break;
					
		case _KARPLUS_STRONG_EXCITATION_PINK_NOISE:
			outputVal =  get_next_pink_noise_val();
			break;
					
		case _KARPLUS_STRONG_EXCITATION_BROWN_NOISE:
			outputVal =  get_next_brown_noise_val();
			break;
			
		case _KARPLUS_STRONG_EXCITATION_SINECHIRP:
			outputVal =  get_next_sine_chirp_val(pos);
			break;
				
		case _KARPLUS_STRONG_EXCITATION_DECAYEDSINE:
			outputVal =  get_next_decayed_sine_val(pos);
			break;
				
		case _KARPLUS_STRONG_EXCITATION_SAWTOOTH_WAVE:
			outputVal =  get_next_sawtooth_wave_val(pos);
			break;
				
		case _KARPLUS_STRONG_EXCITATION_SQUARE_WAVE:
			outputVal =  get_next_square_wave_val(pos);
			break;
	}

	outputVal *= (1.0 - character_variation);
	outputVal += character_variation * get_next_white_noise_val();

	return outputVal;		
}

/**
*	@brief	Return calculated pluck dumping coefficient
*	@param	none
*	@return the calculated pluck dumping coefficient
*/
float DSP_KarplusStrong::calc_pluck_damping_coefficient()
{
	/*
		|- pluckDampingMax
		|
		|               | - pluckDampingVariationMax         | -
		|               | (pluckDampingMax - pluckDamping) * |
		|               | pluckDampingVariation              | pluckDamping
		|- pluckDamping | -                                  | Variation
		|               | (pluckDamping - pluckDampingMin) * | Difference
		|               | pluckDampingVariation              |
		|               | - pluckDampingVariationMin         | -
		|
		|- pluckDampingMin
		*/

	pluck_damping_variation_min = 
		pluck_damping - (pluck_damping - _KARPLUS_STRONG_MIN_PLUCK_DAMPING) * pluck_damping_variation;
	
	pluck_damping_variation_max =
	    pluck_damping + (_KARPLUS_STRONG_MAX_PLUCK_DAMPING - pluck_damping) * pluck_damping_variation;
	
	pluck_damping_variation_difference =
	    pluck_damping_variation_max - pluck_damping_variation_min;
	
	pluck_damping_coefficient =
	    pluck_damping_variation_min + ((float)rand() / (float)RAND_MAX) * pluck_damping_variation_difference;

	return pluck_damping_coefficient;
}

/**
*	@brief	Return calculated lowpass filter smoothing factor
*	@param	none
*	@return calculated lowpass filter smoothing factor
*/
float DSP_KarplusStrong::calc_lpf_smoothing_factor(int noteNum)
{
	if (string_dumping_calculation_mode == _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT)
	{
		lpf_smoothing_factor = string_damping;
	}
	else if (string_dumping_calculation_mode == _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC)
	{
		// this is copied verbatim from the flash one
		// is magical, don't know how it works
		lpf_smoothing_factor = string_damping + pow(noteNum / 44.0, 0.5) * (1 - string_damping) * 0.5 +
		    (1 - string_damping) * (float)rand() / (float)RAND_MAX * string_damping_variation;
	}

	return lpf_smoothing_factor;
}

/**
*	@brief	update last sample value in buffer
*	@param	none
*	@return none
*/
void DSP_KarplusStrong::update_prev_samp()
{
	if (buffer_index > 0) 
	{
		prior_samp = buffer[buffer_index - 1];
	}
	else 
	{
		prior_samp = buffer[buffer_len - 1];
	}		
}

/**
*	@brief	Fill buffer with excitation samples
*	@param	none
*	@return none
*/
void DSP_KarplusStrong::init_excitation_samples()
{
	for (int i = 0; i < buffer_len; i++)
	{
		buffer[i] = get_next_excitation_val(i) * magnitude;
	}
}

/**
*	@brief	Return next output sample
*	@param	none
*	@return none
*/
float DSP_KarplusStrong::get_next_output_value()
{
	//	static float prior;
	float in, out = 0;	
	in = buffer[buffer_index];
	out = low_pass(prior_samp, in, lpf_smoothing_factor); // * normDecay;//decay; 
	buffer[buffer_index] = out * active_decay;
	
	if (++buffer_index >= buffer_len)
	{
		buffer_index = 0;
	}

	prior_samp = in;
	
	//	resonate();
//	if(out < -0.99f)
//		out = -0.99f;
//	else if(out > 0.99f)
//		out = 0.99f;
	energy = 0.999f * energy + 0.001f * out * out;
	return out * 4.f;	
}

/**
*	@brief	Resonate
*	@param	none
*	@return none
*/
void DSP_KarplusStrong::resonate()
{
	/*const*/ float c0 = 2.0 * sin(M_PI * 3.4375 / (float)sample_rate); // not a constant: sample_rate is changeable
	/*const*/ float c1 = 2.0 * sin(M_PI * 6.124928687214833 / (float)sample_rate);
	const float r0 = 0.98;
	const float r1 = 0.98;
	
	static float r00 = 0.0;
	static float f00 = 0.0;
	static float r10 = 0.0;
	static float f10 = 0.0;
	static float f0 = 0.0;
	static float resonated_sample = 0.0;
	static float resonated_sample_post_high_pass = 0.0;
	// by making the smoothing factor large, we make the cutoff
	// frequency very low, acting as just an offset remover
	const float highPassSmoothingFactor = 0.97;
	static float lastOutput = 0.0;
	static float lastInput = 0.0;
	
	r00 *= r0;
	r00 += (f0 - f00) * c0;
	f00 += r00;
	f00 -= f00 * f00 * f00 * 0.166666666666666;
	r10 *= r1;
	r10 += (f0 - f10) * c1;
	f10 += r10;
	f10 -= f10 * f10 * f10 * 0.166666666666666;
	f0 = buffer[buffer_index];
	resonated_sample = f0 + (f00 + f10) * 2.0;

	// I'm not sure why, but the resonating process plays
	// havok with the DC offset - it jumps around everywhere.
	// We put it back to zero DC offset by adding a high-pass
	// filter with a super low cutoff frequency.
	resonated_sample_post_high_pass = resonated_sample; //highPass(
	//	    lastOutput,
	//		lastInput,
	//		resonatedSample,
	//		highPassSmoothingFactor);
	buffer[buffer_index] = resonated_sample_post_high_pass;

	lastOutput = resonated_sample_post_high_pass;
	lastInput = resonated_sample;
}

/**
*	@brief	A lowpass filter
*	@param	lastOutput last(previous) lpf output sample
*	@param	currentInput input sample
*	@param	smoothingFactor smoothing factor
*	@return none
*/
float DSP_KarplusStrong::low_pass(float last_output, float current_input, float smoothing_factor)
{
	float current_output = 0.0;
	current_output = smoothing_factor * current_input + (1.0 - smoothing_factor) * last_output;
	return current_output;
}

/**
*	@brief	A highpass filter
*	@param	lastOutput last(previous) hpf output sample
*	@param	currentInput input sample
*	@param	smoothingFactor smoothing factor
*	@return none
*/
float DSP_KarplusStrong::high_pass(float last_output, float last_input, float current_input, float smoothing_factor)
{
	float current_output = 0.0;
	current_output = smoothing_factor * smoothing_factor * (last_output + current_input - last_input);

	return current_output;
}	

