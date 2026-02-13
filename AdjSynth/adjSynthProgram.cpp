/**
*	@file		adjSynthProgram.cpp
*	@author		Nahum Budin
*	@date		23-Sep-2025
*	@version	1.3 
*					1. Code refactoring and notaion.
*					2. Rename Patch to Preset (Patch is to be used for full setup settings)
*					3. Intinitialize max num of voices in constructor
*					
*	History:\n	
*		
*		version	1.2		5-Oct-2024	Code refactoring and notaion.
*		version	1.1	4-Feb-2021	Code refactoring and notaion.
*		version	1.0	15-Nov-2019 
*
*	@brief		Handle AdjSynth MIDI programs
*				A MIDI program is a adjSyntg preset (voice).
*/

#include <sys/time.h>
#include <stdlib.h>

//#include "modSynth.h"
#include "../Settings/settings.h"
#include "adjSynth.h"
#include "adjSynthVoice.h"
#include "adjSynthProgram.h"
#include "synthKeyboard.h"
#include "../utils/utils.h"

extern pthread_mutex_t voice_busy_mutex;

int SynthProgram::prog_numbers = 0; // Static program number counter

/**
*   @brief  Condtructor - creates an instance of a SynthProgram object
*   @param	samp_rate		sample-rate
*   @param	block_size		audio block-size
*   @param  voices			number of voices
*   @param	first_v_index	1st voice index
*   @param wt_size			wavetable size (must be a power of 2
*   @param  aud_mng			pointer to an AudioManager object
*   @return none
*/
SynthProgram::SynthProgram(
	int samp_rate,
	int block_size,
	int voices,
	int first_v_index,
	int wt_size,
	AudioManager *aud_mng)
{
	int v_num, res, i;
	/* Temp name */
	char name[64] = "Preset1";
	uint32_t preset_version;

	// Allocate a sequntial ID - this is a static variable.
	prog_num = prog_numbers++;
	
	audio_manager = aud_mng;

	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);

	portamento_enabled = false;
	portamento_time = 0.0f;
	first_voice_index = first_v_index;

	char start_preset_name_x[128];
	//	sprintf(startPatchName_x, "%s%s", Synthesizer::getInstance()->synthPatchParams_x->getPatchPath(), "StartPatch");  // .xml is added inside call.
	
	// Verify that the wavetable size is a power of 2
	if ((wt_size & (wt_size - 1)) != 0)
	{
		// Not a power of 2
		printf("Error: wavetable length must be a power of 2");
		sleep(3);
		exit(1);
	}
	
	//if (set_patch_settings_default_params_callback_ptr)
	//{
	//	set_patch_settings_default_params_callback_ptr(&active_patch_params, prog_num);
	//}

	// Set the voice with the default preset parameters values
	AdjSynth::get_instance()->set_default_preset_parameters(&active_preset_params, prog_num);
	
	active_preset_params.name = "default_preset";
	active_preset_params.settings_type = _ADJ_SYNTH_PRESET_PARAMS;
	settings_manager = new Settings(&active_preset_params);
	active_preset_params.version = settings_manager->get_settings_version();

	program_wavetable = new Wavetable();
	program_wavetable->size = wt_size;
	program_wavetable->samples = (float*)malloc(wt_size * sizeof(float));
	
	synth_pad_creator = new SynthPADcreator(program_wavetable, program_wavetable->size);
	program_wavetable->base_freq =
		synth_pad_creator->set_base_frequency(program_wavetable, _PAD_DEFAULT_BASE_NOTE);

	mso_wtab = new DSP_MorphingSinusOscWTAB();
	mso_wtab->calc_segments_lengths(&mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->base_waveform_tab, &mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	mso_wtab->calc_segments_lengths(&mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->morphed_waveform_tab, &mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);	

	// synthVoice instances are dynamiclly created in set_num_of_voices(int nov) hereafter
	for (i = 0; i < _SYNTH_MAX_NUM_OF_VOICES; i++)
	{
		synth_voices[i] = NULL;
	}

	num_of_voices = _SYNTH_MAX_NUM_OF_VOICES;
	set_num_of_voices(voices); // This will also create the synthVoice objects
}

SynthProgram::~SynthProgram()
{
	delete[] program_wavetable->samples;
	delete program_wavetable;

	delete[] mso_wtab->base_waveform_tab;
	delete[] mso_wtab->morphed_waveform_tab;
}

