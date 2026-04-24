/**
*	@file		adjSynthProgram.h
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.3 
*					1. Code refactoring and notaion.
*					2. Rename Patch to Preset (Patch is to be used for full setup settings).
*					
*	@version	1.1	4-Feb-2021
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and audio block-size handling
*				1.0	15-Nov-2019 
*
*	@brief		Handle AdjSynth MIDI programs
*	
*				The adjSynth Program implements a preset voice settings, in a similar way of a MIDI program.
*				In this way, the adjSynth voices may be dynamically allocated and set with various "sounds".
*				
*				In this version, there are 19 programs (0-18):
*					0-15	MIDI-mapping mode programs used each for a MIDI channel 1-16
*							Settings are loaded with presets and cannot be addited
*					16		Sketch 1 - settings can be changed online
*					17		Sketch 2 - settings can be changed online
*					18		Sketch 3 - settings can be changed online
*					
*				
*					
*/

#pragma once

#include <stdint.h>

#include "../Settings/settings.h"
#include "adjSynthVoice.h"
#include "adjSynthPADcreator.h"
#include "../Audio/audioManager.h"
#include "../DSP/dspMorphedSineOsc.h"
#include "../LibAPI/types.h"

class SynthVoice;

class SynthProgram
{
public:

	SynthProgram(
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE,
		int voices = 2,
		int first_v_index = 0,
		int wt_size = _PAD_DEFAULT_WAVETABLE_SIZE,
		AudioManager *aud_mng = NULL);

	~SynthProgram();
	
	void register_set_preset_settings_default_params_callback_ptr(func_ptr_int_settings_parms_ptr_int_t ptr);
	int activate_set_preset_settings_default_params_callback(_settings_params_t* params, int prog);
	
	
	void register_mark_voice_bussy_callback_ptr(func_ptr_void_int_t ptr);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	void set_num_of_voices(int nov);
	int get_num_of_voices();

	void set_program_preset_params(_settings_params_t *preset_params);

	void set_portamento_time(float porta);
	void set_portamento_time(int porta);
	float get_portamento_level();

	void enable_portamento();
	void disable_portamento();
	bool portamento_is_enabled();

	float get_note_frequency();
	void update_actual_frequency();

	SynthVoice *get_free_voice();
	void free_voice(int voice);

	SynthVoice *synth_voices[_SYNTH_MAX_NUM_OF_VOICES] = { NULL };

	Settings *settings_manager = NULL; 
	_settings_params_t active_preset_params, prev_active_preset_params_x;  

	DSP_MorphingSinusOscWTAB *mso_wtab = NULL;

	// synthPAD wavetable
	SynthPADcreator *synth_pad_creator = NULL;
	Wavetable *program_wavetable = NULL;
	

private:
	
	AudioManager *audio_manager = NULL;
	int prog_num;
	// Global program number identifier allocator
	static int prog_numbers;
	// Maximum number of polyphonic voices assigned to this program
	int num_of_voices;
	// Indicates 1st voice index out of all synthesizer voices. 
	// e.g. firstVoiceIndex = 10 and numOfVoices = 12 => program voices: 10 to 21.
	int first_voice_index;

	bool portamento_enabled;
	// Portamento gliding time
	float portamento_time;
	// Portamento frequency glide factor (1-iRrefreshRate/portaTime/iSAMPLE_RATE) */
	float porata_div = 1.0f;
	// Holds previous note num 
	int prev_note_ind;
	// Indicates if this Note is higher (1) or lower (-1) than the previous note 
	int note_diff;
	//  Note frequency 
	float note_freq = 440.0f;
	// Actual frequency when deviating (e.g. portamento glide) 
	float actual_freq = 440.0f;

	struct timeval start_time;
	
	int sample_rate, audio_block_size;
	
	func_ptr_int_settings_parms_ptr_int_t set_preset_settings_default_params_callback_ptr = NULL;
	func_ptr_void_int_t mark_voice_bussy_callback_ptr = NULL;
};
