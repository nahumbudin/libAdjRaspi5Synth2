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

#pragma once

#include "dspWaveformTable.h"
#include "../LibAPI/audio.h"

// Waveform segments
enum en_segments 
{
	seg_0a, 
	seg_ab, 
	seg_bc, 
	seg_cd, 
	seg_de, 
	seg_ef,
	seg_fend
};

// Waveform segments positions
enum en_positions 
{ 
	pos_a, 
	pos_b, 
	pos_c, 
	pos_d, 
	pos_e, 
	pos_f 
};

// Sinusoide quadratures
enum en_sin_quad 
{ 
	quad_1, 
	quad_2, 
	quad_3, 
	quad_4 
};

// Holds segments cross-points
typedef struct
{
	int a_position;
	int b_position;
	int c_position;
	int d_position;
	int e_position;
	int f_position;
} st_positions;

// Holds segments length
typedef struct
{
	int seg_0a_length;
	int seg_ab_length;
	int seg_bc_length;
	int seg_cd_length;
	int seg_de_length;
	int seg_ef_length;
	int seg_fend_length;
} st_segments_length;

// Sine-wave preset positions
static const st_positions _st_positions_sine = { 512, 513, 1024, 1025, 1536, 1537 };
// Symetric square-wave preset positions
static const st_positions _st_positions_square = { 1, 1023, 1024, 1025, 1026, 2046 };
// Sawtooth-wave preset positions
static const st_positions _st_positions_saw = { 1, 2, 1024, 1023, 2045, 2046 };

// MSO waveform table
class DSP_MorphingSinusOscWTAB
{
public:	
	//	friend class SynthVoice;

	DSP_MorphingSinusOscWTAB(int samp_rate = _DEFAULT_SAMPLE_RATE);

	~DSP_MorphingSinusOscWTAB();

	// static DSP_MorphingSinusOscWTAB *get_morphing_sinus_osc_wtab_instance(int samp_rate);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	float get_fundemental_frequency();
	
	int get_wavetable_length();
	
	int init_waveform_tables(int samp_rate);

	int set_segment_position(en_positions pos, int value, st_positions *positions);
	void set_morphing_symetry(int symp);
	int get_morphing_symetry();
	void calc_segments_lengths(st_segments_length *seglens, st_positions *positions);
	int calc_wtab(DSP_WaveformTab *wtab, st_segments_length *seglens, st_positions *position);

	// Waveform tables
	DSP_WaveformTab *base_waveform_tab, *morphed_waveform_tab;
	// Holds the position of each segment of the base waveform
	st_positions base_segment_positions;
	// Holds the position of each segment of the morphed waveform
	st_positions morphed_segment_positions;
	// Holds the segments length of the base waveform
	st_segments_length base_segment_lengths;
	// Holds the segments length of the morphed waveform
	st_segments_length morphed_segment_lengths;

private:

	//static DSP_MorphingSinusOscWTAB *dsp_morphing_sinus_osc_wtab;

	int calc_segment_length(int start, int stop);
	float get_sin_segment_value(int index, int seg_start, int seg_len, en_sin_quad quadrature);

	// Holds the symetry of the morphed waveform
	int morphing_symetry;
	
	int sample_rate;
	
	int waveform_tab_len;
	int min_segment_length;
	float waveform_fundemental_frequency;
	float waveform_max_frequency;
	float wave_form_min_frequency;
};


class DSP_MorphingSinusOsc
{
public:

	friend class SynthVoice;

	DSP_MorphingSinusOsc(DSP_MorphingSinusOscWTAB *wtab_ptr,
		int i_d,
		int det_oct,
		int det_sem_ton,
		int det_cnts,
		float mag = 1.0f);

	float get_next_mso_wtab_val(float freq, int offset = 0);

	int get_id();

	void set_pwm_dcycle_set_val(int dc);
	int get_pwm_dcycle_set_val();

	void set_pwm_dcycle(int dc);
	int get_pwm_dcycle();

	void set_freq_detune_oct(int oct);
	int get_freq_detune_oct();

	void set_freq_detune_semitones(int sem);
	int get_freq_detune_semitones();

	void set_freq_detune_cents_set_value(int cnt);
	int get_freq_detune_cents_set_value();

	void set_freq_detune_cents(float cnt);
	float get_freq_detune_cents();

	void set_magnitude(float mag);
	float get_magnitde();

	float set_send_level_1(float snd);
	float set_send_level_1(int snd);
	float set_send_level_2(float snd);
	float set_send_level_2(int snd);
	float get_send_level_1();
	float get_send_level_2();
	
	DSP_MorphingSinusOscWTAB *get_wavetable();
	void set_wavetable(DSP_MorphingSinusOscWTAB *new_wtab);

  private:

	int id;
	
	float wtab_index;

	DSP_MorphingSinusOscWTAB *wtab;

	int pwm_percents_set;
	int pwm_percents;
	int detune_octave;
	int detune_semitones;
	float detune_cents;
	int detune_cents_set_value; // -15.0, -14.5 ... 0 ... +14.5, +15.0 -> 0..60 values
	bool sync_is_on;
	bool track_is_on;
	bool fix_tone_is_on;
	float harmonies_dist_factor;
	float harmonies_detune_factor;
	int unison_mode;
	float magnitude;

	// Output send level
	float send_level_1;
	float send_level_2;

};
