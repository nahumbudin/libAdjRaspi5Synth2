/**
*	@file		dspAdjSynthVoiceOsc.cpp
*	@author		Nahum Budin
*	@date		21-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	25_Jan-2021 1.1 Code refactoring and notaion. 
*				9-Jun-2018  1.0 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Oscilators Handling (dspAdjSynthVoice.h).
*				
*/

#include "dspAdjSynthVoice.h"

/**
*	@brief	Set OSC_1 send level to channel 1
*	@param lev OSC_1 send level to channel 1 0.0 to 1.0
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_1_send_filter_1_level(float lev)
{
	osc_1_send_filter_1_level = lev;
	
	if (osc_1_send_filter_1_level < 0.0f)
	{
		osc_1_send_filter_1_level = 0.0f;
	}
	else if (osc_1_send_filter_1_level > 1.0f)
	{
		osc_1_send_filter_1_level = 1.0f;
	}
}

/**
*	@brief	Set OSC_1 send level to channel 1 (int)
*	@param lev OSC_1 send level to channel 1 0-100
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_1_send_filter_1_level(int lev)
{
	set_osc_1_send_filter_1_level((float)lev / 100.0f);
}

/**
*	@brief	return OSC_1 send level to channel 1
*	@param none
*	@return OSC_1 send level to channel 1
*/
float DSP_AdjSynthVoice::get_osc_1_send_filter_1_level() 
{ 
	return osc_1_send_filter_1_level; 
}

/**
*	@brief	Set OSC_1 send level to channel 2
*	@param lev OSC_1 send level to channel 2 0.0 to 1.0
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_1_send_filter_2_level(float lev)
{
	osc_1_send_filter_2_level = lev;
	
	if (osc_1_send_filter_2_level < 0.0f)
	{
		osc_1_send_filter_2_level = 0.0f;
	}
	else if (osc_1_send_filter_2_level > 1.0f)
	{
		osc_1_send_filter_2_level = 1.0f;
	}
}

/**
*	@brief	Set OSC_1 send level to channel 2 (int)
*	@param lev OSC_1 send level to channel 2 (int) 0 to 100
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_1_send_filter_2_level(int lev)
{
	set_osc_1_send_filter_2_level((float)lev / 100.0f);
}

/**
*	@brief	return OSC_1 send level to channel 2
*	@param none
*	@return OSC_1 send level to channel 2
*/
float DSP_AdjSynthVoice::get_osc_1_send_filter_2_level() 
{ 
	return osc_1_send_filter_2_level;
}

/**
*	@brief	Set OSC_1 active LFO frequency modulator and mode (delayed or not)
*	@param	Lfo OSC_1 active LFO frequency modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_freq_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		osc_1_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		osc_1_freq_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set OSC_1 active LFO frequency modulator level
*	@param	Lev OSC_1 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_freq_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_1_freq_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)Lev/100.0;
	}
}

/**
*	@brief	Set OSC_1 active ENV frequency modulator 
*	@param	Env OSC_1 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_freq_mod_env(int env)
{
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		osc_1_freq_mod_env = env;
	}
}

/**
*	@brief	Set OSC_1 active ENV frequency modulator level
*	@param	Lev OSC_1 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_freq_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_1_freq_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)lev / 100.0; 
	}
}

/**
*	@brief	Set OSC_1 active LFO PWM modulator and mode (delayed or not)
*	@param	Lfo OSC_1 active LFO PWM modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_pwm_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		osc_1_pwm_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		osc_1_pwm_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set OSC_1 active LFO PWM modulator level
*	@param	Lev OSC_1 active LFO PWM modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_pwm_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_1_pwm_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)Lev/100.0;
	}
}

/**
*	@brief	Set OSC_1 active ENV PWM modulator 
*	@param	Env OSC_1 active ENV PWM modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_pwm_mod_env(int env)
{
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		osc_1_pwm_mod_env = env;
	}
}

/**
*	@brief	Set OSC_1 active ENV PWM modulator level
*	@param	Lev OSC_1 active ENV PWM modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_pwm_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_1_pwm_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)lev / 100.0; 
	}
}

/**
*	@brief	Set OSC_1 active LFO amp modulator and mode (delayed or not)
*	@param	Lfo OSC_1 active LFO amp modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_amp_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		osc_1_amp_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		osc_1_amp_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set OSC_1 active LFO amp modulator level
*	@param	Lev OSC_1 active LFO amp modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_amp_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_1_amp_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)Lev/100.0;
	}
}

/**
*	@brief	Set OSC_1 active ENV amp modulator 
*	@param	Env OSC_1 active ENV amp modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_amp_mod_env(int env)
{
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		osc_1_amp_mod_env = env;
	}
}

/**
*	@brief	Set OSC_1 active amp frequency modulator level
*	@param	Lev OSC_1 active amp frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_amp_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_1_amp_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)lev / 100.0; 
	}
}


/**
*	@brief	Return OSC_1 active LFO frequency modulator number
*	@param	none
*	@return OSC_1 active LFO frequency modulator number
*/
int DSP_AdjSynthVoice::get_osc_1_freq_mod_lfo() 
{ 
	return osc_1_freq_mod_lfo;  
}

