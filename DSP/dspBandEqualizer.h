/**
*	@file		dspBandEqualizer.h
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

#pragma once

#define _BAND_EQUALIZER_PRESET_USER			0;
#define _BAND_EQUALIZER_PRESET_FLAT			1;

enum iir_filter_bands
{
	_band_31_hz    = 0,
	_band_62_hz,
	_band_125_hz,
	_band_250_hz,
	_band_500_hz,
	_band_1000_hz,
	_band_2000_hz,
	_band_4000_hz,
	_band_8000_hz,
	_band_16000_hz
};

//					31Hz			62Hz		....
const float bands_alfa[] = {
	0.000723575,
	0.001445062,
	0.002904926,
	0.005776487,
	0.011422552,
	0.02234653,
	0.04286684,
	0.079552886,
	0.1199464,
	0.159603
};
const float bands_beta[] = {
	0.49855285, 
	0.497109876,
	0.494190149,
	0.488447026,
	0.477154897,
	0.455306941,
	0.414266319,
	0.340894228,
	0.2601072,
	0.1800994
};
const float bands_gama[] = {
	0.998544628,
	0.997077038,
	0.994057064, 
	0.987917799,
	0.975062733,
	0.947134157,
	0.88311345, 
	0.728235763,
	0.3176087, 
	-0.4435172
};

const float presets[1][10] = { { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f } };

class IIRbandpassFilter
{
public:
	IIRbandpassFilter(enum iir_filter_bands band);
	void set_filter_band(enum iir_filter_bands band);
	float get_filter_output(float in);

private:
	float alfa, beta, gama;
	float x_n_1, x_n_2, y_n_1, y_n_2;
};

class DSP_BandEqualizer
{
public:
	DSP_BandEqualizer();
	void set_band_level(enum iir_filter_bands band, int level);
	float get_band_level(enum iir_filter_bands band);
	void set_preset(int prst);
	float get_equalizer_next_output(float in);

private:
	IIRbandpassFilter *filters[_band_16000_hz + 1];
	float bands_levels[_band_16000_hz + 1];
	int preset;
	float total_gain;
};
