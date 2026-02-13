/**
*	@file		modSynthDefaultSettingsParamsAudio.cpp
*	@author		Nahum Budin
*	@date		23-Sep-2025
*	@version	1.2
*					1. Refactoring rename patches to presets parameters.
*	
*	@brief		Set default audio settings parameters
*
*	History:\n
*		version 1.1		13-Oct-2024	
*		version	1.0		18-Jan-2021
*	
*/

#include "modSynth.h"

int ModSynth::set_default_settings_parameters_audio(_settings_params_t *params)
{
	int res;

	res = general_settings_manager->set_int_param(
		params,
		"adjsynth.audio.driver_type",
		_AUDIO_JACK,
		_AUDIO_ALSA,
		_AUDIO_JACK,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_audio_driver_type_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= general_settings_manager->set_int_param(
		params,
		"adjsynth.audio.block_size",
		_AUDIO_BLOCK_SIZE_512,
		_AUDIO_BLOCK_SIZE_1024,
		_AUDIO_BLOCK_SIZE_256,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_audio_block_size_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= general_settings_manager->set_int_param(
		params,
		"adjsynth.audio.sample_rate",
		_SAMPLE_RATE_44,
		_SAMPLE_RATE_48,
		_SAMPLE_RATE_44,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_audio_sample_rate_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= general_settings_manager->set_bool_param(
		params,
		"adjsynth.audio_jack.auto_start_state",
		_JACK_AUTO_START_EN,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_audio_jack_auto_start_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		-1);

	res |= general_settings_manager->set_int_param(
		params,
		"adjsynth.audio_jack.mode",
		_DEFAULT_JACK_MODE,
		_JACK_MODE_SERVER_CONTROL,
		_JACK_MODE_APP_CONTROL,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_audio_jack_mode_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1); // Global, no program specific

	res |= general_settings_manager->set_bool_param(
		params,
		"adjsynth.audio_jack.auto_connect_state",
		_JACK_AUTO_CONNECT_AUDIO_EN,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_audio_jack_auto_connect_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		-1);
	return res;
}

