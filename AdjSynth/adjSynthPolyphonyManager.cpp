/**
 *	@file		adjSynthPolyphonyManager.cpp
 *	@author		Nahum Budin
 *	@date		6-Feb-2025
 *	@version	1.0 1st version
 *					1. Not manging cores loads - let the OS do it.TODO:
*					2. Adding mutex
*	
*	Based on adjSynthPolyphony.cpp version 1.1 3-Feb-2021
*
*	@brief		Handle AdjSynth polyphony using threads.
*	
*/

#include <stddef.h>
#include <thread>

#include "adjSynthPolyphonyManager.h"
#include "../libAdjRaspi5Synth2.h"

AdjPolyphonyManager *AdjPolyphonyManager::poly_manager_instance = NULL;

AdjPolyphonyManager::AdjPolyphonyManager(int num_of_voices)
{
	max_number_of_voices = num_of_voices;

	if (max_number_of_voices > _SYNTH_MAX_NUM_OF_VOICES)
	{
		number_of_cores = _SYNTH_MAX_NUM_OF_VOICES;
	}

	number_of_cores = std::thread::hardware_concurrency();

	if (number_of_cores > _SYNTH_MAX_NUM_OF_CORES)
	{
		number_of_cores = _SYNTH_MAX_NUM_OF_CORES;
	}

	if (number_of_cores > 1)
	{
		max_num_of_voices_per_core = max_number_of_voices / (number_of_cores - 1);
	}
	else
	{
		max_num_of_voices_per_core = max_number_of_voices;
	}

	for (int i = 0; i < _SYNTH_MAX_NUM_OF_CORES; i++)
	{
		cores_load[i] = 0;
	}

	gettimeofday(&start_time, NULL);

	pthread_mutex_init(&voice_allocations_mutex, NULL);
}

AdjPolyphonyManager *AdjPolyphonyManager::get_poly_manger_instance(int num_of_voices)
{
	if (poly_manager_instance == NULL)
	{
		poly_manager_instance = new AdjPolyphonyManager(num_of_voices);
	}

	return poly_manager_instance;
}

AdjPolyphonyManager::~AdjPolyphonyManager()
{
	if (poly_manager_instance != NULL)
	{
		delete poly_manager_instance;
		poly_manager_instance = NULL;
	}
}

/**
 *   @brief  Get a free voice.
 *		Start by looking if this note and program is already playing.
 *		  (It is assumed that if a program note is already playing, 
 *		   it should be reused, as playing the same note again is useless).
 *		If no note to reuse is found, look for a free voice.
 *		If no free voice is found, look for the 1st voice to become used (oldest)
 *   @param  note	requested note
 *   @param	program	requested program
 *   @return voice number or -1 if no voice is free, -2 if CPU is too loaded, -3 if params are out of range
 */
int AdjPolyphonyManager::get_voice(int note, int program)
{
	int voice;

	if (mod_synth_get_cpu_utilization() > 70)
	// CPU is too loaded
	{
		fprintf(stderr, "Get note: CPU is too loaded!\n");
		return -2;
	}

	if ((note < 0) || (note > 127) || (program < 0) ||
		(program >= mod_synth_get_synthesizer_num_of_programs()))
	// Illegal parameters range
	{
		return -3;
	}

	pthread_mutex_lock(&voice_allocations_mutex);

	// Look for a program voice note that is already playing
	voice = get_reused_note(note, program);

	if (voice >= 0)
	{
		pthread_mutex_unlock(&voice_allocations_mutex);
		
		return voice;
	}

	// No program voice note found - look for a free voice

	for (voice = 0; voice < mod_synth_get_synthesizer_num_of_polyphonic_voices(); voice++)
	{
		if (AdjSynth::get_instance()->synth_voice[voice] != NULL)
		{
			if (!(AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_active() ||
				  AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_wait_for_not_active()))
			{
				// A free voice - don't mark active yet, let activate_resource() do it
				//AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_active();

				pthread_mutex_unlock(&voice_allocations_mutex);
				
				return voice;
			}
		}
	}	
	
	// No free voice found - reuse the oldest voice
	voice = get_oldest_voice();
	if (voice >= 0)
	{
		pthread_mutex_unlock(&voice_allocations_mutex);
		
		return voice;
	}

	pthread_mutex_unlock(&voice_allocations_mutex);

	return -1;
}

