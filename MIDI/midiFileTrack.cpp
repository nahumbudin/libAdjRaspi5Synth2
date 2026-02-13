/**
*	@file		midiFileTrack.cpp
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

#include "midiFile.h"
#include "midiFileTrack.h"
#include "../LibAPI/midi.h"

/** Creates an empty midi file track. */
MidiFileTrack::MidiFileTrack(int trk_num)
{
	track_num = trk_num;
	instrument = 0;
}

/** Creates a MidiTrack based on a MIDI events list.
 *  Extract the NoteOn/NoteOff events to gather the list of MidiNotes.
 *  Also extract the Lyrics and Text events to gather the lyrics/text only tracks
 */
//MidiFileTrack::MidiFileTrack(std::list<MidiFileEvent> events, int trk_num)
MidiFileTrack::MidiFileTrack(std::vector<MidiFileEvent> events, int trk_num)
{
	track_num = trk_num;
	instrument = 0;

	for (MidiFileEvent mevent : events)
	{
		if (mevent.event_command == _MIDI_EVENT_NOTE_ON && mevent.velocity > 0)
		{
			MidiFileNote *note = new MidiFileNote(mevent.start_time, mevent.channel, mevent.note_number, 0);
			add_note(*note);
		}
		else if (mevent.event_command == _MIDI_EVENT_NOTE_ON && mevent.velocity == 0)
		{
			note_off_event(mevent.channel, mevent.note_number, mevent.start_time);
		}
		else if (mevent.event_command == _MIDI_EVENT_NOTE_OFF)
		{

			note_off_event(mevent.channel, mevent.note_number, mevent.start_time);
		}
		else if (mevent.event_command == _MIDI_EVENT_PROGRAM_CHANGE)
		{

			instrument = mevent.instrument;
		}
		else if (mevent.metaevent == _MIDI_META_EVENT_LYRIC)
		{
			lyrics.push_back(mevent);
			text_and_lyrics.push_back(mevent);
		}
		else if (mevent.metaevent == _MIDI_META_EVENT_TEXT)
		{
			text.push_back(mevent);
			text_and_lyrics.push_back(mevent);
		}
		/* Add Tempo events to lyrics and text to affect lyrics and text playing tempo */
		else if (mevent.metaevent == _MIDI_META_EVENT_TEMPO)
		{
			lyrics.push_back(mevent);
			lyrics.push_back(mevent);
			text_and_lyrics.push_back(mevent);
		}
	}

	if (notes.size() > 0 && notes.begin()->get_channel() == 9)
	{
		instrument = 128; /* Percussion */
	}
}


int MidiFileTrack::get_track_num()
{
	return track_num;
}

//std::list<MidiFileNote> MidiFileTrack::get_notes()
std::vector<MidiFileNote> MidiFileTrack::get_notes()
{
	return notes;
}

int MidiFileTrack::get_instrument()
{
	return instrument;
}

void MidiFileTrack::set_instrument(int inst)
{
	instrument = inst;
}

//std::list<MidiFileEvent> MidiFileTrack::get_lyrics()
std::vector<MidiFileEvent> MidiFileTrack::get_lyrics()
{
	return lyrics;
}

//void MidiFileTrack::set_lyrics(std::list<MidiFileEvent> lyr)
void MidiFileTrack::set_lyrics(std::vector<MidiFileEvent> lyr)
{
	lyrics = lyr;
}

//std::list<MidiFileEvent> MidiFileTrack::get_text()
std::vector<MidiFileEvent> MidiFileTrack::get_text()
{
	return text;
}

//void MidiFileTrack::set_text(std::list<MidiFileEvent> txt)
void MidiFileTrack::set_text(std::vector<MidiFileEvent> txt)
{
	text = txt;
}

//std::list<MidiFileEvent> MidiFileTrack::get_text_and_lyrics()
std::vector<MidiFileEvent> MidiFileTrack::get_text_and_lyrics()
{
	return text_and_lyrics;
}

//void MidiFileTrack::set_text_and_lyrics(std::list<MidiFileEvent> txt_lyr)
void MidiFileTrack::set_text_and_lyrics(std::vector<MidiFileEvent> txt_lyr)
{
	text_and_lyrics = txt_lyr;
}

/** Add a MidiNote to this track.  
 ** This is called for each NoteOn event */
void MidiFileTrack::add_note(MidiFileNote note)
{
	notes.push_back(note);
}

/** Activated when a NoteOff event occurred.
 *  Find the MidiNote of the corresponding NoteOn event,
 *  and update the duration of the MidiNote.
 */
void MidiFileTrack::note_off_event(int chan, int note_num, int end_t)
{
	for (MidiFileNote note : notes)
	{
		if (note.get_channel() == chan &&
			note.get_note_number() == note_num &&
			note.get_duration() == 0)
		{
			note.note_off_event(end_t);
			return;
		}
	}
}

/** Add a lyric event to this track */
void MidiFileTrack::add_lyric_event(MidiFileEvent lyr)
{
	lyrics.push_back(lyr);
}

/** Add a text event to this track */
void MidiFileTrack::add_text_event(MidiFileEvent txt)
{
	text.push_back(txt);
}

/** Add a text and lyric event to this track */
void MidiFileTrack::add_text_and_lyric_event(MidiFileEvent txt_lyr)
{
	text_and_lyrics.push_back(txt_lyr);
}

/**
*   @brief  Returns a deep copy of this track.
*   @param	MidiFileTrack a pointer to the cloned track object
*   @return void
*/
void MidiFileTrack::clone(MidiFileTrack *track)
{
	for (MidiFileNote note : notes)
	{
		MidiFileNote *mnote = new MidiFileNote(note.get_start_time(), 
											   note.get_channel(),
											   note.get_note_number(),
											   note.get_duration());
		
		track->notes.push_back(*mnote);
	}

	for (MidiFileEvent mevent : lyrics)
	{
		track->add_lyric_event(mevent);
	}

	for (MidiFileEvent mevent : text)
	{
		track->add_text_event(mevent);
	}

	for (MidiFileEvent mevent : text_and_lyrics)
	{
		track->add_text_and_lyric_event(mevent);
	}
}

/**
 *  Returns a string with the track number and instrument and a list of all track notes
 */
std::string MidiFileTrack::track_string()
{
	std::string trk_str = "Track number=" + std::to_string(get_track_num()) + 
						  " instrument=" + std::to_string(get_instrument()) + "\n";
	
	for (MidiFileNote note : notes)
	{
		trk_str = trk_str + std::to_string(note.get_note_number()) + "\n";
	}

	trk_str += "End Track\n";
	
	return trk_str;
}
