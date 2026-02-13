/**
*	@file		audioManager.h
*	@author		Nahum Budin
*	@date		1-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	1.1
*					1. Code refactoring and notaion.
*					2. Adding settings of sample-rate and audio block-size
*					3. Adding callback to activate tasks when audio update cycle starts
*					4. Adding call back to activate each voice update-cycle
*					5. Adding callback to activate tasks when audio update cycle ends
*					6. Moving Jack server activation from main.(start_jack_service())
*				1.0	11-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October 18, 2017)
*
*	@brief		Manage and control audio system
*/

#pragma once

//#include "../headers.h"

#include <pthread.h>

#include "../libAdjRaspi5Synth2.h"
#include "../Jack/JackConfigurationFile.h"

#include "audioCommons.h"
#include "audioVoice.h"
#include "audioBlock.h"

#include "../LibAPI/audio.h"
#include "../LibAPI/synthesizer.h"
#include "../LibAPI/types.h"

#include "../ALSA/alsaAudioHandling.h"

extern pthread_mutex_t update_mutex[_SYNTH_MAX_NUM_OF_VOICES];

class AudioVoiceFloat;

class AudioManager {
public:
		
	~AudioManager();
	
	static AudioManager *get_instance();
	
	int create_audio_shared_memory(int seed);
	
	AudioVoiceFloat *get_audio_voice(int voice_num = 0);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	int start_jack_service(
					int mode = _DEFAULT_JACK_MODE, 
		bool auto_start = _DEFAULT_JACK_AUTO_START,
		bool auto_connect = _DEFAULT_JACK_AUTO_CONNECT_AUDIO);
	
	int start_audio_service(
					int driver = _DEFAULT_AUDIO_DRIVER, 
		int samp_rate = _DEFAULT_SAMPLE_RATE, 
		int block_size = _DEFAULT_BLOCK_SIZE);
	int stop_audio_service();
	void start_audio_update_thread();
	void stop_audio_update_thread();	
	void start_alsa_main_thread();
	void stop_alsa_main_thread();	
	void start_jack_connect_thread();
	void stop_jack_connect_thread();	
	
	int set_period_time_us(unsigned long ptu);
	unsigned long get_period_time_us();
	int start_update_process_periodic_timer();
	int stop_update_process_periodic_timer();
	
	void callback_audio_update_cycle_start_tasks(int param);
	void register_callback_audio_update_cycle_start_tasks(func_ptr_void_int_t ptr);
	
	void callback_audio_voice_update(int voice_num);
	void register_callback_audio_voice_update(func_ptr_void_int_t ptr);
	
	void callback_audio_update_cycle_end_tasks(int param);
	void register_callback_audio_update_cycle_end_tasks(func_ptr_void_int_t ptr);

	//	AlsaLibHandle alsa_handler;
	
		// Shared memory to transfer audio blocks to the alsa library handler
		//	void *audio_shared_mem_pointer1, *audio_shared_mem_pointer2, *audio_shared_mem_pointer3, *audio_shared_mem_pointer4;
		//	int audio_shared_mem_id1, audio_shared_mem_id2, audio_shared_mem_id3, audio_shared_mem_id4;
		//	shared_memory_audio_block_float_stereo_struct_t *audio_block_stereo_float_shared_memory1;
		//	shared_memory_audio_block_float_stereo_struct_t *audio_block_stereo_float_shared_memory2;
		//	shared_memory_audio_block_float_stereo_struct_t *audio_block_stereo_float_shared_memory3;
		//	shared_memory_audio_block_float_stereo_struct_t *audio_block_stereo_float_shared_memory4;

	void *audio_shared_mem_voices_output_pointers[_SYNTH_MAX_NUM_OF_VOICES];
	int audio_shared_mem_voices_output_ids[_SYNTH_MAX_NUM_OF_VOICES];
	shared_memory_audio_block_float_stereo_struct_t 
		*audio_block_stereo_float_shared_memory_voices_output[_SYNTH_MAX_NUM_OF_VOICES];

	void *audio_shared_mem_stereo_output_pointer;
	int audio_shared_mem_stereo_output_id;
	
	shared_memory_audio_block_float_stereo_struct_t *audio_block_stereo_float_shared_memory_outputs;
	
	AudioConnectionsManagerFloat *connections_manager;
	
	static func_ptr_void_int_t callback_audio_update_cycle_start_tasks_ptr;
	static func_ptr_void_int_t callback_audio_voice_update_ptr;
	static func_ptr_void_int_t callback_audio_update_cycle_end_tasks_ptr;
	
	
private:
	
	AudioManager(int mseed);
	
	static AudioManager *audio_manager_instance;
	
	AlsaHandler *alsa_handler;
	
	JackConfigurationFile* config_file = NULL;
	
	pthread_t update_thread_id;
	pthread_t alsa_thread_id;	
	pthread_t jack_thread_id;
	pthread_t process_periodic_timer_thread_id;

	bool jack_thread_is_running;
	
	// period time in us 
	unsigned long period_time_us;

	int mem_seed;
	
	int sample_rate;
	int audio_block_size;

	bool audio_service_started;
	
};

// Main thread running audio updates 
void *AUDMNG_update_thread(void *arg);

void *AUDMNG_run_alsa(void *threadid);

void *AUDMNG_try_connect_jack(void *threadid);

void *AUDMNG_periodic_update_timer_thread(void *threadid);
