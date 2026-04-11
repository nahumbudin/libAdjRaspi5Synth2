/**
 *	@file		adjSynthEventsHandlingKPS.cpp
 *	@author		Nahum Budin
 *	@date		11-Oct-2024
 *	@version	1.1
 *					1. Code refactoring and notaion.
 *					2. Update callbacks are changed from block to only program
 *						_EXEC_BLOCK_CALLBACK -> _EXEC_CALLBACK.
*					
*	@brief		AdjHeart Synthesizer Kurplus Strong String Generator Events Handling
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a Karplus-Strong-String generator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int karpid	target generator: _KARPLUS_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM\n
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE\n
*				_KARPLUS_STRONG_STRING_DAMPING, _KARPLUS_STRONG_STRING_DAMPING_VARIATION\n
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM:\n
\verbatim
	_KARPLUS_STRONG_EXCITATION_WHITE_NOISE, _KARPLUS_STRONG_EXCITATION_PINK_NOISE
	_KARPLUS_STRONG_EXCITATION_BROWN_NOISE, _KARPLUS_STRONG_EXCITATION_SINECHIRP
	_KARPLUS_STRONG_EXCITATION_DECAYEDSINE, _KARPLUS_STRONG_EXCITATION_SAWTOOTH_WAVE
	_KARPLUS_STRONG_EXCITATION_SQUARE_WAVE\n
\endverbatim
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE:\n
\verbatim
	_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT, _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING: 0-100;\n 
\verbatim
	0->getKarplusStrongMinStringDumping() 
	100 -> getKarplusStrongMaxStringDumping()
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING_VARIATION: 0-100\n
\verbatim
	0 -> getKarplusStrongMinStringDumpingVariations() 
	100 -> getKarplusStrongMaxStringDumpingVariations()
\endverbatim
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY: 0-100:\n 
\verbatim
	0 -> getKarplusStrongMinDecay()  
	100 -> getKarplusStrongMaxDecay()
\endverbatim
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS: 0-100\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2: 0-100\n
*	
*	@parm _settings_params_t *params active parameters
*	@param	int program	program number
*
*   @return 0
*/
int AdjSynth::karplus_event_int(int karlplusid, int eventid, int val, _settings_params_t *params, int program)
{
	int result = -1;
	
	switch (karlplusid)
	{
	case _KARPLUS_1_EVENT:
		if (eventid == _KARPLUS_STRONG_EXCITATION_WAVEFORM)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.excitation_waveform_type",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.excitation_waveform_type");
		}
		else if (eventid == _KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.excitation_waveform_variations",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.excitation_waveform_variations");
		}
		else if (eventid == _KARPLUS_STRONG_STRING_DUMP_CALC_MODE)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.string_damping_calculation_mode",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.string_damping_calculation_mode");
		}
		else if (eventid == _KARPLUS_STRONG_STRING_DAMPING)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.string_damping",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.string_damping");
		}
		else if (eventid == _KARPLUS_STRONG_STRING_DAMPING_VARIATION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.string_damping_variations",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.string_damping_variations");
		}
		else if (eventid == _KARPLUS_STRONG_PLUCK_DAMPING)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.pluck_damping",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.pluck_damping");
		}
		else if (eventid == _KARPLUS_STRONG_PLUCK_DAMPING_VARIATION)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.pluck_damping_variations",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.pluck_damping_variations");
		}
		else if (eventid == _KARPLUS_STRONG_ON_DECAY)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.on_decay",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.on_decay");
		}
		else if (eventid == _KARPLUS_STRONG_OFF_DECAY)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.off_decay",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.off_decay");
		}
		else if (eventid == _KARPLUS_STRONG_SEND_1)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.send_filter_1",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.send_filter_1");
		}
		else if (eventid == _KARPLUS_STRONG_SEND_2)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.karplus_synth.send_filter_2",
				val,
				_EXEC_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.send_filter_2");
		}
		break;
	}
	
	return result;
}



/**
*   @brief  Initiates a Karplus-Strong-String generator related event with boolean value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int karpid	target generator: _KARPLUS_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KARPLUS_STRONG_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				true, false\n
*				
*	@parm _settings_params_t *params active parameters
*	@param	int program	program number
*				
*   @return 0
*/
int AdjSynth::karplus_event_bool(int karplusid, int eventid, bool val, _settings_params_t *params, int program)
{
	int result = -1;
	
	switch (karplusid)
	{
	case _KARPLUS_1_EVENT:
		if (eventid == _KARPLUS_STRONG_ENABLE)
		{
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.karplus_synth.enabled",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
			
			result = update_program_voices_parameter(program, "adjsynth.karplus_synth.enabled");
		}
		break;
	}
	
	return result;
}


