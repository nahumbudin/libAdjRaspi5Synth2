/**
*	@file		dspAdjSynthVoice.h
*	@author		Nahum Budin
*	@date		21-Sep-2024
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Leaving only common parts (specific synth implemented as children objects)
*					
*	@History	25_Jan-2021 1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate and bloc-size settings
*					3. Adding wait for not active state (wait until voice energy goes low and 
*						then go to not active) and voice-end callback.
*				9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer dsp processing object representing a single polyphonic voice.
*				Each object may include 2 independent signal paths: signal_path_1 and signa_path_2.
*				Only signal_path_1 is intilized as a defualt. 
*				Creating an object with a parameter num_of_active_siganl_pathes=2 will enable both pathes.
*				
*				signal_path_1 includes the following signal sources:
*					1. Oscilator (dspOsc)
*					2. Karplus Strong FFT synthesizer (dspKarplusStrong).
*					3. Morphed Sine synthesizer (dspMorphedSine).
*					4. Noise Generator (dspNoise).
*					
*				signal_path_2 includes the following signal sources:
*					1. Oscilator (dspOsc)
*					
*				Each signal source may be enabled or disabled independently.
*				
*				signal_path_2 Oscilator may be phased-locked on signal_path_1 Oscilator.
*				
*				The signal sources supports various modulation inputs:
*					1. Frequency modulation
*					2. PWM/Symetry modulation
*					2. Amplitude modulation.
*					
*				Each object comprises 2 state-variable filter (LPF, HPF, BPF and ALL-Pass),
*				filter_1 and filter_2, regardless of the number of pathes.
*				Each active source may be send to both filters inputs.
*				Each filter supports a frequency modulation input.
*					
*				The output of each filter is directed into a distortion effect processors,
*				distortion_1 and distortion_2.				
*				
*				The output of each distortion may be directed to 2 Amplifier stages, amp_1 and amp_2.
*				Each amp comprises a PAN modulation input.
*				
*				Each amp output may be sent to the voice 2 outputs Left and Right.*				
*
*				Each object comprises various modulation sources that each can be routed to multiple
*				modulation inputs, as applicable:
*					1. 5 ADSR modulators
*					2. 5 LFO modulators.
*
*				
*/

#pragma once

#include "../LibAPI/types.h"

#include "dspOsc.h"
#include "dspNoise.h"
#include "dspKarplusStrong.h"
#include "dspMorphedSineOsc.h"
#include "dspWaveformTable.h"
#include "dspWavetable.h"
#include "dspFilter.h"
#include "dspAmp.h"
#include "dspDistortion.h"
#include "dspAdsr.h"

class DSP_Osc;	
 

class DSP_AdjSynthVoice
{
public:
	
	DSP_AdjSynthVoice(int voice,
		int samp_rate,
		int block_size,	
		Wavetable *synth_pad_wavetable = NULL,
		func_ptr_void_int_t voice_end_event_callback_pointer = NULL);
	
	void set_in_use();
	void set_not_in_use();
	bool is_in_use();
	
	void set_voice_active();
	void set_voice_inactive();
	bool voice_is_active();
	void no_activity_detected(int voice_id);
	
	void set_voice_waits_for_not_active();
	void set_voice_not_waiting_for_not_active();
	bool is_voice_waits_for_not_active();
	void register_voice_end_event_callback(func_ptr_void_int_t func_ptr);
	
	void set_voice_frequency(float frq);
	float get_voice_frequency();
	
	void enable_osc_1();
	void enable_osc_2();
	void enable_noise_1();
	void enable_karplus_1();
	void enable_morphed_sin_1();
	void enable_pad_synth_1();
	void enable_distortion();

	void disable_osc_1();
	void disable_osc_2();
	void disable_noise_1();
	void disable_karplus_1();
	void disable_morphed_sin_1();
	void disable_pad_synth_1();
	void disable_distortion();
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	void copy_my_state_to(DSP_AdjSynthVoice target);
	
	void set_osc_1_send_filter_1_level(float lev);
	void set_osc_1_send_filter_1_level(int lev);
	float get_osc_1_send_filter_1_level();
	
	void set_osc_1_send_filter_2_level(float lev);
	void set_osc_1_send_filter_2_level(int lev);
	float get_osc_1_send_filter_2_level();
	
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
	
	void set_osc_1_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_1_amp_env_modulation(float mod_factor, float mod_val);
	void set_osc_1_pwm_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_1_pwm_env_modulation(float mod_factor, float mod_val);
	void set_osc_1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_1_freq_env_modulation(float mod_factor, float mod_val);

	
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
	
	
	void set_osc_2_send_filter_1_level(float lev);
	void set_osc_2_send_filter_1_level(int lev);
	float get_osc_2_send_filter_1_level();
	
