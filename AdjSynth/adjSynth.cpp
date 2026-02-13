/**
 *	@file		adjSynth.h
 *	@author		Nahum Budin
 *	@date		23-Sep-2025
 *	@version	1.4
 *					1. Code refactoring and notaion.
 *					2. Bugs fix.
 *					3. Redfining the Programs concept Programs.h
 *					4. Adding support in both old and new MIDI program objects.
 *					5. Polyphony - Not manging cores loads - let the OS do it.
 *					6. "Manua" copy of settings structure (mutx added)
 *					7. Adding DSP Out setting parameters: gain, pan, send, lfo and lfo level
 *					8. Adding voice send parameters
 *					9. Changing main Amp gain and send update callbacks to non block callbacks.
 *					10. Adding Panic action
 *					11. Refactoring lfo_delays[] -> global array in adjSynth.
 *					12. Adding global LFOs for modulation.
*					
*	@brief		A collection of 4 synthesizers: Additive, Karplus String, PAD and Morphed Sine Oscilator (MSO)
*					
*	@brief		A collection of 4 synthesizers: Additive, Noise, Karplus String, PAD and Morphed Sine Oscilator (MSO)
*				Note - the FluidSynth is not handeled by this object. 
*	
*	History:\n
*	
*	version 1.3		4-Oct-2024	Code refactoring and notaion.*	
*	version 1.2		4-Feb--2021:
*			1. Code refactoring and notaion.
*			2. Adding sample-rate and bloc-size settings
*			3. Adding start/stop audio
*	version	1.1		18-Jan-2021: Adding audio parameters settings
*	version 1.0		15_Nov-2019: First version
*/

#include <mutex>

//#include <omp.h>

#include  <thread>

#include "adjSynth.h"
#include "../utils/utils.h"
#include "../Audio/audioBlock.h"
#include "../Audio/audioBandEqualizer.h"
#include "../Jack/jackAudioClients.h"
#include "../Audio/audioPolyphonyMixer.h"
#include "../MIDI/midiStream.h"
#include "../Settings/settings.h"
#include "../DSP/dspVoice.h"
#include "../Instrument/instrumentMidiPlayer.h"

class DSP_Voice;

/* Holds the delay values of all LFO index selection */
uint32_t AdjSynth::lfo_delays[_NUM_OF_LFOS * _NUM_OF_LFO_DELAY_OPTIONS + 1]; // 6 LFOs, 5 states; 1 None

/* This is the top most object of the synthesizer - a singltone. */
AdjSynth *AdjSynth::adj_synth = NULL;

/* A monophonic signal generator and processor that generates audio blocks. 
 * Plyphony is created by using multiple voice objects */
SynthVoice *AdjSynth::synth_voice[_SYNTH_MAX_NUM_OF_VOICES] = { NULL };

/* This object manges the allocation of voices to create a polyphonic 
   capabilities  - a singltone. See more detaile in adjSynthPolyphonyManagerThreads.h */
AdjPolyphonyManager *AdjSynth::synth_polyphony_manager = NULL;

/* This the polyphony manager main thread - ststic */
AdjPolyphonyManager *AdjSynth::synth_polyphony_manager_threads = NULL;

/* Holds the number of detected CPU cores (4 for Raspberry Pi 5)  - static. */
int AdjSynth::num_of_cores = 1;

/* Represents the number of voice objects that are allocated to a CPU core 
   at current time  - static. */
int AdjSynth::num_of_core_voices = 1;

/* A mutex to handle note on / off opperations */
pthread_mutex_t voice_manage_mutex;

/* A mutex to handle busy/not-busy voices marking */
pthread_mutex_t voice_busy_mutex;

/* Mutex to controll audio memory blocks allocation */
pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

/* A callback function that is initiated by the AudioManager audio-update thread
   The audio update thread calculates the given voice number output audio block,
   and send it to the audio system mixer. */ 
void callback_audio_voice_update(int voice_num)
{
	if (AdjSynth::synth_voice[voice_num]->audio_voice->is_voice_active() ||
		AdjSynth::synth_voice[voice_num]->audio_voice->is_voice_wait_for_not_active())
	{
		// Run only if the voice is active or the voice is in a Release segment and
		//	we now wait until the Release phase will end so the voice object can be released. 
		AdjSynth::synth_voice[voice_num]->update_all();
	}
}

/*  A callback that is initiated by the AudioManager audio-update thread, that updates all
 *  the voices and audio blocks of the last stage of the audio pipeline. 
 *  TODO: why is there an int param a parameter? (callback template?) */
void callback_audio_update_cycle_end_tasks(int param)
{
	AudioBlockFloat *p;
	for (p = *AdjSynth::get_instance()->audio_polyphony_mixer->audio_first_update; p; p = p->audio_next_update)
	{
		p->update();
	}
}

/* A callback that is initiated by the SynthProg object to set a group of voices
   with a the given parameters. All the group voices are marked as a Program.
   A program reperesents a specific sound, like in MIDI, for example a piano program. */
int set_patch_settings_default_params_callback_wrapper(_settings_params_t *params, int prog)
{
	return AdjSynth::get_instance()->set_default_preset_parameters(params, prog);
}

