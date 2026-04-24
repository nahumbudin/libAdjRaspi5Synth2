/**
* @file		instrumentMidiMixer.cpp
*	@author		Nahum Budin
*	@date		6-Apr-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements a MIDI Mixer instrument.
*	
*	History:\n
*	
*/

#include "instrumentMidiMixer.h"

func_ptr_void_int_int_t InstrumentMidiMixer::midi_mixer_channel_volume_update_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiMixer::midi_mixer_channel_send_update_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiMixer::midi_mixer_channel_pan_update_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiMixer::midi_mixer_channel_pan_mod_lfo_update_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiMixer::midi_mixer_channel_pan_mod_lfo_level_update_callback_ptr = NULL;
InstrumentMidiMixer::InstrumentMidiMixer()
	: Instrument(_INSTRUMENT_NAME_MIDI_MIXER_STR_KEY, true, false, false) // MIDI input (for control messages?), no audio output, no MIDI output
{	
	active_preset = 0;
	
	//this->alsa_midi_sequencer_events_handler->set_active_midi_channels(0xffff); // All channels active by default
	//this->alsa_midi_sequencer_events_handler->update_active_midi_channels_on_channel_change = true;
	
	instrument_midi_mixer_instance = this;
	
	set_default_settings_parameters();

}
		
InstrumentMidiMixer::~InstrumentMidiMixer()
{
	
}

void InstrumentMidiMixer::update_midi_mixer_gui()
{
	int channel;
	int res = 0;
	_settings_int_param_t int_value;
	
	for (channel = 0; channel < 16; channel++)
	{
		res |= instrument_settings_manager->get_int_param(
							active_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".level",
			&int_value);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_volume_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings_manager->get_int_param(
							active_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".send",
			&int_value);	
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_send_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings_manager->get_int_param(
							active_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".pan",
			&int_value);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_pan_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings_manager->get_int_param(
							active_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".pan_mod_lfo",
			&int_value);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_pan_mod_lfo_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings_manager->get_int_param(
							active_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".pan_mod_lfo_level",
			&int_value);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_pan_mod_lfo_level_update_callback(channel, int_value.value);
		}
	}
}


void InstrumentMidiMixer::register_midi_mixer_channel_volume_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_mixer_channel_volume_update_callback_ptr = ptr;
}

void InstrumentMidiMixer::register_midi_mixer_channel_send_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_mixer_channel_send_update_callback_ptr = ptr;
}	

void InstrumentMidiMixer::register_midi_mixer_channel_pan_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_mixer_channel_pan_update_callback_ptr = ptr;
}

void InstrumentMidiMixer::register_midi_mixer_channel_pan_mod_lfo_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_mixer_channel_pan_mod_lfo_update_callback_ptr = ptr;
}

void InstrumentMidiMixer::register_midi_mixer_channel_pan_mod_lfo_level_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_mixer_channel_pan_mod_lfo_level_update_callback_ptr = ptr;
}

void InstrumentMidiMixer::midi_mixer_channel_volume_update_callback(int channel, int value)
{
	if (midi_mixer_channel_volume_update_callback_ptr != NULL)
	{
		midi_mixer_channel_volume_update_callback_ptr(channel, value);
	}
	
}

void InstrumentMidiMixer::midi_mixer_channel_send_update_callback(int channel, int value)
{
	if (midi_mixer_channel_send_update_callback_ptr != NULL)
	{
		midi_mixer_channel_send_update_callback_ptr(channel, value);
	}
}

void InstrumentMidiMixer::midi_mixer_channel_pan_update_callback(int channel, int value)
{
	if (midi_mixer_channel_pan_update_callback_ptr != NULL)
	{
		midi_mixer_channel_pan_update_callback_ptr(channel, value);
	}
}

void InstrumentMidiMixer::midi_mixer_channel_pan_mod_lfo_update_callback(int channel, int value)
{
	if (midi_mixer_channel_pan_mod_lfo_update_callback_ptr != NULL)
	{
		midi_mixer_channel_pan_mod_lfo_update_callback_ptr(channel, value);
	}
}

