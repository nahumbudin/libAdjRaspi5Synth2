/**
*	@file		dspVoice.cpp
*	@author		Nahum Budin
*	@date		14-Sep-2024
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
*	@brief		Synthesizer voice dsp processing parent object.
*				The object may include 2 independent signal paths: signal_path_1 and signa_path_2.
*				Only 1 path is intilized as a defualt. 
*				Creating an object with a parameter num_of_active_siganl_pathes=2 will enable both pathes.
*				
*				Each signal path comprises:
*				A signal source: source_1 in path_1 and sourc_2 in path_2.
*				Each signal source may support various modulation inputs, as applicable:
*					1. Frequency modulation
*					2. PWM/Symetry modulation
*					2. Amplitude modulation.
*					
*				Each object comprises 2 state-variable filter (LPF, HPF, BPF and ALL-Pass),
*				filter_1 and filter_2 regardless of the number of pathes.
*				Each active source may be send to both filters inputs.
*				Each filter supports a frequency modulation input.
*				
*				The output of each filter is directed into a distortion effect distortion_1
*				and distortion_2.*				
*				
*				The output of each distortion may be directed to 2 Amplifier stages, amp_1 and amp_2.
*				Each amp comprises a PAN modulation input.
*				
*
*				The object comprises various modulation sources that each can be routed to multiple
*				modulation inputs, as applicable:
*					1. 5 ADSR modulators
*					2. 5 LFO modulators.
*
*				Each amp output may be sent to the voice 2 outputs Left and Right.*
*/

#pragma once

// Control sub samplig rate divider
#define _CONTROL_SUB_SAMPLING				16

#include "../LibAPI/types.h"

#include "dspAdsr.h"
#include "dspAmp.h"
#include "dspFilter.h"
#include "dspOsc.h"
#include "dspDistortion.h"

class DSP_Voice
{
public:

	// friend class SynthVoice;
	
	DSP_Voice(
		int voice,
		int samp_rate,
		int block_size,	
		int num_of_active_siganl_pathes = 1,
		func_ptr_void_int_t voice_end_event_callback_pointer = NULL);
	
	void in_use();
	void not_in_use();
	bool is_in_use();
	
	void set_voice_active();
	void set_voice_inactive();
	bool voice_is_active();

	void set_voice_waits_for_not_active();
	void set_voice_not_waits_for_not_active();
	bool is_voice_waits_for_not_active();
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	void set_wait_for_not_active();
	void reset_wait_for_not_active();
	bool get_wait_for_not_active_state();
	
	void register_voice_end_event_callback(func_ptr_void_int_t func_ptr);

	void copy_my_state_to(DSP_Voice target);
	
	void set_voice_signal_1_frequency(float frq);
	float get_voice_signal_1_frequency();
	
	void set_voice_signal_2_frequency(float frq);
	float get_voice_signal_2_frequency();
	
	
	
	void set_filter1_freq_mod_lfo(int lfo);
	void set_filter1_freq_mod_lfo_level(int lev);
	void set_filter1_freq_mod_env(int env);
	void set_filter1_freq_mod_env_level(int lev);

	int get_filter1_freq_mod_lfo();
	float get_filter1_freq_mod_lfo_level();
	int get_filter1_freq_mod_env();
	float get_filter1_freq_mod_env_level();
	
	void set_filter1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_filter1_freq_env_modulation(float mod_factor, float mod_val);

	void set_filter2_freq_mod_lfo(int lfo);
	void set_filter2_freq_mod_lfo_level(int lev);
	void set_filter2_freq_mod_env(int env);
	void set_filter2_freq_mod_env_level(int Lev);

	int get_filter2_freq_mod_lfo();
	float get_filter2_freq_mod_lfo_level();
	int get_filter2_freq_mod_env();
	float get_filter2_freq_mod_env_level();
	
	void set_filter2_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_filter2_freq_env_modulation(float mod_factor, float mod_val);
	
	void set_distortion1_drive(float drv);
	void set_distortion1_range(float rng);
	void set_distortion1_blend(float bln);
	void enable_distortion1_auto_gain();
	void disable_distortion1_auto_gain();

	void set_distortion2_drive(float drv);
	void set_distortion2_range(float rng);
	void set_distortion2_blend(float bln);
	void enable_distortion2_auto_gain();
	void disable_distortion2_auto_gain();

