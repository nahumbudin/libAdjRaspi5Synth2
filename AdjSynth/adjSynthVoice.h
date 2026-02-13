/**
 *	@file		adjSynthVoice.h
 *	@author		Nahum Budin
 *	@date		3-Oct-2024
 *	@version	1.2
 *					1. Code refactoring and notaion.
 *					2. Add LFO6 and ADSR6 to set_voice_params()
 *					3. Change ADR set_sec to set _sec_log in set_voice_params()
 *					4. Adding voice gain, pan and send parameters
*					
*	@version	2-Feb--2021	1.1
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					3. Adding audio manager object ref.
*				9-Nov-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 June 13, 2018
*
*	@brief		Synthesizer voice object
*/

#pragma once

#include "../Settings/settings.h"
#include "../commonDefs.h"
#include "../LibAPI/synthesizer.h"
#include "../commonDefs.h"
#include "../Audio/audioVoice.h"
#include "../Audio/audioOutput.h"
#include "../Audio/audioManager.h"

#include "../DSP/dspMorphedSineOsc.h"

class DSP_Voice;
class AudioVoiceFloat;
class AudioManager;


class SynthVoice
{
public:
	SynthVoice(int vnum = 0,
		int prg = 0, 
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE,
		_settings_params_t *params = NULL,
		DSP_MorphingSinusOscWTAB *mso_tab = NULL,
		Wavetable *synth_pad_wavetable = NULL,
		AudioManager *aud_mng = NULL);
	
	SynthVoice* get_instance();
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	void set_voice_params(_settings_params_t *params);
	_settings_params_t *get_voice_params();

	void assign_dsp_voice(DSP_Voice *dspv = NULL);

	void set_allocated_program(int prg);
	int get_allocated_program();

	int set_gain_1(int gain);
	int set_gain_2(int gain);
	int set_send_1(int send);
	int set_send_2(int send);
	int set_pan_1(int pan);
	int set_pan_2(int pan);
	int set_pan_lfo_1(int lfo);
	int set_pan_lfo_2(int lfo);
	int set_pan_lfo_level_1(int lev);
	int set_pan_lfo_level_2(int lev);

	void set_pad_wave_table(Wavetable *padwt);

	void update_all();

	AudioVoiceFloat *audio_voice = NULL;

	DSP_Voice *dsp_voice = NULL;

	int voice_num;
	int allocated_to_program_num;
	int allocated_to_program_voice_num;

	DSP_MorphingSinusOscWTAB *mso_wtab = NULL;
	Wavetable *pad_wavetable = NULL;

	int out_gain_1, out_pan_1, out_send_1, out_pan_lfo_1, out_pan_lfo_level_1;
	int out_gain_2, out_pan_2, out_send_2, out_pan_lfo_2, out_pan_lfo_level_2;

  private:	
	
	// Play Mode: Poly, Solo, MIDI
	int play_mode;	
	bool update_in_progress;
	bool update_enable;
	
	int sample_rate, audio_block_size;
	
	AudioManager *audio_manager = NULL;
	
	AudioOutputFloat *audio_out = NULL;
	
	AudioConnectionFloat *connection_voice_out_ch1 = NULL;
	AudioConnectionFloat *connection_voice_out_ch2 = NULL;
	
	// for update_all - holds the 1st object in the propagate chain
	AudioBlockFloat *audio_first_update[_MAX_STAGE_NUM] = { NULL };
	
	_settings_params_t active_params, prev_active_params;
	Settings *settings_manager = NULL;
};
