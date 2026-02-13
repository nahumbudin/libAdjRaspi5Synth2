/**
 *	@file		audioManager.cpp
 *	@author		Nahum Budin
 *	@date		23-Sep-2025
 *	@version	1.3
 *					1. Code refactoring and notaion.
 *					2. Bugs fix.
 *					3. Temp comment ALSA handler - to be implemented. TODO:
 *
 *	@brief		Manage and control audio system
 *
 *	History:
 *			Ver1.2	1-Oct-2024	Code refactoring and notaion.
 *			Ver1.1
 *				1. Code refactoring and notaion.
 *				2. Adding settings of sample-rate and audio block-size
 *				3. Adding callback to activate tasks when audio update cycle starts
 *				4. Adding call back to activate each voice update-cycle
 *				5. Adding callback to activate tasks when audio update cycle ends
 *				6. Moving Jack server activation from main.(start_jack_service())
 *
 *			Ver1.0	11-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October 18, 2017)
 *
 *
 */

#include <sys/shm.h>		//Used for shared memory
#include <sys/time.h>
// #include <omp.h>

#include "audioManager.h"
#include "audioCommons.h"
#include "../Misc/priorities.h"
#include "../commonDefs.h"
#include "../ALSA/alsaAudioHandling.h"
#include "../Jack/jackAudioClients.h"
#include "../LibAPI/synthesizer.h"

/* A pointer to the singleton AudioManageFloat instance */
AudioManager *AudioManager::audio_manager_instance = NULL;

func_ptr_void_int_t AudioManager::callback_audio_update_cycle_start_tasks_ptr = NULL;
func_ptr_void_int_t AudioManager::callback_audio_voice_update_ptr = NULL;
func_ptr_void_int_t AudioManager::callback_audio_update_cycle_end_tasks_ptr = NULL;

/* Mutexs to control polyphonic voices update process */
pthread_mutex_t update_mutex[_SYNTH_MAX_NUM_OF_VOICES];

// Mutex to controll audio memory blocks allocation 
extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

/* Update thread conditional variable signaling */
pthread_cond_t update_thread_cv = PTHREAD_COND_INITIALIZER;
/* Update thread control mutex */
pthread_mutex_t update_thread_mutex = PTHREAD_MUTEX_INITIALIZER; 

/* True when update thread is running, false otherwise */
bool update_thread_is_running = false;
/* True when update periodic timer thread is running, false otherwise */
bool periodic_update_timer_thread_is_running = false;

bool jack_out_connected = false, jack_in_connected = false;

/**
*   @brief  Create and return a pointer to the singleton AudioManageFloat instance.
*   @param  none
*   @return a pointer to the singleton AudioManageFloat instance
*/
AudioManager *AudioManager::get_instance() {
		
	if (!audio_manager_instance)
	{	
		audio_manager_instance = new AudioManager(1234);
	}
		
	return audio_manager_instance;
}

/**
*   @brief  Create an AudioManageFloat object instance.
*   @param  mseed	a random number used for allocating shared memory.
*   @return none
*/
AudioManager::AudioManager(int mseed)
{	
	mem_seed = mseed;
	if (mem_seed < 1)
	{
		fprintf(stderr, "Audio Manager: Memory Seed Error");
		return;
	}
	
	audio_service_started = false;
	
	audio_manager_instance = this;
	
	//alsa_handler = AlsaHandler::get_instance(); TODO: ALSA AUDIO <<<<<<<<<<<<<<<<<<<<<<<<<
	
	period_time_us = _DEFAULT_JACK_AUD_PERIOD_TIME_USEC;
	
	jack_thread_is_running = false;
	
	connections_manager = new AudioConnectionsManagerFloat();
	
	// Create shared memory blocks for audio data transfer
	create_audio_shared_memory(mem_seed);
	
	for (int i = 0; i < _SYNTH_MAX_NUM_OF_VOICES; i++) 
	{
		pthread_mutex_init(&update_mutex[i], NULL);
	}
	
	config_file = new JackConfigurationFile("/home/pi/AdjRaspi5Synth/JackConfig.txt"); // TODO: get path from configuration
}


AudioManager::~AudioManager()
{
	int core;
	
	//for (int i = 0; i < mod_synth_get_synthesizer_num_of_polyphonic_voices(); i++)
	//{	
	//	pthread_mutex_destroy(&update_mutex[i]);
	//}
	
	if (audio_service_started)
	{
		stop_audio_service();
	}
}

