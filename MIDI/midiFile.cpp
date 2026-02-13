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

#include <stdio.h>
#include <fstream>
#include <thread>
#include <queue>
#include <functional>

#include "../LibAPI/midi.h"
#include "midiFile.h"
#include "midiFileReader.h"
#include "midiFileException.h"
#include "math.h"


/** Creates an empty MidiFile */
MidiFile::MidiFile(std::string file_path)
{
	midi_file_path = file_path;
}

MidiFile::MidiFile(std::vector<uint8_t> raw_data, std::string file_path,
				   bool auto_scl, bool auto_drm)
{
	midi_file_path = file_path;
	auto_drums = auto_drm;
	auto_scales = auto_scl;	
	
	all_events.clear();
	tracks.clear();
	combined_track.clear();
	combined_lyrics_track.clear();
	combined_text_track.clear();
	combined_text_and_lyrics_track.clear();
	processed_and_combined_text_and_lyrics_track.clear();
	
	parse(raw_data, auto_drums, auto_scales);

}


/** Return a String representation of a Midi event */
std::string MidiFile::event_name_string(int midi_event_code)
{
	if (midi_event_code >= _MIDI_EVENT_NOTE_OFF && midi_event_code < _MIDI_EVENT_NOTE_OFF + 16)
	{
		return "NoteOff";
	}
	else if (midi_event_code >= _MIDI_EVENT_NOTE_OFF && midi_event_code < _MIDI_EVENT_NOTE_OFF + 16)
	{
		return "NoteOn";
	}
	else if (midi_event_code >= _MIDI_EVENT_KEY_PRESSURE && midi_event_code < _MIDI_EVENT_KEY_PRESSURE + 16)
	{
		return "KeyPressure";
	}
	else if (midi_event_code >= _MIDI_EVENT_CONTROL_CHANGE && midi_event_code < _MIDI_EVENT_CONTROL_CHANGE + 16)
	{
		return "ControlChange";
	}
	else if (midi_event_code >= _MIDI_EVENT_PROGRAM_CHANGE && midi_event_code < _MIDI_EVENT_PROGRAM_CHANGE + 16)
	{
		return "ProgramChange";
	}
	else if (midi_event_code >= _MIDI_EVENT_CHANNEL_PRESSURE && midi_event_code < _MIDI_EVENT_CHANNEL_PRESSURE + 16)
	{
		return "ChannelPressure";
	}
	else if (midi_event_code >= _MIDI_EVENT_PITCH_BEND && midi_event_code < _MIDI_EVENT_PITCH_BEND + 16)
	{
		return "PitchBend";
	}
	else if (midi_event_code == _MIDI_META_EVENT)
	{
		return "MetaEvent";
	}
	else if (midi_event_code == _MIDI_META_SYSEX_1 || midi_event_code == _MIDI_META_SYSEX_2)
	{
		return "SysexEvent";
	}
	else
	{
		return "Unknown";
	}
}

/** Return a String representation of a Midi meta-event */
std::string MidiFile::meta_event_name_string(int midi_event_code)
{
	if (midi_event_code == _MIDI_META_EVENT_SEQUENCE)
	{
		return "MetaEventSequence";
	}
	else if (midi_event_code == _MIDI_META_EVENT_TEXT)
	{
		return "MetaEventText";
	}
	else if (midi_event_code == _MIDI_META_EVENT_COPYRIGHT)
	{
		return "MetaEventCopyright";
	}
	else if (midi_event_code == _MIDI_META_EVENT_SEQUENCE_NAME)
	{
		return "MetaEventSequenceName";
	}
	else if (midi_event_code == _MIDI_META_EVENT_INSTRUMENT)
	{
		return "MetaEventInstrument";
	}
	else if (midi_event_code == _MIDI_META_EVENT_LYRICS)
	{
		return "MetaEventLyrics";
	}
	else if (midi_event_code == _MIDI_META_EVENT_MARKER)
	{
		return "MetaEventMarker";
	}
	else if (midi_event_code == _MIDI_META_EVENT_END_OF_TRACK)
	{
		return "MetaEventEndOfTrack";
	}
	else if (midi_event_code == _MIDI_META_EVENT_TEMPO)
	{
		return "MetaEventTempo";
	}
	else if (midi_event_code == _MIDI_META_EVENT_SMPTE_OFFSET)
	{
		return "MetaEventSMPTEOffset";
	}
	else if (midi_event_code == _MIDI_META_EVENT_TIME_SIGNATURE)
	{
		return "MetaEventTimeSignature";
	}
	else if (midi_event_code == _MIDI_META_EVENT_KEY_SIGNATURE)
	{
		return "MetaEventKeySignature";
	}
	else
	{
		return "Unknown";
	}
}

