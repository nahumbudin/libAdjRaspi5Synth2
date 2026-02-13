/**
 *	@file		dspMorphedSineOsc.h
 *	@author		Nahum Budin
 *	@date		11-Oct-2025
 *	@version	1.2
 *					1. Removing singleton.
 *					2. Adding set_wavetable() function to enable replacing with program wtab
 *
 *	@History
 *				version 1.1 23-Jan-2021
 *					1. Code refactoring and notaion.
 *					2. Adding sample-rate settings
 *				version 1.0 31-Oct-2019	(dspWaveformLUT) (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
 *
 *	@brief		An oscilator that uses waveform table to generate segmented sinusudial modulated waveforms.
 */

#include "dspMorphedSineOsc.h"
#include "../Audio/audioCommons.h"
#include "../utils/utils.h"
#include "../LibAPI/synthesizer.h"

// DSP_MorphingSinusOscWTAB *DSP_MorphingSinusOscWTAB::dsp_morphing_sinus_osc_wtab = NULL;

DSP_MorphingSinusOscWTAB::DSP_MorphingSinusOscWTAB(int samp_rate)
{
	waveform_tab_len = _AUDIO_MAX_BUF_SIZE;
	min_segment_length = 1;	
	wave_form_min_frequency = 0.1f;
	
	base_waveform_tab = NULL;
	morphed_waveform_tab = NULL;
	
	init_waveform_tables(samp_rate);
}

DSP_MorphingSinusOscWTAB::~DSP_MorphingSinusOscWTAB()
{
	if (base_waveform_tab != NULL) 
	{
		delete base_waveform_tab;
	}
	
	if (morphed_waveform_tab != NULL) 
	{
		delete morphed_waveform_tab;
	}
}

/**
*	@brief	Return a singlton WTAB instance
*	@param	none
*	@return a singlton WTAB instance
*/

/*
DSP_MorphingSinusOscWTAB *DSP_MorphingSinusOscWTAB::get_morphing_sinus_osc_wtab_instance(int samp_rate)
{
	//TODO: different sample rate.
	if (dsp_morphing_sinus_osc_wtab == NULL)
	{
		dsp_morphing_sinus_osc_wtab = new DSP_MorphingSinusOscWTAB(samp_rate);
	}

	return dsp_morphing_sinus_osc_wtab;
}
*/

/**
*	@brief	Sets WTAB sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_MorphingSinusOscWTAB::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	waveform_fundemental_frequency = (float)sample_rate / (float)waveform_tab_len;
	waveform_max_frequency = (float)sample_rate / 2.1f;
	
	return sample_rate;
}

/**
*	@brief	Returns WTAB set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_MorphingSinusOscWTAB::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*	@brief	Returns WTAB fundemental frequency
*	@param	none
*	@return set sample-rate
*/	
float DSP_MorphingSinusOscWTAB::get_fundemental_frequency() 
{ 
	return waveform_fundemental_frequency; 
}

/**
*	@brief	Returns WTAB length
*	@param	none
*	@return WTAB length
*/	
int DSP_MorphingSinusOscWTAB::get_wavetable_length()
{
	return waveform_tab_len;
}

/**
*	@brief	Initialize the waveform tables
*	@param	none
*	@return 0
*/
int DSP_MorphingSinusOscWTAB::init_waveform_tables(int samp_rate)
{
	// set samplerate, fundemental and max frequencies
	set_sample_rate(samp_rate);
	
	base_segment_positions = _st_positions_sine;
	morphed_segment_positions = base_segment_positions;
	morphing_symetry = 70;

	if (base_waveform_tab != NULL)
	{
		delete[] base_waveform_tab;
	}	
	
	base_waveform_tab = new DSP_WaveformTab(waveform_tab_len);
	
	if (morphed_waveform_tab != NULL)
	{
		delete[] morphed_waveform_tab;
	}
	
	morphed_waveform_tab = new DSP_WaveformTab(waveform_tab_len);
	
	return 0;
}

