/**
 * @file		HttpBridge_IntParamsDispatcher.cpp
 *	@author		Nahum Budin
 *	@date		14-June-2026
 *	@version	1.0
 *
 *	@brief		HTTP Server REST Api call for integer parameter dispatching.
 *
 */

#include "HttpBridge.h"
#include "../modSynth.h"
#include "../LibAPI/modSynthAPI.h"

#include "../LibAPI/libAdjRaspi5SynthAPI.h"

void HttpBridge::set_int_parm_dispatcher(int module_id, int sub_module_id, int param_id, int val)
{
	
	int res = 0;
	
	if (module_id == _MOD_SYNTH_EVENT)
	{
		switch (sub_module_id)
		{
		case _OSC_1_EVENT:
			// Oscillator 1
			res |= mod_synth_vco_event_int(_OSC_1_EVENT, param_id, val);
			
			// Trigger GUI update for OSC1 only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_OSC1);
			
			break;
			
		case _OSC_2_EVENT:
			// Oscillator 2
			res |= mod_synth_vco_event_int(_OSC_2_EVENT, param_id, val);
			
			// Trigger GUI update for OSC2 only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_OSC2);
			
			break;
			
		case _MSO_1_EVENT:
			// Morphed Sine Oscillator
			res |= mod_synth_mso_event_int(_MSO_1_EVENT, param_id, val);
			
			mod_synth_mso_event_int(_MSO_1_EVENT, _MSO_CALC_BASE_LUT, mod_synth_get_active_sketch());
			// Recalculates morphed waveform.
			mod_synth_mso_event_int(_MSO_1_EVENT, _MSO_CALC_MORPHED_LUT, mod_synth_get_active_sketch());
			
			// Trigger GUI update for MSO only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_MSO);
			
			break;
			
		case _NOISE_1_EVENT:
			// Noise Oscillator
			res |= mod_synth_noise_event_int(_NOISE_1_EVENT, param_id, val);
			
			// Trigger GUI update for Noise only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_NOISE);
			
			break;	
			
		case _KARPLUS_1_EVENT:
			// Karplus-Strong String Synthesizer
			res |= mod_synth_karplus_event_int(_KARPLUS_1_EVENT, param_id, val);
			
			// Trigger GUI update for Karplus-Strong String Synthesizer only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_KPS);
			
			break;
			
		case _PAD_1_EVENT:
			// Pad Synthesizer
			res |= mod_synth_pad_event_int(_PAD_1_EVENT, param_id, val);
			
			// Trigger GUI update for Pad Synthesizer only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_PAD);
			
			break;
			
		case _DISTORTION_1_EVENT:
			// Distortion Effect 1
			res |= mod_synth_distortion_event_int(_DISTORTION_1_EVENT, param_id, val);
			
			// Trigger GUI update for Distortion Effect 1 only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_DISTORTION);
			
			break;
			
		case _DISTORTION_2_EVENT:
			// Distortion Effect 2
			res |= mod_synth_distortion_event_int(_DISTORTION_2_EVENT, param_id, val);
			
			// Trigger GUI update for Distortion Effect 2 only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_DISTORTION);
			
			break;
			
		case _FILTER_1_EVENT:
			// Filter 1
			res |= mod_synth_filter_event_int(_FILTER_1_EVENT, param_id, val);
			
			// Trigger GUI update for Filter 1 only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_FILTERS);
			
			break;	
			
		case _FILTER_2_EVENT:
			// Filter 2
			res |= mod_synth_filter_event_int(_FILTER_2_EVENT, param_id, val);
			
			// Trigger GUI update for Filter 2 only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_FILTERS);
			
			break;
			
		case _LFO_1_EVENT:
			// LFO 1
			res |= mod_synth_modulator_event_int(_LFO_1_EVENT, param_id, val);
			
			// Trigger GUI update for LFOs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_LFOS);
			
			break;
			
		case _LFO_2_EVENT:
			// LFO 2
			res |= mod_synth_modulator_event_int(_LFO_2_EVENT, param_id, val);
			
			// Trigger GUI update for LFOs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_LFOS);
			
			break;
			
		case _LFO_3_EVENT:
			// LFO 3
			res |= mod_synth_modulator_event_int(_LFO_3_EVENT, param_id, val);
			
			// Trigger GUI update for LFOs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_LFOS);
			
			break;
			
		case _LFO_4_EVENT:
			// LFO 4
			res |= mod_synth_modulator_event_int(_LFO_4_EVENT, param_id, val);
			
			// Trigger GUI update for LFOs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_LFOS);
			
			break;
			
		case _LFO_5_EVENT:
			// LFO 5
			res |= mod_synth_modulator_event_int(_LFO_5_EVENT, param_id, val);
			
			// Trigger GUI update for LFOs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_LFOS);
			
			break;
			
		case _LFO_6_EVENT:
			// LFO 6
			res |= mod_synth_modulator_event_int(_LFO_6_EVENT, param_id, val);	
			
			// Trigger GUI update for LFOs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_LFOS);
			
			break;
			
		case _ENV_1_EVENT:
			// Envelope 1
			res |= mod_synth_modulator_event_int(_ENV_1_EVENT, param_id, val);
			
			// Trigger GUI update for ADSRs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_ADSRS);
			
			break;
			
		case _ENV_2_EVENT:
			// Envelope 2
			res |= mod_synth_modulator_event_int(_ENV_2_EVENT, param_id, val);
			
			// Trigger GUI update for ADSRs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_ADSRS);
			
			break;
			
		case _ENV_3_EVENT:
			// Envelope 3
			res |= mod_synth_modulator_event_int(_ENV_3_EVENT, param_id, val);
			
			// Trigger GUI update for ADSRs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_ADSRS);
			
			break;
			
		case _ENV_4_EVENT:
			// Envelope 4
			res |= mod_synth_modulator_event_int(_ENV_4_EVENT, param_id, val);
			
			// Trigger GUI update for ADSRs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_ADSRS);
			
			break;
			
		case _ENV_5_EVENT:
			// Envelope 5
			res |= mod_synth_modulator_event_int(_ENV_5_EVENT, param_id, val);
			
			// Trigger GUI update for ADSRs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_ADSRS);
			
			break;
			
		case _ENV_6_EVENT:
			// Envelope 6
			res |= mod_synth_modulator_event_int(_ENV_6_EVENT, param_id, val);
			
			// Trigger GUI update for ADSRs only
			mod_synth_trigger_analog_selective_update(ANALOG_UPDATE_ADSRS);
			
			break;
				
		default:
			fprintf(stderr, "Warning: Received set_int_parm_dispatcher call with unknown sub_module_id %d\n", sub_module_id);
			break;			
		}
	}
	else
	{
		// Will not be used currently for the Machine Learning Synthesizer model creation.
		switch (module_id)
		{
		case _FLUID_SYNTH_1_EVENT:
			// Fluid Synthesizer 1
			break;

		case _HAMMOND_ORGAN_EVENT:
			// Hammond Organ
			break;

		case _STRING_SYNTH_EVENT:
			// Karplus-Strong String Synthesizer
			break;

		case _MSO_SYNTH_EVENT:
			// MSO Synthesizer
			break;
		
		case _PAD_SYNTH_EVENT:
			// Pad Synthesizer
			break;

			//case _MODULE_SYNTH_PATCH: TODO:
			//	// Synth Patch Preset
			//	break;
		
		case _REVERB_EVENT:
			// Reverb Effect
			break;

		case _BAND_EQUALIZER_EVENT:
			// Graphic Equalizer Effect
			break;
		
		case _MIDI_MIXER_1_EVENT:
			// MIDI Mixer
			break;

		case _MIDI_PLAYER_EVENT:
			// MIDI Player
			break;

		case _MIDI_MAPPING_EVENT:
			// MIDI Mapper
			break;

			//case MODULE_MIDI_VIRTUAL_INPUT_EVENT: TODO:
			//	// MIDI Virtual Input
			//	break;

		case _KBD_1_EVENT:
			// Keyboard Control
			break;
		
		case _RECORDER_1_EVENT:
		// Recorder
			switch (param_id)
			{					
			case _START_RECORDING_EVENT:
				res |= ModSynth::get_instance()->jack_recorder->start_recording(
					ModSynth::get_instance()->audio_recorder->get_recording_mode(), 
					ModSynth::get_instance()->audio_recorder->get_remote_mp3_file_path(), 
					_DEFAULT_RECORDING_BITRATE, 
					_DEFAULT_RECORDING_SAMPLE_RATE);
					
				break;
						
			case _STOP_RECORDING_EVENT:
				res |= ModSynth::get_instance()->jack_recorder->stop_recording();
				break;
						
			default:
				fprintf(stderr, "Warning: Received set_int_parm_dispatcher call with unknown param_id %d for Recorder module\n", param_id);
				break;
				break;
			}
			
		default:
			fprintf(stderr, "Warning: Received set_int_parm_dispatcher call with unknown module_id %d\n", module_id);
			break;
		}
	}
}