/** Get the list of tracks */
std::vector<MidiFileTrack> MidiFile::get_tracks()
{
	return tracks;
}

/** Get the time signature */
MidiFileTimeSignature *MidiFile::get_time_signature()
{
	return time_sig;
}

/** Get the file path */
std::string MidiFile::get_file_path()
{
	return midi_file_path;
}

/** Get the total length (in pulses) of the song */
int MidiFile::get_total_pulses()
{
	return total_pulses;
}

/** Get the combined MIDI events track */
std::vector<MidiFileEvent> MidiFile::get_combined_track()
{ 
	return combined_track; 
}

/** Get the combined lyrics events track */
std::vector<MidiFileEvent> MidiFile::get_combined_lyrics_track()
{
	return combined_lyrics_track;
}

/** Get the combined text events track */
std::vector<MidiFileEvent> MidiFile::get_combined_text_track()
{
	return combined_text_track;
}

/** Get the combined text and lyricsevents track */
std::vector<MidiFileEvent> MidiFile::get_combined_text_and_lyrics_track()
{
	return combined_text_and_lyrics_track;
}

/** Get the processed and combined text and lyricsevents track */
std::vector<MidiFileEvent> MidiFile::get_processed_and_combined_text_and_lyrics_track()
{
	return processed_and_combined_text_and_lyrics_track;
}

/** Get the Scale detection cross correlation peak position (TODO: Automation) */
int *MidiFile::get_scale_detect_cross_correlation_peak_pos() 
{ 
	return &max_scale_peak_pos[0][0]; 
}

/** Parse the given Midi file, and return an instance of this MidiFile
     * class.  After reading the midi file, this object will contain:
     * - The raw list of midi events
     * - The Time Signature of the song
     * - All the tracks in the song which contain notes.
     * - The number, starttime, and duration of each note.
     * 
     *	returns -1 on error.
     */
int MidiFile::parse(std::vector<uint8_t> raw_data, bool auto_scl, bool auto_drm)
{
	std::string id_str;
	int len;

	std::vector<MidiFileTrack> tracks;
	track_per_channel = false;

	MidiFileReader *file = new MidiFileReader(raw_data);

	// File must start with a MThd feader
	id_str = file->read_string(4);
	if (id_str != "MThd")
	{
		printf("MIDI file oesn't start with MThd");
		return -1;
	}
	/* Next is the header length */
	len = file->read_int();
	if (len != 6)
	{
		printf("MIDI file bad MThd header");
		return -1;
	}
	// Get header information
	track_mode = (short)file->read_short();
	int num_tracks = file->read_short();
	quarter_note = file->read_short();
	
	/* Build a list of all the file events and each track of the file */
	for (int track_num = 0; track_num < num_tracks; track_num++)
	{
		all_events.push_back(read_track(file, track_num));
		MidiFileTrack *track = new MidiFileTrack(all_events.at(track_num), track_num);
		if (track->get_notes().size() > 0)
		{
			tracks.push_back(*track);
		}
	}

	// Get the length of the song in pulses
	for (MidiFileTrack track : tracks)
	{
		/* Find longest track length - look at last notr of each track. */
		MidiFileNote last = track.get_notes().at(track.get_notes().size() - 1);
		if (total_pulses < last.get_start_time() + last.get_duration())
		{
			total_pulses = last.get_start_time() + last.get_duration();
		}
	}

	/* If we only have one track with multiple channels, then treat
	 * each channel as a separate track.
	 */ 
	if (tracks.size() == 1 && has_multiple_channels(tracks.at(0)))
	{
		tracks = split_channels(tracks.at(0), all_events.at(tracks.at(0).track_num));
		track_per_channel = true;
	}

	/* Look for the file Musical-Scale and for a free MIDI channel */
	if (auto_scl)
	{
		// TODO:
	}

	/* Build an arpeggio track  */
	if (auto_drm)
	{
		// TODO:
	}

	/* Build a single track that holds all events ordered by time.
     * This will be used for playing the MIDI file using an events player
     * and for building the text lyrics sheet
     */
	combined_track = combine_events_to_single_track(all_events);
	
	/* Verify that notes are in increasing order. */
	check_start_times(tracks);
	
	/* Determine the time signature */
	int tempo_count = 0;
	long tempo = 0;
	int numerator = 0;
	int denominator = 0;
	
	for (std::vector<MidiFileEvent> events : all_events)
	{
		for (MidiFileEvent mevent : events)
		{
			if (mevent.metaevent == _MIDI_META_EVENT_TEMPO)
			{
				// Take average of all tempos !!!! TODO:
				tempo += mevent.tempo;
				tempo_count++;
			}
			
			if (mevent.metaevent == _MIDI_META_EVENT_TIME_SIGNATURE && numerator == 0)
			{
				numerator = mevent.numerator;
				denominator = mevent.denominator;
			}
		}
	}
	
	if (tempo == 0)
	{
		/* Default value. */
		tempo = 500000; /* 500,000 microseconds = 0.05 sec */
	}
	else
	{
		tempo = tempo / tempo_count;
	}
	
	if (numerator == 0)
	{
		/* Default value. */
		numerator = 4;
		denominator = 4;
	}

	time_sig = new MidiFileTimeSignature(numerator, denominator, quarter_note, (int)tempo);
	
	return 0;
}


