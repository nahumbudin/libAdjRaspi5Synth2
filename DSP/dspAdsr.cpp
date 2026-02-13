/**
* @file		dspAdsr.h
*	@author		Nahum Budin
*	@date		14-Sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*	
*	@History	23_Jan-2021 1.1
*					1. Code refactoring and notaion.
*					2. Adding update interval parameter as a setting parameter
*				30-Oct-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Envelope generator with 2 adjustable level outputs Send-1 and Send-2.
*/
#include <stdio.h>

#include "../LibAPI/synthesizer.h"

#include "dspAdsr.h"


/**
*   @brief  Constructor.
*   @param  voice polyphonic voice number this adsr is assigned to
*	@param	out_val initial output value (0.0 to 1.0)
*	@param	minimum_val minimum allowed output value
*	@param	time update interval time in seconds
*   @return void
*/
DSP_ADSR::DSP_ADSR(int voic, float out_val, float minimum_val, float time)
{
	voice = voic;

	set_state(ADSR_STATE_IDLE);
	output_val = out_val;
	if (output_val < 0)
	{
		output_val = 0;
	}
	else if (output_val > 1.0f)
	{
		output_val = 1.0f;
	}

	min_val = minimum_val;
	update_interval_sec = time;	
	send_level_1 = 0.0f;
	send_level_2 = 0.0f;
	forced_release_step = update_interval_sec / ADSR_FORCED_RELEASE_TIME_SEC;
	note_on = false;
}

DSP_ADSR::DSP_ADSR(int voic, float time)
{
	voice = voic;
	init(time);
}

/**
*   @brief  Initialize generator to default values and state.
*   @param  updtInt	update time interval [sec]
*   @return 0 if OK; -1 if error
*/
int DSP_ADSR::init(float updt_inter)
{
	if (updt_inter <= 0.f)
	{
		return -1;
	}
	else
	{
		set_state(ADSR_STATE_IDLE);
		output_val = 0;
		min_val = 0.05f;
		update_interval_sec = updt_inter;
		send_level_1 = 0.0f;
		send_level_2 = 0.0f;
		forced_release_step = update_interval_sec / ADSR_FORCED_RELEASE_TIME_SEC;
		note_on = false;
		
		return 0;
	}
}

/**
*   @brief  Clear output value to zero and set state to Attack.
*   @param  none
*   @return void
*/
void DSP_ADSR::reset() {
		
	set_state(ADSR_STATE_ATTACK);
	output_val = 0;
	forced_release_out_val = min_val;
}

