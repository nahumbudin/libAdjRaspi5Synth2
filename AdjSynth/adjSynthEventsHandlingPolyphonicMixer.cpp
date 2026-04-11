/**
*	@file		adjSynthEventsHandlingPolyphonicMixer.cpp
*	@author		Nahum Budin
*	@date		2-Feb-2026
*	@version	1.0 1st release.
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a polyphonic-mixer related event with integer value
*   @param  int mixid	target amp-mixer: _POLYPHONIC_MIXER_EVENT
*	@param	int eventid	specific event code:\n
*				_MIXER_CHAN_n_LEVEL, _MIXER_CHAN_n_PAN, _MIXER_CHAN_n_SEND (n: 1-16)\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				LEVEL, SEND: 0-100;\n
*				PAN: 0-100; 0 -> Left   50 -> Mid   100- -> Right\n*
*   @return 0
*/

int AdjSynth::polyphonic_mixer_event(int polmixid, int eventid, int val, _settings_params_t *params, int program)
{
	int res = 0;
	int result = -1;
	
	if (polmixid != _POLYPHONIC_MIXER_EVENT)
	{
		return -1;
	}
	
	if ((eventid >= _MIXER_CHAN_1_LEVEL) && (eventid <= _MIXER_CHAN_16_LEVEL))
	{
		res = adj_synth_settings_manager->set_int_param_value
				(params,
			"adjsynth.poly_mixer.level",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.poly_mixer.level");
	}
	else if ((eventid >= _MIXER_CHAN_1_PAN) && (eventid <= _MIXER_CHAN_16_PAN))
	{
		res |= adj_synth_settings_manager->set_int_param_value
				(params,
			"adjsynth.poly_mixer.pan",
			val,
			_EXEC_CALLBACK,
			program);
		
		result |= update_program_voices_parameter(program, "adjsynth.poly_mixer.pan");
	}
	else if ((eventid >= _MIXER_CHAN_1_SEND) && (eventid <= _MIXER_CHAN_16_SEND))
	{
		res = adj_synth_settings_manager->set_int_param_value
				(params,
			"adjsynth.poly_mixer.send",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.poly_mixer.send");
	}
	else if ((eventid >= _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL) && (eventid <= _MIXER_CHAN_16_PAN_MOD_LFO_LEVEL))
	{
		res = adj_synth_settings_manager->set_int_param_value
				(params,
			"adjsynth.poly_mixer.pan_lfo_level",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.poly_mixer.pan_lfo_level");
	}
	else if ((eventid >= _MIXER_CHAN_1_PAN_MOD_LFO) && (eventid <= _MIXER_CHAN_16_PAN_MOD_LFO))
	{
			res = adj_synth_settings_manager->set_int_param_value
				(params,
			"adjsynth.poly_mixer.pan_lfo_num",
			val,
			_EXEC_CALLBACK,
			program);
		
		result = update_program_voices_parameter(program, "adjsynth.poly_mixer.pan_lfo_num");
	}
	else
	{
		return -1;
	}

	return result;
}
