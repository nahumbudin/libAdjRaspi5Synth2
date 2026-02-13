/**
 *	@file		adjSynthDefaultPresetParamsFilter.cpp
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.3
 *					1. Rename patch to preset parameters.
 *					2. Adding default program settings callbacks
*	
*	@brief		Set default patch Filters parameters
*
*	History:\n
*	
*	version 1.2	4-Oct-2024 Code refactoring and notaion.
*	version 1.1	5-Feb-2021
*					1. Using sample-rate
*			1.0	15_Nov-2019	First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_preset_parameters_filter(_settings_params_t *params, int prog)
{
	int res;

	res = adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.frequency",
		(int)((1000.0f - _FILTER_MIN_CENTER_FREQ) / (float)(((float)sample_rate / 6.7) - _FILTER_MIN_CENTER_FREQ) * 100.0f), // max filter center frequency
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_frequency_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_frequency_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.octave",
		100 / 7, // ~1.0
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_octave_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_octave_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.q",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_q_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_q_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.keyboard_track",
		100,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_kbd_track_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_kbd_track_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.band",
		_FILTER_BAND_PASS_ALL,
		_FILTER_BAND_PASS_ALL,
		_FILTER_BAND_LPF,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_band_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_band_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.freq_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_freq_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_freq_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.freq_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_freq_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_freq_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.freq_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_freq_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_freq_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter1.freq_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_1_freq_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_1_freq_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.frequency",
		(int)((1000.0f - _FILTER_MIN_CENTER_FREQ) / (float)(((float)sample_rate / 6.7) - _FILTER_MIN_CENTER_FREQ) * 100.0f), // max filter center frequency
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_frequency_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_frequency_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.octave",
		100 / 7, // ~1.0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_octave_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_octave_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.q",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_q_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_q_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.keyboard_track",
		100,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_kbd_track_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_kbd_track_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.band",
		_FILTER_BAND_PASS_ALL,
		_FILTER_BAND_PASS_ALL,
		_FILTER_BAND_LPF,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_band_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_band_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.freq_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_freq_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_freq_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.freq_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_freq_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_freq_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.freq_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_freq_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.filter2.freq_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_filter_2_freq_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_filter_2_freq_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);
	
	return res;
}
