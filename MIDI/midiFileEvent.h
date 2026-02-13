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

#pragma once

#include <stdint.h>
#include <vector>

#define _MIDI_EVENT_SOURCE_DEFAULT 0

class MidiFileEvent
{
  public:
	MidiFileEvent();

	void clone(MidiFileEvent *event);
	int compare(MidiFileEvent event_a, MidiFileEvent event_b);

	/** NoteOn, NoteOff, etc.  Full list is in class MidiFile */
	uint8_t event_command;
	/** The absolute time this event occurs */
	int start_time;
	/** The channel this event occurs on */
	uint8_t channel;
	/** The note number  */
	uint8_t note_number;
	/** The volume of the note */
	uint8_t velocity;
	/** The instrument */
	uint8_t instrument;
	/** The metaevent, used if eventflag is MetaEvent */
	uint8_t metaevent;
	/** The tempo, for Tempo meta events */
	int tempo;
	/** The numerator, for TimeSignature meta events */
	uint8_t numerator;
	/** The denominator, for TimeSignature meta events */
	uint8_t denominator;
	/** The time between the previous event and this on */
	int delta_time;
	/** False if this is using the previous eventCommand (running status) */
	bool has_running_status;
	/** The total number of bytes in a none metaevent */
	uint16_t event_length;
	/** The key pressure */
	uint8_t key_pressure;
	/** The controller number */
	uint8_t control_num;
	/** The controller value */
	uint8_t control_value;
	/** The channel pressure */
	uint8_t chan_pressure;
	/** The metaevent length  */
	int meta_length;

	/** Originating track number */
	uint8_t originating_track_num;
	/** The pitch bend value */
	uint16_t pitch_bend;
	/** The raw byte value, for Sysex and meta events */
	std::vector<uint8_t> raw_values;
	

  private:
	
	/** Used for recording.
     *  The time in ms set at event creation time. Used for recording.
     *  To calculate time from start time subtract recording start time */
	long start_time_ms;
	/** Who created this event */
	int source;
	/** True when event-processing changed this event note-number */
	bool note_number_was_changed;
	/** Original value of not-number if event-processing changed it */
	uint8_t original_note_number;
	/** True when event-processing changed this event command */
	bool note_command_was_changed;
	/** Original value of the command if event-processing changed it */
	uint8_t original_command;
	/** Original value of the channel if event-processing changed it */
	uint8_t original_channel;
	/** True when event-processing changed this event channel */
	bool channel_was_changed;
	
	

};
