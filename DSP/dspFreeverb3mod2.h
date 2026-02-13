/**
*	@file		dspFreeverb3mod2.h
*	@author		Nahum Budin
*	@date		2-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	23-Jan-2021	1.1	Code refactoring and notaion. 
*				2-Nov-2019	1.0	
*
*	@brief		Reverb effect.
*	
*	(c) Copyright 2016, Sean Connelly(@ voidqk), http : //syntheti.cc
*	MIT License
*	Project Home: https://github.com/voidqk/sndfilter
*
*	reverb algorithm is based on Progenitor2 reverb effect from freeverb3:
*   http://www.nongnu.org/freeverb3/
*/

#pragma once

#include "../LibAPI/types.h"

/*
*	This API works by first initializing an sf_reverb_state_st structure, then using it to process a
*	sample in chunks.
*	
*	For example, say we're processing a stream in 128 samples per chunk:
*	
*	  sf_reverb_state_st rv;
*	  sf_presetreverb(&rv, 44100, SF_REVERB_PRESET_DEFAULT);
*	
*	  for each 128 length sample:
*	    sf_reverb_process(&rv, 128, input, output);
*	
*	Notice that sf_reverb_process will change a lot of the member variables inside of the state
*	structure, since these values must be carried over across chunk boundaries.
*	
*	Also notice that the choice to divide the sound into chunks of 128 samples is completely
*	arbitrary from the reverb's perspective
*	
*	---
*	
*	Non-convolution based reverb effects are made up from a lot of smaller effects.
*	
*	Each reverb algorithm's sound is based on how the designers setup these smaller effects and
*	chained them together.
*	
*	This particular setup is based on Progenitor2 from Freeverb3, and uses the following components:
*	   1. Delay.
*	   2. 1st order IIR filter (lowpass filter, highpass filter).
*	   3. Biquad filter (lowpass filter, all-pass filter).
*	   4. Early reflection.
*	   5. Oversampling.
*	   6. DC cut.
*	   7. Fractal noise.
*	   8. Low-frequency oscilator (LFO).
*	   9. All-pass filter.
*	  10. 2nd order All-pass filter.
*	  11. 3rd order All-pass filter with modulation.
*	  12. Modulated all-pass filter.
*	  13. Delayed feedforward comb filter.
*	
*	Each of these components is broken into their own structures (sf_rv_*), and the reverb effect
*	uses these in the final state structure (sf_reverb_state_st).
*	
*	Each component is designed to work one step at a time, so any size sample can be streamed through
*	in one pass.
*/

typedef struct 
{
	float L; // left channel sample
	float R; // right channel sample
} sf_sample_st;


// delay
// delay buffer size; maximum size allowed for a delay
#define _SF_REVERB_DS			9814
typedef struct 
{
	int pos; // current write position
	int size; // delay size
	float buf[_SF_REVERB_DS]; // delay buffer
} sf_rv_delay_st;

// 1st order IIR filter
typedef struct 
{
	float a2; // coefficients
	float b1;
	float b2;
	float y1; // state
} sf_rv_iir1_st;

// biquad
// Note: we don't use biquad.c because we want to step through the sound one sample at a time, and one
//       channel at a time.
typedef struct 
{
	float b0; // biquad coefficients
	float b1;
	float b2;
	float a1;
	float a2;
	float xn1; // input[n - 1]
	float xn2; // input[n - 2]
	float yn1; // output[n - 1]
	float yn2; // output[n - 2]
} sf_rv_biquad_st;

// early reflection
typedef struct 
{
	int				delay_tbl_L[18], delay_tbl_R[18];
	sf_rv_delay_st  delay_PW_L, delay_PW_R;
	sf_rv_delay_st  delay_RL, delay_LR;
	sf_rv_biquad_st allpass_XL, allpass_XR;
	sf_rv_biquad_st allpass_L, allpass_R;
	sf_rv_iir1_st   lpf_L, lpf_R;
	sf_rv_iir1_st   hpf_L, hpf_R;
	float wet_1, wet_2;
} sf_rv_earlyref_st;

// oversampling
// maximum oversampling factor
#define _SF_REVERB_OF						4
typedef struct 
{
	int factor; // oversampling factor [1 to SF_REVERB_OF]
	sf_rv_biquad_st lpf_U; // lowpass filter used for upsampling
	sf_rv_biquad_st lpf_D; // lowpass filter used for downsampling
} sf_rv_oversample_st;

// dc cut
typedef struct 
{
	float gain;
	float y1;
	float y2;
} sf_rv_dccut_st;

// Fractal noise cache
// Noise buffer size; must be a power of 2 because it's generated via fractal generator
#define _SF_REVERB_NS						(1<<15)
typedef struct 
{
	int pos; // current read position in the buffer
	float buf[_SF_REVERB_NS]; // buffer filled with noise
} sf_rv_noise_st;

// low-frequency oscilator (LFO)
typedef struct 
{
	float re; // real part
	float im; // imaginary part
	float sn; // sin of angle increment per sample
	float co; // cos of angle increment per sample
	int count; // number of samples generated so far (used to apply small corrections over time)
} sf_rv_lfo_st;

// All-pass filter
// Maximum size
#define _SF_REVERB_APS						6299
typedef struct 
{
	int pos;
	int size;
	float feedback;
	float decay;
	float buf[_SF_REVERB_APS];
} sf_rv_allpass_st;

// 2nd order all-pass filter
// Maximum sizes of the two buffers
#define _SF_REVERB_AP2S1					11437
#define _SF_REVERB_AP2S2					3449
typedef struct 
{
	//    line 1, line 2
	int   pos1, pos2;
	int   size1, size2;
	float feedback1, feedback2;
	float decay1, decay2;
	float buf1[_SF_REVERB_AP2S1], buf2[_SF_REVERB_AP2S2];
} sf_rv_allpass2_st;