/**
*	@brief	Set a segment position
*	@param	pos (enPositions) which position
*	@param	value position value 1 to _MSO_WAVEFORM_LENGTH
*	@param	positions a pointer to a stPositions structure
*	@return position if done -1 otherwise
*/
int DSP_MorphingSinusOscWTAB::set_segment_position(en_positions pos, int value, st_positions *positions)
{
	if ((value < 1) || (value >= waveform_tab_len - 1) || (positions == nullptr))
	{
		return -1;
	}
	else
	{
		switch (pos)
		{
			case pos_a:
				if ((value > 0) && (value < positions->b_position))
				{
					positions->a_position = value;
				}
				return positions->a_position;

			case pos_b:
				if ((value > positions->a_position) && (value < positions->c_position))
				{
					positions->b_position = value;
				}
				return positions->b_position;

			case pos_c:
				if ((value > positions->b_position) && (value < positions->d_position))
				{
					positions->c_position = value;
				}
				return positions->c_position;

			case pos_d:
				if ((value > positions->c_position) && (value < positions->e_position))
				{
					positions->d_position = value;
				}
				return positions->d_position;

			case pos_e:
				if ((value > positions->d_position) && (value < positions->f_position))
				{
					positions->e_position = value;
				}
				return positions->e_position;

			case pos_f:
				if ((value > positions->e_position) && (value < waveform_tab_len - 1))
				{
					positions->f_position = value;
				}
				return positions->f_position;

			default:
				return -1;
		}
	}
}

/**
*	@brief	Return segment length
*	@param	start start of segment
*	@param	stop end of segment
*	@return segment length if OK -1 otherwise
*/
int DSP_MorphingSinusOscWTAB::calc_segment_length(int start, int stop)
{
	if ((start < 0) || (stop > waveform_tab_len - 1) || (start >= stop))
	{
		// Error
		return -1;
	}
	else
	{
		return stop - start;
	}
}

/**
*	@brief	Return a segment value
*	@param	index value LUT index position
*	@param	seg_start start segment
*	@param	seg_len	segment length
*	@param	quadrature sine qudrature (enSinQuad: quad_1...4)
*	@return value if done -2 otherwise
*/
float DSP_MorphingSinusOscWTAB::get_sin_segment_value(int index, int seg_start, int seg_len, 
														en_sin_quad quadrature)
{
	float value;
	
	if (seg_len < 1)
	{
		value = -2;
	}
	else
	{
		value =  sin(((float)(index - seg_start) / (float)seg_len) * M_PI_2 + 
					(float)((int)quadrature * M_PI_2));
	}
	
	return value;
}

/**
*	@brief	Set morphing symetry
*	@param	symp symetry 0-100 (70 -> full symetry)
*	@return void
*/
void DSP_MorphingSinusOscWTAB::set_morphing_symetry(int symp)
{
	float scale;

	morphing_symetry = symp;
	if (morphing_symetry < 0)
	{
		morphing_symetry = 0;
	}
	else if (morphing_symetry > 100)
	{
		morphing_symetry = 100;
	}

	scale = 0.3 + (float)morphing_symetry / 100.f;

	morphed_segment_positions.a_position = (int)((float)base_segment_positions.a_position * scale);
	morphed_segment_positions.b_position = (int)((float)base_segment_positions.b_position * scale);
	morphed_segment_positions.c_position = (int)((float)base_segment_positions.c_position * scale);
	morphed_segment_positions.d_position = (int)((float)base_segment_positions.d_position * scale);
	morphed_segment_positions.e_position = (int)((float)base_segment_positions.e_position * scale);
	morphed_segment_positions.f_position = (int)((float)base_segment_positions.f_position * scale);

	if (morphed_segment_positions.b_position <= morphed_segment_positions.a_position)
	{
		morphed_segment_positions.b_position = morphed_segment_positions.a_position + 2;
	}
	
	if (morphed_segment_positions.b_position >= waveform_tab_len - 10)
	{
		morphed_segment_positions.b_position = waveform_tab_len - 10;
	}
	else if (morphed_segment_positions.b_position < 4)
	{
		morphed_segment_positions.b_position = 4;
	}

	if (morphed_segment_positions.c_position <= morphed_segment_positions.b_position)
	{
		morphed_segment_positions.c_position = morphed_segment_positions.b_position + 2;
	}

	if (morphed_segment_positions.c_position >= waveform_tab_len - 8)
	{
		morphed_segment_positions.c_position = waveform_tab_len - 8;
	}
	else if (morphed_segment_positions.c_position < 6)
	{
		morphed_segment_positions.c_position = 6;
	}

	if (morphed_segment_positions.d_position <= morphed_segment_positions.c_position)
	{
		morphed_segment_positions.d_position = morphed_segment_positions.c_position + 2;
	}
	
	if (morphed_segment_positions.d_position >= waveform_tab_len - 6)
	{
		morphed_segment_positions.d_position = waveform_tab_len - 6;
	}
	else if (morphed_segment_positions.d_position < 8)
	{
		morphed_segment_positions.d_position = 8;
	}

	if (morphed_segment_positions.e_position <= morphed_segment_positions.d_position)
	{
		morphed_segment_positions.e_position = morphed_segment_positions.d_position + 2;
	}
	
	if (morphed_segment_positions.e_position >= waveform_tab_len - 4)
	{
		morphed_segment_positions.e_position = waveform_tab_len - 4;
	}
	else if (morphed_segment_positions.e_position < 10)
	{
		morphed_segment_positions.e_position = 10;
	}

	if (morphed_segment_positions.f_position <= morphed_segment_positions.e_position)
	{
		morphed_segment_positions.f_position = morphed_segment_positions.e_position + 2;
	}
	
	if (morphed_segment_positions.f_position >= waveform_tab_len - 2)
	{
		morphed_segment_positions.f_position = waveform_tab_len - 2;
	}
	else if (morphed_segment_positions.f_position < 12)
	{
		morphed_segment_positions.f_position = 12;
	}

	calc_segments_lengths(&morphed_segment_lengths, &morphed_segment_positions);
}

