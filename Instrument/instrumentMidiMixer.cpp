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
	: Instrument(_INSTRUMENT_NAME_MIDI_MIXER_STR_KEY, false, false, false) // No MIDI input, no audio output, no MIDI output
{	
	active_preset = 0;
	
	
	
	instrument_midi_mixer_instance = this;

	
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
		res |= instrument_settings->get_int_param(
							active_preset_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".level",
			&int_value);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_volume_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings->get_int_param(
							active_preset_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".send",
			&int_value);	
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_send_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings->get_int_param(
							active_preset_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".pan",
			&int_value);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_pan_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings->get_int_param(
							active_preset_settings_params,
			string("adjsynth.mixer_channel_") + to_string(channel + 1) + ".pan_mod_lfo",
			&int_value);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			midi_mixer_channel_pan_mod_lfo_update_callback(channel, int_value.value);
		}
		
		res |= instrument_settings->get_int_param(
							active_preset_settings_params,
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
