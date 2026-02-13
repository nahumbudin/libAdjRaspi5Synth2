/**
 *	@file		adjSynthEventsHandlingNoise.cpp
 *	@author		Nahum Budin
 *	@date		11-Oct-2024
 *	@version	1.1
 *					1. Code refactoring and notaion.
 *					2. Update callbacks are changed from block to only program
 *						_EXEC_BLOCK_CALLBACK -> _EXEC_CALLBACK.
*	
*	@brief		AdjHeart Synthesizer Noise Generator Events Handling
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"


/**
*   @brief  Initiates a Noise generator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int noiseid	target noise generator: _NOISE_1_EVENT
*	@param	int eventid	specific event code:\n
*				_NOISE_COLOR, _NOISE_SEND_1, _NOISE_SEND_2\n
*				_NOISE_AMP_MOD_LFO, _NOISE_AMP_MOD_ENV
*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_NOISE_COLOR\n
\verbatim
	_WHITE_NOISE, _PINK_NOISE, _BROWN_NOISE
\endverbatim
*				_NOISE_SEND_1, _NOISE_SEND_2: 0-100\n
*				NOISE_AMP_MOD_LFO:\n
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
*				NOISE_AMP_MOD_LFO_LEVEL: 0-100\n
*				NOISE_AMP_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_NOISE_AMP_MOD_ENV_LEVEL: 0-100\n		
*
*	@param	int program	program number
*   @return 0
*/
int AdjSynth::noise_event_int(int noiseid, int eventid, int val, _settings_params_t *params, int program)
{
	switch (eventid)
	{
	case _NOISE_COLOR:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.noise.color",
			val,
			_EXEC_CALLBACK,
			program);	
			
		break;

	case _NOISE_SEND_1:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.noise.send_filter_1",
			val,
			_EXEC_CALLBACK,
			program);	
			
		break;

	case _NOISE_SEND_2:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.noise.send_filter_2",
			val,
			_EXEC_CALLBACK,
			program);
			
		break;

	case _NOISE_AMP_MOD_LFO:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.noise.amp_modulation_lfo_num",
			val,
			_EXEC_CALLBACK,
			program);
			
		break;

	case _NOISE_AMP_MOD_LFO_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.noise.amp_modulation_lfo_level",
			val,
			_EXEC_CALLBACK,
			program);
			
		break;

	case _NOISE_AMP_MOD_ENV:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.noise.amp_modulation_env_num",
			val,
			_EXEC_CALLBACK,
			program);
			
		break;

	case _NOISE_AMP_MOD_ENV_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.noise.amp_modulation_env_level",
			val,
			_EXEC_CALLBACK,
			program);
			
		break;			
	}
	
	return 0;
}

/**
*   @brief  Initiates a Noise generator related event with booleam value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int noiseid	target noise generator: _NOISE_1_EVENT
*	@param	int eventid	specific event code: _NOISE_ENABLE\n	
*
*	@param	bool val event parameter value  true or false (enable/disable)
*	@param	int program	program number
*   @return 0
*/
int AdjSynth::noise_event_bool(int noiseid, int eventid, bool val, _settings_params_t *params, int program)
{
	switch (eventid)
	{
	case _NOISE_ENABLE:
		if (val)
		{
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.noise.enabled",
				true,
				_EXEC_CALLBACK,
				program);
		}
		else
		{
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.noise.enabled",
				false,
				_EXEC_CALLBACK,
				program);
		}
		
		break;
	}
	
	return 0;
}
