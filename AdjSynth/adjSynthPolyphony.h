/**
*	@file		adjSynthPolyphony.h
*	@author		Nahum Budin
*	@date		3-Feb-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					
*	@version	1.0		15-Nov-2019 
*
*	@brief		Handle AdjSynth polyphony
*/

#pragma once

#include <struct_timeval.h>

#include "../LibAPI/synthesizer.h"

class AdjSynthPolyphony
{
public:

	static AdjSynthPolyphony *get_instance();

	/* Look for a free voice based on core load; 
	 Look also if this note is already allocated to this program
	*/
	int get_reused_note(int note = -1, int program = 0);
	int get_less_busy_core();
	int get_free_voice(int core);
	int get_oldest_voice();
	int get_voice_note(int note = -1, int program = 0);
	void free_voice(int voice = -1, bool pend = true);

	int activate_resource(int res_num = -1, int note = -1, int program = 0);

	void inc_busy_core_voices_count(int core);
	void dec_busy_core_voices_count(int core);
	void clear_busy_core_voices_count(int core);
	int get_busy_core_voices_count(int core);


private:
	
	AdjSynthPolyphony();
	static AdjSynthPolyphony *synth_polyphony;

	// Bussy core voices
	volatile int busy_core_voices[_SYNTH_MAX_NUM_OF_CORES];

	struct timeval start_time;
};
