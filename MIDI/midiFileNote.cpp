/**
*	@file		midiFileNote.cpp
*	@author		Nahum Budin
*	@date		7-Aug-2024
*	@version	1.0
*
*	@brief		A MidiNote is created when we encounter a NoteOff event.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#include "midiFileNote.h"

/* Create a new MidiNote.
     * This is called when a NoteOn event is encountered in the MidiFile.
     */
MidiFileNote::MidiFileNote(int strt_time, int chan, int note_numb, int dur)
{
	start_time = strt_time;
	channel = chan;
	note_number = note_numb;
	duration = dur;
}

int MidiFileNote::get_start_time()
{
	return start_time;
}

void MidiFileNote::set_start_time(int time)
{
	start_time = time;
}

int MidiFileNote::get_end_time()
{
	return start_time + duration;
}

int MidiFileNote::get_channel()
{
	return channel;
}
void MidiFileNote::set_channel(int ch)
{
	channel = ch;
}

int MidiFileNote::get_note_number()
{
	return note_number;
}

void MidiFileNote::set_note_number(int nn)
{
	note_number = nn;
}

int MidiFileNote::get_duration()
{
	return duration;
}

void MidiFileNote::set_duration(int dur)
{
	duration = dur;
}

/* Used to indicate that a Note off event occurs for this note at the given time.
 * Calculate the note duration based on the noteoff event.
 */
void MidiFileNote::note_off_event(int end_time)
{
	duration = end_time - start_time;
}

/** Compares two MidiFileNotes objects based on their start times.
 *  If the start times are equal, compare by their numbers - return the diference.
 *  Else return the diffence between the start times.
 */
int MidiFileNote::compare(MidiFileNote note_a, MidiFileNote note_b)
{
	if (note_a.start_time == note_b.start_time)
	{
		return note_a.note_number - note_b.note_number;
	}
	else
	{
		return note_a.start_time - note_b.start_time;
	}
}

/**
*   @brief  Returns a copy of this note.
*   @param	MidiFileNote a pointer to the cloned note object
*   @return void
*/
void MidiFileNote::clone(MidiFileNote *note)
{
	note = new MidiFileNote(start_time, channel, note_number, duration);
}
