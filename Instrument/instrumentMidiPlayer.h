/**
*	@file		instrumenyMidiPlayer.h
*	@author		Nahum Budin
*	@date		8-Aug-2024
*	@version	1.0
*
*	@brief		MIDI files player.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#pragma once

#include <vector>
#include <string>

#include "instrument.h"
#include "instrumentMidiPlayerPlaybackThread.h"
#include "../MIDI/midiFile.h"

using namespace std;

class InstrumentMidiPlayer : public Instrument
{
  public:
	/* Flags to indicate each channel ON notes (used to turn OFF when pausing)*/
	static bool notes_on[16][128];

	InstrumentMidiPlayer(AlsaMidiSysControl *alsa_control = NULL,
						 string *alsa_client_in_name=NULL);
	~InstrumentMidiPlayer();

	static InstrumentMidiPlayer *get_instrument_midi_player_instance();

	void register_clear_all_playing_notes_callback(func_ptr_void_void_t ptr);
	void register_midi_change_program_callback(func_ptr_void_uint8_t_uint8_t ptr);
	void register_midi_change_channel_volume_callback(func_ptr_void_uint8_t_uint8_t ptr);
	void register_send_midi_events_vector_callback(func_ptr_void_midi_events_vector_int ptr);
	void register_midi_player_potision_update_callback(func_ptr_void_int_t ptr);
	void register_midi_player_total_playing_time_update_callback(func_ptr_void_int_int_t ptr);
	void register_midi_player_playing_time_update_callback(func_ptr_void_int_int_t ptr);
	void register_midi_player_song_remaining_time_update_callback(func_ptr_void_int_int_t ptr);

	int init_new_file(string path, string title= "");

	int init_new_playback_thread();
	
	bool is_player_playing();
	bool is_playback_thread_running();

	static func_ptr_void_void_t clear_all_playing_notes_callback_ptr;
	static func_ptr_void_uint8_t_uint8_t midi_change_program_callback_ptr;
	static func_ptr_void_uint8_t_uint8_t midi_change_channel_volume_callback_ptr;
	static func_ptr_void_midi_events_vector_int send_midi_events_vector_callback_ptr;
	static func_ptr_void_int_t midi_player_potision_update_callback_ptr;
	static func_ptr_void_int_int_t midi_player_total_playing_time_update_callback_ptr;
	static func_ptr_void_int_int_t midi_player_playing_time_update_callback_ptr;
	static func_ptr_void_int_int_t midi_player_file_remaining_time_update_callback_ptr;

	void play();
	void pause();
	void stop();

	void set_off_all_on_notes();
	void send_all_notes_off_command();
	void send_all_sounds_off_command();

	void send_midi_commands(vector<MidiFileEvent> events, int vol);

  private:
	MidiPlaybackThread *midi_playback_threads;

	/** File relative play position 0-100 */
	static int file_relative_position;
	
	/* data to hold file raw data */
	static vector<uint8_t> file_data_bytes;
	/** File name */
	static string file_title;
	/** File full path and name */
	static string file_path;
	/** True when a file was already intialized */
	static bool file_initialized;
	/** True when Playback thread is running */
  	//static bool playback_thread_is_running;
	/** The midi file to play */
	static MidiFile *midi_file;
	/** Player volume */
	static int playback_Volume;
	/* Player speed % */
	static int playback_relative_speed;
	static float player_speed;
	/* The number of pulses per millisec */
	static double pulses_per_msec;

	static InstrumentMidiPlayer *instrument_midi_player_instance;

	void terminate();
	bool has_midi_header(vector<uint8_t> data);

};
	

	

	

