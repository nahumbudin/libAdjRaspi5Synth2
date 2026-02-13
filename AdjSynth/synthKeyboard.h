/**
*	@file		synthKeyboard.h
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	1.1 29-Jan-2021
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*				1.0		9-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 15, 2018)
*
*	@brief		Keyboard handling
*/

#pragma once

#include <stdint.h>
#include <sys/time.h>

#include "../Settings/settings.h"
#include "../LibAPI/audio.h"

#define _PORTAMENTO_MAX_TIME_SEC		2.0f

class SynthKeyboard
{
public:
	SynthKeyboard(
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE);
	~SynthKeyboard();
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	
	void set_portamento_time(float porta);
	void set_portamento_time(int porta);
	float get_portamento_level();
	
	void enable_portamento();
	void disable_portamento();
	bool portamento_is_enabled();
	
	void set_sensitivity(int sens);
	int get_sensitivity();

	void set_low_sensitivity(int sens);
	int get_low_sensitivity();

	void set_split_point(int sp);
	int get_split_point();
	
	int get_scaled_velocity(int vel);
	int get_low_scaled_velocity(int vel);
	
	float get_note_frequency();
	void update_actual_frequency();
	
	void  midi_play_note_on(uint8_t channel, uint8_t byte2, uint8_t byte3);
	void  midi_play_note_off(uint8_t channel, uint8_t byte2, uint8_t byte3);
	
private:
	bool portamento_enabled;
	float portamento_time;
	int sensitivity;
	float sensitivity_slope;
	float sensitivity_offset;

	int low_sensitivity;
	float low_sensitivity_slope;
	float low_sensitivity_offset;

	int split_point;
	
	// Indicates if a new Portamento glide has started 
	//	bool newPorta;
	// Portamento frequency glide factor (1-iRrefreshRate/portaTime/iSAMPLE_RATE) */
	float porata_div = 1.0f;
	// Holds previous note num 
	int prev_note_ind; // Add polyphony
	// Indicates if this Note is higher (1) or lower (-1) than the previous note */
	int note_diff; // Add polyphony
	// Note frequency 
	float note_freq;
	// Actual frequency when deviating (e.g. portamento glide) 
	float actual_freq;
	
	struct timeval start_time;
	
	int sample_rate, audio_block_size;
};