/**
*   @brief  Allocates shared memory segments for audio transfer.
*   @param  seed	a random number used for allocating shared memory.
*   @return 0 if ok
*/
int AudioManager::create_audio_shared_memory(int seed)
{
	int voice, err = 0;
	
	for (voice = 0; voice < _SYNTH_MAX_NUM_OF_VOICES; voice++)
	{
		//fprintf(stderr, "Audio MAnager: Creating voices outputs shared memory %i...\n", voice);
		audio_shared_mem_voices_output_ids[voice] =
			shmget((key_t)(38131 + _AUDIO_MAX_BUF_SIZE + 1 + voice + seed), 
			sizeof(shared_memory_audio_block_float_stereo_struct_t),
			0666 | IPC_CREAT);
		if (audio_shared_mem_voices_output_ids[voice] == -1)
		{
			err = errno;
			fprintf(stderr, "Audio-manager: Shared voices outputs shared memory voice %i shmget() failed err %s\r\n", voice, snd_strerror(err));
			exit(EXIT_FAILURE);
		}

		//Make the shared memory accessible to the program
		audio_shared_mem_voices_output_pointers[voice] = shmat(audio_shared_mem_voices_output_ids[voice], (void *)0, 0);
		if (audio_shared_mem_voices_output_pointers[voice] == (void *) - 1)
		{
			err = errno;
			fprintf(stderr, "Audio-manager: Shared memory voice %i shmat() failed err %s\r\n", voice, snd_strerror(err));
			exit(EXIT_FAILURE);
		}
	
		//	fprintf(stderr, "Shared memory %i attached at %i\r\n", voice, (int)audio_shared_mem_voices_output_pointers[voice]);
		//Assign the shared_memory segment to allocated memory segment
		audio_block_stereo_float_shared_memory_voices_output[voice] = 
			(shared_memory_audio_block_float_stereo_struct_t *)audio_shared_mem_voices_output_pointers[voice];
	}

	//	fprintf(stderr, "Creating stereo output shared memory %i...\n", voice);
	audio_shared_mem_stereo_output_id =
		/*shmget((key_t)(38151 + _AUDIO_MAX_BUF_SIZE + 1 + voice + seed),*/
		shmget((key_t)(38151 + _AUDIO_MAX_BUF_SIZE + 100 + seed),
		sizeof(shared_memory_audio_block_float_stereo_struct_t),
		0666 | IPC_CREAT);
	if (audio_shared_mem_stereo_output_id == -1)
	{
		err = errno;
		fprintf(stderr, "Audio-manager Shared stereo output shared memory voice %i shmget() failed err %s\r\n", 
				voice, snd_strerror(err));
		exit(EXIT_FAILURE);
	}

	//Make the shared memory accessible to the program
	audio_shared_mem_stereo_output_pointer = shmat(audio_shared_mem_stereo_output_id, (void *)0, 0);
	if (audio_shared_mem_stereo_output_pointer == (void *) - 1)
	{
		fprintf(stderr, "Audio-manager Sterso output Shared memory voice %i shmat() failed error %s\r\n", voice, snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	
	//	fprintf(stderr, "Stereo output Shared memory %i attached at %i\r\n", voice, (int)audio_shared_mem_stereo_output_pointer);
	
	//Assign the shared_memory segment to allocated memory segment
	audio_block_stereo_float_shared_memory_outputs = 
		(shared_memory_audio_block_float_stereo_struct_t *)audio_shared_mem_stereo_output_pointer;

	return err;
}

/**
*	@brief	Sets the sample-rate. Update also period_time
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).					
*	@return set sample-rate
*/	
int AudioManager::set_sample_rate(int samp_rate)
{
	if (audio_service_started)
	{
		stop_audio_service();
	}
	
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	// audioVoice->set_sample_rate
	
	period_time_us = calc_period_time_us(sample_rate, audio_block_size);
	
	if (audio_service_started)
	{
		start_audio_service();
	}
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int AudioManager::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size, time_period and the table phase step based on the size  
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int AudioManager::set_audio_block_size(int size)
{
	if (is_valid_audio_block_size(size))
	{
		if (audio_service_started)
		{	
			stop_audio_service();
		}
		
		audio_block_size = size;
		
		// audioVoice->set_block_size
		
		period_time_us = calc_period_time_us(sample_rate, audio_block_size);
		
		if (audio_service_started)
		{
			start_audio_service();
		}
		
		return audio_block_size;
	}
	else
	{
		return -1;
	}
}
	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int AudioManager::get_audio_block_size() {	return audio_block_size; }

/**
*   @brief  Starts the JACK audio service  
*   @param  mode			Jack mode:	_JACK_MODE_APP_CONTROL (application sets server params)
*										_JACK_MODE_SERVER_CONTROL (server sets app params)
*	@param	auto_start		_JACK_AUTO_START_DIS (app does not start JACK server)
*							_JACK_AUTO_START_EN (app starts JACK server)
*	@parma	auto_connect	_JACK_AUTO_CONNECT_DIS (app do not initiate connection automatically)
*							_JACK_AUTO_CONNECT_DIS (app initiates connection automatically)
*   @return 0 if OK
*/	
int AudioManager::start_jack_service(int mode, bool auto_start, bool auto_connect)
{
	int res, retry = 0;
	
	// Get audio manager and set sample-rate and buffer-size to app values
	initialize_jack_server_interface();
	// Set mode to app or server control
	res = set_jack_mode(mode);
	if (res < 0)
	{
		fprintf(stderr, "Audio manager: setting jack mode error\n");
		exit(-1);
	}
	
	if (auto_start)
	{
		enable_jack_auto_start();
	}
	else
	{
		disable_jack_auto_start();
	}
	
	if (auto_connect)
	{
		enable_jack_auto_connect_audio();
	}
	else
	{
		disable_jack_auto_connect_audio();
	}
	
	char process_name[] = "qjackctl"; 
	
	pid_t pid = Utils::get_process_id(process_name);
	
	if (get_jack_auto_start_state())
	{
		// start jack server
		if (pid == 0)
		{
			// QjackCtl is not already running
			system("qjackctl &"); // Run qjackctl in the background and return (&)
		}
		system("pactl unload-module module-jack-sink"); // clean previous run
		system("pactl unload-module module-jack-source");
		system("pactl load-module module-jack-sink channels=2");
		system("pactl load-module module-jack-source");
		system("pacmd set-default-sink jack_out");
	}
	
	if (get_jack_auto_connect_audio_state())
	{
		start_jack_connect_thread();
		while ((retry < 6) && !jack_out_connected && !jack_in_connected)
		{
			sleep(1); // TODO: wait for connection
		}		
		
		if (!jack_out_connected || !jack_in_connected)
		{
			fprintf(stderr, "Audio manager: auto connect not completted\n");
		}
		else
		{
			// Add more gaddgets and auto connect base on configuration file.
			
			//config_file->execute_configuration(); <<<<<<<<<<<<<<<<<<
		}
			
	}
	
	
	
	return 0;
}


/**
*   @brief  Start the audio services.
*   @param  driver		audio driver type - _AUDIO_ALSA or _AUDIO_JACK.
*	@param	samp_rate	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*						if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*	@param  block_size	audio block size:_AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 if ok
*/
int AudioManager::start_audio_service(int driver, int samp_rate, int block_size)
{
	//	startUpdateProcessPeriodicTimer();
	
	if (audio_service_started)
	{
		stop_audio_service();
	}
	
	audio_service_started = true;

	start_audio_update_thread();
	
	if (driver == _AUDIO_ALSA)
	{
		//		stop_jack_connect_thread();
		// disconnect_jack_audio_ports_out();TODO: ALSA AUDIO <<<<<<<<<<<<<<<<<<<<<<<<<
		// start_alsa_main_thread();TODO: ALSA AUDIO <<<<<<<<<<<<<<<<<<<<<<<<<
	}
	else if (driver == _AUDIO_JACK)
	{
		// stop_alsa_main_thread(); TODO: ALSA AUDIO <<<<<<<<<<<<<<<<<<<<<<<<<
		start_jack_connect_thread();
		
		int mode = get_jack_mode();
		bool auto_start = get_jack_auto_start_state();
		bool auto_connect = get_jack_auto_connect_audio_state();
		start_jack_service(mode, auto_start, auto_connect);
	}
	
	return 0;
}

/**
*   @brief  Stop the audio services.
*   @param  none.
*   @return 0 if ok
*/
int AudioManager::stop_audio_service()
{
	audio_service_started = false;
	stop_audio_update_thread();
	stop_jack_connect_thread();
	disconnect_jack_audio_ports_out();
	disconnect_jack_audio_ports_in();
	//stop_alsa_main_thread();TODO: ALSA AUDIO <<<<<<<<<<<<<<<<<<<<<<<<<
	
	return 0;
}

/**
*   @brief  Start the audio update thread.
*   @param  none
*   @return none
*/
void AudioManager::start_audio_update_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;	
	struct sched_param params;
	
	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_UPDATE;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// setting the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	err = errno;
	if (ret != 0) 
	{
		// Print the error
		fprintf(stderr, "Audio-manager: Unsuccessful in setting Update thread realtime prio\n");  
	}
	
	update_thread_is_running = true;
	
	ret = pthread_create(&update_thread_id, &tattr, AUDMNG_update_thread, NULL);
	pthread_setname_np(update_thread_id, "aud_mng_upda_thread");
}

/**
*   @brief  Stop the audio update thread.
*   @param  none
*   @return none
*/
void AudioManager::stop_audio_update_thread()
{
	update_thread_is_running = false;
}

/**
*   @brief  Start the main ALSA thread.
*   @param  none
*   @return none
*/
void AudioManager::start_alsa_main_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_ALSA;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// setting the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	err = errno;
	if (ret != 0) {
		// Print the error
		fprintf(stderr, "Unsuccessful in setting ALSA Main thread realtime prio\n");
	}
	
	alsa_handler->thread_is_running = true;
	period_time_us = calc_period_time_us(sample_rate, audio_block_size);
	set_period_time_us(period_time_us);
	// Run audio thread
	pthread_create(&alsa_thread_id, &tattr, AUDMNG_run_alsa, (void *)1);
	pthread_setname_np(alsa_thread_id, "aud_mng_alsa_tethread");
}

/**
*   @brief  Stop the main ALSA thread.
*   @param  none
*   @return none
*/
void AudioManager::stop_alsa_main_thread()
{
	alsa_handler->thread_is_running = false;
}

/**
*   @brief  Start the JACK connect thread.
*   @param  none
*   @return none
*/
void AudioManager::start_jack_connect_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_JACK;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// setting the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	err = errno;
	if (ret != 0) 
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting JACK Main thread realtime prio\n");
	}
	
	jack_thread_is_running = true;
	// Run audio thread
	ret = pthread_create(&jack_thread_id, &tattr, AUDMNG_try_connect_jack, (void *)1);
	pthread_setname_np(jack_thread_id, "aud_mng_jack_connect_tethread");
}