/** Parse a single Midi track into a list of MidiEvents.
 * Entering this function, the file offset should be at the start of
 * the MTrk header.  Upon exiting, the file offset should be at the
 * start of the next MTrk header.
 */
std::vector<MidiFileEvent> MidiFile::read_track(MidiFileReader *file, int track_num)
{
	std::vector<MidiFileEvent> result; //(20);
	int start_time = 0;
	
	std::string id = file->read_string(4);
	
	/* Every track starts with a MTrk header */
	if (id != ("MTrk"))
	{
		throw new MidiFileException("Bad MTrk header", file->get_offset() - 4);
	}
	
	int track_len = file->read_int();
	int track_end = track_len + file->get_offset();

	uint8_t event_flag = 0;
	uint8_t last_valid_running_status = 0;

	while (file->get_offset() < track_end)
	{
		/* If the midi file is truncated here, we can still recover.
		 * Just return what we've parsed so far.
		 */

		int start_offset, delta_time;
		uint8_t peek_event;
		try
		{
			start_offset = file->get_offset();
			delta_time = file->read_var_len();
			start_time += delta_time;
			peek_event = file->peek_byte();
		}
		catch (MidiFileException e)
		{
			return result;
		}

		/* Get next event and update its start time based on the accumulated delta-times. */
		MidiFileEvent mevent;
		//= new MidiFileEvent();
		//result.push_back(mevent);
		mevent.delta_time = delta_time;
		mevent.start_time = start_time;

		mevent.originating_track_num = (uint8_t)(track_num & 0x0f);

		/* msb of an event 1st byte is set (0x8z, 0x9z,...,0xfz) */
		if (peek_event >= 0x80)
		{
			event_flag = file->read_byte();

			if (peek_event < 0xf0)
			{
				mevent.has_running_status = true;
				// Last valid running state
				last_valid_running_status = event_flag;
			}
			else
			{
				//  f0-ff: cancel running status
				mevent.has_running_status = false;
			}
		}

		if (event_flag >= _MIDI_EVENT_NOTE_ON && event_flag < _MIDI_EVENT_NOTE_ON + 16)
		{
			mevent.event_command = _MIDI_EVENT_NOTE_ON;
			mevent.channel = ((uint8_t)(event_flag - _MIDI_EVENT_NOTE_ON));
			mevent.note_number = file->read_byte();
			mevent.velocity = file->read_byte();
			mevent.event_length = 3;
		}
		else if (event_flag >= _MIDI_EVENT_NOTE_OFF && event_flag < _MIDI_EVENT_NOTE_OFF + 16)
		{
			mevent.event_command = _MIDI_EVENT_NOTE_OFF;
			mevent.channel = ((uint8_t)(event_flag - _MIDI_EVENT_NOTE_OFF));
			mevent.note_number = file->read_byte();
			mevent.velocity = file->read_byte();
			mevent.event_length = 3;
		}
		else if (event_flag >= _MIDI_EVENT_KEY_PRESSURE && event_flag < _MIDI_EVENT_KEY_PRESSURE + 16)
		{
			mevent.event_command = _MIDI_EVENT_KEY_PRESSURE;
			mevent.channel = ((uint8_t)(event_flag - _MIDI_EVENT_KEY_PRESSURE));
			mevent.note_number = file->read_byte();
			mevent.key_pressure = file->read_byte();
			mevent.event_length = 3;
		}
		else if (event_flag >= _MIDI_EVENT_CONTROL_CHANGE && event_flag < _MIDI_EVENT_CONTROL_CHANGE + 16)
		{
			mevent.event_command = _MIDI_EVENT_CONTROL_CHANGE;
			mevent.channel = ((uint8_t)(event_flag - _MIDI_EVENT_CONTROL_CHANGE));
			mevent.control_num = file->read_byte();
			mevent.control_value = file->read_byte();
			mevent.event_length = 3;
		}
		else if (event_flag >= _MIDI_EVENT_PROGRAM_CHANGE && event_flag < _MIDI_EVENT_PROGRAM_CHANGE + 16)
		{
			mevent.event_command = _MIDI_EVENT_PROGRAM_CHANGE;
			mevent.channel = ((uint8_t)(event_flag - _MIDI_EVENT_PROGRAM_CHANGE));
			mevent.instrument = file->read_byte();
			mevent.event_length = 2;
		}
		else if (event_flag >= _MIDI_EVENT_CHANNEL_PRESSURE && event_flag < _MIDI_EVENT_CHANNEL_PRESSURE + 16)
		{
			mevent.event_command = _MIDI_EVENT_CHANNEL_PRESSURE;
			mevent.channel = ((uint8_t)(event_flag - _MIDI_EVENT_CHANNEL_PRESSURE));
			mevent.chan_pressure = file->read_byte();
			mevent.event_length = 2;
		}
		else if (event_flag >= _MIDI_EVENT_PITCH_BEND && event_flag < _MIDI_EVENT_PITCH_BEND + 16)
		{
			mevent.event_command = _MIDI_EVENT_PITCH_BEND;
			mevent.channel = ((uint8_t)(event_flag - _MIDI_EVENT_PITCH_BEND));
			mevent.pitch_bend = (short)file->read_short();
			mevent.event_length = 3;
		}
		else if (event_flag == _MIDI_META_SYSEX_1)
		{
			mevent.event_command = _MIDI_META_SYSEX_1;
			mevent.meta_length = file->read_var_len();
			mevent.raw_values = file->read_bytes(mevent.meta_length);
		}
		else if (event_flag == _MIDI_META_SYSEX_2)
		{
			mevent.event_command = _MIDI_META_SYSEX_2;
			mevent.meta_length = file->read_var_len();
			mevent.raw_values = file->read_bytes(mevent.meta_length);
		}
		else if (event_flag == _MIDI_META_EVENT)
		{
			mevent.event_command = _MIDI_META_EVENT;
			mevent.metaevent = file->read_byte();
			mevent.meta_length = file->read_var_len();
			mevent.raw_values = file->read_bytes(mevent.meta_length);
			
			if (mevent.meta_length == 0)
			{
				mevent.meta_length = 1;
			}
			
			if (mevent.metaevent == _MIDI_META_EVENT_TIME_SIGNATURE)
			{
				if (mevent.meta_length < 2)
				{
					throw new MidiFileException(
						"Meta Event Time Signature len == " + 
							std::to_string(mevent.meta_length) + " != 4",
						file->get_offset());
				}
				else
				{
					mevent.numerator = ((uint8_t)mevent.raw_values[0]);
					mevent.denominator = ((uint8_t)pow(2, mevent.raw_values[1]));
				}
			}
			else if (mevent.metaevent == _MIDI_META_EVENT_TEMPO)
			{
				if (mevent.meta_length != 3)
				{
					throw new MidiFileException(
						"Meta Event Tempo len == " + 
							std::to_string(mevent.meta_length) +
							" != 3",
						file->get_offset());
				}
				mevent.tempo = ((mevent.raw_values[0] & 0xFF) << 16) |
								((mevent.raw_values[1] & 0xFF) << 8) |
								(mevent.raw_values[2] & 0xFF);
			}
			else if (mevent.metaevent == _MIDI_META_EVENT_END_OF_TRACK)
			{
				/* break;  */
			}
		}
		else
		{
			throw new MidiFileException("Unknown event " + 
										std::to_string(mevent.event_command),
										file->get_offset() - 1);
		}
		
		result.push_back(mevent);
		
		/* Recover last valid running status value */
		event_flag = last_valid_running_status;
	}

	return result;
}

