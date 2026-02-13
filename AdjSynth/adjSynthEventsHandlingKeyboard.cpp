/**
*	@file		adjSynthEventsHandlingKeyboard.cpp
*	@author		Nahum Budin
*	@date		11-Oct-2024
*	@version	1.1
*					1. Code refactoring and notaion.
*
*	@brief		AdjHeart Synthesizer Keyboard Events Handling
*
*	History:\n
*
*	version 1.0		16-12-2019:
*		First version
*
*/

#include "adjSynth.h"

/**
*   @brief  Initiates keyboard related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int kbid	target keyboard: _KBD_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KBD_PORTAMENTO_LEVEL, _KBD_SENSITIVITY_LEVEL, _KBD_LOW_SENSITIVITY_LEVEL\n
*				_KBD_SPLIT_POINT\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_KBD_PORTAMENTO_LEVEL: 1-100\n
*				_KBD_SENSITIVITY_LEVEL: 0-100\n
*				_KBD_LOW_SENSITIVITY_LEVEL: 0-100\n
*				_KBD_SPLIT_POINT: _KBD_SPLIT_POINT_NONE, _KBD_SPLIT_POINT_C2, _KBD_SPLIT_POINT_C3\n
*				_KBD_SPLIT_POINT_C4, _KBD_SPLIT_POINT_C5\n
*	@param	int program	program number
*
*   @return 0
*/
int AdjSynth::kbd_event_int(int kbid, int eventid, int val, _settings_params_t *params)
{
	if (kbid == _KBD_1_EVENT)
	{
		switch (eventid)
		{
		case _KBD_PORTAMENTO_LEVEL:
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.keyboard.portamento",
				val,
				_EXEC_CALLBACK,
				-1); // -1: no program (common resource)

			break;

		case _KBD_SENSITIVITY_LEVEL:
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.keyboard.sensetivity",
				val,
				_EXEC_CALLBACK,
				-1);

			break;

		case _KBD_LOW_SENSITIVITY_LEVEL:
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.keyboard.sensetivity_low",
				val,
				_EXEC_CALLBACK,
				-1);

			break;

		case _KBD_SPLIT_POINT:
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.keyboard.split_point",
				val,
				_EXEC_CALLBACK,
				-1);

			break;

		default:
			return 0;
		}
	}

	return 0;
}

int AdjSynth::kbd_event_bool(int kbid, int eventid, bool val, _settings_params_t *params)
{
	if (kbid == _KBD_1_EVENT)
	{
		switch (eventid)
		{
		case _KBD_PORTAMENTO_ENABLE:
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.keyboard.portamento_state",
				val,
				_EXEC_CALLBACK,
				-1);

			break;

		case _KBD_POLY_MODE_LIMIT:
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.keyboard.polyphonic_mode",
				_KBD_POLY_MODE_LIMIT,
				_EXEC_CALLBACK,
				-1);

				
			break;

		case _KBD_POLY_MODE_FIFO:
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.keyboard.polyphonic_mode",
				_KBD_POLY_MODE_FIFO,
				_EXEC_CALLBACK,
				-1);
				
			break;

		case _KBD_POLY_MODE_REUSE:
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.keyboard.polyphonic_mode",
				_KBD_POLY_MODE_REUSE,
				_EXEC_CALLBACK,
				-1);

			break;

		default:
			return 0;
		}
	}

	return 0;
}
