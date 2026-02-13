/**
*	@file		adjSynthPolyphony.cpp
*	@author		Nahum Budin
*	@date		3-Feb-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					
*	@version	1.0		15-Nov-2019 
*
*	@brief		Handle AdjSynth polyphony
*/
	
#include <sys/time.h>

#include "adjSynthPolyphony.h"
//#include "adjSynth.h"
//#include "synthKeyboard.h"
//#include "adjSynthProgram.h"

#include "../commonDefs.h"

extern pthread_mutex_t voice_busy_mutex;

AdjSynthPolyphony *AdjSynthPolyphony::synth_polyphony = NULL;

AdjSynthPolyphony::AdjSynthPolyphony()
{
	synth_polyphony = this;

	for (int i = 0; i < _SYNTH_MAX_NUM_OF_CORES; i++)
	{	
		busy_core_voices[i] = 0;
	}

	gettimeofday(&start_time, NULL);
}

AdjSynthPolyphony *AdjSynthPolyphony::get_instance()
{
	if (synth_polyphony == NULL)
	{	
		synth_polyphony = new AdjSynthPolyphony();
	}

	return synth_polyphony;
}

/**
*   @brief  Returns a voice num that already assigned to this note and program
*   @param  note	requested note
*   @param	program	requested program
*   @return a voice num that is already assigned to this note and program;
*			-2 if CPU is too loaded; -3 if params are out of range
*/
int AdjSynthPolyphony::get_reused_note(int note, int program)
{
	return 0;
	/*
	int i, result = -1;
	
	if (mod_synth_get_cpu_utilization() > 90)
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
	
	// Look if any active voice already produces this note and allocated to this program
	for (i = 0; i < mod_synth_get_synthesizer_num_of_polyphonic_voices(); i++)
	{
		if (AdjSynth::get_instance()->synth_voice[i] != NULL)
		{
			if ((AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_active() ||
				AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_wait_for_not_active()) &&
				AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_note() == note &&
				AdjSynth::get_instance()->synth_voice[i]->get_allocated_program() == program)
			{
				result = i;
				break;
			}
		}
	}

	return result;
	*/
}

/**
*   @brief  Returns the number of core that runs the smallest number of voices
*   @param  none
*   @return the number of core that runs the smallest number of voices
*/
int AdjSynthPolyphony::get_less_busy_core()
{
	return 0;
	
	/*
	int i = 0, min_voices, min_core, result;
	
	// Look for the less busy core
	min_voices = 1000;
	min_core = 1000;
	for (i = 0; i < mod_synth_get_number_of_cores(); i++)
	{
		if (busy_core_voices[i] < min_voices)
		{
			min_voices = busy_core_voices[i];
			min_core = i;
		}
	}

	if (min_core >= mod_synth_get_number_of_cores()) 
	{
		// No min core found - error
		return -1;
	}
	else 
	{
		return min_core;
	}
	
	*/
}

/**
*   @brief  Returns the number of a non busy voice on a selected core
*   @param  core selected core
*   @return the number of a non busy voice on a selected core 
*/
int AdjSynthPolyphony::get_free_voice(int core)
{
	return 0;
	
	/*
	int result, voice = 0;
	//	struct timeval timestamp;
	int min_voice = -1, min_core = -1;
	bool reused = false;

	result = -1;
	// Look for a free voice on selected core
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
		else
		{
			break;
		}
	}

	if (voice < mod_synth_get_synthesizer_num_of_polyphonic_voices())
	{
		result = voice;
		//	printf("Min Core %i Free voice %i", core, voice);
	}

	return result;
	*/
}

/**
*   @brief  Returns the voice number of the voice which is active for the longest time
*   @param  none
*   @return the voice number of the voice which is active for the longest time
*/
int AdjSynthPolyphony::get_oldest_voice()
{			
	return 0;
	
	/*
	int i;
	int minvoice = -1;
	uint64_t mintime = UINT64_MAX;

	// Alocate first to be allocated in the past (oldest)
	for (i = 0; i < mod_synth_get_synthesizer_num_of_polyphonic_voices(); i++)
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
	*/
}

/**
*   @brief  Returns the voice number of the voice that is part of the
*			provided program and plays the provided note
*   @param  note	note number
*   @param	program	program number
*   @return the voice number of the voice that is part of the
*			provided program and plays the provided note;
*			-1 if paramters are out of range
*/
int AdjSynthPolyphony::get_voice_note(int note, int program)
{
	return 0;
	
	/*
	int result = -1;
	int i = 0, count = 0;
	voice_resource_t duplicate_voices[_SYNTH_MAX_NUM_OF_VOICES];
	int64_t mintime = INT64_MAX;
	int minvoice = -1;

	if ((note < 0) || (note > 127) || (program < 0) ||
		(program >= mod_synth_get_synthesizer_num_of_programs()))
	{	
		// Illegal parameters range
		return -1;
	}

	for (i = 0; i < mod_synth_get_synthesizer_num_of_polyphonic_voices(); i++)
	{
		// Look if this note is assigned to this program and still active (may be more than 1)
		if (AdjSynth::get_instance()->synth_voice[i] != NULL)
		{
			if ((AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_note() == note) &&
				(AdjSynth::get_instance()->synth_voice[i]->get_allocated_program() == program) &&
				AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_active() &&
				!AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_wait_for_not_active())
			{
				//			duplicatevoices[count++] = voices[i];
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
	//	if (result < 0)
	//		fprintf(stderr, "getvoice not found ");

	return result;
	*/
}

