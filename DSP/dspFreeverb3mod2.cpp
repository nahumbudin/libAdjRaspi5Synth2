/**
*	@file		dspFreeverb3mod2.cpp
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

#include <math.h>
#include <cstring>

#include "dspFreeverb3mod2.h"

// utility functions
static inline float db_2_lin(float db)
{
	// dB to linear
	return powf(10.0f, 0.05f * db);
}

static inline int clamp_i(int v, int min, int max) 
{
	return v < min ? min : (v > max ? max : v);
}

static inline float clamp_f(float v, float min, float max) 
{
	return v < min ? min : (v > max ? max : v);
}

static bool is_prime(int v)
{
	if (v < 0)
	{
		return is_prime(-v);
	}

	if (v < 2) {
		return false;
	}
	
	if (v <= 3)
	{
		return true;
	}

	if ((v % 2) == 0 || (v % 3) == 0)
	{
		return false;
	}
	
	int max = (int)sqrt((double)v);

	for (int i = 5; i <= max; i += 6)
	{
		if ((v % i) == 0 || (v % (i + 2)) == 0)
		{
			return false;
		}
	}
	
	return true;
}

static inline int next_prime(int v)
{
	while (!is_prime(v))
	{
		v++;
	}
	
	return v;
}

// Generate a random float [0, 1] using a simple (but good quality) RNG
static inline float rand_float()
{
	static uint32_t seed = 123; // doesn't matter
	static uint32_t i = 456; // doesn't matter
	uint32_t m = 0x5bd1e995;
	uint32_t k = i++ * m;
	seed = (k ^ (k >> 24) ^ (seed * m)) * m;
	uint32_t R = (seed ^ (seed >> 13)) & 0x007FFFFF; // get 23 random bits
	union { uint32_t i; float f; } u;					// = { i = 0x3F800000 | R };
	u.i = 0x3F800000 | R;
	return u.f - 1.0;
}


// Component implementation

//	Components are in the basic format of `<component>_make` to initialize a structure and
// `<component>_step` to perform a single step with the component


// delay
static inline void delay_make(sf_rv_delay_st *delay, int size)
{
	delay->pos = 0;
	delay->size = clamp_i(size, 1, _SF_REVERB_DS);
	memset(delay->buf, 0, sizeof(float) * delay->size);
}

static inline float delay_step(sf_rv_delay_st *delay, float v)
{
	float out = delay->buf[delay->pos];
	delay->buf[delay->pos] = v;
	delay->pos = (delay->pos + 1) % delay->size;
	return out;
}

// delay_get(d, 1) returns the last written value
// delay_get(d, 2) returns the second-last written value
// ..etc
static inline float delay_get(sf_rv_delay_st *delay, int offset)
{
	if (offset > delay->size)
	{
		return delay->buf[delay->pos];
	}
	else if (offset <= 0)
	{
		offset = 1;
	}
	
	int pos = delay->pos - offset;
	if (pos < 0)
	{
		pos += delay->size;
	}

	return delay->buf[pos];
}

static inline float delay_get_last(sf_rv_delay_st *delay)
{
	return delay->buf[delay->pos];
}

// iir1
static inline void iir1_make_LPF(sf_rv_iir1_st *iir1, int rate, float freq)
{
	// 1st order IIR lowpass filter (Butterworth)
	freq = clamp_f(freq, 0, rate / 2);
	float omega2 = (float)M_PI * freq / (float)rate;
	float tano2 = tanf(omega2);
	iir1->b1 = iir1->b2 = tano2 / (1.0f + tano2);
	iir1->a2 = (1.0f - tano2) / (1.0f + tano2);
	iir1->y1 = 0;
}

static inline void iir1_makeHPF(sf_rv_iir1_st *iir1, int rate, float freq)
{
	// 1st order IIR highpass filter (Butterworth)
	freq = clamp_f(freq, 0, rate / 2);
	float omega2 = (float)M_PI * freq / (float)rate;
	float tano2 = tanf(omega2);
	iir1->b1 = 1.0f / (1.0f + tano2);
	iir1->b2 = -iir1->b1;
	iir1->a2 = (1.0f - tano2) / (1.0f + tano2);
	iir1->y1 = 0;
}

static inline float iir1_step(sf_rv_iir1_st *iir1, float v)
{
	float out = v * iir1->b1 + iir1->y1;
	iir1->y1 = out * iir1->a2 + v * iir1->b2;
	return out;
}

// biquad
static inline void biquad_make_LPF(sf_rv_biquad_st *biquad, int rate, float freq, float bw)
{
	freq = clamp_f(freq, 0, rate / 2);
	float omega = 2.0f * (float)M_PI * freq / (float)rate;
	float cs = cosf(omega);
	float sn = sinf(omega);
	float alpha = sn * sinhf((float)M_LN2 * 0.5f * bw * omega / sn);
	float a0inv = 1.0f / (1.0f + alpha);
	biquad->b0 = a0inv * (1.0f - cs) * 0.5f;
	biquad->b1 = 2.0f * biquad->b0;
	biquad->b2 = biquad->b0;
	biquad->a1 = a0inv * -2.0f * cs;
	biquad->a2 = a0inv * (1.0f - alpha);
	biquad->xn1 = 0;
	biquad->xn2 = 0;
	biquad->yn1 = 0;
	biquad->yn2 = 0;
}

static inline void biquad_make_LPF_Q(sf_rv_biquad_st *biquad, int rate, float freq, float bw)
{
	freq = clamp_f(freq, 0, rate / 2);
	float omega = 2.0f * (float)M_PI * freq / (float)rate;
	float cs = cosf(omega);
	float alpha = sinf(omega) * 2.0f * bw; // different alpha calculation than makeLPF above
	float a0inv = 1.0f / (1.0f + alpha);
	biquad->b0 = a0inv * (1.0f - cs) * 0.5f;
	biquad->b1 = 2.0f * biquad->b0;
	biquad->b2 = biquad->b0;
	biquad->a1 = a0inv * -2.0f * cs;
	biquad->a2 = a0inv * (1.0f - alpha);
	biquad->xn1 = 0;
	biquad->xn2 = 0;
	biquad->yn1 = 0;
	biquad->yn2 = 0;
}

static inline void biquad_make_APF(sf_rv_biquad_st *biquad, int rate, float freq, float bw)
{
	freq = clamp_f(freq, 0, rate / 2);
	float omega = 2.0f * (float)M_PI * freq / (float)rate;
	float sn = sinf(omega);
	float alpha = sn * sinhf((float)M_LN2 * 0.5f * bw * omega / sn);
	float a0inv = 1.0f / (1.0f + alpha);
	biquad->b0 = a0inv * (1.0f - alpha);
	biquad->b1 = a0inv * -2.0f * cosf(omega);
	biquad->b2 = a0inv * (1.0f + alpha);
	biquad->a1 = biquad->b1;
	biquad->a2 = biquad->b0;
	biquad->xn1 = 0;
	biquad->xn2 = 0;
	biquad->yn1 = 0;
	biquad->yn2 = 0;
}

static inline float biquad_step(sf_rv_biquad_st *biquad, float v)
{
	float out = v * biquad->b0 + biquad->xn1 * biquad->b1 + biquad->xn2 * biquad->b2 -
				biquad->yn1 * biquad->a1 - biquad->yn2 * biquad->a2;
	biquad->xn2 = biquad->xn1;
	biquad->xn1 = v;
	biquad->yn2 = biquad->yn1;
	biquad->yn1 = out;
	return out;
}


// earlyref
static inline void early_ref_make(sf_rv_earlyref_st *early_ref, int rate, float factor, float width)
{
	static const sf_sample_st delay_tbl[18] = 
	{
		// seconds to look backwards
		{ 0.0043f, 0.0053f },
		{ 0.0215f, 0.0225f },
		{ 0.0225f, 0.0235f },
		{ 0.0268f, 0.0278f },
		{ 0.0270f, 0.0290f },
		{ 0.0298f, 0.0288f },
		{ 0.0458f, 0.0468f },
		{ 0.0485f, 0.0475f },
		{ 0.0572f, 0.0582f },
		{ 0.0587f, 0.0577f },
		{ 0.0595f, 0.0575f },
		{ 0.0612f, 0.0622f },
		{ 0.0707f, 0.0697f },
		{ 0.0708f, 0.0718f },
		{ 0.0726f, 0.0736f },
		{ 0.0741f, 0.0751f },
		{ 0.0753f, 0.0763f },
		{ 0.0797f, 0.0817f }
	};

	early_ref->wet_1 = width * 0.5f + 0.5f;
	early_ref->wet_2 = (1.0f - width) * 0.5f;

	int lr_delay = 0.0002f * (float)rate;
	delay_make(&early_ref->delay_RL, lr_delay);
	delay_make(&early_ref->delay_LR, lr_delay);

	biquad_make_APF(&early_ref->allpass_XL, rate, 740.0f, 4.0f);
	early_ref->allpass_XR = early_ref->allpass_XL;

	biquad_make_APF(&early_ref->allpass_L, rate, 150.0f, 4.0f);
	early_ref->allpass_R = early_ref->allpass_L;

	factor *= rate;
	for (int i = 0; i < 18; i++)
	{
		early_ref->delay_tbl_L[i] = delay_tbl[i].L * factor;
		early_ref->delay_tbl_R[i] = delay_tbl[i].R * factor;
	}

	delay_make(&early_ref->delay_PW_L, early_ref->delay_tbl_L[17] + 10);
	delay_make(&early_ref->delay_PW_R, early_ref->delay_tbl_R[17] + 10);

	iir1_make_LPF(&early_ref->lpf_L, rate, 20000.0f);
	early_ref->lpf_R = early_ref->lpf_L;

	iir1_makeHPF(&early_ref->hpf_L, rate, 4.0f);
	early_ref->hpf_R = early_ref->hpf_L;
}

static inline sf_sample_st earlyref_step(sf_rv_earlyref_st *earlyref, sf_sample_st input)
{
	static const sf_sample_st gaintbl[18] = 
	{
		{ 0.841f, 0.842f },
		{ 0.504f, 0.506f },
		{ 0.491f, 0.489f },
		{ 0.379f, 0.382f },
		{ 0.380f, 0.300f },
		{ 0.346f, 0.346f },
		{ 0.289f, 0.290f },
		{ 0.272f, 0.271f },
		{ 0.192f, 0.193f },
		{ 0.193f, 0.192f },
		{ 0.217f, 0.217f },
		{ 0.181f, 0.195f },
		{ 0.180f, 0.192f },
		{ 0.181f, 0.166f },
		{ 0.176f, 0.186f },
		{ 0.142f, 0.131f },
		{ 0.167f, 0.168f },
		{ 0.134f, 0.133f }
	};

	float wetL = 0, wetR = 0;
	delay_step(&earlyref->delay_PW_L, input.L);
	delay_step(&earlyref->delay_PW_R, input.R);

	for (int i = 0; i < 18; i++)
	{
		wetL += gaintbl[i].L * delay_get(&earlyref->delay_PW_L, earlyref->delay_tbl_L[i]);
		wetR += gaintbl[i].R * delay_get(&earlyref->delay_PW_R, earlyref->delay_tbl_R[i]);
	}

	float L = delay_step(&earlyref->delay_RL, input.R + wetR);
	L = biquad_step(&earlyref->allpass_XL, L);
	L = biquad_step(&earlyref->allpass_L, earlyref->wet_1 * wetL + earlyref->wet_2 * L);
	L = iir1_step(&earlyref->hpf_L, L);
	L = iir1_step(&earlyref->lpf_L, L);

	float R = delay_step(&earlyref->delay_LR, input.L + wetL);
	R = biquad_step(&earlyref->allpass_XR, R);
	R = biquad_step(&earlyref->allpass_R, earlyref->wet_1 * wetR + earlyref->wet_2 * R);
	R = iir1_step(&earlyref->hpf_R, R);
	R = iir1_step(&earlyref->lpf_R, R);

	sf_sample_st out;
	out.L = L;
	out.R = R;
	return out;
}

//
// oversample
//
static inline void oversample_make(sf_rv_oversample_st *oversample, int factor)
{
	oversample->factor = clamp_i(factor, 1, _SF_REVERB_OF);
	biquad_make_LPF_Q(&oversample->lpf_U,
		2 * oversample->factor,
		1.0f,
		0.5773502691896258f); // 1/sqrt(3)
	oversample->lpf_D = oversample->lpf_U;
}

// output length must be oversample->factor
static inline void oversample_stepup(sf_rv_oversample_st *over_sample, float input, float *output)
{
	if (over_sample->factor == 1)
	{
		output[0] = input;
		return;
	}

	output[0] = biquad_step(&over_sample->lpf_U, input * over_sample->factor);
	for (int i = 1; i < over_sample->factor; i++)
	{
		output[i] = biquad_step(&over_sample->lpf_U, 0);
	}
}

// input length must be oversample->factor
static inline float oversample_stepdown(sf_rv_oversample_st *oversample, float *input)
{
	if (oversample->factor == 1)
	{
		return input[0];
	}
	
	for (int i = 0; i < oversample->factor; i++)
	{
		biquad_step(&oversample->lpf_D, input[i]);
	}

	return input[0];
}

// dccut
static inline void dccut_make(sf_rv_dccut_st *dc_cut, int rate, float freq)
{
	freq = clamp_f(freq, 0, rate / 2);
	float ang = 2.0f * (float)M_PI * freq / (float)rate;
	float sn = sinf(ang);
	float sqrt3 = 1.7320508075688772f;
	dc_cut->gain = (sqrt3 - 2.0f * sn) / (sn + sqrt3 * cosf(ang));
	dc_cut->y1 = 0;
	dc_cut->y2 = 0;
}

static inline float dccut_step(sf_rv_dccut_st *dc_cut, float v)
{
	float out = v - dc_cut->y1 + dc_cut->gain * dc_cut->y2;
	dc_cut->y1 = v;
	dc_cut->y2 = out;
	return out;
}

// noise
static inline void noise_make(sf_rv_noise_st *noise)
{
	noise->pos = _SF_REVERB_NS;
}

static inline float noise_step(sf_rv_noise_st *noise)
{
	if (noise->pos >= _SF_REVERB_NS)
	{
		// need to generate more noise
		noise->pos = 0;
		int len = _SF_REVERB_NS;
		int tot = 1;
		float r = 0.8f;
		float rmul = 0.7071067811865475f; // 1/sqrt(2)
		noise->buf[0] = 0;
		while (len > 1)
		{
			float left = 0;
			for (int i = tot - 1; i >= 0; i--)
			{
				float right = left;
				left = noise->buf[i * len];
				float midpoint = (left + right) * 0.5f;
				float newv = midpoint + r * (2.0f * rand_float() - 1.0f); // displace by random amt
				noise->buf[i * len + (len / 2)] = clamp_f(newv, -1.0f, 1.0f);
			}
			len /= 2;
			tot *= 2;
			r *= rmul;
		}
	}

	return noise->buf[noise->pos++];
}

// lfo
static inline void lfo_make(sf_rv_lfo_st *lfo, int rate, float freq)
{
	lfo->count = 0;
	lfo->re = 1.0f;
	lfo->im = 0.0f;
	float theta = 2.0f * (float)M_PI * freq / (float)rate;
	lfo->sn = sinf(theta);
	lfo->co = cosf(theta);
}

static inline float lfo_step(sf_rv_lfo_st *lfo)
{
	float v = lfo->im;
	float re = lfo->re * lfo->co - lfo->im * lfo->sn;
	float im = lfo->re * lfo->sn + lfo->im * lfo->co;
	if (lfo->count++ > 100000)
	{
		// if we've gathered a lot of samples, then it's probably a good idea to make sure our LFO
		// hasn't accumulated a bunch of errors
		lfo->count = 0;
		float leninv = 1.0f / sqrtf(re * re + im * im);
		re *= leninv;
		im *= leninv;
	}

	lfo->re = re;
	lfo->im = im;

	return v;
}

// allpass
static inline void allpass_make(sf_rv_allpass_st *allpass, int size, float feedback, float decay)
{
	allpass->pos = 0;
	allpass->size = clamp_i(size, 1, _SF_REVERB_APS);
	allpass->feedback = feedback;
	allpass->decay = decay;
	memset(allpass->buf, 0, sizeof(float) * allpass->size);
}

static inline float allpass_step(sf_rv_allpass_st *allpass, float v)
{
	v += allpass->feedback * allpass->buf[allpass->pos];
	float out = allpass->decay * allpass->buf[allpass->pos] - allpass->feedback * v;
	allpass->buf[allpass->pos] = v;
	allpass->pos = (allpass->pos + 1) % allpass->size;
	return out;
}

// allpass2
static inline void allpass2_make(sf_rv_allpass2_st *allpass2,
	int size1,
	int size2,
	float feedback1,
	float feedback2,
	float decay1,
	float decay2)
{
	allpass2->pos1 = 0;
	allpass2->pos2 = 0;
	allpass2->size1 = clamp_i(size1, 1, _SF_REVERB_AP2S1);
	allpass2->size2 = clamp_i(size2, 1, _SF_REVERB_AP2S2);
	allpass2->feedback1 = feedback1;
	allpass2->feedback2 = feedback2;
	allpass2->decay1 = decay1;
	allpass2->decay2 = decay2;
	memset(allpass2->buf1, 0, sizeof(float) * allpass2->size1);
	memset(allpass2->buf2, 0, sizeof(float) * allpass2->size2);
}

static inline float allpass2_step(sf_rv_allpass2_st *allpass2, float v)
{
	v += allpass2->feedback2 * allpass2->buf2[allpass2->pos2];
	float out = allpass2->decay2 * allpass2->buf2[allpass2->pos2] - v * allpass2->feedback2;
	v += allpass2->feedback1 * allpass2->buf1[allpass2->pos1];
	allpass2->buf2[allpass2->pos2] = allpass2->decay1 * allpass2->buf1[allpass2->pos1] -
		v * allpass2->feedback1;
	allpass2->buf1[allpass2->pos1] = v;
	allpass2->pos1 = (allpass2->pos1 + 1) % allpass2->size1;
	allpass2->pos2 = (allpass2->pos2 + 1) % allpass2->size2;
	return out;
}

static inline float allpass2_get1(sf_rv_allpass2_st *allpass2, int offset)
{
	if (offset > allpass2->size1)
	{
		return allpass2->buf1[allpass2->pos1];
	}
	else if (offset <= 0)
	{
		offset = 1;
	}
	
	int rp = allpass2->pos1 - offset;
	if (rp < 0)
	{
		rp += allpass2->size1;
	}

	return allpass2->buf1[rp];
}

static inline float allpass2_get2(sf_rv_allpass2_st *allpass2, int offset)
{
	if (offset > allpass2->size2)
	{
		return allpass2->buf2[allpass2->pos2];
	}
	else if (offset <= 0)
	{
		offset = 1;
	}
	
	int rp = allpass2->pos2 - offset;
	if (rp < 0)
	{
		rp += allpass2->size2;
	}
	
	return allpass2->buf2[rp];
}

// allpass3
static inline void allpass3_make(sf_rv_allpass3_st *allpass3,
	int size1,
	int msize1,
	int size2,
	int size3,
	float feedback1,
	float feedback2,
	float feedback3, 
	float decay1,
	float decay2,
	float decay3)
{
	size1 = clamp_i(size1, 1, _SF_REVERB_AP3S1);
	msize1 = clamp_i(msize1, 1, _SF_REVERB_AP3M1);
	if (msize1 > size1)
	{
		msize1 = size1;
	}

	int newsize = size1 + msize1;
	allpass3->rpos1 = (msize1 * 2) % newsize;
	allpass3->wpos1 = 0;
	allpass3->pos2 = 0;
	allpass3->pos3 = 0;
	allpass3->size1 = newsize;
	allpass3->msize1 = msize1;
	allpass3->size2 = clamp_i(size2, 1, _SF_REVERB_AP3S2);
	allpass3->size3 = clamp_i(size3, 1, _SF_REVERB_AP3S3);
	allpass3->feedback1 = feedback1;
	allpass3->feedback2 = feedback2;
	allpass3->feedback3 = feedback3;
	allpass3->decay1 = decay1;
	allpass3->decay2 = decay2;
	allpass3->decay3 = decay3;
	memset(allpass3->buf1, 0, sizeof(float) * allpass3->size1);
	memset(allpass3->buf2, 0, sizeof(float) * allpass3->size2);
	memset(allpass3->buf3, 0, sizeof(float) * allpass3->size3);
}

static inline float allpass3_step(sf_rv_allpass3_st *allpass3, float v, float mod)
{
	mod = (mod + 1.0f) * (float)allpass3->msize1;
	float floormod = floorf(mod);
	float mfrac = mod - floormod;
	int rpos1 = allpass3->rpos1 - (int)floormod;
	if (rpos1 < 0)
	{
		rpos1 += allpass3->size1;
	}

	int rpos2 = rpos1 - 1;
	if (rpos2 < 0)
	{
		rpos2 += allpass3->size1;
	}

	v += allpass3->feedback3 * allpass3->buf3[allpass3->pos3];
	float out = allpass3->decay3 * allpass3->buf3[allpass3->pos3] - allpass3->feedback3 * v;
	v += allpass3->feedback2 * allpass3->buf2[allpass3->pos2];
	allpass3->buf3[allpass3->pos3] = allpass3->decay2 * allpass3->buf2[allpass3->pos2] -
		allpass3->feedback2 * v;
	float tmp = allpass3->buf1[rpos2] * mfrac + allpass3->buf1[rpos1] * (1.0f - mfrac);
	v += allpass3->feedback1 * tmp;
	allpass3->buf2[allpass3->pos2] = allpass3->decay1 * tmp - allpass3->feedback1 * v;
	allpass3->buf1[allpass3->wpos1] = v;
	allpass3->wpos1 = (allpass3->wpos1 + 1) % allpass3->size1;
	allpass3->rpos1 = (allpass3->rpos1 + 1) % allpass3->size1;
	allpass3->pos2 = (allpass3->pos2 + 1) % allpass3->size2;
	allpass3->pos3 = (allpass3->pos3 + 1) % allpass3->size3;
	
	return out;
}

static inline float allpass3_get1(sf_rv_allpass3_st *allpass3, int offset)
{
	if (offset > allpass3->size1)
	{
		return allpass3->buf1[allpass3->rpos1];
	}
	else if (offset <= 0)
	{
		offset = 1;
	}

	int rp = allpass3->rpos1 - offset;
	if (rp < 0)
	{
		rp += allpass3->size1;
	}

	return allpass3->buf1[rp];
}

static inline float allpass3_get2(sf_rv_allpass3_st *allpass3, int offset)
{
	if (offset > allpass3->size2)
	{
		return allpass3->buf2[allpass3->pos2];
	}
	else if (offset <= 0)
	{
		offset = 1;
	}
	
	int rp = allpass3->pos2 - offset;
	if (rp < 0)
	{
		rp += allpass3->size2;
	}

	return allpass3->buf2[rp];
}

static inline float allpass3_get3(sf_rv_allpass3_st *allpass3, int offset)
{
	if (offset > allpass3->size3)
	{
		return allpass3->buf3[allpass3->pos3];
	}
	else if (offset <= 0)
	{
		offset = 1;
	}

	int rp = allpass3->pos3 - offset;
	if (rp < 0)
	{
		rp += allpass3->size3;
	}

	return allpass3->buf3[rp];
}

// allpassm
static inline void allpassm_make(sf_rv_allpassm_st *allpassm,
	int size,
	int msize,
	float feedback,
	float decay)
{
	size = clamp_i(size, 1, _SF_REVERB_APMS);
	msize = clamp_i(msize, 1, _SF_REVERB_APMM);
	if (msize > size)
	{
		msize = size;
	}

	int newsize = size + msize;
	allpassm->rpos = (msize * 2) % newsize;
	allpassm->wpos = 0;
	allpassm->size = newsize;
	allpassm->msize = msize;
	allpassm->feedback = feedback;
	allpassm->decay = decay;
	allpassm->z1 = 0;
	memset(allpassm->buf, 0, sizeof(float) * allpassm->size);
}

static inline float allpassm_step(sf_rv_allpassm_st *allpassm, float v, float mod, float fbmod)
{
	float mfeedback = allpassm->feedback + fbmod;
	mod = (mod + 1.0f) * (float)allpassm->msize;
	float floormod = floorf(mod);
	float mfrac = 1.0f - mod + floormod;
	int rpos1 = allpassm->rpos - (int)floormod;
	if (rpos1 < 0)
	{
		rpos1 += allpassm->size;
	}

	int rpos2 = rpos1 - 1;
	if (rpos2 < 0)
	{
		rpos2 += allpassm->size;
	}

	allpassm->z1 = allpassm->buf[rpos2] + mfrac * (allpassm->buf[rpos1] - allpassm->z1);
	allpassm->rpos = (allpassm->rpos + 1) % allpassm->size;
	allpassm->buf[allpassm->wpos] = v + allpassm->z1 * mfeedback;
	v = allpassm->decay * allpassm->z1 - allpassm->buf[allpassm->wpos] * mfeedback;
	allpassm->wpos = (allpassm->wpos + 1) % allpassm->size;

	return v;
}

// comb
static inline void comb_make(sf_rv_comb_st *comb, int size)
{
	comb->pos = 0;
	comb->size = clamp_i(size, 1, _SF_REVERB_CS);
	memset(comb->buf, 0, sizeof(float) * comb->size);
}

static inline float comb_step(sf_rv_comb_st *comb, float v, float feedback)
{
	v = comb->buf[comb->pos] * feedback + v;
	comb->buf[comb->pos] = v;
	comb->pos = (comb->pos + 1) % comb->size;
	return v;
}

// reverb implementation

// now that all the components are done (thank god), we can start on the actual reverb effect

void sf_presetreverb(sf_reverb_state_st *rv, int rate, sf_reverb_preset preset)
{
	// sorry for the bad formatting, I've tried to cram this in as best as I could
	struct 
	{
		int osf; float p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16;
	} ps[] = 
	{

		//OSF ERtoLt ERWet Dry ERFac ERWdth Wdth Wet Wander BassB Spin InpLP BasLP DmpLP OutLP RT60  Delay
		{ /*1*/1, 0.40f, -9.0f, -10, 1.6f, 0.7f, 1.0f, -0, 0.27f, 0.15f, 0.7f, 17000, 500, 7000, 10000, 3.2f, 0.020f },
		{ /*2*/1, 0.30f, -9.0f, -8, 1.0f, 0.7f, 1.0f, -8, 0.32f, 0.25f, 0.7f, 18000, 600, 9000, 17000, 2.1f, 0.010f },
		{ /*1*/1, 0.30f, -9.0f, -8, 1.0f, 0.7f, 1.0f, -8, 0.27f, 0.20f, 0.5f, 18000, 600, 7000, 9000, 2.3f, 0.010f },
		{ /*2*/1, 0.30f, -9.0f, -8, 1.2f, 0.7f, 1.0f, -8, 0.27f, 0.20f, 0.7f, 18000, 500, 8000, 16000, 2.8f, 0.010f },
		{ /*1*/1, 0.30f, -9.0f, -8, 1.2f, 0.7f, 1.0f, -8, 0.25f, 0.15f, 0.5f, 18000, 500, 6000, 8000, 2.9f, 0.010f },
		{ /*2*/1, 0.20f, -9.0f, -8, 1.4f, 0.7f, 1.0f, -8, 0.17f, 0.20f, 1.0f, 18000, 400, 9000, 14000, 3.8f, 0.018f },
		{ /*2*/1, 0.20f, -9.0f, -8, 1.5f, 0.7f, 1.0f, -8, 0.20f, 0.20f, 0.5f, 18000, 400, 5000, 7000, 4.2f, 0.018f },
		{ /*2*/1, 0.70f, -8.0f, -8, 0.7f, -0.4f, 0.8f, -8, 0.20f, 0.30f, 1.6f, 18000, 1000, 18000, 18000, 0.5f, 0.005f },
		{ /*3*/1, 0.70f, -8.0f, -8, 0.8f, 0.6f, 0.9f, -8, 0.30f, 0.30f, 0.4f, 18000, 300, 10000, 18000, 0.5f, 0.005f },
		{ /*2*/1, 0.50f, -8.0f, -8, 1.2f, -0.4f, 0.8f, -8, 0.20f, 0.10f, 1.6f, 18000, 1000, 18000, 18000, 0.8f, 0.008f },
		{ /*2*/1, 0.50f, -8.0f, -8, 1.2f, 0.6f, 0.9f, -8, 0.30f, 0.10f, 0.4f, 18000, 300, 10000, 18000, 1.2f, 0.016f },
		{ /*2*/1, 0.20f, -8.0f, -8, 2.2f, -0.4f, 0.9f, -8, 0.20f, 0.10f, 1.6f, 18000, 1000, 16000, 18000, 1.8f, 0.010f },
		{ /*2*/1, 0.20f, -8.0f, -8, 2.2f, 0.6f, 0.9f, -8, 0.30f, 0.10f, 0.4f, 18000, 500, 9000, 18000, 1.9f, 0.020f },
		{ /*2*/1, 0.50f, -7.0f, -7, 1.2f, -0.4f, 0.8f, -70, 0.20f, 0.10f, 1.6f, 18000, 1000, 18000, 18000, 0.8f, 0.008f },
		{ /*2*/1, 0.50f, -7.0f, -7, 1.2f, 0.6f, 0.9f, -70, 0.30f, 0.10f, 0.4f, 18000, 300, 10000, 18000, 1.2f, 0.016f },
		{ /*2*/1, 0.00f, -70.0f, -20, 1.0f, 1.0f, 1.0f, -8, 0.20f, 0.10f, 1.6f, 18000, 1000, 16000, 18000, 1.8f, 0.000f },
		{ /*2*/1, 0.00f, -70.0f, -20, 1.0f, 1.0f, 1.0f, -8, 0.30f, 0.20f, 0.4f, 18000, 500, 9000, 18000, 1.9f, 0.000f },
		{ /*2*/1, 0.10f, -16.0f, -15, 1.0f, 0.1f, 1.0f, -5, 0.35f, 0.05f, 1.0f, 18000, 100, 10000, 18000, 12.0f, 0.000f },
		{ /*2*/1, 0.10f, -16.0f, -15, 1.0f, 0.1f, 1.0f, -5, 0.40f, 0.05f, 1.0f, 18000, 100, 9000, 18000, 30.0f, 0.000f }

	};

