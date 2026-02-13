/**
*	@file		adjSynthPolyphonyManager.h
*	@author		Nahum Budin
*	@date		9-Oct-2024
*	@version	1.0 1st version	
*	
*
*	Based on adjSynthPolyphony.cpp version 1.1 3-Feb-2021
*
*	@brief		Handle AdjSynth polyphony
*/


#pragma once

#include "../LibAPI/synthesizer.h"

class AdjPolyphonyManager
{
public:
	
	static AdjPolyphonyManager *get_poly_manger_instance(int num_of_voic);
	
	int get_number_of_cores();
	
	int increase_core_processing_load_weight(int core, int wght);
	int decrease_core_processing_load_weight(int core, int wght);
	int clear_core_processing_load_weight(int core);
	int get_core_processing_load_weight(int core);
	
	int get_less_busy_core();
	int get_a_free_voice(int core);
	
	int get_oldest_voice();
	int get_reused_note(int note = -1, int program = 0);
	
	int activate_resource(int res_num = -1, int note = -1, int program = 0);
	void free_voice(int voice = -1, bool pend = true);
	
	/* Processing weights. TODO: adjust. */
	static const int voice_processing_weight = 1;
	static const int freeverb3mod2_processing_weight = 20;
	static const int reverb_processing_weight = 10;
	
	
private:
	
	AdjPolyphonyManager(int num_of_voic = 4);
	
	static AdjPolyphonyManager *poly_manager_instance;
	
	int number_of_cores;
	int max_num_of_voices_per_core;
	int max_number_of_voices;
	
	/* Used as a measure for the load distribution between the cores.
	 * Each voice weight is 1. 
	 * Insruments or orocesses that consumes more processing power, for example
	 *   Reverbration, are assigned with a higher weight. 
	 * Every time a core is assigned with a new process, its load counter is incremented
	 *  by the process wight, and every time a process is terminated and removed from
	 *  a core, the process wheight is decreasd by the process wight. */
	int cores_load[_SYNTH_MAX_NUM_OF_CORES];
	
	
	struct timeval start_time;
};
