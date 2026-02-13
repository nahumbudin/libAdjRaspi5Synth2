/**
 *	@file		adjSynthSetHammondPercussionMode.cpp
 *	@author		Nahum Budin
 *	@date		16-Jan-2026
 *	@version	1.4
 *					1. Replace block callbacks with program callbacks.
 *
 *	@brief		Set the OSC1 and OSC2 paramters according to the selected Hammond Organ
 *				percussion mode.
 *
 *	History:\n
 *
 *	version 1.3 5-Oct-2024
 *				1. Code refactoring and notaion.
 *	version	1.2	5-Feb-2021	Code refactoring and notaion.
 *	version 1.1	6-Dec-2019
 *	version 1.0	15_Nov-2019:
 *		First version
 *
 */

#include "adjSynth.h"

/**
*   @brief  Set OSC2 parametrs based on the Hammond-mode of OSC1
*			Note - it only changes the setting parameters.
*				   Re setting the AdjHeart must be executed to make the setup active.
*   @param	mod the Hammond-mode of OSC1
*   @param	params	a _setting_params_t parameters struct
*   @return void
*/
void AdjSynth::set_hammond_percusion_mode(int mod, _settings_params_t *params, int program)
{
	if (mod == _HAMMOND_PERCUSION_MODE_OFF)
	{
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.osc2.send_filter_1",
			0,
			_EXEC_CALLBACK, // _EXEC_BLOCK_CALLBACK
			program);
		
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.osc2.send_filter_2",
			0,
			_EXEC_CALLBACK, 
			program);
		
		adj_synth_settings_manager->set_int_param_value
			(params,
			"adjsynth.osc1.hammond_mode",
			_HAMMOND_PERCUSION_MODE_OFF,
			_EXEC_CALLBACK, 
			program);
		
		hammond_percussion_on = false;
	}
	else 
	{
		if ((mod > _HAMMOND_PERCUSION_MODE_OFF) && (mod <= _HAMMOND_PERCUSION_MODE_3RD_NORM_FAST))
		{
			hammond_percussion_on = true;
			
			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.osc2.enabled",
				true,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.waveform",
				_OSC_WAVEFORM_SINE,
				_EXEC_CALLBACK, 
				program);
		
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc1.hammond_mode",
				mod,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.amp_modulation_env_num",
				_ENV_2,
				_EXEC_CALLBACK);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.amp_modulation_env_level",
				params->int_parameters_map.at("adjsynth.osc1.amp_modulation_env_level").value,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.pwm_modulation_env_num",
				_ENV_NONE,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.freq_modulation_env_num",
				_ENV_NONE,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.amp_modulation_lfo_num",
				_LFO_NONE,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.pwm_mod_lfo_num",
				_LFO_NONE,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.freq_mod_lfo_num",
				_LFO_NONE,
				_EXEC_CALLBACK);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.detune_cents",
				0,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.waveform",
				_OSC_WAVEFORM_SINE,
				_EXEC_CALLBACK, 
				program);
			
			//			adj_synth_settings_manager->set_bool_param_value
			//				(params,
			//				"adjsynth.osc2.fix_tone_on",
			//				false,
			//				_EXEC_BLOCK_CALLBACK);

			adj_synth_settings_manager->set_bool_param_value
				(params,
				"adjsynth.osc2.sync_on_osc_1",
				false,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.osc2.symetry",
				50,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_2.attack",
				0,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_2.sustain",
				0,
				_EXEC_CALLBACK, 
				program);
			
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_2.release",
				0,
				_EXEC_CALLBACK, 
				program);

			if ((mod == _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW) ||
				(mod == _HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST) ||
				(mod == _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW) ||
				(mod == _HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST))
			{
				// Soft
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.send_filter_1",
					params->int_parameters_map.at("adjsynth.osc1.send_filter_1").value / 3,
					_EXEC_CALLBACK, 
					program);
				
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.send_filter_2",
					params->int_parameters_map.at("adjsynth.osc1.send_filter_2").value / 3,
					_EXEC_CALLBACK, 
					program);
				
				hammond_percussion_soft = true;
			}
			else if ((mod == _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW) ||
					 (mod == _HAMMOND_PERCUSION_MODE_2ND_NORM_FAST) ||
					 (mod == _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW) ||
					 (mod == _HAMMOND_PERCUSION_MODE_3RD_NORM_FAST))
			{
				// Norm
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.send_filter_1",
					params->int_parameters_map.at("adjsynth.osc1.send_filter_1").value,
					_EXEC_CALLBACK, 
					program);
				
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.send_filter_2",
					params->int_parameters_map.at("adjsynth.osc1.send_filter_2").value,
					_EXEC_CALLBACK, 
					program);
				
				hammond_percussion_soft = false;
			}

			if ((mod == _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW) ||
				(mod == _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW) ||
				(mod == _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW) ||
				(mod == _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW))
			{
				// Slow
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.env_2.decay",
					40,
					_EXEC_CALLBACK, 
					program);
			
				hammond_percussion_slow = true;
			}
			else if ((mod == _HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST) ||
					 (mod == _HAMMOND_PERCUSION_MODE_2ND_NORM_FAST) ||
					 (mod == _HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST) ||
					 (mod == _HAMMOND_PERCUSION_MODE_3RD_NORM_FAST))
			{
				// Fast
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.env_2.decay",
					10,
					_EXEC_CALLBACK, 
					program);
				
				hammond_percussion_slow = false;
			}

			if ((mod == _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW) ||
				(mod == _HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST) ||
				(mod == _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW) ||
				(mod == _HAMMOND_PERCUSION_MODE_2ND_NORM_FAST))
			{
				// 2nd
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.tune_offset_oct",
					1,
					_EXEC_CALLBACK, 
					program);
				
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.tune_offset_semitones",
					7,
					_EXEC_CALLBACK, 
					program);
				
				hammond_ercussion_3_rd = false;
			}
			else if ((mod == _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW) ||
					 (mod == _HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST) ||
					 (mod == _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW) ||
					 (mod == _HAMMOND_PERCUSION_MODE_3RD_NORM_FAST))
			{
				// 3rd
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.tune_offset_oct",
					2,
					_EXEC_CALLBACK, 
					program);
				
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.osc2.tune_offset_semitones",
					0,
					_EXEC_CALLBACK, 
					program);
				
				hammond_ercussion_3_rd = true;
			}
		}
	}
	
	//callback_update_ui();
}
