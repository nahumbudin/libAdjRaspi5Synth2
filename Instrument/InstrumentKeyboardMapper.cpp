/**
*	@file		instrumentKeyboardMapper.cpp
*	@author		Nahum Budin
*	@date		5-May-2026
*	@version	1.0
*					1. Initial version.
*					
*	@brief		MIDI keyboard mapper instrument.
*				It maps incoming MIDI events generated from the midi keyboard it connects to.
*				It can be used to split the keyboard into 2 zones, and assign each zone keys to a different MIDI channel,
*					regrdless of the keyboard assigned MIDI channel.
*				Each zone can be individually transposed by a specified number of octaves.
*
*	History:\n
*/

#include "instrumentKeyboardMapper.h"
#include "../LibAPI/synthesizer.h"

InstrumentKeyboardMapper *InstrumentKeyboardMapper::instrument_keyboard_mapper_instance = NULL;

InstrumentKeyboardMapper::InstrumentKeyboardMapper(AlsaMidiSysControl *alsa_control,
	std::string *alsa_client_in_name)
	: Instrument(_INSTRUMENT_NAME_KEYBOARD_MAPPER_STR_KEY, true, false, true, // midi in, no audio out, midi out
				 alsa_control, alsa_client_in_name, AdjSynth::get_instance(), NULL, _INSTRUMENT_TYPE_CONTROL)
{
	instrument_keyboard_mapper_instance = this;
	
	split_point_note = 60; // C4
	
	zone_1_low_note = 0;
	zone_1_high_note = split_point_note - 1; // B3 
	zone_1_midi_chan = 0;
	zone_1_transpose = 0;
	
	zone_2_low_note = split_point_note; // C4
	zone_2_high_note = 127;
	zone_2_midi_chan = 1;
	zone_2_transpose = 0;
	
	
	
	/* Keyboard mapper input handles all channels */
	this->alsa_midi_sequencer_events_handler->set_active_midi_channels(0xffff);
	
	this->alsa_midi_sequencer_events_handler->update_active_midi_channels_on_channel_change = true;
}

InstrumentKeyboardMapper::~InstrumentKeyboardMapper()
{
}

InstrumentKeyboardMapper *InstrumentKeyboardMapper::get_instrument_keyboard_mapper_instance()
{
	return instrument_keyboard_mapper_instance;
}

void InstrumentKeyboardMapper::set_split_point_note(int note)
{
	if ((note >= 12) && (note < 116)) // Ensure split point is within a reasonable range (C1 to B8)
	{
		split_point_note = note;
		
		zone_1_high_note = split_point_note - 1;
		zone_2_low_note = split_point_note;
	}
}

int InstrumentKeyboardMapper::get_split_point_note()
{
	return split_point_note;
}

void InstrumentKeyboardMapper::set_zone_1_midi_chan(int chan)
{
	if ((chan >= 0) && (chan < 16))
	{
		zone_1_midi_chan = chan;
	}
}

int InstrumentKeyboardMapper::get_zone_1_midi_chan()
{
	return zone_1_midi_chan;
}

void InstrumentKeyboardMapper::set_zone_1_transpose(int octave)
{
	if ((octave >= _OSC_DETUNE_MIN_OCTAVE) && (octave <= _OSC_DETUNE_MAX_OCTAVE))
	{
		zone_1_transpose = octave;
	}
}

int InstrumentKeyboardMapper::get_zone_1_transpose()
{
	return zone_1_transpose;
}

void InstrumentKeyboardMapper::set_zone_2_midi_chan(int chan)
{
	if ((chan >= 0) && (chan < 16))
	{
		zone_2_midi_chan = chan;
	}
}

int InstrumentKeyboardMapper::get_zone_2_midi_chan()
{
	return zone_2_midi_chan;
}

void InstrumentKeyboardMapper::set_zone_2_transpose(int octave)
{
	if ((octave >= _OSC_DETUNE_MIN_OCTAVE) && (octave <= _OSC_DETUNE_MAX_OCTAVE))
	{
		zone_2_transpose = octave;
	}
}