void InstrumentMidiMixer::midi_mixer_channel_pan_mod_lfo_level_update_callback(int channel, int value)
{
	if (midi_mixer_channel_pan_mod_lfo_level_update_callback_ptr != NULL)
	{
		midi_mixer_channel_pan_mod_lfo_level_update_callback_ptr(channel, value);
	}
}

// Handles ALSA MIDI sequencer controller events (e.g. CC messages)
//oid InstrumentMidiMixer::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
//
//	switch (num)
//	{
//		case _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2:
//			api_settings_events_handler(
//				_MIDI_MIXER_1_EVENT,
//				_MIXER_CHAN_1_LEVEL + channel,
//				val,
//				active_settings_params,
//				0); // no program.
//			break;
//		
//		case _MIDI_EVENT_CHANNEL_SEND_BYTE_2:
//			api_settings_events_handler(
//				_MIDI_MIXER_1_EVENT,
//				_MIXER_CHAN_1_SEND + channel,
//				val,
//				active_settings_params,
//				0); // no program.	
//			break;
//		
//		case _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2:
//			api_settings_events_handler(
//				_MIDI_MIXER_1_EVENT,
//				_MIXER_CHAN_1_PAN + channel,
//				val,
//				active_settings_params,
//				0); // no program.
//			break;
//		
//		case _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2:
//			api_settings_events_handler(
//				_MIDI_MIXER_1_EVENT,
//				_MIXER_CHAN_1_PAN_MOD_LFO + channel,
//				val,
//				active_settings_params,
//				0); // no program.
//			break;
//		
//		case _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2:
//			api_settings_events_handler(
//				_MIDI_MIXER_1_EVENT,
//				_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
//				val,
//				active_settings_params,
//				0); // no program.
//			break;
//		
//		default:
//			break;
//	}
//