/**
*	@brief	Return OSC_1 active LFO frequency modulator level
*	@param	none
*	@return OSC_1 active LFO frequency modulator level
*/
float DSP_AdjSynthVoice::get_osc_1_freq_mod_lfo_level() 
{ 
	return osc_1_pwm_mod_lfo_level; 
}

/**
*	@brief	Return OSC_1 active ENV frequency modulator number
*	@param	none
*	@return OSC_1 active ENV frequency modulator number
*/
int DSP_AdjSynthVoice::get_osc_1_freq_mod_env() 
{ 
	return osc_1_freq_mod_env; 
}

/**
*	@brief	Return OSC_1 active ENV frequency modulator level
*	@param	none
*	@return OSC_1 active ENV frequency modulator level
*/
float DSP_AdjSynthVoice::get_osc_1_freq_mod_env_level() 
{ 
	return osc_1_freq_mod_env_level; 
}

/**
*	@brief	Return OSC_1 active LFO pwm modulator number
*	@param	none
*	@return OSC_1 active LFO pwm modulator number
*/
int DSP_AdjSynthVoice::get_osc_1_pwm_mod_lfo() 
{ 
	return osc_1_pwm_mod_lfo; 
}

/**
*	@brief	Return OSC_1 active LFO pwm modulator level
*	@param	none
*	@return OSC_1 active LFO pwm modulator level
*/
float DSP_AdjSynthVoice::get_osc_1_pwm_mod_lfo_level() 
{ 
	return osc_1_pwm_mod_lfo_level; 
}

/**
*	@brief	Return OSC_1 active ENV pwm modulator number
*	@param	none
*	@return OSC_1 active ENV pwm modulator number
*/
int DSP_AdjSynthVoice::get_osc_1_pwm_mod_env() 
{ 
	return osc_1_pwm_mod_env; 
}

/**
*	@brief	Return OSC_1 active ENV pwm modulator level
*	@param	none
*	@return OSC_1 active ENV pwm modulator level
*/
float DSP_AdjSynthVoice::get_osc_1_pwm_mod_env_level() 
{ 
	return osc_1_pwm_mod_env_level; 
}

/**
*	@brief	Return OSC_1 active LFO amplitude modulator number
*	@param	none
*	@return OSC_1 active LFO amplitude modulator number
*/
int DSP_AdjSynthVoice::get_osc_1_amp_mod_lfo() 
{ 
	return osc_1_amp_mod_lfo; 
}

/**
*	@brief	Return OSC_1 active LFO amplitude modulator level
*	@param	none
*	@return OSC_1 active LFO amplitude modulator level
*/
float DSP_AdjSynthVoice::get_osc_1_amp_mod_lfo_level() 
{ 
	return osc_1_amp_mod_lfo_level; 
}

