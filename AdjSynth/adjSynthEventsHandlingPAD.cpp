/**
 *	@file		adjSynthEventsHandlingPAD.cpp
 *	@author		Nahum Budin
 *	@date		12-Oct-2025
 *	@version	1.3
 *					1. Update callbacks are changed from block to only program
 *						_EXEC_BLOCK_CALLBACK -> _EXEC_CALLBACK..
 *
 *	@brief		AdjHeart Synthesizer PAD Synthesizer Events Handling
 *
 *	History:\n
 *
 *		version 1.2 11-Oct-2024 Code refactoring and notaion.
*		version	1.1	5-Feb-2021	Code refactoring and notaion.
*		version 1.0	15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a PAD related event with integer value (affects all voices).
*			http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int padid	target PAD: _PAD_1_EVENT
*	@param	int eventid	specific event code:\n 
*				_PAD_DETUNE_OCTAVE, _PAD_DETUNE_SEMITONES, _PAD_DETUNE_CENTS\n 
*				_PAD_HARMONY_LEVEL_1, _PAD_HARMONY_LEVEL_2, _PAD_HARMONY_LEVEL_3\n
*				_PAD_HARMONY_LEVEL_4, _PAD_HARMONY_LEVEL_5, _PAD_HARMONY_LEVEL_6\n
*				_PAD_HARMONY_LEVEL_7, _PAD_HARMONY_LEVEL_8, _PAD_DETUNE\n
*				_PAD_FREQ_MOD_LFO, _PAD_FREQ_MOD_LFO_LEVEL\n
*				_PAD_AMP_MOD_LFO, _PAD_AMP_MOD_LFO_LEVEL\n
*				_PAD_QUALITY, _PAD_SHAPE\n
*				_PAD_BASE_NOTE, _PAD_BASE_WIDTH\n
*				_PAD_GENERATE, _PAD_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_PAD_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
*				_PAD_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
*				_PAD_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
*				_PAD_FILTER_SEND_1, _PAD_FILTER_SEND_2: 0-100\n
*				_PAD_FREQ_MOD_LFO, _PAD_AMP_MOD_LFO:\n
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
*				_PAD_FREQ_MOD_LFO_LEVEL, _PAD_AMP_MOD_LFO_LEVEL: 0-100\n
*				_PAD_FREQ_MOD_ENV, _PAD_AMP_MOD_ENV:\n
\verbatim
_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, _ENV_5
\endverbatim
*				_PAD_FREQ_MOD_ENV_LEVEL, _PAD_AMP_MOD_ENV_LEVEL: 0-100\n
*				_PAD_QUALITY:\n 
\verbatim
	_PAD_QUALITY_32K, _PAD_QUALITY_64K, _PAD_QUALITY_128K
	_PAD_QUALITY_256K, _PAD_QUALITY_512K, _PAD_QUALITY_1024K
\endverbatim
*				_PAD_SHAPE:\n
\verbatim
_PAD_SHAPE_RECTANGULAR, _PAD_SHAPE_GAUSSIAN, _PAD_SHAPE_DOUBLE_EXP
\endverbatim
*				_PAD_BASE_NOTE:\n
\verbatim
_PAD_BASE_NOTE_C2, _PAD_BASE_NOTE_G2, _PAD_BASE_NOTE_C3, _PAD_BASE_NOTE_G3
_PAD_BASE_NOTE_C4, _PAD_BASE_NOTE_G4, _PAD_BASE_NOTE_C5, _PAD_BASE_NOTE_G5
_PAD_BASE_NOTE_C6, _PAD_BASE_NOTE_G6
\endverbatim
*				_PAD_BASE_WIDTH: 0-100\n
*				_PAD_GENERATE: (any value)
*				_PAD_DETUNE: 0-100\n
*				_PAD_ENABLE: false, true
*	@param	_settings_params_t params	active settings params
*	@param int program	program number
*
*   @return 0
*/
int AdjSynth::pad_event_int(int padid, int eventid, int val, _settings_params_t *params, int program)
{
	int value = val;
	int result = -1;
		
	if (eventid == _PAD_DETUNE_OCTAVE)
	{
		val += _OSC_DETUNE_MIN_OCTAVE;
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.tune_offset_octave",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.tune_offset_octave");
	}
	else if (eventid == _PAD_DETUNE_SEMITONES)
	{
		val += _OSC_DETUNE_MIN_SEMITONES;
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.tune_offset_semitones",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.tune_offset_semitones");
	}
	else if (eventid == _PAD_DETUNE_CENTS)
	{
		val += _OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL; // 0.25 steps

		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.tune_offset_cent",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.tune_offset_cent");
	}
	else if (eventid == _PAD_FILTER_SEND_1)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.send_filter_1",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.send_filter_1");
	}
	else if (eventid == _PAD_FILTER_SEND_2)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.send_filter_2",
			val,
			_EXEC_BLOCK_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.send_filter_2");
	}
	else if (eventid == _PAD_FREQ_MOD_LFO)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.freq_modulation_lfo_num",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.freq_modulation_lfo_num");
	}
	else if (eventid == _PAD_FREQ_MOD_LFO_LEVEL)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.freq_modulation_lfo_level",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.freq_modulation_lfo_level");
	}
	else if (eventid == _PAD_FREQ_MOD_ENV)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.freq_modulation_env_num",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.freq_modulation_env_num");
	}
	else if (eventid == _PAD_FREQ_MOD_ENV_LEVEL)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.freq_modulation_env_level",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.freq_modulation_env_level");
	}
	else if (eventid == _PAD_AMP_MOD_LFO)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.amp_modulation_lfo_num",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.amp_modulation_lfo_num");
	}
	else if (eventid == _PAD_AMP_MOD_LFO_LEVEL)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.amp_modulation_lfo_level",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.amp_modulation_lfo_level");
	}
	else if (eventid == _PAD_AMP_MOD_ENV)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.amp_modulation_env_num",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.amp_modulation_env_num");
	}
	else if (eventid == _PAD_AMP_MOD_ENV_LEVEL)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.amp_modulation_env_level",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.amp_modulation_env_level");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_1)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_0",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_0");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_2)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_1",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_1");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_3)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_2",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_2");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_4)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_3",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_3");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_5)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_4",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_4");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_6)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_5",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_5");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_7)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_6",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_6");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_8)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_7",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_7");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_9)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_8",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_8");
	}
	else if (eventid == _PAD_HARMONY_LEVEL_10)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_level_9",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_level_9");
	}
	else if (eventid == _PAD_DETUNE)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.harmonies_detune",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.harmonies_detune");
	}
	else if (eventid == _PAD_QUALITY)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.quality",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.quality");
	}
	else if (eventid == _PAD_SHAPE)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.shape",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.shape");
	}
	else if (eventid == _PAD_BASE_NOTE)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.base_note",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.base_note");
	}
	else if (eventid == _PAD_BASE_WIDTH)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.base_width",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.base_width");
	}
	else if (eventid == _PAD_GENERATE)
	{
		//		AdjSynth::get_instance()->synthPADcreator->getprofile();
		synth_program[program]->synth_pad_creator->generate_wavetable(synth_program[program]->program_wavetable);

	}
	else if (eventid == _PAD_SHAPE_CUTOFF)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.pad_synth.shape_cutoff",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.shape_cutoff");
	}

	return result;
}


/**
*   @brief  Initiates a PAD related event with bool value (affects all voices).
*			http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int padid	target PAD: _PAD_1_EVENT
*	@param	int eventid	specific event code: _PAD_ENABLE\n 
*	@param	int val event parameter value true/fale:\n
*
*   @return 0
*/
int AdjSynth::pad_event_bool(int padid, int eventid, bool val, _settings_params_t *params, int program)
{
	int result = -1;
	
	if (eventid == _PAD_ENABLE)
	{
		adj_synth_settings_manager->set_bool_param_value
			(params,
			"adjsynth.pad_synth.enabled",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.pad_synth.enabled");
	}	
	
	return result;
}