/**
 *   @brief  Free a voice.
 *   @param  voice	voice number
 *   @param	pending if set true, wait untill envelope is zero
 *   @return tvoid
 */
void AdjPolyphonyManager::free_voice(int voice, int program, bool pend)
{
	pthread_mutex_lock(&voice_allocations_mutex);
	
	if ((voice >= 0) && (voice < mod_synth_get_synthesizer_num_of_polyphonic_voices()) &&
		(program >= 0) && (program < mod_synth_get_synthesizer_num_of_programs()))
	{
		if (AdjSynth::get_instance()->synth_voice[voice] != NULL)
		{
			if (pend)
			{
				// The voice will be free only when voice envelope will decay to zero.
				AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_wait_for_not_active();
			}
			else
			{
				// Free the voice immediately
#ifdef _USE_NEW_POLY_MIXER_
				AdjSynth::get_instance()->synth_program[program]->deallocate_voice_from_program(voice);
#else
				AdjSynth::get_instance()->synth_program[program]->free_voice(progvoice);
#endif
				AdjSynth::get_instance()->synth_voice[voice]->audio_voice->reset_wait_for_not_active();
				AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_note(-1);
				AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_timestamp(0);
				AdjSynth::get_instance()->synth_voice[voice]->set_allocated_program(-1);
				AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_inactive(); // TODO: need?
				
				// Update GUI ?
				AdjSynth::get_instance()->mark_voice_not_busy_callback(voice);
			}
		}
	}

	pthread_mutex_unlock(&voice_allocations_mutex);
}

/**
 *   @brief  Returns the voice number of the voice which is active for the longest time
 *   @param  none
 *   @return the voice number of the voice which is active for the longest time
 */
int AdjPolyphonyManager::get_oldest_voice()
{
	int v;
	int minvoice = -1;
	uint64_t mintime = UINT64_MAX;

	//pthread_mutex_lock(&voice_allocations_mutex);

	// Alocate first to be allocated in the past (oldest)
	for (v = 0; v < mod_synth_get_synthesizer_num_of_polyphonic_voices(); v++)
	{
		if (AdjSynth::get_instance()->synth_voice[v] != NULL)
		{
			if (AdjSynth::get_instance()->synth_voice[v]->audio_voice->get_timestamp() < mintime)
			{
				mintime = AdjSynth::get_instance()->synth_voice[v]->audio_voice->get_timestamp();
				minvoice = v;
			}
		}
	}

	//pthread_mutex_unlock(&voice_allocations_mutex);

	return minvoice;
}

/**
 *   @brief  Returns a voice num that is already assigned to this note and program.
 *			If more than 1 found, look for the 1st to become used (oldest).
 *			Used for reactivating a program note that is already playing.
 *   @param  note	requested note
 *   @param	program	requested program
 *   @return a voice num that is already assigned to this note and program;
 *			-2 if CPU is too loaded; -3 if params are out of range
 */
int AdjPolyphonyManager::get_reused_note(int note, int program)
{
	int v, result = -1;
	SynthVoice *voice = NULL;

	if (mod_synth_get_cpu_utilization() > 70)
	// CPU is too loaded
	{
		return -2;
	}

	if ((note < 0) || (note > 127) || (program < 0) ||
		(program >= mod_synth_get_synthesizer_num_of_programs()))
	// Illegal parameters range
	{
		return -3;
	}

	//pthread_mutex_lock(&voice_allocations_mutex);
	
	// Look for an active voice that is already assigned to this note and program
	for (v = 0; v < mod_synth_get_synthesizer_num_of_polyphonic_voices(); v++)
	{
		voice = AdjSynth::get_instance()->synth_program[program]->get_voice(v);

		if (voice != NULL)
		{
			// if (voice->get_allocated_program() == program)
			//{
			//  Voice is assigned to this program - 
			//	no need to check: synth_program[program]->get_voice(v)
				if (voice->audio_voice->get_note() == note)
				{
					// Voice is assigned to this note					
					if ((voice->audio_voice->is_voice_active()) ||  /* && */
						(voice->audio_voice->is_voice_wait_for_not_active()))
					{
						// Voice is currentlly active
						result = v;
						break;
					}
				}
			//}
		}
	}

	//pthread_mutex_unlock(&voice_allocations_mutex);

	return result;
}