/**
*   @brief  Sets the update interval time [sec].
*   @param  up_int	update interval time [sec]
*   @return 0 if OK; -1 if param out of range
*/
int DSP_ADSR::set_update_interval(float up_int)
{
	if (up_int > 0.f)
	{
		update_interval_sec = up_int;
		return 0;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Returns update interval time [sec].
*   @param  none
*   @return update interval time [sec]
*/
float DSP_ADSR::get_update_interval() { return update_interval_sec; }

/**
*   @brief  Return the generator output vale.
*   @param  none
*   @return float output value 0-1.0
*/
float DSP_ADSR::get_output_val() { return output_val; }

/**
*   @brief  Sets the generator state.
*   @param  st state ADSR_STATE_IDLE, ADSR_STATE_OFF_RELEASE, ADSR_STATE_RELEASE, 
*					 ADSR_STATE_SUSTAIN, ADSR_STATE_FORCE_RELEASE
*   @return new state if OK; -1 if illegal specified state
*/
int DSP_ADSR::set_state(int st)
{
	if ((st >= ADSR_STATE_IDLE) && (st <= ADSR_STATE_OFF_RELEASE))
	{
			
		if ((st == ADSR_STATE_RELEASE) && (state != ADSR_STATE_SUSTAIN))
		{
			state = ADSR_STATE_FORCE_RELEASE;
			forced_release_out_val = output_val;
			//	fprintf(stderr, "Forced Out lev (new): %f\n", forcedReleaseOutVal);
		}
		else
		{
			state = st;
			if (state == ADSR_STATE_FORCE_RELEASE)
			{
				forced_release_out_val = output_val; // Curent output value
				//	fprintf(stderr, "Forced Out lev (cont): %f\n", forcedReleaseOutVal);
			}
		}
		
		//	fprintf(stderr, "State: %i\n", state);
		
		return state;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Return the generator state.
*   @param  none
*   @return generator state
*/
int DSP_ADSR::get_state() { return state; }

/** 
*	@brief	Set Attack time in seconds  (actual value)
*	@param atsec (float) attack time in seconds 0 to ADSR_MAX_ATTACK_TIME_SEC
*	@return Attack time set value
*/
float DSP_ADSR::set_attack_time_sec(float at_sec)
{
	attack_time_sec = at_sec;
	if (attack_time_sec > ADSR_MAX_ATTACK_TIME_SEC) 
	{
		attack_time_sec = ADSR_MAX_ATTACK_TIME_SEC;
	}
	else if (attack_time_sec <= 0.005f) 
	{
		attack_time_sec = 0.005f;
	}
		
	attack_step = update_interval_sec / attack_time_sec;
		
	return attack_time_sec;
}

/**
*	@brief	Set Attack time 0-100 and calculate the time in seconds
*			based on a logarithmic base(10) in the range of 0 to ADSR_MAX_ATTACK_TIME_SEC
*	@param atsec attack time index
*	@return Attack time set value in sec
*/
float DSP_ADSR::set_attack_time_sec_log(int at_sec)
{
	return set_attack_time_sec(Utils::calc_log_scale_100_float(0.0, ADSR_MAX_ATTACK_TIME_SEC, 10.0, at_sec));
}

/**
*	@brief	Returns attack time in seconds
*	@param none
*	@return Attack time set value in sec
*/
float DSP_ADSR::get_attack_time() { return attack_time_sec; }

/**
*	@brief	Returns attack step
*	@param none
*	@return Attack step
*/
float DSP_ADSR::get_attack_step() { return attack_step; }

/**
*	@brief	Set Decay time in seconds  (actual value)
*	@param atsec (float) attack time in seconds ADSR_MIN_DECAY_TIME_SEC to 
*		ADSR_MAX_DECAY_TIME_SEC
*	@return Attack time set value
*/
float DSP_ADSR::set_decay_time_sec(float dc_sec)
{
	decay_time_sec = dc_sec;
	if (decay_time_sec > ADSR_MAX_DECAY_TIME_SEC) 
	{
		decay_time_sec = ADSR_MAX_DECAY_TIME_SEC;
	}
	else if (decay_time_sec <= 0.002f) 
	{
		decay_time_sec = 0.002f;
	}
		
	decay_step = update_interval_sec / decay_time_sec;
		
	return decay_time_sec;
}

/**
*	@brief	Set Decay time 0-100 and calculate the time in seconds
*			based on a logarithmic base(10) in the range of 0 to ADSR_MAX_ATTACK_TIME_SEC
*	@param dcsec decay time index 0-100
*	@return Attack time set value in sec
*/
float DSP_ADSR::set_decay_time_sec_log(int dc_sec)
{
	return set_decay_time_sec(Utils::calc_log_scale_100_float(0.0, ADSR_MAX_DECAY_TIME_SEC, 10.0, dc_sec));
}

/**
*	@brief	Returns decay time in seconds
*	@param none
*	@return Attack time set value in sec
*/
float DSP_ADSR::get_decay_time() { return decay_time_sec; }

/**
*	@brief	Returns decay step
*	@param none
*	@return decay step
*/
float DSP_ADSR::get_decay_step() { return decay_step; }

/** 
 *	@param Set Sustain level 0-100%
 *	@param stsec: Sustain vale
 *	@return Resulted Sustain value 
*/
int DSP_ADSR::set_sustain_level(int st_sec)
{
	sustain_level_precentages = st_sec;
	if (sustain_level_precentages > 100) 
	{
		sustain_level_precentages = 100;
	}
	else if (sustain_level_precentages < 0) 
	{
		sustain_level_precentages = 0;
	}
		
	return sustain_level_precentages;	
}

/**
*	@brief	Returns Sustain level
*	@param none
*	@return Sustain level
*/
int DSP_ADSR::get_sustain_level() { return sustain_level_precentages; }

/**
*	@brief	Set Release time in seconds  (actual value)
*	@param atsec (float) attack time in seconds ADSR_MIN_RELEASE_TIME_SEC to
*		ADSR_MAX_RELEASE_TIME_SEC
*	@return Release time set value
*/
float DSP_ADSR::set_release_time_sec(float rl_sec)
{
	release_time_sec = rl_sec;
	if (release_time_sec > ADSR_MAX_RELEASE_TIME_SEC) 
	{
		release_time_sec = ADSR_MAX_RELEASE_TIME_SEC;
	}
	else if (release_time_sec <= 0.08f) 
	{
		release_time_sec = 0.08f;
	}
		
	release_step = update_interval_sec / release_time_sec;
		
	return release_time_sec;
}

/**
*	@brief	Set Release time 0-100 and calculate the time in seconds
*			based on a logarithmic base(10) in the range of 0 to ADSR_MAX_RELEASE_TIME_SEC
*	@param dcsec decay time index 0-100
*	@return Attack time set value in sec
*/
float DSP_ADSR::set_release_time_sec_log(int dc_sec)
{
	return set_release_time_sec(Utils::calc_log_scale_100_float(0.0, ADSR_MAX_RELEASE_TIME_SEC, 10.0, dc_sec));
}

/**
*	@brief	Returns release time in seconds
*	@param none
*	@return Releasetime set value in sec
*/
float DSP_ADSR::get_release_time() { return release_time_sec; }

/**
*	@brief	Returns release step
*	@param none
*	@return Release step
*/
float DSP_ADSR::get_release_step() { return release_step; }

/**
*	@brief	Set output 1 send level 0-1.0
*	@param snd 0.0-1.0
*	@return send level 1
*/
float DSP_ADSR::set_send_level_1(float snd)
{
	send_level_1 = snd;
	if (send_level_1 < 0.0f) 
	{
		send_level_1 = 0.0f;
	}
	else if (send_level_1 > 1.0f) 
	{
		send_level_1 = 1.0f;
	}
	
	return send_level_1;
}

/**
*	@brief	Set output 1 send level 0-100
*	@param snd 0-100
*	@return send level 1 0.0-1.0
*/
float DSP_ADSR::set_send_level_1(int snd)
{
	float level = (float)snd / 100.0f;

	set_send_level_1(level);
	return level;
}

/**
*	@brief	Set output 2 send level 0-1.0
*	@param snd 0.0-1.0
*	@return send level 2
*/
float DSP_ADSR::set_send_level_2(float snd)
{
	send_level_2 = snd;
	if (send_level_2 < 0.0f) 
	{
		send_level_2 = 0.0f;
	}
	else if (send_level_2 > 1.0f) 
	{
		send_level_2 = 1.0f;
	}
	
	return send_level_2;
}

/**
*	@brief	Set output 1 send level 0-100
*	@param snd 0-100
*	@return send level 1 0.0-1.0
*/
float DSP_ADSR::set_send_level_2(int snd)
{
	float level = (float)snd / 100.0f;

	set_send_level_2(level);

	return level;
}

/**
*	@brief	Returns send level output 1
*	@param none
*	@return send level output 1
*/
float DSP_ADSR::get_send_level_1() { return send_level_1; }

/**
*	@brief	Returns send level output 2
*	@param none
*	@return send level output 2
*/
float DSP_ADSR::get_send_level_2() { return send_level_2; }

/**
*	@brief	Returns the note on elapsed time in sec
*	@param none
*	@return note on elapsed tim in sec
*/
uint32_t DSP_ADSR::get_note_on_elapsed_time() { return note_on_elapsed_time;  }


/** 
 *	@brief	Calculate the next Output value 0-1.0
 *	@param	none
 *	@return: ext Output value 0-1.0 
 */	
float DSP_ADSR::calc_next_envelope_val() {
		
	static float decay_off_level = 0.0f;
	
	switch (state) {
		
		case ADSR_STATE_IDLE:
			output_val = 0.0f;
			break;
				
		case ADSR_STATE_ATTACK:
			output_val += attack_step;
			if (output_val >= 1.0) {
				// End of attack state 
				output_val = 1.0f;
				set_state(ADSR_STATE_DECAY);
			}
			break;
					
		case ADSR_STATE_DECAY:
			// +1: if Sustain==0 
			output_val -= decay_step * (1 - (((float)sustain_level_precentages + 1) / 100.0));
			if ((output_val - (((float)sustain_level_precentages + 1) / 100.0) <= min_val))
			{
				set_state(ADSR_STATE_SUSTAIN);
			}
			break;
						
		case ADSR_STATE_SUSTAIN:
			// No change, unless the level is very small, than release 
			if (output_val <= 2*min_val)
			{
				output_val -= 0.1f * release_time_sec;
			}
			if (output_val <= min_val)
			{
				output_val = 0.0f;
			}
			break;
		
		case ADSR_STATE_RELEASE:
			if (output_val <= release_step * (((float)sustain_level_precentages + 1) / 100.0))
			{
				output_val = 0.0f;
				set_state(ADSR_STATE_IDLE);
			}
			else
			{
				output_val -= release_step * (((float)sustain_level_precentages + 1) / 100.0);
			}
			break;
					
		case ADSR_STATE_FORCE_RELEASE:
			if (output_val <= forced_release_step * forced_release_out_val)
			{
				output_val = 0.0f;
				if (note_on)
				{
					set_state(ADSR_STATE_ATTACK);
				}
				else
				{
					set_state(ADSR_STATE_IDLE);
				}
			}
			else
			{
				output_val -= forced_release_step * forced_release_out_val;
			}
			break;

		case ADSR_STATE_OFF_RELEASE:
			if (decay_off_level == 0)
			{
				decay_off_level = output_val;
			}
				
			if ((output_val <= release_step * decay_off_level) || (decay_off_level <= 0.1))
			{
				output_val = 0.0f;
				decay_off_level = 0.0f;
				set_state(ADSR_STATE_IDLE);
			}
			else
			{
				output_val -= release_step * decay_off_level;
			}

			break;
	}
	
	//	fprintf(stderr, "Ladsr Out: %f  sent: %f\n", outputVal, sendLevel);

	if (note_on)
	{
		note_on_elapsed_time++;
	}
	
	return output_val;
}

/**
 *	@brief	set note on state and restart note-on timer
 *	@param	none
 *	@return: none
 */
void DSP_ADSR::set_note_on() 
{ 
	note_on = true;
	note_on_elapsed_time = 0;
}

/**
 *	@brief	reset note on state 
 *	@param	none
 *	@return: none
 */
void DSP_ADSR::set_note_off() 
{ 
	note_on = false; 
}