// 3rd order all-pass filter with modulation
// Maximum sizes of the three buffers and maximum mod size of the first line.
#define _SF_REVERB_AP3S1						8171
#define _SF_REVERB_AP3M1						683
#define _SF_REVERB_AP3S2						4597
#define _SF_REVERB_AP3S3						7541
typedef struct 
{
	//    line 1,  line 1(with modulation), line 2, line 3
	int   rpos1, wpos1, pos2, pos3;
	int   size1, msize1, size2, size3;
	float feedback1, feedback2, feedback3;
	float decay1, decay2, decay3;
	float buf1[_SF_REVERB_AP3S1 + _SF_REVERB_AP3M1], buf2[_SF_REVERB_AP3S2], buf3[_SF_REVERB_AP3S3];
} sf_rv_allpass3_st;

// modulated all-pass filter
// maximum size and maximum mod size
#define _SF_REVERB_APMS						8681
#define _SF_REVERB_APMM						137
typedef struct 
{
	int rpos, wpos;
	int size, msize;
	float feedback;
	float decay;
	float z1;
	float buf[_SF_REVERB_APMS + _SF_REVERB_APMM];
} sf_rv_allpassm_st;

// Comb filter
// Maximum size of the buffer
#define _SF_REVERB_CS						4229
typedef struct 
{
	int pos;
	int size;
	float buf[_SF_REVERB_CS];
} sf_rv_comb_st;

//
// The final reverb state structure
//
// Note: this is about 2megs, so you might not want to throw these around willy-nilly
typedef struct {
	sf_rv_earlyref_st   early_ref;
	sf_rv_oversample_st oversample_L, oversample_R;
	sf_rv_dccut_st      dc_cut_L, dc_cut_R;
	sf_rv_noise_st      noise;
	sf_rv_lfo_st        lfo1;
	sf_rv_iir1_st       lfo1_lpf;
	sf_rv_allpassm_st   diff_L[10], diff_R[10];
	sf_rv_allpass_st    cross_L[4], cross_R[4];
	sf_rv_iir1_st       clpf_L, clpf_R; // cross LPF
	sf_rv_delay_st      cdelay_L, cdelay_R; // cross delay
	sf_rv_biquad_st     bass_ap_L, bass_ap_R; // bass all-pass
	sf_rv_biquad_st     bass_lp_L, bass_lp_R; // bass lowpass
	sf_rv_iir1_st       damp_lp_L, damp_lp_R; // dampening lowpass
	sf_rv_allpassm_st   damp_ap1_L, damp_ap1_R; // dampening all-pass (1)
	sf_rv_delay_st      damp_d_L, damp_d_R; // dampening delay
	sf_rv_allpassm_st   damp_ap2_L, damp_ap2_R; // dampening all-pass (2)
	sf_rv_delay_st      cbass_d1_L, cbass_d1_R; // cross-fade bass delay (1)
	sf_rv_allpass2_st   cbass_ap1_L, cbass_ap1_R; // cross-fade bass allpass (1)
	sf_rv_delay_st      cbass_d2_L, cbass_d2_R; // cross-fade bass delay (2)
	sf_rv_allpass3_st   cbass_ap2_L, cbass_ap2_R; // cross-fade bass allpass (2)
	sf_rv_lfo_st        lfo2;
	sf_rv_iir1_st       lfo2_lpf;
	sf_rv_comb_st       comb_L, comb_R;
	sf_rv_biquad_st     last_lpf_L, last_lpf_R;
	sf_rv_delay_st      last_delay_L, last_delay_R;
	sf_rv_delay_st      inp_delay_L, inp_delay_R;
	int out_co[32];
	float loop_decay;
	float wet1, wet2;
	float wander;
	float bass_b;
	float ertolate; // early reflection mix parameters
	float eref_wet;
	float dry;
} sf_reverb_state_st;

// populate a reverb state with a preset
void sf_presetreverb(sf_reverb_state_st *state, int rate, sf_reverb_preset preset);

// populate a reverb state with advanced parameters
void sf_advancereverb(
		sf_reverb_state_st *rv,
	int rate,					// input sample rate (samples per second)
	int oversamplefactor,		// how much to oversample [1 to 4]
	float ertolate,				// early reflection amount [0 to 1]
	float erefwet,				// dB, final wet mix [-70 to 10]
	float dry,					// dB, final dry mix [-70 to 10]
	float ereffactor,			// early reflection factor [0.5 to 2.5]
	float erefwidth,			// early reflection width [-1 to 1]
	float width,				// width of reverb L/R mix [0 to 1]
	float wet,					// dB, reverb wetness [-70 to 10]
	float wander,				// LFO wander amount [0.1 to 0.6]
	float bassb,				// bass boost [0 to 0.5]
	float spin,					// LFO spin amount [0 to 10]
	float inputlpf,				// Hz, lowpass cutoff for input [200 to 18000]
	float basslpf,				// Hz, lowpass cutoff for bass [50 to 1050]
	float damplpf,				// Hz, lowpass cutoff for dampening [200 to 18000]
	float outputlpf,			// Hz, lowpass cutoff for output [200 to 18000]
	float rt60,					// reverb time decay [0.1 to 30]
	float delay					// seconds, amount of delay [-0.5 to 0.5]
	);

// This function will process the input sound based on the state passed.
// The input and output buffers should be the same size
void sf_reverb_process(sf_reverb_state_st *state,
	int size,
	sf_sample_st *input,
	sf_sample_st *output);