int AdjPolyphonyManager::get_number_of_cores()
{
	return number_of_cores;
}

int AdjPolyphonyManager::increase_core_processing_load_weight(int core, int wght)
{
	if ((core < number_of_cores) && (wght > 0))
	{
		cores_load[core] += wght;
	}
	else
	{
		return -1;
	}

	return cores_load[core];
}

int AdjPolyphonyManager::decrease_core_processing_load_weight(int core, int wght)
{
	if ((core < number_of_cores) && (wght > 0))
	{
		cores_load[core] -= wght;
		if (cores_load[core] < 0)
		{
			/* This should never happen! */
			cores_load[core] = 0;
		}
	}
	else
	{
		return -1;
	}

	return cores_load[core];
}

int AdjPolyphonyManager::get_core_processing_load_weight(int core)
{
	if (core < number_of_cores)
	{
		return cores_load[core];
	}
	else
	{
		return -1;
	}
}

int AdjPolyphonyManager::clear_core_processing_load_weight(int core)
{
	if (core < number_of_cores)
	{
		cores_load[core] = 0;
		return cores_load[core];
	}
	else
	{
		return -1;
	}
}

int AdjPolyphonyManager::get_less_busy_core()
{
	int i, min_load = 1000, min_core, result = 1000;

	// Look for the less busy core
	min_load = 1000;
	min_core = 1000;
	for (i = 0; i < number_of_cores; i++)
	{
		if (cores_load[i] < min_load)
		{
			min_load = cores_load[i];
			min_core = i;
		}
	}

	if (min_core >= number_of_cores)
	{
		// No min core found - error
		return -1;
	}
	else
	{
		return min_core;
	}
}

/**
*   @brief  Returns the number of the 1st found non busy voice on a selected core
*   @param  core selected core
*   @return the number of the 1st found non busy voice on a selected core 
*/
int AdjPolyphonyManager::get_a_free_voice(int core)
{
	int result, voice = 0;
	//	struct timeval timestamp;
	int min_voice = -1, min_core = -1;
	bool reused = false;

	//pthread_mutex_lock(&voice_allocations_mutex);
	
	result = -1;
	// Look for a free voice on selected core
	// Voices are distributed to cores as follows: (for example: 48 voices and 4 cores)
	// core 0: voices 0-15; core 1: voices 16-31; core 2: voices 32-47; core 3: voices 48-63
	voice = core * AdjSynth::num_of_core_voices;
	for (voice; voice < (core + 1) * AdjSynth::num_of_core_voices; voice++)
	{
		if (AdjSynth::get_instance()->synth_voice[voice] != NULL)
		{
			if (!(AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_active() ||
				  AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_wait_for_not_active()))
			{
				// A free voice
				break;
			}
		}
		//else
		//{
		//	break;
		//}
	}

	if (voice < mod_synth_get_synthesizer_num_of_polyphonic_voices())
	{
		result = voice;
		//	printf("Min Core %i Free voice %i", core, voice);
	}

	//pthread_mutex_unlock(&voice_allocations_mutex);

	return result;
}



/**
*   @brief  Activate a voice resource..
*   @param  res	voice number
*   @param	note	specified note
*   @param	program	specified program
*   @return 0 if done.
*/
int AdjPolyphonyManager::activate_resource(int res_num, int note, int program)
{
	struct timeval timestamp;
	bool reused = false;

	if ((res_num < 0) || (res_num >= mod_synth_get_synthesizer_num_of_polyphonic_voices()) ||
		(note < 0) || (note > 127) || (program < 0) ||
		(program > mod_synth_get_synthesizer_num_of_programs()))
	{
		// Illegal parameters range
		return -1;
	}

	pthread_mutex_lock(&voice_allocations_mutex);

	gettimeofday(&timestamp, NULL);
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->set_timestamp((timestamp.tv_usec - start_time.tv_usec) / 1000 + (timestamp.tv_sec - start_time.tv_sec) * 1000);
	//AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->set_active();
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->reset_wait_for_not_active();
	AdjSynth::get_instance()->synth_voice[res_num]->set_allocated_program(program);
	// Mark voice active LAST, after all initialization is complete
	//AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->set_active(); // In note on

	pthread_mutex_unlock(&voice_allocations_mutex);

	return 0;
}