/**
*   @brief  Stop the JACK connect thread.
*   @param  none
*   @return none
*/
void AudioManager::stop_jack_connect_thread()
{
	jack_thread_is_running = false;
}

/**
*   @brief  Set period time [uSec] of the periodic timer.
*   @param  ptu	period time uSec
*   @return 0
*/
int AudioManager::set_period_time_us(unsigned long ptu) { 
	
	period_time_us = ptu;
	
	return 0;
}
/**
*   @brief  Get period time [uSec] of the periodic timer.
*   @param  none	
*   @return period time uSec
*/
unsigned long AudioManager::get_period_time_us() 	
{ 
	
	return period_time_us; 

}

/**
*   @brief  start the periodic update timer (period_time usec).
*   @param  none
*   @return 0
*/
int AudioManager::start_update_process_periodic_timer()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;

	if (period_time_us < 1000)
	{
		// period < 1msec
		fprintf(stderr, "Unsuccessful in starting process update periodic timer thread: period < 1msec\n");
		exit(1);
	}

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_UPDATE_TIMER;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// setting the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	err = errno;
	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting process update periodic timer thread realtime prio\n");
	}

	periodic_update_timer_thread_is_running = true;
	// Run audio thread
	ret = pthread_create(&process_periodic_timer_thread_id, &tattr, AUDMNG_periodic_update_timer_thread, (void *)1);
	pthread_setname_np(process_periodic_timer_thread_id, "aud_mng_updat_timer_tethread");

	return ret;
}