	void set_osc_2_send_filter_2_level(float lev);
	void set_osc_2_send_filter_2_level(int lev);
	float get_osc_2_send_filter_2_level();
	
	
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
	
	void set_osc_2_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_2_amp_env_modulation(float mod_factor, float mod_val);
	void set_osc_2_pwm_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_2_pwm_env_modulation(float mod_factor, float mod_val);
	void set_osc_2_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_osc_2_freq_env_modulation(float mod_factor, float mod_val);

	
	
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
	
	
	void set_osc_2_sync_on_osc_1();
	void set_osc_2_not_sync_on_osc_1();
	bool get_osc_2_sync_on_osc_1_state();
	
	
	void set_noise_1_send_filter_1_level(float lev);
	void set_noise_1_send_filter_1_level(int lev);
	float get_noise_1_send_filter_1_level();
	
	void set_noise_1_send_filter_2_level(float lev);
	void set_noise_1_send_filter_2_level(int lev);
	float get_noise_1_send_filter_2_level();
	
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
	
	void set_mso_1_freq_mod_lfo(int lfo);
	void set_mso_1_freq_mod_lfo_level(int lev);
	void set_mso_1_freq_mod_env(int env);
	void set_mso_1_freq_mod_env_level(int lev);
	
	void set_mso_1_pwm_mod_lfo(int lfo);
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
	float get_mso_1_pwm_mod_lfo_level();
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
	

	void set_pad_1_send_filter_1_level(float lev);
	void set_pad_1_send_filter_1_level(int lev);
	float get_pad_1_send_filter_1_level();

	void set_pad_1_send_filter_2_level(float lev);
	void set_pad_1_send_filter_2_level(int lev);
	float get_pad_1_send_filter_2_level();
	
	void set_pad_1_freq_mod_lfo(int lfo);
	void set_pad_1_freq_mod_lfo_level(int lev);
	void set_pad_1_freq_mod_env(int env);
	void set_pad_1_freq_mod_env_level(int lev);

	void set_pad_1_amp_mod_lfo(int lfo);
	void set_pad_1_amp_mod_lfo_level(int lev);
	void set_pad_1_amp_mod_env(int env);
	void set_pad_1_amp_mod_env_level(int lev);
	
	
	int get_pad_1_freq_mod_lfo();
	float get_pad_1_freq_mod_lfo_level();
	int get_pad_1_freq_mod_env();
	float get_pad_1_freq_mod_env_level();

	int get_pad_1_pwm_mod_lfo();
	float get_pad_1_pwm_mod_lfo_level();
	int get_pad_1_pwm_mod_env();
	float get_pad_1_pwm_mod_env_level();

	int get_pad_1_amp_mod_lfo();
	float get_pad_1_amp_mod_lfo_level();
	int get_pad_1_amp_mod_env();
	float get_pad_1_amp_mod_env_level();
	
	
	void set_pad_1_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_pad_1_amp_env_modulation(float mod_factor, float mod_val);
	void set_pad_1_pwm_lfo_modulation(float mod_factor, float mod_val);
	void set_pad_1_pwm_env_modulation(float mod_factor, float mod_val);
	void set_pad_1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_pad_1_freq_env_modulation(float mod_factor, float mod_val);
	
	
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
	void set_filter_2_freq_mod_env_level(int lev);

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
	
	int get_amp_1_ch_1_pan_lfo_modulation();
	float get_amp_1_ch_1_pan_lfo_modulation_level();
	
	void set_amp_1_ch_1_pan_lfo_modulation(float mod_factor, float mod_val);
	
	/* ??? */
	void set_amp_1_ch_2_pan_mod_lfo(int lfo);
	void set_amp_1_ch_2_pan_mod_lfo_level(int lev);
	
	int get_amp_1_ch_2_pan_lfo_modulation();
	float get_amp_1_ch_2_pan_lfo_modulation_level();
	
	void set_amp_1_ch_2_pan_lfo_modulation(float mod_factor, float mod_val);
	