/**
*	@brief	Register a callback function to set the program preset default params
*	@param	ptr  a pointer to a function void foo(_setting_params_t *params, int prog_num)					
*	@return void
*/
void SynthProgram::register_set_preset_settings_default_params_callback_ptr(func_ptr_int_settings_parms_ptr_int_t ptr)
{
	set_preset_settings_default_params_callback_ptr = ptr;
}

/**
*	@brief	Activates a callback function to set the program preset default params
*	@param	params		a pointer to a setting_params_t preset settings
*	@parm	prog		program number
*	@return set operation results
*/
int SynthProgram::activate_set_preset_settings_default_params_callback(_settings_params_t* params, int prog)
{
	if (set_preset_settings_default_params_callback_ptr)
	{
		return set_preset_settings_default_params_callback_ptr(params, prog);
	}
	
	return 0;
}

/**
*	@brief	Register a callback function to set voice busy indication (gui)
*	@param	ptr  a pointer to a function void foo(int voice_num)					
*	@return void
*/
void SynthProgram::register_mark_voice_bussy_callback_ptr(func_ptr_void_int_t ptr)
{
	mark_voice_bussy_callback_ptr = ptr;
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int SynthProgram::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int SynthProgram::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int SynthProgram::set_audio_block_size(int size)
{
	int res = 0;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;				
		res = audio_block_size;
	}
	else
	{
		res = -1;
	}
	
	return res;
}
	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int SynthProgram::get_audio_block_size() 
{ 
	return audio_block_size; 
}

/**
*   @brief  Set the program preset params
*   @param  vparams a pointer to a _setting_params_t struct holding the patch params
*   @return void
*/
void SynthProgram::set_program_preset_params(_settings_params_t *params)
{
	settings_res_t res;
	_settings_str_param_t str_param;
	
	// Go over all string parameters
	for (std::map<std::string,
		_settings_str_param_t>::iterator param = params->string_parameters_map.begin();
		param != params->string_parameters_map.end(); ++param)
	{
		settings_manager->set_string_param_value(&active_preset_params,
			param->first,
			param->second.value,
			_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
			prog_num);
	}

	res = settings_manager->get_string_param(params, "name", &str_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		active_preset_params.name = str_param.value;
	}

	// Go over all integer parameters
	for (std::map<std::string,
		_settings_int_param_t>::iterator param = params->int_parameters_map.begin();
		param != params->int_parameters_map.end(); ++param)
	{
		settings_manager->set_int_param_value(&active_preset_params,
			param->first,
			param->second.value,
			_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
			prog_num);
	}

	// Go over all float parameters
	for (std::map<std::string,
		_settings_float_param_t>::iterator param = params->float_parameters_map.begin();
		param != params->float_parameters_map.end(); ++param)
	{
		settings_manager->set_float_param_value(&active_preset_params,
			param->first,
			param->second.value,
			_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
			prog_num);
	}

	// Go over all boolean parameters
	for (std::map<std::string,
		_settings_bool_param_t>::iterator param = params->bool_parameters_map.begin();
		param != params->bool_parameters_map.end(); ++param)
	{
		settings_manager->set_bool_param_value(&active_preset_params,
			param->first,
			param->second.value,
			_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
			prog_num);
	}

	synth_pad_creator->generate_wavetable(program_wavetable);

	mso_wtab->calc_segments_lengths(&mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->morphed_waveform_tab, &mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);
}