AdjSynth::AdjSynth()
{
	/* Holds a voice number. */
	int voice;
	
	/* Holds the singletone instance of the AdjHeart object. */
	adj_synth = this;

	// Create a settings managers
	adj_synth_settings_manager = new Settings(&active_adj_synth_settings_params);
	// Initialize the settings parameters structure.
	//set_default_preset_parameters(&active_adj_synth_settings_params, 0);

	// TODO: reorgenize the various settings types used in previous version.
	
	/* Holds the available number of polyphonic voices. */
	num_of_voices = _SYNTH_MAX_NUM_OF_VOICES;
	
	/* Holds the available number of programs (piano, flute, custom-sound.....)*/
	num_of_programs = _SYNTH_MAX_NUM_OF_PROGRAMS;
	
	/* Holds the total CPU utilization in %. */
	cpu_utilization = 0;
	
	/* Specific settings of the Hammond Organ instrument. */ // TODO: why here?
	hammond_percussion_on = false;
	hammond_percussion_slow = false;
	hammond_percussion_soft = false; 
	hammond_ercussion_3_rd = false;
	
	/* The id of the active sketch. Sketch-program 1 to 3 represents an online editted sketches. */
	active_sketch = _SKETCH_PROGRAM_1;

	// Allocate midi blocks data memory pool. Pre allocation makes execution much faster.
	allocate_midi_stream_messages_memory_pool(_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE);
	allocate_raw_data_mssgs_memory_pool(_MAX_RAWDATA_MSSGS_POOL_SIZE);

	init_lfo_delays();

	// Get the number of cpu cores.
	num_of_cores = std::thread::hardware_concurrency();
	if (num_of_cores < 1)
	{
		num_of_cores = 1;
	}
	
	// Calculate the max number of voices per core
	num_of_core_voices = mod_synth_get_synthesizer_num_of_polyphonic_voices() / num_of_cores;

	// Calculate the max number of voices that can be allocated to the CPU N-1 cores. 
	if (num_of_cores > 1)
	{
		num_of_core_voices = mod_synth_get_synthesizer_num_of_polyphonic_voices() / (num_of_cores - 1);
	}
	else
	{
		num_of_core_voices = mod_synth_get_synthesizer_num_of_polyphonic_voices();
	}	

	// Init the mutexes.
	pthread_mutex_init(&voice_manage_mutex, NULL);
	pthread_mutex_init(&voice_busy_mutex, NULL);
	pthread_mutex_init(&voice_mem_blocks_allocation_control_mutex, NULL);

	// Set the main audio parameters.
	set_sample_rate(_DEFAULT_SAMPLE_RATE);
	set_audio_block_size(_DEFAULT_BLOCK_SIZE);

	// Allocate audio blocks data memory pool. Pre allocation save time at runtime.
	AllocateAudioMemoryBlocksFloatPool(_MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE, audio_block_size);
	
	// Create a Keyboard-control  instance
	kbd1 = new SynthKeyboard(sample_rate, audio_block_size);
	// Do not change with patch TODO: ?
	poly_mode = _KBD_POLY_MODE_REUSE; // When all voices are allocated, reuse the oldest one. // _KBD_POLY_MODE_FIFO;
	play_mode = _PLAY_MODE_POLY; // Not monophonic
	midi_mapping_mode = _DEFAULT_MIDI_MAPPING_MODE; // Mapping mode

	// Create the polyphony manager.
	synth_polyphony_manager = AdjPolyphonyManager::get_poly_manger_instance(num_of_voices);
	
	// Create and set an audio manager singletone - Don't change the order (stage - TODO:)
	audio_manager = AudioManager::get_instance();
	audio_manager->set_sample_rate(sample_rate);
	audio_manager->set_audio_block_size(audio_block_size);

	sleep(5); // Wait a bit to let audio manager settle.

	// Calulate the audio-blocks perid in us.
	audio_manager->set_period_time_us(((unsigned long)audio_block_size * 1000000) / sample_rate + 0.5);	
	
	// A call back that initiates audio block update cycle.
	audio_manager->register_callback_audio_voice_update(&callback_audio_voice_update);
	// A call back that is called when the audio blocks update cycle is completed.
	audio_manager->register_callback_audio_update_cycle_end_tasks(&callback_audio_update_cycle_end_tasks);

	// A Wavetable of float samples handling. Samples may be output as 2 values using 2 pointers.
	//   This wavetable is used by a program that is based on the PAD synthesizer module.
	program_wavetable = new Wavetable();
	program_wavetable->size = _PAD_DEFAULT_WAVETABLE_SIZE;
	program_wavetable->samples = (float*)malloc(_PAD_DEFAULT_WAVETABLE_SIZE * sizeof(float));
	
	/* This object is used to creat wavetables for the PAD synthesizer module. */
	synth_pad_creator = new SynthPADcreator(program_wavetable, program_wavetable->size, sample_rate);
	program_wavetable->base_freq =
		synth_pad_creator->set_base_frequency(program_wavetable, _PAD_DEFAULT_BASE_NOTE);

	/* A wave table that is used by the Morphed Sinus Oscilator synthesizer module. */
	mso_wtab = new DSP_MorphingSinusOscWTAB(sample_rate);
	// Calculate the base waveform parameters.
	mso_wtab->calc_segments_lengths(&mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->base_waveform_tab, &mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	// Calculate the morphed waveform parameters.
	mso_wtab->calc_segments_lengths(&mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->morphed_waveform_tab, &mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);

// TODO:?
	/* Create an Audio Mixer object singletone. */

	global_lfo_1 = new DSP_Osc(voice + 2000,
						_OSC_WAVEFORM_SINE,
						50,
						0,
						0,
						0,
						false,
						false,
						false,
						0.0f,
						0.0f,
						_OSC_UNISON_MODE_12345678);
	set_global_lfo_1_frequency((float)10);

	global_lfo_2 = new DSP_Osc(voice + 2000,
							   _OSC_WAVEFORM_SINE,
							   50,
							   0,
							   0,
							   0,
							   false,
							   false,
							   false,
							   0.0f,
							   0.0f,
							   _OSC_UNISON_MODE_12345678);
	set_global_lfo_2_frequency((float)10);

	global_lfo_3 = new DSP_Osc(voice + 2000,
							   _OSC_WAVEFORM_SINE,
							   50,
							   0,
							   0,
							   0,
							   false,
							   false,
							   false,
							   0.0f,
							   0.0f,
							   _OSC_UNISON_MODE_12345678);
	set_global_lfo_3_frequency((float)10);

	global_lfo_4 = new DSP_Osc(voice + 2000,
							   _OSC_WAVEFORM_SINE,
							   50,
							   0,
							   0,
							   0,
							   false,
							   false,
							   false,
							   0.0f,
							   0.0f,
							   _OSC_UNISON_MODE_12345678);
	set_global_lfo_4_frequency((float)10);

	global_lfo_5 = new DSP_Osc(voice + 2400,
							   _OSC_WAVEFORM_SINE,
							   50,
							   0,
							   0,
							   0,
							   false,
							   false,
							   false,
							   0.0f,
							   0.0f,
							   _OSC_UNISON_MODE_12345678);
	set_global_lfo_5_frequency((float)10);

	global_lfo_6 = new DSP_Osc(voice + 2500,
							   _OSC_WAVEFORM_SINE,
							   50,
							   0,
							   0,
							   0,
							   false,
							   false,
							   false,
							   0.0f,
							   0.0f,
							   _OSC_UNISON_MODE_12345678);
	set_global_lfo_6_frequency((float)10);

#ifdef _USE_NEW_POLY_MIXER_
	audio_polyphony_mixer = AudioPolyMixer::get_instance(
		_AUDIO_STAGE_0,
		num_of_voices,
		audio_block_size,
		_SYNTH_MAX_NUM_OF_PROGRAMS,
		_DEFAULT_MIDI_MAPPING_MODE,
		&audio_common_first_update);

#else
	audio_polyphony_mixer = AudioPolyphonyMixerFloat::get_instance(
		_AUDIO_STAGE_0,
		num_of_voices,
		_SYNTH_MAX_NUM_OF_PROGRAMS,
		audio_block_size,
		_DEFAULT_MIDI_MAPPING_MODE,
		&audio_common_first_update);

	
#endif

	audio_polyphony_mixer->set_active();

	/* An Audio Equalizer object. */
	audio_equalizer = new AudioBandEqualizer(_AUDIO_STAGE_0, audio_block_size, &audio_common_first_update);
	
	/* A reverb module (common to all voices) */
	audio_reverb = new AudioReverb(_AUDIO_STAGE_0, sample_rate, audio_block_size, &audio_common_first_update);

	/* An audio output object - get the audio block stream and use a shared memory to transfer to Jack. */
	audio_out = new AudioOutputFloat(_AUDIO_STAGE_0,
		audio_block_size,
		audio_manager->audio_block_stereo_float_shared_memory_outputs,
		&audio_common_first_update,
		100); // Main output id

	/* An audio connections for the mixer output. */
	connection_mixer_out_L = audio_manager->connections_manager->get_audio_connection();
	connection_mixer_out_R = audio_manager->connections_manager->get_audio_connection();

	/* An audio connections for the eqalizer output. */
	connection_equilizer_out_L = audio_manager->connections_manager->get_audio_connection();
	connection_equilizer_out_R = audio_manager->connections_manager->get_audio_connection();

	/* An audio connections for the reverb output. */
	connection_reverb_out_L = audio_manager->connections_manager->get_audio_connection();
	connection_reverb_out_R = audio_manager->connections_manager->get_audio_connection();

	/* An audio connections for the mixer send output (routed to effects). */
	connection_mixer_send_L = audio_manager->connections_manager->get_audio_connection();
	connection_mixer_send_R = audio_manager->connections_manager->get_audio_connection();
	
	
	// Temporarlly connect poly mixer output to audio out  TODO: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	connection_mixer_out_L->connect(audio_polyphony_mixer, _LEFT, audio_out, _LEFT);
	connection_mixer_out_R->connect(audio_polyphony_mixer, _RIGHT, audio_out, _RIGHT);
	
	
	
	// Connect the mixer output to the equilizer input.
	//connection_mixer_out_L->connect(audio_polyphony_mixer, _LEFT, audio_equalizer, _LEFT);
	//connection_mixer_out_R->connect(audio_polyphony_mixer, _RIGHT, audio_equalizer, _RIGHT);

	// Connect the mixer send output to the reverb input.
	connection_mixer_send_L->connect(audio_polyphony_mixer, _SEND_LEFT, audio_reverb, _LEFT);
	connection_mixer_send_R->connect(audio_polyphony_mixer, _SEND_RIGHT, audio_reverb, _RIGHT);	
	
	// Temporarally Connect reverb output to audio out input. TODO: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	connection_reverb_out_L->connect(audio_reverb, _LEFT, audio_out, _LEFT);
	connection_reverb_out_R->connect(audio_reverb, _RIGHT, audio_out, _RIGHT);

	// Connect the reverb output to the equilizer input.
	//connection_reverb_out_L->connect(audio_reverb, _LEFT, audio_equalizer, _SEND_LEFT);
	//connection_reverb_out_R->connect(audio_reverb, _RIGHT, audio_equalizer, _SEND_RIGHT);

	// Connect the equilizer output to the audio output input.
	//connection_equilizer_out_L->connect(audio_equalizer, _LEFT, audio_out, _LEFT);
	//connection_equilizer_out_R->connect(audio_equalizer, _RIGHT, audio_out, _RIGHT);
	
	
	
	// Set audio parameters.
	set_audio_driver_type(_DEFAULT_AUDIO_DRIVER);
	set_sample_rate(_DEFAULT_SAMPLE_RATE);
	set_audio_block_size(_DEFAULT_BLOCK_SIZE);
	
	// Call only after sample rate is set.
	init_lfo_delays();
}

AdjSynth::~AdjSynth()
{
	// Nothin hjere yet.
}

/**
*   @brief  retruns the single (singltone) AdjSynth instance
*   @param  none
*   @return the single modular synth instance
*/
AdjSynth *AdjSynth::get_instance()
{
	if (adj_synth == NULL)
	{
		adj_synth = new AdjSynth();
	}
	
	return adj_synth;
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int AdjSynth::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}

	// TODO: reallocate audio blocks.?

	if (kbd1)
	{
		kbd1->set_sample_rate(sample_rate);
	}

	if (audio_manager)
	{
		audio_manager->set_period_time_us(((unsigned long)audio_block_size * 1000000) / sample_rate + 0.5);
	}

	if (mso_wtab)
	{
		mso_wtab->set_sample_rate(sample_rate);
	}

	if (audio_reverb)
	{
		audio_reverb->set_sample_rate(sample_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int AdjSynth::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return size if OK; -1 param out of range
*/
int AdjSynth::set_audio_block_size(int size)
{	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;

		if (kbd1)
		{
			kbd1->set_audio_block_size(audio_block_size);
		}

		if (audio_manager)
		{
			audio_manager->set_period_time_us(((unsigned long)audio_block_size * 1000000) / sample_rate + 0.5);
		}

		if (audio_polyphony_mixer)
		{
			audio_polyphony_mixer->set_audio_block_size(audio_block_size);
		}

		if (audio_equalizer)
		{
			audio_equalizer->set_audio_block_size(audio_block_size);
		}

		if (audio_reverb)
		{
			audio_reverb->set_audio_block_size(audio_block_size);
		}

		if (audio_out)
		{
			audio_out->set_audio_block_size(audio_block_size);
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
int AdjSynth::get_audio_block_size() 
{ 
	return audio_block_size; 
}

/**
*	@brief	Sets the audio driver type
*			Settings will be effective only after the next call to start_audio().
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@return set audio-driver
*/
int AdjSynth::set_audio_driver_type(int driver)
{
	if (is_valid_audio_driver(driver))
	{
		audio_driver_type = driver;
	}
	else
	{
		audio_driver_type = _DEFAULT_AUDIO_DRIVER;
	}
	
	return audio_driver_type;
}

/**
*	@brief	Returns the audio-driver type
*	@param	none
*	@return audio driver type
*/	
int AdjSynth::get_audio_driver_type() 
{ 
	return audio_driver_type; 
}

/**
*   @brief  Starts the audio service.
*			Must be called only after intializtion.
*   @param  none
*   @return 0 if done
*/
int AdjSynth::start_audio(int driver, int samp_rate, int block_size)
{
	int res = 0;
	
	stop_audio();
	
	set_audio_driver_type(driver);
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
	
	return audio_manager->start_audio_service(driver, sample_rate, audio_block_size);
}

/**
*   @brief  Stops the audio service.
*   @param  none
*   @return 0 if done
*/
int AdjSynth::stop_audio()
{
	return audio_manager->stop_audio_service();
}

/**
*   @brief  retruns a pointer to an original main dsp voice 
*			(original dsp voice settings to be reproduced when a dsp voice is disallocated by a voice program).
*   @param  voice voice number
*   @return  pointer to an original main dsp voice if OK; NULL if param out of range
*/	
DSP_Voice *AdjSynth::get_original_main_dsp_voices(int voice)
{
	if ((voice >= 0) && (voice < num_of_voices))
	{	
		return original_main_dsp_voices[voice];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Create and initialize all the synth voice instances and
*			set synthvoice instances to point at program[0] 
*   @param  none
*   @return void
*/
void AdjSynth::init_synth_voices()
{
	for (int voice = 0; voice < num_of_voices; voice++)
	{
		synth_voice[voice] = new SynthVoice(
			voice,
			active_sketch,
			sample_rate,
			audio_block_size,
			&active_adj_synth_preset_params,
			mso_wtab,
			program_wavetable,
			audio_manager);

#ifdef _USE_NEW_MIDI_PROGRAM
		
#else		
		// Assign a DSP_Voice object instance
		synth_voice[voice]->assign_dsp_voice(synth_program[active_sketch]->synth_voices[voice]->dsp_voice);
#endif
		
		// Assingn LUTs  TODO: 13-Oct-2025 needed?
		synth_voice[voice]->mso_wtab = synth_program[active_sketch]->mso_wtab;
		synth_voice[voice]->pad_wavetable = synth_program[active_sketch]->program_wavetable;
	}
}

/**
*   @brief  Create and initialize the synth programs instances
*   @param  none
*   @return void
*/
void AdjSynth::init_synth_programs()
{
	// Program[16] to Program[18] are the active non MIDI mapping mode programs used for editting analog patches, etc.
	// Program[0] to Program[15] are MIDI-mapping mode programs used each for a MIDI channel 1-16

#ifdef _USE_NEW_MIDI_PROGRAM
	for (int program = 0; program < num_of_programs; program++)
	{
		_settings_params_t *default_params = new _settings_params_t;
		set_default_preset_parameters(default_params, 0);
		synth_program[program] = new AdjSynthPrograms(program, _PAD_DEFAULT_WAVETABLE_SIZE,
													   default_params, audio_polyphony_mixer);
	}
#else
	for (int program = 0; program < num_of_programs; program++)
	{
		synth_program[program] = new SynthProgram(
			sample_rate,
			audio_block_size,
			num_of_voices,
			0, // 1st voice num'
			_PAD_DEFAULT_WAVETABLE_SIZE,
			audio_manager);
	}
#endif 
	
}

/**
*   @brief  initialize lfo delay times (0, 500, 1000, 1500, 2000ms) sub sampling count   
*   @param  none
*   @return 0;
*/
int AdjSynth::init_lfo_delays()
{
	int lfo_num, state;
	uint32_t lfo_500_ms_delay_cont = sample_rate / _CONTROL_SUB_SAMPLING / 2;       
	
	lfo_delays[0] = 0; // None LFO
	
	for (state = 0; state < _NUM_OF_LFO_DELAY_OPTIONS; state++)
	{
		for (lfo_num = 0; lfo_num < _NUM_OF_LFOS; lfo_num++)
		{
			lfo_delays[state * _NUM_OF_LFOS + lfo_num + 1] = lfo_500_ms_delay_cont * state;
		}
	}
	
	return 0;
}


/**
*   @brief   set the adj synth master volume
*   @param  vol	volume level 0-100
*   @return void
*/
void AdjSynth::set_master_volume(int vol)
{
	if ((vol >= 0) && (vol <= 100))
	{
		master_volume = vol;
		audio_out->set_master_volume(vol);
	}
}

/**
*   @brief  get the adj synth master volume
*   @param	none 
*   @return volume level 0-100
*/
int AdjSynth::get_master_volume()
{
	return master_volume;
}

/**
 *	@brief	Set LFO_1 frequency
 *	@param	freq	LFO_1 frequncy 0-100
 *		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
 *	@return void
 */
void AdjSynth::set_global_lfo_1_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	global_lfo_1_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (global_lfo_1_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		global_lfo_1_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *	@brief	Set LFO_2 frequency
 *	@param	freq	LFO_2 frequncy 0-100
 *		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
 *	@return void
 */
void AdjSynth::set_global_lfo_2_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	global_lfo_2_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (global_lfo_2_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		global_lfo_2_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *	@brief	Set LFO_3 frequency
 *	@param	freq	LFO_3 frequncy 0-100
 *		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
 *	@return void
 */
void AdjSynth::set_global_lfo_3_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	global_lfo_3_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (global_lfo_3_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		global_lfo_3_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *	@brief	Set LFO_4 frequency
 *	@param	freq	LFO_4 frequncy 0-100
 *		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
 *	@return void
 */
void AdjSynth::set_global_lfo_4_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	global_lfo_4_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (global_lfo_4_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		global_lfo_4_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *	@brief	Set LFO_5 frequency
 *	@param	freq	LFO_5 frequncy 0-100
 *		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
 *	@return void
 */
void AdjSynth::set_global_lfo_5_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	global_lfo_5_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (global_lfo_5_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		global_lfo_5_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *	@brief	Set LFO_6 frequency
 *	@param	freq	LFO_6 frequncy 0-100
 *		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
 *	@return void
 */
void AdjSynth::set_global_lfo_6_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	global_lfo_6_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (global_lfo_6_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		global_lfo_6_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*   @brief  Init AdjSynth settings parameters (keyboard,equilizer, reverb)
*   @param  none
*   @return void
*/
int AdjSynth::init_synth_settings_params(_settings_params_t *settings_params)
{
	if (settings_params == NULL)
	{
		return -1;
	}
	
	int res = 0;

	_settings_bool_param_t bool_param;
	_settings_int_param_t int_param;

	res = adj_synth_settings_manager->get_bool_param(settings_params, 
							"adjsynth.reverb3m.enable_state", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		if (bool_param.value)
		{
			audio_reverb->rev3m_enable();
		}
		else
		{
			audio_reverb->rev3m_disable();
		}
	}

	res |= adj_synth_settings_manager->get_bool_param(settings_params, 
						"adjsynth.reverb.enable_state", &bool_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		if (bool_param.value)
		{
			audio_reverb->rev_enable();
		}
		else
		{
			audio_reverb->rev_disable();
		}
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb3m.preset", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		sf_presetreverb(&audio_reverb->rv3m, sample_rate, (sf_reverb_preset)int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.room_size", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_room_size(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.damp", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_damp(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.wet", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_wet(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.dry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_dry(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.mode", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_mode(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_31_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_31_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_31_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_62_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_62_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_62_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_125_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_125_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_125_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_250_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_250_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_250_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_500_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_500_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_500_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_1k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_1000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_1000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_2k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_2000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_2000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_4k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_4000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_4000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_8k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_8000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_8000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_16k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_16000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_16000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.preset", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_preset(int_param.value);
		audio_equalizer->band_equalizer_R->set_preset(int_param.value);
	}
	
	// TODO: Mixer?
	
	return res;	
}

/**
*   @brief  Retruns the actual max num of voices
*   @param  none
*   @return the actual max num of voices
*/
int AdjSynth::get_num_of_voices() 
{ 
	return num_of_voices; 
}

/**
*   @brief  Retruns the actual max num of Midi Programs
*   @param  none
*   @return the actual max num of Midi Programs
*/
int AdjSynth::get_num_of_programs() 
{ 
	return num_of_programs; 
}

/**
*   @brief  Sets the active sketch num
*   @param  apt	the active sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @return void
*/
void AdjSynth::set_active_sketch(int ask)
{
	if ((ask >= _SKETCH_PROGRAM_1) && (ask <= _SKETCH_PROGRAM_3))
	{
		active_sketch = ask;
	}
}

/**
*   @brief  Retruns the active sketch num
*   @param  none
*   @return the active sketch num _SKETCH_1 to _SKETCH_3
*/
int AdjSynth::get_active_sketch() 
{ 
	return active_sketch; 
}

/**
*   @brief  Copy source sketch params to a destination sketch params
*   @param  int srcsk	sorce sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @param  int destsk	destination sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @return 0 if done
*/
int AdjSynth::copy_sketch(int srcsk, int destsk)
{
	if ((srcsk == destsk) || (srcsk < _SKETCH_PROGRAM_1) || (srcsk > _SKETCH_PROGRAM_3) ||
		(destsk < _SKETCH_PROGRAM_1) || (destsk > _SKETCH_PROGRAM_3))
	{
		return -1;
	}
	else
	{
#ifdef _USE_NEW_MIDI_PROGRAM
		synth_program[destsk]->set_program_preset_params(synth_program[srcsk]->active_preset_params);
#else
		synth_program[destsk]->set_program_preset_params(&synth_program[srcsk]->active_preset_params);
#endif		
		
		
	}

	return 0;
}

/**
*   @brief  Initilize the voices polyphonic state and paramters
*   @param  none
*   @return void
*/
void AdjSynth::init_poly()
{
	for (int voice = 0; voice < _SYNTH_MAX_NUM_OF_VOICES; voice++)
	{
		if (synth_voice[voice] != NULL)
		{	
			synth_voice[voice]->audio_voice->init_poly();
		}
		// TODO: currently not in use - mark voice is bussy /not busy on GUI
		//mark_voice_not_busy_callback(voice);

		for (int core = 0; core < AdjSynth::synth_polyphony_manager->get_number_of_cores(); core++)
		{	
			AdjSynth::synth_polyphony_manager->clear_core_processing_load_weight(core);
		}

		for (int program = 0; program < _SYNTH_MAX_NUM_OF_PROGRAMS; program++)
		{
			SynthVoice *synth_voice = NULL;

#ifdef _USE_NEW_MIDI_PROGRAM
			synth_voice = synth_program[program]->get_voice(voice);
#else
			synth_voice = synth_program[program]->synth_voices[voice];
#endif			
			if (synth_voice != NULL)
			{	
				synth_voice->dsp_voice->not_in_use();
			}
		}
	}
}

/**
*   @brief  Start the jack out thread processing and connect to the Jackserver
*   @param	none
*   @return void
*/
void AdjSynth::init_jack()
{
	int res;
	sleep(1); // TODO: why?
	res = initialize_jack_server_interface();
	// TODO: can the remarked lines be removed?
	//	res |= intilize_jack_server_connection_out("AdjHeartSynth_out", "default");
	//	res |= intilize_jack_server_connection_in("AdjHeartSynth_in", "default");
}

/**
 *   @brief  Turn all active voices and notes off
 *   @param	none
 *   @return void
 */
void AdjSynth::synth_panic_ection()
{
	InstrumentMidiPlayer::get_instrument_midi_player_instance()->send_all_notes_off_command();
	InstrumentMidiPlayer::get_instrument_midi_player_instance()->send_all_sounds_off_command();
}

/**
*   @brief  Start the audio update process. - Implemented above.
*   @param	none
*   @return void
*/
//void AdjSynth::start_audio()
//{
//#ifdef _USE_ALSA
//	audioManager->startAudioService(_AUDIO_ALSA);
//#else
//	audio_manager->start_audio_service(_AUDIO_JACK);
//#endif
//}

/**
*   @brief  Set MIDI mapping mode.
*			In non mapping mode, MIDI channel is ignored.
*			Non mapping mode enables editing and creating patches.
*			In mapping mode, patches can only be loaded into each program/channel.
*   @param	mod	_MIDI_MAPPING_MODE_NON_MAPPING or _MIDI_MAPPING_MODE_MAPPING
*   @return void
*/
void AdjSynth::set_midi_mapping_mode(int mod)
{
	if (mod == _MIDI_MAPPING_MODE_SKETCH)
	{
		midi_mapping_mode = _MIDI_MAPPING_MODE_SKETCH;
		
		pthread_mutex_lock(&voice_manage_mutex);

		//		for (int voice = 0; voice < _SYNTH_MAX_NUM_OF_VOICES; voice++) TODO: ?
		//		{
					// Assign all voices their original (non-mapung mode) params.
					//			synth_voice[voice]->assignDspVoice(originalMainDspVoices[voice]);
					//			synth_voice[voice]->msoLUT = synth_voice[voice]->dspVoice->original_msoLUT;
					//			synth_voice[voice]->padWavetable = synth_voice[voice]->dspVoice->original_padWavetable;

					// Set all voices settings to point at porogram[0] active patch settings
					//			init_synth_voices();
					//		}
		
					//		init_synth_voices();

		init_poly();

		//	synth_program[0]->set_num_of_voices(num_of_voices); TODO: ?

		pthread_mutex_unlock(&voice_manage_mutex);
	}
	else if (mod == _MIDI_MAPPING_MODE_MAPPING)
	{
		midi_mapping_mode = _MIDI_MAPPING_MODE_MAPPING;
	}

	audio_polyphony_mixer->set_midi_mapping_mode(midi_mapping_mode);
}

int AdjSynth::get_midi_mapping_mode() 
{ 
	return midi_mapping_mode; 
}

/**
*   @brief  Set the settings handling object and the params structure
*   @param	settings		a pointer to a ModSynthSettings settings handling object
*   @param	patch_param		a pointer to a _setting_params_t params structure holding patch params
*   @param	settings_params	a pointer to a _setting_params_t params structure holding settings params
*   @return 0 if done
*/
int AdjSynth::set_settings_params(Settings *settings, 
	//								  /*_setting_params_t *patch_params,*/ 
	_settings_params_t *settings_params)
{
	return_val_if_true(settings_params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);

	adj_synth_settings_manager = settings;
	
	///active_adj_synth_settings_params = *settings_params;

	active_adj_synth_settings_params.bool_parameters_map = settings_params->bool_parameters_map;
	active_adj_synth_settings_params.float_parameters_map = settings_params->float_parameters_map;
	active_adj_synth_settings_params.int_parameters_map = settings_params->int_parameters_map;
	active_adj_synth_settings_params.string_parameters_map = settings_params->string_parameters_map;

	active_adj_synth_settings_params.name = "default_settings";
	active_adj_synth_settings_params.settings_type = "instrument_settings_param";
	active_adj_synth_settings_params.version = settings->get_settings_version();

	// Amp level, pan send mixer settings not preset
	
	return 0;
}

/**
*   @brief  Gets a program preset name
*   @param  prog	program num (1-16)
*   @return program patch name
*/
std::string AdjSynth::get_program_preset_name(int prog)
{
	if ((prog >= 0) && (prog <= 16))
	{
#ifdef _USE_NEW_MIDI_PROGRAM
		return synth_program[prog]->active_preset_params->name;
#else
		return synth_program[prog]->active_preset_params.name;
#endif		
		
	}
	else
	{
		return string("--------");
	}
}

/**
*   @brief  retruns a pointer to the active preset params struct
*   @param  none
*   @return a pointer to the active preset params struct
*/
_settings_params_t *AdjSynth::get_active_preset_params()
{
#ifdef _USE_NEW_MIDI_PROGRAM
	return synth_program[active_sketch]->active_preset_params;
#else
	return &synth_program[active_sketch]->active_preset_params;
#endif
	
	
}

/**
*   @brief  retruns a pointer to the active settings params struct
*   @param  none
*   @return a pointer to the active settings params struct
*/
_settings_params_t *AdjSynth::get_active_settings_params()
{
	return &active_adj_synth_settings_params;
}

/**
*   @brief  retruns a pointer to the active general settings params struct TODO: reevaluate the setting params types.
*   @param  none
*   @return a pointer to the active general settings params struct
*/
//_setting_params_t *AdjSynth::get_active_general_settings_params()
//{
//	return &active_adj_synth_general_settings_params;
//}

/**
*   @brief  Set the preset parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_preset_parameters(_settings_params_t *params, int prog)
{
	int res = 0;
	
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = set_default_preset_parameters_vco(params, prog);
	res |= set_default_preset_parameters_noise(params, prog);
	res |= set_default_preset_parameters_kps(params, prog);
	res |= set_default_preset_parameters_mso(params, prog);
	res |= set_default_preset_parameters_pad(params, prog);
	res |= set_default_preset_parameters_amp(params, prog);
	res |= set_default_preset_parameters_distortion(params, prog);
	res |= set_default_preset_parameters_modulators(params, prog);

	res |= set_default_preset_parameters_filter(params, prog);
	
	res |= set_default_preset_parameters_polyphonic_mixer(params, prog);

	res |= set_default_preset_parameters_output(params, prog);

	return res;
}

/**
*   @brief  Set the settings parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_settings_parameters(_settings_params_t *params)
{	
	int res = 0;
	
	res = set_default_settings_parameters_equalizer(params);
	res |= set_default_settings_parameters_reverb(params);
	res |= set_default_settings_parameters_keyboard(params);
	res |= set_default_settings_parameters_mixer(params);

	//	// TODO: GLobal part of modsynth
	//	res |= set_default_settings_parameters_mixer(params);
	//	res |= set_default_settings_parameters_keyboard(params);
	
	return res;
}

/**
*   @brief  Set the general settings parameters to their default values TODO: reevaluate the setting params types.
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
/*
int AdjSynth::set_default_general_settings_parameters(_setting_params_t *params)
{
	int res = 0;

	res = set_default_settings_parameters_audio(params);

	//	// TODO: GLobal part of modsynth
	//	res |= set_default_settings_parameters_mixer(params);
	//	res |= set_default_settings_parameters_keyboard(params);

	return res;
}
*/

// UI updates callbacks initiation

void AdjSynth::mark_voice_not_busy_callback(int vnum)
{
	// Used to update the UI when a voice is freed - TODO: currentlly not implemented
	//callback_mark_voice_not_busy(vnum);
}

void AdjSynth::mark_voice_busy_callback(int vnum)
{
	// TODO:
	//callback_mark_voice_busy(vnum);
}

void AdjSynth::set_cpu_utilization_callback(int util)
{
	// TODO:
}

int AdjSynth::get_cpu_utilization_callback()
{
	return cpu_utilization; // TODO: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< utilization not set
}

void AdjSynth::update_ui_callback()
{
	// TODO:
}


/**
*   @brief  Play note on 
*			Allocate free voice
*			In portamento mode use momophonic voice 0 only.
*			In non mapping mode assign selected free voice with selected program params
*   @param	channel	MIDI channel: 0-15 patch 1-3: 16-18
*	@param	byte2	note midi num
*	@param	byte3	note velocity (if set to 0, note off will be executed).
*	@param	voc		voice (NA).
*   @return void
*/
void  AdjSynth::midi_play_note_on(uint8_t channel, uint8_t byte2, uint8_t byte3, int voc)
{
	int voice, core, scaledMagnitude, prog = 0;
	bool reused = false;
	SynthVoice *prog_voice = NULL;

	if (byte3 == 0)
	// Note off
	{
		midi_play_note_off(channel, byte2, byte3);

		return;
	}
	
	if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING) // TODO:
	{
		prog = channel;
	}
	else
	{
		prog = active_sketch;
	}

	if (kbd1->portamento_is_enabled()) // TODO mobe portamento to programs?
	{
		// TODO: Polyphonic portamento?
		synth_voice[_SYNTH_VOICE_1]->audio_voice->set_active();
		synth_voice[_SYNTH_VOICE_1]->audio_voice->reset_wait_for_not_active();
		mark_voice_busy_callback(_SYNTH_VOICE_1);
		voice = _SYNTH_VOICE_1;
	}

	voice = -1;

	// New - Polyphony manager version
	pthread_mutex_lock(&voice_manage_mutex);
	
	voice = synth_polyphony_manager->get_voice((int)byte2, prog); // TODO: program = 0??? old note
	
	if (voice < 0)
	{
		// No free voice found!
		pthread_mutex_unlock(&voice_manage_mutex);
		fprintf(stderr, "midi_play_note_on: No free voice found!\n");
		
		return;
	}

	// Voice found

	// Debug
	fprintf(stderr, "midi_play_note_on: %i voice: %i program: %i \n", byte2, voice, prog);

	
	// Asign the voice to the program
	synth_program[prog]->assign_voice_with_preset_program_params(synth_voice[voice], voice);

	// Assingn LUTs - done in program assignment 13-Oct-2025
	//synth_voice[voice]->mso_wtab = synth_program[prog]->mso_wtab;
	//synth_voice[voice]->pad_wavetable = synth_program[prog]->program_wavetable;

	// Initialize voice metadata (timestamp, program allocation) but DON'T activate yet
	synth_polyphony_manager->activate_resource(voice, (int)byte2, prog);
	//		kbd1->voices[voice].note = byte2;
	
	synth_voice[voice]->audio_voice->set_note(byte2);
	fprintf(stderr, "Voice %i on\n", voice);

	// Calculate and set the note frequency
	kbd1->midi_play_note_on(channel, byte2, byte3);

	// Velocity scaling based on keyboard split point and velocity
	if (kbd1->get_split_point() == _KBD_SPLIT_POINT_NONE)
	{
		// No split point - general velocity scaling
		scaledMagnitude = kbd1->get_scaled_velocity(byte3);
	}
	else if ((int)byte2 >= 12 * (kbd1->get_split_point() + 3))
	{
		// Above split point - general velocity scaling
		scaledMagnitude = kbd1->get_scaled_velocity(byte3);
	}
	else
	{
		// Below split point - low velocity scaling
		scaledMagnitude = kbd1->get_low_scaled_velocity(byte3);
	}

	// Set the voice parameters
	synth_voice[voice]->dsp_voice->set_voice_frequency(kbd1->get_note_frequency());

	synth_voice[voice]->dsp_voice->filter_1->set_kbd_freq(kbd1->get_note_frequency());
	synth_voice[voice]->dsp_voice->filter_2->set_kbd_freq(kbd1->get_note_frequency());

	synth_voice[voice]->audio_voice->set_output_level((float)scaledMagnitude / 127);
	synth_voice[voice]->dsp_voice->karplus_1->note_on(byte2, (float)scaledMagnitude / 127);

	// Trigger the ADSR envelopes
	synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_1);
	synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_2);
	synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_3);
	synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_4);
	synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_5);
	synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_6);

	// Ensure all writes above are completed and visible to all CPU cores
	// before marking the voice as active
	__sync_synchronize();

	// NOW mark voice as active - all initialization is complete
	// Note: activate_resource already set it active, but we're ensuring proper state
	// If you removed set_active from activate_resource, uncomment the line below:
	synth_voice[voice]->audio_voice->set_active();

	pthread_mutex_unlock(&voice_manage_mutex);

	/*   Old - Core management version
		else
		{
			if (poly_mode == _KBD_POLY_MODE_REUSE) // Reuse a voice that is already playing the same note
			{
				voice = synth_polyphony_manager->get_reused_note((int)byte2, prog); // TODO: program = 0
				if (voice > -1)
				{
					reused = true;
				}
			}
		}

		if (voice < 0)
		{
			// Not found yet
			core = synth_polyphony_manager->get_less_busy_core();
			// Look for a free voice
			voice = synth_polyphony_manager->get_a_free_voice(core);

			if ((voice < 0) && (poly_mode == _KBD_POLY_MODE_FIFO))
			{
				// Not found yet - get the oldest active
				voice = synth_polyphony_manager->get_oldest_voice();
			}
		}


			if ((voice > -1) && !reused)
		{
			//	moved up
			 //	if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
			//		{
			//			prog = channel;
			//	}
			//	else
			//	{
			//		prog = active_sketch;
			}

			// Voice found and if not reused, get a free voice from mapped program
			//prog_voice = synth_program[prog]->get_free_voice();
			voice = synth_polyphony_manager->get_a_free_voice(core);
			//Old - Core management version
						  
			//if (!prog_voice)
			//	voice = -1; 

	if (voice < 0)
	{
		pthread_mutex_unlock(&voice_manage_mutex);
		fprintf(stderr, "midi_play_note_on: No free voice found!\n");
		return;
	} 
		else 
		{
			synth_program[prog]->assign_voice_with_preset_program_params(synth_voice[voice], voice);
			// OLD Code
			// Assign the program voice to the free voice
			//synth_voice[voice]->assign_dsp_voice(prog_voice->dsp_voice);
			// Assingn LUTs
			synth_voice[voice]->mso_wtab = synth_program[prog]->mso_wtab;			
			synth_voice[voice]->pad_wavetable = synth_program[prog]->program_wavetable;

			// Old Code
			//synth_voice[voice]->allocated_to_program_num = prog_voice->allocated_to_program_num;
			//synth_voice[voice]->allocated_to_program_voice_num = prog_voice->voice_num;

#ifdef _USE_NEW_POLY_MIXER_
			
#else			
			audio_polyphony_mixer->preserve_gain_pan(voice);
			// Get pointers to parameters so they can be modified
			audio_polyphony_mixer->set_voice_gain_1_ptr(voice, prog);
			audio_polyphony_mixer->set_voice_gain_2_ptr(voice, prog);
			audio_polyphony_mixer->set_voice_pan_1_ptr(voice, prog);
			audio_polyphony_mixer->set_voice_pan_2_ptr(voice, prog);
			audio_polyphony_mixer->set_voice_send_1_ptr(voice, prog);
			audio_polyphony_mixer->set_voice_send_2_ptr(voice, prog);
#endif

			fprintf(stderr,
					"midi_play_note_on: %i voice: %i program: %i allocated to prog %i\n",
					byte2,
					voice, // prog_voice->voice_num,
					prog,
					prog); // prog_voice->allocated_to_program_num);
		}
	} 
_voice_is_on:
	if ((voice > -1) && (synth_voice[voice] != NULL))
	{
		if (!reused) //
		{
			core = voice / num_of_core_voices;
			pthread_mutex_lock(&voice_busy_mutex);
			synth_polyphony_manager->increase_core_processing_load_weight(core, 
							AdjPolyphonyManager::voice_processing_weight);
			pthread_mutex_unlock(&voice_busy_mutex);
			mark_voice_busy_callback(voice);
			printf("Bussy %i %i %i %i\n",
				synth_polyphony_manager->get_core_processing_load_weight(0),
				synth_polyphony_manager->get_core_processing_load_weight(1),
				synth_polyphony_manager->get_core_processing_load_weight(2),
				synth_polyphony_manager->get_core_processing_load_weight(3));
		}
		
		synth_polyphony_manager->activate_resource(voice, (int)byte2, prog);
		//		kbd1->voices[voice].note = byte2;
		synth_voice[voice]->audio_voice->set_note(byte2);
		fprintf(stderr, "On voice %i\n", voice);

		//		if (sequencer1->mainTrack->recording)
		//		{
		//			sequencer1->addEvent((int)byte2, channel, (int)byte3, true, SynthSequencer::getInstance()->mainTrack);
		//		}
		
		kbd1->midi_play_note_on(channel, byte2, byte3);

		if (kbd1->get_split_point() == _KBD_SPLIT_POINT_NONE)
		{
			scaledMagnitude = kbd1->get_scaled_velocity(byte3);
		}
		else if ((int)byte2 >= 12 * (kbd1->get_split_point() + 3))
		{
			scaledMagnitude = kbd1->get_scaled_velocity(byte3);
		}
		else
		{
			scaledMagnitude = kbd1->get_low_scaled_velocity(byte3);
		}

		synth_voice[voice]->dsp_voice->set_voice_frequency(kbd1->get_note_frequency());

		synth_voice[voice]->dsp_voice->filter_1->set_kbd_freq(kbd1->get_note_frequency());
		synth_voice[voice]->dsp_voice->filter_2->set_kbd_freq(kbd1->get_note_frequency());

		synth_voice[voice]->audio_voice->set_magnitude((float)scaledMagnitude / 127);
		synth_voice[voice]->dsp_voice->karplus_1->note_on(byte2, (float)scaledMagnitude / 127);


		//		fprintf(stderr, "%i %f\n", voice, kbd1->getNoteFrequency());

		//		fprintf(stderr, "freq %f\n", kbd1->getNoteFrequency());

		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_1);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_2);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_3);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_4);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_5);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr_6);

		//		fprintf(stderr, "\nsynth on %i %i ", byte2, voice);
	}
	else
		fprintf(stderr, "note %i on not found  ", byte2);

	pthread_mutex_unlock(&voice_manage_mutex);
*/
}

/**
*   @brief  Play note off
*			Restore original voice params
*   @param	channel	MIDI channel 0-15
*	@param	byte2	note midi num
*	@param	byte3	note velocity (usually 0).
*	@param	voc		voice (NA).
*   @return void
*/
void  AdjSynth::midi_play_note_off(uint8_t channel, uint8_t byte2, uint8_t byte3, int voc)
{
	int voice = -1, program = 0;

	if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
	{
		program = channel;
	}
	else
	{
		program = active_sketch;
	}

	pthread_mutex_lock(&voice_manage_mutex);

	// Look for the voice that plays this note
#ifdef _USE_NEW_MIDI_PROGRAM
	voice = synth_program[program]->get_voice_num_playing_note(byte2);
#else
	voice = synth_polyphony_manager->get_reused_note(byte2, program);
#endif

	if (voice != -1)
	{		
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr_1);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr_2);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr_3);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr_4);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr_5);
		synth_voice[voice]->dsp_voice->karplus_1->note_off();
		synth_polyphony_manager->free_voice(voice, true); // go to pending untill env is zero
		fprintf(stderr, "midi_play_note_off  %i voice: %i prog: %i\n", byte2, voice, program);

#ifdef _USE_NEW_MIDI_PROGRAM
		voice = synth_program[program]->deallocate_voice_from_program(voice);
#else
		// ???
#endif
		//synthVoice[voice]->assignDspVoice(originalMainDspVoices[voice]);

	}
	else
	{
		fprintf(stderr, "midi_play_note_off %i not found program: %i\n ", byte2, program);
	}

	kbd1->midi_play_note_off(channel, byte2, byte3);

	pthread_mutex_unlock(&voice_manage_mutex);
}