int InstrumentMidiMixer::api_settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{
	
	if (moduleid != _MIDI_MIXER_1_EVENT)
	{
		return 0; // Not handled
	}
	
	char keyString[128];
	int i;

	switch (paramid)
	{	
	case _MIXER_CHAN_1_LEVEL:
	case _MIXER_CHAN_2_LEVEL:
	case _MIXER_CHAN_3_LEVEL:
	case _MIXER_CHAN_4_LEVEL:
	case _MIXER_CHAN_5_LEVEL:
	case _MIXER_CHAN_6_LEVEL:
	case _MIXER_CHAN_7_LEVEL:
	case _MIXER_CHAN_8_LEVEL:
	case _MIXER_CHAN_9_LEVEL:
	case _MIXER_CHAN_10_LEVEL:
	case _MIXER_CHAN_11_LEVEL:
	case _MIXER_CHAN_12_LEVEL:
	case _MIXER_CHAN_13_LEVEL:
	case _MIXER_CHAN_14_LEVEL:
	case _MIXER_CHAN_15_LEVEL:
	case _MIXER_CHAN_16_LEVEL:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.level", paramid - _MIXER_CHAN_1_LEVEL + 1);
		instrument_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			0, //_EXEC_CALLBACK,
			paramid - _MIXER_CHAN_1_LEVEL);

		break;

	case _MIXER_ALL_LEVEL:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.level", i + 1);
			instrument_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				0, //_EXEC_CALLBACK,
				i);
		}
		break;

	case _MIXER_CHAN_1_PAN:
	case _MIXER_CHAN_2_PAN:
	case _MIXER_CHAN_3_PAN:
	case _MIXER_CHAN_4_PAN:
	case _MIXER_CHAN_5_PAN:
	case _MIXER_CHAN_6_PAN:
	case _MIXER_CHAN_7_PAN:
	case _MIXER_CHAN_8_PAN:
	case _MIXER_CHAN_9_PAN:
	case _MIXER_CHAN_10_PAN:
	case _MIXER_CHAN_11_PAN:
	case _MIXER_CHAN_12_PAN:
	case _MIXER_CHAN_13_PAN:
	case _MIXER_CHAN_14_PAN:
	case _MIXER_CHAN_15_PAN:
	case _MIXER_CHAN_16_PAN:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan", paramid - _MIXER_CHAN_1_PAN + 1);
		instrument_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			0, //_EXEC_CALLBACK,
			paramid - _MIXER_CHAN_1_PAN);

		break;

	case _MIXER_ALL_PAN:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.pan", i + 1);
			instrument_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				0, //_EXEC_CALLBACK,
				i);
			
		}
		break;
		
	case _MIXER_CHAN_1_PAN_MOD_LFO:
	case _MIXER_CHAN_2_PAN_MOD_LFO:
	case _MIXER_CHAN_3_PAN_MOD_LFO:
	case _MIXER_CHAN_4_PAN_MOD_LFO:
	case _MIXER_CHAN_5_PAN_MOD_LFO:
	case _MIXER_CHAN_6_PAN_MOD_LFO:
	case _MIXER_CHAN_7_PAN_MOD_LFO:
	case _MIXER_CHAN_8_PAN_MOD_LFO:
	case _MIXER_CHAN_9_PAN_MOD_LFO:
	case _MIXER_CHAN_10_PAN_MOD_LFO:
	case _MIXER_CHAN_11_PAN_MOD_LFO:
	case _MIXER_CHAN_12_PAN_MOD_LFO:
	case _MIXER_CHAN_13_PAN_MOD_LFO:
	case _MIXER_CHAN_14_PAN_MOD_LFO:
	case _MIXER_CHAN_15_PAN_MOD_LFO:
	case _MIXER_CHAN_16_PAN_MOD_LFO:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo", paramid - _MIXER_CHAN_1_PAN_MOD_LFO + 1);
		instrument_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			0, //_EXEC_CALLBACK,
			paramid - _MIXER_CHAN_1_PAN);

		break;

	case _MIXER_ALL_PAN_MOD_LFO:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo", i + 1);
			instrument_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				0, //_EXEC_CALLBACK,
				i);
			
		}
		break;
		
	case _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_2_PAN_MOD_LFO_LEVEL:	
	case _MIXER_CHAN_3_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_4_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_5_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_6_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_7_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_8_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_9_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_10_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_11_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_12_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_13_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_14_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_15_PAN_MOD_LFO_LEVEL:
	case _MIXER_CHAN_16_PAN_MOD_LFO_LEVEL:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo_level", paramid - _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + 1);
		instrument_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			0, //_EXEC_CALLBACK,
			paramid - _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL);
		break;
		
	case _MIXER_ALL_PAN_MOD_LFO_LEVEL:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo_level", i + 1);
			instrument_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				0, //_EXEC_CALLBACK,
				i);
		}
		break;
						
	case _MIXER_CHAN_1_SEND:
	case _MIXER_CHAN_2_SEND:
	case _MIXER_CHAN_3_SEND:
	case _MIXER_CHAN_4_SEND:
	case _MIXER_CHAN_5_SEND:
	case _MIXER_CHAN_6_SEND:
	case _MIXER_CHAN_7_SEND:
	case _MIXER_CHAN_8_SEND:
	case _MIXER_CHAN_9_SEND:
	case _MIXER_CHAN_10_SEND:
	case _MIXER_CHAN_11_SEND:
	case _MIXER_CHAN_12_SEND:
	case _MIXER_CHAN_13_SEND:
	case _MIXER_CHAN_14_SEND:
	case _MIXER_CHAN_15_SEND:
	case _MIXER_CHAN_16_SEND:
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.send", paramid - _MIXER_CHAN_1_SEND + 1);
		instrument_settings_manager->set_int_param_value
			(params,
			string(keyString),
			val,
			0, //_EXEC_CALLBACK,
			paramid - _MIXER_CHAN_1_SEND);
		
		break;

	case _MIXER_ALL_SEND:
		for (i = 0; i < 16; i++)
		{
			sprintf(keyString, "adjsynth.mixer_channel_%i.send", i + 1);
			instrument_settings_manager->set_int_param_value(params,
				string(keyString),
				val,
				0, //_EXEC_CALLBACK,
				i);
		}
		break;
	}
	
	return 0;
}

int InstrumentMidiMixer::api_settings_events_handler(int moduleid, int paramid, float val, _settings_params_t *params, int program)
{
	
	return 0;
}

int InstrumentMidiMixer::api_settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	
	return 0;
}

int InstrumentMidiMixer::api_settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{
	
	return 0;
}

