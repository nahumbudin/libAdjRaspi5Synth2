/**
*	@file		dspVoice.h
*	@author		Nahum Budin
*	@date		3-Oct-2025
*	@version	1.3
*					1. Fixing bug - added the ADSR6 and LFO6.
*					2. Set ADR values: use the _log functions
*					3. Adding LFO/ADSR 6 settings
*					4.  Refactoring lfo_delays[] -> global array in adjSynth.
*					5. Force source outputs to zero when voice is not active.
*					6. Adding option to use global LFOs for modulation calculations.

*
*	@History
*				version 1.2	16-Oct-2024
*				version 1.1	25_Jan-2021
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					3. Adding wait for not active state (wait until voice energy goes low and then go to not active)
*						and voice-end callback.
*				version 1.0	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*/

#pragma once

#include "dspKarplusStrong.h"
#include "dspMorphedSineOsc.h"
#include "dspWavetable.h"
#include "dspAmp.h"
#include "dspFilter.h"
#include "dspNoise.h"
#include "dspOsc.h"
#include "dspDistortion.h"
#include "dspAdsr.h"

class DSP_Osc;

class DSP_Voice
{
public:

	friend class SynthVoice;
	
	DSP_Voice(
		int voice,
		int samp_rate,
		int block_size,	
		DSP_MorphingSinusOscWTAB *mso_wtab = NULL,
		Wavetable *synth_pad_wavetable = NULL,
		func_ptr_void_int_t voice_end_event_callback_pointer = NULL);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	void set_wait_for_not_active();
	void reset_wait_for_not_active();
	bool get_wait_for_not_active_state();
	
	void register_voice_end_event_callback(func_ptr_void_int_t func_ptr);

	void copy_my_state_to(DSP_Voice target);
	
	void in_use();
	void not_in_use();
	bool is_in_use();
	
	void set_voice_frequency(float frq);
	float get_voice_frequency();

	void enable_osc_1();
	void enable_osc_2();
	void enable_noise_1();
	void enable_karplus_1();
	//	void enable_drawbars();
	void enable_morphed_sin_1();
	void enable_pad_synth_1();
	void enable_distortion_1();
	void enable_distortion_2();

	void disable_osc_1();
	void disable_osc_2();
	void disable_noise_1();
	void disable_karplus_1();
	//	void disable_drawbars();
	void disable_morphed_sin_1();
	void disable_pad_synth_1();
	void disable_distortion_1();
	void disable_distortion_2();
	
	void set_osc_1_send_filter_1_level(float lev);
	void set_osc_1_send_filter_1_level(int lev);
	float get_osc_1_send_filter_1_level();
	
	void set_osc_1_send_filter_2_level(float lev);
	void set_osc_1_send_filter_2_level(int lev);
	float get_osc_1_send_filter_2_level();
	
	
	void set_osc_2_send_filter_1_level(float lev);
	void set_osc_2_send_filter_1_level(int lev);
	float get_osc_2_send_filter_1_level();
	
	void set_osc_2_send_filter_2_level(float lev);
	void set_osc_2_send_filter_2_level(int lev);
	float get_osc_2_send_filter_2_level();
	
	void set_noise_1_send_filter_1_level(float lev);
	void set_noise_1_send_filter_1_level(int lev);
	float get_noise_1_send_filter_1_level();
	
	void set_noise_1_send_filter_2_level(float lev);
	void set_noise_1_send_filter_2_level(int lev);
	float get_noise_1_send_filter_2_level();
	
	void set_karplus_1_send_filter_1_level(float lev);
	void set_karplus_1_send_filter_1_level(int lev);
	float get_karplus_1_send_filter_1_level();
	
	void set_karplus_1_send_filter_2_level(float lev);
	void set_karplus_1_send_filter_2_level(int lev);
	float get_karplus_1_send_filter_2_level();

	void set_mso_1_send_filter_1_level(float lev);
	void set_mso_1_send_filter_1_level(int lev);
	float get_mso_1_send_filter_1_level();