/** Return true if this track contains multiple channels.
 * If a MidiFile contains only one track, and it has multiple channels,
 * then we treat each channel as a separate track.
 */
bool MidiFile::has_multiple_channels(MidiFileTrack track)
{
	int channel = track.get_notes().at(0).get_channel();
	
	for (MidiFileNote note : track.get_notes())
	{
		if (note.get_channel() != channel)
		{
			return true;
		}
	}
	
	return false;
}

/** Split the given track into multiple tracks, separating each
 *   channel into a separate track.
 */
std::vector<MidiFileTrack> MidiFile::split_channels(MidiFileTrack orig_track, std::vector<MidiFileEvent> events)
{
	/* Find the instrument used for each channel */
	int channel_instruments[16];
	
	for (MidiFileEvent mevent : events)
	{
		if (mevent.event_command == _MIDI_EVENT_PROGRAM_CHANGE)
		{
			channel_instruments[mevent.channel] = mevent.instrument;
		}
	}
	
	channel_instruments[9] = 128; /* Channel 9 = Percussion */

	/* Will hold the per channel track. */
	std::vector<MidiFileTrack> result;
	
	for (MidiFileNote note : orig_track.get_notes())
	{
		bool found_channel = false;
		
		for (MidiFileTrack track : result)
		{
			/* Look for the channels' track and add the note. */
			if (note.get_channel() == track.get_notes().at(0).get_channel())
			{
				found_channel = true;
				track.add_note(note);
				break; // <<<<<<<<<<<<<<<<<<<< only one track per channel? Added Aug 10 2024
			}
		}
		
		if (!found_channel)
		{
			/* No channel tracka found to this note - create a new channel track. */
			MidiFileTrack *track = new MidiFileTrack(result.size() + 1);
			track->add_note(note);
			track->set_instrument(channel_instruments[note.get_channel()]);
			result.push_back(*track);
		}
	}
	/* Handle lyrics track (Karaoke) */
	std::vector<MidiFileEvent> lyrics = orig_track.get_lyrics();
	
	if (!lyrics.empty())
	{
		for (MidiFileEvent lyric_event : lyrics)
		{
			for (MidiFileTrack track : result)
			{
				if (lyric_event.channel == track.get_notes().at(0).get_channel())
				{
					/* Add the lyric event to its channel track. */
					track.add_lyric_event(lyric_event);
				}
			}
		}
	}
	
	return result;
}