/**
*   @brief  stop the periodic update timer .
*   @param  none
*   @return 0
*/
int AudioManager::stop_update_process_periodic_timer()
{
	periodic_update_timer_thread_is_running = false;
	return 0;
}

/**
*   @brief  A callback function that is called at the begining of every audio update cycle.
*			Registered function should be small as possible.
*   @param  none
*   @return 0
*/
void AudioManager::callback_audio_update_cycle_start_tasks(int param)
{
	if (callback_audio_update_cycle_start_tasks_ptr)
	{
		callback_audio_update_cycle_start_tasks_ptr(param);
	}
}

/**
*   @brief  Registera  callback function that is called at the begining of every audio update cycle.
*			Registered function should be small as possible.
*   @param  none
*   @return 0
*/
void AudioManager::register_callback_audio_update_cycle_start_tasks(func_ptr_void_int_t ptr)
{
	callback_audio_update_cycle_start_tasks_ptr = ptr;
}

/**
*   @brief  A callback function that is called to activate every audio voice block update cycle.
*			Registered function should be small as possible.
*   @param  none
*   @return 0
*/
void AudioManager::callback_audio_voice_update(int voice_num)
{
	if (callback_audio_voice_update_ptr)
	{
		callback_audio_voice_update_ptr(voice_num);
	}
}