	void set_mso_1_send_filter_2_level(float lev);
	void set_mso_1_send_filter_2_level(int lev);
	float get_mso_1_send_filter_2_level();

	void set_pad_1_send_filter_1_level(float lev);
	void set_pad_1_send_filter_1_level(int lev);
	float get_pad_1_send_filter_1_level();

	void set_pad_1_send_filter_2_level(float lev);
	void set_pad_1_send_filter_2_level(int lev);
	float get_pad_1_send_filter_2_level();
	
	void set_voice_active();
	void set_voice_inactive();
	bool is_voice_active();

	void set_voice_waits_for_not_active();
	void set_voice_not_waits_for_not_active();
	bool is_voice_waits_for_not_active();
	
	void set_osc_1_freq_mod_lfo(int lfo);
	void set_osc_1_freq_mod_lfo_level(int lev);
	void set_osc_1_freq_mod_env(int env);
	void set_osc_1_freq_mod_env_level(int lev);

	void set_osc_1_pwm_mod_lfo(int lfo);
	void set_osc_1_pwm_mod_lfo_level(int lev);
	void set_osc_1_pwm_mod_env(int env);
	void set_osc_1_pwm_mod_env_level(int lev);

	void set_osc_1_amp_mod_lfo(int lfo);
	void set_osc_1_amp_mod_lfo_level(int lev);
	void set_osc_1_amp_mod_env(int env);
	void set_osc_1_amp_mod_env_level(int lev);

	int get_osc_1_freq_mod_lfo();
	float get_osc_1_freq_mod_lfo_level();
	int get_osc_1_freq_mod_env();
	float get_osc_1_freq_mod_env_level();

	int get_osc_1_pwm_mod_lfo();
	float get_osc_1_pwm_mod_lfo_level();
	int get_osc_1_pwm_mod_env();
	float get_osc_1_pwm_mod_env_level();

	int get_osc_1_amp_mod_lfo();
	float get_osc_1_amp_mod_lfo_level();
	int get_osc_1_amp_mod_env();
	float get_osc_1_amp_mod_env_level();
	
	void set_osc_1_amp_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc_1_amp_env_modulation(float mod_factor, float mod_val); 
	void set_osc_1_pwm_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc_1_pwm_env_modulation(float mod_factor, float mod_val); 
	void set_osc_1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_1_freq_env_modulation(float mod_factor, float mod_val);

	void set_osc_2_freq_mod_lfo(int lfo);
	void set_osc_2_freq_mod_lfo_level(int lev);
	void set_osc_2_freq_mod_env(int env);
	void set_osc_2_freq_mod_env_level(int lev);

	void set_osc_2_pwm_mod_lfo(int lfo);
	void set_osc_2_pwm_mod_lfo_level(int lev);
	void set_osc_2_pwm_mod_env(int env);
	void set_osc_2_pwm_mod_env_level(int lev);

	void set_osc_2_amp_mod_lfo(int lfo);
	void set_osc_2_amp_mod_lfo_level(int lev);
	void set_osc_2_amp_mod_env(int env);
	void set_osc_2_amp_mod_env_level(int lev);

	int get_osc_2_freq_mod_lfo();
	float get_osc_2_freq_mod_lfo_level();
	int get_osc_2_freq_mod_env();
	float get_osc_2_freq_mod_env_level();

	int get_osc_2_pwm_mod_lfo();
	float get_osc_2_pwm_mod_lfo_level();
	int get_osc_2_pwm_mod_env();
	float get_osc_2_pwm_mod_env_level();

	int get_osc_2_amp_mod_lfo();
	float get_osc_2_amp_mod_lfo_level();
	int get_osc_2_amp_mod_env();
	float get_osc_2_amp_mod_env_level();
	
