/**
*	@file		instrumenyMidiPlayerPlaybackTread.h
*	@author		Nahum Budin
*	@date		14-Aug-2024
*	@version	1.0
*
*	@brief		MIDI files player main playback thread.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#pragma once

#include <time.h>
#include <signal.h>

#include "../MIDI/midiFile.h"

using namespace std;

class MidiPlaybackThread
{
  public:
	MidiPlaybackThread(MidiFile *mid_file,
					   func_ptr_void_void_t clr_all_playing_notes_callback_ptr,
					   func_ptr_void_uint8_t_uint8_t midi_change_prog_callback_ptr,
					   func_ptr_void_uint8_t_uint8_t midi_change_chan_volume_callback_ptr,
					   func_ptr_void_midi_events_vector_int send_midi_events_vec_ptr,
					   func_ptr_void_int_t midi_player_pos_update_callback_ptr,
					   func_ptr_void_int_int_t midi_player_tot_play_time_update_callback_ptr,
					   func_ptr_void_int_int_t midi_player_play_time_update_callback_ptr,
					   func_ptr_void_int_int_t midi_player_file_rem_time_update_callback_ptr);

	~MidiPlaybackThread();

	void set_playback_speed(float spd);

	static void start_playing();
	static void pause_playing();
	static void stop_playing();

	static bool is_playback_thread_running();
	static bool is_update_thread_running();	
	
	static bool is_player_playing();

	void set_pulses_per_ms(double ppm);

  private:
	/* True when Playback thread is running */
	static bool playback_thread_is_running;
	/* True when Update thread is running */
	static bool update_thread_is_running;
	/* The playing state of the Player */
	static int player_state;
	/* True if playback is Paused */
	static bool paused;
	/* St true to auto loopback */
	static bool auto_loop_back_on;

	/* MIDI file parameters */
	/* The number of pulses per millisec */
	static double pulses_per_msec;
	/* Absolute time when music started playing (msec) */
	static long start_playing_time_ms;
	/* Time (in pulses) when music started playing */
	static double start_pulse_time;
	/* Time (in pulses) music is currently at */
	static double current_pulse_time;
	/* Time (in pulses) music was last at */
	static double prev_pulse_time;
	/* Time (in pulses) when playing is paused */
	double pause_time;
	/* Pause start time */
	static long pause_start_time_msec;
	/* Pause stop time */
	static long pause_stop_time_msec;
	/* Pause duration time */
	static long pause_duration_time_msec;
	/* Next event to be played */
	static int next_played_event_index;
	/* Set true when MIDI events playing is in progress */
	static bool play_midi_events_in_progress;
	/* Set true when events playing play-state changes */
	static bool events_play_state_changed;
	/** File play position 0-100*/
	static int file_position;
	/** File total play time Minutes */
	static int file_total_playing_time_minutes;
	/** File total play time Minutes */
	static int file_total_playing_time_seconds;
	/** File play elapsed time Minutes */
	static int file_playing_time_minutes;
	/** File play elapsed time Seconds */
	static int file_playing_time_seconds;
	/** File renaining play time msec */
	static int remaining_file_play_msec;
	/** File renaining play time Seconds */
	static int remaining_file_play_sec;
	/** File renaining play time Minutes */
	static int remaining_file_play_min;

	/* Events Playback volume (0-100) */
	static int playback_volume;
	/* MIDI Events Playback speed 50% to 150% of file BPM) */
	static float speed;
	/* Played file */
	static MidiFile *midi_file;
	/* MIDI file full path */
	string midi_file_path;

	static func_ptr_void_void_t clear_all_playing_notes_callback_ptr;
	static func_ptr_void_uint8_t_uint8_t midi_change_program_callback_ptr;
	static func_ptr_void_uint8_t_uint8_t midi_change_channel_volume_callback_ptr;
	static func_ptr_void_midi_events_vector_int send_midi_events_vector_callback_ptr;
	static func_ptr_void_int_t midi_player_potision_update_callback_ptr;
	static func_ptr_void_int_int_t midi_player_playing_time_update_callback_ptr;
	static func_ptr_void_int_int_t midi_player_file_remaining_time_update_callback_ptr;
	static func_ptr_void_int_int_t midi_player_file_total_time_update_callback_ptr;

	/* Playback Thread */
	static void *playback_thread(void *thread_id);
	static pthread_t playback_thread_id;
	/* Update Thread */
	static void *update_thread(void *thread_id);
	static pthread_t update_thread_id;

	static void do_play();
	static void do_stop();
	static void start_play_midi_file_events();
	static void stop_play_midi_file_events();

	static void start_playback_thread();
	static void stop_playback_thread();
	static void start_update_thread();
	static void stop_update_thread();
};
