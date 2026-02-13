/**
*	@file		audioPolyMixer.h
*	@author		Nahum Budin
*	@date		29-Sep-2025
*	@version	1.0
* 
* 	
*	Mix audio ch1 and ch2 of all voices into a master stereo Left and Right output signals, and Left and Right send signals.
*
*	Unlike the previous version (audioPolyphonyMixer.h), this version does not handle the voice
*   allocations to programs but only directlly the audio mixing of all the voices.
*	The voice allocation to programs is handled by AdjSynthPrograms object. 
*	
**	In addition, this version uses global LFOs for pan modulation.
* 
*/

#pragma once

#include "audioBlock.h"
#include "../DSP/dspOsc.h" // for LFOs
#include "../LibAPI/synthesizer.h"
#include "../LibAPI/midi.h"

//class DSP_Osc;

/* Global used to set individual output level/pan/send for non program operation ????*/
extern float master_level1, master_level2, master_pan1, master_pan2, master_send1, master_send2;

class AudioPolyMixer : public AudioBlockFloat
{
	friend class AudioBlockFloat;

  public:
	static AudioPolyMixer *get_instance(
		int stage,
		int num_of_voices,
		int num_of_programs,
		int block_size,
		int mapping_mode,
		AudioBlockFloat **audio_first_update_ptr);

	~AudioPolyMixer();

	void set_active();

	int set_audio_block_size(int size);
	int get_audio_block_size();

	int set_midi_mapping_mode(int mode);
	int get_midi_mapping_mode();

	int set_voice_level(int voice, float level);
	int set_voice_pan(int voice, float pan);
	int set_voice_send(int voice, float send);
	int set_voice_pan_lfo(int voice, int lfo);
	int set_voice_pan_lfo_level(int voice, float level);
	
	void set_pan_lfo_modulation_value(float mod_factor, float mod_val, float pan_value, float *value);
	void calc_next_pan_modulation_value(int voice);
	
	virtual void update(void);
	
	//void set_amp_1_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value);
	//void set_amp_2_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value);

	//void calc_next_modulation_values(float *mod_value_ch1, float *mod_value_ch2);
	
	/*
	void set_master_level_1(int lev);
	void set_master_level_2(int lev);
	void set_master_pan_1(int pan);
	void set_master_pan_2(int pan);
	void set_master_send_1(int snd);
	void set_master_send_2(int snd);
	*/

	/* Set a voice gain1 level value.*/
	//int set_voice_gain1_level(int voice, float level);
	/* Set a voice gain2 level value.*/
	//int set_voice_gain2_level(int voice, float level);
	/* Set a voice send1 level value.*/
	//int set_voice_send1_level(int voice, float level);
	/* Set a voice send2 level value.*/
	//int set_voice_send2_level(int voice, float level);
	/* Set a voice pan1 value.*/
	//int set_voice_pan1(int voice, float pan);
	/* Set a voice pan2 value.*/
	//int set_voice_pan2(int voice, float pan);
	
	/*
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
	*/
	
	/*
	void set_amp_1_pan_mod_lfo(int lfo);
	void set_amp_1_pan_mod_lfo_level(int lev);
	void set_amp_2_pan_mod_lfo(int lfo);
	void set_amp_2_pan_mod_lfo_level(int lev);
	*/
	
	
	/* Modulation LFOs */
	//DSP_Osc *lfo1, *lfo2, *lfo3, *lfo4, *lfo5, *lfo6;

  private:
	AudioPolyMixer(
		int stage = 0,
		int block_size = _DEFAULT_BLOCK_SIZE,
		int num_of_programs = _SYNTH_MAX_NUM_OF_PROGRAMS,
		int mapping_mode = _DEFAULT_MIDI_MAPPING_MODE,
		int num_of_voices = _SYNTH_MAX_NUM_OF_VOICES,
		AudioBlockFloat **audio_first_update_ptr = NULL);

	/* Holds the number of mixer inputs (voices)*/	
	static int num_of_inputs;
	
	/* Holds the audio block size */
	int audio_block_size;
	
	float voice_level[_SYNTH_MAX_NUM_OF_VOICES];
	float voice_pan[_SYNTH_MAX_NUM_OF_VOICES];
	float voice_send[_SYNTH_MAX_NUM_OF_VOICES];
	int voice_pan_lfo[_SYNTH_MAX_NUM_OF_VOICES];
	float voice_pan_lfo_level[_SYNTH_MAX_NUM_OF_VOICES];
	
	/* Holds the modulation LFO output values */
	float lfo_out[_NUM_OF_LFOS];
	
	/* Holds the PAN modulation factor (LFO and PAN level) */
	float voice_pan_mod_level[_SYNTH_MAX_NUM_OF_VOICES];
	
	/* Holds the midi mapping mode:
	 *	_MIDI_MAPPING_MODE_MAPPING - midi channels are used, 
	 *	_MIDI_MAPPING_MODE_SKETCH - midi chanels are ignored */
	int midi_mapping_mode;

	static AudioPolyMixer *audio_poly_mixer_instance;

	/* Holds all voices gain 1 level */
	//float gain1[_SYNTH_MAX_NUM_OF_VOICES];
	/* Holds all voices gain 2 level */
	//float gain2[_SYNTH_MAX_NUM_OF_VOICES];
	///* Holds all voices pan 1 value */
	//float pan1[_SYNTH_MAX_NUM_OF_VOICES];
	/* Holds all voices pan 2 value */
	//float pan2[_SYNTH_MAX_NUM_OF_VOICES];
	/* Holds all voices send 1 value */
	//float send1[_SYNTH_MAX_NUM_OF_VOICES];
	///* Holds all voices send 2 value */
	//float send2[_SYNTH_MAX_NUM_OF_VOICES];
	
	/* Holds the modulation LFOs frequency (normlized to modulation sub-sample rate) */
	//float lfo_1_actual_freq, lfo_2_actual_freq, lfo_3_actual_freq;
	//float lfo_4_actual_freq, lfo_5_actual_freq, lfo_6_actual_freq;

	/* Holds the amp1 and the amp2 pan modulation selected modulated LFO index */
	//int amp_1_pan_mod_lfo, amp_2_pan_mod_lfo;
	
	/* Holds the amp1 and the amp2 pan modulation LFO level (depth) */
	//float amp_1_pan_mod_lfo_level, amp_2_pan_mod_lfo_level;

	/* Holds the amp1 and the amp2 pan modulation value */
	//float amp_1_pan_mod, amp_2_pan_mod;

};