/**
*	@brief	Return OSC_1 active ENV amplitude modulator number
*	@param	none
*	@return OSC_1 active ENV amplitude modulator number
*/
int DSP_AdjSynthVoice::get_osc_1_amp_mod_env() 
{ 
	return osc_1_amp_mod_env; 
}

/**
*	@brief	Return OSC_1 active ENV amplitude modulator level
*	@param	none
*	@return OSC_1 active ENV amplitude modulator level
*/
float DSP_AdjSynthVoice::get_osc_1_amp_mod_env_level() 
{ 
	return osc_1_amp_mod_env_level; 
}


/**
*	@brief	Set OSC_2 send level to channel 1
*	@param lev OSC_2 send level to channel 1 0.0 to 1.0
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_2_send_filter_1_level(float lev)
{
	osc_2_send_filter_1_level = lev;
	
	if (osc_2_send_filter_1_level < 0.0f)
	{
		osc_2_send_filter_1_level = 0.0f;
	}
	else if (osc_2_send_filter_1_level > 1.0f)
	{
		osc_2_send_filter_1_level = 1.0f;
	}
}

/**
*	@brief	Set OSC_2 send level to channel 1 (int)
*	@param lev OSC_2 send level to channel 1 0-100
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_2_send_filter_1_level(int lev)
{
	set_osc_2_send_filter_1_level((float)lev / 100.0f);
}

/**
*	@brief	return OSC_2 send level to channel 1
*	@param none
*	@return OSC_2 send level to channel 1
*/
float DSP_AdjSynthVoice::get_osc_2_send_filter_1_level() 
{ 
	return osc_2_send_filter_1_level; 
}

/**
*	@brief	Set OSC_2 send level to channel 2
*	@param lev OSC_2 send level to channel 2 0.0 to 1.0
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_2_send_filter_2_level(float lev)
{
	osc_2_send_filter_2_level = lev;
	
	if (osc_2_send_filter_2_level < 0.0f)
	{
		osc_2_send_filter_2_level = 0.0f;
	}
	else if (osc_2_send_filter_2_level > 1.0f)
	{
		osc_2_send_filter_2_level = 1.0f;
	}
}

/**
*	@brief	Set OSC_2 send level to channel 2 (int)
*	@param lev OSC_2 send level to channel 2 (int) 0 to 100
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_2_send_filter_2_level(int lev)
{
	set_osc_2_send_filter_2_level((float)lev / 100.0f);
}

/**
*	@brief	return OSC_2 send level to channel 2
*	@param none
*	@return OSC_2 send level to channel 2
*/
float DSP_AdjSynthVoice::get_osc_2_send_filter_2_level() 
{ 
	return osc_2_send_filter_2_level;
}

