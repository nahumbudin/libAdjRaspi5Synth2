/**
*	@file		dspSineWaveGenerator.h
*	@author		Nahum Budin
*	@date		13-Sep2024
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Change wavefor table length to _SINE_WAVETABLE_SIZE (2048)
*					    and not based on the audio-block length.
*					
*	@History	24_Jan-2021	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate settings
*				31-Oct-2019 (dspWaveformLUT) (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Sine Wave Oscilator.
*/

#include "dspSineWaveGenerator.h"
#include "../utils/utils.h"
#include "../LibAPI/synthesizer.h"

DSP_SineWaveGenerator::DSP_SineWaveGenerator(int samp_rate)
{
	int i;
	
	harmonies_dist_factor = _MIN_HARMONIES_DISTORTION_FACTOR;
	harmonies_detune_factor = _MIN_HARMONIES_DETUNE_FACTOR;
	
	min_frequency = 0.1f;
	
	set_sine_wavetable_size(_SINE_WAVETABLE_SIZE);	
	init_sine_waveform_table(samp_rate);
	
	for (i = 0; i < _NUM_OF_HARMONIES; i++)
	{
		harmonies_active[i] = true;
		harmonies_levels[i] = 0.0f;	
	}
	
	harmonies_levels[0] = 1.0f;
	
	unison_mode = _OSC_UNISON_MODE_12345678;
	
	square = false;
	
	set_harmonizig_frequencies(unison_mode, harmonies_detune_factor);	
}

/**
*	@brief	Sets the sample-rate
*	@param	sample rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_SineWaveGenerator::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	/* Audio blocks rate. */
	fundemental_frequency = (float)sample_rate / (float)sine_wavetable_size;
	max_frequency = (float)sample_rate / 2.1f;
	
	return sample_rate;
}

/**
*	@brief	Returns set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_SineWaveGenerator::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the sine wavetable size and the table phase step based on the table size  
*   @param  int size: sine wavetable size min 512, max 2048 (defualt) must be  512, 1024 or 2048
*   @return set block size OK; -1 param out of range
*/
int DSP_SineWaveGenerator::set_sine_wavetable_size(int size)
{
	if ((size == 512) || (size == 1024) || (size == 2048))
	{
		sine_wavetable_size = size;
		/* Waveform Table phase step size */
		wtab_phase_step = (2.0f * M_PI / (float)sine_wavetable_size);
		return sine_wavetable_size;
	}
	else
	{
		return -1;
	}
}
	
/**
*   @brief  retruns the sine wavetable size  
*   @param  none
*   @return buffer size
*/	
int DSP_SineWaveGenerator::get_sine_wavetable_size() {	return sine_wavetable_size; }

/**
*   @brief  initialize sample rate, sine wavetable and wavetable indexes (for harmonies)  
*   @param  sample rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*			if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*   @return buffer size
*/	
void DSP_SineWaveGenerator::init_sine_waveform_table(int samp_rate)
{
	int i;
	
	// set sample rate, fundemental and max frequencies
	set_sample_rate(samp_rate);
	
	// Build a Sine waveform table 
	for (i = 0; i < sine_wavetable_size; ++i) 
	{
		sine_wtab[i] = (float)sin(i * wtab_phase_step);	
	}
	
	init_wtab_indexes();
}

/**
*	@brief	Set unisonic mode (harmonies frequencies)
*	@param mod unisonic mode _OSC_UNISON_MODE_12345678 to _OSC_UNISON_MODE_Cm7_CHORD 
*		(see libAPI oscilator.h)
*	@return unison mode if OK; -1 if param out of range
*/
int DSP_SineWaveGenerator::set_unison_mode(int mod)
{
	if ((mod >= _OSC_UNISON_MODE_12345678) && (mod <= _OSC_UNISON_MODE_Cm7_CHORD))
	{
		unison_mode = mod;
		set_harmonizig_frequencies(mod, harmonies_detune_factor);
		return unison_mode;
	}
	else
	{
		return -1;
	}
}