	void set_amp1_pan_mod_lfo(int lfo);
	void set_amp1_pan_mod_lfo_level(int lev);
	void set_amp2_pan_mod_lfo(int lfo);
	void set_amp2_pan_mod_lfo_level(int lev);

	int get_amp1_pan_lfo_modulation();
	float get_amp1_pan_lfo_modulationLevel();
	int get_amp2_pan_lfo_modulation();
	float get_amp2_pan_lfo_modulationLevel();

	void set_amp_1_pan_lfo_modulation(float mod_factor, float mod_val);
	void set_amp_2_pan_lfo_modulation(float mod_factor, float mod_val);
	
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents);
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents, float modulation);
	
	void set_lfo1_frequency(float freq);
	void set_lfo2_frequency(float freq);
	void set_lfo3_frequency(float freq);
	void set_lfo4_frequency(float freq);
	void set_lfo5_frequency(float freq);

	void calc_next_modulation_values();

	void update_voice_modulation(int voice = 0);
	
	void calc_next_synthesizer_output_value();
	float get_next_output_value_ch1();
	float get_next_output_value_ch2();
	
	void adsr_note_on(DSP_ADSR *adsr);
	void adsr_note_off(DSP_ADSR *adsr);
	
	void enable_synth1();
	void enable_synth2();
	
	void enable_distortion(); 
	
	void disable_synth1();
	void disable_synth2();
	void disable_distortion();
	
	virtual void set_source_1_send_filter1_level(float lev);
	virtual void set_source_1_send_filter1_level(int lev);
	virtual float get_source_1_send_filter1_level();
	
	virtual void set_source_1_send_filter2_level(float lev);
	virtual void set_source_1_send_filter2_level(int lev);
	virtual float get_source_1_send_filter2_level();
	
	virtual void set_source_2_send_filter1_level(float lev);
	virtual void set_source_2_send_filter1_level(int lev);
	virtual float get_source_2_send_filter1_level();
	
	virtual void set_source_2_send_filter2_level(float lev);
	virtual void set_source_2_send_filter2_level(int lev);
	virtual float get_source_2_send_filter2_level();
	
	virtual void set_source_1_freq_mod_lfo(int lfo);
	virtual void set_source_1_freq_mod_lfo_level(int lev);
	virtual void set_source_1_freq_mod_env(int env);
	virtual void set_source_1_freq_mod_env_level(int lev);
	
	virtual void set_source_1_pwm_mod_lfo(int lfo);
	virtual void set_source_1_pwm_mod_lfo_level(int lev);
	virtual void set_source_1_pwm_mod_env(int env);
	virtual void set_source_1_pwm_mod_env_level(int lev);
	
	virtual void set_source_1_amp_mod_lfo(int lfo);
	virtual void set_source_1_amp_mod_lfo_level(int lev);
	virtual void set_source_1_amp_mod_env(int env);
	virtual void set_source_1_amp_mod_env_level(int lev);
	
	virtual int get_source_1_freq_mod_lfo();
	virtual float get_source_1_freq_mod_lfo_level();
	virtual int get_source_1_freq_mod_env();
	virtual float get_source_1_freq_mod_env_level();
	
	virtual int get_source_1_pwm_mod_lfo();
	virtual float get_source_1_pwm_mod_lfo_level();
	virtual int get_source_1_pwm_mod_env();
	virtual float get_source_1_pwm_mod_env_level();
	
	virtual int get_source_1_amp_mod_lfo();
	virtual float get_source_1_amp_mod_lfo_level();
	virtual int get_source_1_amp_mod_env();
	virtual float get_source_1_amp_mod_env_level();
	
	virtual void set_source_1_amp_lfo_modulation(float mod_factor, float mod_val); 
	virtual void set_source_1_amp_env_modulation(float mod_factor, float mod_val); 
	virtual void set_source_1_pwm_lfo_modulation(float mod_factor, float mod_val); 
	virtual void set_source_1_pwm_env_modulation(float mod_factor, float mod_val); 
	virtual void set_source_1_freq_lfo_modulation(float mod_factor, float mod_val);
	virtual void set_source_1_freq_env_modulation(float mod_factor, float mod_val);
	
	virtual void set_source_2_freq_mod_lfo(int lfo);
	virtual void set_source_2_freq_mod_lfo_level(int lev);
	virtual void set_source_2_freq_mod_env(int env);
	virtual void set_source_2_freq_mod_env_level(int lev);
	
	virtual void set_source_2_pwm_mod_lfo(int lfo);
	virtual void set_source_2_pwm_mod_lfo_level(int lev);
	virtual void set_source_2_pwm_mod_env(int env);
	virtual void set_source_2_pwm_mod_env_level(int lev);
	
	virtual void set_source_2_amp_mod_lfo(int lfo);
	virtual void set_source_2_amp_mod_lfo_level(int lev);
	virtual void set_source_2_amp_mod_env(int env);
	virtual void set_source_2_amp_mod_env_level(int lev);
	
	virtual int get_source_2_freq_mod_lfo();
	virtual float get_source_2_freq_mod_lfo_level();
	virtual int get_source_2_freq_mod_env();
	virtual float get_source_2_freq_mod_env_level();
	
	virtual int get_source_2_pwm_mod_lfo();
	virtual float get_source_2_pwm_mod_lfo_level();
	virtual int get_source_2_pwm_mod_env();
	virtual float get_source_2_pwm_mod_env_level();
	
	virtual int get_source_2_amp_mod_lfo();
	virtual float get_source_2_amp_mod_lfo_level();
	virtual int get_source_2_amp_mod_env();
	virtual float get_source_2_amp_mod_env_level();
	
	virtual void set_source_2_amp_lfo_modulation(float mod_factor, float mod_val); 
	virtual void set_source_2_amp_env_modulation(float mod_factor, float mod_val); 
	virtual void set_source_2_pwm_lfo_modulation(float mod_factor, float mod_val); 
	virtual void set_source_2_pwm_env_modulation(float mod_factor, float mod_val); 
	virtual void set_source_2_freq_lfo_modulation(float mod_factor, float mod_val);
	virtual void set_source_2_freq_env_modulation(float mod_factor, float mod_val);
	
	virtual int set_source_1_freq_detune_oct_value(int oct);
	virtual int get_source_1_freq_detune_oct_set_value();
	
	virtual int set_source_1_freq_detune_semitones_value(int sem);
	virtual int get_source_1_freq_detune_semitones_set_value();
	
	virtual int set_source_1_freq_detune_cents_set_value(int cnt);
	virtual int get_source_1_freq_detune_cents_set_value();
	
	
	virtual int set_source_2_freq_detune_oct_value(int oct);
	virtual int get_source_2_freq_detune_oct_set_value();
	
	virtual int set_source_2_freq_detune_semitones_value(int sem);
	virtual int get_source_2_freq_detune_semitones_set_value();
	
	virtual int set_source_2_freq_detune_cents_value(int cnt);
	virtual int get_source_2_freq_detune_cents_set_value();
	
	
	DSP_Filter *filter1 = NULL, *filter2 = NULL;
	DSP_Distortion *distortion1 = NULL, *distortion2 = NULL;
	DSP_Amp *out_amp = NULL;
	
	DSP_Osc *lfo1 = NULL;
	DSP_Osc *lfo2 = NULL;
	DSP_Osc *lfo3 = NULL;
	DSP_Osc *lfo4 = NULL;
	DSP_Osc *lfo5 = NULL;

	DSP_ADSR *adsr1 = NULL;
	DSP_ADSR *adsr2 = NULL;
	DSP_ADSR *adsr3 = NULL;
	DSP_ADSR *adsr4 = NULL;
	DSP_ADSR *adsr5 = NULL;
	
	float signal_path_1_out;
	float signal_path_2_out;
	float amp_1_pan_mod_value;
	float amp_2_pan_mod_value;
	// Public to enable fast direct access (instead of get() )
	uint32_t source_1_freq_mod_lfo_delay, source_1_pwm_mod_lfo_delay, source_1_amp_mod_lfo_delay;
	uint32_t source_2_freq_mod_lfo_delay, source_2_pwm_mod_lfo_delay, source_2_amp_mod_lfo_delay;
	uint32_t filter_1_freq_mod_lfo_delay, filter_2_freq_mod_lfo_delay;
	uint32_t amp_1_pan_mod_lfo_delay, amp_2_pan_mod_lfo_delay;
	
	
	