/**
*	@brief	Return morphing symetry
*	@param	none
*	@return symetry 0-100 (70 -> full symetry)
*/
int DSP_MorphingSinusOscWTAB::get_morphing_symetry() 
{ 
	return morphing_symetry; 
}

/**
*	@brief	Calculate all segments lengths
*	@param	seglens a pointer to a stSegmentsLength struct that hols segments lengths
*	@param	positions a pointer to a stPositions struct that holds segment positions
*	@return void
*/
void DSP_MorphingSinusOscWTAB::calc_segments_lengths(st_segments_length *seg_lens, st_positions *positions)
{
	int len;

	len = calc_segment_length(0, positions->a_position);
	if (len > 0)
	{
		seg_lens->seg_0a_length = len;
	}

	len = calc_segment_length(positions->a_position, positions->b_position);
	if (len > 0)
	{
		seg_lens->seg_ab_length = len;
	}

	len = calc_segment_length(positions->b_position, positions->c_position);
	if (len > 0)
	{
		seg_lens->seg_bc_length = len;
	}

	len = calc_segment_length(positions->c_position, positions->d_position);
	if (len > 0)
	{
		seg_lens->seg_cd_length = len;
	}

	len = calc_segment_length(positions->d_position, positions->e_position);
	if (len > 0)
	{
		seg_lens->seg_de_length = len;
	}

	len = calc_segment_length(positions->e_position, positions->f_position);
	if (len > 0)
	{
		seg_lens->seg_ef_length = len;
	}

	len = calc_segment_length(positions->f_position, waveform_tab_len - 1);
	if (len > 0)
	{
		seg_lens->seg_fend_length = len;
	}
}

/**
*	@brief	Calculate LUT values
*	@param	lut a pointer to a DSP_WaveformLUT object
*	@param	seglens a pointer to a stSegmentsLength struct that hols segments lengths
*	@param	positions a pointer to a stPositions struct that holds segment positions
*	@return 0 if OK -1 if illegal params
*/
int DSP_MorphingSinusOscWTAB::calc_wtab(DSP_WaveformTab *wtab, st_segments_length *seg_lens, 
										st_positions *positions)
{
	int i;
	en_segments segment = seg_0a;
	float val;

	if (wtab == nullptr)
	{
		return -1;
	}
	else
	{
		//	printf("\n");
		for (i = 0; i < waveform_tab_len; i++)
		{
			switch (segment)
			{
				case seg_0a:
					wtab->set_wtab_val(i, get_sin_segment_value(i, 0, seg_lens->seg_0a_length, quad_1));
					if (i >= positions->a_position)
					{
						segment = (en_segments)(segment + 1);
					}
					break;

				case seg_ab:
					wtab->set_wtab_val(i, 1.f);
					if (i >= positions->b_position)
					{
						segment = (en_segments)(segment + 1);
					}
					break;

				case seg_bc:
					wtab->set_wtab_val(i, get_sin_segment_value(i, positions->b_position, seg_lens->seg_bc_length, quad_2));
					if (i >= positions->c_position)
					{
						segment = (en_segments)(segment + 1);
					}
					break;

				case seg_cd:
					wtab->set_wtab_val(i, 0.f);
					if (i >= positions->d_position)
					{
						segment = (en_segments)(segment + 1);
					}
					break;

				case seg_de:
					wtab->set_wtab_val(i, get_sin_segment_value(i, positions->d_position, seg_lens->seg_de_length, quad_3));
					if (i >= positions->e_position)
					{
						segment = (en_segments)(segment + 1);
					}
					break;

				case seg_ef:
					wtab->set_wtab_val(i, -1.f);
					if (i >= positions->f_position)
					{
						segment = (en_segments)(segment + 1);
					}
					break;

				case seg_fend:
					wtab->set_wtab_val(i, get_sin_segment_value(i, positions->f_position, seg_lens->seg_fend_length, quad_4));
					break;
				}
			
			//			printf("%.2f ", lut->getLUTval(i));
			//			if (i % 16 == 0)
			//			{
			//				printf("\n");
			//			}
		}
	}
	
	return 0;
}

