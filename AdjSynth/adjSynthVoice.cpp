/**
 *	@file		adjSynthVoice.cpp
 *	@author		Nahum Budin
 *	@date		17-Jan-2025
 *	@version	1.3
 *					1. Set filter frequency using the set_frequency_relative() method.
 *					2. Set filter resonance using the set_resonance_relative() method.
 *					3. Set filter octave using the set_octave_relative() method.
 *					4. Set filter keyboard frequency track using the set_kbd_freq_relative() method.
 *					5. Added set polyphonic mixer voice parameters.
 *					6. Added the option to set online a specific setting param of an active voice by set_voice_params().
 *					7. Added common settings parameters to set_voice_params().
 *
 *	@History	1.2 3-Oct-2024
 *					1. Code refactoring and notaion.
 *					2. Add LFO6 and ADSR6 to set_voice_params()
 *					3. Change ADR set_sec to set _sec_log in set_voice_params()
 *					4. Adding PAD wavetable parms to the set_voice_params()
 *				2-Feb--2021	1.1
 *					1. Code refactoring and notaion.
 *					2. Adding sample-rate and bloc-size settings
 *					3. Adding audio manager object ref.
 *				9-Nov-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 June 13, 2018
 *
 *	@brief		Synthesizer voice object
 */

#include "adjSynthVoice.h"
#include "../DSP/dspVoice.h"

/**
*	@brief	Creates a SynthVoice instance
*	@param	vnum	voice id number 
*	@param	prg		allocated to program number
*	@param	params	a pointer to a _setting_params_t structure (patch parameters) 
*	@param	msolut	a pointer to a DSP_MorphingSinusOscLUT object
*	@param	synthPADwavetable a pointer to a Wavetable object
*	@return none
*/
SynthVoice::SynthVoice(
	int vnum,
	int prg,
	int samp_rate,
	int block_size,
	_settings_params_t *params, // No need for this - the program will update the voice 
								// parameters when a voice is assigned to a program.
	DSP_MorphingSinusOscWTAB *msotab,
	Wavetable *synth_pad_wavetable, 
	AudioManager *aud_mng)
{
	out_gain_1 = out_gain_2 = 50;
	out_pan_1 = out_pan_2 = 50;
	out_send_1 = out_send_2 = 0;
	out_pan_lfo_1 = out_pan_lfo_2 = 0;
	out_pan_lfo_level_1 = out_pan_lfo_level_2 = 0;
	
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
	
	audio_manager = aud_mng;
	
	mso_wtab = msotab;
	pad_wavetable = synth_pad_wavetable;
	
	update_in_progress = false;

	voice_num = vnum;
	set_allocated_program(prg);
	allocated_to_program_voice_num = -1;
	// No need for this - the program will update the voice parameters when a voice is assigned to a program.
	//settings_manager->settings_params_deep_copy(&active_params, params);
		
	dsp_voice = new DSP_Voice(voice_num, sample_rate, audio_block_size, mso_wtab, pad_wavetable, NULL); //<<<<<< 
	audio_voice = new AudioVoiceFloat(
			_AUDIO_STAGE_2,			// process stage 2
		sample_rate,
		audio_block_size,
		voice_num, 
		2,						// number of outputs
		dsp_voice, 
		&audio_first_update[_AUDIO_STAGE_2]);

	audio_out = new AudioOutputFloat(
		_AUDIO_STAGE_7,
		audio_block_size,
		audio_manager->audio_block_stereo_float_shared_memory_voices_output[voice_num], // Output sample shared memory page 
																						// (not to be confused with audio output buffer -> Jack)
		&audio_first_update[_AUDIO_STAGE_7],
		voice_num,  // id
		false); // disable limiter for now (will be added later in the master output block)
	
	// Audio connections - Synth Voice -> Audio Output
	connection_voice_out_ch1 = audio_manager->connections_manager->get_audio_connection();
	connection_voice_out_ch2 = audio_manager->connections_manager->get_audio_connection();
	
	connection_voice_out_ch1->connect(audio_voice, _SYNTH_VOICE_OUT_1, audio_out, _AMP_AUDIO_IN_CH1);
	connection_voice_out_ch2->connect(audio_voice, _SYNTH_VOICE_OUT_2, audio_out, _AMP_AUDIO_IN_CH2);

	settings_manager = new Settings(&active_params);

	// No need for this - the program will update the voice parameters when a voice is assigned to a program.
	//set_voice_params(&active_params);
}

