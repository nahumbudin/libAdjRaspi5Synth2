/**
*	@file		adjSynthEventsHandlingEquilizer.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		AdjHeart Synthesizer Band Equilizer Events Handling
*
*	History:\n
*	
*	version 1.1		5-Dec-2019
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a 10 bands equilizer related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int beqid	target equilizer: _BAND_EQUALIZER_EVENT
*	@param	int eventid	specific event code:\n
*				_BAND_EQUALIZER_BAND_31_LEVEL, _BAND_EQUALIZER_BAND_62_LEVEL\n
*				_BAND_EQUALIZER_BAND_125_LEVEL, _BAND_EQUALIZER_BAND_250_LEVEL\n
*				_BAND_EQUALIZER_BAND_500_LEVEL, _BAND_EQUALIZER_BAND_1K_LEVEL\n
*				_BAND_EQUALIZER_BAND_2K_LEVEL, _BAND_EQUALIZER_BAND_4K_LEVEL\n
*				_BAND_EQUALIZER_BAND_8K_LEVEL, _BAND_EQUALIZER_BAND_16K_LEVEL\n
*				_BAND_EQUILIZER_PRESET, _BAND_EQUALIZER_EVENT\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_BAND_EQUALIZER_BAND_xxx_LEVEL: 0-40; 0 -> -20db   40 -> +20db\n
*				_BAND_EQUILIZER_PRESET: not implemented\n
*				_BAND_EQUALIZER_EVENT: 1;
*	@param	int program	program number
*
*   @return 0
*/
int AdjSynth::band_equilizer_event(int beqid, int eventid, int val, _settings_params_t *params)
{
	
	if (beqid != _BAND_EQUALIZER_EVENT)
	{
		return -1;
	}
	
	switch (eventid)
	{
	case _BAND_EQUALIZER_BAND_31_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_31_level",
			val,
			_EXEC_CALLBACK,
			-1); // -1: no program (common resource)
			
		break;
			
	case _BAND_EQUALIZER_BAND_62_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_62_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_125_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_125_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_250_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_250_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_500_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_500_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_1K_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_1k_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_2K_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_2k_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_4K_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_4k_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_8K_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_8k_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;
			
	case _BAND_EQUALIZER_BAND_16K_LEVEL:
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_16k_level",
			val,
			_EXEC_CALLBACK,
			-1);
			
		break;

	case _BAND_EQUALIZER_SET_ALL_ZERO:
		for (int band = _band_31_hz; band <= _band_16000_hz; band++)
		{
			audio_equalizer->band_equalizer_L->set_band_level((iir_filter_bands)band, 0);
			audio_equalizer->band_equalizer_R->set_band_level((iir_filter_bands)band, 0);
		}

		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.equilizer.band_31_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_62_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_125_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_250_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_500_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_1k_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_2k_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_4k_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_8k_level",
			0,
			_EXEC_CALLBACK,
			-1);

		adj_synth_settings_manager->set_int_param_value
		(params,
			"adjsynth.equilizer.band_16k_level",
			0,
			_EXEC_CALLBACK,
			-1);


		update_ui_callback();

		break;
	}
	
	return 0;
}


int AdjSynth::band_equilizer_event_bool(int beqid, int eventid, bool val, _settings_params_t *params)
{
	return 0;
}
