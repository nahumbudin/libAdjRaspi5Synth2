/**
*	@file		midiFileEvent.h
*	@author		Nahum Budin
*	@date		6-Aug-2024
*	@version	1.0
*
*	@brief		A MIDI file event.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#include "midiFileEvent.h"

MidiFileEvent::MidiFileEvent(){
	
	source = _MIDI_EVENT_SOURCE_DEFAULT;
	has_running_status = false;
	note_number_was_changed = false;
	channel_was_changed = false;
	originating_track_num = -1;
}

/**
*   @brief  Returns a copy of this event.
*   @param	MidiFileEvent a pointer to the cloned event object
*   @return void
*/
void MidiFileEvent::clone(MidiFileEvent *event)
{
	event->delta_time = delta_time;
	event->start_time = start_time;
	event->start_time_ms = start_time_ms;
	event->has_running_status = has_running_status;
	event->event_command = event_command;
	event->channel = channel;
	event->note_number = note_number;
	event->velocity = velocity;
	event->instrument = instrument;
	event->key_pressure = key_pressure;
	event->chan_pressure = chan_pressure;
	event->control_num = control_num;
	event->control_value = control_value;
	event->pitch_bend = pitch_bend;
	event->numerator = numerator;
	event->denominator = denominator;
	event->tempo = tempo;
	event->metaevent = metaevent;
	event->meta_length = meta_length;
	event->raw_values = raw_values;
	event->event_length = event_length;
	event->source = source;
	event->note_number_was_changed = note_number_was_changed;
	event->original_note_number = original_note_number;
	event->channel_was_changed = channel_was_changed;
	event->original_channel = channel;
	event->originating_track_num = originating_track_num;
}

/**
*   @brief  Compares two MidiEvents based on their start times. .
*   @param	event a and event b
*   @return if start times are equal
*				event a note number - evant b note number if the commands are equal
*				event a commans - event b command if comands are not equal
*			if start time are not equal
*				evant a start time - event b start time*			 
*/
int MidiFileEvent::compare(MidiFileEvent event_a, MidiFileEvent event_b)
{
	if (event_a.start_time == event_b.start_time)
	{
		if (event_a.event_command == event_b.event_command)
		{
			return event_a.note_number - event_b.note_number;
		}
		else
		{
			return event_a.event_command - event_b.event_command;
		}
	}
	else
	{
		return event_a.start_time - event_b.start_time;
	}
}