/**
*	@file		midiConversions.cpp
*	@author		Nahum Budin
*	@date		17-Aug-2024
*	@version	1.0
*
*	@brief		Convert MIDI events to bytes.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#include "midiConversions.h"
#include "../LibAPI/midi.h"

/**
 * Convert MIDI event(s) to a bytes-stream (vector).
 */
bool convert_midi_event_to_mssg(MidiFileEvent event, std::vector<uint8_t> *bytes, int vol)
{
	if (((event.metaevent != 0) && (event.meta_length != 0)) || (event.event_length != 0))
	{
		bytes->clear();
		
		if ((event.event_command == _MIDI_EVENT_NOTE_OFF) ||
			(event.event_command == _MIDI_EVENT_NOTE_ON))
		{
			if (event.event_length == 3)
			{
				bytes->push_back(event.event_command + event.channel);
				bytes->push_back(event.note_number);
				bytes->push_back((float)event.velocity * (float)vol / 100.0);
				return true;
			}
			else if (event.event_length == 2)
			{
				bytes->push_back(event.note_number);
				bytes->push_back((float)event.velocity * (float)vol / 100.0);
				return true;
			}
		}
		else if (event.event_command == _MIDI_EVENT_KEY_PRESSURE)
		{
			if (event.event_length == 3)
			{
				bytes->push_back(event.event_command + event.channel);
				bytes->push_back(event.note_number);
				bytes->push_back(event.key_pressure);
				return true;
			}
			else if (event.event_length == 2)
			{
				bytes->push_back(event.note_number);
				bytes->push_back(event.key_pressure);
				return true;
			}
		}
		else if (event.event_command == _MIDI_EVENT_CONTROL_CHANGE)
		{
			if (event.event_length == 3)
			{
				bytes->push_back(event.event_command + event.channel);
				bytes->push_back(event.control_num);
				bytes->push_back(event.control_value);
				return true;
			}
			else if (event.event_length == 2)
			{
				bytes->push_back(event.control_num);
				bytes->push_back(event.control_value);
				return true;
			}
		}
		else if (event.event_command == _MIDI_EVENT_PITCH_BEND)
		{
			if (event.event_length == 3)
			{
				bytes->push_back(event.event_command + event.channel);
				bytes->push_back((event.pitch_bend >> 8) & 0xFF);
				bytes->push_back(event.pitch_bend & 0xFF);
				return true;
			}
			else if (event.event_length == 2)
			{
				bytes->push_back((event.pitch_bend >> 8) & 0xFF);
				bytes->push_back(event.pitch_bend & 0xFF);
				return true;
			}
		}
		else if (event.event_command == _MIDI_EVENT_PROGRAM_CHANGE)
		{
			if (event.event_length == 2)
			{
				bytes->push_back(event.event_command + event.channel);
				bytes->push_back(event.instrument);
				return true;
			}
			else if (event.event_length == 1)
			{
				bytes->push_back(event.instrument);
				return true;
			}
		}
		else if (event.event_command == _MIDI_EVENT_CHANNEL_PRESSURE)
		{

			if (event.event_length == 2)
			{
				bytes->push_back(event.event_command + event.channel);
				bytes->push_back(event.chan_pressure);
				return true;
			}
			else if (event.event_length == 1)
			{
				bytes->push_back(event.chan_pressure);
				return true;
			}
		}
	}

	return false;
}