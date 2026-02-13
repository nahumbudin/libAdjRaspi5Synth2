/**
*	@file		dspOsc.cpp
*	@author		Nahum Budin
*	@date		27-Sep_2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					2. Removing dependencies on block-size
*					
*	@History	25-Jan-2021	1.1
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate settings
*				31-Oct-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Generates waveforms samples.
*/

#include <stdio.h>
#include <stdlib.h>
#include "dspOsc.h"

#include "dspOsc.h"
#include "dspSineWaveGenerator.h"

#include "../utils/utils.h"

/**
 *	@brief	Creates an Oscilator object instance
 *	@param	id			OSC unique Id number
 *	@param	form		waveform
 *	@param	pwmdc		pwm dutycycle
 *	@param	detOct		frequency detune - octaves
 *	@param	detStone	frequency detune - semitones
 *	@param	detCnts		frequency detune - cents
 *	@param	snc			sync enable
 *	@param	trk			tracking enable
 *	@param	fix			fix frequency enable
 *	@param	harmDetune	harmonies detune
 *	@param	harmDist	harmonies distortion
 *	@param	unisonMod	unison mode
 *	@param	mag			output magnitude (level)
 *	@param	samp_rate	sample rate
 *	@int	bloc_size	audio block size
 *	@return	none
*/
DSP_Osc::DSP_Osc(int iD,
	int form,
	int pwmDc,
	int detOct,
	int detStone,
	int detCnts, 
	bool snc,
	bool trk,
	bool fix,
	float harmDetune,
	float harmDist,
	int unisonMod,
	float mag,
	int samp_rate)
{
	int res;
	float resf;
	
	res = init(samp_rate);		
	if (res < 0)
	{
		fprintf(stderr, "DSP_Osc: illegal block size\n");
		abort();
	}
	
	sine_wave = new DSP_SineWaveGenerator(sample_rate);
	triangle_wave = new DSP_TriangleWaveGenerator(sample_rate, (float)pwmDc / 100.0f);
	square_wave = new DSP_SquareWaveGenerator(sample_rate, (float)pwmDc / 100.0f);
	sample_hold_wave = new DSP_SampleHoldWaveGenerator(sample_rate);
	
	id = iD;
	res = set_waveform(form);
	if (res < 0)
	{
		fprintf(stderr, "DSP_Osc: illegal waveform\n");
		abort();
	}
	
	set_pwm_dcycle_set_val(pwmDc);	
	set_pwm_dcycle(pwmDc);	
	set_freq_detune_oct(detOct);	
	set_freq_detune_semitones(detStone);	
	set_freq_detune_cents_set_value(detCnts); // settnigs is int	
	set_sync_state(snc);
	set_track_state(trk);	
	
	res = set_unison_mode(unisonMod);
	if (res < 0)
	{
		fprintf(stderr, "DSP_Osc: error setting unison mode\n");
		abort();
	}
	
	resf = set_harmonies_detune(harmDetune);
	if (resf < 0)
	{
		fprintf(stderr, "DSP_Osc: set harmonies detune error\n");
		abort();
	}
		
	resf = set_harmonies_distortion(harmDist);
	if (resf < 0)
	{
		fprintf(stderr, "DSP_Osc: set harmonies distortion error\n");
		abort();
	}	
	
	set_fix_tone_state(fix);
	set_magnitude(mag);
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_Osc::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}

	if (sine_wave)
	{
		sine_wave->set_sample_rate(sample_rate);
	}

	if (triangle_wave)
	{
		triangle_wave->set_sample_rate(sample_rate);
	}

	if (square_wave)
	{
		square_wave->set_sample_rate(sample_rate);
	}

	if (sample_hold_wave)
	{
		sample_hold_wave->set_sample_rate(sample_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_Osc::get_sample_rate() { return sample_rate; }

/**
*   @brief  Redefine sample rate and block size  
*   @param  none
*   @return audio block size if OK; -1 if params out of range
*/	
int DSP_Osc::init(int samp_rate)
{
	int res;
	
	set_sample_rate(samp_rate);

	if (sine_wave)
	{
		sine_wave->set_sample_rate(sample_rate);
	}

	if (square_wave)
	{
		square_wave->set_sample_rate(sample_rate);
	}

	if (triangle_wave)
	{
		triangle_wave->set_sample_rate(sample_rate);
	}

	if (sample_hold_wave)
	{
		sample_hold_wave->set_sample_rate(sample_rate);
	}

	return res;
}

/**
*	@brief	Return OSC id number
*	@param none
*	@return OSC id number
*/
int DSP_Osc::get_id() 
{ 
	return id; 
}

/**
*	@brief	Set OSC waveform
*	@param wform waveform
*	@return selected waveform if OK; -1 if param out of range
*/
int DSP_Osc::set_waveform(int wform)
{
	if ((wform >= _OSC_WAVEFORM_SINE) && (wform <= _OSC_WAVEFORM_SAMPHOLD))
	{
		waveform = wform;
		return waveform;
	}
	else
	{
		return -1;
	}
}

/**
*	@brief	Return OSC waveform
*	@param none
*	@return OSC waveform
*/
int DSP_Osc::getWaveform() 
{ 
	return waveform; 
}

/**
*	@brief	Set OSC PWM dutycycle settings only ?????
*	@param dc OSC PWM dutycycle
*	@return set DC
*/
int DSP_Osc::set_pwm_dcycle_set_val(int dc) 
{
	pwm_percents_set = dc;
	if (pwm_percents_set < 5)
	{
		pwm_percents_set = 5;
	}
	else if (pwm_percents_set > 95)
	{
		pwm_percents_set = 95;
	}
	
	return pwm_percents_set;
}

/**
*	@brief	Return OSC PWM dutycycle settings only
*	@param none
*	@return OSC PWM dutycycle settings
*/
int DSP_Osc::get_pwm_dcycle_set_val() { return pwm_percents_set; }

/**
*	@brief	Set OSC PWM dutycycle value 
*	@param dc OSC PWM dutycycle
*	@return OSC PWM dutycycle
*/
int DSP_Osc::set_pwm_dcycle(int dc)
{
	pwm_percents = dc;
	if (pwm_percents < 5) 
	{
		pwm_percents = 5;
	}
	else if (pwm_percents > 95) 
	{	
		pwm_percents = 95; 
	}
	
	if (square_wave)
	{
		square_wave->set_pwm((float)pwm_percents / 100.0f);
	}
	
	if (triangle_wave)
	{
		triangle_wave->set_asymetry((float)pwm_percents / 100.0f);
	}
	
	return pwm_percents;
}

/**
*	@brief	Return OSC PWM dutycycl
*	@param none
*	@return OSC PWM dutycycle
*/
int DSP_Osc::get_pwm_dcycle() 
{ 
	return pwm_percents; 
}

/**
*	@brief	Set OSC frequency detune octaves
*	@param oct OSC frequency detune octaves
*	@return set octave detune
*/
int DSP_Osc::set_freq_detune_oct(int oct)
{
	detune_octave = oct;
	
	if (detune_octave < _OSC_DETUNE_MIN_OCTAVE)
	{
		detune_octave < _OSC_DETUNE_MIN_OCTAVE;
	}
	else if (detune_octave > _OSC_DETUNE_MAX_OCTAVE)
	{
		detune_octave = _OSC_DETUNE_MAX_OCTAVE;
	}
	
	return detune_octave;
}

/**
*	@brief	Return OSC frequency detune octaves
*	@param none
*	@return OSC frequency detune octaves
*/
int DSP_Osc::get_freq_detune_oct() 
{ 
	return detune_octave; 
}

/**
*	@brief	Set OSC frequency detune semitones 
*	@param oct OSC frequency detune semitones 
*	@return set frequency detune semitones value
*/
int DSP_Osc::set_freq_detune_semitones(int sem)
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
	
	return detune_semitones;
}

/**
*	@brief	Return OSC frequency detune semitones
*	@param none
*	@return OSC frequency detune semi tones
*/
int DSP_Osc::get_freq_detune_semitones() 
{ 
	return detune_semitones; 
}

/**
*	@brief	Set OSC frequency detune cents (int)
*	@param oct OSC frequency detune cents 
*	@return frequency detune cents (int)
*/
int DSP_Osc::set_freq_detune_cents_set_value(int cnt)
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
	
	return detune_cents_set_value;
}