int InstrumentKeyboardMapper::get_zone_2_transpose()
{
	return zone_2_transpose;
}

void InstrumentKeyboardMapper::set_bypass_on(bool bypass)
{
	bypass_on = bypass;
}

bool InstrumentKeyboardMapper::get_bypass_on()
{
	return bypass_on;	
}


void InstrumentKeyboardMapper::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	uint8_t bytes[3];
	int status, new_channel, new_note;
	
	if (bypass_on)
	{
		new_channel = channel;
		new_note = note;
	}
	else if ((note >= zone_1_low_note) && (note <= zone_1_high_note))
	{
		// Zone 1
		new_channel = zone_1_midi_chan;
		new_note = note + zone_1_transpose * 12;
	}
	else if ((note >= zone_2_low_note) && (note <= zone_2_high_note))
	{
		// Zone 2
		new_channel = zone_2_midi_chan;
		new_note = note + zone_2_transpose * 12;
	}
	else
	{
		// Note is out of both zones - ignore it (do not send it to MIDI output)
		return;
	}
	
	bytes[0] = _MIDI_EVENT_NOTE_ON + (new_channel & 0x0f);
	bytes[1] = new_note;
	bytes[2] = velocity;

	std::lock_guard<std::mutex> lock(midiout_mutex);
	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper note on data to MIDI output: %s", snd_strerror(status));
	}
}
void InstrumentKeyboardMapper::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	uint8_t bytes[3];
	int status, new_channel, new_note;
	
	if (bypass_on)
	{
		new_channel = channel;
		new_note = note;
	}
	else if ((note >= zone_1_low_note) && (note <= zone_1_high_note))
	{
		// Zone 1
		new_channel = zone_1_midi_chan;
		new_note = note + zone_1_transpose * 12;
	}
	else if ((note >= zone_2_low_note) && (note <= zone_2_high_note))
	{
		// Zone 2
		new_channel = zone_2_midi_chan;
		new_note = note + zone_2_transpose * 12;
	}
	else
	{
		// Note is out of both zones - ignore it (do not send it to MIDI output)
		return;
	}
	
	bytes[0] = _MIDI_EVENT_NOTE_OFF + (new_channel & 0x0f);
	bytes[1] = new_note;
	bytes[2] = velocity;

	std::lock_guard<std::mutex> lock(midiout_mutex);
	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper note on data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentKeyboardMapper::change_program_handler(uint8_t channel, uint8_t program)
{
	uint8_t bytes[2];
	int status;

	// Just pass the program change command through, without any modification, since it is not related to the keyboard zones.
	bytes[0] = _MIDI_EVENT_PROGRAM_CHANGE + (channel & 0x0f);
	bytes[1] = program;

	if ((status = snd_rawmidi_write(midiout, bytes, 2)) < 0)
	{
		printf("Problem writing midi mapper change program data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentKeyboardMapper::channel_pressure_handler(uint8_t channel, uint8_t val)
{
	uint8_t bytes[2];
	int status;
	
	// Just pass the program change command through, without any modification, since it is not related to the keyboard zones.
	bytes[0] = _MIDI_EVENT_CHANNEL_PRESSURE + (channel & 0x0f);
	bytes[1] = val;

	if ((status = snd_rawmidi_write(midiout, bytes, 2)) < 0)
	{
		printf("Problem writing midi mapper change program data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentKeyboardMapper::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	uint8_t bytes[3];
	int status;
	int scaled_volume;

	// Just pass the program change command through, without any modification, since it is not related to the keyboard zones.
	bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (channel & 0x0f);
	bytes[1] = num;
	bytes[2] = val;

	std::lock_guard<std::mutex> lock(midiout_mutex);
	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper change control data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentKeyboardMapper::pitch_bend_handler(uint8_t channel, int pitch)
{
		
}

void InstrumentKeyboardMapper::sysex_handler(uint8_t *message, int len)
{
	
}

void InstrumentKeyboardMapper::control_box_events_handler(int event_id, uint16_t event_value)
{
	
}


