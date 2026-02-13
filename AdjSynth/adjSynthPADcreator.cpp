/**
*	@file		adjSynthPADcreator.cpp
*	@author		Nahum Budin
*	@date		14-Oct-2025
*	@version	1.3 
*					1. Bug fixing.
				
*	@History
*	version 1.2 5-Oct-2024	Code refactoring and notaion
*	version	1.1 3-Feb-2021	Code refactoring and notaion.
*	version	1.0	15-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October, 2018)
*
*	@brief		PAD waveform table synthesis
*				https://github.com/zynaddsubfx/zynaddsubfx
*/

#include <cstring>
#include <unistd.h>

#include "adjSynthPADcreator.h"
#include "../utils/FFTwrapper.h"
#include "../utils/utils.h"
#include "../commonDefs.h"

const int num_of_pad_harmonies = _PAD_NUM_OF_HAROMONIES;

/**
* @brief	intialize the PAD creator
* @param	wavetable_buff	a pointer to a Wavetable object
* @param	int size		wavetable length).
* @param	int samp_rate	sample-rate(Hz)
* @return	void.
*/
SynthPADcreator::SynthPADcreator(Wavetable *wavetable_buff, int size, int samp_rate)
{	
	init(wavetable_buff, size, samp_rate);
}

/**
* @brief	creates and intializes the PAD creator instance.
* @param	wavetable_buff	a pointer to a Wavetable object
* @param	int size		wavetable length).
* @return	void.
*/
void SynthPADcreator::init(Wavetable *wavetable_buff, int size, int samp_rate)
{
	set_sample_rate(samp_rate);
	
	if ((wavetable_buff == NULL) || (size <= 0))
	{
		external_wavetable = false;
		wavetable = new Wavetable();
		wavetable->size = _PAD_DEFAULT_WAVETABLE_SIZE;
		wavetable->samples = new float[wavetable->size];
		wavetable->samples[0] = 0.0f;
		wavetable->base_freq = _PAD_DEFAULT_BASE_NOTE_FREQ;
	}
	else
	{
		external_wavetable = true;
		wavetable = wavetable_buff;
		// Verify that size is a power of 2
		if ((wavetable->size & (wavetable->size - 1)) != 0)
		{
			// Not a power of 2
			printf("PAD creator Error: wavetable length must be a power of 2");
			sleep(3);
			exit(1);
		}
	}

	base_harmony_bandwidth = 500.f;
	harmony_shape = _PAD_SHAPE_RECTANGULAR;
	harmony_shape_cutoff = _PAD_SHAPE_CUTOFF_FULL;
	spectrum_length = wavetable->size / 2;
	set_base_note(wavetable, _PAD_DEFAULT_BASE_NOTE);
	harmonies_levels[0] = 1.f;
	for (int i = 1; i < _PAD_NUM_OF_HAROMONIES; i++)
	{
		harmonies_levels[i] = 0;
	}
	harmonies_levels[2] = 0.3f;
	harmonies_detune = 0;

	base_harmony_bandwidth = 500.f;

	spectrum = (float*)malloc(spectrum_length * sizeof(float));
}

