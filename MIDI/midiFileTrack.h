/**
*	@file		midiFileTrack.h
*	@author		Nahum Budin
*	@date		7-Aug-2024
*	@version	1.0
*
*	@brief		A MIDI file track.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#pragma once

#include <list>
#include <string>

#include "midiFileNote.h"
#include "midiFileEvent.h"

/* @class MidiFileTrack
 * 
 * The MidiFileTrack object takes as input the raw MidiFileEvents from the track, and gets:
 * - The list of MIDI notes in the track.
 * - The first instrument used in the track.
 *
 * For each NoteOn event in the MIDI file, a new MidiNote is created
 * and added to the track, using the add_note() method.
 *
 * The note_off() method is called when a NoteOff event is encountered,
 * in order to update the duration of the MidiFileNote objects.
 * 
 */

class MidiFileTrack
{
  public:
	MidiFileTrack(int trk_num);
	MidiFileTrack(std::vector<MidiFileEvent> events, int trk_num);
	//MidiFileTrack(std::list<MidiFileEvent> events, int trk_num);


	int get_track_num();
	
	std::vector<MidiFileNote> get_notes();
	std::vector<MidiFileEvent> get_lyrics();
	void set_lyrics(std::vector<MidiFileEvent> lyr);
	std::vector<MidiFileEvent> get_text();
	void set_text(std::vector<MidiFileEvent> txt);
	std::vector<MidiFileEvent> get_text_and_lyrics();
	void set_text_and_lyrics(std::vector<MidiFileEvent> txt_lyr);
	
	
	//std::list<MidiFileNote> get_notes();
	//std::list<MidiFileEvent> get_lyrics();
	//void set_lyrics(std::list<MidiFileEvent> lyr);
	//std::list<MidiFileEvent> get_text();
	//void set_text(std::list<MidiFileEvent> txt);
	//std::list<MidiFileEvent> get_text_and_lyrics();
	//void set_text_and_lyrics(std::list<MidiFileEvent> txt_lyr);

	int get_instrument();
	void set_instrument(int inst);

	void add_note(MidiFileNote note);
	void note_off_event(int chan, int note_num, int end_t);

	void add_lyric_event(MidiFileEvent lyr);
	void add_text_event(MidiFileEvent txt);
	void add_text_and_lyric_event(MidiFileEvent txt_lyr);

	void clone(MidiFileTrack *track);

	std::string track_string();
	/** The track number */
	int track_num;

  private:
	
	/** List of Midi notes */
	std::vector<MidiFileNote> notes;
	std::vector<MidiFileEvent> lyrics;
	/** The text in this track */
	std::vector<MidiFileEvent> text;
	/** The text and lyrics in this track */
	std::vector<MidiFileEvent> text_and_lyrics;
	
	/** List of Midi notes */
	//std::list<MidiFileNote> notes;
	//std::list<MidiFileEvent> lyrics;
	/** The text in this track */
	//std::list<MidiFileEvent> text;
	/** The text and lyrics in this track */
	//std::list<MidiFileEvent> text_and_lyrics;
	
	/** Instrument for this track */
	int instrument;
	/** The lyrics in this track (used for Kraoke) */
	
	
};