/**
*	@brief	Set harmonies distortion factor
*	@param  harmonies distortion factor 
*			MIN_HARMONIES_DISTORTION_FACTOR - MAX_HARMONIES_DISTORTION_FACTOR
*	@return distortion factor
*/
float DSP_SineWaveGenerator::set_harmoies_distortion_factor(float dist)
{
	harmonies_dist_factor = dist;
	
	if (harmonies_dist_factor > _MAX_HARMONIES_DISTORTION_FACTOR) 
	{
		harmonies_dist_factor = _MAX_HARMONIES_DISTORTION_FACTOR;
	}
	else if (harmonies_dist_factor < _MIN_HARMONIES_DISTORTION_FACTOR) 
	{
		harmonies_dist_factor = _MIN_HARMONIES_DISTORTION_FACTOR;
	}
	
	return harmonies_dist_factor;
}

/**
*	@brief	Return harmonies distortion factor
*	@param non
*	@return harmonies distortion factor
*/
float DSP_SineWaveGenerator::get_harmoies_distortion_factor() { return harmonies_dist_factor; }

/**
*	@brief	Set harmonies detune factor
*	@param harm harmonies distortion factor
*			MIN_HARMONIES_DETUNE_FACTOR - MAX_HARMONIES_DETUNE_FACTOR
*	@return void
*/
float DSP_SineWaveGenerator::set_harmonies_detune_factor(float harm)
{
	harmonies_detune_factor = harm;
	
	if (harmonies_detune_factor > _MAX_HARMONIES_DETUNE_FACTOR) 
	{
		harmonies_detune_factor = _MAX_HARMONIES_DETUNE_FACTOR;
	}
	else if (harmonies_detune_factor < _MIN_HARMONIES_DETUNE_FACTOR) 
	{
		harmonies_detune_factor = _MIN_HARMONIES_DETUNE_FACTOR;
	}
	
	set_harmonizig_frequencies(unison_mode, harmonies_detune_factor);
	
	return harmonies_detune_factor;
}

/**
*	@brief	Return harmonies detune factor
*	@param non
*	@return harmonies detune factor
*/
float DSP_SineWaveGenerator::get_harmonies_detune_factor() { return harmonies_detune_factor; }

/**
*	@brief	Set harmonies levels
*	@param har harmony number 0 - NUM_OF_HARMONIES
*	@param lev harmony level 0-1.0
*	@return harmony level if OK; -1 if harmony param is out of range
*/
float DSP_SineWaveGenerator::set_harmony_level(int har, float lev)
{
	if ((har < 0) || (har > _NUM_OF_HARMONIES)) 
	{
		return -1;
	}
	
	harmonies_levels[har] = lev;
	
	if (harmonies_levels[har] < 0.0f) 
	{
		harmonies_levels[har] = 0.0f;
	}
	else if (harmonies_levels[har] > 1.0f) 
	{
		harmonies_levels[har] = 1.0f;
	}
	
	return harmonies_levels[har];
}

/**
*	@brief	Return an harmony level
*	@param har harmony number 0 - NUM_OF_HARMONIES
*	@return harmony level 0-1.0; -1 if harmony param is out of range
*/
float DSP_SineWaveGenerator::get_harmony_level(int har)
{
	if ((har < 0) || (har > _NUM_OF_HARMONIES)) 
	{
		return -1;
	}
	else 
	{
		return harmonies_levels[har];	
	}
}

/**
*	@brief	Return the cycle restarted sync state
*	@param none
*	@return true when cycle restarted false otherwise
*/
bool DSP_SineWaveGenerator::get_fund_harm_cycle_restarted_state() { return cycle_restarted; }


/** 
 *	@brief	Get next value from sine wave table
 *	@param  (float) Frequency (Hz)
 *	@return float Sine value next value
*/	 
float DSP_SineWaveGenerator::get_next_sine_wtab_val(float freq)
{
	float value = get_sine_wtab_val(&fl_harmonies_wtab_indexes[0], harmonies_dist_factor);
	// update sine wavetable indexes 
	calc_sin_wtab_index(&fl_harmonies_wtab_indexes[0], harmonies_dist_factor, freq);
	return value;
}