/**
*	@brief	Set OSC_2 active LFO frequency modulator and mode (delayed or not)
*	@param	Lfo OSC_2 active LFO frequency modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_freq_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		osc_2_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		osc_2_freq_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set OSC_2 active LFO frequency modulator level
*	@param	Lev OSC_2 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_freq_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_2_freq_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)Lev/100.0;
	}
}

/**
*	@brief	Set OSC_2 active ENV frequency modulator 
*	@param	Env OSC_2 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_freq_mod_env(int env)
{
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		osc_2_freq_mod_env = env;
	}
}

/**
*	@brief	Set OSC_2 active ENV frequency modulator level
*	@param	Lev OSC_2 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_freq_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_2_freq_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)lev / 100.0; 
	}
}

/**
*	@brief	Set OSC_2 active LFO PWM modulator and mode (delayed or not)
*	@param	Lfo OSC_2 active LFO PWM modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_pwm_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		osc_2_pwm_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		osc_2_pwm_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set OSC_2 active LFO PWM modulator level
*	@param	Lev OSC_2 active LFO PWM modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_pwm_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_2_pwm_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)Lev/100.0;
	}
}

/**
*	@brief	Set OSC_2 active ENV PWM modulator 
*	@param	Env OSC_2 active ENV PWM modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_pwm_mod_env(int env)
{
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		osc_2_pwm_mod_env = env;
	}
}

/**
*	@brief	Set OSC_2 active ENV PWM modulator level
*	@param	Lev OSC_2 active ENV PWM modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_pwm_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_2_pwm_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)lev / 100.0; 
	}
}

/**
*	@brief	Set OSC_2 active LFO amp modulator and mode (delayed or not)
*	@param	Lfo OSC_2 active LFO amp modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_amp_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		osc_2_amp_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		osc_2_amp_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set OSC_2 active LFO amp modulator level
*	@param	Lev OSC_2 active LFO amp modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_amp_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_2_amp_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)Lev/100.0;
	}
}

/**
*	@brief	Set OSC_2 active ENV amp modulator 
*	@param	Env OSC_2 active ENV amp modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_amp_mod_env(int env)
{
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		osc_2_amp_mod_env = env;
	}
}

/**
*	@brief	Set OSC_2 active amp frequency modulator level
*	@param	Lev OSC_2 active amp frequency modulator level 0 to 100
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_amp_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		osc_2_amp_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev); // (float)lev / 100.0; 
	}
}


/**
*	@brief	Return OSC_2 active LFO frequency modulator number
*	@param	none
*	@return OSC_2 active LFO frequency modulator number
*/
int DSP_AdjSynthVoice::get_osc_2_freq_mod_lfo() 
{ 
	return osc_2_freq_mod_lfo;  
}

/**
*	@brief	Return OSC_2 active LFO frequency modulator level
*	@param	none
*	@return OSC_2 active LFO frequency modulator level
*/
float DSP_AdjSynthVoice::get_osc_2_freq_mod_lfo_level() 
{ 
	return osc_2_pwm_mod_lfo_level; 
}

/**
*	@brief	Return OSC_2 active ENV frequency modulator number
*	@param	none
*	@return OSC_2 active ENV frequency modulator number
*/
int DSP_AdjSynthVoice::get_osc_2_freq_mod_env() 
{ 
	return osc_2_freq_mod_env; 
}

/**
*	@brief	Return OSC_2 active ENV frequency modulator level
*	@param	none
*	@return OSC_2 active ENV frequency modulator level
*/
float DSP_AdjSynthVoice::get_osc_2_freq_mod_env_level() 
{ 
	return osc_2_freq_mod_env_level; 
}

/**
*	@brief	Return OSC_2 active LFO pwm modulator number
*	@param	none
*	@return OSC_2 active LFO pwm modulator number
*/
int DSP_AdjSynthVoice::get_osc_2_pwm_mod_lfo() 
{ 
	return osc_2_pwm_mod_lfo; 
}

/**
*	@brief	Return OSC_2 active LFO pwm modulator level
*	@param	none
*	@return OSC_2 active LFO pwm modulator level
*/
float DSP_AdjSynthVoice::get_osc_2_pwm_mod_lfo_level() 
{ 
	return osc_2_pwm_mod_lfo_level; 
}

/**
*	@brief	Return OSC_2 active ENV pwm modulator number
*	@param	none
*	@return OSC_2 active ENV pwm modulator number
*/
int DSP_AdjSynthVoice::get_osc_2_pwm_mod_env() 
{ 
	return osc_2_pwm_mod_env; 
}

/**
*	@brief	Return OSC_2 active ENV pwm modulator level
*	@param	none
*	@return OSC_2 active ENV pwm modulator level
*/
float DSP_AdjSynthVoice::get_osc_2_pwm_mod_env_level() 
{ 
	return osc_2_pwm_mod_env_level; 
}

/**
*	@brief	Return OSC_2 active LFO amplitude modulator number
*	@param	none
*	@return OSC_2 active LFO amplitude modulator number
*/
int DSP_AdjSynthVoice::get_osc_2_amp_mod_lfo() 
{ 
	return osc_2_amp_mod_lfo; 
}

