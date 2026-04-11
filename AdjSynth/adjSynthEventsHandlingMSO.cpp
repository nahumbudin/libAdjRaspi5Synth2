/**
 *	@file		adjSynthEventsHandlingMSO.cpp
 *	@author		Nahum Budin
 *	@date		11-Oct-2025
 *	@version	1.3
 *					1. Update callbacks are changed from block to only program
 *						_EXEC_BLOCK_CALLBACK -> _EXEC_CALLBACK.
 *
 *	@brief		AdjHeart Synthesizer Morphed Sinus Oscilator Events Handling
 *
 *	History:\n
 *
 *		version 1.2 11-Oct-2025	Code refactoring and notaion.
*		version	1.1	5-Feb-2021	Code refactoring and notaion.	
*		version 1.0	15_Nov-2019 First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a Morphed Sinus Oscilator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int msoid	target mso : _MSO_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MSO_SEGMENT_A_POSITION, _MSO_SEGMENT_B_POSITION, _MSO_SEGMENT_C_POSITION\n
*				_MSO_SEGMENT_D_POSITION, _MSO_SEGMENT_E_POSITION, _MSO_SEGMENT_F_POSITION
*				_MSO_SYMETRY

*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_MSO_SEGMENT_x_POSITION : 0 - _MSO_WAVEFORM_LENGTH - 1 (511)
*				_MSO_SYMETRY : 0 - 100

*				MSO_SEND_1, _NOISE_SEND_2: 0-100\n
*				MSO_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				MSO_AMP_MOD_LFO_LEVEL: 0-100\n

*				MSO_AMP_MOD_ENV:\n
\verbatim
_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, ENV_5
\endverbatim
*				_MSO_AMP_MOD_ENV_LEVEL: 0-100\n
*
*	@param	_settings_params_t params	active settings params
*	@param	int program	program number
*   @return void
*/
int AdjSynth::mso_event_int(int msoid, int eventid, int val, _settings_params_t *params, int program)
{
	int voice, i, logLev, j = 0, value = val;
	int result = -1;

	switch (msoid)
	{
	case _MSO_1_EVENT:
		if (eventid == _MSO_SEGMENT_A_POSITION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.segment_position_a",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.segment_position_a");
		}
		else if (eventid == _MSO_SEGMENT_B_POSITION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.segment_position_b",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.segment_position_b");
		}
		else if (eventid == _MSO_SEGMENT_C_POSITION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.segment_position_c",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.segment_position_c");
		}
		else if (eventid == _MSO_SEGMENT_D_POSITION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.segment_position_d",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.segment_position_d");
		}
		else if (eventid == _MSO_SEGMENT_E_POSITION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.segment_position_e",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.segment_position_e");
		}
		else if (eventid == _MSO_SEGMENT_F_POSITION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.segment_position_f",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.segment_position_f");
		}
		else if (eventid == _MSO_SYMETRY)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.symmetry",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.symmetry");
		}
		else if (eventid == _MSO_CALC_BASE_LUT)
		{
			synth_program[program]->mso_wtab->calc_segments_lengths(
				&synth_program[program]->mso_wtab->base_segment_lengths, 
				&synth_program[program]->mso_wtab->base_segment_positions);

			synth_program[program]->mso_wtab->calc_wtab(  /* [0] ?*/
				synth_program[program]->mso_wtab->base_waveform_tab, 
				&synth_program[program]->mso_wtab->base_segment_lengths, 
				&synth_program[program]->mso_wtab->base_segment_positions);
		}
		else if (eventid == _MSO_CALC_MORPHED_LUT)
		{			
			synth_program[program]->mso_wtab->set_morphing_symetry(synth_program[program]->mso_wtab->get_morphing_symetry());
			
			synth_program[program]->mso_wtab->calc_wtab(
				synth_program[program]->mso_wtab->morphed_waveform_tab,
				&synth_program[program]->mso_wtab->morphed_segment_lengths,
				&synth_program[program]->mso_wtab->morphed_segment_positions);
		}
		else if (eventid == _MSO_DETUNE_OCTAVE)
		{
			val += _OSC_DETUNE_MIN_OCTAVE;
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.tune_offset_oct",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.tune_offset_oct");
		}
		else if (eventid == _MSO_DETUNE_SEMITONES)
		{
			val += _OSC_DETUNE_MIN_SEMITONES;
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.tune_offset_semitones",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.tune_offset_semitones");
		}
		else if (eventid == _MSO_DETUNE_CENTS)
		{
			val += _OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL; // 0.25 steps

			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.tune_offset_cents",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.tune_offset_cents");
		}
		else if (eventid == _MSO_FILTER_SEND_1)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.send_filter_1",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.send_filter_1");
		}
		else if (eventid == _MSO_FILTER_SEND_2)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.send_filter_2",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.send_filter_2");
		}
		else if (eventid == _MSO_FREQ_MOD_LFO)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.freq_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.freq_modulation_lfo_num");
		}
		else if (eventid == _MSO_FREQ_MOD_LFO_LEVEL)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.freq_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.freq_modulation_lfo_level");
		}
		else if (eventid == _MSO_FREQ_MOD_ENV)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.freq_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.freq_modulation_env_num");
		}
		else if (eventid == _MSO_FREQ_MOD_ENV_LEVEL)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.freq_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.freq_modulation_env_level");
		}
		else if (eventid == _MSO_PWM_MOD_LFO)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.pwm_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.pwm_modulation_lfo_num");
		}
		else if (eventid == _MSO_PWM_MOD_LFO_LEVEL)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.pwm_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.pwm_modulation_lfo_level");
		}
		else if (eventid == _MSO_PWM_MOD_ENV)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.pwm_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.pwm_modulation_env_num");
		}
		else if (eventid == _MSO_PWM_MOD_ENV_LEVEL)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.pwm_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.pwm_modulation_env_level");
		}
		else if (eventid == _MSO_AMP_MOD_LFO)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.amp_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.amp_modulation_lfo_num");
		}
		else if (eventid == _MSO_AMP_MOD_LFO_LEVEL)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.amp_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.amp_modulation_lfo_level");
		}
		else if (eventid == _MSO_AMP_MOD_ENV)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.amp_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.amp_modulation_env_num");
		}
		else if (eventid == _MSO_AMP_MOD_ENV_LEVEL)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.mso_synth.amp_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.amp_modulation_env_level");
		}
	}
	
	return result;
}

/**
*   @brief  Initiates a Morphed Sinus Oscilator related event with boolean value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int msoid	target mso : _MSO_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MSO_ENABLE
*	@param	boolean val event parameter value true / false:\n*
*	@param	_settings_params_t params	active settings params
*	@param	int program	program number
*   @return void
*/
int AdjSynth::mso_event_bool(int msoid, int eventid, bool val, _settings_params_t *params, int program)
{
	int result = -1;
	
	switch (msoid)
	{
	case _MSO_1_EVENT:
		if (eventid == _MSO_ENABLE)
		{
			adj_synth_settings_manager->set_bool_param_value
					(params,
				"adjsynth.mso_synth.enabled",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.mso_synth.enabled");
			
		}
		break;
	}
	
	return result;
}