/**
 *	@brief	Get a value from the Sine LUT including harmonies; applly distortion 
 *	@param	hrmoniesIndexes		a pointer to an array to harmonies indexes into LUT
 *	@param	distort				distortion level
 *	@return	next value from the Sine LUT including harmonies including appllied distortion 
*/
float DSP_SineWaveGenerator::get_sine_wtab_val(float *hrmonies_indexes, float distort)
{
	float value = 0;
	float normalize = 0;
	int harmony;
			
	for (harmony = 0; harmony < _NUM_OF_HARMONIES; harmony++) 
	{
		if ((harmonies_levels[harmony] > 0) && harmonies_active[harmony]) 
		{
			// Only enabled-harmonies or non-zero level harmonies
			int index = (int)hrmonies_indexes[harmony];
			if (distort > 0) 
			{
				index = (int)(distort * index + index * index * (1.f - distort) / sine_wavetable_size);
				// Wrap index 
				while (index < 0)
				{
					index += sine_wavetable_size;
				}
				while (index >= sine_wavetable_size) 
				{
					index -= sine_wavetable_size;
				}
			}
			// Get value
			if (square)
			{
				// Generate a square wave instead of a sine wave
				if (sine_wtab[index] > 0)
				{
					value += harmonies_levels[harmony];
				}
				else
				{
					value -= harmonies_levels[harmony];
				}
			}
			else
			{
				// Add each harmony 
				value += sine_wtab[index] * harmonies_levels[harmony];
			}

			/* Look for fundemental harmony upward zero crossing */
			if (harmony == 0)
			{
				if ((value > 0.0f) && (prev_sin_value <= 0.0f))
				{
					// Mark cycle-started sync for fundemental frequency up rising zero crossing
					cycle_restarted = true;
					//	printf("val %f  prev %f\n", value, prevSinValue);
				}
				else
				{
					cycle_restarted = false;
				}
				prev_sin_value = value;
			}
			// Sum all harmonies levels for normalization
			normalize += harmonies_levels[harmony];
		}
	}
	// Normalize output 
	if (normalize == 0) {
		
		normalize = 1.0f;
	}
	
	value /= normalize;
	
	return value;
}

/**
 *	@brief	Calculate new wavetable get-value index for all active and non zero amplitude harmonies
 *	@param hrmoniesIndexes		a pointer for an arry of each harmony index into LUT
 *	@param detune				harmonization detune factor
 *	@param freq					Fundamental frequency (Hz)
 *	@rturn void
*/
void DSP_SineWaveGenerator::calc_sin_wtab_index(float *hrmonies_indexes, float detune, float freq)
{
	float index;

	for (int harmony = 0; harmony < _NUM_OF_HARMONIES; harmony++) 
	{ 
		if ((harmonies_levels[harmony] > 0) && harmonies_active[harmony])
		{
			index = hrmonies_indexes[harmony];
			index += freq * harmonizing_frequencies[harmony] / fundemental_frequency;
			while ((int)index >= sine_wavetable_size) 
			{
				index -= sine_wavetable_size;
			}
			
			hrmonies_indexes[harmony] = index;
		}
	}
}