	void set_lfo_1_frequency(float freq);
	void set_lfo_2_frequency(float freq);
	void set_lfo_3_frequency(float freq);
	void set_lfo_4_frequency(float freq);
	void set_lfo_5_frequency(float freq);
	
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents);
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents, float modulation);
	
	void calc_next_modulation_values();
	void update_voice_modulation(int voice = 0);
	void calc_next_oscilators_output_value();
	float get_next_output_value_ch_1();
	float get_next_output_value_ch_2();
	
	void adsr_note_on(DSP_ADSR *adsr);
	void adsr_note_off(DSP_ADSR *adsr);
	
	
	// Public to enable fast direct access (instead of get() )
	uint32_t osc_1_freq_mod_lfo_delay, osc_1_pwm_mod_lfo_delay, osc_1_amp_mod_lfo_delay;
	uint32_t osc_2_freq_mod_lfo_delay, osc_2_pwm_mod_lfo_delay, osc_2_amp_mod_lfo_delay;
	uint32_t noise_1_amp_mod_lfo_delay;
	uint32_t mso_1_freq_mod_lfo_delay, mso_1_pwm_mod_lfo_delay, mso_1_amp_mod_lfo_delay;
	uint32_t pad_1_freq_mod_lfo_delay, pad_1_amp_mod_lfo_delay;
	uint32_t filter_1_freq_mod_lfo_delay, filter_2_freq_mod_lfo_delay;
	uint32_t amp_1_ch_1_pan_mod_lfo_delay, amp_1_ch_2_pan_mod_lfo_delay;
	
	bool osc_1_active, osc_2_active;
	bool noise_1_active;
	bool karplus_1_active;
	bool mso_1_active;
	bool pad_1_active;
	bool distortion_1_active, distortion_2_active;
	
	DSP_Osc *osc_1, *osc_2;	
	DSP_Noise *noise_1;
	DSP_KarplusStrong *karplus_1;
	DSP_MorphingSinusOsc *mso_1;
	DSP_MorphingSinusOscWTAB *mso_wtab_1, *original_mso_wtab_1;
	DSP_Filter *filter_1, *filter_2;
	DSP_Distortion *distortion_1, *distortion_2;
	DSP_Amp *out_amp_1;
	DSP_Wavetable *pad_1;
	Wavetable *pad_1_wavetable, *original_pad_1_wavetable;
	
	DSP_Osc *lfo_1, *lfo_2, *lfo_3, *lfo_4, *lfo_5;

	DSP_ADSR *adsr_1, *adsr_2, *adsr_3, *adsr_4, *adsr_5;
	
