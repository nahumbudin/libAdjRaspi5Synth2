/**
*	@file		dspVoice.cpp
*	@author		Nahum Budin
*	@date		14-Sep-2024
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Leaving only common parts (specific synth implemented as children objects)
*					
*	@History	25_Jan-2021 1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate and bloc-size settings
*					3. Adding wait for not active state (wait until voice energy goes low and 
*						then go to not active) and voice-end callback.
*				9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing parent object.
*				The object may include 2 independent signal paths: signal_path_1 and signa_path_2.
*				Only 1 path is intilized as a defualt. 
*				Creating an object with a parameter num_of_active_siganl_pathes=2 will enable both pathes.
*				
*				Each signal path comprises:
*				A signal source: source_1 in path_1 and sourc_2 in path_2.
*				Each signal source may support various modulation inputs, as applicable:
*					1. Frequency modulation
*					2. PWM/Symetry modulation
*					2. Amplitude modulation.
*					
*				Each object comprises 2 state-variable filter (LPF, HPF, BPF and ALL-Pass),
*				filter_1 and filter_2 regardless of the number of pathes.
*				Each active source may be send to both filters inputs.
*				Each filter supports a frequency modulation input.
*				
*				The output of each filter is directed into a distortion effect distortion_1
*				and distortion_2.*				
*				
*				The output of each distortion may be directed to 2 Amplifier stages, amp_1 and amp_2.
*				Each amp comprises a PAN modulation input.
*				
*
*				The object comprises various modulation sources that each can be routed to multiple
*				modulation inputs, as applicable:
*					1. 5 ADSR modulators
*					2. 5 LFO modulators.
*
*				Each amp output may be sent to the voice 2 outputs Left and Right.*
*/

#include "dspVoice.h"