/**
*	@brief	Return OSC frequency detune cents settings only
*	@param none
*	@return OSC frequency detune octaves settings
*/
int DSP_Osc::get_freq_detune_cents_set_value() 
{ 
	return detune_cents_set_value; 
}

/**
*	@brief	Set OSC frequency detune cents (float)
*	@param cnt OSC frequency detune cents
*	@return frequency detune cents
*/
float DSP_Osc::set_freq_detune_cents(float cnt)
{
	detune_cents = cnt;
	
	if (detune_cents < _OSC_DETUNE_MIN_CENTS)
	{
		detune_cents = _OSC_DETUNE_MIN_CENTS;
	}
	else if (detune_cents > _OSC_DETUNE_MAX_CENTS)
	{
		detune_cents = _OSC_DETUNE_MAX_CENTS;
	}
	
	return detune_cents;
}

/**
*	@brief	Return OSC frequency detune cents 
*	@param none
*	@return OSC frequency detune cents
*/
float DSP_Osc::get_freq_detune_cents() 
{ 
	return detune_cents; 
}

/**
*	@brief	Set OSC sync state
*	@param snc true - sync on
*	@return void
*/
void DSP_Osc::set_sync_state(bool snc) 
{ 
	sync_is_on = snc; 
}

/**
*	@brief	Return OSC sync state
*	@param none
*	@return true - sync on
*/
bool DSP_Osc::get_sync_state() 
{ 
	return sync_is_on; 
}

