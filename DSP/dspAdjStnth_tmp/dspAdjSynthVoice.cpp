/**
*	@file		dspAdjSynthVoice.h
*	@author		Nahum Budin
*	@date		21-Sep-2024
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
*	@brief		Synthesizer dsp processing object representing a single polyphonic voice.
*				Each object may include 2 independent signal paths: signal_path_1 and signa_path_2.
*				Only signal_path_1 is intilized as a defualt. 
*				Creating an object with a parameter num_of_active_siganl_pathes=2 will enable both pathes.
*				
*				signal_path_1 includes the following signal sources:
*					1. Oscilator (dspOsc)
*					2. Karplus Strong FFT synthesizer (dspKarplusStrong).
*					3. Morphed Sine synthesizer (dspMorphedSine).
*					4. Noise Generator (dspNoise).
*					
*				signal_path_2 includes the following signal sources:
*					1. Oscilator (dspOsc)
*					
*				Each signal source may be enabled or disabled independently.
*				
*				signal_path_2 Oscilator may be phased-locked on signal_path_1 Oscilator.
*				
*				The signal sources supports various modulation inputs:
*					1. Frequency modulation
*					2. PWM/Symetry modulation
*					2. Amplitude modulation.
*					
*				Each object comprises 2 state-variable filter (LPF, HPF, BPF and ALL-Pass),
*				filter_1 and filter_2, regardless of the number of pathes.
*				Each active source may be send to both filters inputs.
*				Each filter supports a frequency modulation input.
*					
*				The output of each filter is directed into a distortion effect processors,
*				distortion_1 and distortion_2.				
*				
*				The output of each distortion may be directed to 2 Amplifier stages, amp_1 and amp_2.
*				Each amp comprises a PAN modulation input.
*				
*				Each amp output may be sent to the voice 2 outputs Left and Right.*				
*
*				Each object comprises various modulation sources that each can be routed to multiple
*				modulation inputs, as applicable:
*					1. 5 ADSR modulators
*					2. 5 LFO modulators.
*
*				
*/

#include "dspAdjSynthVoice.h"

