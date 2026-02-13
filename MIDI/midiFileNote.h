/**
*	@file		midiFileNote.h
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

#pragma once

/* @class MidiFileNote
 * A MidiNote contains
 *
 * start_time -  The time (measured in pulses) when the note is pressed.
 * channel    -  The channel the note is from.  This is used when matching
 *               NoteOff events with the corresponding NoteOn event.
 *               The channels for the NoteOn and NoteOff events must be
 *               the same.
 * note_number - The note number, from 0 to 127.  Middle C is 60.
 * duration  -   The time duration (measured in pulses) after which the
 *               note is released.
 *
 * A MidiFileNote is created when we encounter a NoteOn event. The duration
 * is initially unknown (set to 0).  When the corresponding NoteOff event
 * is found, the duration is set by the method note_off().
 */

class MidiFileNote
{
  public:
	/* Create a new MidiNote.
     * This is called when a NoteOn event is encountered in the MidiFile.
     */
	MidiFileNote(int strt_time, int chan, int note_numb, int dur);

	int get_start_time();
	void set_start_time(int time);

	int get_end_time();

	int get_channel();
	void set_channel(int ch);

	int get_note_number();
	void set_note_number(int nn);

	int get_duration();
	void set_duration(int dur);

	void note_off_event(int end_time);

	int compare(MidiFileNote note_a, MidiFileNote note_b);

	void clone(MidiFileNote *note);

  private:
	/** The start time, in pulses */
	int start_time;
	/** The channel */
	int channel;
	/** The note, from 0 to 127. Middle C is 60 */
	int note_number;
	/** The duration, in pulses */
	int duration;
};
