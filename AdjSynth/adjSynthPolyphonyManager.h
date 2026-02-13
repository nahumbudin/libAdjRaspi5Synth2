/**
*	@file		adjSynthPolyphonyManagerThreads.h
*	@author		Nahum Budin
*	@date		6-Feb-2025
*	@version	1.0 1st version
*					1. Not manging cores loads - let the OS do it.
*					2. Adding mutex
*
*	Based on adjSynthPolyphony.cpp version 1.1 3-Feb-2021
*
*	@brief		Handle AdjSynth polyphony using threads.
*
*	The polyphony manager is responsible for managing polyphonic voice objects by:
*       1. Allocating free objects and recycling not active objects.
*       2. Load balancing.
*
*   The system allocates and creates a preset number of polyphonic voice objects _MAX_NUM_OF_POLYPHONIC_VOICES.
*
*   This number shall be set to M x (N-1), where N is the number of cores (See more details hereafter).
*
*	Each of these polyphonic voice objects may be in one of the following states:
*		1. Not Active - the object is in an idle state.
*		2. Active - the object is generating or processing a voice signal.
*		3. Waiting for Not Active -
*	       - An active object state is changed to this state when the object receives a Note-Off command,
*	         triggering the envelope Release state.
*	       - When the object-generated voice output audio signal amplitude is decayed below a threshold
*	         (the Release envelope phase end), the object state is changed back to Not Active.
*
*	When asked for, the polyphonic manager allocates a Not Active object and sets its state to Active.
*	In principle, this object may be reallocated only after it returns to the Not Active state.
*
*	If all the allocated objects are active and a new request for a voice object is received, the polyphonic
*	manager may act based on its polyphonic mode state, as follows:
*		1. Blocked Mode - no new voice-block will be allocated until there is at least one Not-Active voice block.
*		2. Reuse - if there is an active voice block that is set to the same new voice block program and note,
*		   this voice block will be set to the Not Active state and will be reactivated. If there are multiple
*		   active voice blocks with the same program and note, the voice block that was created 1st (oldest) will be selected.
*		3. Force Oldest - the oldest voice block will be forced to the Not-Active state, and then allocated.
*
*		Let OS manage the CPU load balancing.!!!!!!!
*
*	The load management is performed by distributing the voice blocks' execution across the platform CPU cores
*	in the most applicable way.
*
*	For a CPU with N cores, N-1 voice blocks' processing threads are created.
*	So, for the Raspberry Pi platform, for example, 3 processing threads are defined.
*
*	All the pre-allocated voice blocks (_SYNTH_MAX_NUM_OF_VOICES) are divided into (N-1) groups, where N isw the
*	number of CPU cores. Each of the groups is associated with one of the processing threads.
*
*	The polyphonic manager is responsible for balancing the processing load by managing the number of active
*	voice blocks that are allocated to each of these threads.
*
*	Each voice block is assigned a weight parameter that indicates the amount of processing.
*	For example, a basic tone-generating voice block is assigned a weight of 1, while a
*	reverberation-processing voice block, for example, which involves more processing, is assigned a weight of 20.
*
*	When a voice block is assigned to a thread, the thread's total weight is incremented by the voice block weight,
*	and when the voice block becomes Not-Active, the thread's total weight is decremented by the voice block weight .
*
*	The polyphonic manager is balancing the load by allocating any new voice block to the thread its total
*	weight parameter is the lowest, and marks the voice block as being allocated to this thread.

*/

#pragma once

#include "../LibAPI/synthesizer.h"

class AdjPolyphonyManager
{
  public:
	static AdjPolyphonyManager *get_poly_manger_instance(int num_of_voices);

	~AdjPolyphonyManager();

	int get_voice(int note = -1, int program = 0);
	void free_voice(int voice = -1, int program = 0, bool pend = true);
	int get_reused_note(int note = -1, int program = 0);
	
	

	int get_number_of_cores();

	int increase_core_processing_load_weight(int core, int wght);
	int decrease_core_processing_load_weight(int core, int wght);
	int clear_core_processing_load_weight(int core);
	int get_core_processing_load_weight(int core);

	int get_less_busy_core();
	int get_a_free_voice(int core);

	

	int activate_resource(int res_num = -1, int note = -1, int program = 0);
	

	/* Processing weights. TODO: adjust. */
	static const int voice_processing_weight = 1;
	static const int freeverb3mod2_processing_weight = 20;
	static const int reverb_processing_weight = 10;

  private:
	AdjPolyphonyManager(int num_of_voic = 4);

	int get_oldest_voice();

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

	/* A mutex to handle voice allocations */
	pthread_mutex_t voice_allocations_mutex;

	struct timeval start_time;
};