/**
*	@brief	Set OSC tracking state
*	@param trk true - tracking on
*	@return void
*/
void DSP_Osc::set_track_state(bool trk) 
{ 
	track_is_on = trk; 
}

/**
*	@brief	Return OSC tracking state
*	@param none
*	@return true - sync on
*/
bool DSP_Osc::get_track_state()
{ 
	return track_is_on; 
}

/**
*	@brief	Return OSC next output value
*	@param	freq frequency (Hz)
*	@return next output value
*/
float DSP_Osc::get_next_output_val(float freq)
{
	float val = 0;
		
	switch (waveform) 
	{		
		case _OSC_WAVEFORM_SQUARE:					
		case _OSC_WAVEFORM_PULSE:				
			val = square_wave->get_next_square_gen_out_val(freq);
			break;
						
		case _OSC_WAVEFORM_TRIANGLE:					
			val = triangle_wave->get_next_triangle_gen_out_val(freq);
			break;
							
		case _OSC_WAVEFORM_SINE:					
			val = sine_wave->get_next_sine_wtab_val(freq);				
			break;
				
		case _OSC_WAVEFORM_SAMPHOLD:
			val = sample_hold_wave->get_next_sample_hold_gen_out_val(freq);
			break;
	}		
	
	return val * magnitude;
}

/**
*	@brief Force a Sync (on a higher pitch Osc) by reseting the phase of the Osc
*		(usually by a lower pitch Osc)
*	@param none
*	@return void
*/
void DSP_Osc::sync()
{
	switch (waveform) 
	{		
		case _OSC_WAVEFORM_SQUARE:					
		case _OSC_WAVEFORM_PULSE:				
			square_wave->sync();
			break;
							
		case _OSC_WAVEFORM_TRIANGLE:					
			triangle_wave->sync();
			break;
								
		case _OSC_WAVEFORM_SINE:					
			sine_wave->init_wtab_indexes();				
			break;
				
		case _OSC_WAVEFORM_SAMPHOLD:
			sample_hold_wave->sync();
			break;		
	}		
}

/**
*	@brief	Return OSC cycle restart sync state
*	@param none
*	@return true - cycle restarted
*/
bool DSP_Osc::get_cycle_restarted_sync_state()
{
	bool res = false;
	
	switch (waveform) 
	{		
		case _OSC_WAVEFORM_SQUARE:					
		case _OSC_WAVEFORM_PULSE:				
			res =  square_wave->get_cycle_restarted_sync_state();
			break;
								
		case _OSC_WAVEFORM_TRIANGLE:					
			res =  triangle_wave->get_cycle_restarted_sync_state();
			break;
									
		case _OSC_WAVEFORM_SINE:					
			res = sine_wave->get_cycle_restarted_sync_state();				
			break;	
				
		case _OSC_WAVEFORM_SAMPHOLD:
			sample_hold_wave->get_cycle_restarted_sync_state();
			break;
	}
	
	return res;
}

/**
*	@brief	Enable unison mode
*	@param none
*	@return void
*/
void DSP_Osc::enable_unison_square() 
{ 
	sine_wave->enable_unison_square(); 
}