/**
*   @brief  Free a voice.
*   @param  voice	voice number
*   @param	pending if set true, wait untill envelope is zero
*   @return tvoid
*/
void AdjSynthPolyphony::free_voice(int voice, bool pend)
{
	int program = 0, progvoice = 0;
	
	/*
	
	if ((voice >= 0) && (voice < mod_synth_get_synthesizer_num_of_polyphonic_voices()))
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

			AdjSynth::get_instance()->synth_program[program]->free_voice(progvoice);

			AdjSynth::get_instance()->synth_voice[voice]->audio_voice->reset_wait_for_not_active();
			AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_note(-1);
			AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_timestamp(0);
			AdjSynth::get_instance()->synth_voice[voice]->set_allocated_program(-1);
			//AdjSynth::get_instance()->synthVoice[voice] = NULL;
			//fprintf(stderr, "free %i ", voice);
			pthread_mutex_lock(&voice_busy_mutex);
			dec_busy_core_voices_count(core);
			pthread_mutex_unlock(&voice_busy_mutex);
			AdjSynth::get_instance()->mark_voice_not_busy_callback(voice);
			printf("Bussy %i %i %i %i\n",
				busy_core_voices[0],
				busy_core_voices[1], 
				busy_core_voices[2],
				busy_core_voices[3]);
		}
	}
	*/
}

/**
*   @brief  Activate a voice resource..
*   @param  res	voice number
*   @param	note	specified note
*   @param	program	specified program
*   @return 0 if done.
*/
int AdjSynthPolyphony::activate_resource(int res_num, int note, int program)
{
	return 0;
	
	/*
	
	struct timeval timestamp;
	bool reused = false;
	
	if ((res_num < 0) || (res_num >= mod_synth_get_synthesizer_num_of_polyphonic_voices()) ||
		(note < 0) || (note > 127) || (program < 0) ||
		(program > mod_synth_get_synthesizer_num_of_programs())) 
	{
		// Illegal parameters range
		return -1;
	}

	gettimeofday(&timestamp, NULL);
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->set_timestamp((timestamp.tv_usec - start_time.tv_usec) / 1000 + (timestamp.tv_sec - start_time.tv_sec) * 1000);
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->set_active();
	AdjSynth::get_instance()->synth_voice[res_num]->audio_voice->reset_wait_for_not_active();
	AdjSynth::get_instance()->synth_voice[res_num]->set_allocated_program(program);
	
	return 0;
	
	*/
}

/**
*   @brief  Increment the number of voices provided core is using.
*   @param  core core number
*   @return void
*/
void AdjSynthPolyphony::inc_busy_core_voices_count(int core)
{
	/*
	if ((core >= 0) && (core < mod_synth_get_number_of_cores()))
	{
		if (busy_core_voices[core] < AdjSynth::num_of_core_voices - 1)
		{
			busy_core_voices[core]++;
		}
	}
	*/
}

/**
*   @brief  Decrement the number of voices provided core is using.
*   @param  core core number
*   @return void
*/
void AdjSynthPolyphony::dec_busy_core_voices_count(int core)
{
	/*
	if ((core >= 0) && (core < mod_synth_get_number_of_cores()))
	{
		if (busy_core_voices[core] > 0)
		{
			busy_core_voices[core]--;
		}
	}
	*/
}

/**
*   @brief  Clear (set to zero) the number of voices provided core is using.
*   @param  core core number
*   @return void
*/
void AdjSynthPolyphony::clear_busy_core_voices_count(int core)
{
	/*
	if ((core >= 0) && (core < mod_synth_get_number_of_cores()))
	{
		if (busy_core_voices[core] > 0)
		{
			busy_core_voices[core] = 0;
		}
	}
	*/
}

/**
*   @brief  Return the number of voices provided core is using.
*   @param  core core number
*   @return void
*/
int AdjSynthPolyphony::get_busy_core_voices_count(int core)
{
	return 0;
	
	/*
	if ((core >= 0) && (core < mod_synth_get_number_of_cores()))
	{
		return busy_core_voices[core];
	}
	else
	{	
		return -1;
	}
	*/
}