/**
*   @brief  Return a pointer to the SynthVoice object instance
*   @param  none
*   @return a pointer to the SynthVoice object instance
*/
SynthVoice *SynthVoice::get_instance() 
{ 
	return this; 
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int SynthVoice::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}

	if (audio_voice)
	{
		audio_voice->set_sample_rate(sample_rate);
	}

	if (mso_wtab)
	{
		mso_wtab->set_sample_rate(sample_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int SynthVoice::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int SynthVoice::set_audio_block_size(int size)
{
	int res = 0;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;

		if (audio_voice)
		{
			audio_voice->set_audio_block_size(audio_block_size);
		}

		if (audio_out)
		{
			audio_out->set_audio_block_size(audio_block_size);
		}

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
int SynthVoice::get_audio_block_size() 
{ 
	return audio_block_size; 
}

/**
*   @brief  Set voice allocated program number
*   @param  prg		program number
*   @return void
*/
void SynthVoice::set_allocated_program(int prg)
{
	if ((prg >= 0) && (prg <= _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		allocated_to_program_num = prg;
	}
}

/**
*   @brief  Return the voice allocated program number
*   @param  none
*   @return the voice allocated program number
*/
int SynthVoice::get_allocated_program() 
{ 
	return allocated_to_program_num;  
}

/**
 *   @brief  Set the voice signal 1 gain
 *   @param  gain  0-100
 *   @return 0
 */
int SynthVoice::set_gain_1(int gain)
{
	if ((gain >= 0) && (gain <= 100))
	{
		out_gain_1 = gain;
		dsp_voice->out_amp_1->set_ch1_gain(gain);
	}
	
	return 0;
}

/**
 *   @brief  Set the voice signal 2 gain
 *   @param  gain  0-100
 *   @return 0
 */
int SynthVoice::set_gain_2(int gain)
{
	if ((gain >= 0) && (gain <= 100))
	{
		out_gain_2 = gain;
		dsp_voice->out_amp_1->set_ch2_gain(gain);
	}

	return 0;
}

/**
 *   @brief  Set the voice signal 1 send
 *   @param  send  0-100
 *   @return 0
 */
int SynthVoice::set_send_1(int send)
{
	if ((send >= 0) && (send <= 100))
	{
		out_send_1 = send;
	}

	return 0;
}

/**
 *   @brief  Set the voice signal 2 send
 *   @param  send  0-100
 *   @return 0
 */
int SynthVoice::set_send_2(int send)
{
	if ((send >= 0) && (send <= 100))
	{
		out_send_2 = send;
	}

	return 0;
}

/**
 *   @brief  Set the voice pan 1 pan
 *   @param  pan  0-100 (50 ceneter)
 *   @return 0
 */
int SynthVoice::set_pan_1(int pan)
{
	if ((pan >= 0) && (pan <= 100))
	{
		out_pan_1 = pan;
		dsp_voice->out_amp_1->set_ch1_pan(pan);
	}

	return 0;
}

/**
 *   @brief  Set the voice pan 2 pan
 *   @param  pan  0-100 (50 ceneter)
 *   @return 0
 */
int SynthVoice::set_pan_2(int pan)
{
	if ((pan >= 0) && (pan <= 100))
	{
		out_pan_2 = pan;
		dsp_voice->out_amp_1->set_ch2_pan(pan);
	}

	return 0;
}

/**
 *   @brief  Set the voice signal 1 pan lfo num
 *   @param  lfo number  0 none 1-6
 *   @return 0
 */
int SynthVoice::set_pan_lfo_1(int lfo)
{
	if ((lfo >= 0) && (lfo <= _NUM_OF_LFOS))
	{
		out_pan_lfo_1 = lfo;
		dsp_voice->set_amp_1_ch_1_pan_mod_lfo(lfo);
	}

	return 0;
}

/**
 *   @brief  Set the voice signal 2 pan lfo num
 *   @param  lfo number  0 none 1-6
 *   @return 0
 */
int SynthVoice::set_pan_lfo_2(int lfo)
{
	if ((lfo >= 0) && (lfo <= _NUM_OF_LFOS))
	{
		out_pan_lfo_2 = lfo;
		dsp_voice->set_amp_1_ch_2_pan_mod_lfo(lfo);
	}

	return 0;
}

/**
 *   @brief  Set the voice signal 1 pan lfo level
 *   @param  lfo level  0-100
 *   @return 0
 */
int SynthVoice::set_pan_lfo_level_1(int lev)
{
	if ((lev >= 0) && (lev <= 100))
	{
		out_pan_lfo_level_1 = lev;
		dsp_voice->set_amp_1_ch_1_pan_mod_lfo_level(lev);
	}

	return 0;
}

/**
 *   @brief  Set the voice signal 1 pan lfo level
 *   @param  lfo level  0-100
 *   @return 0
 */
int SynthVoice::set_pan_lfo_level_2(int lev)
{
	if ((lev >= 0) && (lev <= 100))
	{
		out_pan_lfo_level_2 = lev;
		dsp_voice->set_amp_1_ch_2_pan_mod_lfo_level(lev);
	}

	return 0;
}

void SynthVoice::set_pad_wave_table(Wavetable *padwt)
{
	if (padwt)
	{
		pad_wavetable = padwt;
		dsp_voice->wavetable_1->wavetable = padwt;
	}
}

/**
*   @brief  Calculate next voice output audio block samples and transmit the blocks
*   @param  none
*   @return void
*/
void SynthVoice::update_all()
{
	AudioBlockFloat *p;
	uint8_t stage;
	int coreNum, ID;
	static uint32_t count = 0;

	if (!update_in_progress)
	{
		update_in_progress = true;
		struct timeval startts;
		struct timeval stopts;

		//if (update_enable) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//{
		pthread_mutex_lock(&update_mutex[voice_num]); // mutex defind at audioManager
		for (stage = 0; stage < _MAX_STAGE_NUM; stage++)
		{
			for (p = audio_first_update[stage]; p; p = p->audio_next_update)
			{
				p->update();
			}
		}

		update_in_progress = false;

		pthread_mutex_unlock(&update_mutex[voice_num]);
		//} if (update_enable)
	}
}

/**
*   @brief  Assign a DSP_Voice object instance
*   @param  dspv	a pointer to a DSP_Voice object instance
*   @return void
*/
void SynthVoice::assign_dsp_voice(DSP_Voice *dspv)
{
	if (dspv)
	{
		dsp_voice = dspv;
		audio_voice->dsp_voice = dspv;
	}
}

/**
*   @brief  Return a pointer to the active setting (patch) parametersce
*   @parm	none
*   @return a pointer to the active setting (patch) parametersce
*/
_settings_params_t *SynthVoice::get_voice_params() 
{ 
	return &active_params;  
}

/**
*   @brief  Set the paramters (patch) using the setting parameters
*   @parm	params	 a pointer to a _setting_params_t struct holding the parameters (patch)
*   @param  key_filter "all" to update all parameters, or specific key to update only one
*   @return 0 if successful, -1 if specific key not found
*/
int SynthVoice::set_voice_params(_settings_params_t *params, _settings_params_t *common_params, const char *key_filter)
{
	int i, j = 0, res;
	bool update_all = (strcmp(key_filter, "all") == 0);
	// Track if specific key was found
	int param_updated = -1; 
	
	_settings_bool_param_t bool_param;
	_settings_int_param_t int_param;
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.osc1.enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1_active = bool_param.value;
			param_updated = 0;
		}
		
		if (!update_all) return param_updated;
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.waveform") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_waveform(int_param.value);
			param_updated = 0;
		}
			
		if (!update_all) return param_updated;
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.tune_offset_oct") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.tune_offset_oct", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_freq_detune_oct(int_param.value);
			param_updated = 0;
		}
	}
	 
	if (update_all || strcmp(key_filter, "adjsynth.osc1.tune_offset_semitones") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.tune_offset_semitones", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_freq_detune_semitones(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.tune_offset_cents") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.tune_offset_cents", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_freq_detune_cents_set_value(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_pwm_dcycle_set_val(int_param.value);
			dsp_voice->osc_1->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.send_filter_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.send_filter_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_send_filter_1_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.send_filter_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.send_filter_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_send_filter_2_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_detune") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_detune", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmonies_detune(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_distortion") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_distortion", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmonies_distortion(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_mode") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_mode", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_unison_mode(int_param.value);
			param_updated = 0;
			// callback to GUI for updating unison mode and labales.
			//callback_set_osc_1_unison_mode(int_param.value);
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_square_wave") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.osc1.unison_square_wave", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			if (bool_param.value)
			{
				dsp_voice->osc_1->enable_unison_square();
			}
			else
			{
				dsp_voice->osc_1->disable_unison_square();
			}
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(0, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(1, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_3") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_3", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(2, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_4") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_4", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(3, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_5") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_5", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(4, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_6") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_6", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(5, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_7") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_7", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(6, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_8") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_8", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(7, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.unison_level_9") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_9", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_1->set_harmony_level(8, int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.osc1.freq_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_freq_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.freq_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_freq_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.freq_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_freq_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.freq_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_freq_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.pwm_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_pwm_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.pwm_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_pwm_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.pwm_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_pwm_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.pwm_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_pwm_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.amp_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_amp_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.amp_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_amp_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.amp_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_amp_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc1.amp_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_1_amp_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.osc2.enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_2_active = bool_param.value;
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.waveform") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_2->set_waveform(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.tune_offset_oct") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.tune_offset_oct", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_2->set_freq_detune_oct(int_param.value);
			param_updated = 0;
		}
	}
	 
	if (update_all || strcmp(key_filter, "adjsynth.osc2.tune_offset_semitones") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.tune_offset_semitones", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_2->set_freq_detune_semitones(int_param.value);
			param_updated = 0;
		}
	}
	 
	if (update_all || strcmp(key_filter, "adjsynth.osc2.tune_offset_cents") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.tune_offset_cents", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_2->set_freq_detune_cents_set_value(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->osc_2->set_pwm_dcycle_set_val(int_param.value);
			dsp_voice->osc_2->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.send_filter_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.send_filter_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_send_filter_1_level(int_param.value);
			param_updated = 0;
		}		
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.send_filter_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.send_filter_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_send_filter_2_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.sync_on_osc_1") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.osc2.sync_on_osc_1", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			if (bool_param.value)
			{
				dsp_voice->set_osc_2_sync_on_osc_1();
			}
			else
			{
				dsp_voice->set_osc_2_not_sync_on_osc_1();
			}
		}
		
		param_updated = 0;
	}
	
	//	dsp_voice->osc2->setFixToneState(params->osc2FixToneIsOn);	
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.freq_modulation_lfo_num") == 0)
	{		
		res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_freq_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.freq_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_freq_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.freq_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_freq_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.freq_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_freq_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.pwm_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_pwm_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.pwm_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_pwm_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.pwm_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_pwm_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.pwm_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_pwm_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.amp_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_amp_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.amp_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_amp_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.amp_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_amp_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.osc2.amp_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_osc_2_amp_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.noise.enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.noise.enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->noise_1_active = bool_param.value;
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.noise.type") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.noise.color", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->noise_1->set_noise_type(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.noise.send_filter_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.noise.send_filter_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_noise_1_send_filter_1_level(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.noise.send_filter_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.noise.send_filter_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_noise_1_send_filter_2_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.noise.amp_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_noise_1_amp_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.noise.amp_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_noise_1_amp_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.noise.amp_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_noise_1_amp_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.noise.amp_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_noise_1_amp_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.karplus_synth.enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1_active = bool_param.value;
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.excitation_waveform_type") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.excitation_waveform_type", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_excitation_waveform_type(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.excitation_waveform_variations") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.excitation_waveform_variations", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_excitation_waveform_variations(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.string_damping") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.string_damping", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_string_damping(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.string_damping_variations") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.string_damping_variations", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_string_damping_variation(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.string_damping_calculation_mode") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.string_damping_calculation_mode", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_string_dumping_calculation_mode(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.pluck_damping") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.pluck_damping", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_pluck_damping(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.pluck_damping_variations") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.pluck_damping_variations", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_pluck_damping_variation(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.on_decay") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.on_decay", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_on_decay(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.off_decay") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.off_decay", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->karplus_1->set_off_decay(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.send_filter_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.send_filter_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_karplus_1_send_filter_1_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.karplus_synth.send_filter_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.send_filter_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_karplus_1_send_filter_2_level(int_param.value);
			param_updated = 0;
		}
	}

	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.mso_synth.enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1_active = bool_param.value;
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.send_filter_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.send_filter_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_send_filter_1_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.send_filter_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.send_filter_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_send_filter_2_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.tune_offset_oct") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.tune_offset_oct", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->set_freq_detune_oct(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.tune_offset_semitones") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.tune_offset_semitones", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->set_freq_detune_semitones(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.tune_offset_cents") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.tune_offset_cents", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->set_freq_detune_cents(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.freq_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_freq_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.freq_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_freq_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.freq_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_freq_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.freq_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_freq_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.pwm_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_pwm_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.pwm_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_pwm_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.pwm_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_pwm_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.pwm_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_pwm_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.amp_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_amp_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.amp_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_amp_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.amp_modulation_env_num") == 0)
	{	
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_amp_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.amp_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_mso_1_amp_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.segment_position_a") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_a", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->wtab->set_segment_position(
				pos_a, int_param.value, &dsp_voice->mso_1->wtab->morphed_segment_positions);
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.segment_position_b") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_b", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->wtab->set_segment_position(
				pos_b,
				int_param.value,
				&dsp_voice->mso_1->wtab->morphed_segment_positions);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.segment_position_c") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_c", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->wtab->set_segment_position(
				pos_c,
				int_param.value,
				&dsp_voice->mso_1->wtab->morphed_segment_positions);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.segment_position_d") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_d", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->wtab->set_segment_position(
				pos_d,
				int_param.value,
				&dsp_voice->mso_1->wtab->morphed_segment_positions);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.segment_position_e") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_e", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->wtab->set_segment_position(
				pos_e,
				int_param.value,
				&dsp_voice->mso_1->wtab->morphed_segment_positions);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.mso_synth.segment_position_f") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_f", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->mso_1->wtab->set_segment_position(
				pos_f,
				int_param.value,
				&dsp_voice->mso_1->wtab->morphed_segment_positions);
			
			param_updated = 0;
		}
	}
	
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.pad_synth.enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->wavetable_1_active = bool_param.value;
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.send_filter_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.send_filter_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_send_filter_1_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.send_filter_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.send_filter_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_send_filter_2_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.tune_offset_octave") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.tune_offset_octave", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->wavetable_1->set_freq_detune_oct(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.tune_offset_semitones") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.tune_offset_semitones", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->wavetable_1->set_freq_detune_semitones(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.tune_offset_cents") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.tune_offset_cents", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->wavetable_1->set_freq_detune_cents(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.freq_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_freq_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.freq_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_freq_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.freq_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_freq_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.freq_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_freq_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.amp_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_amp_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.amp_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_amp_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.amp_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_amp_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.amp_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_pad_1_amp_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.quality") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.quality", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_wavetable_length(
				AdjSynth::get_instance()->synth_program[allocated_to_program_num]->program_wavetable,
				int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.base_note") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.base_note", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_base_note(
				AdjSynth::get_instance()->synth_program[allocated_to_program_num]->program_wavetable,
				int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.base_width") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.base_width", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_base_harmony_width(
				int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.shape") == 0)
	{	
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.shape", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_shape(
				int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.shape_cutoff") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.shape_cutoff", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_shape_cutoff(
				int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_0") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_0", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
					0, (float)int_param.value / 100.f);
				param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_1") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_1", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				1,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_2") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_2", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				2,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_3") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_3", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				3,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_4") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_4", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				4,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_5", &int_param);
	if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
	{
		AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
			5, (float)int_param.value / 100.f);
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_6") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_6", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				6,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_7") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_7", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				7,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_8") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_8", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				8,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_level_9") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_level_9", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmony_level(
				9,
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.pad_synth.harmonies_detune") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.harmonies_detune", &int_param);
		if ((res == _SETTINGS_KEY_FOUND) && (allocated_to_program_num >= 0))
		{
			AdjSynth::get_instance()->synth_program[allocated_to_program_num]->synth_pad_creator->set_harmonies_detune(
				(float)int_param.value / 100.f);
			param_updated = 0;
		}
	}

	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.frequency") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.filter1.frequency", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_1->set_frequency_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.octave") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.filter1.octave", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_1->set_octave_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.q") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.filter1.q", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_1->set_resonance_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.keyboard_track") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.filter1.keyboard_track", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_1->set_kbd_track_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.band") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.filter1.band", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_1->set_band(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.freq_modulation_lfo_num") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_1_freq_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.freq_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_1_freq_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.freq_modulation_env_num") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_1_freq_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter1.freq_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_1_freq_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.frequency") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.frequency", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_2->set_frequency_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.octave") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.octave", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_2->set_octave_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.q") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.q", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_2->set_resonance_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.keyboard_track") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.keyboard_track", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_2->set_kbd_track_relative(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.band") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.band", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->filter_2->set_band(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.freq_modulation_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_2_freq_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.freq_modulation_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_2_freq_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.freq_modulation_env_num") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_env_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_2_freq_mod_env(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.filter2.freq_modulation_env_level") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_env_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_filter_2_freq_mod_env_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.poly_mixer.level") == 0)
	{
		res = settings_manager->get_int_param(common_params, "adjsynth.poly_mixer.level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_level(voice_num, (float)int_param.value / 100.0);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.poly_mixer.pan") == 0)
	{
		res = settings_manager->get_int_param(common_params, "adjsynth.poly_mixer.pan", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_pan(voice_num, (float)(int_param.value - 50) / 50.0); // -1.0 to 1.0
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.poly_mixer.send") == 0)
	{
		res = settings_manager->get_int_param(common_params, "adjsynth.poly_mixer.send", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_send(voice_num, (float)int_param.value / 100.0); 
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.poly_mixer.pan_lfo_num") == 0)
	{
		res = settings_manager->get_int_param(common_params, "adjsynth.poly_mixer.pan_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_pan_lfo(voice_num, int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.poly_mixer.pan_lfo_level") == 0)
	{
		res = settings_manager->get_int_param(common_params, "adjsynth.poly_mixer.pan_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			AdjSynth::get_instance()->audio_polyphony_mixer->set_voice_pan_lfo_level(voice_num, (float)int_param.value / 100.0);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.distortion_1.blend") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.distortion_1.blend", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->distortion_1->set_blend((float)int_param.value / 100.f);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.distortion_1.drive") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.distortion_1.drive", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->distortion_1->set_drive((float)int_param.value / 100.f);
			param_updated = 0;
		}	
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.distortion_1.range") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.distortion_1.range", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->distortion_1->set_range((float)int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.distortion_2.blend") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.distortion_2.blend", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->distortion_2->set_blend((float)int_param.value / 100.f);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.distortion_2.drive") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.distortion_2.drive", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->distortion_2->set_drive((float)int_param.value / 100.f);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.distortion_2.range") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.distortion_2.range", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->distortion_2->set_range((float)int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.distortion.enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.distortion.enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->distortion_1_active = bool_param.value;
			dsp_voice->distortion_2_active = bool_param.value;
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.distortion.auto_gain_enabled") == 0)
	{
		res = settings_manager->get_bool_param(params, "adjsynth.distortion.auto_gain_enabled", &bool_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			if (bool_param.value)
			{
				dsp_voice->distortion_1->enable_auto_gain();
				dsp_voice->distortion_2->enable_auto_gain();
			}
			else
			{
				dsp_voice->distortion_1->disable_auto_gain();
				dsp_voice->distortion_2->disable_auto_gain();
			}
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.amp_ch1.level") == 0)
	{	
		// Changed to AudioMixer for programs support?
		if (dsp_voice->out_amp_1->levels_are_fixed() == false)
		{
			res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.level", &int_param);
			if (res == _SETTINGS_KEY_FOUND)
			{
				dsp_voice->out_amp_1->set_ch1_gain(int_param.value);
			}
			
			res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.pan", &int_param);
			if (res == _SETTINGS_KEY_FOUND)
			{
				dsp_voice->out_amp_1->set_ch1_pan(int_param.value);
			}
			
			res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.level", &int_param);
			if (res == _SETTINGS_KEY_FOUND)
			{
				dsp_voice->out_amp_1->set_ch2_gain(int_param.value);
			}
			
			res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.pan", &int_param);
			if (res == _SETTINGS_KEY_FOUND)
			{
				dsp_voice->out_amp_1->set_ch2_pan(int_param.value);
			}
		}
		param_updated = 0;
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.amp_ch1.pan_modulation_lfo_num") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.pan_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_amp_1_ch_1_pan_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.amp_ch1.pan_modulation_lfo_level") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.pan_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_amp_1_ch_1_pan_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.amp_ch2.pan_modulation_lfo_num") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.pan_modulation_lfo_num", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_amp_1_ch_2_pan_mod_lfo(int_param.value);	
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.amp_ch2.pan_modulation_lfo_level") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.pan_modulation_lfo_level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_amp_1_ch_2_pan_mod_lfo_level(int_param.value);
			param_updated = 0;
		}
	}
	
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_1.waveform") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.lfo_1.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_1->set_waveform(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_1.rate") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_1.rate", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_lfo_1_frequency(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_1.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_1.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_1->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_2.waveform") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_2.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_2->set_waveform(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_2.rate") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_2.rate", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_lfo_2_frequency(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_2.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_2.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_2->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_3.waveform") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_3.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_3->set_waveform(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_3.rate") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_3.rate", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_lfo_3_frequency(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_3.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_3.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_3->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_4.waveform") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_4.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_4->set_waveform(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_4.rate") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_4.rate", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_lfo_4_frequency(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_4.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_4.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_4->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_5.waveform") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_5.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_5->set_waveform(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_5.rate") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_5.rate", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_lfo_5_frequency(int_param.value);
			param_updated = 0;
		}
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.lfo_5.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_5.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_5->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.lfo_6.waveform") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_6.waveform", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_6->set_waveform(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.lfo_6.rate") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_6.rate", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_lfo_6_frequency(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.lfo_6.symmetry") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.lfo_6.symmetry", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->lfo_6->set_pwm_dcycle(int_param.value);
			param_updated = 0;
		}
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_1.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_1->set_attack_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_1.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_1->set_decay_time_sec_log(int_param.value);
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.env_1.sustain") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.env_1.sustain", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->adsr_1->set_sustain_level(int_param.value);
			param_updated = 0;
		}
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_1.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_1->set_release_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_2.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_2->set_attack_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_2.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_2->set_decay_time_sec_log(int_param.value);
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.env_2.sustain") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.env_2.sustain", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->adsr_2->set_sustain_level(int_param.value);
			param_updated = 0;
		}
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_2.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_2->set_release_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_3.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_3->set_attack_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_3.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_3->set_decay_time_sec_log(int_param.value);
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.env_3.sustain") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.env_3.sustain", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->adsr_3->set_sustain_level(int_param.value);
			param_updated = 0;
		}
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_3.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_3->set_release_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_4.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_4->set_attack_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_4.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_4->set_decay_time_sec_log(int_param.value);
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.env_4.sustain") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.env_4.sustain", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->adsr_4->set_sustain_level(int_param.value);
			param_updated = 0;
		}
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_4.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_4->set_release_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_5.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_5->set_attack_time_sec_log(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_5.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_5->set_decay_time_sec_log(int_param.value);
	}
	
	if (update_all || strcmp(key_filter, "adjsynth.env_5.sustain") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.env_5.sustain", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->adsr_5->set_sustain_level(int_param.value);
			param_updated = 0;
		}
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_5.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_5->set_release_time_sec_log(int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.env_6.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_6->set_attack_time_sec_log(int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.env_6.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_6->set_decay_time_sec_log(int_param.value);
	}

	if (update_all || strcmp(key_filter, "adjsynth.env_6.sustain") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.env_6.sustain", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->adsr_6->set_sustain_level(int_param.value);
			param_updated = 0;
		}
	}

	res = settings_manager->get_int_param(params, "adjsynth.env_6.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr_6->set_release_time_sec_log(int_param.value);
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.gain_1") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.gain_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_gain_1(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.gain_2") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.gain_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_gain_2(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.send_1") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.send_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_send_1(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.send_2") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.send_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_send_2(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.pan_1") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.pan_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_pan_1(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.pan_2") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.pan_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_pan_2(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.send_lfo_1") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.pan_lfo_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_amp_1_ch_1_pan_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.send_lfo_2") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.voice_out.pan_lfo_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->set_amp_1_ch_2_pan_mod_lfo(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.lfo_level_1") == 0)
	{
		res = settings_manager->get_int_param(params, "adjsynth.voice_out.pan_lfo_level_1", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_pan_lfo_level_1(int_param.value);
			param_updated = 0;
		}
	}

	if (update_all || strcmp(key_filter, "adjsynth.voice_out.pan_lfo_level_2") == 0)
	{
	res = settings_manager->get_int_param(params, "adjsynth.voice_out.pan_lfo_level_2", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_pan_lfo_level_2(int_param.value);
			param_updated = 0;
		}
	}
	
	return param_updated;
}
