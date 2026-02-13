/**
*	@file		midiFile.h
*	@author		Nahum Budin
*	@date		7-Feb-2026
*	@version	1.1
*					1. Optimize combine_events_to_single_track() by using 
*					   a min-heap (priority queue) to merge events from all 
*					   tracks in O(n log k) time, where n is the total number 
*					   of events and k is the number of tracks (GitHub Copilot).
*
*	@brief		MIDI files handling.
*
*	History:\n
*		version 1.0	6-Aug-2024 Initial
*
*	Based on my Android TilTune Player Java Project 
*
*/

/* MIDI file format.
 *
 * The Midi File format is described below.  The description uses
 * the following abbreviations.
 *
 * u1     - One byte
 * u2     - Two bytes (big endian)
 * u4     - Four bytes (big endian)
 * varlen - A variable length integer, that can be 1 to 4 bytes. The
 *          integer ends when you encounter a byte that doesn't have
 *          the 8th bit set (a byte less than 0x80).
 * len?   - The length of the data depends on some code
 *
 *
 * The Midi files begins with the main Midi header
 * u4 = The four ascii characters 'MThd'
 * u4 = The length of the MThd header = 6 bytes
 * u2 = 0 if the file contains a single track
 *      1 if the file contains one or more simultaneous tracks
 *      2 if the file contains one or more independent tracks
 * u2 = number of tracks
 * u2 = if >  0, the number of pulses per quarter note
 *      if <= 0, then ???
 *
 * Next come the individual Midi tracks.  The total number of Midi
 * tracks was given above, in the MThd header.  Each track starts
 * with a header:
 *
 * u4 = The four ascii characters 'MTrk'
 * u4 = Amount of track data, in bytes.
 *
 * The track data consists of a series of Midi events.  Each Midi event
 * has the following format:
 *
 * varlen  - The time between the previous event and this event, measured
 *           in "pulses".  The number of pulses per quarter note is given
 *           in the MThd header.
 * u1      - The Event code, always betwee 0x80 and 0xFF
 * len?    - The event data.  The length of this data is determined by the
 *           event code.  The first byte of the event data is always < 0x80.
 *
 * The event code is optional.  If the event code is missing, then it
 * defaults to the previous event code.  For example:
 *
 *   varlen, eventcode1, eventdata,
 *   varlen, eventcode2, eventdata,
 *   varlen, eventdata,  // eventcode is eventcode2
 *   varlen, eventdata,  // eventcode is eventcode2
 *   varlen, eventcode3, eventdata,
 *   ....
 *
 *   How do you know if the eventcode is there or missing? Well:
 *   - All event codes are between 0x80 and 0xFF
 *   - The first byte of eventdata is always less than 0x80.
 *   So, after the varlen delta time, if the next byte is between 0x80
 *   and 0xFF, its an event code.  Otherwise, its event data.
 *
 * The Event codes and event data for each event code are shown below.
 *
 * Code:  u1 - 0x80 thru 0x8F - Note Off event.
 *             0x80 is for channel 1, 0x8F is for channel 16.
 * Data:  u1 - The note number, 0-127.  Middle C is 60 (0x3C)
 *        u1 - The note velocity.  This should be 0
 *
 * Code:  u1 - 0x90 thru 0x9F - Note On event.
 *             0x90 is for channel 1, 0x9F is for channel 16.
 * Data:  u1 - The note number, 0-127.  Middle C is 60 (0x3C)
 *        u1 - The note velocity, from 0 (no sound) to 127 (loud).
 *             A value of 0 is equivalent to a Note Off.
 *
 * Code:  u1 - 0xA0 thru 0xAF - Key Pressure
 * Data:  u1 - The note number, 0-127.
 *        u1 - The pressure.
 *
 * Code:  u1 - 0xB0 thru 0xBF - Control Change
 * Data:  u1 - The controller number
 *        u1 - The value
 *
 * Code:  u1 - 0xC0 thru 0xCF - Program Change
 * Data:  u1 - The program number.
 *
 * Code:  u1 - 0xD0 thru 0xDF - Channel Pressure
 *        u1 - The pressure.
 *
 * Code:  u1 - 0xE0 thru 0xEF - Pitch Bend
 * Data:  u2 - Some data
 *
 * Code:  u1     - 0xFF - Meta Event
 * Data:  u1     - Metacode
 *        varlen - Length of meta event
 *        u1[varlen] - Meta event data.
 *
 *
 * The Meta Event codes are listed below:
 *
 * Metacode: u1         - 0x0  Sequence Number
 *           varlen     - 0 or 2
 *           u1[varlen] - Sequence number
 *
 * Metacode: u1         - 0x1  Text
 *           varlen     - Length of text
 *           u1[varlen] - Text
 *
 * Metacode: u1         - 0x2  Copyright
 *           varlen     - Length of text
 *           u1[varlen] - Text
 *
 * Metacode: u1         - 0x3  Track Name
 *           varlen     - Length of name
 *           u1[varlen] - Track Name
 *
 * Metacode: u1         - 0x58  Time Signature
 *           varlen     - 4
 *           u1         - numerator
 *           u1         - log2(denominator)
 *           u1         - clocks in metronome click
 *           u1         - 32nd notes in quarter note (usually 8)
 *
 * Metacode: u1         - 0x59  Key Signature
 *           varlen     - 2
 *           u1         - if >= 0, then number of sharps
 *                        if < 0, then number of flats * -1
 *           u1         - 0 if major key
 *                        1 if minor key
 *
 * Metacode: u1         - 0x51  Tempo
 *           varlen     - 3
 *           u3         - quarter note length in microseconds
 */

