/**
 *	@file		adjSynthDefaultPresethParamsVCO.cpp
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.2
 *					1. Rename patch to preset parameters.
 *					2. Adding default program settings callbacks
*	
*	@brief		Set default patch VCOs parameters
*
*	History:\n
*	
*	version	1.1		4-Oct-2024
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Set the patch VCOs parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_preset_parameters_vco(_settings_params_t *params, int prog)
{
	int res = 0;
	
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);

	res = adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.osc1.enabled",
		true,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_enabled_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_enabled_cb,
		_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_BLOCK_START_INDEX | 
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK |_SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_pwm_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_pwm_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.send_filter_1",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_send_filter_1_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_send_filter_1_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.send_filter_2",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_send_filter_2_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_send_filter_2_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.tune_offset_oct",
		0,
		_OSC_DETUNE_MAX_OCTAVE,
		_OSC_DETUNE_MIN_OCTAVE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_tune_offset_oct_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_tune_offset_oct_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.tune_offset_semitones",
		0,
		_OSC_DETUNE_MAX_SEMITONES,
		_OSC_DETUNE_MIN_SEMITONES,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_tune_offset_semitones_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_tune_offset_semitones_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.tune_offset_cents",
		0,
		_OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_tune_offset_cents_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_tune_offset_cents_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.freq_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_freq_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_freq_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.freq_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_freq_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_freq_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.freq_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_freq_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_freq_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.freq_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_freq_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_freq_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.pwm_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_pwm_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_pwm_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.pwm_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_pwm_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_pwm_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.pwm_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_pwm_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_pwm_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.pwm_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_pwm_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_pwm_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.amp_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_amp_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_amp_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.amp_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_amp_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_amp_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.amp_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_amp_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_amp_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.amp_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_amp_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_amp_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_mode",
		_OSC_UNISON_MODE_12345678,
		_OSC_UNISON_MODE_Cm7_CHORD,
		_OSC_UNISON_MODE_12345678,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_mod_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_mod_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.hammond_percussion_mode",
		_HAMMOND_PERCUSION_MODE_OFF,
		_HAMMOND_PERCUSION_MODE_3RD_NORM_FAST,
		_HAMMOND_PERCUSION_MODE_OFF,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_hammond_percussion_mode_cb,
		0,
		num_of_voices - 1,
		NULL, // TODO:: set_voice_block_osc_1_hammond_percussion_mode, - Can't mutex block
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_1",
		100,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_1_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_1_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_2",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_2_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_2_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_3",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_3_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_3_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_4",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_4_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_4_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_5",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_5_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_5_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_6",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_6_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_6_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_7",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_7_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_7_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_8",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_8_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_8_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_level_9",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_level_9_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_level_9_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_distortion",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_distortion_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_distortion_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc1.unison_detune",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_detune_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_detune_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res = adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.osc1.unison_square_wave",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_1_unison_set_square_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_1_unison_set_square_cb,
		_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res = adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.osc2.enabled",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_enabled_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_enabled_cb,
		_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res = adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.waveform",
		_OSC_WAVEFORM_SINE,
		_OSC_WAVEFORM_SAMPHOLD,
		_OSC_WAVEFORM_SINE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_waveform_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_waveform_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.symmetry",
		50,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_pwm_symmetry_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_pwm_symmetry_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.send_filter_1",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_send_filter_1_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_send_filter_1_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.send_filter_2",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_send_filter_2_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_send_filter_2_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.tune_offset_oct",
		0,
		_OSC_DETUNE_MAX_OCTAVE,
		_OSC_DETUNE_MIN_OCTAVE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_tune_offset_oct_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_tune_offset_oct_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.tune_offset_semitones",
		0,
		_OSC_DETUNE_MAX_SEMITONES,
		_OSC_DETUNE_MIN_SEMITONES,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_tune_offset_semitones_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_tune_offset_semitones_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.tune_offset_cents",
		0,
		_OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_tune_offset_cents_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_tune_offset_cents_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.freq_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_freq_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_freq_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.freq_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_freq_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_freq_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.freq_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_freq_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_freq_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.freq_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_freq_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_freq_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.pwm_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_pwm_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_pwm_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.pwm_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_pwm_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_pwm_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.pwm_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_pwm_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_pwm_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.pwm_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_pwm_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_pwm_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.amp_modulation_lfo_num",
		_LFO_NONE,
		_LFO_6_DELAYED_2000MS,
		_LFO_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_amp_modulation_lfo_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_amp_modulation_lfo_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.amp_modulation_lfo_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_amp_modulation_lfo_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_amp_modulation_lfo_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.amp_modulation_env_num",
		_ENV_NONE,
		_ENV_6,
		_ENV_NONE,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_amp_modulation_env_num_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_amp_modulation_env_num_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_int_param(
		params,
		"adjsynth.osc2.amp_modulation_env_level",
		0,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_amp_modulation_env_level_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_amp_modulation_env_level_cb,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);

	res |= adj_synth_settings_manager->set_bool_param(
		params,
		"adjsynth.osc2.sync_on_osc_1",
		false,
		_ADJ_SYNTH_PRESET_PARAMS,
		set_program_osc_2_sync_on_osc1_cb,
		0,
		num_of_voices - 1,
		set_voice_block_osc_2_sync_on_osc_1_state_cb,
		_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX |
			_SET_BLOCK_STOP_INDEX | _SET_CALLBACK | _SET_BLOCK_CALLBACK,
		prog);
	
	return res;
}