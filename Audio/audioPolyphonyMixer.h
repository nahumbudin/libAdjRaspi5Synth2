/**
*	@file		audioPolyphonyMixer.h
*	@author		Nahum Budin
*	@date		1-Feb-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					3. Adding midi maping mode settings (midi/sketch)
*					4. Adding voice-status settings (null, not-active, wait-for-not-active, active)
*					
*	@version	1.0		11-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 17, 2017)
*
*	@brief		Mix audio ch1 and ch2 of all voices into stereo Left and Right output signals 
*/

#pragma once

#include "audioBlock.h"
#include "../DSP/dspOsc.h"		// for LFOs
#include "../LibAPI/synthesizer.h"
#include "../LibAPI/midi.h"

//#include "audioFloatMultiCore.h"
//#include "audioBlockFloatMultiCore.h"
//#include "defs.h"
//#include "dspOsc.h"

extern float master_level_1, master_level_2, master_pan_1, master_pan_2, master_send_1, master_send_2;
extern float program_level_1[_SYNTH_MAX_NUM_OF_PROGRAMS], program_level_2[_SYNTH_MAX_NUM_OF_PROGRAMS];
extern float program_pan_1[_SYNTH_MAX_NUM_OF_PROGRAMS], program_pan_2[_SYNTH_MAX_NUM_OF_PROGRAMS];
extern float program_send_1[_SYNTH_MAX_NUM_OF_PROGRAMS], program_send_2[_SYNTH_MAX_NUM_OF_PROGRAMS];
extern float program_idle_level_1, program_idle_level_2;
extern float program_idle_pan_1, program_idle_pan_2;
extern float program_idle_send_1, program_idle_send_2;

class DSP_Osc;

class AudioPolyphonyMixerFloat : public AudioBlockFloat
{
	friend class AudioBlockFloat;
public:
  static AudioPolyphonyMixerFloat* get_instance(
		int stage,
		int num_of_voices,
		int num_of_programs,
		int block_size,
		int mapping_mode,
		AudioBlockFloat** audio_first_update_ptr);
		
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	int set_midi_maping_mode(int mode);
	int get_midi_maping_mode();
	
	static void set_voice_active(int voice);
	static void set_voice_not_active(int voice);
	static bool voice_is_active(int voice);

	static void set_voice_wait_for_not_active(int voice);
	static void reset_voice_wait_for_not_active(int voice);
	static bool voice_waits_for_not_active(int voice);

	void set_active();

	void set_master_level_1(int lev);
	void set_master_level_2(int lev);
	void set_master_pan_1(int pan);
	void set_master_pan_2(int pan);
	void set_master_send_1(int snd);
	void set_master_send_2(int snd);

	void set_program_level_1(int prog, int lev);
	void set_program_level_2(int prog, int lev);
	void set_program_pan_1(int prog, int pan);
	void set_program_pan_2(int prog, int pan);
	void set_program_send_1(int prog, int snd);
	void set_program_send_2(int prog, int snd);

	float *get_master_level_1_ptr();
	float *get_master_level_2_ptr();
	float *get_master_pan_1_ptr();
	float *get_master_pan_2_ptr();
	float *get_master_send_1_ptr();
	float *get_master_send_2_ptr();

	float *get_program_level_1_ptr(int prog);
	float *get_program_level_2_ptr(int prog);
	float *get_program_pan_1_ptr(int prog);
	float *get_program_pan_2_ptr(int prog);
	float *get_program_send_1_ptr(int prog);
	float *get_program_send_2_ptr(int prog);

	void set_voice_gain_1_ptr(int voice, int prog);
	void set_voice_gain_2_ptr(int voice, int prog);
	void set_voice_pan_1_ptr(int voice, int prog);
	void set_voice_pan_2_ptr(int voice, int prog);
	void set_voice_send_1_ptr(int voice, int prog);
	void set_voice_send_2_ptr(int voice, int prog);


	float *get_voice_gain_1_ptr(int voice);
	float *get_voice_gain_2_ptr(int voice);
	float *get_voice_pan_1_ptr(int voice);
	float *get_voice_pan_2_ptr(int voice);
	float *get_voice_send_1_ptr(int voice);
	float *get_voice_send_2_ptr(int voice);

