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

#pragma once

#include <math.h>
#include <stdint.h>

#include "../libAdjRaspi5Synth2.h"
#include "../Audio/audioCommons.h"
#include "../LibAPI/synthesizer.h"

//#include "../defs.h"

//#define NUM_OF_HARMONIES					9
//#define _NUM_OF_HARMONIZING_FREQUENCIES		13
#define _SINE_WAVETABLE_SIZE				2048
#define _SYNTH_FUNDEMENTAL_FREQUENCY		((float)AUDIO_SAMPLE_RATE / (float)AudioSettings::get_instence()->get_audio_buffer_size())
//#define _MAX_HARMONIES_DISTORTION_FACTOR	3.0f
//#define _MIN_HARMONIES_DISTORTION_FACTOR	1.0f
//#define _MAX_HARMONIES_DETUNE_FACTOR		0.08f
//#define _MIN_HARMONIES_DETUNE_FACTOR		0.0f
//#define _SINE_LUT_PHASE_STEP				(2.0f * M_PI / (float)AudioSettings::get_instence()->get_audio_buffer_size())
//#define _SINE_MAX_FREQUENCY					((float)AUDIO_SAMPLE_RATE/2.1f)
//#define _SINE_MIN_FREQUENCY					0.1f

class DSP_SineWaveGenerator
{
public:
	DSP_SineWaveGenerator(int samp_rate = _DEFAULT_SAMPLE_RATE);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	float get_fundemental_frequency();
	
	int set_sine_wavetable_size(int size);
	int get_sine_wavetable_size();
	
	void init_sine_waveform_table(int samp_rate);
	
	int set_unison_mode(int mod);
	float set_harmoies_distortion_factor(float dist);
	float get_harmoies_distortion_factor();
	float set_harmonies_detune_factor(float det);
	float get_harmonies_detune_factor();
	float set_harmony_level(int har, float lev);
	float get_harmony_level(int har);
	bool get_fund_harm_cycle_restarted_state();
	float get_next_sine_wtab_val(float freq); 
	float get_sine_wtab_val(float *hrmon_indexes, float distort);
	void calc_sin_wtab_index(float *hrmon_indexes, float detune, float freq);
	
	void set_harmonizig_frequencies(int mod, float harmon_fact);
	void set_harominies_active(uint16_t mask);
	void get_harominies_active(bool *active);
	void init_wtab_indexes();
	bool get_cycle_restarted_sync_state();
	
	void enable_unison_square();
	void disable_unison_square();
	bool get_unison_square_state();
	
private:
	int num_of_active_haromnies;
	int unison_mode;
	float  harmonies_dist_factor;
	float harmonies_detune_factor;
	// Sine Lookup Table (LUT)
	float sine_wtab[_AUDIO_MAX_BUF_SIZE];
	float harmonizing_frequencies[_SINE_WAVETABLE_SIZE] = { 1.0f };
	float harmonies_levels[_NUM_OF_HARMONIES];
	bool harmonies_active[_NUM_OF_HARMONIES];
	float fl_harmonies_wtab_indexes[_NUM_OF_HARMONIES];
	// Indicates a new cycle of the fundemental harmony has just restarted *  /
	bool cycle_restarted;
	// When true generate a squre wave (and not sine)
	bool square;

	float prev_sin_value = 0.0f;
	
	int sample_rate;
	float fundemental_frequency;
	int sine_wavetable_size;
	float max_frequency;
	float min_frequency;
	float wtab_phase_step;
}
;