/** Combine the events in the given tracks into a single MidiTrack.
 *  The individual tracks are already sorted.  
 *  To merge them, we use a mergesort-like algorithm.
 */
std::vector<MidiFileEvent> MidiFile::combine_events_to_single_track(std::vector<std::vector<MidiFileEvent>> events)
{
	/* Add all events into one track */
	std::vector<MidiFileEvent> result;

	if (events.size() == 0)
	{
		return result;
	}
	else if (events.size() == 1)
	{
		// only 1 track
		//return events.at(0);
		return events[0];
	}
	
	// **Optimization 1**: Reserve capacity to avoid reallocations
	size_t total_events = 0;
	for (const auto& track : events)
	{
		total_events += track.size();
	}
	result.reserve(total_events);
	
	// **Optimization 2**: Use a min-heap (priority queue) for O(log M) extraction
	// instead of O(M) linear search
	struct TrackIterator
	{
		size_t track_num;		// Which track this event comes from
		size_t event_index;		// Position within that track
		int start_time;			// Timestamp for comparison
		
		bool operator>(const TrackIterator& other) const
		{
			return start_time > other.start_time; // Min-heap
		}
	};
	
	std::priority_queue<TrackIterator, std::vector<TrackIterator>, std::greater<TrackIterator>> pq;
	
	// Initialize heap with first event from each track
	for (size_t track_num = 0; track_num < events.size(); ++track_num)
	{
		if (!events[track_num].empty())
		{
			pq.push({ track_num, 0, events[track_num][0].start_time });
		}
	}
	
	// **Optimization 3**: Merge using heap - O(N log M) complexity
	while (!pq.empty())
	{
		TrackIterator current = pq.top();		// Get earliest event
		pq.pop();								// Remove it from heap

		// **Optimization 4**: Use reference to avoid copying
		const std::vector<MidiFileEvent>& track = events[current.track_num];
		result.push_back(track[current.event_index]);

		// Push next event from same track
		size_t next_index = current.event_index + 1;
		if (next_index < track.size())
		{
			pq.push({ current.track_num, next_index, track[next_index].start_time });
		}
	}

	
	/*
	int event_index[events.size() + 1];
	int event_count[events.size() + 1];

	for (int tracknum = 0; tracknum < events.size(); tracknum++)
	{
		event_index[tracknum] = 0;
		event_count[tracknum] = events.at(tracknum).size();
	}

	MidiFileEvent prev_event;

	while (true)
	{

		MidiFileEvent lowest_event;
		int lowest_track = -1;
		bool lowest_event_set = false;

		// Find the next earliest merged event from all tracks
		for (int track_num = 0; track_num < events.size(); track_num++)
		{
			std::vector<MidiFileEvent> events_track = events.at(track_num);
			
			if (event_index[track_num] >= event_count[track_num])
			{
				// All this track events were added, go to next track on last note of current track
				continue;
			}

			MidiFileEvent event = events_track.at(event_index[track_num]);

			if (!lowest_event_set)
			{
				// 1st event merged in
				lowest_event = event;
				lowest_track = track_num;
				lowest_event_set = true;
			}
			else if (event.start_time < lowest_event.start_time)
			{
				// not the 1st note but starts earlier
				lowest_event = event;
				lowest_track = track_num;
			}			              
		}

		if (!lowest_event_set)
		{
			// no more events found - We've finished the merge 
			break;
		}
		// Go to next event
		event_index[lowest_track]++;
		result.push_back(lowest_event);
		prev_event = lowest_event;
	}

*/

	return result;
}