/**
*   @brief  Return a free program voice resource.
*   @param  none
*   @return a pointer to a free program voice resource.
*/
SynthVoice *SynthProgram::get_free_voice()
{
	int voice = 0;
	struct timeval timestamp;
	uint64_t mintime = UINT64_MAX;
	int minvoice = -1, mincore = -1;
	bool reused = false;

	if (mod_synth_get_cpu_utilization() > 90)
	{	
		// DSP urilization is too high
		return NULL;
	}

	if (portamento_enabled)
	{
		synth_voices[_SYNTH_VOICE_1]->audio_voice->set_active();
		synth_voices[_SYNTH_VOICE_1]->audio_voice->reset_wait_for_not_active();
		if (mark_voice_bussy_callback_ptr)
		{
			mark_voice_bussy_callback_ptr(first_voice_index);
		}
		//		return synthVoices[firstVoiceIndex];
		return synth_voices[0];
	}

	// Look for a free voice
	voice = 0;
	while ((voice < num_of_voices) && synth_voices[voice]->dsp_voice->is_in_use())
	{
		voice++;
	}

	if (voice < num_of_voices)
	{
		synth_voices[voice]->dsp_voice->in_use();
		return synth_voices[voice];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Free in-use program voice resource.
*   @param  voice	voice resource number
*   @return void
*/
void SynthProgram::free_voice(int voice)
{
	if ((voice >= 0) && (voice < num_of_voices))
	{
		synth_voices[voice]->dsp_voice->not_in_use();
		printf("program: %i free voice: %i\n", prog_num, voice);
	}
		
}

/**
*   @brief  Set the number of voices.
*			Create additional new voices if required.
*   @param  nov	number of voice resources to create.
*   @return void
*/
void SynthProgram::set_num_of_voices(int nov)
{	
	int i;	

	if ((nov > 0) && (nov <= _SYNTH_MAX_NUM_OF_VOICES))
	{
		num_of_voices = nov;
		// Increase num of voices per demand.
		for (i = 0; i < num_of_voices; i++)
		{
			if (synth_voices[i] == NULL)
			{
				synth_voices[i] = new SynthVoice(
					i,
					prog_num,
					sample_rate,
					audio_block_size,
					&active_preset_params, 
					mso_wtab,
					program_wavetable,
					audio_manager);
				synth_voices[i]->set_voice_params(&active_preset_params);
			}
		}
	}	
}

/**
*   @brief  Return the number of voices.
*   @param  none
*   @return the number of voices
*/
int SynthProgram::get_num_of_voices() { return num_of_voices; }

/**
*   @brief  Set the portamento time in sec (float).
*   @param  porta	the portamento time in sec (0 to PORTAMENTO_MAX_TIME_SEC)
*   @return void
*/
void SynthProgram::set_portamento_time(float porta)
{
	portamento_time = porta;

	if (portamento_time < 0.0f)
	{
		portamento_time = 0.0f;
	}
	else if (portamento_time > _PORTAMENTO_MAX_TIME_SEC)
	{
		portamento_time = _PORTAMENTO_MAX_TIME_SEC;
	}

	porata_div = 1.0f - (float)((float)audio_block_size / (portamento_time + 0.004f) / (float)sample_rate);
}

/**
*   @brief  Set the portamento time in sec imn logaritmic scale (int).
*   @param  porta	the portamento time index (0 to 100)
*   @return void
*/
void SynthProgram::set_portamento_time(int porta)
{
	set_portamento_time(Utils::calc_log_scale_100_float(0.0f, _PORTAMENTO_MAX_TIME_SEC, 10.0, porta));
}

/**
*   @brief  Return the portamento time in sec (float).
*   @param  none
*   @return the portamento time in sec (float).
*/
float SynthProgram::get_portamento_level() { return portamento_time; }

/**
*   @brief  Enable Portamento.
*   @param  none
*   @return void
*/
void SynthProgram::enable_portamento() { portamento_enabled = true; }

/**
*   @brief  Disable Portamento.
*   @param  none
*   @return void
*/
void SynthProgram::disable_portamento() { portamento_enabled = false; }

/**
*   @brief  Return Portamento state.
*   @param  none
*   @return true if Portaqmento is enabled
*/
bool SynthProgram::portamento_is_enabled() { return portamento_enabled; }

/**
*   @brief  Return playde note frequency.
*			If Portamento is active, return the current note frequency
*   @param  none
*   @return playde note frequency.
*/
float SynthProgram::get_note_frequency()
{
	if (portamento_enabled)
	{	
		return actual_freq;
	}
	else
	{	
		return note_freq;
	}
}

/**
*   @brief  Update playde note frequency based on Portamento settings and Note-on ellapsed time.
*   @param  none
*   @return void
*/
void SynthProgram::update_actual_frequency()
{
	if (note_diff > 0)
	{
		// Glide up
		actual_freq /= porata_div;
		if (actual_freq > note_freq)
		{
			actual_freq = note_freq;
			note_diff = 0;
		}
	}
	else if (note_diff < 0)
	{
		// Glide down
		actual_freq *= porata_div;
		if (actual_freq < note_freq)
		{
			actual_freq = note_freq;
			note_diff = 0;
		}
	}
	else
	{
		// Same note - no glide
		actual_freq = note_freq;
	}
}