/**
*	@brief	Disable unison mode
*	@param none
*	@return void
*/
void DSP_Osc::disable_unison_square() 
{ 
	sine_wave->disable_unison_square(); 
}

bool DSP_Osc::get_unison_square_state() 
{ 
	return sine_wave->get_unison_square_state(); 
}

/**
*	@brief	Set harmonies detune factor
*	@param det harmonies detune factor
*	@return set harmonies detune factor > 0; -1 if error
*/
float DSP_Osc::set_harmonies_detune(float det)
{
	harmonies_detune_factor = det;
	
	if (harmonies_detune_factor < _MIN_HARMONIES_DETUNE_FACTOR)
	{
		harmonies_detune_factor = _MIN_HARMONIES_DETUNE_FACTOR;
	}
	else if (harmonies_detune_factor > _MAX_HARMONIES_DETUNE_FACTOR)
	{
		harmonies_detune_factor = _MAX_HARMONIES_DETUNE_FACTOR;
	}
	
	if (sine_wave)
	{
		return sine_wave->set_harmonies_detune_factor(harmonies_detune_factor);
	}
	else
	{
		return -1;
	}
}

/**
*	@brief	Set harmonies detune factor (int)
*	@param det harmonies detune factor (int)
*	@return set harmonies detune factor (float) > 0; -1 if error
*/
float DSP_Osc::set_harmonies_detune(int det)
{
	return set_harmonies_detune((float)_MIN_HARMONIES_DETUNE_FACTOR + 
		(float)(_MAX_HARMONIES_DETUNE_FACTOR - _MIN_HARMONIES_DETUNE_FACTOR) *
		(float)det / 100.0f);
}

/**
*	@brief	Return harmonies detune factor
*	@param none
*	@return harmonies detune factor
*/
float DSP_Osc::get_harmonies_detune() 
{ 
	return harmonies_detune_factor; 
}

/**
*	@brief	Set harmonies distortion factor
*	@param det harmonies distortion factor
*	@return set harmonies distortion factor (float) > 0; -1 if error
*/
float DSP_Osc::set_harmonies_distortion(float dis)
{
	harmonies_dist_factor = dis;
	
	if (harmonies_dist_factor < _MIN_HARMONIES_DISTORTION_FACTOR)
	{
		harmonies_dist_factor = _MIN_HARMONIES_DISTORTION_FACTOR;
	}
	else if (harmonies_dist_factor > _MAX_HARMONIES_DISTORTION_FACTOR)
	{
		harmonies_dist_factor = _MAX_HARMONIES_DISTORTION_FACTOR;
	}
	
	if (sine_wave)
	{
		return sine_wave->set_harmoies_distortion_factor(harmonies_dist_factor);
	}
	else
	{
		return -1;
	}
}

/**
*	@brief	Set harmonies distortion factor (int)
*	@param det harmonies distortion factor (int)
*	@return set harmonies distortion factor (float) > 0; -1 if error
*/
float DSP_Osc::set_harmonies_distortion(int dis)
{
	return set_harmonies_distortion((float)_MIN_HARMONIES_DISTORTION_FACTOR + 
				(float)(_MAX_HARMONIES_DISTORTION_FACTOR - _MIN_HARMONIES_DISTORTION_FACTOR) *
				(float)dis / 100.0f);
}

/**
*	@brief	Return harmonies distortion factor
*	@param none
*	@return harmonies distortion factor
*/
float DSP_Osc::get_harmonies_distortion() 
{ 
	return harmonies_dist_factor; 
}

/**
*	@brief	Set unison mode
*	@param mod unison mode
*	@return unison mode if OK; -1 if error
*/
int DSP_Osc::set_unison_mode(int mod)
{
	unison_mode = mod;
	
	if (unison_mode < _OSC_UNISON_MODE_12345678)
	{
		unison_mode = _OSC_UNISON_MODE_12345678;
	}
	else if (unison_mode > _OSC_UNISON_MODE_Cm7_CHORD)
	{
		unison_mode = _OSC_UNISON_MODE_Cm7_CHORD;
	}
	
	sine_wave->set_unison_mode(unison_mode);
	
	if (sine_wave)
	{
		switch (unison_mode)
		{
			case _OSC_UNISON_MODE_12345678:
				// Set active harmonies bit mask
				sine_wave->set_harominies_active(0x00ff);
				break;

			case _OSC_UNISON_MODE_HAMMOND:
				sine_wave->set_harominies_active(0x01ff);
				break;

			case _OSC_UNISON_MODE_OCTAVES:
				sine_wave->set_harominies_active(0x008b);
				break;
					
			case _OSC_UNISON_MODE_C_CHORD:
				sine_wave->set_harominies_active(0x0007);
				break;
						
			case _OSC_UNISON_MODE_Cm_CHORD:
				sine_wave->set_harominies_active(0x0007);
				break;
					
			case _OSC_UNISON_MODE_C7_CHORD:
				sine_wave->set_harominies_active(0x000f);
				break;
					
			case _OSC_UNISON_MODE_Cm7_CHORD:
				sine_wave->set_harominies_active(0x000f);
				break;			
		}
		
		return unison_mode;
	}
	else
	{
		return -1;
	}	
}

