/**
*	@file		LibAPI_getMidiMixerParams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get active Midi Mixer settings parameters
*
*	History: 
*				version 1.2	15-Oct-2024	Rename patch to preset parameters.
*				version	1.1	9-Feb-2021	Code refactoring and notaion.
*				version 1.0	17-jan-2020	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int mod_synth_get_active_midi_mixer_setting_param(int paramid)
{
	int res_mixer;
	_settings_int_param_t int_param_mixer;
	char keyString[128];

	switch (paramid)
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

		sprintf(keyString, "adjsynth.mixer_channel_%i.level", paramid - _MIXER_CHAN_1_LEVEL + 1);
		res_mixer = settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_settings_params(),
			string(keyString),
			&int_param_mixer);
		if (res_mixer == _SETTINGS_KEY_FOUND)
		{
			return int_param_mixer.value;
		}
		else
		{
			return 0;
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

		sprintf(keyString, "adjsynth.mixer_channel_%i.pan", paramid - _MIXER_CHAN_1_PAN + 1);
		res_mixer = settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_settings_params(),
			string(keyString),
			&int_param_mixer);
		if (res_mixer == _SETTINGS_KEY_FOUND)
		{
			return int_param_mixer.value;
		}
		else
		{
			return 50;
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

		sprintf(keyString, "adjsynth.mixer_channel_%i.send", paramid - _MIXER_CHAN_1_SEND + 1);
		res_mixer = settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_settings_params(),
			string(keyString),
			&int_param_mixer);
		if (res_mixer == _SETTINGS_KEY_FOUND)
		{
			return int_param_mixer.value;
		}
		else
		{
			return 0;
		}

		break;
	}
	
	return 0;
}