/**
*	@brief	Set harmonies frequncies based on unison mode and detuning factor 
*	@param mode	unison mode
*	@param harmonFact harmonization detune factor
*	@return void
*/
void DSP_SineWaveGenerator::set_harmonizig_frequencies(int mode, float harmon_fact)
{	
	switch (mode)
	{
	case _OSC_UNISON_MODE_12345678:
	case _OSC_UNISON_MODE_OCTAVES:
		harmonizing_frequencies[0] = 1.0f;
		harmonizing_frequencies[1] = 2.0f + harmon_fact;
		harmonizing_frequencies[2] = 3.0f - harmon_fact;
		harmonizing_frequencies[3] = 4.0f + (harmon_fact / 2.0f);
		harmonizing_frequencies[4] = 5.0f - (harmon_fact / 2.0f);
		harmonizing_frequencies[5] = 6.0f + (harmon_fact / 3.0f);
		harmonizing_frequencies[6] = 7.0f - (harmon_fact / 3.0f);
		harmonizing_frequencies[7] = 8.0f + (harmon_fact / 4.0f);
		harmonizing_frequencies[8] = 9.0f - (harmon_fact / 4.0f);
		harmonizing_frequencies[9] = 10.0f + (harmon_fact / 5.0f);
		harmonizing_frequencies[10] = 11.0f - (harmon_fact / 5.0f);
		harmonizing_frequencies[11] = 12.0f + (harmon_fact / 5.0f);
		harmonizing_frequencies[12] = 13.0f - (harmon_fact / 5.0f);
		break;

	case _OSC_UNISON_MODE_HAMMOND:
		harmonizing_frequencies[0] = 0.5f;
		harmonizing_frequencies[1] = pow(2.0, 7.0 / 12.0) + harmon_fact; // 1.5
		harmonizing_frequencies[2] = 1.0f - harmon_fact;
		harmonizing_frequencies[3] = 2.0f + (harmon_fact / 2.0f);
		harmonizing_frequencies[4] = pow(2.0, (12.0 + 7.0) / 12.0) - (harmon_fact / 2.0f); // 3
		harmonizing_frequencies[5] = 4.0f + (harmon_fact / 3.0f);
		harmonizing_frequencies[6] = pow(2.0, (24.0 + 4.0) / 12.0) - (harmon_fact / 3.0f); // 5
		harmonizing_frequencies[7] = pow(2.0, (24.0 + 7.0) / 12.0) + (harmon_fact / 4.0f); // 6
		harmonizing_frequencies[8] = 8.0f - (harmon_fact / 4.0f);
		break;
			
	case _OSC_UNISON_MODE_C_CHORD:
		harmonizing_frequencies[0] = 1.0f;
		harmonizing_frequencies[1] = pow(2.0, 4.0 / 12.0) + harmon_fact;
		harmonizing_frequencies[2] = pow(2.0, 7.0 / 12.0) - harmon_fact;
		break;
			
	case _OSC_UNISON_MODE_Cm_CHORD:
		harmonizing_frequencies[0] = 1.0f;
		harmonizing_frequencies[1] = pow(2.0, 3.0 / 12.0) + harmon_fact;
		harmonizing_frequencies[2] = pow(2.0, 7.0 / 12.0) - harmon_fact;
		break;
			
	case _OSC_UNISON_MODE_C7_CHORD:
		harmonizing_frequencies[0] = 1.0f;
		harmonizing_frequencies[1] = pow(2.0, 4.0 / 12.0) + harmon_fact;
		harmonizing_frequencies[2] = pow(2.0, 7.0 / 12.0) - harmon_fact;
		harmonizing_frequencies[3] = pow(2.0, 10.0 / 12.0) - harmon_fact;
		break;
			
	case _OSC_UNISON_MODE_Cm7_CHORD:
		harmonizing_frequencies[0] = 1.0f;
		harmonizing_frequencies[1] = pow(2.0, 3.0 / 12.0) + harmon_fact;
		harmonizing_frequencies[2] = pow(2.0, 7.0 / 12.0) - harmon_fact;
		harmonizing_frequencies[3] = pow(2.0, 10.0 / 12.0) - harmon_fact;
		break;
	}
}

/**
 *	@brief	Set harmonies active state
 *	@param mask a bit map mask b0 - harmony 0, b1 - harmony 1....
 *			bit = 1 - enable
 *	@return	none
*/
void DSP_SineWaveGenerator::set_harominies_active(uint16_t mask)
{
	for (int i = 0; i < _NUM_OF_HARMONIES; i++)
	{
		if ((mask & (1 << i)) != 0)
		{
			harmonies_active[i] = true;
		}
		else
		{
			harmonies_active[i] = false;
		}
	}	
}

/**
 *	@brief	Get harmonies active state
 *	@param active  a pointer to a bit map mask b0 - harmony 0, b1 - harmony 1....
 *			bit = 1 - enable
 *	@return	void
*/
void DSP_SineWaveGenerator::get_harominies_active(bool *active) { active = &harmonies_active[0]; }

void DSP_SineWaveGenerator::init_wtab_indexes()
{
	for (int i = 0; i < _NUM_OF_HARMONIES; ++i) 
	{		
		fl_harmonies_wtab_indexes[i] = 0;			
	}

	cycle_restarted = false;
}

/**
*	@brief	Return the cycle restarted sync state
*	@param none
*	@return true when cycle restarted false otherwise
*/
bool DSP_SineWaveGenerator::get_cycle_restarted_sync_state() { return cycle_restarted; }

/**
*	@brief	Enable unison square wave state
*	@param  none
*	@return	void
*/
void DSP_SineWaveGenerator::enable_unison_square() { square = true; }

/**
*	@brief	Disable unison square wave state
*	@param  none
*	@return	void
*/
void DSP_SineWaveGenerator::disable_unison_square() { square = false; }

/**
*	@brief	Returnse unison square wave state
*	@param  none
*	@return	unison square wave state
*/
bool DSP_SineWaveGenerator::get_unison_square_state() { return square; }