/**
*	@brief	Creates a MSO object instance
*	@param	lutptr a pointer to a DSP_MorphingSinusOscLUT LUT instance
*	@param	id unique MSO id number
*	@param	detOct detune octave 
*	@param	detStone detune semitones
*	@param	detCnts detune cents
*	@param	mag output magnitude (level)
*	@return none
*/
DSP_MorphingSinusOsc::DSP_MorphingSinusOsc(
	DSP_MorphingSinusOscWTAB *lutptr,
	int i_d,
	int detOct,
	int detStone,
	int detCnts,
	float mag)
{
	id = i_d;
	set_freq_detune_oct(detOct);
	set_freq_detune_semitones(detStone);
	set_freq_detune_cents(detCnts);
	set_magnitude(mag);
	
	wtab_index = 0;
	wtab = lutptr;
}


/**
 *	@brief	Returns a pointer to the DSP_MorphingSinusOscWTAB wavetable object.
 *	@param	none
 *	@return a pointer to the DSP_MorphingSinusOscWTAB wavetable object
 */
DSP_MorphingSinusOscWTAB *DSP_MorphingSinusOsc::get_wavetable()
{
	return wtab;
}

/**
 *	@brief	Set a pointer to a DSP_MorphingSinusOscWTAB wavetable object.
 *	@param	a pointer to the table
 *	@return void
 */
void DSP_MorphingSinusOsc::set_wavetable(DSP_MorphingSinusOscWTAB *new_wtab)
{
	if (new_wtab)
	{
		wtab = new_wtab;
	}
}

/**
*	@brief	Return next MSO LUT sample
*	@param	freq MSO frequency (Hz)
*	@param	offset output index offset 
*	@return next MSO LUT sample
*/
float DSP_MorphingSinusOsc::get_next_mso_wtab_val(float freq, int offset)
{
	float ofst_index, val;
	
	wtab_index += freq  / wtab->get_fundemental_frequency();
	// Wrap
	while ((int)wtab_index >= wtab->morphed_waveform_tab->get_wtab_length()) 
	{
		wtab_index -= wtab->morphed_waveform_tab->get_wtab_length();
	}

	if (offset > 0)
	{
		ofst_index = wtab_index + (float)offset;
		// Wrap
		while ((int)ofst_index >= wtab->morphed_waveform_tab->get_wtab_length()) 
		{
			ofst_index -= wtab->morphed_waveform_tab->get_wtab_length();
		}
		val = wtab->morphed_waveform_tab->get_wtab_val((int)ofst_index);
	}
	else
	{
		val = wtab->morphed_waveform_tab->get_wtab_val((int)wtab_index);
	}	

	return val * magnitude;
}

/**
*	@brief	Return MSO id number
*	@param	none
*	@return void
*/
int DSP_MorphingSinusOsc::get_id() 
{ 
	return id; 
}

/**
*	@brief	Set MSO PWM dutycycle ??? below?
*	@param	dc MSO PWM dutycycle 5-95
*	@return void
*/
void DSP_MorphingSinusOsc::set_pwm_dcycle_set_val(int dc)
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
}

/**
*	@brief	Return MSO PWM dutycycle
*	@param	none
*	@return MSO PWM dutycycle 5-95
*/
int DSP_MorphingSinusOsc::get_pwm_dcycle_set_val() 
{ 
	return pwm_percents_set; 
}

/**
*	@brief	Set MSO PWM dutycycle ??? above?
*	@param	dc MSO PWM dutycycle 5-95
*	@return void
*/
void DSP_MorphingSinusOsc::set_pwm_dcycle(int dc)
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
}

/**
*	@brief	Return MSO PWM dutycycle
*	@param	none
*	@return MSO PWM dutycycle 5-95
*/
int DSP_MorphingSinusOsc::get_pwm_dcycle() 
{ 
	return pwm_percents; 
}

/**
*	@brief	Set MSO frequency detune octave
*	@param	octMSO frequency detune octave _OSC_DETUNE_MIN_OCTAVE to _OSC_DETUNE_MAX_OCTAVE
*	@return void
*/
void DSP_MorphingSinusOsc::set_freq_detune_oct(int oct)
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
}