/**
*	@brief	Return unison mode
*	@param none
*	@return unison mode
*/
int DSP_Osc::get_unison_mode() 
{ 
	return unison_mode; 
}

/**
*	@brief	Set harmony level
*	@param harm harmony number
*	@param lev harmony level
*	@return set harmony level (float 0-1.0) if OK; -1 if parameters out of range
*/
float DSP_Osc::set_harmony_level(int harm, int lev)
{
	if (harm >= _NUM_OF_HARMONIES)
	{
		return -1;
	}
	
	if (lev < 0)
	{
		lev = 0;
	}
	else if (lev > 100)
	{
		lev = 100;
	}
	
	if (sine_wave)
	{
		return sine_wave->set_harmony_level(harm, (float)lev / 100.0);
	}
	else
	{
		return -1;
	}
}

/**
*	@brief	Return harmony level
*	@param harm harmony number
*	@param lev harmony level
*	@return set harmony level 0-100 if OK; -1 if parameters out of range
*/
int DSP_Osc::get_harmony_level(int harm)
{	
	if (harm >= _NUM_OF_HARMONIES)
	{
		return -1;
		;
	}
	
	if (sine_wave)
	{
		return (int)(sine_wave->get_harmony_level(harm) * 100);
	}
	else
	{
		return -1;
	}
}

/**
*	@brief	Set OSC magnitude (volume)
*	@param mag OSC magnitude (volume)
*	@return void
*/
float DSP_Osc::set_magnitude(float mag)
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
	
	return magnitude;
}

/**
*	@brief	return OSC magnitude (volume)
*	@param none
*	@return OSC magnitude (volume)
*/
float DSP_Osc::get_magnitde() 
{ 
	return magnitude; 
}

/**
*	@brief	Set OSC fix tone state
*	@param fix OSC fix tone state
*	@return void
*/
void DSP_Osc::set_fix_tone_state(bool fix) 
{ 
	fix_tone_is_on = fix; 
}

/**
*	@brief	Return OSC fix tone state
*	@param none
*	@return OSC fix tone state true - enabled
*/
bool DSP_Osc::get_fix_tone_state() 
{ 
	return fix_tone_is_on; 
}

/**
*	@brief	Set OSC send level to channel 1
*	@param snd OSC send level to channel 1
*	@return send level 1
*/
float DSP_Osc::set_send_level_1(float snd)
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
*	@brief	Set OSC send level to channel 1 (int)
*	@param snd OSC send level to channel 1 (int)
*	@return send level (int)
*/
int DSP_Osc::set_send_level_1(int snd)
{
	float send = (float)snd / 100.0f;

	set_send_level_1(send);

	return snd;
}

/**
*	@brief	Set OSC send level to channel 2
*	@param snd OSC send level to channel 2
*	@return send level 2
*/
float DSP_Osc::set_send_level_2(float snd)
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
*	@brief	Set OSC send level to channel 2 (int)
*	@param snd OSC send level to channel 2 (int)
*	@return send level 2 (int)
*/
int DSP_Osc::set_send_level_2(int snd)
{
	float send = (float)snd / 100.0f;

	set_send_level_2(send);
	
	return send;
}

/**
*	@brief	return OSC send level to channel 1
*	@param none
*	@return snd OSC send level to channel 1
*/
float DSP_Osc::get_send_level_1() 
{ 
	return send_level_1; 
}

/**
*	@brief	return OSC send level to channel 2
*	@param none
*	@return snd OSC send level to channel 2
*/
float DSP_Osc::get_send_level_2() 
{ 
	return send_level_2; 
}

