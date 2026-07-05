/**
 * @file		HttpBridge_FloatBoolStringDispatchers.cpp
 *	@author		Nahum Budin
 *	@date		14-June-2026
 *	@version	1.0
 *
 *	@brief		HTTP Server REST Api call for float, bool, and string parameter dispatching.
 *
 */

#include "HttpBridge.h"
#include "../modSynth.h"

#include "../LibAPI/libAdjRaspi5SynthAPI.h"


void HttpBridge::set_float_parm_dispatcher(int module_id, int sub_module_id, int param_id, float val)
{
	
}

void HttpBridge::set_bool_parm_dispatcher(int module_id, int sub_module_id, int param_id, bool val)
{
	int res = 0;
	
	if (module_id == _MOD_SYNTH_EVENT)
	{
		switch (sub_module_id)
		{
		case _OSC_1_EVENT:
			// Oscillator 1
			res |= mod_synth_vco_event_bool(_OSC_1_EVENT, param_id, val);
			break;
				
		case _OSC_2_EVENT:
			// Oscillator 2
			res |= mod_synth_vco_event_bool(_OSC_2_EVENT, param_id, val);
			break;
				
		case _MSO_1_EVENT:
			// Morphed Sine Oscillator
			res |= mod_synth_mso_event_bool(_MSO_1_EVENT, param_id, val);
			break;
				
		case _NOISE_1_EVENT:
			// Noise Oscillator
			res |= mod_synth_noise_event_bool(_NOISE_1_EVENT, param_id, val);
			break;	
				
		case _KARPLUS_1_EVENT:
			// Karplus-Strong String Synthesizer
			res |= mod_synth_karplus_event_bool(_KARPLUS_1_EVENT, param_id, val);
			break;
				
		case _PAD_1_EVENT:
			// Pad Synthesizer
			res |= mod_synth_pad_event_bool(_PAD_1_EVENT, param_id, val);
			break;
				
		case _DISTORTION_1_EVENT:
			// Distortion Effect 1
			res |= mod_synth_distortion_event_bool(_DISTORTION_1_EVENT, param_id, val);
			break;
				
		case _DISTORTION_2_EVENT:
			// Distortion Effect 2
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
			break;
			
		default:
			fprintf(stderr, "Warning: Received set_int_parm_dispatcher call with unknown module_id %d\n", module_id);
			break;
		}
	}	
}

void HttpBridge::set_string_parm_dispatcher(int module_id, int sub_module_id, int param_id, const std::string &val)
{
	if (module_id == _MOD_SYNTH_EVENT)
	{
	}
	else if (module_id == _RECORDER_1_EVENT)
	{	
		switch (param_id)
		{
		case _SET_RECORDING_FILE_PATH_EVENT:
			ModSynth::get_instance()->audio_recorder->set_remote_mp3_file_path(val);
			break;
		}
	}
			
}