	void set_osc_2_amp_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc_2_amp_env_modulation(float mod_factor, float mod_val); 
	void set_osc_2_pwm_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc_2_pwm_env_modulation(float mod_factor, float mod_val); 
	void set_osc_2_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_2_freq_env_modulation(float mod_factor, float mod_val);

	void set_noise_1_amp_mod_lfo(int lfo);
	void set_noise_1_amp_mod_lfo_level(int lev);
	void set_noise_1_amp_mod_env(int env);
	void set_noise_1_amp_mod_env_level(int lev);

	int get_noise_1_amp_mod_lfo();
	float get_noise_1_amp_mod_lfo_level();
	int get_noise_1_amp_mod_env();
	float get_noise_1_amp_mod_env_level();
	
	void set_noise_1_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_noise_1_amp_env_modulation(float mod_factor, float mod_val);

	void set_mso_1_freq_mod_lfo(int lfo);
	void set_mso_1_freq_mod_lfo_level(int lev);
	void set_mso_1_freq_mod_env(int env);
	void set_mso_1_freq_mod_env_level(int lev);

	void set_mso_1_pwm_mod_lfo(int lLfo);
	void set_mso_1_pwm_mod_lfo_level(int lev);
	void set_mso_1_pwm_mod_env(int env);
	void set_mso_1_pwm_mod_env_level(int lev);

	void set_mso_1_amp_mod_lfo(int lfo);
	void set_mso_1_amp_mod_lfo_level(int lev);
	void set_mso_1_amp_mod_env(int env);
	void set_mso_1_amp_mod_env_level(int lev);

	int get_mso_1_freq_mod_lfo();
	float get_mso_1_freq_mod_lfo_level();
	int get_mso_1_freq_mod_env();
	float get_mso_1_freq_mod_env_level();

	int get_mso_1_pwm_mod_lfo();
	float get_mso_1_pwm_mod_lfo_lLevel();
	int get_mso_1_pwm_mod_env();
	float get_mso_1_pwm_mod_env_level();

	int get_mso_1_amp_mod_lfo();
	float get_mso_1_amp_mod_lfo_level();
	int get_mso_1_amp_mod_env();
	float get_mso_1_amp_mod_env_level();

	void set_mso_1_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_mso_1_amp_env_modulation(float mod_factor, float mod_val);
	void set_mso_1_pwm_lfo_modulation(float mod_factor, float mod_val);
	void set_mso_1_pwm_env_modulation(float mod_factor, float mod_val);
	void set_mso_1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_mso_1_freq_env_modulation(float mod_factor, float mod_val);

	void set_pad_1_freq_mod_lfo(int lfo);
	void set_pad_1_freq_mod_lfo_level(int lev);
	void set_pad_1_freq_mod_env(int env);
	void set_pad_1_freq_mod_env_level(int lev);

	void set_pad_1_amp_mod_lfo(int lfo);
	void set_pad_1_amp_mod_lfo_level(int lev);
	void set_pad_1_amp_mod_env(int env);
	void set_pad_1_amp_mod_env_level(int lev);

	int get_pad_1_freq_mod_lfo();
	float get_pad_1_freq_mod_lfo_lLevel();
	int get_pad_1_freq_mod_env();
	float get_pad_1_freq_mod_env_level();

	int get_pad_1_amp_mod_lfo();
	float get_pad_1_amp_mod_lfo_level();
	int get_pad_1_amp_mod_env();
	float get_pad_1_amp_mod_env_level();

	void set_pad_1_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_pad_1_amp_env_modulation(float mod_factor, float mod__vval);
	void set_pad_1_freq_lfo_modulation(float mod_factor, float mod_vVal);
	void set_pad_1_freq_env_modulation(float mod_factor, float mod_vVal);

	void set_filter_1_freq_mod_lfo(int lfo);
	void set_filter_1_freq_mod_lfo_level(int lev);
	void set_filter_1_freq_mod_env(int env);
	void set_filter_1_freq_mod_env_level(int lev);