private:
	int num_of_Active_paths;
	
	int init_lfo_delays();
	
	/* Virtual functions that must be specifically defined for each child object. */
	virtual int set_synth_specific_sample_rate(int sam_r);
	virtual int set_synth_specific_audio_block_size(int blk_s);
	virtual void copy_my_specific_state_to(DSP_Voice target);
	virtual void set_source_1_specific_frequency(float freq);
	virtual void set_source_2_specific_frequency(float freq);
	
	bool used;
	
	bool synth2_sync_on_synth1;
	
	int sample_rate;
	int audio_block_size;
	
	uint32_t lfo_delays[_NUM_OF_LFOS * _NUM_OF_LFO_DELAY_OPTIONS + 1]; // 5 LFOs, 5 states; 1 None
	
	int voice_id;
	bool voice_active;
	bool voice_waits_for_not_active;
	float source_1_frequency = 440.0f;
	float source_2_frequency = 440.0f;
	
	float lfo_output_value[_NUM_OF_LFOS], adsr_output_value[_NUM_OF_ADSRS];
	
	int filter_1_freq_mod_lfo_modulator, filter_1_freq_mod_env_modulator;
	int filter_2_freq_mod_lfo_modulator, filter_2_freq_mod_env_modulator;
	float filter_1_freq_mod_lfo_level, filter_1_freq_mod_env_level;
	float filter_2_freq_mod_lfo_level, filter_2_freq_mod_env_level;
	
	int amp_1_pan_mod_lfo_modulator, amp_2_pan_mod_lfo_modulator;
	float amp_1_pan_mod_lfo_level, amp_2_pan_mod_lfo_level;
	
	float lfo1_actual_freq, lfo2_actual_freq, lfo3_actual_freq, lfo4_actual_freq, lfo5_actual_freq;
	
	float source_1_send_filter_1_level;
	float source_1_send_filter_2_level;		
	float source_1_amp_lfo_modulation_value;
	float source_1_amp_env_modulation_value;
	float source_1_pwm_lfo_modulation_value;
	float source_1_pwm_env_modulation_value;
	float source_1_freq_lfo_modulation_value;
	float source_1_freq_env_modulation_value;
	bool source_1_active;
	
	float source_2_send_filter_1_level;
	float source_2_send_filter_2_level;		
	float source_2_amp_lfo_modulation_value;
	float source_2_amp_env_modulation_value;
	float source_2_pwm_lfo_modulation_value;
	float source_2_pwm_env_modulation_value;
	float source_2_freq_lfo_modulation_value;
	float source_2_freq_env_modulation_value;
	bool source_2_active;
	
	float filter_1_freq_lfo_modulation_value; 
	float filter_1_freq_env_modulation_value; 
	
	float filter_2_freq_lfo_modulation_value; 
	float filter_2_freq_env_modulation_value;
	
	float source_1_freq_mod_value; 
	float source_2_freq_mod_value;
	float detune_source_1; 
	float detune_source_2;
	float act_freq_source_1; 
	float act_freq_source_2;
	float source_1_pwm_mod_value; 
	float source_2_pwm_mod_value; 
	float source_1_amp_mod_value; 
	float source_2_amp_mod_value; 
	
	float filter_1_freq_mod_value;  
	float filter_2_freq_mod_value;
	
	int source_1_freq_mod_lfo_modulator, source_1_pwm_mod_lfo_modulator, source_1_amp_mod_lfo_modulator;
	float source_1_freq_mod_lfo_level, source_1_pwm_mod_lfo_level, source_1_amp_mod_lfo_level;
	int source_1_freq_mod_env_modulator, source_1_pwm_mod_env_modulator, source_1_amp_mod_env_modulator;
	float source_1_freq_mod_env_level, source_1_pwm_mod_env_level, source_1_amp_mod_env_level;

	int source_2_freq_mod_lfo_modulator, source_2_pwm_mod_lfo_modulator, source_2_amp_mod_lfo_modulator;
	float source_2_freq_mod_lfo_level, source_2_pwm_mod_lfo_level, source_2_amp_mod_lfo_level;
	int source_2_freq_mod_env_modulator, source_2_pwm_mod_env_modulator, source_2_amp_mod_env_modulator;
	float source_2_freq_mod_env_level, source_2_pwm_mod_env_level, source_2_amp_mod_env_level;
	
	bool distortion_1_active, distortion_2_active;

	func_ptr_void_int_t voice_end_event_callback_ptr;
	
};