/**
*   @brief  Registera  callback function that is called at the begining of every audio update cycle.
*			Registered function should be small as possible.
*   @param  none
*   @return 0
*/
void AudioManager::register_callback_audio_voice_update(func_ptr_void_int_t ptr)
{
	callback_audio_voice_update_ptr = ptr;
}

/**
*   @brief  A callback function that is called at the end of every audio update cycle.
*			Registered function should be small as possible.
*   @param  none
*   @return 0
*/
void AudioManager::callback_audio_update_cycle_end_tasks(int param)
{
	if (callback_audio_update_cycle_end_tasks_ptr)
	{
		callback_audio_update_cycle_end_tasks_ptr(param);
	}
}

/**
*   @brief  Registera  callback function that is called at the end of every audio update cycle.
*			Registered function should be small as possible.
*   @param  none
*   @return 0
*/
void AudioManager::register_callback_audio_update_cycle_end_tasks(func_ptr_void_int_t ptr)
{
	callback_audio_update_cycle_end_tasks_ptr = ptr;
}

/**
*   @brief  Main audio-block processing update thread
*   @param  arg a pointer to a void argument (not in use)
*   @return void*
*/
void* AUDMNG_update_thread(void *arg)
{
	int lockCount = 0, voice, count;
	int ID;
	int core, utilization;
	struct timeval start_ts, prv_start_ts;
	struct timeval stop_ts;
	
	unsigned long period_time_us;
	
	while (update_thread_is_running)
	{
		
		pthread_mutex_lock(&update_thread_mutex);
		pthread_cond_wait(&update_thread_cv, &update_thread_mutex);
		pthread_mutex_unlock(&update_thread_mutex);
			
		// Activate update cycle start tasks (e.g. ModSynth::update_tasks() )
		if (AudioManager::callback_audio_update_cycle_start_tasks_ptr)
		{
			AudioManager::callback_audio_update_cycle_start_tasks_ptr(0); // 0 - dummy param
		}

		prv_start_ts.tv_usec = start_ts.tv_usec;
		gettimeofday(&start_ts, NULL);

		//fprintf(stderr, "Time from last block: %i Time from prev block end time [us]: %i\n", 
		//		start_ts.tv_usec- prv_start_ts.tv_usec, start_ts.tv_usec - stop_ts.tv_usec);

		//omp_set_num_threads(4/*Synthesizer::numOfCores*/);
				
		int i;

//#pragma omp parallel if (AdjSynth::num_of_cores > 1) //private(voice)
//		{
//#pragma omp for schedule(static)	
		for (voice = 0; voice < mod_synth_get_synthesizer_num_of_polyphonic_voices(); voice++)
		{
			if(AdjSynth::synth_voice[voice]->audio_voice->is_voice_active() ||
					AdjSynth::synth_voice[voice]->audio_voice->is_voice_wait_for_not_active())

			{
				// Activate each voice update
				if (AudioManager::callback_audio_voice_update_ptr)
				{
					AudioManager::callback_audio_voice_update_ptr(voice);
				}
										
				//gettimeofday(&stopts, NULL);
				//printf("Core: %i Th: %i  voice: %i  : %i \n", core, ID, voice, stopts.tv_usec-startts.tv_usec);
			}
		}
			
//	}//#pragma omp parallel
		
		// Update common blocks: poly-mixer, reverb, stereo-output	
		if (AudioManager::callback_audio_update_cycle_end_tasks_ptr)
		{
			AudioManager::callback_audio_update_cycle_end_tasks_ptr(0); // 0 - dummy param
		}
		// Below should be in the above callback
//		AudioBlockFloat *p;
//		for (p = *AdjSynth::get_instance()->audio_poly_mixer->audio_first_update; p; p = p->audio_next_update)
//		{
//			p->update();
//		}

		// Global LFOs Modulators update
		for (int samp = 0; samp < AudioManager::get_instance()->get_audio_block_size() / _CONTROL_SUB_SAMPLING; samp++)
		{
			AdjSynth::get_instance()->global_lfo_1_output_value[samp] = 
				AdjSynth::get_instance()->global_lfo_1->get_next_output_val(AdjSynth::get_instance()->global_lfo_1_actual_freq);
			
			AdjSynth::get_instance()->global_lfo_2_output_value[samp] =
				AdjSynth::get_instance()->global_lfo_2->get_next_output_val(AdjSynth::get_instance()->global_lfo_2_actual_freq);
			
			AdjSynth::get_instance()->global_lfo_3_output_value[samp] =
				AdjSynth::get_instance()->global_lfo_3->get_next_output_val(AdjSynth::get_instance()->global_lfo_3_actual_freq);
			
			AdjSynth::get_instance()->global_lfo_4_output_value[samp] =
				AdjSynth::get_instance()->global_lfo_4->get_next_output_val(AdjSynth::get_instance()->global_lfo_4_actual_freq);
			
			AdjSynth::get_instance()->global_lfo_5_output_value[samp] =
				AdjSynth::get_instance()->global_lfo_5->get_next_output_val(AdjSynth::get_instance()->global_lfo_5_actual_freq);
			
			AdjSynth::get_instance()->global_lfo_6_output_value[samp] =
				AdjSynth::get_instance()->global_lfo_6->get_next_output_val(AdjSynth::get_instance()->global_lfo_6_actual_freq);
			
		}

		gettimeofday(&stop_ts, NULL);		

		count++;
		if ((count % 40) == 0)
		{
			period_time_us = AudioManager::get_instance()->get_period_time_us();
		
			if (period_time_us < 1000)
			{
				// period < 1msec
				fprintf(stderr, "Audio update thread: period < 1msec\n");
				exit(1);
			}			
			
			utilization = (int)(float(stop_ts.tv_usec - start_ts.tv_usec) / (float)period_time_us * 100.0);
			//callback_update_utilization_bar(utilization);
			fprintf(stderr, "Update Cycle time= %i\n", stop_ts.tv_usec - start_ts.tv_usec);
			count = 0;
		}

		lockCount = 0;

	}
	
	return NULL;
}