	float get_voice_left_output(int voice, float in1, float in2);
	float get_voice_right_output(int voice, float in1, float in2);

	void preserve_gain_pan(int voice);
	void restore_gain_pan(int voice);

	void preserve_gain_pan_send(int voice);
	void restore_gain_pan_send(int voice);

	void set_lfo_1_frequency(float freq);
	void set_lfo_2_frequency(float freq);
	void set_lfo_3_frequency(float freq);
	void set_lfo_4_frequency(float freq);
	void set_lfo_5_frequency(float freq);
	void set_lfo_6_frequency(float freq);

	void set_lfo_1_waveform(float wf);
	void set_lfo_2_waveform(float wf);
	void set_lfo_3_waveform(float wf);
	void set_lfo_4_waveform(float wf);
	void set_lfo_5_waveform(float wf);
	void set_lfo_6_waveform(float wf);

	void set_lfo_1_stmmetry(float sym);
	void set_lfo_2_stmmetry(float sym);
	void set_lfo_3_stmmetry(float sym);
	void set_lfo_4_stmmetry(float sym);
	void set_lfo_5_stmmetry(float sym);
	void set_lfo_6_stmmetry(float sym);

	void set_amp_1_pan_mod_lfo(int lfo);
	void set_amp_1_pan_mod_lfo_level(int lev);
	void set_amp_2_pan_mod_lfo(int lfo);
	void set_amp_2_pan_mod_lfo_level(int lev);

	void set_amp_1_pan_lfo_modulation(float mod_factor, float mod_val);
	void set_amp_2_pan_lfo_modulation(float mod_factor, float mod_val);

	void calc_next_modulation_values();


	void set_amp_1_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value);
	void set_amp_2_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value);

	void calc_next_modulation_values(float *value_ch1, float *value_ch2);
	
	virtual void update(void);

	DSP_Osc *lfo1, *lfo2, *lfo3, *lfo4, *lfo5, *lfo6;
	

private:
  AudioPolyphonyMixerFloat(
		int stage = 0,
		int block_size = _DEFAULT_BLOCK_SIZE,
		int num_of_programs = _SYNTH_MAX_NUM_OF_PROGRAMS,
		int mapping_mode = _DEFAULT_MIDI_MAPPING_MODE,
		int num_of_voices = _SYNTH_MAX_NUM_OF_VOICES,
		AudioBlockFloat** audio_first_update_ptr = NULL);

	static int inputs;
	int programs;

	float *gain1[_SYNTH_MAX_NUM_OF_VOICES];
	float *gain2[_SYNTH_MAX_NUM_OF_VOICES];
	float *pan1[_SYNTH_MAX_NUM_OF_VOICES];
	float *pan2[_SYNTH_MAX_NUM_OF_VOICES];
	float *send1[_SYNTH_MAX_NUM_OF_VOICES];
	float *send2[_SYNTH_MAX_NUM_OF_VOICES];

	float *preserved_gain_1[_SYNTH_MAX_NUM_OF_VOICES];
	float *preserved_gain_2[_SYNTH_MAX_NUM_OF_VOICES];
	float *preserved_pan_1[_SYNTH_MAX_NUM_OF_VOICES];
	float *preserved_pan_2[_SYNTH_MAX_NUM_OF_VOICES];	
	float *preserved_send_1[_SYNTH_MAX_NUM_OF_VOICES];
	float *preserved_send_2[_SYNTH_MAX_NUM_OF_VOICES];

	float lfo_1_actual_freq, lfo_2_actual_freq, lfo_3_actual_freq;
	float lfo_4_actual_freq, lfo_5_actual_freq, lfo_6_actual_freq;
	float lfo_out[_NUM_OF_LFOS];

	int amp_1_pan_mod_lfo, amp_2_pan_mod_lfo;
	float amp_1_pan_mod_lfo_level, amp_2_pan_mod_lfo_level;
	float amp_1_pan_mod, amp_2_pan_mod;
	
	int audio_block_size;
	int midi_mapping_mode;
	static bool voice_active[_SYNTH_MAX_NUM_OF_VOICES];
	static bool voice_wait_for_not_active[_SYNTH_MAX_NUM_OF_VOICES];

	static AudioPolyphonyMixerFloat* audio_poly_mixer_instance;
};