/**
*	@brief	Return MSO frequency detune octave
*	@param	none
*	@return MSO frequency detune octave _OSC_DETUNE_MIN_OCTAVE to _OSC_DETUNE_MAX_OCTAVE
*/
int DSP_MorphingSinusOsc::get_freq_detune_oct() 
{ 
	return detune_octave; 
}

/**
*	@brief	Set MSO frequency detune semitones
*	@param	sem MSO frequency detune semitones _OSC_DETUNE_MIN_SEMITONES to _OSC_DETUNE_MAX_SEMITONES
*	@return void
*/
void DSP_MorphingSinusOsc::set_freq_detune_semitones(int sem)
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
*	@brief	Return MSO frequency detune semitones
*	@param	none
*	@return MSO frequenct detune semitones _OSC_DETUNE_MIN_SEMITONES to _OSC_DETUNE_MAX_SEMITONES
*/
int DSP_MorphingSinusOsc::get_freq_detune_semitones() 
{ return 
		detune_semitones; 
}

/**
*	@brief	Set MSO frequenct detune cents (int)
*	@param	cnt MSO frequenct detune cents (int) (_OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS)/_OSC_DETUNE_CENTS_FACTORIAL
*	@return void
*/
void DSP_MorphingSinusOsc::set_freq_detune_cents_set_value(int cnt)
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
*	@brief	Return MSO frequency detune cents (int)
*	@param	none
*	@return MSO frequenct detune cents (_OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS)/_OSC_DETUNE_CENTS_FACTORIAL
*/
int DSP_MorphingSinusOsc::get_freq_detune_cents_set_value() 
{ 
	return detune_cents_set_value; 
}

/**
*	@brief	Set MSO frequency detune cents (float)
*	@param	cnt MSO frequency detune cents (float) _OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS
*	@return void
*/
void DSP_MorphingSinusOsc::set_freq_detune_cents(float cnt)
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
}

/**
*	@brief	Return MSO frequenct detune cents (float)
*	@param	none
*	@return MSO frequenct detune cents (float) _OSC_DETUNE_MIN_CENTS to _OSC_DETUNE_MAX_CENTS
*/
float DSP_MorphingSinusOsc::get_freq_detune_cents() 
{ 
	return detune_cents; 
}

/**
*	@brief	Set MSO magnitude (volume)
*	@param	mag MSO output level magnitude 0.0-1.0
*	@return void
*/
void DSP_MorphingSinusOsc::set_magnitude(float mag)
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
*	@brief	Return MSO magnitude (volume)
*	@param	none
*	@return MSO output level magnitude 0.0-1.0
*/
float DSP_MorphingSinusOsc::get_magnitde() 
{ 
	return magnitude; 
}

/**
*	@brief	Set MSO send level to channel 1
*	@param	snd MSO send level to channel 1 0.0-1.0
*	@return snd MSO send level to channel 1 0.0-1.0
*/
float DSP_MorphingSinusOsc::set_send_level_1(float snd)
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
*	@brief	Set MSO send level to channel 1 (int)
*	@param	snd MSO send level to channel 1 0-100
*	@return MSO send level to channel 1 0.0-1.0
*/
float DSP_MorphingSinusOsc::set_send_level_1(int snd)
{
	float send = (float)snd / 100.0f;

	set_send_level_1(send);
	return send;
}

/**
*	@brief	Set MSO send level to channel 2
*	@param	snd MSO send level to channel 2 0.0-1.0
*	@return MSO send level to channel 2 0.0-1.0
*/
float DSP_MorphingSinusOsc::set_send_level_2(float snd)
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
*	@brief	Set MSO send level to channel 2 (int)
*	@param	snd MSO send level to channel 2 0-100
*	@return MSO send level to channel 2 0.0-1.0
*/
float DSP_MorphingSinusOsc::set_send_level_2(int snd)
{
	float send = (float)snd / 100.0f;
	
	set_send_level_2(send);
	return send;
}

/**
*	@brief	Return MSO send level to channel 1
*	@param	none
*	@return MSO send level to channel 1 0.0-1.0
*/
float DSP_MorphingSinusOsc::get_send_level_1() 
{ 
	return send_level_1; 
}

/**
*	@brief	Return MSO send level to channel 2
*	@param	none
*	@return MSO send level to channel 2 0.0-1.0
*/
float DSP_MorphingSinusOsc::get_send_level_2() 
{ 
	return send_level_2;
}