/** Check that the MidiNote start times are in increasing order.
     * This is for debugging purposes.
     */
void MidiFile::check_start_times(std::vector<MidiFileTrack> tracks)
{
	for (MidiFileTrack track : tracks)
	{
		int prevtime = -1;
		
		for (MidiFileNote note : track.get_notes())
		{
			if (note.get_start_time() < prevtime)
			{
				throw new MidiFileException("Internal parsing error", 0);
			}
			
			prevtime = note.get_start_time();
		}
	}
}

/** Search the events for a ControlChange event with the same
 *  channel and control number.  If a matching event is found,
 *  update the control value.  Else, add a new ControlChange event.
 */
void MidiFile::update_control_change(std::vector<MidiFileEvent> new_events, MidiFileEvent change_event)
{
	for (MidiFileEvent mevent : new_events)
	{
		if ((mevent.event_command == change_event.event_command) &&
			(mevent.channel == change_event.channel) &&
			(mevent.control_num == change_event.control_num))
		{
			mevent.control_value = change_event.control_value;
			return;
		}
	}
	
	new_events.push_back(change_event);
}

/** Start the Midi music at the given pause time (in pulses).
 *  Remove any NoteOn/NoteOff events that occur before the pause time.
 *  For other events, change the delta-time to 0 if they occur
 *  before the pause time.  Return the modified Midi Events.
 */
std::vector<std::vector<MidiFileEvent>> MidiFile::start_at_pause_time(vector<vector<MidiFileEvent>> list, 
																   int pause_time)
{
	vector<vector<MidiFileEvent>> new_list(list.size());

	for (int track_num = 0; track_num < list.size(); track_num++)
	{
		vector<MidiFileEvent> events = list.at(track_num);
		
		vector<MidiFileEvent> new_events(events.size());
		
		new_list.push_back(new_events);

		bool found_event_after_pause = false;
		
		for (MidiFileEvent mevent : events)
		{

			if (mevent.start_time < pause_time)
			{
				if (mevent.event_command == _MIDI_EVENT_NOTE_ON ||
					mevent.event_command == _MIDI_EVENT_NOTE_OFF)
				{

					/* Skip NoteOn/NoteOff event */
				}
				else if (mevent.event_command == _MIDI_EVENT_CONTROL_CHANGE)
				{
					mevent.delta_time = 0;
					update_control_change(new_events, mevent);
				}
				else
				{
					mevent.delta_time = 0;
					new_events.push_back(mevent);
				}
			}
			else if (!found_event_after_pause)
			{
				mevent.delta_time = (mevent.start_time - pause_time);
				new_events.push_back(mevent);
				found_event_after_pause = true;
			}
			else
			{
				new_events.push_back(mevent);
			}
		}
	}
	
	return new_list;
}

/** Reads a MIDI file. */
int MidiFile::open_midi_file(std::string path, std::vector<uint8_t> *data)
{
	char byte;

	if (path == "")
	{
		return -1;
	}

	std::ifstream midi_file;
	midi_file.open(path, std::ifstream::in);
	int err = errno;
	if (!midi_file.is_open())
	{
		fprintf(stderr, "Can't open %s MIDI file for read!\n", path.c_str());
		return -2;
	}

	while (midi_file.get(byte))
	{
		data->push_back((uint8_t)byte);
	}

	midi_file.close();

	return 0;
}