#define CASE(prs, i)                                                                        \
		case prs: sf_advancereverb(rv, rate, ps[i].osf, ps[i].p1, ps[i].p2, ps[i].p3, ps[i].p4, \
			ps[i].p5, ps[i].p6, ps[i].p7, ps[i].p8, ps[i].p9, ps[i].p10, ps[i].p11, ps[i].p12,  \
			ps[i].p13, ps[i].p14, ps[i].p15, ps[i].p16); return;
	switch (preset) {
		CASE(_SF_REVERB_PRESET_DEFAULT, 0)
		CASE(_SF_REVERB_PRESET_SMALLHALL1, 1)
		CASE(_SF_REVERB_PRESET_SMALLHALL2, 2)
		CASE(_SF_REVERB_PRESET_MEDIUMHALL1, 3)
		CASE(_SF_REVERB_PRESET_MEDIUMHALL2, 4)
		CASE(_SF_REVERB_PRESET_LARGEHALL1, 5)
		CASE(_SF_REVERB_PRESET_LARGEHALL2, 6)
		CASE(_SF_REVERB_PRESET_SMALLROOM1, 7)
		CASE(_SF_REVERB_PRESET_SMALLROOM2, 8)
		CASE(_SF_REVERB_PRESET_MEDIUMROOM1, 9)
		CASE(_SF_REVERB_PRESET_MEDIUMROOM2, 10)
		CASE(_SF_REVERB_PRESET_LARGEROOM1, 11)
		CASE(_SF_REVERB_PRESET_LARGEROOM2, 12)
		CASE(_SF_REVERB_PRESET_MEDIUMER1, 13)
		CASE(_SF_REVERB_PRESET_MEDIUMER2, 14)
		CASE(_SF_REVERB_PRESET_PLATEHIGH, 15)
		CASE(_SF_REVERB_PRESET_PLATELOW, 16)
		CASE(_SF_REVERB_PRESET_LONGREVERB1, 17)
		CASE(_SF_REVERB_PRESET_LONGREVERB2, 18)
	}
