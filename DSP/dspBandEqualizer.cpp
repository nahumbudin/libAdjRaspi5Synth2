/**
*	@file		dspBandEqualizer.cpp
*	@author		Nahum Budin
*	@date		3-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@History	23-Jan-2021	1.1	Code refactoring and notaion.
*				31-Oct-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1
*
* https://www.nxp.com/docs/en/application-note/AN2110.pdf
*
*	@brief		10 Band Equalizer
*/

#include <math.h>
#include "dspBandEqualizer.h"

/**
*	@brief	Creates an IIR filter instance of provided frequency band
*	@param	band filter frequency band.
*	@return none
*/
IIRbandpassFilter::IIRbandpassFilter(enum iir_filter_bands band)
{
	set_filter_band(band);
}

/**
*	@brief	Set filter frequency band
*	@param	band filter frequency band.
*	@return void
*/
void IIRbandpassFilter::set_filter_band(enum iir_filter_bands band)
{
	alfa = bands_alfa[band];
	beta = bands_beta[band];
	gama = bands_gama[band];

	x_n_1 = 0.f;
	x_n_2 = 0.f;
	y_n_1 = 0.f;
	y_n_2 = 0.f;
}

/**
*	@brief	Returns filter next output value
*	@param	band filter frequency band.
*	@return filter next output value
*/
float IIRbandpassFilter::get_filter_output(float in)
{
	float y_n = 2.f*((alfa * (in - x_n_2)) + (gama * y_n_1) - (beta * y_n_2));
	
	x_n_2 = x_n_1;
	x_n_1 = in;
	y_n_2 = y_n_1;
	y_n_1 = y_n;

	return y_n;
}

/**
*	@brief	Creates an equilizer instance
*	@param	none.
*	@return none
*/
DSP_BandEqualizer::DSP_BandEqualizer()
{
	total_gain = 0.f;
	
	for (int i = 0; i <= _band_16000_hz; i++)
	{
		filters[i] = new IIRbandpassFilter((iir_filter_bands)i);
		set_band_level((iir_filter_bands)i, 0);
		//	totalGain += bandsLevels[i];
		bands_levels[i] = 1.f;
	}

	total_gain = 1.f;
}

/**
*	@brief	Sets frequency band output level
*	@param	band filter frequency band.
*	@param	requency band output level (-50 to +50) (-20db to +20db)
*	@return void
*/
void DSP_BandEqualizer::set_band_level(enum iir_filter_bands band, int level)
{
	int i;

	if (level > 50)
	{
		level = 50;
	}
	else if (level < -50)
	{
		level = -50;
	}
	// Level is in db: 0db-> 1.0; +20db->10; -20db->0.1
	bands_levels[band] = pow(10.0, (double)level / 20.0);
	// Normalize
	total_gain = 0.f;
	for (i = _band_31_hz; i <= _band_16000_hz; i++)
	{
		total_gain += bands_levels[i];
	}

	total_gain /= 10.0;

	if (total_gain < 0.1f)
	{
		total_gain = 0.1f;
	}
}

/**
*	@brief	Returns frequency band output level
*	@param	band filter frequency band.
*	@return frequency band output level
*/
float DSP_BandEqualizer::get_band_level(enum iir_filter_bands band) { return bands_levels[band]; }

/**
*	@brief	Sets equilizer bands levels preset
*	@param	prst equilizer bands levels preset index.
*	@return void
*/
void DSP_BandEqualizer::set_preset(int prst)
{
	// TODO: range checking

	preset = prst;
}

/**
*	@brief	Returns equilizer next output value
*	@param	in	new input sample.
*	@return equilizer next output value
*/
float DSP_BandEqualizer::get_equalizer_next_output(float in)
{
	int band;
	float out = 0.f;

	for (band = _band_31_hz; band <= _band_16000_hz; band++)
	{
		out += filters[band]->get_filter_output(in) * bands_levels[band];
	}

	return out / total_gain;
}
