/**
*	@file		adjSynthEventsHandlingAudio.cpp
*	@author		Nahum Budin
*	@date		11-Oct-2024
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		AdjHeart Synthesizer Audio Events Handling
*
*	History:\n
*	
*	version 1.0		19-Jan-2021:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief Initiates an audio related event with integer value.
*   All available parameters values are defined in LibAPI/synthesizer.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_MODE, _AUDIO_SAMPLE_RATE, _AUDIO_BLOCK_SIZE\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_MODE : _JACK_MODE_MANUAL, _JACK_MODE_AUTO\n
*		_AUDIO_JACK_SAMPLE_RATE : _JACK_SAMPLE_RATE_44, _JACK_SAMPLE_RATE_48\n
*		_AUDIO_JACK_BLOCK_SIZE : _JACK_BLOCK_SIZE_256, _JACK_BLOCK_SIZE_512, _JACK_BLOCK_SIZE_1024\n
*	@param  _setting_params_t *params: global parameters structure\n
*	@param int prog program num: n/a
*/

int AdjSynth::audio_event_int(int audid, int eventid, int val, _settings_params_t *params, int program)
{
	if (audid == _AUDIO_EVENT_1)
	{
		switch (eventid)
		{
		case _AUDIO_DRIVER:
			adj_synth_settings_manager->set_int_param_value(params,
				"adjsynth.audio.driver_type",
				val,
				_EXEC_CALLBACK,
				program);
			break;
				
		case _AUDIO_SAMPLE_RATE:
			adj_synth_settings_manager->set_int_param_value(params,
				"adjsynth.audio.sample_rate",
				val,
				_EXEC_CALLBACK,
				program);
			break;

		case _AUDIO_BLOCK_SIZE:
			adj_synth_settings_manager->set_int_param_value(params,
				"adjsynth.audio.block_size",
				val,
				_EXEC_CALLBACK,
				program);
			break;
				
		case _AUDIO_JACK_MODE:
			adj_synth_settings_manager->set_int_param_value(params,
				"adjsynth.audio_jack.mode",
				val,
				_EXEC_CALLBACK,
				program);
			break;				
		}
	}

	return 0;
}

/**
*   @brief Initiates an audio related event with boolean value.
*   All available parameters values are defined in defs.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_AUTO_START, _AUDIO_JACK_AUTO_CONNECT\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_AUTO_START : _JACK_AUTO_START_DIS, _JACK_AUTO_START_EN\n
*		_AUDIO_JACK_AUTO_CONNECTT : _JACK_AUTO_CONNECT_DIS, _JACK_AUTO_CONNECT_EN\n
*	@param  _setting_params_t *params: global parameters structure\n
*	@param int prog program num: n/a
*/
int AdjSynth::audio_event_bool(int audid, int eventid, bool val, _settings_params_t *params, int program)
{
	if (audid == _AUDIO_EVENT_1)
	{
		switch (eventid)
		{
		case _AUDIO_JACK_AUTO_START:
			adj_synth_settings_manager->set_bool_param_value(params,
				"adjsynth.audio_jack.auto_start_state",
				val,
				_EXEC_CALLBACK,
				program);
			break;
			
		case _AUDIO_JACK_AUTO_CONNECT:
			adj_synth_settings_manager->set_bool_param_value(params,
				"adjsynth.audio_jack.auto_connect_state",
				val,
				_EXEC_CALLBACK,
				program);
			break; 
		}
	}
	
	return 0;
}