private:
	
	int init_lfo_delays();
	
	int voice_id;
	
	bool used;
	bool voice_active;
	bool voice_waits_for_not_active;
	func_ptr_void_int_t voice_end_event_callback_ptr;
	
	float frequency;
	
	int sample_rate;
	int audio_block_size;
	
	
	float osc_1_out;
	float osc_2_out;
	float noise_1_out;
	float karplus_1_out;
	float mso_1_out;
	float pad_1_out_1, pad_1_out_2;
	
	bool osc_2_sync_on_osc_1;
	
	float osc_1_send_filter_1_level;
	float osc_1_send_filter_2_level;
	float osc_1_amp_lfo_modulation_value;
	float osc_1_amp_env_modulation_value;
	float osc_1_pwm_lfo_modulation_value;
	float osc_1_pwm_env_modulation_value;
	float osc_1_freq_lfo_modulation_value;
	float osc_1_freq_env_modulation_value;
	
	float osc_1_freq_modulation_value;
	float osc_1_freq_detune_value;
	float osc_1_actual_frequency;
	float osc_1_pwm_modulation_value;
	float osc_1_amp_modulation_value;
	
	int osc_1_freq_mod_lfo, osc_1_pwm_mod_lfo, osc_1_amp_mod_lfo;
	float osc_1_freq_mod_lfo_level, osc_1_pwm_mod_lfo_level, osc_1_amp_mod_lfo_level;
	int osc_1_freq_mod_env, osc_1_pwm_mod_env, osc_1_amp_mod_env;
	float osc_1_freq_mod_env_level, osc_1_pwm_mod_env_level, osc_1_amp_mod_env_level;
	
	
	float osc_2_send_filter_1_level;
	float osc_2_send_filter_2_level;
	float osc_2_amp_lfo_modulation_value;
	float osc_2_amp_env_modulation_value;
	float osc_2_pwm_lfo_modulation_value;
	float osc_2_pwm_env_modulation_value;
	float osc_2_freq_lfo_modulation_value;
	float osc_2_freq_env_modulation_value;
	
	float osc_2_freq_modulation_value;
	float osc_2_freq_detune_value;
	float osc_2_actual_frequency;
	float osc_2_pwm_modulation_value;
	float osc_2_amp_modulation_value;
	
	int osc_2_freq_mod_lfo, osc_2_pwm_mod_lfo, osc_2_amp_mod_lfo;
	float osc_2_freq_mod_lfo_level, osc_2_pwm_mod_lfo_level, osc_2_amp_mod_lfo_level;
	int osc_2_freq_mod_env, osc_2_pwm_mod_env, osc_2_amp_mod_env;
	float osc_2_freq_mod_env_level, osc_2_pwm_mod_env_level, osc_2_amp_mod_env_level;
	
	float noise_1_send_filter_1_level;
	float noise_1_send_filter_2_level;
	
	int noise_1_amp_mod_lfo, noise_1_amp_mod_env;
	float noise_1_amp_mod_lfo_level, noise_1_amp_mod_env_level;
	
	float noise_1_amp_lfo_modulation_value;
	float noise_1_amp_env_modulation_value;
	float noise_1_amp_modulation_value;
	
	float karplus_1_send_filter_1_level;
	float karplus_1_send_filter_2_level;
	
	float mso_1_send_filter_1_level;
	float mso_1_send_filter_2_level;
	
	int mso_1_freq_mod_lfo, mso_1_pwm_mod_lfo, mso_1_amp_mod_lfo;
	float mso_1_freq_mod_lfo_level, mso_1_pwm_mod_lfo_level, mso_1_amp_mod_lfo_level;
	int mso_1_freq_mod_env, mso_1_pwm_mod_env, mso_1_amp_mod_env;
	float mso_1_freq_mod_env_level, mso_1_pwm_mod_env_level, mso_1_amp_mod_env_level;
	float mso_1_amp_lfo_modulation_value; 
	float mso_1_amp_env_modulation_value; 
	float mso_1_pwm_lfo_modulation_value; 
	float mso_1_pwm_env_modulation_value; 
	float mso_1_freq_lfo_modulation_value;
	float mso_1_freq_env_modulation_value;
	
	float mso_1_freq_modulation_value;
	float mso_1_freq_detune_value;
	float mso_1_actual_frequency;
	float mso_1_pwm_modulation_value;
	float mso_1_amp_modulation_value;
	
	float pad_1_send_filter_1_level;
	float pad_1_send_filter_2_level;
	
	int pad_1_freq_mod_lfo, pad_1_pwm_mod_lfo, pad_1_amp_mod_lfo;
	float pad_1_freq_mod_lfo_level, pad_1_pwm_mod_lfo_level, pad_1_amp_mod_lfo_level;
	int pad_1_freq_mod_env, pad_1_pwm_mod_env, pad_1_amp_mod_env;
	float pad_1_freq_mod_env_level, pad_1_pwm_mod_env_level, pad_1_amp_mod_env_level;
	float pad_1_amp_lfo_modulation_value; 
	float pad_1_amp_env_modulation_value; 
	float pad_1_pwm_lfo_modulation_value; 
	float pad_1_pwm_env_modulation_value; 
	float pad_1_freq_lfo_modulation_value;
	float pad_1_freq_env_modulation_value;
	
	float pad_1_freq_modulation_value;
	float pad_1_freq_detune_value;
	float pad_1_actual_frequency;
	float pad_1_amp_modulation_value;
	
	int filter_1_freq_mod_lfo, filter_1_freq_mod_env;
	float filter_1_freq_mod_lfo_level, filter_1_freq_mod_env_level;
	
	int filter_2_freq_mod_lfo, filter_2_freq_mod_env;
	float filter_2_freq_mod_lfo_level, filter_2_freq_mod_env_level;
	
	float filter_1_freq_lfo_modulation_value;
	float filter_1_freq_env_modulation_value;
	
	float filter_2_freq_lfo_modulation_value;
	float filter_2_freq_env_modulation_value;
	
	float filter_1_freq_modulation_value;
	float filter_2_freq_modulation_value;
	
	int amp_1_ch_1_pan_mod_lfo, amp_1_ch_2_pan_mod_lfo;
	float amp_1_ch_1_pan_mod_lfo_level, amp_1_ch_2_pan_mod_lfo_level;
	
	float amp_1_ch_1_pan_lfo_modulation_value, amp_1_ch_2_pan_lfo_modulation_value;
	
	float lfo_1_actual_freq, lfo_2_actual_freq, lfo_3_actual_freq;
	float lfo_4_actual_freq, lfo_5_actual_freq;
	
	float lfo_out_value[_NUM_OF_LFOS];
	float adsr_out_value[_NUM_OF_ADSRS];
	
	uint32_t lfo_delays[_NUM_OF_LFOS * _NUM_OF_LFO_DELAY_OPTIONS + 1]; // 5 LFOs, 5 states; 1 None
	
	
	
	
	
};