#undef CASE
}

void sf_advancereverb(sf_reverb_state_st *rv,
	int rate,
	int oversample_factor,
	float ertolate,
	float eref_wet,
	float dry,
	float eref_factor,
	float eref_width,
	float width,
	float wet,
	float wander,
	float bassb,
	float spin,
	float input_lpf,
	float bass_lpf,
	float damp_lpf,
	float output_lpf,
	float rt_60,
	float delay)
{
	rv->ertolate = ertolate;
	rv->eref_wet = db_2_lin(eref_wet);
	rv->dry = db_2_lin(dry);
	wet = db_2_lin(wet);
	rv->wet1 = wet * (width * 0.5f + 0.5f);
	rv->wet2 = wet * ((1.0f - width) * 0.5f);
	rv->wander = wander;
	rv->bass_b = bassb;

	early_ref_make(&rv->early_ref, rate, eref_factor, eref_width);

	oversample_make(&rv->oversample_L, oversample_factor);
	rv->oversample_R = rv->oversample_L;
	int osrate = rate * rv->oversample_L.factor;

	dccut_make(&rv->dc_cut_L, osrate, 5.0f);
	rv->dc_cut_R = rv->dc_cut_L;

	noise_make(&rv->noise);

	lfo_make(&rv->lfo1, osrate, spin);
	iir1_make_LPF(&rv->lfo1_lpf, osrate, 20.0f);
	lfo_make(&rv->lfo2, osrate, sqrtf(100.0f - (10.0f - spin) * (10.0f - spin)) * 0.5f);
	iir1_make_LPF(&rv->lfo2_lpf, osrate, 12.0f);

	static const int diffLc[10] = { 617, 535, 434, 347, 218, 162, 144, 122, 109, 74 };
	static const int diffRc[10] = { 603, 547, 416, 364, 236, 162, 140, 131, 111, 79 };
	int totfactor = osrate / 34125;
	int msize = next_prime(10 * osrate / 34125);
	for (int i = 0; i < 10; i++)
	{
		allpassm_make(&rv->diff_L[i], next_prime(diffLc[i] * totfactor), msize, -0.78f, 1);
		allpassm_make(&rv->diff_R[i], next_prime(diffRc[i] * totfactor), msize, -0.78f, 1);
	}

	static const int crossLc[4] = { 430, 341, 264, 174 };
	static const int crossRc[4] = { 447, 324, 247, 191 };
	for (int i = 0; i < 4; i++)
	{
		allpass_make(&rv->cross_L[i], next_prime(crossLc[i] * totfactor), 0.78f, 1);
		allpass_make(&rv->cross_R[i], next_prime(crossRc[i] * totfactor), 0.78f, 1);
	}

	iir1_make_LPF(&rv->clpf_L, osrate, input_lpf);
	rv->clpf_R = rv->clpf_L;

	delay_make(&rv->cdelay_L, next_prime(1572 * totfactor));
	delay_make(&rv->cdelay_R, next_prime(16 * totfactor));
	delay_make(&rv->damp_d_L, next_prime(2 * totfactor));
	delay_make(&rv->damp_d_R, next_prime(totfactor));
	delay_make(&rv->cbass_d1_L, next_prime(1055 * totfactor));
	delay_make(&rv->cbass_d1_R, next_prime(1460 * totfactor));
	delay_make(&rv->cbass_d2_L, next_prime(344 * totfactor));
	delay_make(&rv->cbass_d2_R, next_prime(500 * totfactor));

	biquad_make_APF(&rv->bass_ap_L, osrate, 150.0f, 4.0f);
	rv->bass_ap_R = rv->bass_ap_L;

	biquad_make_LPF(&rv->bass_lp_L, osrate, bass_lpf, 2.0f);
	rv->bass_lp_R = rv->bass_lp_L;

	iir1_make_LPF(&rv->damp_lp_L, osrate, damp_lpf);
	rv->damp_lp_R = rv->damp_lp_L;

	float decay0 = powf(10.0f, log10f(0.237f) / rt_60);
	float decay1 = powf(10.0f, log10f(0.938f) / rt_60);
	float decay2 = powf(10.0f, log10f(0.844f) / rt_60);
	float decay3 = powf(10.0f, log10f(0.906f) / rt_60);
	rv->loop_decay = decay0;
	msize = next_prime(32 * totfactor);
	allpassm_make(&rv->damp_ap1_L, next_prime(239 * totfactor), msize, 0.375f, decay2);
	allpassm_make(&rv->damp_ap1_R, next_prime(205 * totfactor), msize, 0.375f, decay2);
	allpassm_make(&rv->damp_ap2_L, next_prime(392 * totfactor), msize, 0.312f, decay3);
	allpassm_make(&rv->damp_ap2_R, next_prime(329 * totfactor), msize, 0.312f, decay3);

	allpass2_make(&rv->cbass_ap1_L,
		next_prime(1944 * totfactor),
		next_prime(612 * totfactor),
		0.250f,
		0.406f,
		decay1,
		decay2);
	allpass2_make(&rv->cbass_ap1_R,
		next_prime(2032 * totfactor),
		next_prime(368 * totfactor),
		0.250f,
		0.406f,
		decay1,
		decay2);

	allpass3_make(&rv->cbass_ap2_L,
		next_prime(1212 * totfactor),
		next_prime(121 * totfactor),
		next_prime(816 * totfactor),
		next_prime(1264 * totfactor),
		0.250f,
		0.250f,
		0.406f,
		decay1,
		decay1,
		decay2);
	allpass3_make(&rv->cbass_ap2_R,
		next_prime(1452 * totfactor),
		next_prime(5 * totfactor),
		next_prime(688 * totfactor),
		next_prime(1340 * totfactor),
		0.250f,
		0.250f,
		0.406f,
		decay1,
		decay1,
		decay2);

	static const int outco[32] = 
	{
		1,
		40,
		192,
		276,
		321,
		110,
		468,
		1572,
		121,
		480,
		103,
		26,
		780,
		1200,
		310,
		780,
		625,
		468,
		312,
		24,
		36,
		790,
		189,
		8,
		10,
		359,
		30,
		10,
		109,
		1310,
		800,
		10
	};
	for (int i = 0; i < 32; i++)
	{
		rv->out_co[i] = outco[i] * totfactor;
	}

	comb_make(&rv->comb_L, next_prime(22 * osrate / 1000));
	rv->comb_R = rv->comb_L;

	biquad_make_LPF(&rv->last_lpf_L, osrate, output_lpf, 1.0f);
	rv->last_lpf_R = rv->last_lpf_L;

	int delaysamp = osrate * delay;
	if (delaysamp >= 0)
	{
		delay_make(&rv->inp_delay_L, 0);
		delay_make(&rv->inp_delay_R, 0);
		delay_make(&rv->last_delay_L, delaysamp);
		delay_make(&rv->last_delay_R, delaysamp);
	}
	else
	{
		delay_make(&rv->inp_delay_L, -delaysamp);
		delay_make(&rv->inp_delay_R, -delaysamp);
		delay_make(&rv->last_delay_L, 0);
		delay_make(&rv->last_delay_R, 0);
	}
}

