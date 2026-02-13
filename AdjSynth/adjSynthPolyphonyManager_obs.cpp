/**
*	@file		adjSynthPolyphonyManager.cpp
*	@author		Nahum Budin
*	@date		23-Sep-2025
*	@version	1.1
*					1. Code refactoring and notaion.
*					2. Bugs fix.
*	
*	Based on adjSynthPolyphony.cpp version 1.1 3-Feb-2021
*
*	@brief		Handle AdjSynth polyphony
*	
*	History:
*	version 1.0		9-Oct-2024  Initial version
*	
*/

#include <stddef.h>
#include <thread>

#include "adjSynthPolyphonyManager.h"
#include "adjSynth.h"
#include "../LibAPI/synthesizer.h"
#include "../commonDefs.h"

extern pthread_mutex_t voice_busy_mutex;

AdjPolyphonyManager *AdjPolyphonyManager::poly_manager_instance = NULL;

AdjPolyphonyManager::AdjPolyphonyManager(int num_of_voic)
{
	max_number_of_voices = num_of_voic;
	
	if (max_number_of_voices > _SYNTH_MAX_NUM_OF_VOICES)
	{
		max_number_of_voices = _SYNTH_MAX_NUM_OF_VOICES;
	}
	
	number_of_cores = std::thread::hardware_concurrency();
	
	if (number_of_cores > _SYNTH_MAX_NUM_OF_CORES)
	{
		number_of_cores = _SYNTH_MAX_NUM_OF_CORES;
	}

	max_num_of_voices_per_core = max_number_of_voices / number_of_cores;
	
	for (int i = 0; i < _SYNTH_MAX_NUM_OF_CORES; i++)
	{
		cores_load[i] = 0;
	}
	
	gettimeofday(&start_time, NULL);
	
}