// Initiate audio thread
void *AUDMNG_run_alsa(void *threadid) {
		
	
	unsigned long period_time_us = AudioManager::get_instance()->get_period_time_us();
		
	if (period_time_us < 1000)
	{
		// period < 1msec
		fprintf(stderr, "Run ALSA thread: period < 1msec\n");
		exit(1);
	}	
	
	//long tid = (long)threadid;
	//tid++;
	
	pthread_t thId = pthread_self();

	//	pthread_setname_np(thId, "alsa_thread");

	AlsaHandler::get_instance()->set_period_time_us(period_time_us);
	AlsaHandler::get_instance()->init();
	
	return NULL;
}

void *AUDMNG_try_connect_jack(void *threadid) {
		
	int res, retry_out, retry_in = 0;
	
	//long tid = (long)threadid;
	//tid++;
	
	pthread_t thId = pthread_self();
	
	//	pthread_setname_np(thId, "jack_thread");

	
	while (retry_out < 5)
	{
		res = intilize_jack_server_connection_out("AdjHeartSynth_out", "default");
		if (res != 0)
		{
			fprintf(stderr, "Jack connect thread: Jack (out) not started");
			retry_out++;
			sleep(1);
		}
		else
		{
			jack_out_connected = true;
			break;
		}
	}
	
	while (retry_in < 5)
	{
		res = intilize_jack_server_connection_in("AdjHeartSynth_in", "default");
		if (res != 0)
		{
			fprintf(stderr, "Jack connect thread: Jack (in) not started");
			retry_in++;
			sleep(1);
		}
		else
		{
			jack_in_connected = true;
			break;
		}
	}
	
	return NULL;
}

void *AUDMNG_periodic_update_timer_thread(void *threadid)
{
	unsigned long period_time_us;
	
	while (periodic_update_timer_thread_is_running)
	{
		period_time_us = AudioManager::get_instance()->get_period_time_us();
		
		if (period_time_us < 1000)
		{
			// period < 1msec
			fprintf(stderr, "Periodic timer thread: period < 1msec\n");
			exit(1);
		}
		
		usleep(period_time_us);

		// Triger update process
		if (pthread_mutex_trylock(&update_thread_mutex) == 0)
		{
			// Signal update thread
			pthread_cond_signal(&update_thread_cv);
			pthread_mutex_unlock(&update_thread_mutex);
		}
	}

	return NULL;
}
