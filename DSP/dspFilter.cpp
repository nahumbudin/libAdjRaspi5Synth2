/**
 *	@file		dspFilter.h
 *	@author		Nahum Budin
 *	@date		17-Jan-2026
 *	@version	1.3
 *					1. Octave exp. multiplied by modulation factor and not added..
 *
 *	@History	1.2	14-Sep-2024
 *					1. Code refactoring and notaion.
 *				23_Jan-2021	1.1
 *								1. Code refactoring and notaion.
 *								2. Adding sample-rate settings
 *				31-Oct-201991.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
 *
 *	@brief		State variable filter.
 */

#include "dspFilter.h"

#include "../LibAPI/synthesizer.h"
#include "../LibAPI/audio.h"
#include "../utils/utils.h"

float DSP_Filter::max_setting_fcenter;
float DSP_Filter::max_setting_fmult;

DSP_Filter::DSP_Filter(int iD, int samp_rate)
{
	id = iD;
	// Default values
	set_sample_rate(samp_rate);
	set_frequency(440.0f);
	set_octave(1.0f); 
	set_resonance(0.7f);
	state_input_prev = 0.0f;
	state_lowpass = 0.0f;
	state_highpass = 0.0f;
	state_bandpass = 0.0f;
	filter_band = _FILTER_BAND_PASS_ALL;
	filters_balance = 50;
	kbd_track = 1.0f;
	set_kbd_freq(get_filter_max_center_frequency()); // TODO: ; must be after setting kbd tracking
	lowpass_tmp = 0.0f;
	bandpass_tmp = 0.0f;
	highpass_tmp = 0.0f;
	
	max_setting_fcenter = (float(samp_rate) / 2.5 * (3.141592654 / (float(samp_rate) * 2.0)));
	max_setting_fmult = sinf((float(samp_rate) / 2.5) * (3.141592654 / (float(samp_rate) * 2.0)));

	clear();
}


void DSP_Filter::clear()
{
	state_input_prev = 0.0f;
	state_lowpass = 0.0f;
	state_bandpass = 0.0f;
}

/**
*	@brief	Set filter frequency
*	@param	fr frequency 20.0 to AUDIO_SAMPLE_RATE/2.5 (Hz)
*	@return void
*/
void DSP_Filter::set_frequency(float fr)
{
	float freq = fr;
	
	if (freq < 20.0)
	{
		freq = 20.0;
	}
	else if (freq > float(sample_rate) / 2.5) 
	{ 
		freq = float(sample_rate) / 2.5;
	}
	
	setting_fcenter = freq * 3.141592654 / (float(sample_rate) * 2.0);
	setting_fmult = sinf(freq * (3.141592654 / (float(sample_rate) * 2.0)));	
}

/**
*	@brief	Set filter frequency int 
*	@param	fr frequency int 0-100 (0: _FILTER_MIN_CENTER_FREQ; 100: _FILTER_MAX_CENTER_FREQ)
*	@return void
*/
void DSP_Filter::set_frequency_relative(int fr)
{
	set_frequency(_FILTER_MIN_CENTER_FREQ +
		(get_filter_max_center_frequency() - _FILTER_MIN_CENTER_FREQ) * fr / 100.0f);
}

/**
*	@brief	Set filter octave
*	@param	oct octave 0.0 to 6.9999
*	@return void
*/
void DSP_Filter::set_octave(float oct)
{
	float n = oct;
	
	// filter's corner frequency is Fcenter * 2^(control * N)
	// where "control" ranges from -1.0 to +1.0
	// and "N" allows the frequency to change from 0 to 7 octaves
	if (n < 0.0)
	{ 
		n = 0.0;
	}
	else if (n > 6.9999)
	{
		n = 6.9999;
	}
	setting_octave_mult = n;	
}

/**
*	@brief	Set filter octave int
*	@param	oct 0-100
*	@return void
*/
void DSP_Filter::set_octave_relative(int oct)
{
	set_octave((float)oct * 6.9999f / 100.0f);
}

/**
*	@brief	Set filter Reaonance (Q)
*	@param	res 0.7 to 5.0
*	@return void
*/
void DSP_Filter::set_resonance(float res)
{
	float q = res;
	
	if (q < 0.7) 
	{ 
		q = 0.7;
	}
	else if (q > 5.0)
	{
		q = 5.0;
	}
	// TODO: allow lower Q when frequency is lower
	setting_damp = (1.0 / q);
}

/**
*	@brief	Set filter Reaonance (Q) int
*	@param	res 0-100
*	@return void
*/
void DSP_Filter::set_resonance_relative(int res)
{
	set_resonance(_FILTER_MIN_Q + (_FILTER_MAX_Q - _FILTER_MIN_Q) * res / 100.0f);
}

/**
*	@brief	Set filter band
*	@param	bnd band _FILTER_BAND_LPF, _FILTER_BAND_HPF, _FILTER_BAND_BPF, _FILTER_BAND_PASS_ALL
*	@return void
*/
void DSP_Filter::set_band(int bnd)
{
	if ((bnd == _FILTER_BAND_LPF) || (bnd == _FILTER_BAND_HPF) ||
		(bnd == _FILTER_BAND_BPF) || (bnd == _FILTER_BAND_PASS_ALL))
	{
		filter_band = bnd;
	}
}