#pragma once

/* @class MidiFile
 *
 * The MidiFile class contains the parsed data from the Midi File.
 * It contains:
 * - All the tracks in the midi file, including all MidiNotes per track.
 * - The time signature (e.g. 4/4, 3/4, 6/8)
 * - The number of pulses per quarter note.
 * - The tempo (number of microseconds per quarter note).
 *
 * The constructor takes a filename as input, and upon returning,
 * contains the parsed data from the midi file.
 *
 * The methods read_track() and read_meta_event() are helper functions called
 * by the constructor during the parsing.
 *
 * After the MidiFile is parsed and created, the user can retrieve the
 * tracks and notes by using the property Tracks and Tracks.Notes.
 *
 * There are two methods for modifying the midi data based on the menu
 * options selected:
 *
 * - ChangeMidiNotes()
 *   Apply the menu options to the parsed MidiFile.  This uses the helper functions:
 *     SplitTrack()
 *     CombineToTwoTracks()
 *     ShiftTime()
 *     Transpose()
 *     RoundStartTimes()
 *     RoundDurations()
 *
 * - ChangeSound()
 *   Apply the menu options to the MIDI music data, and save the modified midi data
 *   to a file, for playback.
 *
 */

#include <string>
#include <list>
#include <vector>

#include "midiFileTrack.h"
#include "midiFileEvent.h"
#include "midiFileTimeSignature.h"
#include "midiFileReader.h"

typedef struct parser_args
{
	std::vector<uint8_t> raw_data;
	bool auto_scl;
	bool auto_drm;
} parser_args_t;

class MidiFile
{
  public:
	MidiFile(std::string file_path);
	MidiFile(std::vector<uint8_t> raw_data, std::string file_path, 
			 bool auto_scl=false, bool auto_drm=false);

	std::vector<MidiFileTrack> get_tracks();
	std::vector<MidiFileEvent> get_combined_track();
	std::vector<MidiFileEvent> get_combined_lyrics_track();
	std::vector<MidiFileEvent> get_combined_text_track();
	std::vector<MidiFileEvent> get_combined_text_and_lyrics_track();
	std::vector<MidiFileEvent> get_processed_and_combined_text_and_lyrics_track();

	int *get_scale_detect_cross_correlation_peak_pos();
	
	MidiFileTimeSignature *get_time_signature();
	std::string get_file_path();
	int get_total_pulses();

	void update_control_change(std::vector<MidiFileEvent> new_events, MidiFileEvent change_event);
	std::vector<std::vector<MidiFileEvent>> start_at_pause_time(vector<vector<MidiFileEvent>> list,
																int pause_time);

	int open_midi_file(std::string path, std::vector<uint8_t> *data);

	int parse(std::vector<uint8_t> raw_data, bool auto_scl = false, bool auto_drm = false);

  private:
	std::string event_name_string(int midi_event_code);
	std::string meta_event_name_string(int midi_event_code);
	bool has_multiple_channels(MidiFileTrack track);
	void check_start_times(std::vector<MidiFileTrack> tracks);

	std::vector<MidiFileEvent> read_track(MidiFileReader *file, int tracknum);
	std::vector<MidiFileTrack> split_channels(MidiFileTrack orig_track, std::vector<MidiFileEvent> events);
	std::vector<MidiFileEvent> combine_events_to_single_track(std::vector<std::vector<MidiFileEvent>> events);

	std::string midi_file_path;
	/** The raw MidiEvents, one vector per track */
	std::vector<std::vector<MidiFileEvent>> all_events;
	/** The tracks of the midifile that have notes */
	std::vector<MidiFileTrack> tracks;
	/** A single track that holds all the MIDI events ordered by time */
	std::vector<MidiFileEvent> combined_track;
	/** A single track that holds all the Lyrics events ordered by time */
	std::vector<MidiFileEvent> combined_lyrics_track;
	/** A single track that holds all the Text events ordered by time */
	std::vector<MidiFileEvent> combined_text_track;
	/** A single track that holds all the Text and Lyrics events ordered by time */
	std::vector<MidiFileEvent> combined_text_and_lyrics_track;
	/** Processed text and lyrics combined events */
	std::vector<MidiFileEvent> processed_and_combined_text_and_lyrics_track;

	int track_mode;
	/** The time signature */
	MidiFileTimeSignature *time_sig;
	/** The number of pulses per quarter note */
	int quarter_note;
	/** The total length of the song, in pulses */
	int total_pulses;
	/** True if we've split each channel into a track */
	bool track_per_channel;
	
	/** Automation means TODO: */
	/** Activates auto drums */
	bool auto_drums;
	/** Activates auto scale */
	bool auto_scales;
	/** Count how many times each note is used in the file (used for looking for Scale) */
	int notes_conters[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	/** Counts how many MIDI events are sent to each channel (used for looking for a "free" channel) */
	int channels_counters[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	/** Least used channel - may be used for solo playing */
	int least_used_channel = 20;
	/** 2nd Least used channel - may be used for Slave solo playing*/
	int second_least_used_channel = 20;
	/** The 3rd lest used channel */
	int third_least_used_channel = 20;
	/** The 4th lest used channel */
	int fourth_least_used_channel = 20;
	/** Scale-pattern: the 7 most used notes */
	int scale_pattern[7];
	/** Peak value of Scale pattern cross correlation */
	int max_scale_peak_val;
	/** Peaks location of Scale pattern cross correlation */
	int max_scale_peak_pos[4][7];
	/* Detected scale type (TODO:) */
	int scale_type;

	// Arpeggio processor
	//Arpeggios mMidiArpeggios;


};
