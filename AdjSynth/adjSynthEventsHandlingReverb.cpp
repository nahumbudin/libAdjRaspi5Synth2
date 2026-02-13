/**
*	@file		adjSynthEventsHandlingReverb.cpp
*	@author		Nahum Budin
*	@date		11-Oct-2024
*	@version	1.1
*					Code refactoring and notaion.
*	
*	@brief		AdjHeart SynthesizerReverbss Events Handling
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a reverbration effect related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ROOM_SIZE, _REVERB_DAMP, _REVERB_WET, _REVERB_DRY\n
*				_REVERB_WIDTH, _REVERB_MODE, _REVERB_PRESET
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ROOM_SIZE: 10-98\n
*				_REVERB_DAMP: 0-100\n
*				_REVERB_WET: 0-100\n
*				_REVERB_DRY: 0-100\n
*				_REVERB_WIDTH: 1-100\n
*				_REVERB_MODE: 0-50
*				_REVERB_PRESET: enum sf_reverb_preset (defined in LibAPI/synthesizer.h)\n
*
*   @return 0
*/
int AdjSynth::reverb_event_int(int revid, int eventid, int val, _settings_params_t *params)
{
	int voice;

	switch (revid)
	{
	case _REVERB_EVENT:
		if (eventid == _REVERB_PRESET)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.reverb3m.preset",
				val,
				_EXEC_CALLBACK,
				-1); // -1: no program (common resource)
		}
		else if (eventid == _REVERB_ROOM_SIZE)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.reverb.room_size",
				val,
				_EXEC_CALLBACK,
				-1);
		}
		else if (eventid == _REVERB_DAMP)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.reverb.damp",
				val,
				_EXEC_CALLBACK,
				-1);
		}
		else if (eventid == _REVERB_WET)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.reverb.wet",
				val,
				_EXEC_CALLBACK,
				-1);
		}
		else if (eventid == _REVERB_DRY)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.reverb.dry",
				val,
				_EXEC_CALLBACK,
				-1);
		}
		else if (eventid == _REVERB_WIDTH)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.reverb.width",
				val,
				_EXEC_CALLBACK,
				-1);
		}
		else if (eventid == _REVERB_MODE)
		{
			adj_synth_settings_manager->set_int_param_value
					(params,
				"adjsynth.reverb.mode",
				val,
				_EXEC_CALLBACK,
				-1);
		}
		else {}

		break;
	}
	
	return 0;
}


/**
*   @brief  Initiates a reverbration effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE: true - enabled; false - disabled\n
*
*   @return 0
*/
int AdjSynth::reverb_event_bool(int revid, int eventid, bool val, _settings_params_t *params)
{
	//	int voice;
	switch (eventid)
	{
	case _REVERB_ENABLE:
		adj_synth_settings_manager->set_bool_param_value
					(params,
			"adjsynth.reverb.enable_state",
			val,
			_EXEC_CALLBACK,
			-1);
		
		// Only 1 reverb active
		if (val)
		{
			adj_synth_settings_manager->set_bool_param_value
					(params,
				"adjsynth.reverb3m.enable_state",
				false,
				_EXEC_CALLBACK,
				-1);	
		}
		break;

	case _REVERB3M_ENABLE:
		adj_synth_settings_manager->set_bool_param_value
					(params,
			"adjsynth.reverb3m.enable_state",
			val,
			_EXEC_CALLBACK,
			-1);
		break;
		// Only 1 reverb active
		if (val)
		{
			adj_synth_settings_manager->set_bool_param_value
					(params,
				"adjsynth.reverb.enable_state",
				false,
				_EXEC_CALLBACK,
				-1);	
		}
	}
	
	return 0;
}

