/**
 *	@file		adjSynthEventsHandlingVCO.cpp
 *	@author		Nahum Budin
 *	@date		11-Oct-2025
 *	@version	1.2
 *					1. Code refactoring and notaion.
 *					2. Update callbacks are changed from block to only program
 *						_EXEC_BLOCK_CALLBACK -> _EXEC_CALLBACK.
*	
*	@brief		AdjHeart Synthesizer VCOs Events Handling
*
*	History:\n
*	
*	version	1.1	5-Feb-2021	Code refactoring and notaion.
*	version 1.0	15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"
#include "../LibAPI/synthesizer.h"

/**
*   @brief  Initiates a VCO related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_PARAM_WAVEFORM, _OSC_PWM_SYMMETRY\n
*				_OSC_DETUNE_OCTAVE, _OSC_DETUNE_SEMITONES, _OSC_DETUNE_CENTS\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2\n
*				_OSC_UNISON_MODE, _OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE  (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_1, _OSC_UNISON_LEVEL_2, _OSC_UNISON_LEVEL_3 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_4, _OSC_UNISON_LEVEL_5, _OSC_UNISON_LEVEL_6 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_7, _OSC_UNISON_LEVEL_8, _OSC_UNISON_LEVEL_9 (valid only for OSC 1)\n
*				_OSC_HAMMOND_PERCUSION_MODE									  (valid only for OSC 1)\n
*				_OSC_FREQ_MOD_LFO, _OSC_FREQ_MOD_LFO_LEVEL\n
*				_OSC_PWM_MOD_LFO, _OSC_PWM_MOD_LFO_LEVEL\n
*				_OSC_AMP_MOD_LFO, _OSC_AMP_MOD_LFO_LEVEL\n
*				_OSC_FREQ_MOD_ENV, _OSC_FREQ_MOD_ENV_LEVEL\n
*				_OSC_PWM_MOD_ENV, _OSC_PWM_MOD_ENV_LEVEL\n
*				_OSC_AMP_MOD_ENV, _OSC_AMP_MOD_ENV_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_OSC_PARAM_WAVEFORM:\n
\verbatim
				_OSC_WAVEFORM_SINE, _OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_PULSE
				_OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAMPHOLD
\endverbatim
*				_OSC_PWM_SYMMETRY: 5-95\n
*				_OSC_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
*				_OSC_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
*				_OSC_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2: 0-100\n
*				_OSC_UNISON_MODE:\n
\verbatim
	_OSC_UNISON_MODE_12345678, _OSC_UNISON_MODE_HAMMOND, _OSC_UNISON_MODE_OCTAVES
	_OSC_UNISON_MODE_C_CHORD, _OSC_UNISON_MODE_Cm_CHORD
	_OSC_UNISON_MODE_C7_CHORD, _OSC_UNISON_MODE_Cm7_CHORD
\endverbatim
*				_OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE: 0-99\n
*				_OSC_UNISON_LEVEL_1 - _OSC_UNISON_LEVEL_9: 0-100\n
*				_OSC_HAMMOND_PERCUSION_MODE:\n
\verbatim
	_HAMMOND_PERCUSION_MODE_OFF, _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST, _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_NORM_FAST, _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST, _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_NORM_FAST
\endverbatim
*				_OSC_FREQ_MOD_LFO, _OSC_PWM_MOD_LFO, _OSC_AMP_MOD_LFO:\n
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
*				_OSC_FREQ_MOD_LFO_LEVEL, _OSC_PWM_MOD_LFO_LEVEL, _OSC_AMP_MOD_LFO_LEVEL: 0-100\n
*				_OSC_FREQ_MOD_ENV, _OSC_PWM_MOD_ENV, _OSC_AMP_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_OSC_FREQ_MOD_ENV_LEVEL, _OSC_PWM_MOD_ENV_LEVEL, _OSC_AMP_MOD_ENV_LEVEL: 0-100\n
*
*   @param	params	a _setting_params_t parameters struct
*	@param	program	program number
*   @return 0 is done.
*/
int AdjSynth::vco_event_int(int vcoid, int eventid, int val, _settings_params_t *params, int program)
{
	return_val_if_true(params == NULL || adj_synth_settings_manager == NULL, _SETTINGS_BAD_PARAMETERS);

	int voice = 0;
	int i = 0;
	int logLev = 0;
	int j = 0;
	int value = val;
	int _tmp_val;
	int result = -1;
	_settings_int_param_t _int_param;

	if ((vcoid < 0) || (vcoid > _NUM_OF_VCOS))
	{	
		return -1;
	}

	switch (eventid)
	{
	case _OSC_PARAM_WAVEFORM:
		if (vcoid == _OSC_1_EVENT)
		{
			// Set settings parameter
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.waveform",
				val,
				_EXEC_CALLBACK,
				program);
			
			// Set active playing voice settings
			result = update_program_voices_parameter(program, "adjsynth.osc1.waveform");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.waveform",
				val,
				_EXEC_CALLBACK,
				program);
			
			// Set active playing voice settings
			result = update_program_voices_parameter(program, "adjsynth.osc2.waveform");
		}
		break;

	case _OSC_PWM_SYMMETRY:
			
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.symmetry",
				val,
				_EXEC_CALLBACK,
				program);
			
			// Set active playing voice settings
			result = update_program_voices_parameter(program, "adjsynth.osc1.symmetry");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.symmetry",
				val,
				_EXEC_CALLBACK,
				program);
			
			// Set active playing voice settings
			result = update_program_voices_parameter(program, "adjsynth.osc2.symmetry");
		}
		break;

	case _OSC_DETUNE_OCTAVE:				
		val += _OSC_DETUNE_MIN_OCTAVE;	
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.tune_offset_oct",
				val,
				_EXEC_CALLBACK,
				program);
			
			// Set active playing voice settings
			result = update_program_voices_parameter(program, "adjsynth.osc1.tune_offset_oct");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.tune_offset_oct",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.tune_offset_oct");
		}
		break;

	case _OSC_DETUNE_SEMITONES:
		val += _OSC_DETUNE_MIN_SEMITONES;	
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.tune_offset_semitones",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.tune_offset_semitones");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.tune_offset_semitones",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.tune_offset_semitones");
		}
		break;

	case _OSC_DETUNE_CENTS:
			
		val += _OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL; // 0.25 steps
				
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.tune_offset_cents",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.tune_offset_cents");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.tune_offset_cents",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.tune_offset_cents");
		}
		break;

	case _OSC_FILTER_SEND_1:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.send_filter_1",
				val,
				_EXEC_CALLBACK,
				program);
				
			adj_synth_settings_manager->get_int_param(params, "adjsynth.osc1.send_filter_1", &_int_param);
				
			if ((_int_param.value != _HAMMOND_PERCUSION_MODE_OFF) &&  hammond_percussion_on)
			{
				// Osc2 amp follows Osc1 amp					
				if (hammond_percussion_soft)
				{
					_tmp_val = val / 3;
				}
				else
				{
					_tmp_val = val;
				}
				
				result = update_program_voices_parameter(program, "adjsynth.osc1.hammond_percussion_mode");
					
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.send_filter_1",
					_tmp_val,
					_EXEC_CALLBACK,
					program);
				
				result = update_program_voices_parameter(program, "adjsynth.osc2.send_filter_1");
					
				update_ui_callback();
			}
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.send_filter_1",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.send_filter_1");
		}
		break;

	case _OSC_FILTER_SEND_2:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.send_filter_2",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.send_filter_2");
				
			adj_synth_settings_manager->get_int_param(params, "adjsynth.osc1.hammond_percussion_mode", &_int_param);
				
			if ((_int_param.value != _HAMMOND_PERCUSION_MODE_OFF) &&  hammond_percussion_on)
			{
				// Osc2 amp follows Osc1 amp					
				if (hammond_percussion_soft)
				{
					_tmp_val = val / 3;
				}
				else
				{
					_tmp_val = val;
				}
					
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.send_filter_2",
					_tmp_val,
					_EXEC_CALLBACK,
					program);
				
				result = update_program_voices_parameter(program, "adjsynth.osc2.send_filter_2");
					
				update_ui_callback();
			}
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.send_filter_2",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.send_filter_2");
		}
			
		break;

	case _OSC_UNISON_MODE:	
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_mode",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_mode");
		}
		break;

	case _OSC_HAMMOND_PERCUSION_MODE:
		if (voice == 0)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.osc1.hammond_percussion_mode",
				val,
				_SET_VALUE); // No action, just set value.
				
			set_hammond_percusion_mode(val, params, program);
			//				setSynthesizerParams(activeParams_x);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.hammond_percussion_mode");
		}
		break;

	case _OSC_UNISON_LEVEL_1:
		{
			if (vcoid == _OSC_1_EVENT)
			{
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc1.unison_level_1",
					val,
					_EXEC_CALLBACK,
					program);
				
				result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_1");
			}
		}
		break;

	case _OSC_UNISON_LEVEL_2:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.osc1.unison_level_2",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_2");
		}
		break;

	case _OSC_UNISON_LEVEL_3:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_level_3",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_3");
		}
		break;

	case _OSC_UNISON_LEVEL_4:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_level_4",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_4");
		}
		break;

	case _OSC_UNISON_LEVEL_5:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_level_5",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_5");
		}
		break;

	case _OSC_UNISON_LEVEL_6:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_level_6",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_6");
		}
		break;

	case _OSC_UNISON_LEVEL_7:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_level_7",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_7");
		}
		break;

	case _OSC_UNISON_LEVEL_8:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_level_8",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_8");
		}
		break;

	case _OSC_UNISON_LEVEL_9:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_level_9",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_9");
		}
		break;

	case _OSC_UNISON_DISTORTION:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_distortion",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_distortion");
		}
		break;

	case _OSC_UNISON_DETUNE:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.unison_detune",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_detune");
		}
		break;

	case _OSC_FREQ_MOD_LFO:
				
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.freq_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.freq_modulation_lfo_num");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.freq_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.freq_modulation_lfo_num");
		}
		break;

	case _OSC_FREQ_MOD_LFO_LEVEL:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.freq_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.freq_modulation_lfo_level");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.freq_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.freq_modulation_lfo_level");
		}
		break;

	case _OSC_FREQ_MOD_ENV:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.freq_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.freq_modulation_env_num");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.freq_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_level_1");
		}
		break;

	case _OSC_FREQ_MOD_ENV_LEVEL:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.freq_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.freq_modulation_env_level");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.freq_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.freq_modulation_env_level");
		}
		break;

	case _OSC_PWM_MOD_LFO:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.pwm_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.pwm_modulation_lfo_num");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.pwm_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.pwm_modulation_lfo_num");
		}
		break;

	case _OSC_PWM_MOD_LFO_LEVEL:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.pwm_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.pwm_modulation_lfo_level");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.pwm_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.pwm_modulation_lfo_level");
		}
		break;

	case _OSC_PWM_MOD_ENV:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.pwm_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.pwm_modulation_env_num");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.pwm_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.pwm_modulation_env_num");
		}
		break;

	case _OSC_PWM_MOD_ENV_LEVEL:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.pwm_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.pwm_modulation_env_level");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.pwm_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.pwm_modulation_env_level");
		}
		break;

	case _OSC_AMP_MOD_LFO:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.amp_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.amp_modulation_lfo_num");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.amp_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.amp_modulation_lfo_num");
		}
		break;

	case _OSC_AMP_MOD_LFO_LEVEL:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.amp_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.amp_modulation_lfo_level");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.amp_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.amp_modulation_lfo_level");
		}
		break;

	case _OSC_AMP_MOD_ENV:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.amp_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.amp_modulation_env_num");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.amp_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.amp_modulation_env_num");
		}
		break;

	case _OSC_AMP_MOD_ENV_LEVEL:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.amp_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.amp_modulation_env_level");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.amp_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.amp_modulation_env_level");
		}
		break;			
	} 
	
	return 0;
}

/**
*   @brief  Initiates a VCO related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_1_UNISON_SQUARE				(valid only for OSC 1)\n
*				_OSC_SYNC							(valid only for OSC 2)\n
*				_OSC_ENABLE
*	@param	bool val event parameter value  true or false (enable/disable)
*	@param	params	a _setting_params_t parameters struct
*	@param	program	program number
*   @return void
*/
int AdjSynth::vco_event_bool(int vcoid, int eventid, bool val, _settings_params_t *params, int program)
{
	int result = -1;
		
	switch (eventid)
	{
	case _OSC_ENABLE:
		if (vcoid == _OSC_1_EVENT)
		{
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.osc1.enabled",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.enabled");
		}
		else if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.osc2.enabled",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.enabled");
		}
		break;	
		
	case _OSC_SYNC:
		if (vcoid == _OSC_2_EVENT)
		{
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.osc2.sync_on_osc_1",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc2.sync_on_osc_1");
		}
			
		break;

	case _OSC_1_UNISON_SQUARE:
		if (vcoid == _OSC_1_EVENT)
		{	
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.osc1.unison_square_wave",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.osc1.unison_square_wave");
		}
			
		break;
			
	}
	
	return result;
}