/**
*	@brief	Return OSC_2 active LFO amplitude modulator level
*	@param	none
*	@return OSC_2 active LFO amplitude modulator level
*/
float DSP_AdjSynthVoice::get_osc_2_amp_mod_lfo_level() 
{ 
	return osc_2_amp_mod_lfo_level; 
}

/**
*	@brief	Return OSC_2 active ENV amplitude modulator number
*	@param	none
*	@return OSC_2 active ENV amplitude modulator number
*/
int DSP_AdjSynthVoice::get_osc_2_amp_mod_env() 
{ 
	return osc_2_amp_mod_env; 
}

/**
*	@brief	Return OSC_2 active ENV amplitude modulator level
*	@param	none
*	@return OSC_2 active ENV amplitude modulator level
*/
float DSP_AdjSynthVoice::get_osc_2_amp_mod_env_level() 
{ 
	return osc_2_amp_mod_env_level; 
}


/**
*	@brief	Set on OSC_2 sync on OSC_1
*	@param	none
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_2_sync_on_osc_1() 
{ 
	osc_2_sync_on_osc_1 = true; 
	if (osc_2 != NULL)
	{
		osc_2->set_sync_state(true);
	}
}

/**
*	@brief	Set off OSC_2 sync on OSC_1
*	@param	none
*	@return void
*/
void DSP_AdjSynthVoice::set_osc_2_not_sync_on_osc_1() 
{ 
	osc_2_sync_on_osc_1 = false; 
	if (osc_2 != NULL)
	{
		osc_2->set_sync_state(false);
	}
}

/**
*	@brief	Return OSC2 sync on OSC1 state
*	@param	none
*	@return true if OSC2 sync on OSC1 is on false otherwise
*/
bool DSP_AdjSynthVoice::get_osc_2_sync_on_osc_1_state() 
{ 
	return osc_2_sync_on_osc_1; 
}

/**
*	@brief	Set Osc_1 active LFO amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_amp_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val; // Modulation value (LFO: -1.0 to 1.0; Env: 0 to 1.0)
	
	if (adsr_1->note_on_elapsed_time < osc_1_amp_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}

	float factor = mod_factor; // 0 to 1.0 : indicates the modulator send set-level
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_1_amp_lfo_modulation_value = 1 - (1.0f + value) * factor / 2.0f;
}

/**
*	@brief	Set Osc_1 active ENV amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_amp_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_1_amp_env_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_1 active LFO pwm modulator modulation value
*	@param	modFactor	modulation factor (depth) 0.0 to 1.0
*	@param	modValue	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_pwm_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < osc_1_pwm_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}

	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_1_pwm_lfo_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_1 active ENV pwm modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_pwm_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_1_pwm_env_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_1 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < osc_1_freq_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}

	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_1_freq_lfo_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_1 active ENV frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_1_freq_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_1_freq_env_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_2 active LFO amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_amp_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val; // Modulation value (LFO: -1.0 to 1.0; Env: 0 to 1.0)
	
	if (adsr_1->note_on_elapsed_time < osc_2_amp_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}

	float factor = mod_factor; // 0 to 1.0 : indicates the modulator send set-level
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_2_amp_lfo_modulation_value = 1 - (1.0f + value) * factor / 2.0f;
}

/**
*	@brief	Set Osc_2 active ENV amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_amp_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_2_amp_env_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_2 active LFO pwm modulator modulation value
*	@param	modFactor	modulation factor (depth) 0.0 to 1.0
*	@param	modValue	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_pwm_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < osc_2_pwm_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}

	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_2_pwm_lfo_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_2 active ENV pwm modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_pwm_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_2_pwm_env_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_2 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr_1->note_on_elapsed_time < osc_2_freq_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}

	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_2_freq_lfo_modulation_value = factor * value;
}

/**
*	@brief	Set Osc_2 active ENV frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_AdjSynthVoice::set_osc_2_freq_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}

	osc_2_freq_env_modulation_value = factor * value;
}