/**
*   @brief  Initializes a DSP_Voice object instance.
*   @param  voice								polyphonic voice number 
*   @param	samp_rate							audio sample-rate
*   @param	bloc_size							audio block-size
*	@param	voice_end_event_callback_pointer	a pointer to a void foo(int voice_num) callback function
*												that if wait for not active state
*												detection is activated, will be 
*												called when voice output energy 
*												goes to zero.
*   @return none
*/
DSP_Voice::DSP_Voice(
	int voice,
	int samp_rate,
	int block_size,	
	int num_of_active_siganl_pathes,
	func_ptr_void_int_t voice_end_event_callback_pointer)
{
	voice_id = voice;
	voice_active = false;
	voice_waits_for_not_active = false;
	num_of_Active_paths = num_of_active_siganl_pathes; // Only 1 path as default
	used = false;
	voice_end_event_callback_ptr = voice_end_event_callback_pointer;
	
	source_1_active = false;
	
	source_1_frequency = 440.0f;
	act_freq_source_1 = 440.0f;
	signal_path_1_out = 0;
	
	source_1_send_filter_1_level = 0;
	source_1_send_filter_2_level = 0;
	
	source_1_freq_mod_lfo_modulator = _LFO_NONE;
	source_1_pwm_mod_lfo_modulator = _LFO_NONE;
	source_1_amp_mod_lfo_modulator = _LFO_NONE;
	source_1_freq_mod_lfo_level = 0;
	source_1_pwm_mod_lfo_level = 0;
	source_1_amp_mod_lfo_level = 0;
	
	source_1_freq_mod_env_modulator = _ENV_NONE;
	source_1_pwm_mod_env_modulator = _ENV_NONE;
	source_1_amp_mod_env_modulator = _ENV_NONE;
	source_1_freq_mod_env_level = 0;
	source_1_pwm_mod_env_level = 0;
	source_1_amp_mod_env_level = 0;
	
	source_1_amp_lfo_modulation_value = 1.0f;
	source_1_amp_env_modulation_value = 1.0f;
	source_1_pwm_lfo_modulation_value = 1.0f;
	source_1_pwm_env_modulation_value = 1.0f;
	source_1_freq_lfo_modulation_value = 0.0f;
	source_1_freq_env_modulation_value = 0.0f;
	
	source_1_freq_mod_value = 0.0f;
	source_1_pwm_mod_value = 0.0f;
	source_1_amp_mod_value = 0.0f;
	
	source_1_freq_mod_lfo_delay = 0;
	source_1_pwm_mod_lfo_delay = 0;
	source_1_amp_mod_lfo_delay = 0;
	
	if(num_of_Active_paths == 2)
	{
		source_2_active = false;
		
		source_2_frequency = 440.0f;
		act_freq_source_2 = 440.0f;
		signal_path_2_out = 0; 
		
		source_2_send_filter_1_level = 0;
		source_2_send_filter_2_level = 0;
		
		source_2_freq_mod_lfo_modulator = _LFO_NONE;
		source_2_pwm_mod_lfo_modulator = _LFO_NONE;
		source_2_amp_mod_lfo_modulator = _LFO_NONE;
		source_2_freq_mod_lfo_level = 0;
		source_2_pwm_mod_lfo_level = 0;
		source_2_amp_mod_lfo_level = 0;
		
		source_2_freq_mod_env_modulator = _ENV_NONE;
		source_2_pwm_mod_env_modulator = _ENV_NONE;
		source_2_amp_mod_env_modulator = _ENV_NONE;
		source_2_freq_mod_env_level = 0;
		source_2_pwm_mod_env_level = 0;
		source_2_amp_mod_env_level = 0;
		
		source_2_amp_lfo_modulation_value = 1.0f;
		source_2_amp_env_modulation_value = 1.0f;
		source_2_pwm_lfo_modulation_value = 1.0f;
		source_2_pwm_env_modulation_value = 1.0f;
		source_2_freq_lfo_modulation_value = 0.0f;
		source_2_freq_env_modulation_value = 0.0f;
		
		source_2_freq_mod_value = 0.0f;
		source_2_pwm_mod_value = 0.0f;
		source_2_amp_mod_value = 0.0f;
		
		source_2_freq_mod_lfo_delay = 0;
		source_2_pwm_mod_lfo_delay = 0;
		source_2_amp_mod_lfo_delay = 0;
	}
	
	filter_1_freq_mod_lfo_modulator = _LFO_NONE;
	filter_1_freq_mod_lfo_level = 0;
	filter_1_freq_mod_env_modulator = _ENV_NONE;
	filter_1_freq_mod_env_level = 0;
	
	filter_1_freq_lfo_modulation_value = 0.0f;
	filter_1_freq_mod_lfo_delay = 0;
	filter_1_freq_env_modulation_value = 0.0f;
	
	filter_1_freq_mod_lfo_delay = 0;
	filter_1_freq_mod_value = 0.0f;
	
	filter_2_freq_mod_lfo_modulator = _LFO_NONE;
	filter_2_freq_mod_lfo_level = 0;
	filter_2_freq_mod_env_modulator = _ENV_NONE;
	filter_2_freq_mod_env_level = 0;
	
	filter_2_freq_lfo_modulation_value = 0.0f;
	filter_2_freq_mod_lfo_delay = 0;
	filter_2_freq_env_modulation_value = 0.0f;
	
	filter_2_freq_mod_lfo_delay = 0;
	filter_2_freq_mod_value = 0.0f;
	
	distortion_1_active = false;
	distortion_2_active = false;
	
	amp_1_pan_mod_lfo_modulator = _LFO_NONE;
	amp_2_pan_mod_lfo_modulator = _LFO_NONE;
	amp_1_pan_mod_lfo_level = 0;
	amp_2_pan_mod_lfo_level = 0;
	amp_1_pan_mod_lfo_delay = 0;
	amp_2_pan_mod_lfo_delay = 0;
		
	lfo1_actual_freq = _CONTROL_SUB_SAMPLING;
	lfo2_actual_freq = _CONTROL_SUB_SAMPLING;
	lfo3_actual_freq = _CONTROL_SUB_SAMPLING;
	lfo4_actual_freq = _CONTROL_SUB_SAMPLING;
	lfo5_actual_freq = _CONTROL_SUB_SAMPLING;
			
	for (int i = 0; i < _NUM_OF_LFOS; i++)
	{
		lfo_output_value[i] = 0.0f;
	}
	
	for (int i = 0; i < _NUM_OF_ADSRS; i++)
	{
		adsr_output_value[i] = 0.0f;
	}
	
	filter1 = new DSP_Filter(voice + 1100, _DEFAULT_SAMPLE_RATE);
	filter1->set_frequency((int)((1000.0f - _FILTER_MIN_CENTER_FREQ) / (float)(filter1->get_filter_max_center_frequency() - _FILTER_MIN_CENTER_FREQ) * 100.0f));
	filter1->set_octave(20);
	filter1->set_resonance((int)((1.0f - _FILTER_MIN_Q) / (float)(_FILTER_MAX_Q - _FILTER_MIN_Q) * 100.0f));
	filter1->set_band(_FILTER_BAND_LPF);
	filter1->set_kbd_track(100);
	set_filter1_freq_mod_lfo(_LFO_NONE);
	set_filter1_freq_mod_lfo_level(0);
	set_filter1_freq_mod_env(_ENV_NONE);
	set_filter1_freq_mod_env_level(0);
	
	filter2 = new DSP_Filter(voice + 1200, _DEFAULT_SAMPLE_RATE);
	filter2->set_frequency((int)((1000.0f - _FILTER_MIN_CENTER_FREQ) / (float)(filter2->get_filter_max_center_frequency() - _FILTER_MIN_CENTER_FREQ) * 100.0f));
	filter2->set_octave(20);
	filter2->set_resonance((int)((1.0f - _FILTER_MIN_Q) / (float)(_FILTER_MAX_Q - _FILTER_MIN_Q) * 100.0f));
	filter2->set_band(_FILTER_BAND_LPF);
	filter2->set_kbd_track(100);
	set_filter2_freq_mod_lfo(_LFO_NONE);
	set_filter2_freq_mod_lfo_level(0);
	set_filter2_freq_mod_env(_ENV_NONE);
	set_filter2_freq_mod_env_level(0);

	distortion1 = new DSP_Distortion();
	distortion2 = new DSP_Distortion();

	out_amp = new DSP_Amp();
	out_amp->set_ch1_gain(20);
	out_amp->set_ch2_gain(20);
	out_amp->set_ch1_pan(50);
	out_amp->set_ch2_pan(50);
	set_amp1_pan_mod_lfo(_LFO_NONE);
	set_amp1_pan_mod_lfo_level(0);
	set_amp2_pan_mod_lfo(_LFO_NONE);
	set_amp2_pan_mod_lfo_level(0);
	
	amp_1_pan_mod_value = 0.0f;
	amp_2_pan_mod_value = 0.0f;

	lfo1 = new DSP_Osc(voice + 2000, // LFO1 id=2000....20xx
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
	set_lfo1_frequency((float)10);
	
	lfo2 = new DSP_Osc(voice + 2100, // LFO1 id=2100....21xx
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
	set_lfo2_frequency((float)10);

	lfo3 = new DSP_Osc(voice + 2200,
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
	set_lfo3_frequency((float)10);

	lfo4 = new DSP_Osc(voice + 2300,
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
	set_lfo4_frequency((float)10);

	lfo5 = new DSP_Osc(voice + 2400,
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
	set_lfo5_frequency((float)10);

	adsr1 = new DSP_ADSR(voice + 3000, (float)_CONTROL_SUB_SAMPLING / samp_rate); // ADSR1 id=3000....30xx
	adsr1->set_attack_time_sec(10);
	adsr1->set_decay_time_sec(10);
	adsr1->set_sustain_level(100);
	adsr1->set_release_time_sec(10);

	adsr2 = new DSP_ADSR(voice + 3100, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr2->set_attack_time_sec(10);
	adsr2->set_decay_time_sec(10);
	adsr2->set_sustain_level(100);
	adsr2->set_release_time_sec(10);

	adsr3 = new DSP_ADSR(voice + 3200, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr3->set_attack_time_sec(10);
	adsr3->set_decay_time_sec(10);
	adsr3->set_sustain_level(100);
	adsr3->set_release_time_sec(10);

	adsr4 = new DSP_ADSR(voice + 3300, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr4->set_attack_time_sec(10);
	adsr4->set_decay_time_sec(10);
	adsr4->set_sustain_level(100);
	adsr4->set_release_time_sec(10);

	adsr5 = new DSP_ADSR(voice + 3400, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr5->set_attack_time_sec(10);
	adsr5->set_decay_time_sec(10);
	adsr5->set_sustain_level(100);
	adsr5->set_release_time_sec(10);
	
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_Voice::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	/* A virtual function to be implemented for each individual instrument, as applicable. */
	set_synth_specific_sample_rate(samp_rate);

	if (filter1)
	{
		filter1->set_sample_rate(sample_rate);
	}

	if (filter2)
	{
		filter2->set_sample_rate(sample_rate);
	}

	if (lfo1)
	{
		lfo1->set_sample_rate(sample_rate);
	}

	if (lfo2)
	{
		lfo2->set_sample_rate(sample_rate);
	}

	if (lfo3)
	{
		lfo3->set_sample_rate(sample_rate);
	}

	if (lfo4)
	{
		lfo4->set_sample_rate(sample_rate);
	}

	if (lfo5)
	{
		lfo5->set_sample_rate(sample_rate);
	}

	if (adsr1)
	{
		adsr1->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr2)
	{
		adsr2->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr3)
	{
		adsr3->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr4)
	{
		adsr4->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr5)
	{
		adsr5->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_Voice::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*   @brief  sets the audio block size. 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int DSP_Voice::set_audio_block_size(int size)
{
	int res = -1;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;
		
		/* A virtual function to be implemented for each individual instrument. */
		set_synth_specific_audio_block_size(audio_block_size);

		/*
		if (lfo1)
		{
			lfo1->set_audio_block_size(audio_block_size);
		}

		if (lfo2)
		{
			lfo2->set_audio_block_size(audio_block_size);
		}

		if (lfo3)
		{
			lfo3->set_audio_block_size(audio_block_size);
		}

		if (lfo4)
		{
			lfo4->set_audio_block_size(audio_block_size);
		}

		if (lfo5)
		{
			lfo5->set_audio_block_size(audio_block_size);
		}
		*/

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
int DSP_Voice::get_audio_block_size() 
{	
	return audio_block_size; 
}

/**
*   @brief  sets the wait for not active state to active  
*   @param  none
*   @return void
*/	
void DSP_Voice::set_wait_for_not_active()
{
	voice_waits_for_not_active = true;
}
	
/**
*   @brief  sets the wait for not active state to not-active  
*   @param  none
*   @return void
*/	
void DSP_Voice::reset_wait_for_not_active()
{
	voice_waits_for_not_active = false;
}

/**
*   @brief  retruns wait for not active satate  
*   @param  none
*   @return buffer true if voice wait for not active state is true
*/	
bool DSP_Voice::get_wait_for_not_active_state() 
{ 
	return voice_waits_for_not_active; 
}

/**
*   @brief  Register a callback function to be called when the  wait for not active 
*			satate changes to not active with the voice number as a parameter. 
*   @param  func_ptr	a pointer to a void foo(int) function
*   @return buffer true if voice wait for not active state is true
*/	
void DSP_Voice::register_voice_end_event_callback(func_ptr_void_int_t func_ptr)
{
	voice_end_event_callback_ptr = func_ptr;
}

/**
*   @brief  Copy the state to a target DSP_Voice object.
*   @param  target	a target DSP_Voice object	
*   @return void
*/
void DSP_Voice::copy_my_state_to(DSP_Voice target)
{
	target.amp_1_pan_mod_value = this->amp_1_pan_mod_value;
	target.amp_2_pan_mod_value = this->amp_2_pan_mod_value;
	
	target.source_1_freq_mod_lfo_delay = this->source_1_freq_mod_lfo_delay;
	target.source_1_pwm_mod_lfo_delay = this->source_1_pwm_mod_lfo_delay;
	target.source_1_amp_mod_lfo_delay = this->source_1_amp_mod_lfo_delay;
	
	target.filter_1_freq_mod_lfo_delay = this->filter_1_freq_mod_lfo_delay;
	target.filter_2_freq_mod_lfo_delay = this->filter_2_freq_mod_lfo_delay;
	target.amp_1_pan_mod_lfo_delay = this->amp_1_pan_mod_lfo_delay;
	target.amp_2_pan_mod_lfo_delay = this->amp_2_pan_mod_lfo_delay;
	
	target.used = this->used;
	target.voice_id = this->voice_id;
	target.voice_active = this->voice_active;
	target.source_1_frequency = this->source_1_frequency;
	target.source_1_freq_mod_lfo_modulator = this->source_1_freq_mod_lfo_modulator;
	target.source_1_pwm_mod_lfo_modulator = this->source_1_pwm_mod_lfo_modulator;
	target.source_1_amp_mod_lfo_modulator = this->source_1_amp_mod_lfo_modulator;
	target.source_1_freq_mod_lfo_level = this->source_1_freq_mod_lfo_level;
	target.source_1_pwm_mod_lfo_level = this->source_1_amp_mod_lfo_level;
	target.source_1_freq_mod_env_modulator = this->source_1_freq_mod_env_modulator;
	target.source_1_pwm_mod_env_modulator = this->source_1_pwm_mod_env_modulator;
	target.source_1_freq_mod_env_level = this->source_1_freq_mod_env_level;
	target.source_1_pwm_mod_env_level = this->source_1_pwm_mod_env_level;
	target.source_1_amp_mod_env_level = this->source_1_amp_mod_env_level;
	target.source_1_send_filter_1_level = this->source_1_send_filter_1_level;
	target.source_1_send_filter_2_level = this->source_1_send_filter_2_level;
	target.source_1_amp_lfo_modulation_value = this->source_1_amp_lfo_modulation_value;
	target.source_1_amp_env_modulation_value = this->source_1_amp_env_modulation_value;
	target.source_1_pwm_lfo_modulation_value = this->source_1_pwm_lfo_modulation_value;
	target.source_1_pwm_env_modulation_value = this->source_1_pwm_env_modulation_value;
	target.source_1_freq_lfo_modulation_value = this->source_1_freq_lfo_modulation_value;
	target.source_1_freq_env_modulation_value = this->source_1_freq_env_modulation_value;
	target.source_1_active = this->source_1_active;
	target.act_freq_source_1 = this->act_freq_source_1;
	target.detune_source_1 = this->detune_source_1;
	
	target.filter_1_freq_mod_lfo_modulator = this->filter_1_freq_mod_lfo_modulator;
	target.filter_1_freq_mod_env_modulator = this->filter_1_freq_mod_env_modulator;
	target.filter_2_freq_mod_lfo_modulator = this->filter_2_freq_mod_lfo_modulator;
	target.filter_2_freq_mod_env_modulator = this->filter_2_freq_mod_env_modulator;
	
	target.filter_1_freq_mod_lfo_level = this->filter_1_freq_mod_lfo_level;
	target.filter_1_freq_mod_env_level = this->filter_1_freq_mod_env_level;
	target.filter_2_freq_mod_lfo_level = this->filter_2_freq_mod_lfo_level;
	target.filter_2_freq_mod_env_level = this->filter_2_freq_mod_env_level;
	
	target.filter_1_freq_lfo_modulation_value = this->filter_1_freq_lfo_modulation_value;
	target.filter_1_freq_env_modulation_value = this->filter_1_freq_env_modulation_value;
	target.filter_2_freq_lfo_modulation_value = this->filter_2_freq_lfo_modulation_value;
	target.filter_2_freq_env_modulation_value = this->filter_2_freq_env_modulation_value;
	
	target.filter_1_freq_mod_lfo_delay = this->filter_1_freq_mod_lfo_delay;
	target.filter_2_freq_mod_lfo_delay = this->filter_2_freq_mod_lfo_delay;
	
	target.filter_1_freq_mod_value = this->filter_1_freq_mod_value;
	target.filter_2_freq_mod_value = this->filter_2_freq_mod_value;
	
	target.source_1_freq_mod_value = this->source_1_freq_mod_value;
	target.source_1_pwm_mod_value = this->source_1_pwm_mod_value;
	
	target.amp_1_pan_mod_lfo_modulator = this->amp_1_pan_mod_lfo_modulator;
	target.amp_2_pan_mod_lfo_modulator = this->amp_2_pan_mod_lfo_modulator;
	target.amp_1_pan_mod_lfo_level = this->amp_1_pan_mod_lfo_level;
	target.amp_2_pan_mod_lfo_level = this->amp_2_pan_mod_lfo_level;
	
	target.lfo1_actual_freq = this->lfo1_actual_freq;
	target.lfo2_actual_freq = this->lfo2_actual_freq;
	target.lfo3_actual_freq = this->lfo3_actual_freq;
	target.lfo4_actual_freq = this->lfo4_actual_freq;
	target.lfo5_actual_freq = this->lfo5_actual_freq;
	
	target.distortion_1_active = this->distortion_1_active;
	target.distortion_2_active = this->distortion_2_active;
	
	if (num_of_Active_paths == 2)
	{
		target.source_2_frequency = this->source_2_frequency;
		target.source_2_freq_mod_lfo_delay = this->source_2_freq_mod_lfo_delay;
		target.source_2_pwm_mod_lfo_delay = this->source_2_pwm_mod_lfo_delay;
		target.source_2_amp_mod_lfo_delay = this->source_2_amp_mod_lfo_delay;
		target.source_2_freq_mod_lfo_modulator = this->source_2_freq_mod_lfo_modulator;
		target.source_2_pwm_mod_lfo_modulator = this->source_2_pwm_mod_lfo_modulator;
		target.source_2_amp_mod_lfo_modulator = this->source_2_amp_mod_lfo_modulator;
		target.source_2_freq_mod_lfo_level = this->source_2_freq_mod_lfo_level;
		target.source_2_pwm_mod_lfo_level = this->source_2_pwm_mod_lfo_level;
		target.source_2_amp_mod_lfo_level = this->source_2_amp_mod_lfo_level;
		target.source_2_freq_mod_env_modulator = this->source_2_freq_mod_env_modulator;
		target.source_2_pwm_mod_env_modulator = this->source_2_pwm_mod_env_modulator;
		target.source_2_amp_mod_env_modulator = this->source_2_amp_mod_env_modulator;
		target.source_2_freq_mod_env_level = this->source_2_freq_mod_env_level;
		target.source_2_pwm_mod_env_level = this->source_2_pwm_mod_env_level;
		target.source_2_amp_mod_env_level = this->source_2_amp_mod_env_level;
		target.source_2_send_filter_1_level = this->source_2_send_filter_1_level;
		target.source_2_send_filter_2_level = this->source_2_send_filter_2_level;
		target.source_2_amp_lfo_modulation_value = this->source_2_amp_lfo_modulation_value;
		target.source_2_amp_env_modulation_value = this->source_2_amp_env_modulation_value;
		target.source_2_pwm_lfo_modulation_value = this->source_2_pwm_lfo_modulation_value;
		target.source_2_pwm_env_modulation_value = this->source_2_pwm_env_modulation_value;
		target.source_2_freq_lfo_modulation_value = this->source_2_freq_env_modulation_value;
		
		target.source_2_freq_mod_value = this->source_2_freq_mod_value;
		target.source_2_pwm_mod_value = this->source_2_pwm_mod_value;
		
		target.source_2_active = this->source_2_active;
		target.act_freq_source_2 = this->act_freq_source_2;
		target.detune_source_2 = this->detune_source_2;
		
		target.synth2_sync_on_synth1 = this->synth2_sync_on_synth1;
	}
	
	/* A virtual function to be implemented for each individual instrument. */
	copy_my_specific_state_to(target);
}

/**
*   @brief  Set voice status to In-use.
*   @param  none
*   @return void
*/
void DSP_Voice::in_use() 
{ 
	used = true; 
}

/**
*   @brief  Set voice status to Not-in-use.
*   @param  none
*   @return void
*/
void DSP_Voice::not_in_use() 
{ 
	used = false; 
}

/**
*   @brief  Return voice In-use status.
*   @param  none
*   @return true if in-use; false otherwise
*/
bool DSP_Voice::is_in_use() 
{ 
	return used; 
}

/**
*	@brief	Enable Snthesizer 1
*	@param none
*	@return void
*/
void DSP_Voice::enable_synth1() 
{ 
	source_1_active = true; 
}

/**
*	@brief	Disable Snthesizer 1
*	@param none
*	@return void
*/
void DSP_Voice::disable_synth1() 
{ 
	source_1_active = false; 
}

/**
*	@brief	Enable Synthesizer 2
*	@param none
*	@return void
*/
void DSP_Voice::enable_synth2() 
{ 
	source_2_active = true; 
}

/**
*	@brief	Disable Synthesizer 2
*	@param none
*	@return void
*/
void DSP_Voice::disable_synth2() 
{ 
	source_2_active = false; 
}

/**
*	@brief	Enable Distortion
*	@param none
*	@return void
*/
void DSP_Voice::enable_distortion() 
{ 
	distortion_1_active = true; 
	distortion_2_active = true; 
}

/**
*	@brief	disable Distortion
*	@param none
*	@return void
*/
void DSP_Voice::disable_distortion() 
{ 
	distortion_1_active = false; 
	distortion_2_active = false; 
}

/**
*   @brief  Set voice source 1 frequency (Hz).
*   @param  freq	voice frequency (Hz) _OSC_MIN_FREQUENCY to _OSC_MAX_FREQUENCY
*   @return void
*/
void DSP_Voice::set_voice_signal_1_frequency(float frq)
{
	source_1_frequency = frq;
	if (source_1_frequency < _OSC_MIN_FREQUENCY)
	{
		source_1_frequency = _OSC_MIN_FREQUENCY;
	}
	else if (source_1_frequency > _OSC_MAX_FREQUENCY)
	{
		source_1_frequency = _OSC_MAX_FREQUENCY;
	}

	/* A virtual function to be implemented for each individual instrument. */
	set_source_1_specific_frequency(source_1_frequency);
}

/**
*   @brief  Return voice source 1 frequency (Hz).
*   @param  none
*   @return voice frequency (Hz)
*/
float DSP_Voice::get_voice_signal_1_frequency() 
{ 
	return source_1_frequency; 
}

/**
*   @brief  Set voice source 1 frequency (Hz).
*   @param  freq	voice frequency (Hz) _OSC_MIN_FREQUENCY to _OSC_MAX_FREQUENCY
*   @return void
*/
void DSP_Voice::set_voice_signal_2_frequency(float frq)
{
	if (num_of_Active_paths == 2)
	{
		source_2_frequency = frq;
		if (source_2_frequency < _OSC_MIN_FREQUENCY)
		{
			source_2_frequency = _OSC_MIN_FREQUENCY;
		}
		else if (source_2_frequency > _OSC_MAX_FREQUENCY)
		{
			source_2_frequency = _OSC_MAX_FREQUENCY;
		}

		/* A virtual function to be implemented for each individual instrument. */
		set_source_1_specific_frequency(source_2_frequency);
	}
}

/**
*   @brief  Return voice source 2 frequency (Hz).
*   @param  none
*   @return voice frequency (Hz)
*/
float DSP_Voice::get_voice_signal_2_frequency() 
{ 
	if (num_of_Active_paths == 2)
	{
		return source_2_frequency; 
	}
	else
	{
		return 0;
	}
}

/**
*	@brief	Set voice active
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_active() 
{ 
	voice_active = true; 
}

/**
*	@brief	Set voice inactive
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_inactive() 
{ 
	voice_active = false; 
}

/**
*	@brief	Return voice active state
*	@param	none
*	@return true if voice is active, false otherwise
*/
bool DSP_Voice::voice_is_active() 
{ 
	return voice_active; 
}

/**
*	@brief	Set voice wait for not active state ON
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_waits_for_not_active()
{
	voice_waits_for_not_active = true;
}

/**
*	@brief	Set voice wait for not active state OFF
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_not_waits_for_not_active()
{
	voice_waits_for_not_active = false;
}

bool DSP_Voice::is_voice_waits_for_not_active()
{
	return voice_waits_for_not_active;
}

/**
*	@brief	Calculate and return a frequency detune factor
*	@param	detune_oct	detune Octaves
*	@param	detune_semitones detune semi-tones
*	@param	detune_percents detune percents
*	@return none
*/
float DSP_Voice::detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents) {
		
	float frq_factor;
		
	frq_factor = (float)((pow(2.0, (float)detune_oct + 
				(float)detune_semitones / 12.0)))*(1.0f + detune_percents / 100.0f);
		
	return frq_factor;
}

/**
*	@brief	Calculate and return a frequency detune factor including frequency modulation
*	@param	detune_oct	detune Octaves
*	@param	detune_semitones detune semi-tones
*	@param	detune_percents detune percents
*	@param	modulation	modulation value
*	@return none
*/
float DSP_Voice::detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents, float modulation) {
		
	float frq_factor;
		
	frq_factor = (float)((pow(2.0, (float)detune_oct + modulation / 1.4f + 
				(float)detune_semitones / 12.0)))*(1.0f + detune_percents / 100.0f);
		
	return frq_factor;
}

/**
*	@brief	Calculate next modulation values.
*	@param	none
*	@return void
*/
void DSP_Voice::calc_next_modulation_values()
{
	lfo_output_value[0] = lfo1->get_next_output_val(lfo1_actual_freq);
	lfo_output_value[1] = lfo2->get_next_output_val(lfo2_actual_freq);
	lfo_output_value[2] = lfo3->get_next_output_val(lfo3_actual_freq);
	lfo_output_value[3] = lfo4->get_next_output_val(lfo4_actual_freq);
	lfo_output_value[4] = lfo5->get_next_output_val(lfo5_actual_freq);

	//	fprintf(stderr, "note on time : %i %i %i\n", adsr1->getNoteToneElapsedTime(),adsr2->getNoteToneElapsedTime(),adsr3->getNoteToneElapsedTime());

	adsr1->calc_next_envelope_val();
	adsr_output_value[0] = adsr1->get_output_val();

	adsr2->calc_next_envelope_val();
	adsr_output_value[1] = adsr2->get_output_val();

	adsr3->calc_next_envelope_val();
	adsr_output_value[2] = adsr3->get_output_val();

	adsr4->calc_next_envelope_val();
	adsr_output_value[3] = adsr4->get_output_val();

	adsr5->calc_next_envelope_val();
	adsr_output_value[4] = adsr5->get_output_val();

	if (source_1_freq_mod_lfo_modulator > _LFO_NONE)
	{
		/* A virtual function to be implemented for each individual instrument. */
		set_source_1_freq_lfo_modulation(source_1_freq_mod_lfo_level, 
										lfo_output_value[source_1_freq_mod_lfo_modulator - 1]);
	}
	else
	{
		source_1_freq_lfo_modulation_value = 0.f;
	}

	if (source_1_freq_mod_env_modulator > _ENV_NONE)
	{
		/* A virtual function to be implemented for each individual instrument. */
		set_source_1_freq_env_modulation(source_1_freq_mod_env_level, 
										adsr_output_value[source_1_freq_mod_env_modulator - 1]);
	}
	else
	{
		set_source_1_freq_env_modulation(source_1_freq_mod_env_level, 0.0f);
	}

	if (source_1_pwm_mod_lfo_modulator > _LFO_NONE)
	{
		/* A virtual function to be implemented for each individual instrument. */
		set_source_1_pwm_lfo_modulation(source_1_pwm_mod_lfo_level, 
										lfo_output_value[source_1_pwm_mod_lfo_modulator - 1]);
	}
	else
	{
		source_1_pwm_lfo_modulation_value = 0.f;
	}

	if (source_1_pwm_mod_env_modulator > _ENV_NONE)
	{
		/* A virtual function to be implemented for each individual instrument. */
		set_source_1_pwm_env_modulation(source_1_pwm_mod_env_level, 
										adsr_output_value[source_1_pwm_mod_env_modulator - 1]);
	}
	else
	{
		set_source_1_pwm_env_modulation(source_1_pwm_mod_env_level, 0.0f);
	}

	if (source_1_amp_mod_lfo_modulator > _LFO_NONE)
	{
		/* A virtual function to be implemented for each individual instrument. */
		set_source_1_amp_lfo_modulation(source_1_amp_mod_lfo_level, 
									lfo_output_value[source_1_amp_mod_lfo_modulator - 1]);
	}
	else
	{
		source_1_amp_lfo_modulation_value = 1.f;
	}

	if (source_1_amp_mod_env_modulator > _ENV_NONE)
	{
		/* A virtual function to be implemented for each individual instrument. */
		set_source_1_amp_env_modulation(source_1_amp_mod_env_level, 
										adsr_output_value[source_1_amp_mod_env_modulator - 1]);
	}
	else
	{
		set_source_1_amp_env_modulation(source_1_amp_mod_env_level, 0.0f);
	}
	
	if (num_of_Active_paths == 2)
	{
		if (source_2_freq_mod_lfo_modulator > _LFO_NONE)
		{
			/* A virtual function to be implemented for each individual instrument. */
			set_source_2_freq_lfo_modulation(source_2_freq_mod_lfo_level, 
				lfo_output_value[source_2_freq_mod_lfo_modulator - 1]);
		}
		else
		{
			source_2_freq_lfo_modulation_value = 0.f;
		}

		if (source_2_freq_mod_env_modulator > _ENV_NONE)
		{
			/* A virtual function to be implemented for each individual instrument. */
			set_source_2_freq_env_modulation(source_2_freq_mod_env_level, 
				adsr_output_value[source_2_freq_mod_env_modulator - 1]);
		}
		else
		{
			set_source_2_freq_env_modulation(source_2_freq_mod_env_level, 0.0f);
		}

		if (source_2_pwm_mod_lfo_modulator > _LFO_NONE)
		{
			/* A virtual function to be implemented for each individual instrument. */
			set_source_2_pwm_lfo_modulation(source_2_pwm_mod_lfo_level, 
				lfo_output_value[source_2_pwm_mod_lfo_modulator - 1]);
		}
		else
		{
			source_2_pwm_lfo_modulation_value = 0.f;
		}

		if (source_2_pwm_mod_env_modulator > _ENV_NONE)
		{
			/* A virtual function to be implemented for each individual instrument. */
			set_source_2_pwm_env_modulation(source_2_pwm_mod_env_level, 
				adsr_output_value[source_2_pwm_mod_env_modulator - 1]);
		}
		else
		{
			set_source_2_pwm_env_modulation(source_2_pwm_mod_env_level, 0.0f);
		}

		if (source_2_amp_mod_lfo_modulator > _LFO_NONE)
		{
			/* A virtual function to be implemented for each individual instrument. */
			set_source_2_amp_lfo_modulation(source_2_amp_mod_lfo_level, 
				lfo_output_value[source_1_amp_mod_lfo_modulator - 1]);
		}
		else
		{
			source_2_amp_lfo_modulation_value = 1.f;
		}

		if (source_2_amp_mod_env_modulator > _ENV_NONE)
		{
			/* A virtual function to be implemented for each individual instrument. */
			set_source_2_amp_env_modulation(source_2_amp_mod_env_level, 
				adsr_output_value[source_2_amp_mod_env_modulator - 1]);
		}
		else
		{
			set_source_2_amp_env_modulation(source_2_amp_mod_env_level, 0.0f);
		}
	}
	
	if (amp_1_pan_mod_lfo_modulator > _LFO_NONE)
	{
		set_amp_1_pan_lfo_modulation(amp_1_pan_mod_lfo_level, 
			lfo_output_value[amp_1_pan_mod_lfo_modulator - 1]);
	}
	else
	{
		set_amp_1_pan_lfo_modulation(0.0f, 0.0f);
	}

	if (amp_2_pan_mod_lfo_modulator > _LFO_NONE)
	{
		set_amp_2_pan_lfo_modulation(amp_2_pan_mod_lfo_level, 
									lfo_output_value[amp_2_pan_mod_lfo_modulator - 1]);
	}
	else
	{
		set_amp_2_pan_lfo_modulation(0.0f, 0.0f);
	}

	if (filter_1_freq_mod_lfo_modulator > _LFO_NONE)
	{
		set_filter1_freq_lfo_modulation(filter_1_freq_mod_lfo_level, 
										lfo_output_value[filter_1_freq_mod_lfo_modulator - 1]);
	}
	else
	{
		filter_1_freq_lfo_modulation_value = 0.f;
	}

	if (filter_1_freq_mod_env_modulator > _ENV_NONE)
	{
		set_filter1_freq_env_modulation(filter_1_freq_mod_env_level, 
										adsr_output_value[filter_1_freq_mod_env_modulator - 1]);
	}
	else
	{
		set_filter1_freq_env_modulation(filter_1_freq_mod_env_level, 0.0f);
	}

	if (filter_2_freq_mod_lfo_modulator > _LFO_NONE)
	{
		set_filter2_freq_lfo_modulation(filter_2_freq_mod_lfo_level, 
			lfo_output_value[filter_2_freq_mod_lfo_modulator - 1]);
	}
	else
	{
		filter_2_freq_lfo_modulation_value = 0.f;
	}

	if (filter_2_freq_mod_env_modulator > _ENV_NONE)
	{
		set_filter2_freq_env_modulation(filter_2_freq_mod_env_level, 
			adsr_output_value[filter_2_freq_mod_env_modulator - 1]);
	}
	else
	{
		set_filter2_freq_env_modulation(filter_2_freq_mod_env_level, 0.0f);
	}
}

/**
*	@brief	Update modulation factors
*	@param	voice	voice number to be updated
*	@return void
*/
void DSP_Voice::update_voice_modulation(int voice)
{
	// Frequency modulation
	source_1_freq_mod_value = 
		source_1_freq_lfo_modulation_value + source_1_freq_env_modulation_value;

	if (source_1_freq_mod_value < -1.0f)
	{
		source_1_freq_mod_value = -1.0f;
	}
	else if (source_1_freq_mod_value > 1.0f)
	{
		source_1_freq_mod_value = 1.0f;
	}
	
	if (num_of_Active_paths == 2)
	{
		source_2_freq_mod_value = 
			source_2_freq_lfo_modulation_value + source_2_freq_env_modulation_value;
	
		if (source_2_freq_mod_value < -1.0f)
		{
			source_2_freq_mod_value = -1.0f;
		}
		else if (source_2_freq_mod_value > 1.0f)
		{
			source_2_freq_mod_value = 1.0f;
		}
	}
	
	detune_source_1 = detune_frequency_factor(
		get_source_1_freq_detune_oct_set_value(),
		get_source_1_freq_detune_semitones_set_value(),
		get_source_1_freq_detune_cents_set_value(),
		source_1_freq_mod_value);
	
	act_freq_source_1  = source_1_frequency * detune_source_1;
	
	if (num_of_Active_paths == 2)
	{
		detune_source_2 = detune_frequency_factor(
		get_source_2_freq_detune_oct_set_value(),
			get_source_2_freq_detune_semitones_set_value(),
			get_source_2_freq_detune_cents_set_value(),
			source_2_freq_mod_value);
		
		act_freq_source_2  = source_2_frequency * detune_source_2;
	}
	
	
	// PWM
	if (source_1_pwm_lfo_modulation_value + source_1_pwm_env_modulation_value > 0.0)
	{
		source_1_pwm_mod_value = 
//			get_source_1_pwm_dcycle_set_val() / 100.0f + 
			source_1_pwm_lfo_modulation_value + 
			source_1_pwm_env_modulation_value;
		
		if (source_1_pwm_mod_value < -1.0f)
		{
			source_1_pwm_mod_value = -1.0f;
		}
		else if (source_1_pwm_mod_value > 1.0f)
		{
			source_1_pwm_mod_value = 1.0f;
		}
		
//		osc1->set_pwm_dcycle(50 + (int)(source_1_pwm_mod_value * 45.0f));
	}
/*	
	if (synth2_pwm_lfo_modulation + synth2_pwm_env_modulation > 0.0)
	{
		pwm_mod_synth2 = osc2->get_pwm_dcycle_set_val() / 100.0f + synth2_pwm_lfo_modulation + 
							synth2_pwm_env_modulation;
		if (pwm_mod_synth2 < -1.0f)
		{
			pwm_mod_synth2 = -1.0f;
		}
		else if (pwm_mod_synth2 > 1.0f)
		{
			pwm_mod_synth2 = 1.0f;
		}
			
		osc2->set_pwm_dcycle(50 + (int)(pwm_mod_synth2 * 45.0f));
	}
	*/
	
	// Amplitude
	//amp_1_modulation_value = synth1_amp_lfo_modulation * synth1_amp_env_modulation;
	//amp_modulation2 = synth2_amp_lfo_modulation * synth2_amp_env_modulation;
	
	// Look for active or activity
	if (
		//(!synth1_active || (synth1_amp_env_modulation < 0.05f) || ((synth1_send_filter1_level < 0.05f) && (synth1_send_filter2_level < 0.05f))) &&
		//(!synth2_active || (synth2_amp_env_modulation < 0.05f) || ((synth2_send_filter1_level < 0.05f) && (synth2_send_filter2_level < 0.05f))) &&
		voice_waits_for_not_active)
	{
		/* Voice shall be not-active */
		//AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_inactive();
		//AdjSynth::get_instance()->synth_voice[voice]->audio_voice->reset_wait_for_not_active();
		//AdjSynth::get_instance()->synth_voice[voice]->assign_dsp_voice(AdjSynth::get_instance()->get_original_main_dsp_voices(voice));
		//AdjSynth::get_instance()->audio_poly_mixer->restore_gain_pan(voice);

	}
	
	// Filter freq
	filter_1_freq_mod_value = 
		filter_1_freq_lfo_modulation_value + filter_1_freq_env_modulation_value;
	if (filter_1_freq_mod_value < -1.0f)
	{
		filter_1_freq_mod_value = -1.0f;
	}
	else if (filter_1_freq_mod_value > 1.0f)
	{
		filter_1_freq_mod_value = 1.0f;
	}
	
	filter_2_freq_mod_value = 
		filter_2_freq_lfo_modulation_value + filter_2_freq_env_modulation_value;
	if (filter_2_freq_mod_value < -1.0f)
	{
		filter_2_freq_mod_value = -1.0f;
	}
	else if (filter_2_freq_mod_value > 1.0f)
	{
		filter_2_freq_mod_value = 1.0f;
	}
}