	int get_filter_1_freq_mod_lfo();
	float get_filter_1_freq_mod_lfo_level();
	int get_filter_1_freq_mod_env();
	float get_filter_1_freq_mod_env_level();
	
	void set_filter_1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_filter_1_freq_env_modulation(float mod_factor, float mod_val);

	void set_filter_2_freq_mod_lfo(int lfo);
	void set_filter_2_freq_mod_lfo_level(int lev);
	void set_filter_2_freq_mod_env(int env);
	void set_filter_2_freq_mod_env_level(int Lev);

	int get_filter_2_freq_mod_lfo();
	float get_filter_2_freq_mod_lfo_level();
	int get_filter_2_freq_mod_env();
	float get_filter_2_freq_mod_env_level();
	
	void set_filter_2_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_filter_2_freq_env_modulation(float mod_factor, float mod_val);

	void set_distortion_1_drive(float drv);
	void set_distortion_1_range(float rng);
	void set_distortion_1_blend(float bln);
	void enable_distortion_1_auto_gain();
	void disable_distortion_1_auto_gain();

	void set_distortion_2_drive(float drv);
	void set_distortion_2_range(float rng);
	void set_distortion_2_blend(float bln);
	void enable_distortion_2_auto_gain();
	void disable_distortion_2_auto_gain();

	void set_amp_1_ch_1_pan_mod_lfo(int lfo);
	void set_amp_1_ch_1_pan_mod_lfo_level(int lev);
	void set_amp_1_ch_2_pan_mod_lfo(int lfo);
	void set_amp_1_ch_2_pan_mod_lfo_level(int lev);

	int get_amp_1_ch_1_pan_lfo_modulation();
	float get_amp_1_ch_1_pan_lfo_modulation_level();
	int get_amp_1_ch_2_pan_lfo_modulation();
	float get_amp_1_ch_2_pan_lfo_modulation_level();

	void set_amp_1_ch_1_pan_lfo_modulation(float mod_factor, float mod_val);
	void set_amp_1_ch_2_pan_lfo_modulation(float mod_factor, float mod_val);
	
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents);
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents, float modulation);
	
	void set_lfo_1_frequency(float freq);
	void set_lfo_2_frequency(float freq);
	void set_lfo_3_frequency(float freq);
	void set_lfo_4_frequency(float freq);
	void set_lfo_5_frequency(float freq);
	void set_lfo_6_frequency(float freq);

	void calc_next_modulation_values(int samp_num, bool use_global_lfos = false);

	void update_voice_modulation(int voice = 0);
	
	void calc_next_oscilators_output_value();
	float get_next_output_value_ch_1();
	float get_next_output_value_ch_2();

	void set_osc_2_sync_on_osc_1();
	void set_osc_2_not_sync_on_osc_1();
	bool get_osc_2_sync_on_osc_1_state();

	void adsr_note_on(DSP_ADSR *adsr);
	void adsr_note_off(DSP_ADSR *adsr);
	
	DSP_Osc *osc_1 = NULL, *osc_2 = NULL;	
	DSP_Noise *noise_1 = NULL;
	DSP_KarplusStrong *karplus_1 = NULL;
	DSP_MorphingSinusOsc *mso_1 = NULL;
	DSP_MorphingSinusOscWTAB *mso_wtab_1 = NULL, *original_mso_wtab_1 = NULL;
	DSP_Filter *filter_1 = NULL, *filter_2 = NULL;
	DSP_Distortion *distortion_1 = NULL, *distortion_2 = NULL;
	DSP_Amp *out_amp_1 = NULL;
	DSP_Wavetable *wavetable_1 = NULL;
	Wavetable *pad_wavetable_1 = NULL, *original_pad_wavetable_1 = NULL;

	DSP_Osc *lfo_1 = NULL;
	DSP_Osc *lfo_2 = NULL;
	DSP_Osc *lfo_3 = NULL;
	DSP_Osc *lfo_4 = NULL;
	DSP_Osc *lfo_5 = NULL;
	DSP_Osc* lfo_6 = NULL;

	DSP_ADSR *adsr_1 = NULL;
	DSP_ADSR *adsr_2 = NULL;
	DSP_ADSR *adsr_3 = NULL;
	DSP_ADSR *adsr_4 = NULL;
	DSP_ADSR *adsr_5 = NULL;
	DSP_ADSR* adsr_6 = NULL;

	float amp_1_pan_mod;
	float amp_2_pan_mod;
	// Public to enable fast direct access (instead of get() )
	uint32_t osc_1_freq_mod_lfo_delay, osc_1_pwm_mod_lfo_delay, osc_1_amp_mod_lfo_delay;
	uint32_t osc_2_freq_mod_lfo_delay, osc_2_pwm_mod_lfo_delay, osc_2_amp_mod_lfo_delay;
	uint32_t noise_1_amp_mod_lfo_delay;
	uint32_t mso_1_freq_mod_lfo_delay, mso_1_pwm_mod_lfo_delay, mso_1_amp_mod_lfo_delay;
	uint32_t wavetable_1_freq_mod_lfo_delay, wavetable_1_amp_mod_lfo_delay;
	uint32_t filter_1_freq_mod_lfo_delay, filter_2_freq_mod_lfo_delay;
	uint32_t amp_1_pan_mod_lfo_delay, amp_2_pan_mod_lfo_delay;
		