void sf_reverb_process(sf_reverb_state_st *rv, int size, sf_sample_st *input, sf_sample_st *output)
{
	// extra hardcoded constants
	int i2;
	const float mod_noise1 = 0.09f;
	const float mod_noise2 = 0.06f;
	const float cross_feed = 0.4f;

	// oversample buffer
	float osL[_SF_REVERB_OF], osR[_SF_REVERB_OF];
	
	for (int i = 0; i < size; i++)
	{
		// early reflection
		sf_sample_st er = earlyref_step(&rv->early_ref, input[i]);
		float erL = er.L * rv->ertolate + input[i].L;
		float erR = er.R * rv->ertolate + input[i].R;
	
		// oversample the single input into multiple outputs
		oversample_stepup(&rv->oversample_L, erL, osL);
		oversample_stepup(&rv->oversample_R, erR, osR);
		
		for (i2 = 0; i2 < rv->oversample_L.factor; i2++) 
		{
			// for each oversampled sample...
			// dc cut
			float out_L = dccut_step(&rv->dc_cut_L, osL[i2]);
			float out_R = dccut_step(&rv->dc_cut_R, osR[i2]);

			// noise
			float mnoise = noise_step(&rv->noise);
			float lfo = (lfo_step(&rv->lfo1) + mod_noise1 * mnoise) * rv->wander;
			lfo = iir1_step(&rv->lfo1_lpf, lfo);
			mnoise *= mod_noise2;

			// diffusion
			for (int i = 0, s = -1; i < 10; i++, s = -s) {
				out_L = allpassm_step(&rv->diff_L[i], out_L, lfo * s, mnoise);
				out_R = allpassm_step(&rv->diff_R[i], out_R, lfo, mnoise * s);
			}

			// cross fade
			float cross_L = out_L, cross_R = out_R;
			for (int i = 0; i < 4; i++) 
			{
				cross_L = allpass_step(&rv->cross_L[i], cross_L);
				cross_R = allpass_step(&rv->cross_R[i], cross_R);
			}

			out_L = iir1_step(&rv->clpf_L, out_L + cross_feed * cross_R);
			out_R = iir1_step(&rv->clpf_R, out_R + cross_feed * cross_L);

			// bass boost
			cross_L = delay_get_last(&rv->cdelay_L);
			cross_R = delay_get_last(&rv->cdelay_R);
			out_L += rv->loop_decay *
				(cross_R + rv->bass_b * biquad_step(&rv->bass_lp_L, biquad_step(&rv->bass_ap_L, cross_R)));
			out_R += rv->loop_decay *
				(cross_L + rv->bass_b * biquad_step(&rv->bass_lp_R, biquad_step(&rv->bass_ap_R, cross_L)));

			// dampening
			out_L = allpassm_step(&rv->damp_ap2_L,
				delay_step(&rv->damp_d_L,
					allpassm_step(&rv->damp_ap1_L,
						iir1_step(&rv->damp_lp_L, out_L),
						lfo,
						mnoise)),
				-lfo,
				-mnoise);

			out_R = allpassm_step(&rv->damp_ap2_R,
				delay_step(&rv->damp_d_R,
					allpassm_step(&rv->damp_ap1_R,
						iir1_step(&rv->damp_lp_R, out_R),
						-lfo,
						-mnoise)),
				lfo,
				mnoise);

			// update cross fade bass boost delay
			delay_step(&rv->cdelay_L,
				allpass3_step(&rv->cbass_ap2_L,
					delay_step(&rv->cbass_d2_L,
						allpass2_step(&rv->cbass_ap1_L,
							delay_step(&rv->cbass_d1_L, out_L))),
					lfo));
			delay_step(&rv->cdelay_R,
				allpass3_step(&rv->cbass_ap2_R,
					delay_step(&rv->cbass_d2_R,
						allpass2_step(&rv->cbass_ap1_R,
							delay_step(&rv->cbass_d1_R, out_R))),
					-lfo));

			float D1 = 
				delay_get(&rv->cbass_d1_L, rv->out_co[0]);
			float D2 =
				delay_get(&rv->cbass_d2_L, rv->out_co[1]) -
				delay_get(&rv->cbass_d2_R, rv->out_co[2]) +
				delay_get(&rv->cbass_d2_L, rv->out_co[3]) -
				delay_get(&rv->cdelay_R, rv->out_co[4]) -
				delay_get(&rv->cbass_d1_R, rv->out_co[5]) -
				delay_get(&rv->cbass_d2_R, rv->out_co[6]);
			float D3 =
				delay_get(&rv->cdelay_L, rv->out_co[7]) +
				allpass2_get1(&rv->cbass_ap1_L, rv->out_co[8]) +
				allpass2_get2(&rv->cbass_ap1_L, rv->out_co[9]) -
				allpass2_get2(&rv->cbass_ap1_R, rv->out_co[10]) +
				allpass3_get1(&rv->cbass_ap2_L, rv->out_co[11]) +
				allpass3_get2(&rv->cbass_ap2_L, rv->out_co[12]) +
				allpass3_get3(&rv->cbass_ap2_L, rv->out_co[13]) -
				allpass3_get2(&rv->cbass_ap2_R, rv->out_co[14]);
			float D4 =
				delay_get(&rv->cdelay_L, rv->out_co[15]);

			float B1 =
				delay_get(&rv->cbass_d1_R, rv->out_co[16]);
			float B2 =
				delay_get(&rv->cbass_d2_R, rv->out_co[17]) -
				delay_get(&rv->cbass_d2_L, rv->out_co[18]) +
				delay_get(&rv->cbass_d2_R, rv->out_co[19]) -
				delay_get(&rv->cdelay_L, rv->out_co[20]) -
				delay_get(&rv->cbass_d1_L, rv->out_co[21]) -
				delay_get(&rv->cbass_d2_L, rv->out_co[22]);
			float B3 =
				delay_get(&rv->cdelay_R, rv->out_co[23]) +
				allpass2_get1(&rv->cbass_ap1_R, rv->out_co[24]) +
				allpass2_get2(&rv->cbass_ap1_R, rv->out_co[25]) -
				allpass2_get2(&rv->cbass_ap1_L, rv->out_co[26]) +
				allpass3_get1(&rv->cbass_ap2_R, rv->out_co[27]) +
				allpass3_get2(&rv->cbass_ap2_R, rv->out_co[28]) +
				allpass3_get3(&rv->cbass_ap2_R, rv->out_co[29]) -
				allpass3_get2(&rv->cbass_ap2_L, rv->out_co[30]);
			float B4 =
				delay_get(&rv->cdelay_R, rv->out_co[31]);

			float D = D1 * 0.469f + D2 * 0.219f + D3 * 0.064f + D4 * 0.045f;
			float B = B1 * 0.469f + B2 * 0.219f + B3 * 0.064f + B4 * 0.045f;

			lfo = iir1_step(&rv->lfo2_lpf, lfo_step(&rv->lfo2) * rv->wander);
			out_L = comb_step(&rv->comb_L, D, lfo);
			out_R = comb_step(&rv->comb_R, B, -lfo);

			out_L = delay_step(&rv->last_delay_L, biquad_step(&rv->last_lpf_L, out_L));
			out_R = delay_step(&rv->last_delay_R, biquad_step(&rv->last_lpf_R, out_R));

			osL[i2] = out_L * rv->wet1 + out_R * rv->wet2 +
				delay_step(&rv->inp_delay_L, osL[i2]) * rv->dry;
			osR[i2] = out_R * rv->wet1 + out_L * rv->wet2 +
				delay_step(&rv->inp_delay_R, osR[i2]) * rv->dry;
		}
	
		float outL = oversample_stepdown(&rv->oversample_L, osL);
		float outR = oversample_stepdown(&rv->oversample_R, osR);
		outL += er.L * rv->eref_wet + input[i].L * rv->dry;
		outR += er.R * rv->eref_wet + input[i].R * rv->dry;
		output[i].L = outL;
		output[i].R = outR;
	}
}
