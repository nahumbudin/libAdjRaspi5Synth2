/**
* @file		dspAdsr.h
*	@author		Nahum Budin
*	@date		14-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*	
*	@History	23_Jan-2021 1.1
*					1. Code refactoring and notaion.
*					2. Adding update interval parameter as a setting parameter
*				30-Oct-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Envelope generator with 2 adjustable level outputs Send-1 and Send-2.
*/

#pragma once

#include "../utils/utils.h"

// Envelope Generator States 
#define ADSR_STATE_IDLE				0
#define ADSR_STATE_ATTACK			1
#define ADSR_STATE_DECAY			2
#define ADSR_STATE_SUSTAIN			3
#define ADSR_STATE_RELEASE			4
// If Release state is forced prior to getting to Sustain 
#define ADSR_STATE_FORCE_RELEASE	5
#define ADSR_STATE_OFF_RELEASE		6

// Forced release time in sec
#define ADSR_FORCED_RELEASE_TIME_SEC 0.01f

class DSP_ADSR
{
public:
	DSP_ADSR(int voic, float out_val, float minimum_val, float time);
	DSP_ADSR(int voic, float time);
	
	int init(float updt_inter);
	void reset();
	int set_update_interval(float up_int);
	float get_update_interval();
	float get_output_val();
	int set_state(int st);
	int get_state();
	float set_attack_time_sec(float at_sec);
	float set_attack_time_sec_log(int at_sec);
	float get_attack_time();
	float get_attack_step();
	float set_decay_time_sec(float dc_sec);
	float set_decay_time_sec_log(int dc_sec);
	float get_decay_time();
	float get_decay_step();
	int set_sustain_level(int st_sec);
	int get_sustain_level();
	float set_release_time_sec(float rl_sec);
	float set_release_time_sec_log(int rl_sec);
	float get_release_time();
	float get_release_step();
	float set_send_level_1(float snd);
	float set_send_level_1(int snd);
	float set_send_level_2(float snd);
	float set_send_level_2(int snd);
	float get_send_level_1();
	float get_send_level_2();
	void set_note_on();
	void set_note_off();

	uint32_t get_note_on_elapsed_time();
	
	float calc_next_envelope_val();	

	// Count the time note is on
	uint32_t note_on_elapsed_time;
	
private:
	int voice;

	// Attack time in seconds
	float attack_time_sec;
	// Decay time to Sustain level in seconds
	float decay_time_sec;
	// Sustain level in %
	int sustain_level_precentages;
	// Release time in seconds
	float release_time_sec;
	// ADSR envelope generator output value 0 to 1.0
	float output_val;
	// Attack incremental step in each iteration
	float attack_step;
	// Decay incremental step in each iteration 
	float decay_step; 
	// Release incremental step in each iteration 
	float release_step;
	// Forced release incremental step in each iteration 
	float forced_release_step;
	// Minimum value to set calculation accuracy limit
	//  or Sustin minimum level: start release 
	float min_val;
	// The output value when a forced release (before getting to sustain) was set
	float forced_release_out_val;
	// ADSR state
	int state;
	// Update time interval in Seconds 
	float update_interval_sec;
	// Output send level
	float send_level_1;
	float send_level_2;
	// True if note is on
	bool note_on;
};



