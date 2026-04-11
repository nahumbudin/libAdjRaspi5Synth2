/**
*	@file		adjSynthEventsHandlingAmp.cpp
*	@author		Nahum Budin
*	@date		10-Oct-2025
*	@version	1.2
*					1. Limiting actions to Sketches only (program 16-18).
*					2. Not a voice block update callback
*	
*	@brief		AdjHeart Synthesizer Amplifiers Events Handling
*
*	History:\n
*	
*		version 1.1		11-Oct-2024
*		version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates an amplifier-mixer related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h 
*   @param  int ampid	target amp-mixer: _AMP_CH1_EVENT, _AMP_CH2_EVENT
*	@param	int eventid	specific event code:\n
*				_AMP_LEVEL, _AMP_PAN, _AMP_PAN_MOD_LFO, _AMP_PAN_MOD_LFO_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_AMP_LEVEL: 0-100;\n
*				_AMP_PAN: 0-100; 0 -> Left   50 -> Mid   100- -> Right\n	
*				_AMP_PAN_MOD_LFO:\n
*	@param	int prog program num
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
*				_AMP_PAN_MOD_LFO_LEVEL: 0-100\n
*				
*	@parm _settings_params_t *params active parameters
*
*   @return 0
*/
int AdjSynth::amp_event(int ampid, int eventid, int val, _settings_params_t *params, int program)
{
	int result = -1;
	
	if (eventid == _AMP_LEVEL)
	{
		// Master Volume - common to all programs
		if (ampid == _AMP_CH1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value(params,
															"adjsynth.amp_ch1.level",
															val,
															_EXEC_CALLBACK,
															program);
			
			result = update_program_voices_parameter(program, "adjsynth.amp_ch1.level");
		}
		else if (ampid == _AMP_CH2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value(params,
															"adjsynth.amp_ch2.level",
															val,
															_EXEC_CALLBACK,
															program);
			
			result = update_program_voices_parameter(program, "adjsynth.amp_ch2.level");			
		}
	}
	else if (eventid == _AMP_SEND)
	{
		// Master Send - common to all programs (TODO: add GUI send per channel)
		if (ampid == _AMP_CH1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value(params,
															"adjsynth.amp_ch1.send",
															val,
															_EXEC_CALLBACK,
															program);
			
			result = update_program_voices_parameter(program, "adjsynth.amp_ch1.send");
		}
		else if (ampid == _AMP_CH2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value(params,
															"adjsynth.amp_ch2.send",
															val,
															_EXEC_CALLBACK,
															program);
			
			result = update_program_voices_parameter(program, "adjsynth.amp_ch2.send");
		}
	}
	else
	{
		if ((program < _PROGRAM_16) || (program > _PROGRAM_18))
		{
			// Sketches only - all other programs: MIDI Mixer control
			return -1;
		}

		switch (eventid)
		{
			case _AMP_PAN:
				if (ampid == _AMP_CH1_EVENT)
				{
					adj_synth_settings_manager->set_int_param_value(
						params,
						"adjsynth.amp_ch1.pan",
						val,
						_EXEC_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp_ch1.pan");
				}
				else if (ampid == _AMP_CH2_EVENT)
				{
					adj_synth_settings_manager->set_int_param_value(
						params,
						"adjsynth.amp_ch2.pan",
						val,
						_EXEC_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp_ch2.pan");
				}

				break;

			case _AMP_PAN_MOD_LFO:
				if (ampid == _AMP_CH1_EVENT)
				{
					adj_synth_settings_manager->set_int_param_value(
						params,
						"adjsynth.amp_ch1.pan_modulation_lfo_num",
						val,
						_EXEC_BLOCK_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp_ch1.pan_modulation_lfo_num");
				}
				else if (ampid == _AMP_CH2_EVENT)
				{
					adj_synth_settings_manager->set_int_param_value(
						params,
						"adjsynth.amp_ch2.pan_modulation_lfo_num",
						val,
						_EXEC_BLOCK_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp_ch2.pan_modulation_lfo_num");
				}

				break;

			case _AMP_PAN_MOD_LFO_LEVEL:
				if (ampid == _AMP_CH1_EVENT)
				{
					adj_synth_settings_manager->set_int_param_value(
						params,
						"adjsynth.amp_ch1.pan_modulation_lfo_level",
						val,
						_EXEC_BLOCK_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp_ch1.pan_modulation_lfo_level");
				}
				else if (ampid == _AMP_CH2_EVENT)
				{
					adj_synth_settings_manager->set_int_param_value(
						params,
						"adjsynth.amp_ch2.pan_modulation_lfo_level",
						val,
						_EXEC_BLOCK_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp_ch2.pan_modulation_lfo_level");
				}

				break;

			case _AMP_FIXED_LEVELS: // TODO:
				if (val == _AMP_FIXED_LEVELS_ENABLE)
				{
					adj_synth_settings_manager->set_bool_param_value(
						params,
						"adjsynth.amp.fixed_levels_enabled",
						true,
						_EXEC_BLOCK_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp.fixed_levels_enabled");
				}
				else if (val == _AMP_FIXED_LEVELS_DISABLE)
				{
					adj_synth_settings_manager->set_bool_param_value(
						params,
						"adjsynth.amp.fixed_levels_enabled",
						false,
						_EXEC_BLOCK_CALLBACK,
						program);
					
					result = update_program_voices_parameter(program, "adjsynth.amp.fixed_levels_enabled");
				}

				break;
		}
	}

	return result;
	
}