DSP_AdjSynthVoice::DSP_AdjSynthVoice(int voice,
									int samp_rate,
									int block_size,	
									Wavetable *synth_pad_wavetable,
									func_ptr_void_int_t voice_end_event_callback_pointer)
{
	voice_id = voice;
	voice_active = false;
	voice_waits_for_not_active = false;
	used = false;
	voice_end_event_callback_ptr = voice_end_event_callback_pointer;
	
	used = false;
	voice_active = false;
	voice_waits_for_not_active = false;
	
	osc_1_active = false;
	osc_2_active = false;
	noise_1_active = false;
	karplus_1_active = false;
	mso_1_active = false;
	pad_1_active = false;
	distortion_1_active = false;
	distortion_2_active = false;
	
	frequency = 440.0f;
	
	osc_1_out = 0.0f;
	osc_2_out = 0.0f;
	noise_1_out = 0.0f;
	karplus_1_out = 0.0f;
	mso_1_out = 0.0f;
	pad_1_out_1 = 0.0f;
	pad_1_out_2 = 0.0f;
	
	osc_1_amp_lfo_modulation_value = 1.0f;
	osc_1_amp_env_modulation_value = 1.0f;
	osc_1_pwm_lfo_modulation_value = 1.0f;
	osc_1_pwm_env_modulation_value = 1.0f;
	osc_1_freq_lfo_modulation_value = 0.0f;
	osc_1_freq_env_modulation_value = 0.0f;
	
	osc_2_amp_lfo_modulation_value = 1.0f;
	osc_2_amp_env_modulation_value = 1.0f;
	osc_2_pwm_lfo_modulation_value = 1.0f;
	osc_2_pwm_env_modulation_value = 1.0f;
	osc_2_freq_lfo_modulation_value = 0.0f;
	osc_2_freq_env_modulation_value = 0.0f;
	
	noise_1_amp_lfo_modulation_value = 1.0f;
	noise_1_amp_env_modulation_value = 1.0f;
	
	mso_1_amp_lfo_modulation_value = 1.0f;
	mso_1_amp_env_modulation_value = 1.0f;
	mso_1_pwm_lfo_modulation_value = 1.0f;
	mso_1_pwm_env_modulation_value = 1.0f;
	mso_1_freq_lfo_modulation_value = 0.0f;
	mso_1_freq_env_modulation_value = 0.0f;
	
	pad_1_amp_lfo_modulation_value = 1.0f;
	pad_1_amp_env_modulation_value = 1.0f;
	pad_1_pwm_lfo_modulation_value = 1.0f;
	pad_1_pwm_env_modulation_value = 1.0f;
	pad_1_freq_lfo_modulation_value = 0.0f;
	pad_1_freq_env_modulation_value = 0.0f;
	
	filter_1_freq_lfo_modulation_value = 0.0f;
	filter_1_freq_env_modulation_value = 0.0f;
	
	filter_2_freq_lfo_modulation_value = 0.0f;
	filter_2_freq_env_modulation_value = 0.0f;
	
	osc_1_freq_modulation_value = 0.0f;
	osc_2_freq_modulation_value = 0.0f;
	mso_1_freq_modulation_value = 0.0f;
	pad_1_freq_modulation_value = 0.0f;
	filter_1_freq_modulation_value = 0.0f;
	filter_2_freq_modulation_value = 0.0f;
	
	osc_1_freq_detune_value = 0.0f;
	osc_2_freq_detune_value = 0.0f;
	mso_1_freq_detune_value = 0.0f;
	pad_1_freq_detune_value = 0.0f;
	
	osc_1_actual_frequency = frequency;
	osc_2_actual_frequency = frequency;
	mso_1_actual_frequency = frequency;
	pad_1_actual_frequency = frequency;
	
	osc_1_amp_modulation_value = 1.0f;
	osc_2_amp_modulation_value = 1.0f;
	mso_1_amp_modulation_value = 1.0f;
	pad_1_amp_modulation_value = 1.0f;
	
	osc_1_pwm_modulation_value = 0.0f;
	osc_2_pwm_modulation_value = 0.0f;
	mso_1_pwm_modulation_value = 0.0f;
	
	init_lfo_delays();	
	
	pad_1_wavetable = synth_pad_wavetable;
	original_pad_1_wavetable = synth_pad_wavetable;
	
	for (int i = 0; i < _NUM_OF_LFOS; i++)
	{
		lfo_out_value[i] = 0;
	}
	
	for (int i = 0; i < _NUM_OF_ADSRS; i++)
	{
		adsr_out_value[i] = 0;
	}
	
	osc_1 = new DSP_Osc(voice + 100,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		15,
		0,
		false,
		false,
		false,
		0,
		0,
		_OSC_UNISON_MODE_12345678);
	
	set_osc_1_send_filter_1_level(0);
	set_osc_1_send_filter_2_level(0);
	set_osc_1_freq_mod_lfo(_LFO_NONE);
	set_osc_1_freq_mod_lfo_level(0);
	set_osc_1_freq_mod_env(_ENV_NONE);
	set_osc_1_freq_mod_env_level(0);
	set_osc_1_pwm_mod_lfo(_LFO_NONE);
	set_osc_1_pwm_mod_lfo_level(0);
	set_osc_1_pwm_mod_env(_ENV_NONE);
	set_osc_1_pwm_mod_env_level(0);
	set_osc_1_amp_mod_lfo(_LFO_NONE);
	set_osc_1_amp_mod_lfo_level(0);
	set_osc_1_amp_mod_env(_ENV_NONE);
	set_osc_1_amp_mod_env_level(0);
	
	set_osc_2_not_sync_on_osc_1();
	
	osc_2 = new DSP_Osc(voice + 200,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		15,
		0,
		false,
		false,
		false,
		0,
		0,
		_OSC_UNISON_MODE_12345678);
	
	set_osc_2_send_filter_1_level(0);
	set_osc_2_send_filter_2_level(0);
	set_osc_2_freq_mod_lfo(_LFO_NONE);
	set_osc_2_freq_mod_lfo_level(0);
	set_osc_2_freq_mod_env(_ENV_NONE);
	set_osc_2_freq_mod_env_level(0);
	set_osc_2_pwm_mod_lfo(_LFO_NONE);
	set_osc_2_pwm_mod_lfo_level(0);
	set_osc_2_pwm_mod_env(_ENV_NONE);
	set_osc_2_pwm_mod_env_level(0);
	set_osc_2_amp_mod_lfo(_LFO_NONE);
	set_osc_2_amp_mod_lfo_level(0);
	set_osc_2_amp_mod_env(_ENV_NONE);
	set_osc_2_amp_mod_env_level(0);
	
	noise_1 = new DSP_Noise(voice + 300);
	noise_1->set_amplitude(1.0f);
	noise_1->set_noise_type(_WHITE_NOISE);
	set_noise_1_send_filter_1_level(0);
	set_noise_1_send_filter_2_level(0);
	set_noise_1_amp_mod_lfo(_LFO_NONE);
	set_noise_1_amp_mod_lfo_level(0);
	set_noise_1_amp_mod_env(_ENV_NONE);
	set_noise_1_amp_mod_env_level(0);
	
	karplus_1 = new DSP_KarplusStrong(voice + 400);
	karplus_1->set_excitation_waveform_type(_KARPLUS_STRONG_EXCITATION_WHITE_NOISE);
	karplus_1->set_string_dumping_calculation_mode(_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT);
	karplus_1->set_on_decay(80);
	karplus_1->set_off_decay(30);
	karplus_1->set_string_damping(50);
	karplus_1->set_string_damping_variation(50);
	karplus_1->set_excitation_waveform_variations(50);
	set_karplus_1_send_filter_1_level(0);
	set_karplus_1_send_filter_2_level(0);
	
	mso_1 = new DSP_MorphingSinusOsc(
		mso_wtab_1,
		voice + 500,
		0,
		15,
		0);
	
	set_mso_1_send_filter_1_level(0);
	set_mso_1_send_filter_2_level(0);
	set_mso_1_freq_mod_lfo(_LFO_NONE);
	set_mso_1_freq_mod_lfo_level(0);
	set_mso_1_freq_mod_env(_ENV_NONE);
	set_mso_1_freq_mod_env_level(0);
	set_mso_1_pwm_mod_lfo(_LFO_NONE);
	set_mso_1_pwm_mod_lfo_level(0);
	set_mso_1_pwm_mod_env(_ENV_NONE);
	set_mso_1_pwm_mod_env_level(0);
	set_mso_1_amp_mod_lfo(_LFO_NONE);
	set_mso_1_amp_mod_lfo_level(0);
	set_mso_1_amp_mod_env(_ENV_NONE);
	set_mso_1_amp_mod_env_level(0);	
	
	pad_1 = new DSP_Wavetable(voice + 500, pad_1_wavetable);
	
	set_pad_1_freq_mod_lfo(_LFO_NONE);
	set_pad_1_freq_mod_lfo_level(0);
	set_pad_1_freq_mod_env(_ENV_NONE);
	set_pad_1_freq_mod_env_level(0);
	set_pad_1_amp_mod_lfo(_LFO_NONE);
	set_pad_1_amp_mod_lfo_level(0);
	set_pad_1_amp_mod_env(_ENV_NONE);
	set_pad_1_amp_mod_env_level(0);	
	
	filter_1 = new DSP_Filter(voice + 1100, _DEFAULT_SAMPLE_RATE);
	filter_1->set_frequency((int)((1000.0f - _FILTER_MIN_CENTER_FREQ) / 
							(float)(filter_1->get_filter_max_center_frequency() - 
							_FILTER_MIN_CENTER_FREQ) * 100.0f));
	filter_1->set_octave(20);
	filter_1->set_resonance((int)((1.0f - _FILTER_MIN_Q) / (float)(_FILTER_MAX_Q - _FILTER_MIN_Q) * 100.0f));
	filter_1->set_band(_FILTER_BAND_LPF);
	filter_1->set_kbd_track(100);
	set_filter_1_freq_mod_lfo(_LFO_NONE);
	set_filter_1_freq_mod_lfo_level(0);
	set_filter_1_freq_mod_env(_ENV_NONE);
	set_filter_1_freq_mod_env_level(0);
		
	distortion_1 = new DSP_Distortion();
	distortion_1->set_blend(0.f);
	distortion_1->set_drive(0.f);
	distortion_1->set_range(0.f);
	distortion_1->disable_auto_gain();
	
	distortion_2 = new DSP_Distortion();
	distortion_2->set_blend(0.f);
	distortion_2->set_drive(0.f);
	distortion_2->set_range(0.f);
	distortion_2->disable_auto_gain();
	
	out_amp_1 = new DSP_Amp();
	out_amp_1->set_ch1_gain(20);
	out_amp_1->set_ch2_gain(20);
	out_amp_1->set_ch1_pan(50);
	out_amp_1->set_ch2_pan(50);
	set_amp_1_ch_1_pan_mod_lfo(_LFO_NONE);
	set_amp_1_ch_1_pan_mod_lfo_level(0);
	set_amp_1_ch_2_pan_mod_lfo(_LFO_NONE);
	set_amp_1_ch_2_pan_mod_lfo_level(0);
	
	lfo_1 = new DSP_Osc(voice + 2000,
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
	set_lfo_1_frequency((float)10);
	
	lfo_2 = new DSP_Osc(voice + 2100,
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
	set_lfo_2_frequency((float)10);

	lfo_3 = new DSP_Osc(voice + 2200,
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
	set_lfo_3_frequency((float)10);

	lfo_4 = new DSP_Osc(voice + 2300,
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
	set_lfo_4_frequency((float)10);

	lfo_5 = new DSP_Osc(voice + 2400,
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
	set_lfo_5_frequency((float)10);
	
	adsr_1 = new DSP_ADSR(voice + 3000, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr_1->set_attack_time_sec(10);
	adsr_1->set_decay_time_sec(10);
	adsr_1->set_sustain_level(100);
	adsr_1->set_release_time_sec(10);

	adsr_2 = new DSP_ADSR(voice + 3100, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr_2->set_attack_time_sec(10);
	adsr_2->set_decay_time_sec(10);
	adsr_2->set_sustain_level(100);
	adsr_2->set_release_time_sec(10);

	adsr_3 = new DSP_ADSR(voice + 3200, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr_3->set_attack_time_sec(10);
	adsr_3->set_decay_time_sec(10);
	adsr_3->set_sustain_level(100);
	adsr_3->set_release_time_sec(10);

	adsr_4 = new DSP_ADSR(voice + 3300, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr_4->set_attack_time_sec(10);
	adsr_4->set_decay_time_sec(10);
	adsr_4->set_sustain_level(100);
	adsr_4->set_release_time_sec(10);

	adsr_5 = new DSP_ADSR(voice + 3400, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr_5->set_attack_time_sec(10);
	adsr_5->set_decay_time_sec(10);
	adsr_5->set_sustain_level(100);
	adsr_5->set_release_time_sec(10);
	
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
}

/**
*   @brief  Set voice status to In-use.
*   @param  none
*   @return void
*/
void DSP_AdjSynthVoice::set_in_use() 
{ 
	used = true; 
}

/**
*   @brief  Set voice status to Not-in-use.
*   @param  none
*   @return void
*/
void DSP_AdjSynthVoice::set_not_in_use() 
{ 
	used = false; 
}

/**
*   @brief  Return voice In-use status.
*   @param  none
*   @return true if in-use; false otherwise
*/
bool DSP_AdjSynthVoice::is_in_use() 
{ 
	return used; 
}

/**
*	@brief	Set voice active
*	@param	none
*	@return none
*/
void DSP_AdjSynthVoice::set_voice_active() 
{ 
	voice_active = true; 
}

/**
*	@brief	Set voice inactive
*	@param	none
*	@return none
*/
void DSP_AdjSynthVoice::set_voice_inactive() 
{ 
	voice_active = false; 
}

/**
*	@brief	Return voice active state
*	@param	none
*	@return true if voice is active, false otherwise
*/
bool DSP_AdjSynthVoice::voice_is_active() 
{ 
	return voice_active; 
}

/**
*	@brief	Called when no activity is detected
*	@param	none
*	@return true if voice is active, false otherwise
*/
void DSP_AdjSynthVoice::no_activity_detected(int voice_id)
{
	/*   TODO: add a callback void no_activity_detected(int voice_num)
	AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_inactive();
	AdjSynth::get_instance()->synth_voice[voice]->audio_voice->reset_wait_for_not_active();
	AdjSynth::get_instance()->synth_voice[voice]->assign_dsp_voice(AdjSynth::get_instance()->get_original_main_dsp_voices(voice));
	AdjSynth::get_instance()->synth_voice[voice]->mso_wtab = original_mso_wtab1;	
	AdjSynth::get_instance()->synth_voice[voice]->pad_wavetable = original_pad_wavetable;
	AdjSynth::get_instance()->audio_poly_mixer->restore_gain_pan(voice);
	*/
	
	
	if (voice_end_event_callback_ptr != NULL)
	{
		voice_end_event_callback_ptr(voice_id);
	}
}

/**
*	@brief	Set voice wait for not active state ON
*	@param	none
*	@return none
*/
void DSP_AdjSynthVoice::set_voice_waits_for_not_active()
{
	voice_waits_for_not_active = true;
}

/**
*	@brief	Set voice wait for not active state OFF
*	@param	none
*	@return none
*/
void DSP_AdjSynthVoice::set_voice_not_waiting_for_not_active()
{
	voice_waits_for_not_active = false;
}

bool DSP_AdjSynthVoice::is_voice_waits_for_not_active()
{
	return voice_waits_for_not_active;
}

/**
*   @brief  Register a callback function to be called when the  wait for not active 
*			satate changes to not active with the voice number as a parameter. 
*   @param  func_ptr	a pointer to a void foo(int) function
*   @return buffer true if voice wait for not active state is true
*/	
void DSP_AdjSynthVoice::register_voice_end_event_callback(func_ptr_void_int_t func_ptr)
{
	voice_end_event_callback_ptr = func_ptr;
}

/**
*   @brief  Set voice frequency (Hz).
*   @param  freq	voice frequency (Hz) _OSC_MIN_FREQUENCY to _OSC_MAX_FREQUENCY
*   @return void
*/
void DSP_AdjSynthVoice::set_voice_frequency(float frq)
{
	frequency = frq;
	if (frequency < _OSC_MIN_FREQUENCY)
	{
		frequency = _OSC_MIN_FREQUENCY;
	}
	else if (frequency > _OSC_MAX_FREQUENCY)
	{
		frequency = _OSC_MAX_FREQUENCY;
	}

	pad_1->set_output_frequency(frequency);
}

/**
*   @brief  Return voice frequency (Hz).
*   @param  none
*   @return voice frequency (Hz)
*/
float DSP_AdjSynthVoice::get_voice_frequency() 
{ 
	return frequency; 
}



/**
*	@brief	Enable Osc_1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_osc_1() 
{ 
	osc_1_active = true; 
}

/**
*	@brief	Enable Osc_2
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_osc_2() 
{ 
	osc_2_active = true; 
}

/**
*	@brief	Enable Noise1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_noise_1() 
{ 
	noise_1_active = true; 
}

/**
*	@brief	Enable KPS
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_karplus_1() 
{ 
	karplus_1_active = true; 
}

/**
*	@brief	Enable Mso_1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_morphed_sin_1() 
{ 
	mso_1_active = true; 
}

/**
*	@brief	Enable Pad_1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_pad_synth_1() 
{ 
	pad_1_active = true; 
}

/**
*	@brief	Enable Distortion
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::enable_distortion() 
{ 
	distortion_1_active = true; 
	distortion_2_active = true; 
}

/**
*	@brief	Disble Osc_1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_osc_1() 
{ 
	osc_1_active = false; 
}

/**
*	@brief	Disable Osc_2
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_osc_2() 
{ 
	osc_2_active = false; 
}

/**
*	@brief	Disable Noise_1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_noise_1() 
{ 
	noise_1_active = false; 
}

/**
*	@brief	Disable Kps1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_karplus_1() 
{ 
	karplus_1_active = false; 
}

/**
*	@brief	Disable Mso_1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_morphed_sin_1() 
{ 
	mso_1_active = false; 
}

/**
*	@brief	Disable Pad1
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_pad_synth_1() 
{ 
	pad_1_active = false; 
}

/**
*	@brief	Disable Distortion
*	@param none
*	@return void
*/
void DSP_AdjSynthVoice::disable_distortion() 
{ 
	distortion_1_active = false; 
	distortion_2_active = false; 
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_AdjSynthVoice::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}

	if (osc_1)
	{
		osc_1->set_sample_rate(sample_rate);
	}

	if (osc_2)
	{
		osc_2->set_sample_rate(sample_rate);
	}

	if (karplus_1)
	{
		karplus_1->set_sample_rate(sample_rate);
	}

	if (mso_1)
	{
		mso_1->get_wavetable()->set_sample_rate(sample_rate);
	}

	if (filter_1)
	{
		filter_1->set_sample_rate(sample_rate);
	}

	if (filter_2)
	{
		filter_2->set_sample_rate(sample_rate);
	}

	if (lfo_1)
	{
		lfo_1->set_sample_rate(sample_rate);
	}

	if (lfo_2)
	{
		lfo_2->set_sample_rate(sample_rate);
	}

	if (lfo_3)
	{
		lfo_3->set_sample_rate(sample_rate);
	}

	if (lfo_4)
	{
		lfo_4->set_sample_rate(sample_rate);
	}

	if (lfo_5)
	{
		lfo_5->set_sample_rate(sample_rate);
	}

	if (adsr_1)
	{
		adsr_1->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr_2)
	{
		adsr_2->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr_3)
	{
		adsr_3->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr_4)
	{
		adsr_4->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr_5)
	{
		adsr_5->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_AdjSynthVoice::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*   @brief  sets the audio block size and the table phase step based on the size  
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int DSP_AdjSynthVoice::set_audio_block_size(int size)
{
	int res = -1;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;

		/*
		if (osc_1)
		{
			osc_1->set_audio_block_size(audio_block_size);
		}

		if (osc_2)
		{
			osc_2->set_audio_block_size(audio_block_size);
		}

		if (lfo_1)
		{
			lfo_1->set_audio_block_size(audio_block_size);
		}

		if (lfo_2)
		{
			lfo_2->set_audio_block_size(audio_block_size);
		}

		if (lfo_3)
		{
			lfo_3->set_audio_block_size(audio_block_size);
		}

		if (lfo_4)
		{
			lfo_4->set_audio_block_size(audio_block_size);
		}

		if (lfo_5)
		{
			lfo_5->set_audio_block_size(audio_block_size);
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
int DSP_AdjSynthVoice::get_audio_block_size() 
{	
	return audio_block_size; 
}

/**
*   @brief  initialize lfo delay times (0, 500, 1000, 1500, 2000ms) sub sampling count   
*   @param  none
*   @return 0;
*/	
int DSP_AdjSynthVoice::init_lfo_delays()
{
	int lfo_num, state;
	uint32_t lfo_500_ms_delay_cont = sample_rate / _CONTROL_SUB_SAMPLING / 2;       
	
	lfo_delays[0] = 0;	
	
	for (state = 0; state < _NUM_OF_LFO_DELAY_OPTIONS; state++)
	{
		for (lfo_num = 0; lfo_num < _NUM_OF_LFOS; lfo_num++)
		{
			lfo_delays[state * _NUM_OF_LFO_DELAY_OPTIONS + state] = lfo_500_ms_delay_cont * state;
		}
	}
	
	return 0;
}

/**
*   @brief  Copy the of this voice to a target DSP_AdjSynthVoice object.
*   @param  target	a target DSP_Voice object	
*   @return void
*/
void DSP_AdjSynthVoice::copy_my_state_to(DSP_AdjSynthVoice target)
{
	target.osc_1_freq_mod_lfo_delay = this->osc_1_freq_mod_lfo_delay;
	target.osc_1_pwm_mod_lfo_delay = this->osc_1_pwm_mod_lfo_delay;
	target.osc_1_amp_mod_lfo_delay = this->osc_1_amp_mod_lfo_delay;
	target.osc_2_freq_mod_lfo_delay = this->osc_2_freq_mod_lfo_delay;
	target.osc_2_pwm_mod_lfo_delay = this->osc_2_pwm_mod_lfo_delay;
	target.osc_2_amp_mod_lfo_delay = this->osc_2_amp_mod_lfo_delay;
	target.noise_1_amp_mod_lfo_delay = this->noise_1_amp_mod_lfo_delay;
	target.mso_1_freq_mod_lfo_delay = this->mso_1_freq_mod_lfo_delay;
	target.mso_1_pwm_mod_lfo_delay = this->mso_1_pwm_mod_lfo_delay;
	target.mso_1_amp_mod_lfo_delay = this->mso_1_amp_mod_lfo_delay;
	target.mso_1_freq_mod_lfo_delay = this->mso_1_freq_mod_lfo_delay;
	target.mso_1_amp_mod_lfo_delay = this->mso_1_amp_mod_lfo_delay;
	target.filter_1_freq_mod_lfo_delay = this->filter_1_freq_mod_lfo_delay;
	target.filter_2_freq_mod_lfo_delay = this->filter_2_freq_mod_lfo_delay;
	target.amp_1_ch_1_pan_mod_lfo_delay = this->amp_1_ch_1_pan_mod_lfo_delay;
	target.amp_1_ch_2_pan_mod_lfo_delay = this->amp_1_ch_2_pan_mod_lfo_delay;
	target.used = this->used;
	target.voice_id = this->voice_id;
	target.voice_active = this->voice_active;
	target.frequency = this->frequency;
	target.osc_1_freq_mod_lfo = this->osc_1_freq_mod_lfo;
	target.osc_1_pwm_mod_lfo = this->osc_1_pwm_mod_lfo;
	target.osc_1_amp_mod_lfo = this->osc_1_amp_mod_lfo;
	target.osc_1_freq_mod_lfo_level = this->osc_1_freq_mod_lfo_level;
	target.osc_1_pwm_mod_lfo_level = this->osc_1_pwm_mod_lfo_level;
	target.osc_1_amp_mod_lfo_level = this->osc_1_amp_mod_lfo_level;
	target.osc_1_freq_mod_env = this->osc_1_freq_mod_env;
	target.osc_1_pwm_mod_env = this->osc_1_pwm_mod_env;
	target.osc_1_amp_mod_env = this->osc_1_amp_mod_env;
	target.osc_1_freq_mod_env_level = this->osc_1_freq_mod_env_level;
	target.osc_1_pwm_mod_env_level = this->osc_1_pwm_mod_env_level;
	target.osc_1_amp_mod_env_level = this->osc_1_amp_mod_env_level;
	target.osc_2_freq_mod_lfo = this->osc_2_freq_mod_lfo;
	target.osc_2_pwm_mod_lfo = this->osc_2_pwm_mod_lfo;
	target.osc_2_amp_mod_lfo = this->osc_2_amp_mod_lfo;
	target.osc_2_freq_mod_lfo_level = this->osc_2_freq_mod_lfo_level;
	target.osc_2_pwm_mod_lfo_level = this->osc_2_pwm_mod_lfo_level;
	target.osc_2_amp_mod_lfo_level = this->osc_2_amp_mod_lfo_level;
	target.osc_2_freq_mod_env = this->osc_2_freq_mod_env;
	target.osc_2_pwm_mod_env = this->osc_2_pwm_mod_env;
	target.osc_2_amp_mod_env = this->osc_2_amp_mod_env;
	target.osc_2_freq_mod_env_level = this->osc_2_freq_mod_env_level;
	target.osc_2_pwm_mod_env_level = this->osc_2_pwm_mod_env_level;
	target.osc_2_amp_mod_env_level = this->osc_2_amp_mod_env_level;
	target.mso_1_freq_mod_lfo = this->mso_1_freq_mod_lfo;
	target.mso_1_pwm_mod_lfo = this->mso_1_pwm_mod_lfo;
	target.mso_1_amp_mod_lfo = this->mso_1_amp_mod_lfo;
	target.mso_1_freq_mod_lfo_level = this->mso_1_freq_mod_lfo_level;
	target.mso_1_pwm_mod_lfo_level = this->mso_1_pwm_mod_lfo_level;
	target.mso_1_amp_mod_lfo_level = this->mso_1_amp_mod_lfo_level;
	target.mso_1_freq_mod_env = this->mso_1_freq_mod_env;
	target.mso_1_pwm_mod_env = this->mso_1_pwm_mod_env;
	target.mso_1_amp_mod_env = this->mso_1_amp_mod_env;
	target.mso_1_freq_mod_env_level = this->mso_1_freq_mod_env_level;
	target.mso_1_pwm_mod_env_level = this->mso_1_pwm_mod_env_level;
	target.mso_1_amp_mod_env_level = this->mso_1_amp_mod_env_level;
	target.mso_1_freq_mod_lfo = this->mso_1_freq_mod_lfo;
	target.mso_1_amp_mod_lfo = this->mso_1_amp_mod_lfo;
	target.mso_1_freq_mod_lfo_level = this->mso_1_freq_mod_lfo_level;
	target.mso_1_amp_mod_lfo_level = this->mso_1_amp_mod_lfo_level;
	target.mso_1_freq_mod_env = this->mso_1_freq_mod_env;
	target.mso_1_amp_mod_env = this->mso_1_amp_mod_env;
	target.mso_1_freq_mod_env_level = this->mso_1_freq_mod_env_level;
	target.mso_1_amp_mod_env_level = this->mso_1_amp_mod_env_level;
	target.noise_1_amp_mod_lfo = this->noise_1_amp_mod_lfo;
	target.noise_1_amp_mod_env = this->noise_1_amp_mod_env;
	target.noise_1_amp_mod_lfo_level = this->noise_1_amp_mod_lfo_level;
	target.noise_1_amp_mod_env_level = this->noise_1_amp_mod_env_level;
	target.filter_1_freq_mod_lfo = this->filter_1_freq_mod_lfo;
	target.filter_1_freq_mod_env = this->filter_1_freq_mod_env;
	target.filter_2_freq_mod_lfo = this->filter_2_freq_mod_lfo;
	target.filter_2_freq_mod_env = this->filter_2_freq_mod_env;
	target.filter_1_freq_mod_lfo_level = this->filter_1_freq_mod_lfo_level;
	target.filter_1_freq_mod_env_level = this->filter_1_freq_mod_env_level;
	target.filter_2_freq_mod_lfo_level = this->filter_2_freq_mod_lfo_level;
	target.filter_2_freq_mod_env_level = this->filter_2_freq_mod_env_level;
	target.amp_1_ch_1_pan_mod_lfo = this->amp_1_ch_1_pan_mod_lfo;
	target.amp_1_ch_1_pan_mod_lfo = this->amp_1_ch_2_pan_mod_lfo;
	target.amp_1_ch_2_pan_mod_lfo_level = this->amp_1_ch_1_pan_mod_lfo_level;
	target.amp_1_ch_2_pan_mod_lfo_level = this->amp_1_ch_2_pan_mod_lfo_level;
	target.lfo_1_actual_freq = this->lfo_1_actual_freq;
	target.lfo_2_actual_freq = this->lfo_2_actual_freq;
	target.lfo_3_actual_freq = this->lfo_3_actual_freq;
	target.lfo_4_actual_freq = this->lfo_4_actual_freq;
	target.lfo_5_actual_freq = this->lfo_5_actual_freq;
	target.osc_1_send_filter_1_level = this->osc_1_send_filter_1_level;
	target.osc_1_send_filter_2_level = this->osc_1_send_filter_2_level;
	target.osc_1_amp_lfo_modulation_value = this->osc_1_amp_lfo_modulation_value;
	target.osc_1_amp_env_modulation_value = this->osc_1_amp_env_modulation_value;
	target.osc_1_pwm_lfo_modulation_value = this->osc_1_pwm_lfo_modulation_value;
	target.osc_1_pwm_env_modulation_value = this->osc_1_pwm_env_modulation_value;
	target.osc_1_freq_lfo_modulation_value = this->osc_1_freq_lfo_modulation_value;
	target.osc_1_freq_env_modulation_value = this->osc_1_freq_env_modulation_value;
	target.osc_1_active = this->osc_1_active;
	target.osc_2_send_filter_1_level = this->osc_2_send_filter_1_level;
	target.osc_2_send_filter_2_level = this->osc_2_send_filter_2_level;
	target.osc_2_amp_lfo_modulation_value = this->osc_2_amp_lfo_modulation_value;
	target.osc_2_amp_env_modulation_value = this->osc_2_amp_env_modulation_value;
	target.osc_2_pwm_lfo_modulation_value = this->osc_2_pwm_lfo_modulation_value;
	target.osc_2_pwm_env_modulation_value = this->osc_2_pwm_env_modulation_value;
	target.osc_2_freq_lfo_modulation_value = this->osc_2_freq_lfo_modulation_value;
	target.osc_2_freq_env_modulation_value = this->osc_2_freq_env_modulation_value;
	target.osc_2_active = this->osc_2_active;
	target.osc_2_sync_on_osc_1 = this->osc_2_sync_on_osc_1;
	target.mso_1_send_filter_1_level = this->mso_1_send_filter_1_level;
	target.mso_1_send_filter_2_level = this->mso_1_send_filter_2_level;
	target.mso_1_amp_lfo_modulation_value = this->mso_1_amp_lfo_modulation_value;
	target.mso_1_amp_env_modulation_value = this->mso_1_amp_env_modulation_value;
	target.mso_1_pwm_lfo_modulation_value = this->mso_1_pwm_lfo_modulation_value;
	target.mso_1_pwm_env_modulation_value = this->mso_1_pwm_env_modulation_value;
	target.mso_1_freq_lfo_modulation_value = this->mso_1_freq_lfo_modulation_value;
	target.mso_1_freq_env_modulation_value = this->mso_1_freq_env_modulation_value;
	target.mso_1_active = this->mso_1_active;
	target.mso_1_send_filter_1_level = this->mso_1_send_filter_1_level;
	target.mso_1_send_filter_2_level = this->mso_1_send_filter_2_level;
	target.mso_1_amp_lfo_modulation_value = this->mso_1_amp_lfo_modulation_value;
	target.mso_1_amp_env_modulation_value = this->mso_1_amp_env_modulation_value;
	target.mso_1_freq_lfo_modulation_value = this->mso_1_freq_lfo_modulation_value;
	target.mso_1_freq_env_modulation_value = this->mso_1_freq_env_modulation_value;
	target.mso_1_active = this->mso_1_active;
	target.noise_1_send_filter_1_level = this->noise_1_send_filter_1_level;
	target.noise_1_send_filter_2_level = this->noise_1_send_filter_2_level;
	target.noise_1_amp_lfo_modulation_value = this->noise_1_amp_lfo_modulation_value;
	target.noise_1_amp_env_modulation_value = this->noise_1_amp_env_modulation_value;
	target.noise_1_active = this->noise_1_active;
	target.pad_1_send_filter_1_level = this->pad_1_send_filter_1_level;
	target.pad_1_send_filter_2_level = this->pad_1_send_filter_2_level;
	target.karplus_1_active = this->karplus_1_active;
	//	target.drawbars1active = this->drawbars1active;

	target.osc_1_freq_modulation_value = this->osc_1_freq_modulation_value;
	target.osc_2_freq_modulation_value = this->osc_2_freq_modulation_value;
	target.mso_1_freq_modulation_value = this->mso_1_freq_modulation_value;
	target.pad_1_freq_modulation_value = this->pad_1_freq_modulation_value;
	target.osc_1_freq_detune_value = this->osc_1_freq_detune_value;
	target.osc_2_freq_detune_value = this->osc_2_freq_detune_value;
	target.mso_1_freq_detune_value = this->mso_1_freq_detune_value;
	target.pad_1_freq_detune_value = this->pad_1_freq_detune_value;
	target.osc_1_actual_frequency = this->osc_1_actual_frequency;
	target.osc_2_actual_frequency = this->osc_2_actual_frequency;
	target.mso_1_actual_frequency = this->mso_1_actual_frequency;
	target.pad_1_actual_frequency = this->pad_1_actual_frequency;
	target.osc_1_pwm_modulation_value = this->osc_1_pwm_modulation_value;
	target.osc_2_pwm_modulation_value = this->osc_2_pwm_modulation_value;
	target.mso_1_pwm_modulation_value = this->mso_1_pwm_modulation_value;
	target.osc_1_amp_modulation_value = this->osc_1_amp_modulation_value;
	target.osc_2_amp_modulation_value = this->osc_2_amp_modulation_value;
	target.mso_1_amp_modulation_value = this->mso_1_amp_modulation_value;
	target.pad_1_amp_modulation_value = this->pad_1_amp_modulation_value;
	target.filter_1_freq_modulation_value = this->filter_1_freq_modulation_value;
	target.filter_2_freq_modulation_value = this->filter_2_freq_modulation_value;
	target.distortion_1_active = this->distortion_1_active;
	target.distortion_2_active = this->distortion_2_active;
}

/**
*	@brief	Calculate and return a frequency detune factor
*	@param	detune_oct	detune Octaves
*	@param	detune_semitones detune semi-tones
*	@param	detune_percents detune percents
*	@return none
*/
float DSP_AdjSynthVoice::detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents) 
{	
	float frq_factor;
		
	frq_factor = (float)((pow(2.0, (float)detune_oct + (float)detune_semitones / 12.0)))*
						(1.0f + detune_percents / 100.0f);
		
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
float DSP_AdjSynthVoice::detune_frequency_factor(int detune_oct, int detune_semitones, 
												float detune_percents, float modulation) {
		
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
void DSP_AdjSynthVoice::calc_next_modulation_values()
{
	lfo_out_value[0] = lfo_1->get_next_output_val(lfo_1_actual_freq);
	lfo_out_value[1] = lfo_2->get_next_output_val(lfo_2_actual_freq);
	lfo_out_value[2] = lfo_3->get_next_output_val(lfo_3_actual_freq);
	lfo_out_value[3] = lfo_4->get_next_output_val(lfo_4_actual_freq);
	lfo_out_value[4] = lfo_5->get_next_output_val(lfo_5_actual_freq);

	//	fprintf(stderr, "note on time : %i %i %i\n", adsr1->getNoteToneElapsedTime(),adsr2->getNoteToneElapsedTime(),adsr3->getNoteToneElapsedTime());

	adsr_1->calc_next_envelope_val();
	adsr_out_value[0] = adsr_1->get_output_val();

	adsr_2->calc_next_envelope_val();
	adsr_out_value[1] = adsr_2->get_output_val();

	adsr_3->calc_next_envelope_val();
	adsr_out_value[2] = adsr_3->get_output_val();

	adsr_4->calc_next_envelope_val();
	adsr_out_value[3] = adsr_4->get_output_val();

	adsr_5->calc_next_envelope_val();
	adsr_out_value[4] = adsr_5->get_output_val();

	if (osc_1_freq_mod_lfo > _LFO_NONE)
	{
		set_osc_1_freq_lfo_modulation(osc_1_freq_mod_lfo_level, lfo_out_value[osc_1_freq_mod_lfo - 1]);
	}
	else
	{
		osc_1_freq_lfo_modulation_value = 0.f;
	}

	if (osc_1_freq_mod_env > _ENV_NONE)
	{
		set_osc_1_freq_env_modulation(osc_1_freq_mod_env_level, adsr_out_value[osc_1_freq_mod_env - 1]);
	}
	else
	{
		set_osc_1_freq_env_modulation(osc_1_freq_mod_env_level, 0.0f);
	}

	if (osc_1_pwm_mod_lfo > _LFO_NONE)
	{
		set_osc_1_pwm_lfo_modulation(osc_1_pwm_mod_lfo_level, lfo_out_value[osc_1_pwm_mod_lfo - 1]);
	}
	else
	{
		osc_1_pwm_lfo_modulation_value = 0.f;
	}

	if (osc_1_pwm_mod_env > _ENV_NONE)
	{
		set_osc_1_pwm_env_modulation(osc_1_pwm_mod_env_level, adsr_out_value[osc_1_pwm_mod_env - 1]);
	}
	else
	{
		set_osc_1_pwm_env_modulation(osc_1_pwm_mod_env_level, 0.0f);
	}

	if (osc_1_amp_mod_lfo > _LFO_NONE)
	{
		set_osc_1_amp_lfo_modulation(osc_1_amp_mod_lfo_level, lfo_out_value[osc_1_amp_mod_lfo - 1]);
	}
	else
	{
		osc_1_amp_lfo_modulation_value = 1.f;
	}

	if (osc_1_amp_mod_env > _ENV_NONE)
	{
		set_osc_1_amp_env_modulation(osc_1_amp_mod_env_level, adsr_out_value[osc_1_amp_mod_env - 1]);
	}
	else
	{
		set_osc_1_amp_env_modulation(osc_1_amp_mod_env_level, 0.0f);
	}

	if (amp_1_ch_1_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_1_ch_1_pan_lfo_modulation(amp_1_ch_1_pan_mod_lfo_level, 
										  lfo_out_value[amp_1_ch_1_pan_mod_lfo - 1]);
	}
	else
	{
		set_amp_1_ch_1_pan_lfo_modulation(0.0f, 0.0f);
	}
	

	if (osc_2_freq_mod_lfo > _LFO_NONE)
	{
		set_osc_2_freq_lfo_modulation(osc_2_freq_mod_lfo_level, lfo_out_value[osc_2_freq_mod_lfo - 1]);
	}
	else
	{
		osc_2_freq_lfo_modulation_value = 0.f;
	}

	if (osc_2_freq_mod_env > _ENV_NONE)
	{
		set_osc_2_freq_env_modulation(osc_2_freq_mod_env_level, adsr_out_value[osc_2_freq_mod_env - 1]);
	}
	else
	{
		set_osc_2_freq_env_modulation(osc_2_freq_mod_env_level, 0.0f);
	}

	if (osc_2_pwm_mod_lfo > _LFO_NONE)
	{
		set_osc_2_pwm_lfo_modulation(osc_2_pwm_mod_lfo_level, lfo_out_value[osc_2_pwm_mod_lfo - 1]);
	}
	else
	{
		osc_2_pwm_lfo_modulation_value = 0.f;
	}

	if (osc_2_pwm_mod_env > _ENV_NONE)
	{
		set_osc_2_pwm_env_modulation(osc_2_pwm_mod_env_level, adsr_out_value[osc_2_pwm_mod_env - 1]);
	}
	else
	{
		set_osc_2_pwm_env_modulation(osc_2_pwm_mod_env_level, 0.0f);
	}

	if (osc_2_amp_mod_lfo > _LFO_NONE)
	{
		set_osc_2_amp_lfo_modulation(osc_2_amp_mod_lfo_level, lfo_out_value[osc_2_amp_mod_lfo - 1]);
	}
	else
	{
		osc_2_amp_lfo_modulation_value = 1.f;
	}

	if (osc_2_amp_mod_env > _ENV_NONE)
	{
		set_osc_2_amp_env_modulation(osc_2_amp_mod_env_level, adsr_out_value[osc_2_amp_mod_env - 1]);
	}
	else
	{
		set_osc_2_amp_env_modulation(osc_2_amp_mod_env, 0.0f);
	}

	if (amp_1_ch_2_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_1_ch_2_pan_lfo_modulation(amp_1_ch_2_pan_mod_lfo_level, 
										  lfo_out_value[amp_1_ch_2_pan_mod_lfo - 1]);
	}
	else
	{
		set_amp_1_ch_2_pan_lfo_modulation(0.0f, 0.0f);
	}

	
	if (noise_1_amp_mod_lfo > _LFO_NONE)
	{
		set_noise_1_amp_lfo_modulation(noise_1_amp_mod_lfo_level, lfo_out_value[noise_1_amp_mod_lfo - 1]);
	}
	else
	{
		noise_1_amp_lfo_modulation_value = 1.f;
	}

	if (noise_1_amp_mod_env > _ENV_NONE)
	{
		set_noise_1_amp_env_modulation(noise_1_amp_mod_env_level, adsr_out_value[noise_1_amp_mod_env - 1]);
	}
	else
	{
		set_noise_1_amp_env_modulation(noise_1_amp_mod_env_level, 0.0f);
	}

	if (mso_1_freq_mod_lfo > _LFO_NONE)
	{
		set_mso_1_freq_lfo_modulation(mso_1_freq_mod_lfo_level, lfo_out_value[mso_1_freq_mod_lfo - 1]);
	}
	else
	{
		mso_1_freq_lfo_modulation_value = 0.f;
	}

	if (mso_1_freq_mod_env > _ENV_NONE)
	{
		set_mso_1_freq_env_modulation(mso_1_freq_mod_env_level, adsr_out_value[mso_1_freq_mod_env - 1]);
	}
	else
	{
		set_mso_1_freq_env_modulation(mso_1_freq_mod_env_level, 0.0f);
	}

	if (mso_1_pwm_mod_lfo > _LFO_NONE)
	{
		set_mso_1_pwm_lfo_modulation(mso_1_pwm_mod_lfo_level, lfo_out_value[mso_1_pwm_mod_lfo - 1]);
	}
	else
	{
		mso_1_pwm_lfo_modulation_value = 0.f;
	}

	if (mso_1_pwm_mod_env > _ENV_NONE)
	{
		set_mso_1_pwm_env_modulation(mso_1_pwm_mod_env_level, adsr_out_value[mso_1_pwm_mod_env - 1]);
	}
	else
	{
		set_mso_1_pwm_env_modulation(mso_1_pwm_mod_env_level, 0.0f);
	}

	if (mso_1_amp_mod_lfo > _LFO_NONE)
	{
		set_mso_1_amp_lfo_modulation(mso_1_amp_mod_lfo_level, lfo_out_value[mso_1_amp_mod_lfo - 1]);
	}
	else
	{
		mso_1_amp_lfo_modulation_value = 1.f;
	}

	if (mso_1_amp_mod_env > _ENV_NONE)
	{
		set_mso_1_amp_env_modulation(mso_1_amp_mod_env_level, adsr_out_value[mso_1_amp_mod_env - 1]);
	}
	else
	{
		set_mso_1_amp_env_modulation(mso_1_amp_mod_env_level, 0.0f);
	}

	if (pad_1_freq_mod_lfo > _LFO_NONE)
	{
		set_pad_1_freq_lfo_modulation(pad_1_freq_mod_lfo_level, lfo_out_value[pad_1_freq_mod_lfo - 1]);
	}
	else
	{
		pad_1_freq_lfo_modulation_value = 0.f;
	}

	if (pad_1_freq_mod_env > _ENV_NONE)
	{
		set_pad_1_freq_env_modulation(pad_1_freq_mod_env_level, adsr_out_value[pad_1_freq_mod_env - 1]);
	}
	else
	{
		set_pad_1_freq_env_modulation(pad_1_freq_mod_env_level, 0.0f);
	}

	if (pad_1_amp_mod_lfo > _LFO_NONE)
	{
		set_pad_1_amp_lfo_modulation(pad_1_amp_mod_lfo_level, lfo_out_value[pad_1_amp_mod_lfo - 1]);
	}
	else
	{
		pad_1_amp_lfo_modulation_value = 1.f;
	}

	if (pad_1_amp_mod_env > _ENV_NONE)
	{
		set_pad_1_amp_env_modulation(pad_1_amp_mod_env_level, adsr_out_value[pad_1_amp_mod_env - 1]);
	}
	else
	{
		set_pad_1_amp_env_modulation(pad_1_amp_mod_env_level, 0.0f);
	}

	if (filter_1_freq_mod_lfo > _LFO_NONE)
	{
		set_filter_1_freq_lfo_modulation(filter_1_freq_mod_lfo_level, lfo_out_value[filter_1_freq_mod_lfo - 1]);
	}
	else
	{
		filter_1_freq_lfo_modulation_value = 0.f;
	}

	if (filter_1_freq_mod_env > _ENV_NONE)
	{
		set_filter_1_freq_env_modulation(filter_1_freq_mod_env_level, adsr_out_value[filter_1_freq_mod_env - 1]);
	}
	else
	{
		set_filter_1_freq_env_modulation(filter_1_freq_mod_env_level, 0.0f);
	}

	if (filter_2_freq_mod_lfo > _LFO_NONE)
	{
		set_filter_2_freq_lfo_modulation(filter_2_freq_mod_lfo_level, lfo_out_value[filter_2_freq_mod_lfo - 1]);
	}
	else
	{
		filter_2_freq_lfo_modulation_value = 0.f;
	}

	if (filter_2_freq_mod_env > _ENV_NONE)
	{
		set_filter_2_freq_env_modulation(filter_2_freq_mod_env_level, 
										adsr_out_value[filter_2_freq_mod_env - 1]);
	}
	else
	{
		set_filter_2_freq_env_modulation(filter_2_freq_mod_env_level, 0.0f);
	}
}

/**
*	@brief	Update modulation factors
*	@param	voice	voice number to be updated
*	@return void
*/
void DSP_AdjSynthVoice::update_voice_modulation(int voice)
{
	// Frequency modulation
	osc_1_freq_modulation_value = osc_1_freq_lfo_modulation_value + osc_1_freq_env_modulation_value;

	if (osc_1_freq_modulation_value < -1.0f)
	{
		osc_1_freq_modulation_value = -1.0f;
	}
	else if (osc_1_freq_modulation_value > 1.0f)
	{
		osc_1_freq_modulation_value = 1.0f;
	}
	
	osc_2_freq_modulation_value = osc_2_freq_lfo_modulation_value + osc_2_freq_env_modulation_value;
	if (osc_2_freq_modulation_value < -1.0f)
	{
		osc_2_freq_modulation_value = -1.0f;
	}
	else if (osc_2_freq_modulation_value > 1.0f)
	{
		osc_2_freq_modulation_value = 1.0f;
	}

	mso_1_freq_modulation_value = mso_1_freq_lfo_modulation_value + mso_1_freq_env_modulation_value;
	if (mso_1_freq_modulation_value < -1.0f)
	{
		mso_1_freq_modulation_value = -1.0f;
	}
	else if (mso_1_freq_modulation_value > 1.0f)
	{
		mso_1_freq_modulation_value = 1.0f;
	}

	pad_1_freq_modulation_value = pad_1_freq_lfo_modulation_value + pad_1_freq_env_modulation_value;
	if (pad_1_freq_modulation_value < -1.0f)
	{
		pad_1_freq_modulation_value = -1.0f;
	}
	else if (pad_1_freq_modulation_value > 1.0f)
	{
		pad_1_freq_modulation_value = 1.0f;
	}
	
	osc_1_freq_detune_value = detune_frequency_factor(
		osc_1->get_freq_detune_oct(),
		osc_1->get_freq_detune_semitones(),
		osc_1->get_freq_detune_cents(),
		osc_1_freq_modulation_value);
	
	osc_2_freq_detune_value = detune_frequency_factor(
		osc_2->get_freq_detune_oct(),
		osc_2->get_freq_detune_semitones(),
		osc_2->get_freq_detune_cents(),
		osc_2_freq_modulation_value);

	mso_1_freq_detune_value = detune_frequency_factor(
		mso_1->get_freq_detune_oct(),
		mso_1->get_freq_detune_semitones(),
		mso_1->get_freq_detune_cents(),
		mso_1_freq_modulation_value);

	pad_1_freq_detune_value = detune_frequency_factor(
		pad_1->get_freq_detune_oct(),
		pad_1->get_freq_detune_semitones(),
		pad_1->get_freq_detune_cents(),
		pad_1_freq_modulation_value);
	
	osc_1_actual_frequency = frequency * osc_1_freq_detune_value;
	osc_2_actual_frequency = frequency * osc_2_freq_detune_value;
	mso_1_actual_frequency = frequency * mso_1_freq_detune_value;
	pad_1_actual_frequency = frequency * pad_1_freq_detune_value;
	
	// PWM
	if (osc_1_pwm_lfo_modulation_value + osc_1_pwm_env_modulation_value > 0.0)
	{
		osc_1_pwm_modulation_value = osc_1->get_pwm_dcycle_set_val() / 100.0f + 
							osc_1_pwm_lfo_modulation_value + osc_1_pwm_env_modulation_value;
		
		if (osc_1_pwm_modulation_value < -1.0f)
		{
			osc_1_pwm_modulation_value = -1.0f;
		}
		else if (osc_1_pwm_modulation_value > 1.0f)
		{
			osc_1_pwm_modulation_value = 1.0f;
		}
		
		osc_1->set_pwm_dcycle(50 + (int)(osc_1_pwm_modulation_value * 45.0f));
	}
	
	if (osc_2_pwm_lfo_modulation_value + osc_2_pwm_env_modulation_value > 0.0)
	{
		osc_2_pwm_modulation_value = osc_2->get_pwm_dcycle_set_val() / 100.0f + 
							osc_2_pwm_lfo_modulation_value + osc_2_pwm_env_modulation_value;
		
		if (osc_2_pwm_modulation_value < -1.0f)
		{
			osc_2_pwm_modulation_value = -1.0f;
		}
		else if (osc_2_pwm_modulation_value > 1.0f)
		{
			osc_2_pwm_modulation_value = 1.0f;
		}
		
		osc_2->set_pwm_dcycle(50 + (int)(osc_2_pwm_modulation_value * 45.0f));
	}

	if (mso_1_pwm_lfo_modulation_value + mso_1_pwm_env_modulation_value > 0.0)
	{
		mso_1_pwm_modulation_value = mso_1->get_pwm_dcycle_set_val() / 100.0f + 
							mso_1_pwm_lfo_modulation_value + mso_1_pwm_env_modulation_value;
		
		if (mso_1_pwm_modulation_value < -1.0f)
		{
			mso_1_pwm_modulation_value = -1.0f;
		}
		else if (mso_1_pwm_modulation_value > 1.0f)
		{
			mso_1_pwm_modulation_value = 1.0f;
		}
		
		mso_1->set_pwm_dcycle(50 + (int)(mso_1_pwm_modulation_value * 45.0f));
	}
	
	// Amplitude
	osc_1_amp_modulation_value = osc_1_amp_lfo_modulation_value * osc_1_amp_env_modulation_value;
	osc_2_amp_modulation_value = osc_2_amp_lfo_modulation_value * osc_2_amp_env_modulation_value;
	mso_1_amp_modulation_value = mso_1_amp_lfo_modulation_value * mso_1_amp_env_modulation_value;
	pad_1_amp_modulation_value = pad_1_amp_lfo_modulation_value * pad_1_amp_env_modulation_value;
	noise_1_amp_modulation_value = noise_1_amp_lfo_modulation_value * noise_1_amp_env_modulation_value;
	
	// Look for active or activity
	if (
		(!osc_1_active || (osc_1_amp_env_modulation_value < 0.05f) || 
		((osc_1_send_filter_1_level < 0.05f) && (osc_1_send_filter_2_level < 0.05f))) &&
		(!osc_2_active || (osc_2_amp_env_modulation_value < 0.05f) || ((osc_2_send_filter_1_level < 0.05f) &&
		(osc_2_send_filter_2_level < 0.05f))) &&
		(!mso_1_active || (mso_1_amp_env_modulation_value < 0.05f) || ((mso_1_send_filter_1_level < 0.05f) &&
		(mso_1_send_filter_2_level < 0.05f))) &&
		(!pad_1_active || (pad_1_amp_env_modulation_value < 0.05f) || ((pad_1_send_filter_1_level < 0.05f) &&
		(pad_1_send_filter_2_level < 0.05f))) &&
		(!noise_1_active || (noise_1_amp_env_modulation_value < 0.05f) || ((noise_1_send_filter_1_level < 0.05f) &&
		(noise_1_send_filter_2_level < 0.05f))) &&
		(!karplus_1_active || (karplus_1->get_energy() < 0.000005) || ((karplus_1_send_filter_1_level < 0.05f) &&
		(karplus_1_send_filter_2_level < 0.05f))) &&
		voice_waits_for_not_active)
	{
		no_activity_detected(voice_id);
	}
	
	// Filter freq
	filter_1_freq_modulation_value = filter_1_freq_lfo_modulation_value + filter_1_freq_env_modulation_value;
	
	if (filter_1_freq_modulation_value < -1.0f)
	{
		filter_1_freq_modulation_value = -1.0f;
	}
	else if (filter_1_freq_modulation_value > 1.0f)
	{
		filter_1_freq_modulation_value = 1.0f;
	}
	
	filter_2_freq_modulation_value = filter_2_freq_lfo_modulation_value + filter_2_freq_env_modulation_value;
	
	if (filter_2_freq_modulation_value < -1.0f)
	{
		filter_2_freq_modulation_value = -1.0f;
	}
	else if (filter_2_freq_modulation_value > 1.0f)
	{
		filter_2_freq_modulation_value = 1.0f;
	}
}

/**
*	@brief	Calculate oscilators and generator next output values
*	@param	none
*	@return void
*/
void DSP_AdjSynthVoice::calc_next_oscilators_output_value()
{
	if (osc_1_active)
	{
		osc_1_out = osc_1->get_next_output_val(osc_1_actual_frequency) * osc_1_amp_modulation_value;
	}
	// Sync
	if (osc_2_sync_on_osc_1)
	{
		if (osc_1->get_cycle_restarted_sync_state())
		{
			osc_2->sync();
		}
	}

	if (osc_2_active)
	{
		osc_2_out = osc_2->get_next_output_val(osc_2_actual_frequency) * osc_2_amp_modulation_value;
	}

	if (noise_1_active)
	{
		noise_1_out = noise_1->get_next_noise_val() * noise_1_amp_modulation_value;  
	}
	
	if (karplus_1_active)
	{
		karplus_1_out = karplus_1->get_next_output_value();
	}

	if (mso_1_active)
	{
		mso_1_out = mso_1->get_next_mso_wtab_val(mso_1_actual_frequency, 0) * mso_1_amp_modulation_value;
	}

	if (pad_1_active)
	{
		pad_1->set_output_frequency(pad_1_actual_frequency, false); // false: do not init pointers
		pad_1->get_next_wavetable_value(&pad_1_out_1, &pad_1_out_2);
		pad_1_out_1 *= pad_1_amp_modulation_value;
		pad_1_out_2 *= pad_1_amp_modulation_value;
	}
}

/**
*	@brief	Calculate voice channel 1 next output value
*	@param	none
*	@return void
*/
float DSP_AdjSynthVoice::get_next_output_value_ch_1()
{		
	float sig_1 = osc_1_out * osc_1_send_filter_1_level +
				 osc_2_out * osc_2_send_filter_1_level +
				 noise_1_out * noise_1_send_filter_1_level +
				 karplus_1_out * karplus_1_send_filter_1_level +
				 mso_1_out * mso_1_send_filter_1_level +
				 pad_1_out_1 * pad_1_send_filter_1_level;

	if (distortion_1_active)
	{
		sig_1 = distortion_1->get_next_output_val(sig_1);
	}
	
	float filter_out = filter_1->filter_output(sig_1, filter_1_freq_modulation_value);
	
	return filter_out;
}

/**
*	@brief	Calculate voice channel 2 next output value
*	@param	none
*	@return void
*/
float DSP_AdjSynthVoice::get_next_output_value_ch_2()
{		
	float sig_2 = osc_1_out * osc_1_send_filter_2_level +
				 osc_2_out * osc_2_send_filter_2_level +
				 noise_1_out * noise_1_send_filter_2_level +
				 karplus_1_out * karplus_1_send_filter_2_level +
				 mso_1_out * mso_1_send_filter_2_level +
				 pad_1_out_2 * pad_1_send_filter_2_level;

	if (distortion_2_active)
	{
		sig_2 = distortion_2->get_next_output_val(sig_2);
	}

	float filter_out = filter_2->filter_output(sig_2, filter_2_freq_modulation_value);

	return filter_out;
}

/**
*	@brief	Trigger a note-on event on ENV generator
*	@param	adsr a pointer to an ENV (ADSR) generator object
*	@return void
*/
void DSP_AdjSynthVoice::adsr_note_on(DSP_ADSR *adsr)
{
	adsr->set_note_on();

	if (adsr->get_state() == ADSR_STATE_IDLE)
	{
		adsr->reset();
	}
	else if (adsr->get_state() != ADSR_STATE_FORCE_RELEASE)
	{
		adsr->set_state(ADSR_STATE_FORCE_RELEASE);
	}
}

/**
*	@brief	Trigger a note-off event on ENV generator
*	@param	adsr a pointer to an ENV (ADSR) generator object
*	@return void
*/
void DSP_AdjSynthVoice::adsr_note_off(DSP_ADSR *adsr)
{
	adsr->set_note_off();

	if (adsr->get_state() == ADSR_STATE_DECAY)
	{			 
		adsr->set_state(ADSR_STATE_OFF_RELEASE);
	}

	else if ((adsr->get_state() != ADSR_STATE_IDLE) && (adsr->get_state() != ADSR_STATE_FORCE_RELEASE))
	{
		adsr->set_state(ADSR_STATE_RELEASE);
	}
}


