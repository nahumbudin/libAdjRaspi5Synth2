/**
 *	@file		adjSynthEventsHandlingFilter.cpp
 *	@author		Nahum Budin
 *	@date		28-Oct-2025
 *	@version	1.2
 *					1. Update callbacks are changed from block to only program
 *						_EXEC_BLOCK_CALLBACK -> _EXEC_CALLBACK.
 *
 *	@brief		AdjHeart Synthesizer Filters Events Handling
 *
 *	History:\n
 *
 *	version 1.1		11-Oct-2024: code refactoring and notaion.
 *	version 1.0		15_Nov-2019:
 *		First version
 *
 */

#include "adjSynth.h"

/**
*   @brief  Initiates a filter related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int filtid	target filter: _FILTER_1_EVENT, _FILTER_2_EVENT
*	@param	int eventid	specific event code:\n
*				_FILTER_FREQ, _FILTER_OCT, _FILTER_Q\n
*				_FILTER_KBD_TRACK, _FILTER_BAND\n
*				_FILTER_FREQ_MOD_LFO, _FILTER_FREQ_MOD_LFO_LEVEL\n
*				_FILTER_FREQ_MOD_ENV, _FILTER_FREQ_MOD_ENV_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_FILTER_FREQ: 0-100; 0->getFilterMinCenterFreq(); 100-> getFilterMaxCenterFreq()\n
*				_FILTER_OCT: 0-100; 0->0.f; 100->6.9999f\n
*				_FILTER_Q: 0-100: 0->getFilterMinQ();  100->getFilterMaxQ()\n
*				_FILTER_KBD_TRACK: 0-100\n				
*				_FILTER_BAND:\n
\verbatim
_FILTER_BAND_LPF, _FILTER_BAND_HPF, _FILTER_BAND_BPF, _FILTER_BAND_PASS_ALL\n
\endverbatim
*				FILTER_FREQ_MOD_LFO:\n
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
*				_FILTER_FREQ_MOD_LFO_LEVEL: 0-100\n
*				_FILTER_FREQ_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_FILTER_FREQ_MOD_ENV_LEVEL: 0-100\n
*	@param	int program	program number
*
*   @return 0
*/
int AdjSynth::filter_event(int filtid, int eventid, int val, _settings_params_t *params, int program)
{
	switch (eventid)
	{
	case _FILTER_FREQ:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.frequency",
				val,
				_EXEC_CALLBACK,
				program);	
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.frequency",
				val,
				_EXEC_CALLBACK,
				program);
		}
		break;

	case _FILTER_OCT:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.octave",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.octave",
				val,
				_EXEC_CALLBACK,
				program);
		}
		break;

	case _FILTER_Q:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.q",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.q",
				val,
				_EXEC_CALLBACK,
				program);
		}
		break;

	case _FILTER_BAND:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.band",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.band",
				val,
				_EXEC_CALLBACK,
				program);
		}
		break;

	case _FILTER_KBD_TRACK:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.keyboard_track",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.keyboard_track",
				val,
				_EXEC_CALLBACK,
				program);
		}

		break;	

	case _FILTER_FREQ_MOD_LFO:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.freq_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.freq_modulation_lfo_num",
				val,
				_EXEC_CALLBACK,
				program);
		}
		break;

	case _FILTER_FREQ_MOD_LFO_LEVEL:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.freq_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.freq_modulation_lfo_level",
				val,
				_EXEC_CALLBACK,
				program);
		}
		break;

	case _FILTER_FREQ_MOD_ENV:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.freq_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.freq_modulation_env_num",
				val,
				_EXEC_CALLBACK,
				program);
		}
		break;

	case _FILTER_FREQ_MOD_ENV_LEVEL:
		if (filtid == _FILTER_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter1.freq_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
		}
		else if (filtid == _FILTER_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.filter2.freq_modulation_env_level",
				val,
				_EXEC_CALLBACK,
				program);
		}
		
		break;
	}
	
	return 0;
}

