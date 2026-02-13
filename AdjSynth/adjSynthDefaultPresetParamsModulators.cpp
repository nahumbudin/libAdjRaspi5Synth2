/**
*	@file		adjSynthDefaultPresetParamsModulators.cpp
*	@author		Nahum Budin
*	@date		24-Sep-2025
*	@version	1.1
*					1. Rename patch to preset parameters.
*					2. Adding default program settings callbacks
*	
*	@brief		Set default patch Modulators parameters
*
*	History:\n
*	
*	version	1.1		4-Oct-2024:
*	version 1.0		15-Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_preset_parameters_modulators(_settings_params_t *params, int prog)
{
	int res;

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_1.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_1_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_1_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_1.rate",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_1_rate_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_1_rate_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_1.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_1_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_1_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_2.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_2_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_2_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_2.rate",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_2_rate_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_2_rate_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_2.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_2_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_2_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_3.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_3_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_3_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_3.rate",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_3_rate_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_3_rate_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_3.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_3_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_3_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_4.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_4_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_4_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_4.rate",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_4_rate_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_4_rate_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_4.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_4_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_5_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_5.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_5_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_5_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_5.rate",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_5_rate_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_5_rate_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_5.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_5_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_5_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_6.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_6_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_6_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_6.rate",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_6_rate_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_6_rate_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.lfo_6.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_lfo_6_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_lfo_6_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_1.attack",
		5,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_env_1_attack_cb,
		0,
		num_of_voices - 1,
		set_voice_block_env_1_attack_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_1.decay",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_env_1_decay_cb,
		0,
		num_of_voices - 1,
		set_voice_block_env_1_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_1.sustain",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_env_1_sustain_cb,
		0,
		num_of_voices - 1,
		set_voice_block_env_1_sustain_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_1.release",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_env_1_release_cb,
		0,
		num_of_voices - 1,
		set_voice_block_env_1_release_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_2.attack",
		5,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_2_attack_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_2.decay",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_2_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_2.sustain",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_2_sustain_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_2.release",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_2_release_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_3.attack",
		5,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_3_attack_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_3.decay",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_3_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_3.sustain",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_3_sustain_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(
		params,
		"adjsynth.env_3.release",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_3_release_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_4.attack",
		5,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_4_attack_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_4.decay",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_4_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_4.sustain",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_4_sustain_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_4.release",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_4_release_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_5.attack",
		5,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_5_attack_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_5.decay",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_5_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_5.sustain",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_5_sustain_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_5.release",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_5_release_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_6.attack",
		5,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_6_attack_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_6.decay",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_6_decay_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_6.sustain",
		80,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_6_sustain_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.env_6.release",
		10,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		0,
		num_of_voices - 1,
		set_voice_block_env_6_release_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
		prog);
	
	return res;
}