/**
*	@file		adjSynthEventsHandlingMixer.cpp
*	@author		Nahum Budin
*	@date		18-Oct-2024
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		AdjHeart Synthesizer Mixer Events Handling
*
*	History:\n	
*			version 1.0		15-Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a midi-mixer related event with integer value 
*			Here we do not set any operational parameters and only update the settings structure.
*			Actual operational paramters are done in the instruments objects.
*   @param  int mixid	target amp-mixer: _MIDI_MIXER_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MIXER_CHAN_n_LEVLE, _MIXER_CHAN_n_PAN, _MIXER_CHAN_n_SEND (n: 1-16)\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				LEVEL, SEND: 0-100;\n
*				PAN: 0-100; 0 -> Left   50 -> Mid   100- -> Right\n*
*   @return 0
*/
int AdjSynth::midi_mixer_event(int mixid, int eventid, int val, _settings_params_t *params)
{
	char keyString[128];
	int i;

	switch (eventid)
	{	
	case _MIXER_CHAN_1_LEVEL:
	case _MIXER_CHAN_2_LEVEL:
	case _MIXER_CHAN_3_LEVEL:
	case _MIXER_CHAN_4_LEVEL:
	case _MIXER_CHAN_5_LEVEL:
	case _MIXER_CHAN_6_LEVEL:
	case _MIXER_CHAN_7_LEVEL:
	case _MIXER_CHAN_8_LEVEL:
	case _MIXER_CHAN_9_LEVEL:
	case _MIXER_CHAN_10_LEVEL:
	case _MIXER_CHAN_11_LEVEL:
	case _MIXER_CHAN_12_LEVEL:
	case _MIXER_CHAN_13_LEVEL:
	case _MIXER_CHAN_14_LEVEL:
	case _MIXER_CHAN_15_LEVEL:
	case _MIXER_CHAN_16_LEVEL:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.level", eventid - _MIXER_CHAN_1_LEVEL + 1);
		adj_synth_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			NULL, //_EXEC_CALLBACK,
			eventid - _MIXER_CHAN_1_LEVEL);

		break;

	case _MIXER_ALL_LEVEL:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.level", i + 1);
			adj_synth_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				NULL, //_EXEC_CALLBACK,
				i);
		}
		break;

	case _MIXER_CHAN_1_PAN:
	case _MIXER_CHAN_2_PAN:
	case _MIXER_CHAN_3_PAN:
	case _MIXER_CHAN_4_PAN:
	case _MIXER_CHAN_5_PAN:
	case _MIXER_CHAN_6_PAN:
	case _MIXER_CHAN_7_PAN:
	case _MIXER_CHAN_8_PAN:
	case _MIXER_CHAN_9_PAN:
	case _MIXER_CHAN_10_PAN:
	case _MIXER_CHAN_11_PAN:
	case _MIXER_CHAN_12_PAN:
	case _MIXER_CHAN_13_PAN:
	case _MIXER_CHAN_14_PAN:
	case _MIXER_CHAN_15_PAN:
	case _MIXER_CHAN_16_PAN:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan", eventid - _MIXER_CHAN_1_PAN + 1);
		adj_synth_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			NULL, //_EXEC_CALLBACK,
			eventid - _MIXER_CHAN_1_PAN);

		break;

	case _MIXER_ALL_PAN:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.pan", i + 1);
			adj_synth_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				NULL, //_EXEC_CALLBACK,
				i);
		}
		break;
		
	case _MIXER_CHAN_1_PAN_MOD_LFO:
	case _MIXER_CHAN_2_PAN_MOD_LFO:
	case _MIXER_CHAN_3_PAN_MOD_LFO:
	case _MIXER_CHAN_4_PAN_MOD_LFO:
	case _MIXER_CHAN_5_PAN_MOD_LFO:
	case _MIXER_CHAN_6_PAN_MOD_LFO:
	case _MIXER_CHAN_7_PAN_MOD_LFO:
	case _MIXER_CHAN_8_PAN_MOD_LFO:
	case _MIXER_CHAN_9_PAN_MOD_LFO:
	case _MIXER_CHAN_10_PAN_MOD_LFO:
	case _MIXER_CHAN_11_PAN_MOD_LFO:
	case _MIXER_CHAN_12_PAN_MOD_LFO:
	case _MIXER_CHAN_13_PAN_MOD_LFO:
	case _MIXER_CHAN_14_PAN_MOD_LFO:
	case _MIXER_CHAN_15_PAN_MOD_LFO:
	case _MIXER_CHAN_16_PAN_MOD_LFO:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo", eventid - _MIXER_CHAN_1_PAN_MOD_LFO + 1);
		adj_synth_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			NULL, //_EXEC_CALLBACK,
			eventid - _MIXER_CHAN_1_PAN);

		break;

	case _MIXER_ALL_PAN_MOD_LFO:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo", i + 1);
			adj_synth_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				NULL, //_EXEC_CALLBACK,
				i);
		}
		break;
		
	case _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_2_PAN_MOD_LFO_LEVEL:	
	case _MIXER_CHAN_3_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_4_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_5_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_6_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_7_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_8_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_9_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_10_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_11_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_12_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_13_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_14_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_15_PAN_MOD_LFO_LEVEL:
			case _MIXER_CHAN_16_PAN_MOD_LFO_LEVEL:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo_level", eventid - _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + 1);
		adj_synth_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			NULL, //_EXEC_CALLBACK,
			eventid - _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL);
		break;
		
	case _MIXER_ALL_PAN_MOD_LFO_LEVEL:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo_level", i + 1);
			adj_synth_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				NULL, //_EXEC_CALLBACK,
				i);
		}
		break;
						
	case _MIXER_CHAN_1_SEND:
	case _MIXER_CHAN_2_SEND:
	case _MIXER_CHAN_3_SEND:
	case _MIXER_CHAN_4_SEND:
	case _MIXER_CHAN_5_SEND:
	case _MIXER_CHAN_6_SEND:
	case _MIXER_CHAN_7_SEND:
	case _MIXER_CHAN_8_SEND:
	case _MIXER_CHAN_9_SEND:
	case _MIXER_CHAN_10_SEND:
	case _MIXER_CHAN_11_SEND:
	case _MIXER_CHAN_12_SEND:
	case _MIXER_CHAN_13_SEND:
	case _MIXER_CHAN_14_SEND:
	case _MIXER_CHAN_15_SEND:
	case _MIXER_CHAN_16_SEND:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.send", eventid - _MIXER_CHAN_1_SEND + 1);
		adj_synth_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			_EXEC_CALLBACK,
			eventid - _MIXER_CHAN_1_SEND);

		break;

	case _MIXER_ALL_SEND:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.send", i + 1);
			adj_synth_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				_EXEC_CALLBACK,
				i);
		}
		break;
	}
	
	return 0;	
}
