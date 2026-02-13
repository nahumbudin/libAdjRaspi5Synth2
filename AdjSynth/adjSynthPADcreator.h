/**
*	@file		adjSynthPADcreator.h
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	1.1 3-Feb-2021
*					1. Code refactoring and notaion.
*					2. Adding sample-rate settings
*				1.0	15-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October, 2018)
*
*	@brief		PAD waveform table synthesis
*				https://github.com/zynaddsubfx/zynaddsubfx
*/

#pragma once


#include "../DSP/dspWavetable.h"
#include "../LibAPI/audio.h"
#include "../LibAPI/synthesizer.h"


extern const int numOfPadHarmonies;

#define _PAD_NUM_OF_HAROMONIES		10
#define _PAD_DEFAULT_WAVETABLE_SIZE	(1 << (15 + _PAD_QUALITY_128K))
#define _PAD_MAX_WAVETABLE_SIZE	(1 << (15 + _PAD_QUALITY_1024K))
#define _PAD_DEFAULT_BASE_NOTE	_PAD_BASE_NOTE_C3
#define _PAD_DEFAULT_BASE_NOTE_FREQ	(float)(65.406f * powf(2.0f, ((float)_PAD_BASE_NOTE_C3 / 2)))

//struct Wavetable;

class SynthPADcreator
{
public:
	SynthPADcreator(Wavetable *wavetable_buff = NULL, int size = 0, int samp_rate = _DEFAULT_SAMPLE_RATE);
	~SynthPADcreator();
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();

	void init(Wavetable *wavetable_buff = NULL, int size = 0, int samp_rate = _DEFAULT_SAMPLE_RATE);

	//static SynthPADcreator *getInstance();
	
	float set_base_harmony_width(int wd = 30);
	float get_base_harmony_width();

	int set_harmony_shape(int shp = _PAD_SHAPE_GAUSSIAN);
	int get_harmony_shape();

	int set_harmony_shape_cutoff(int shpco = _PAD_SHAPE_CUTOFF_FULL);
	int get_harmony_shape_cutoff();

	int set_harmony_level(int harm, float lev);
	float get_harmony_level(int harm);

	float set_harmonies_detune(float det);
	float get_harmonies_detune();

	int set_wavetable_length(Wavetable *wt = NULL, int len = _PAD_DEFAULT_WAVETABLE_SIZE);
	int get_wavetable_length(Wavetable *wt = NULL);

	int set_base_note(Wavetable *wt = NULL, int note = _PAD_DEFAULT_BASE_NOTE);
	int get_base_note();

	float set_base_frequency(Wavetable *wt = NULL, int base_note = _PAD_DEFAULT_BASE_NOTE);
	float get_base_frequency();

	int get_profile_size();
	float *get_profile_data();

	int get_spectrum_size();
	float *get_spectrum_data();

	float get_profile(float *smp = NULL, int size = profile_size);

	int generate_wavetable(Wavetable *wt = NULL);

	static const int profile_size = 512;
	
	
private:

	bool external_wavetable;

	float detune_harmony(int harm);

	float set_get_bandwidth(int bw);

	void generate_spectrum_bandwidth_mode(
		float *spectrum,
		int size,
		float base_freq,
		const float *profile,
		int profile_size,
		float bw_scale);

	//	static SynthPADcreator *synthPADcreator;
	
	float base_harmony_bandwidth;
	int harmony_shape;
	int harmony_shape_cutoff;
	float harmonies_levels[_PAD_NUM_OF_HAROMONIES];
	float harmonies_detune;
	int spectrum_length;
	int base_note;
	float base_frequency;

	/*static*/ float profile[profile_size] = { 0 };

	float *spectrum;
	Wavetable *wavetable;
	
	int sample_rate;
};