SynthPADcreator::~SynthPADcreator()
{
	if ((wavetable != NULL) && !external_wavetable)
	{
		delete[] wavetable->samples;
		delete(wavetable);
	}

	if (spectrum != NULL) 
	{
		delete[] spectrum;
	}
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int SynthPADcreator::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int SynthPADcreator::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
* @brief	Sets the base harmony width
* @param	wd	base harmony width 0-100
* @return	base harmony width if OK; -1 if param out of range.
*/
float SynthPADcreator::set_base_harmony_width(int wd)
{
	if ((wd >= 0) && (wd <= 100))
	{
		return set_get_bandwidth(wd * 10);
	}
	else
	{
		return -1;
	}
}

/**
* @brief	Returns the base harmony width
* @param	none
* @return	base harmony width.
*/
float SynthPADcreator::get_base_harmony_width() 
{ 
	return 
		base_harmony_bandwidth; 
}


/**
* @brief	Set the harmonies envelope shape
* @param	shp shape: _PAD_SHAPE_GAUSSIAN, _PAD_SHAPE_RECTANGULAR, _PAD_SHAPE_DOUBLE_EXP
* @return	base harmony width if OK; -1 if param out of range.
*/
int SynthPADcreator::set_harmony_shape(int shp)
{
	if (is_valid_pad_harmonies_envelope_shape(shp))
	{
		harmony_shape = shp;
		return harmony_shape;
	}
	else
	{
		return -1;
	}
}

int SynthPADcreator::get_harmony_shape() 
{ 
	return harmony_shape; 
}

/**
* @brief	Set the harmonies envelope shape cutoff	
* @param	shp shape: _PAD_SHAPE_CUTOFF_FULL, _PAD_SHAPE_CUTOFF_UPPER, _PAD_SHAPE_CUTOFF_LOWER
* @return	harmony shape envelope cutoff if OK; -1 if param out of range.
*/
int SynthPADcreator::set_harmony_shape_cutoff(int shpco)
{
	if (is_valid_pad_harmonies_envelope_shape_cutoff(shpco))
	{
		harmony_shape_cutoff = shpco;
		return harmony_shape_cutoff;
	}
	else
	{
		return -1;
	}
}

/**
* @brief	Returns the harmonies envelope shape cutoff	
* @param	none
* @return	harmony shape envelope cutoff.
*/
int SynthPADcreator::get_harmony_shape_cutoff() 
{ 
	return harmony_shape_cutoff; 
}

/**
* @brief	Set the harmonies level	
* @param	harm	harmony number (0 - _PAD_NUM_OF_HAROMONIES)
* @param	lev		harmony number (0 - 1.0)
* @return	harmony number if OK; -1 if param out of range.
*/
int SynthPADcreator::set_harmony_level(int harm, float lev)
{
	if ((harm >= 0) && (harm < _PAD_NUM_OF_HAROMONIES) && (lev >= 0) && (lev <= 1.f))
	{
		harmonies_levels[harm] = lev;
		return harmony_shape;
	}
	else
	{
		return -1;
	}
}

/**
* @brief	Returns an harmony level							
* @param	harm	harmony number (0 - _PAD_NUM_OF_HAROMONIES)
* @return	harmony level 0-1.0 if OK; -1 if param out of range.
*/
float SynthPADcreator::get_harmony_level(int harm)
{
	if ((harm >= 0) && (harm < _PAD_NUM_OF_HAROMONIES))
	{	
		return harmonies_levels[harm];
	}
	else
	{
		return -1;
	}
}

/**
* @brief	Set the harmonies detune level							
* @param	det	harmonies detune level (0 - 1.0)
* @return	harmony detune level (0-1.0) if OK; -1 if param out of range.
*/
float SynthPADcreator::set_harmonies_detune(float det)
{
	if ((det >= 0) && (det <= 1.0))
	{	
		harmonies_detune = det;
		return harmonies_detune;
	}
	else
	{
		return -1;
	}
}

/**
* @brief	Returns the harmonies detune level							
* @param	none
* @return	harmony detune level (0-1.0) .
*/
float SynthPADcreator::get_harmonies_detune() 
{ 
	return harmonies_detune; 
}

/**
* @brief	set wavetable buffer length (1<<(15+len). 
*			(deletes prev table and allocates a new one)
* param		wt	a pointer to a wave table object of type Wavetable
* @param	int len _PAD_QUALITY_32K - _PAD_QUALITY_1024K (0-5).
* @return	length (samples) if OK; -1 if params out of range.
*/
int SynthPADcreator::set_wavetable_length(Wavetable *wt, int len)
{
	if (wt == NULL)
	{
		return -1;
	}
	
	if ((len >= _PAD_QUALITY_32K) && (len <= _PAD_QUALITY_1024K) &&
		(1 << (15 + len) != wt->size)) // No need to change
	{
		delete[] wt->samples;
		//	delete(wt);
		//	wt = new Wavetable();
		wt->size = (1 << (15 + len));
		wt->samples = new float[wt->size];
		wt->samples[0] = 0.0f;
		wt->base_freq = _PAD_DEFAULT_BASE_NOTE_FREQ;
		
		if (spectrum != NULL)
		{
			delete[] spectrum;
		}

		spectrum_length = wt->size / 2;
		spectrum = (float*)malloc(spectrum_length * sizeof(float));
		
		return wt->size;
	}

	return -1;
}

/**
* @brief	Returns wavetable buffer length (1<<(15+len). 
* param		wt	a pointer to a wave table object of type Wavetable
* @return	length (samples) if OK; -1 if parameter is NULL.
*/
int SynthPADcreator::get_wavetable_length(Wavetable *wt) 
{ 
	if (wt == NULL)
	{
		return -1;
	}
	else 
	{
		return wavetable->size; 
	}
}

/**
* @brief	Sets the wavetable base note. 
* param		wt	a pointer to a wave table object of type Wavetable
* @param	note  _PAD_BASE_NOTE_C2 - _PAD_BASE_NOTE_G6 (0-9).
* @return	length (samples) if OK; -1 if params out of range.
*/
int SynthPADcreator::set_base_note(Wavetable *wt, int note)
{
	if (wt == NULL)
	{
		return -1;
	}
	
	if ((note >= _PAD_BASE_NOTE_C2) && (note <= _PAD_BASE_NOTE_G6))
	{
		base_note = note;
		set_base_frequency(wt, note);
		return base_note;
	}
	else
	{
		return -1;
	}
}

/**
* @brief	Returns the base-note. 
* param		none
* @return	base-note  _PAD_BASE_NOTE_C2 - _PAD_BASE_NOTE_G6 (0-9)..
*/
int SynthPADcreator::get_base_note() 
{ 
	return base_note; 
}

/**
* @brief	Sets the wavetable base frequency based on given base-note. 
* param		wt	a pointer to a wave table object of type Wavetable
* @param	base-note  _PAD_BASE_NOTE_C2 - _PAD_BASE_NOTE_G6 (0-9).
* @return	base-note frequency (Hz) if OK; -1 if params out of range.
*/
float SynthPADcreator::set_base_frequency(Wavetable *wt, int base_note)
{
	if (wt == NULL)
	{
		return -1;
	}
	
	if ((base_note >= _PAD_BASE_NOTE_C2) && (base_note <= _PAD_BASE_NOTE_G6))
	{
		// C2 65.406Hz
		base_frequency = 65.406f * powf(2.0f, (base_note / 2));
		if (base_note % 2 == 1)
		{
			// Gx (not Cx)
			base_frequency *= 1.5f;
		}
	}

	wt->base_freq = base_frequency;

	return base_frequency;
}

/**
* @brief	Returns the wavetable base frequency. 
* param		none
* @return	base-note frequency (Hz).
*/
float SynthPADcreator::get_base_frequency() 
{ 
	return base_frequency; 
}

/**
* @brief	Returns the profile size. 
* param		none
* @return	profile size.
*/
int SynthPADcreator::get_profile_size() 
{ 
	return profile_size; 
}

/**
* @brief	Returns a pointer to the profile data. 
* param		none
* @return	a pointer to the profile data.
*/
float *SynthPADcreator::get_profile_data() 
{ 	
	return &profile[0]; 
}

/**
* @brief	Returns the spectrum size. 
* param		none
* @return	spectrum size.
*/
int SynthPADcreator::get_spectrum_size() 
{ 
	return spectrum_length; 
}

/**
* @brief	Returns a pointer to the spectrum data. 
* param		none
* @return	a pointer to the spectrum data.
*/
float *SynthPADcreator::get_spectrum_data() 
{ 
	return &spectrum[0]; 
}

/**
* @brief	Detunes harmony (even/odd). 
* param		harm	harmony number
* @return	harmony detune factor
*/
float SynthPADcreator::detune_harmony(int harm)
{
	if (harm == 1)
	{
		return 1.f;
	}
	else if ((harm % 2) == 0)
	{
		return 1.f + harmonies_detune / 50.f;
	}
	else
	{
		return 1.f - harmonies_detune / 50.f;
	}	
}

/**
* @brief  Calculates and returns the real bandwidth in cents
* @param  bandwidth (0-100)
* @return real bandwidth in cents
*/
float SynthPADcreator::set_get_bandwidth(int bw)
{
	base_harmony_bandwidth = bw;
	
	if (base_harmony_bandwidth <= 0)
	{
		base_harmony_bandwidth = 10.f;
	}
	else if (base_harmony_bandwidth > 1000.f)
	{
		base_harmony_bandwidth = 1000.f;
	}

	float result = powf(base_harmony_bandwidth / 1000.0f, 1.1f);
	result = powf(10.0f, result * 4.0f) * 0.25f;
	
	return result;
}

/**
	* @brief	Generates the long spectrum array for Bandwidth mode (only amplitudes are generated; phases will be random)
	* @param	float *spectrum		pointer to the spectrum buffer
	* @param	int   size			size of spectrum buffer
	* @param	float base_freq		frequency of base harmonic
	* @param	float *profile		pointer to the base harmony shape buffer
	* @param	int   profile_size	size of profile
	* @param	bw_scale				bandwidth increasing factor
	*/
void SynthPADcreator::generate_spectrum_bandwidth_mode(
	float *spectrum,
	int size,
	float base_frq,
	const float *profile,
	int profile_size,
	float bw_scale) 
{
	float bandwidth_cents, power, real_frq, bw, rap, ibase_frq;
	int harmonic_num, bwi, c_freq, i, src, sp_freq, id_freq, freq_sum, fsp_freq;
	
	memset(spectrum, 0, sizeof(float) * size);

	bandwidth_cents = set_get_bandwidth(base_harmony_bandwidth);
	power = 1.f;

	//for each harmonic
	for (harmonic_num = 1; harmonic_num <= _PAD_NUM_OF_HAROMONIES; harmonic_num++)
	{
		// harmony freq
		real_frq = detune_harmony(harmonic_num) * base_frq * harmonic_num;	
		if (real_frq > (float)sample_rate * 0.49999f)
		{
			break;
		}
		if (real_frq < 20.0f)
		{
			break;
		}
		
		if (harmonies_levels[harmonic_num - 1] < 1e-4)
		{
			continue;
		}

		//compute the bandwidth of each harmonic
		bw = ((powf(2.0f, bandwidth_cents / 1200.0f) - 1.0f) * base_frq / bw_scale)
			* powf(real_frq / base_frq, power);
		bwi = (int)((bw / ((float)sample_rate * 0.5f) * size)) + 1;

		if (bwi > profile_size)
		{
			// The bandwidth is larger than the profilesize
			rap = sqrt((float)profile_size / (float)bwi);
			c_freq = (int)(real_frq / ((float)sample_rate * 0.5f) * size) - bwi / 2;
			for (i = 0; i < bwi; ++i) 
			{
				src = i * rap * rap;
				sp_freq = i + c_freq;
				if (sp_freq < 0)
				{
					continue;
				}
				if (sp_freq >= size)
				{
					break;
				}
				spectrum[sp_freq] += harmonies_levels[harmonic_num - 1] * profile[src] * rap;
			}
		}
		else
		{
			// The bandwidth is smaller than the profilesize
			rap = sqrt((float)bwi / (float)profile_size);
			ibase_frq = real_frq / ((float)sample_rate * 0.5f) * size;
			for (i = 0; i < profile_size; ++i)
			{
				id_freq = (i / (float)profile_size - 0.5f) * bwi;
				freq_sum = id_freq + ibase_frq;
				sp_freq = (int)freq_sum;
				fsp_freq = freq_sum - sp_freq;
				if (sp_freq <= 0)
				{
					continue;
				}
				
				if (sp_freq >= size - 1)
				{
					break;
				}
				spectrum[sp_freq] += harmonies_levels[harmonic_num - 1] * profile[i]  * rap * (1.0f - fsp_freq);
				spectrum[sp_freq + 1] += harmonies_levels[harmonic_num - 1] * profile[i]  * rap * fsp_freq;
			}
		}
	}
}

/**
* @brief  Generates the wavetable
* @param  wt	a pointer to a wave table object of type Wavetable
* @return 0 if OK; -1; if wt is NULL; -2 wt size out of range or null data
*/
int SynthPADcreator::generate_wavetable(Wavetable *wt)
{
	int i;

	if (wt == NULL)
	{
		return -1;
	}

	if ((wt->size <= 0) || (wt->samples == NULL))
	{
		return -2;
	}

	const float bwadjust = get_profile(&profile[0], profile_size);
	
	// prepare the IFFT
	FFTwrapper *fft = new FFTwrapper(wavetable->size);
	fft_t      *fftfreqs = new fft_t[spectrum_length];

	generate_spectrum_bandwidth_mode(
		spectrum,
		spectrum_length,
		base_frequency,
		profile,
		profile_size,
		bwadjust);
	
	//randomize the phases
	for (i = 1; i < spectrum_length; ++i) 
	{
		fftfreqs[i] = FFTpolar(spectrum[i], (float)RND * 2 * PI);
	}
	//that's all; here is the only ifft for the whole sample;
	//no windows are used ;-)
	fft->freqs2smps(fftfreqs, wt->samples);
	//normalize(rms)
	float rms = 0.0f;
	for (int i = 0; i < wt->size; ++i)
	{
		rms += wt->samples[i] * wt->samples[i];
	}
	rms = sqrt(rms);
	if (rms < 0.000001f)
	{
		rms = 1.0f;
	}
	rms *= sqrt(262144.0f / wt->size); //262144=2^18
	for (int i = 0; i < wt->size; ++i)
	{
		wt->samples[i] *= 1.0f / rms * 200.0f;
	}

	//Cleanup
	delete(fft);
	delete[] fftfreqs;
	
	return 0;
}

/**
* @brief  Build the profile
* @param  smp		a pointer to the data
* @param  size		data size
* @return  profile bandwidth ; -1 if smp = null
*/
float SynthPADcreator::get_profile(float *smp, int size)
{
	if (smp == NULL)
	{
		return -1;
	}
	
	for (int i = 0; i < size; ++i)
	{
		smp[i] = 0.0f;
	}

	const int super_sample = 16;
	// Width of basic harmony (0->40; 100->1.01)
	float width = powf(120.0f / (float)(base_harmony_bandwidth / 10.f + 19), 2.0f);

	for (int i = 0; i < size * super_sample; ++i) 
	{
		bool  make_zero = false;
		// Frequency scaling
		float x = i * 1.0f / (size * (float)super_sample);

		float origx = x;

		// This creates symetry around the profile center.
		// for N=512 
		// BaseWidth=0   -> width=40 ->250..262
		// BaseWidth=100 -> width=1  ->  0..512
		x = (x - 0.5f) * width + 0.5f;
		if (x < 0.0f) 
		{
			x = 0.0f;
			make_zero = true;
		}
		else
		{
			if (x > 1.0f) 
			{
				x = 1.0f;
				make_zero = true;
			}
		}

		// Shape cutoff mode (do nothing if full)
		if (harmony_shape_cutoff == _PAD_SHAPE_CUTOFF_UPPER)
		{
			x = x * 0.5f + 0.5f;
		}
		else if (harmony_shape_cutoff == _PAD_SHAPE_CUTOFF_LOWER)
		{
			x = x * 0.5f;
		}
		// e.g. for 47-51 Full: (-1 to 1)  Lower: (-1 to 0)  Upper (0 to 1)
		x = fmod(x + 1000.0f, 1.0f) * 2.0f - 1.0f;
		// Build the base shapefunction of the profile
		float f;
		if (make_zero)
		{
			f = 0;
		}
		else
		{
			switch (harmony_shape) {
			case _PAD_SHAPE_RECTANGULAR:
				f = expf(-(x * x));
				if (f < 0.5f)
				{
					f = 0.0f;
				}
				else
				{
					f = 1.0f;
				}
				break;

			case _PAD_SHAPE_DOUBLE_EXP:
				f = expf(-(fabs(x)));
				break;
					
			case _PAD_SHAPE_GAUSSIAN:
			default:
				f = expf(-(x * x));
				break;
			}
		}

		smp[i / super_sample] += f / super_sample;
	}


	//normalize the profile (make the max. to be equal to 1.0f)
	float max = 0.0f;
	for (int i = 0; i < size; ++i) 
	{
		if (smp[i] < 0.0f)
		{
			smp[i] = 0.0f;
		}
		if (smp[i] > max)
		{
			max = smp[i];
		}
	}

	if (max < 0.00001f)
	{
		max = 1.0f;
	}

	float bias = 0.f;
	// Clamp to zero if not rectangle
	if (harmony_shape != _PAD_SHAPE_RECTANGULAR)
	{
		if (harmony_shape_cutoff != _PAD_SHAPE_CUTOFF_UPPER)
		{
			// lower or full: start from "left to right"
			for (int i = 0; i < size; ++i)
			{
				if (smp[i] >= max / 10.f)
				{
					bias = smp[i];
					break;
				}
			}
		}
		else
		{
			// upper: start from "right to left"
			for (int i = size - 1; i >= 0; --i)
			{
				if (smp[i] >= max / 10.f)
				{
					bias = smp[i];
					break;
				}
			}
		}

		for (int i = 0; i < size; ++i)
		{
			smp[i] -= bias;
			if (smp[i] < 0)
			{
				smp[i] = 0;
			}
		}
	}
		
	for (int i = 0; i < size; ++i)
	{
		smp[i] /= (max / (1.f - bias));
	}
	

	//compute the estimated perceived bandwidth
	float sum = 0.0f;
	int   i;

	for (i = 0; i < size / 2 - 2; ++i) 
	{
		sum += smp[i] * smp[i] + smp[size - i - 1] * smp[size - i - 1];
		if (sum >= 4.0f)
		{
			break;
		}
	}

	float result = 1.0f - 2.0f * i / (float)size;
	return result;
}