private:
	//int init_lfo_delays();
	
	bool used;
	
	int sample_rate;
	int audio_block_size;
	
	//uint32_t lfo_delays[_NUM_OF_LFOS * _NUM_OF_LFO_DELAY_OPTIONS + 1]; // 5 LFOs, 5 states; 1 None
	
	int voice;
	bool voice_active;
	bool voice_waits_for_not_active;
	float frequency = 440.0f;
	
	float osc_1_out;
	float osc_2_out;
	float noise_1_out;
	float karplus_1_out;
	float mso_1_out;
	float wavetable_1_out_1, wavetable_1_out_2;

	float lfo_out[_NUM_OF_LFOS], adsr_out[_NUM_OF_ADSRS];	

	int osc_1_freq_mod_lfo, osc_1_pwm_mod_lfo, osc_1_amp_mod_lfo;
	float osc_1_freq_mod_lfo_level, osc_1_pwm_mod_lfo_level, osc_1_amp_mod_lfo_level;
	int osc_1_freq_mod_env, osc_1_pwm_mod_env, osc_1_amp_mod_env;
	float osc_1_freq_mod_env_level, osc_1_pwm_mod_env_level, osc_1_amp_mod_env_level;

	int osc_2_freq_mod_lfo, osc_2_pwm_mod_lfo, osc_2_amp_mod_lfo;
	float osc_2_freq_mod_lfo_level, osc_2_pwm_mod_lfo_level, osc_2_amp_mod_lfo_level;
	int osc_2_freq_mod_env, osc_2_pwm_mod_env, osc_2_amp_mod_env;
	float osc_2_freq_mod_env_level, osc_2_pwm_mod_env_level, osc_2_amp_mod_env_level;

	int mso_1_freq_mod_lfo, mso_1_pwm_mod_lfo, mso_1_amp_mod_lfo;
	float mso_1_freq_mod_lfo_level, mso_1_pwm_mod_lfo_level, mso_1_amp_mod_lfo_level;
	int mso_1_freq_mod_env, mso_1_pwm_mod_env, mso_1_amp_mod_env;
	float mso_1_freq_mod_env_level, mso_1_pwm_mod_env_level, mso_1_amp_mod_env_level;

	int wavetable_1_freq_mod_lfo, wavetable_1_amp_mod_lfo;
	float wavetable_1_freq_mod_lfo_level, wavetable_1_amp_mod_lfo_level;
	int wavetable_1_freq_mod_env, wavetable_1_amp_mod_env;
	float wavetable_1_freq_mod_env_level, wavetable_1_amp_mod_env_level;

	int noise_1_amp_mod_lfo, noise_1_amp_mod_env;
	float noise_1_amp_mod_lfo_level, noise_1_amp_mod_env_level;

	int filter_1_freq_mod_lfo, filter_1_freq_mod_env, filter_2_freq_mod_lfo, filter_2_freq_mod_env;
	float filter_1_freq_mod_lfo_level, filter_1_freq_mod_env_level, filter_2_freq_mod_lfo_level, filter_2_freq_mod_env_level;

	int amp_1_pan_mod_lfo, amp_2_pan_mod_lfo;
	float amp_1_pan_mod_lfo_level, amp_2_pan_mod_lfo_level;

	float lfo_1_actual_freq, lfo_2_actual_freq, lfo_3_actual_freq;
	float lfo_4_actual_freq, lfo_5_actual_freq, lfo_6_actual_freq;
	
	float osc_1_send_filter_1_level;
	float osc_1_send_filter_2_level;		
	float osc_1_amp_lfo_modulation;
	float osc_1_amp_env_modulation;
	float osc_1_pwm_lfo_modulation;
	float osc_1_pwm_env_modulation;
	float osc_1_freq_lfo_modulation;
	float osc_1_freq_env_modulation;
	bool osc_1_active;
	
	float osc_2_send_filter_1_level;
	float osc_2_send_filter_2_level;
	float osc_2_amp_lfo_modulation; 
	float osc_2_amp_env_modulation; 
	float osc_2_pwm_lfo_modulation; 
	float osc_2_pwm_env_modulation; 
	float osc_2_freq_lfo_modulation;
	float osc_2_freq_env_modulation;
	bool osc_2_active;
	bool osc_2_sync_on_osc_1;

	float mso_1_send_filter_1_level;
	float mso_1_send_filter_2_level;
	float mso_1_amp_lfo_modulation; 
	float mso_1_amp_env_modulation; 
	float mso_1_pwm_lfo_modulation; 
	float mso_1_pwm_env_modulation; 
	float mso_1_freq_lfo_modulation;
	float mso_1_freq_env_modulation;
	bool mso_1_active;

	float wavetable_1_send_filter_1_level;
	float wavetable_1_send_filter_2_level;
	float wavetable_1_amp_lfo_modulation; 
	float wavetable_1_amp_env_modulation; 
	float wavetable_1_freq_lfo_modulation;
	float wavetable_1_freq_env_modulation;
	bool wavetable_1_active;
	
	float noise_1_send_filter_1_level;
	float noise_1_send_filter_2_level;
	float noise_1_amp_lfo_modulation; 
	float noise_1_amp_env_modulation; 
	bool noise_1_active;
	
	float karplus_1_send_filter_1_level;
	float karplus_1_send_filter_2_level;
	bool karplus_1_active;

	//	bool drawbars1active;
	
	float filter_1_freq_lfo_modulation; 
	float filter_1_freq_env_modulation; 
	
	float filter_2_freq_lfo_modulation; 
	float filter_2_freq_env_modulation; 
	
	float freq_mod_osc_1; 
	float freq_mod_osc_2; 
	float freq_mod_mso_1; 
	float freq_mod_pad_1;
	float osc_1_detune; 
	float osc_2_detune; 
	float mso_1_detune; 
	float pad_1_detune;
	float act_freq_osc_1; 
	float act_freq_osc_2; 
	float act_freq_mso_1; 
	float act_freq_pad_1;
	float pwm_mod_osc_1; 
	float pwm_mod_osc_2; 
	float pwm_mod_mso_1; 
	float mag_modulation_osc_1; 
	float mag_modulation_osc_2; 
	float mag_modulation_mso_1; 
	float mag_modulation_pad_1;
	float filter_1_freq_mod;  
	float filter_2_freq_mod; 

	bool distortion_1_active, distortion_2_active;

	func_ptr_void_int_t voice_end_event_callback_ptr;

};