AdjPolyphonyManager *AdjPolyphonyManager::get_poly_manger_instance(int num_of_voic)
{
	if (poly_manager_instance == NULL)
	{
		poly_manager_instance = new AdjPolyphonyManager(num_of_voic);
	}
	
	return poly_manager_instance;
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
			/* This should never happend! */
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
	int result, voice_num;
	int min_voice = -1, min_core = -1;
	bool reused = false;
	
	if (core < number_of_cores)
	{
		// Look for a free voice on selected core
		voice_num = core * max_num_of_voices_per_core;
		for (voice_num; voice_num < (core + 1) * max_num_of_voices_per_core; voice_num++)
		{
			if (AdjSynth::get_instance()->synth_voice[voice_num] != NULL)
			{
				if (!(AdjSynth::get_instance()->synth_voice[voice_num]->audio_voice->is_voice_active() ||
					  AdjSynth::get_instance()->synth_voice[voice_num]->audio_voice->is_voice_wait_for_not_active()))
				{
					// A free voice
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (voice_num < max_number_of_voices)
		{
			return voice_num;
			//	printf("Min Core %i Free voice %i", core, voice);
		}
		else
		{
			return -1;
		}
	}
	else
	{
		{
			return -1;
		}
	}
}

/**
*   @brief  Returns the voice number of the voice which is active for the longest time
*   @param  none
*   @return the voice number of the voice which is active for the longest time
*/
int AdjPolyphonyManager::get_oldest_voice()
{			
	int i;
	int minvoice = -1;
	uint64_t mintime = UINT64_MAX;

	// Alocate first to be allocated in the past (oldest)
	for (i = 0; i < max_number_of_voices; i++)
	{
		if (AdjSynth::get_instance()->synth_voice[i] != NULL)
		{
			if (AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_timestamp() < mintime)
			{
				mintime = AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_timestamp();
				minvoice = i;
			}
		}
	}

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
	int i, result = -1, count = 0; 
	voice_resource_t duplicate_voices[_SYNTH_MAX_NUM_OF_VOICES]; 
	int64_t mintime = INT64_MAX;
	int minvoice = -1;

	
	if (mod_synth_get_cpu_utilization() > 90)
		// CPU is too loaded
	{	
		return -2;
	}

	if ((note < 0) || (note > 127) || (program < 0) ||
		(program >= AdjSynth::get_instance()->get_num_of_programs()))
		// Illegal parameters range
	{	
		return -3;
	}
	
	// Look if any active voice already produces this note and allocated to this program
	for (i = 0; i < AdjSynth::get_instance()->get_num_of_voices(); i++)
	{
		if (AdjSynth::get_instance()->synth_voice[i] != NULL)
		{
			if ((AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_active() ||
				AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_wait_for_not_active()) &&
				AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_note() == note &&
				AdjSynth::get_instance()->synth_voice[i]->get_allocated_program() == program)
			{
				duplicate_voices[count].timestamp = AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_timestamp();
				duplicate_voices[count++].id = AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_voice_num();
			}
		}
	}
	
	if (count > 0)
	{
		// At least one was found - Look for the 1st to become used
		for (i = 0; i < count; i++)
		{
			if (duplicate_voices[i].timestamp < mintime)
			{
				mintime = duplicate_voices[i].timestamp;
				minvoice = duplicate_voices[i].id;
			}
		}

		result = minvoice;
	}

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
	
	if ((res_num < 0) || (res_num >= AdjSynth::get_instance()->get_num_of_voices()) ||
		(note < 0) || (note > 127) || (program < 0) ||
		(program > AdjSynth::get_instance()->get_num_of_programs())) 
	{
		// Illegal parameters range
		return -1;
	}

	gettimeofday(&timestamp, NULL);
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->set_timestamp(
		(timestamp.tv_usec - start_time.tv_usec) / 1000 + (timestamp.tv_sec - start_time.tv_sec) * 1000);
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->set_active();
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->reset_wait_for_not_active();
	AdjSynth::get_instance()->synth_voice[res_num]->set_allocated_program(program);
	
	return 0;
}

/**
*   @brief  Free a voice.
*   @param  voice	voice number
*   @param	pending if set true, wait untill envelope is zero
*   @return tvoid
*/
void AdjPolyphonyManager::free_voice(int voice, bool pend)
{
	int program = 0, progvoice = 0;
	
	if ((voice >= 0) && (voice < AdjSynth::get_instance()->get_num_of_voices()))
	{
		int core = voice / AdjSynth::num_of_core_voices;

		if (pend)
		{
			// The voice will be free only when voice envelope will decay to zero.
			AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_wait_for_not_active();
			//	fprintf(stderr, "free pend %i ", voice);
		}
		else
		{
			// Free now
			program = AdjSynth::get_instance()->synth_voice[voice]->allocated_to_program_num;  
			progvoice = AdjSynth::get_instance()->synth_voice[voice]->allocated_to_program_voice_num;
			if (AdjSynth::get_instance()->get_midi_mapping_mode() == _MIDI_MAPPING_MODE_SKETCH)
			{
				program = AdjSynth::get_instance()->get_active_sketch();
			}
			fprintf(stderr, "free program: %i voice: %i\n", program, progvoice);

#ifdef _USE_NEW_MIDI_PROGRAM
			
#else
			AdjSynth::get_instance()->synth_program[program]->free_voice(progvoice);
#endif
			AdjSynth::get_instance()->synth_voice[voice]->audio_voice->reset_wait_for_not_active();
			AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_note(-1);
			AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_timestamp(0);
			AdjSynth::get_instance()->synth_voice[voice]->set_allocated_program(-1);
			//AdjSynth::get_instance()->synthVoice[voice] = NULL;
			//fprintf(stderr, "free %i ", voice);
			pthread_mutex_lock(&voice_busy_mutex);
			decrease_core_processing_load_weight(core, voice_processing_weight);
			pthread_mutex_unlock(&voice_busy_mutex);
			AdjSynth::get_instance()->mark_voice_not_busy_callback(voice);
			printf("Bussy %i %i %i %i\n",
				cores_load[0],
				cores_load[1], 
				cores_load[2],
				cores_load[3]);
		}
	}
}