/**
*	@brief	Set keyboard frequency
*	@param	frq _FILTER_MIN_CENTER_FREQ to _FILTER_MAX_CENTER_FREQ (Hz)
*	@return void
*/
void DSP_Filter::set_kbd_freq(float frq)
{	
	float kbd_freq = frq * kbd_track;
	
	if (kbd_freq < _FILTER_MIN_CENTER_FREQ)
	{
		kbd_freq = _FILTER_MIN_CENTER_FREQ;
	}
	else if (kbd_freq > get_filter_max_center_frequency()) 
	{ 
		kbd_freq = get_filter_max_center_frequency();
	}
	
	setting_kbd_fcenter = kbd_freq * 3.141592654 / ((float)sample_rate * 2.0);
	setting_kbd_fmult = sinf(kbd_freq * (3.141592654 / ((float)sample_rate * 2.0)));			
}

void DSP_Filter::set_kbd_freq_relative(int frq) 
{
	float kbd_freq = _FILTER_MIN_CENTER_FREQ + (float)frq / 100.f * 
		(get_filter_max_center_frequency() - _FILTER_MIN_CENTER_FREQ);
	
	set_kbd_freq(kbd_freq);
}

/**
*	@brief	Set heyboard tracking
*	@param	kbdt 0.0 to 1.0
*	@return void
*/
void DSP_Filter::set_kbd_track(float kbdt)
{
	kbd_track = kbdt;
	
	if (kbd_track < 0)
	{
		kbd_track = 0;
	}
	else if (kbd_track > 1.0)
	{
		kbd_track = 1.0;
	}
}

/**
*	@brief	Set heyboard tracking (int)
*	@param	kbdt 0 to 100
*	@return void
*/
void DSP_Filter::set_kbd_track_relative(int kbdt)
{
	set_kbd_track((float)kbdt / 100.0f);
}

/**
*	@brief	Return next filter output sample
*	@param	input input sample
*	@param	fmod frequency modulation factor
*			0 to sinf((AUDIO_SAMPLE_RATE / 2.5) * (3.141592654 / (AUDIO_SAMPLE_RATE * 2.0)))
*	@return void
*/
float DSP_Filter::filter_output(float input, float fmod)
{
	int i;	
	float input_prev;
	float lowpass, bandpass, highpass;
	float lowpass_tmp, bandpass_tmp, highpass_tmp;
	float fmult, damp, modulate, out;

	if (filter_band == _FILTER_BAND_PASS_ALL)
	{
		return input;
	}

	modulate = setting_fmult * pow(2.0, (double)(setting_octave_mult * fmod)); ////
	
	fmult = modulate + setting_kbd_fmult;
	if (fmult > max_setting_fmult)
	{
		fmult = max_setting_fmult;
	}

	// TODO: fmult(mod)
	
	damp = setting_damp;
	input_prev = state_input_prev;
	lowpass = state_lowpass;
	bandpass = state_bandpass;
				
	lowpass = lowpass + fmult * bandpass;
	highpass = ((input + input_prev) / 2.0f) - lowpass - damp * bandpass;
	input_prev = input;
	bandpass = bandpass + fmult * highpass;
	lowpass_tmp = lowpass;
	bandpass_tmp = bandpass;
	highpass_tmp = highpass;
	lowpass = lowpass + fmult * bandpass;
	highpass = input - lowpass - damp * bandpass;
	bandpass = bandpass + fmult * highpass;
	lowpass_tmp = lowpass + lowpass_tmp;
	bandpass_tmp = bandpass + bandpass_tmp;
	highpass_tmp = highpass + highpass_tmp;
	
	switch (filter_band)
	{				
		case _FILTER_BAND_LPF:
			out = lowpass_tmp;
			break;
						
		case _FILTER_BAND_HPF:
			out = highpass_tmp;
			break;
						
		case _FILTER_BAND_BPF:
			out = bandpass_tmp;
			break;
	}
		
	state_input_prev = input_prev;
	state_lowpass = lowpass;
	state_bandpass = bandpass;
	
	return out;
}

/**
*	@brief	Sets fliter sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100)
*	@return set sample-rate
*/	
int DSP_Filter::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	max_setting_fcenter = (float(sample_rate) / 2.5 * (3.141592654 / (float(sample_rate) * 2.0)));
	max_setting_fmult = sinf((float(sample_rate) / 2.5) * (3.141592654 / (float(sample_rate) * 2.0)));
	
	return sample_rate;
}
	
/**
*	@brief	Returns fliter set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_Filter::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*	@brief	Returns fliter max center frequency based on sample-rate
*	@param	none
*	@return max center frequency
*/
float DSP_Filter::get_filter_max_center_frequency()
{
	return (float)sample_rate / 6.7f; // ????? /6?
}


