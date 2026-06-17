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
#include "../LibAPI/synthesizer.h"

void HttpBridge::set_int_parm_dispatcher(int module_id, int sub_module_id, int param_id, int val)
{
	switch (module_id)
	{
	case _MOD_SYNTH_EVENT:
		// Analog Synthesizer

		break;
		
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