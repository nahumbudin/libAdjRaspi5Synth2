/**
*	@file		instrumenyMidiPlayerPlaybackTread.cpp
*	@author		Nahum Budin
*	@date		28-Mar-2026
*	@version	1.1
*			1. Added support for going forward and backward in the file.
*			2. Added suport for loopback playing control
*			3. Added support for playback volume and speed control.
*			4. Added support for retrieving midi file meta data.
*
*	@brief		MIDI files player.
*
*	History:\n
*		Version 1.0 14-Aug-2024 Initial
*
*	Based on my Android TilTune Player Java Project 
*
*/

#include <sys/time.h>
#include <stddef.h>
#include <thread>

#include "../LibAPI/defines.h"
#include "../LibAPI/midi.h"
#include "../utils/utils.h"
#include "../Misc/priorities.h"

#include "instrumentMidiPlayerPlaybackThread.h"
#include "instrumentMidiPlayer.h"

/* Static parameters */
pthread_t MidiPlaybackThread::playback_thread_id = NULL;
pthread_t MidiPlaybackThread::update_thread_id = NULL;
int MidiPlaybackThread::next_played_event_index = 0;
bool MidiPlaybackThread::playback_thread_is_running = false;
bool MidiPlaybackThread::update_thread_is_running = false;
int MidiPlaybackThread::player_state = _MIDI_PLAYER_STATE_STOPPED;
bool MidiPlaybackThread::play_midi_events_in_progress = false;
bool MidiPlaybackThread::events_play_state_changed = false;
double MidiPlaybackThread::pulses_per_msec;
long MidiPlaybackThread::start_playing_time_ms = 0;
double MidiPlaybackThread::start_pulse_time = 0;
double MidiPlaybackThread::current_pulse_time = 0;
double MidiPlaybackThread::prev_pulse_time = -10;
long MidiPlaybackThread::pause_duration_time_msec = 0;
long MidiPlaybackThread::pause_start_time_msec = 0;
long MidiPlaybackThread::pause_stop_time_msec = 0;
int MidiPlaybackThread::playback_volume = 50;
bool MidiPlaybackThread::auto_loop_back_on = false;
float MidiPlaybackThread::speed = 1.0f;
int MidiPlaybackThread::file_position = 0;
int MidiPlaybackThread::file_total_playing_time_minutes = 0;
int MidiPlaybackThread::file_total_playing_time_seconds = 0;
int MidiPlaybackThread::file_playing_time_minutes = 0;
int MidiPlaybackThread::file_playing_time_seconds = 0;
int MidiPlaybackThread::remaining_file_play_msec = 0;
int MidiPlaybackThread::remaining_file_play_sec = 0;
int MidiPlaybackThread::remaining_file_play_min = 0;
bool MidiPlaybackThread::paused = false;
bool MidiPlaybackThread::seek_in_progress = false;
pthread_mutex_t MidiPlaybackThread::state_mutex = PTHREAD_MUTEX_INITIALIZER;

func_ptr_void_void_t MidiPlaybackThread::clear_all_playing_notes_callback_ptr = NULL;
func_ptr_void_uint8_t_uint8_t MidiPlaybackThread::midi_change_program_callback_ptr = NULL;
func_ptr_void_uint8_t_uint8_t MidiPlaybackThread::midi_change_channel_volume_callback_ptr = NULL;
func_ptr_void_midi_events_vector_int MidiPlaybackThread::send_midi_events_vector_callback_ptr = NULL;
func_ptr_void_int_t MidiPlaybackThread::midi_player_potision_update_callback_ptr = NULL;
func_ptr_void_int_int_t MidiPlaybackThread::midi_player_playing_time_update_callback_ptr = NULL;
func_ptr_void_int_int_t MidiPlaybackThread::midi_player_file_remaining_time_update_callback_ptr = NULL;
func_ptr_void_int_int_t MidiPlaybackThread::midi_player_file_total_time_update_callback_ptr = NULL;

MidiFile *MidiPlaybackThread::midi_file = NULL;

MidiPlaybackThread::MidiPlaybackThread(MidiFile *mid_file,
									   func_ptr_void_void_t clr_all_playing_notes_callback_ptr,
									   func_ptr_void_uint8_t_uint8_t midi_change_prog_callback_ptr,
									   func_ptr_void_uint8_t_uint8_t midi_change_chan_volume_callback_ptr,
									   func_ptr_void_midi_events_vector_int send_midi_events_vec_ptr,
									   func_ptr_void_int_t midi_player_pos_update_callback_ptr,
									   func_ptr_void_int_int_t midi_player_tot_play_time_update_callback_ptr,
									   func_ptr_void_int_int_t midi_player_play_time_update_callback_ptr,
									   func_ptr_void_int_int_t midi_player_file_rem_time_update_callback_ptr)
{
	midi_file = mid_file;
	
	this->clear_all_playing_notes_callback_ptr = clr_all_playing_notes_callback_ptr;
	this->midi_change_program_callback_ptr = midi_change_prog_callback_ptr;
	this->midi_change_channel_volume_callback_ptr = midi_change_chan_volume_callback_ptr;
	this->send_midi_events_vector_callback_ptr = send_midi_events_vec_ptr;
	this->midi_player_potision_update_callback_ptr = midi_player_pos_update_callback_ptr;
	this->midi_player_playing_time_update_callback_ptr = midi_player_play_time_update_callback_ptr;
	this->midi_player_file_remaining_time_update_callback_ptr = midi_player_file_rem_time_update_callback_ptr;
	this->midi_player_file_total_time_update_callback_ptr = midi_player_tot_play_time_update_callback_ptr;

	midi_file_path = midi_file->get_file_path();

	start_pulse_time = 0;

	start_playing_time_ms = get_current_time_ms();
	pause_start_time_msec = start_playing_time_ms;
	pause_stop_time_msec = start_playing_time_ms;
}

MidiPlaybackThread::~MidiPlaybackThread()
{
	//thread_is_running = false;
	//delete_playback_update_timer();
}

/* Returns playback thread status */
bool MidiPlaybackThread::is_playback_thread_running()
{
	return playback_thread_is_running;
}

/* Returns playback playing status */
bool MidiPlaybackThread::is_player_playing()
{
	return play_midi_events_in_progress;
}

void MidiPlaybackThread::set_pulses_per_ms(double ppm)
{
	pulses_per_msec = ppm;
}

midi_file_meta_data_t MidiPlaybackThread::get_file_metadata()
{
	if (midi_file != NULL)
	{
		return midi_file->get_file_metadata();
	}
	else
	{
		midi_file_meta_data_t empty_data;
		return empty_data;
	}
}

void MidiPlaybackThread::set_playback_speed(int spd)
{
	float new_speed = spd / 100.0;

	if (new_speed < 0.5f)
	{
		new_speed = 0.5f;
	}
	else if (new_speed > 1.5f)
	{
		new_speed = 1.5f;
	}

	pthread_mutex_lock(&state_mutex); // ← LOCK ENTIRE OPERATION

	// If not playing, just set speed and return
	if (player_state != _MIDI_PLAYER_STATE_PLAYING)
	{
		speed = new_speed;
		pthread_mutex_unlock(&state_mutex);
		return;
	}

	// If playing, just update speed and timing in-place
	// No need to pause/resume
	long current_time_ms = get_current_time_ms();
	long target_elapsed_ms = (long)(current_pulse_time / (pulses_per_msec * speed));

	// Update speed
	speed = new_speed;

	// Recalculate start time for new speed
	start_playing_time_ms = current_time_ms -
							(long)(current_pulse_time / (pulses_per_msec * speed)) -
							pause_duration_time_msec;

	pthread_mutex_unlock(&state_mutex);
}

int MidiPlaybackThread::get_playback_speed()
{
	return (int)(speed * 100);
}

void MidiPlaybackThread::set_playback_volume(int vol)
{
	if (vol < 0)
	{
		vol = 0;
	}
	else if (vol > 100)
	{
		vol = 100;
	}
	playback_volume = vol;
}
int MidiPlaybackThread::get_playback_volume()
{
	return playback_volume;
}

void MidiPlaybackThread::start_playing()
{

	if (midi_file == NULL)
	{
		return;
	}

	switch (player_state)
	{
		case _MIDI_PLAYER_STATE_INIT_STOP:
		case _MIDI_PLAYER_STATE_INIT_PAUSE:
			// Not ready
			return;

		case _MIDI_PLAYER_STATE_PLAYING:
			// Already playing
			pause_playing();
			break;

		default:
			/* play state is stopped or paused */
			/* Start playing */
			do_play();
			break;
	}
}

/** The actual pause is done when the timer is invoked. */
void MidiPlaybackThread::pause_playing()
{
	if (midi_file == NULL)
	{
		return;
	}

	else if (player_state == _MIDI_PLAYER_STATE_PLAYING)
	{
		player_state = _MIDI_PLAYER_STATE_INIT_PAUSE;
	}
}

void MidiPlaybackThread::stop_playing()
{

	if ((midi_file == NULL) || player_state == _MIDI_PLAYER_STATE_STOPPED)
	{
		return;
	}

	switch (player_state)
	{
		case _MIDI_PLAYER_STATE_INIT_STOP:
		case _MIDI_PLAYER_STATE_INIT_PAUSE:
		case _MIDI_PLAYER_STATE_PLAYING:
			/* Wait for timer to finish */
			player_state = _MIDI_PLAYER_STATE_INIT_STOP;
			do_stop();
			break;
		
		case _MIDI_PLAYER_STATE_PAUSED:
			paused = false;
			do_stop();
			break;
	}
}

void MidiPlaybackThread::go_backward()
{
	// Early return conditions
	if (midi_file == NULL || seek_in_progress)
	{
		return;
	}

	// Lock seeking
	seek_in_progress = true;

	// Remember if we were playing
	bool was_playing = (player_state == _MIDI_PLAYER_STATE_PLAYING);

	// Pause playback first if currently playing
	if (was_playing)
	{
		pause_playing();
		
		// Wait for pause to complete
		int wait_count = 0;
		while (player_state != _MIDI_PLAYER_STATE_PAUSED && wait_count < 50)
		{
			usleep(10000); // Wait 10ms
			wait_count++;
		}
	}

	const double skip_duration_ms = 5000.0; // 5 seconds
	const double skip_pulses = skip_duration_ms * pulses_per_msec;

	// Calculate target pulse time
	double target_pulse_time = current_pulse_time - skip_pulses;

	// Don't go before the beginning
	if (target_pulse_time < 0)
	{
		target_pulse_time = 0;
	}

	// Search BACKWARDS from current position for efficiency
	const std::vector<MidiFileEvent> &file_events = midi_file->get_combined_track();
	int target_index = 0;

	for (int i = next_played_event_index; i >= 0; i--)
	{
		if (file_events[i].start_time <= target_pulse_time)
		{
			target_index = i;
			break; // Found the closest event, stop searching
		}
	}

	// Clear any playing notes
	if (clear_all_playing_notes_callback_ptr != NULL)
	{
		clear_all_playing_notes_callback_ptr();
	}

	// Update position
	next_played_event_index = target_index;
	current_pulse_time = target_pulse_time;
	prev_pulse_time = target_pulse_time;

	// Recalculate timing to maintain playback continuity
	long current_time_ms = get_current_time_ms();
	long target_elapsed_ms = (long)(target_pulse_time / pulses_per_msec);

	// Adjust start time as if we had been playing from the new position
	start_playing_time_ms = current_time_ms - target_elapsed_ms - pause_duration_time_msec;

	//fprintf(stderr,
	//	"Jumped backward to %.2fs (index %d)%s\n",
	//	target_elapsed_ms / 1000.0,
	//	target_index,
	//	was_playing ? " - will resume" : "");

	// Resume playback if it was playing before
	if (was_playing)
	{
		start_playing();
	}

	// Unlock seeking
	seek_in_progress = false;
}

void MidiPlaybackThread::go_forward()
{
	// Early return conditions
	if (midi_file == NULL || seek_in_progress)
	{
		return;
	}

	// Lock seeking
	seek_in_progress = true;

	// Remember if we were playing
	bool was_playing = (player_state == _MIDI_PLAYER_STATE_PLAYING);

	// Pause playback first if currently playing
	if (was_playing)
	{
		pause_playing();
		
		// Wait for pause to complete
		int wait_count = 0;
		while (player_state != _MIDI_PLAYER_STATE_PAUSED && wait_count < 50)
		{
			usleep(10000); // Wait 10ms
			wait_count++;
		}
	}

	const double skip_duration_ms = 5000.0; // 5 seconds
	const double skip_pulses = skip_duration_ms * pulses_per_msec;

	// Calculate target pulse time
	double target_pulse_time = current_pulse_time + skip_pulses;
    
	// Don't go past the end
	if (target_pulse_time > midi_file->get_total_pulses())
	{
		target_pulse_time = midi_file->get_total_pulses();
	}

	// Search FORWARD from current position
	const std::vector<MidiFileEvent> &file_events = midi_file->get_combined_track();
	int target_index = next_played_event_index;
    
	for (int i = next_played_event_index; i < file_events.size(); i++)
	{
		if (file_events[i].start_time <= target_pulse_time)
		{
			target_index = i;
		}
		else
		{
			break; // Passed the target, stop searching
		}
	}

	// Clear any playing notes
	if (clear_all_playing_notes_callback_ptr != NULL)
	{
		clear_all_playing_notes_callback_ptr();
	}

	// Update playback position
	next_played_event_index = target_index;
	current_pulse_time = target_pulse_time;
	prev_pulse_time = target_pulse_time;

	fprintf(stderr, "FORWARD: target_pulse=%f, total_pulses=%d, index=%d, total_events=%zu\n",
			target_pulse_time,
			midi_file->get_total_pulses(),
			target_index,
			file_events.size());
    
	// Adjust timing references
	long current_time_ms = get_current_time_ms();
	long elapsed_time_ms = (long)(target_pulse_time / pulses_per_msec);
	start_playing_time_ms = current_time_ms - elapsed_time_ms - pause_duration_time_msec;

	//fprintf(stderr,
	//	"Jumped forward to %.2fs (index %d)%s\n",
	//	elapsed_time_ms / 1000.0,
	//	target_index,
	//	was_playing ? " - will resume" : "");

	// Resume playback if it was playing before
	if (was_playing)
	{
		start_playing();
	}

	// Unlock seeking
	seek_in_progress = false;
}

void MidiPlaybackThread::set_auto_loop_back(bool auto_loop_back)
{
	auto_loop_back_on = auto_loop_back;
}
bool MidiPlaybackThread::is_auto_loop_back_on()
{
	return auto_loop_back_on;
}

void MidiPlaybackThread::do_play()
{	
	if (paused)
	{
		start_pulse_time = current_pulse_time;
		pause_stop_time_msec = get_current_time_ms();
		pause_duration_time_msec += (pause_stop_time_msec - pause_start_time_msec);
	}
	else
	{
		start_pulse_time = 0;   
		current_pulse_time = 0; 
		prev_pulse_time = 0;
		start_playing_time_ms = get_current_time_ms();
		next_played_event_index = 0;
	}

	start_play_midi_file_events();

	start_update_thread();

	return;
}

/* Performs the actual stop, by stopping the playback
 *  and clearing the state. */
void MidiPlaybackThread::do_stop()
{
	pthread_mutex_lock(&state_mutex);
	player_state = _MIDI_PLAYER_STATE_STOPPED;
	start_pulse_time = 0;
	current_pulse_time = 0;
	prev_pulse_time = 0;
	pthread_mutex_unlock(&state_mutex);

	stop_play_midi_file_events();
	stop_update_thread();

	// Wait for threads to actually exit
	wait_for_threads_to_exit();
}

/** Performs the actual start of the MIDI events playback  */
void MidiPlaybackThread::start_play_midi_file_events()
{

	play_midi_events_in_progress = true;
	events_play_state_changed = true;

	if (!paused)
	{
		// Init Playing
		start_playing_time_ms = get_current_time_ms();
		pause_duration_time_msec = 0;
		next_played_event_index = 0;
	}
	else
	{
		paused = false;
	}

	player_state = _MIDI_PLAYER_STATE_PLAYING;
	start_playback_thread();
	start_update_thread();
}

// TODO:
/* Perform the actual stop of the MIDI events playback,
 *  clear any leftovers. */
void MidiPlaybackThread::stop_play_midi_file_events()
{
	/* Clear leftovers */
	if (clear_all_playing_notes_callback_ptr != NULL)
	{
		clear_all_playing_notes_callback_ptr();
	}
	play_midi_events_in_progress = false;
	events_play_state_changed = true;
	stop_playback_thread();
	stop_update_thread();
}

void MidiPlaybackThread::start_playback_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;

	pthread_mutex_lock(&state_mutex);

	// Verify that the playback thread is not already running
	if (playback_thread_is_running)
	{
		pthread_mutex_unlock(&state_mutex);
		return;
	}

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_PLAYBACK;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);

	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting MIDI Playback thread realtime prio\n");
	}

	// Set the control flag BEFORE creating thread
	playback_thread_is_running = true;

	// Create and start the thread
	ret = pthread_create(&playback_thread_id, &tattr, playback_thread, (void *)100);

	if (ret != 0)
	{
		fprintf(stderr, "Failed to create playback thread: %d\n", ret);
		playback_thread_is_running = false;
		pthread_mutex_unlock(&state_mutex);
		return;
	}

	char thname[] = {"midi_playback_thread"};
	pthread_setname_np(playback_thread_id, &thname[0]);

	pthread_mutex_unlock(&state_mutex);
}

void MidiPlaybackThread::stop_playback_thread()
{
	pthread_mutex_lock(&state_mutex);
	playback_thread_is_running = false;
	pthread_mutex_unlock(&state_mutex);
}

void *MidiPlaybackThread::playback_thread(void *thread_id)
{
	MidiFileEvent current_event;
	long actual_play_time_msec;
	std::vector<MidiFileEvent> file_events;

	// Loop until stopped
	while (playback_thread_is_running)
	{
		/* Any change in playback state? */
		if (events_play_state_changed)
		{
			// State changed
			events_play_state_changed = false; // Only once
		}
		else if (play_midi_events_in_progress)
		{
			/* Playing. Get 1st event.
             * Collect all events that should be played with the current event 	*/
			vector<MidiFileEvent> events;
			file_events = midi_file->get_combined_track();

			/** Recover any MIDI events that were modified by the event processor - TODO: */

			/* Get 1st event */
			current_event = file_events.at(next_played_event_index);
			events.push_back(current_event);
			//next_played_event_index++; // Repeat in loop bellow? TODO:
			/* Start looping through all events */
			while ((next_played_event_index < midi_file->get_combined_track().size()) && // not last event
				   play_midi_events_in_progress) // Check for valid event? TODO:
			{ 
				/* Still running */
				/* Get next event (needs the 1st again? TODO:*/
				current_event = file_events.at(next_played_event_index);
				/* Trap Program Change messages */
				if (current_event.event_command == _MIDI_EVENT_PROGRAM_CHANGE)
				{
					if (midi_change_program_callback_ptr != NULL)
					{
						midi_change_program_callback_ptr(current_event.channel, current_event.instrument);
					}
				}
				/* Trap Set Channel Volume messages */
				else if ((current_event.event_command == _MIDI_EVENT_CONTROL_CHANGE) &&
						 (current_event.control_num == _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2))
				{
					uint8_t chan = current_event.channel;
					uint8_t vol = current_event.control_value;
					if ((chan >= 0) && (chan < 16) && (vol >= 0) && (vol < 128))
					{
						if (midi_change_channel_volume_callback_ptr != NULL)
						{
							midi_change_channel_volume_callback_ptr(chan, vol);
						}
					}
				}
				// Trap Note On events for controling the volume of the played notes
				else if ((current_event.event_command == _MIDI_EVENT_NOTE_ON) &&
						 (current_event.velocity > 0))
				{
					current_event.velocity = (uint8_t)((current_event.velocity * playback_volume) / 100);
				}
				/* Calculate elapsed playing time from start-playing time (ref as zero) */
				actual_play_time_msec = (long)(get_current_time_ms() - start_playing_time_ms - pause_duration_time_msec);
				current_pulse_time = actual_play_time_msec * pulses_per_msec * speed;
				/* Is it time to send next MIDI event(s) */
				if (current_pulse_time >= events.at(0).start_time)
				{
					/* Collect all events that should be played with the current event */
					/* Is it a close (in time) event? */
					if (/*(currentEvent != null) TODO: check event is OK  && */
						(events.size() <= 3) &&
						((current_event.start_time - events.at(0).start_time) < 
							 (midi_file->get_time_signature()->get_quarter() / 128))) // 1/128 resolution
					{
						events.push_back(current_event);
						next_played_event_index++;
					}
					else
					{
						/* no, just send what we have until now */
						/*events = mMIDIeventsProcessor.eventsProcessor((ArrayList<MidiFileEvent>)vEvents.clone(),
																	   device.getPlayingMode(),
																	   PlaybackMIDIeventsProcessor.iOriginalEvents,
																	   Playing.getLastPlayedCord(),
																	   device.getPlayingMidiChannel(),
																	   Arpeggios.getVariationMode(),
																	   device.getPlayingVolume());
						*/

						if (send_midi_events_vector_callback_ptr != NULL)
						{
							send_midi_events_vector_callback_ptr(events, playback_volume);
						}
						/* Clear and add the new */
						events.clear();
						events.push_back(current_event);
						next_played_event_index++;
					}					
				}
				
				usleep(1000);

			} // while ((next_played_event_index < midi_file->get_combined_track().size())

			if (!auto_loop_back_on)
			{
				/* Not in loop mode */
				stop_play_midi_file_events();
				player_state = _MIDI_PLAYER_STATE_STOPPED;
				InstrumentMidiPlayer::get_instrument_midi_player_instance()->send_all_notes_off_command();
				InstrumentMidiPlayer::get_instrument_midi_player_instance()->send_all_sounds_off_command();
			}
			else
			{
				/* Loop is ON */
				start_playing_time_ms = (long)get_current_time_ms();
				pause_duration_time_msec = 0;
				next_played_event_index = 0;
				/** Recover any MIDI events that were modified by the event processor */
				/*
					 *mMIDIeventsProcessor.eventsProcessor(midiFile.getCombinedTrack(),
														 device.getPlayingMode(),
														 PlaybackMIDIeventsProcessor.iProcessedEvents,
														 Playing.getLastPlayedCord(),
														 device.getPlayingMidiChannel(),
														 Arpeggios.getVariationMode(),
														 device.getPlayingVolume());
					*/
			}

		} // else if (play_midi_events_in_progress)


	} // while (thread_is_running)

	return 0;
}

void MidiPlaybackThread::start_update_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;

	pthread_mutex_lock(&state_mutex);

	// Verify that the update thread is not already running
	if (update_thread_is_running)
	{
		pthread_mutex_unlock(&state_mutex);
		return;
	}

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_PLAYBACK;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);

	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting MIDI Playback Update thread realtime prio\n");
	}

	// Set the control flag BEFORE creating thread
	update_thread_is_running = true;

	// Create and start the thread
	ret = pthread_create(&update_thread_id, &tattr, update_thread, (void *)100);

	if (ret != 0)
	{
		fprintf(stderr, "Failed to create update thread: %d\n", ret);
		update_thread_is_running = false;
		pthread_mutex_unlock(&state_mutex);
		return;
	}

	char thname[] = {"midi_playback_update_thread"};
	pthread_setname_np(update_thread_id, &thname[0]);

	pthread_mutex_unlock(&state_mutex);
}
void MidiPlaybackThread::stop_update_thread()
{
	pthread_mutex_lock(&state_mutex);
	update_thread_is_running = false;
	pthread_mutex_unlock(&state_mutex);
}

bool MidiPlaybackThread::is_update_thread_running()
{
	return update_thread_is_running;
}

void MidiPlaybackThread::wait_for_threads_to_exit()
{
	pthread_t playback_tid = 0;
	pthread_t update_tid = 0;
	bool wait_playback = false;
	bool wait_update = false;

	pthread_mutex_lock(&state_mutex);

	if (playback_thread_is_running && playback_thread_id != 0)
	{
		playback_tid = playback_thread_id;
		wait_playback = true;
	}

	if (update_thread_is_running && update_thread_id != 0)
	{
		update_tid = update_thread_id;
		wait_update = true;
	}

	pthread_mutex_unlock(&state_mutex);

	// Wait outside the mutex to avoid deadlock
	if (wait_playback)
	{
		pthread_join(playback_tid, NULL);
	}

	if (wait_update)
	{
		pthread_join(update_tid, NULL);
	}
}

void *MidiPlaybackThread::update_thread(void *thread_id)
{
	long lmsec = 0;
	long local_start_time;
	long local_pause_duration;

	while (update_thread_is_running)
	{
		pthread_mutex_lock(&state_mutex);

		if (midi_file == NULL)
		{
			player_state = _MIDI_PLAYER_STATE_STOPPED;
			update_thread_is_running = false;
			pthread_mutex_unlock(&state_mutex);
			return 0;
		}

		// Copy timing values while holding lock
		local_start_time = start_playing_time_ms;
		local_pause_duration = pause_duration_time_msec;
		int local_player_state = player_state;
		double local_current_pulse = current_pulse_time;

		pthread_mutex_unlock(&state_mutex);

		lmsec = (long)(get_current_time_ms() - (local_start_time + local_pause_duration));

		switch (local_player_state)
		{
		case _MIDI_PLAYER_STATE_STOPPED:
		case _MIDI_PLAYER_STATE_PAUSED:
			break;

		case _MIDI_PLAYER_STATE_INIT_STOP:
			break;

		case _MIDI_PLAYER_STATE_PLAYING: {
			pthread_mutex_lock(&state_mutex);

			//pulses_per_msec = pulses_per_msec * speed; // should not be updated every event
			prev_pulse_time = current_pulse_time;

			file_position = (int)(current_pulse_time / midi_file->get_total_pulses() * 100);

			remaining_file_play_msec = (int)(midi_file->get_total_pulses() / (pulses_per_msec * speed));

			//static int debug_count = 0;
			//if (debug_count++ % 10 == 0) // Print every 10th time
			//{
			//	fprintf(stderr, "DEBUG: total_pulses=%d, ppm=%f, speed=%f, total_ms=%d (%d:%02d)\n",
			//			midi_file->get_total_pulses(),
			//			pulses_per_msec,
			//			speed,
			//			remaining_file_play_msec,
			//			remaining_file_play_msec / 60000,
			//			(remaining_file_play_msec / 1000) % 60);
			//}

			pthread_mutex_unlock(&state_mutex);

			if (midi_player_potision_update_callback_ptr != NULL)
			{
				midi_player_potision_update_callback_ptr(file_position);
			}

			// Calculate times (using local copy of lmsec to avoid corruption)
			file_total_playing_time_seconds = (int)((remaining_file_play_msec / 1000) % 60);
			file_total_playing_time_minutes = (int)((remaining_file_play_msec / 1000) / 60);

			if (midi_player_file_total_time_update_callback_ptr != NULL)
			{
				midi_player_file_total_time_update_callback_ptr(file_total_playing_time_minutes, file_total_playing_time_seconds);
			}

			//fprintf(stderr, "Playing Time: %i : %i\n", file_playing_time_minutes, file_playing_time_seconds);
			
			/* Update Playing time */
			file_playing_time_seconds = (int)((lmsec / 1000) % 60);
			file_playing_time_minutes = (int)((lmsec / 1000) / 60);

			if (midi_player_playing_time_update_callback_ptr != NULL)
			{
				midi_player_playing_time_update_callback_ptr(file_playing_time_minutes, file_playing_time_seconds);
			}

			//pthread_mutex_lock(&state_mutex);
			//remaining_file_play_msec = (int)(midi_file->get_total_pulses() / (pulses_per_msec * speed));
			//pthread_mutex_unlock(&state_mutex);

			remaining_file_play_sec = (int)(((remaining_file_play_msec - lmsec) / 1000) % 60);
			remaining_file_play_min = (int)(((remaining_file_play_msec - lmsec) / 1000) / 60);

			if (midi_player_file_remaining_time_update_callback_ptr != NULL)
			{
				midi_player_file_remaining_time_update_callback_ptr(remaining_file_play_min, remaining_file_play_sec);
			}

			//file_total_playing_time_seconds = (int)((remaining_file_play_msec / 1000) % 60);
			//file_total_playing_time_minutes = (int)(((remaining_file_play_msec / 1000) - file_total_playing_time_seconds) / 60);
			//
			//if (midi_player_file_total_time_update_callback_ptr != NULL)
			//{
			//	midi_player_file_total_time_update_callback_ptr(file_total_playing_time_minutes, file_total_playing_time_seconds);
			//}

			pthread_mutex_lock(&state_mutex);

			if (current_pulse_time > midi_file->get_total_pulses())
			{
				if (auto_loop_back_on)
				{
					pthread_mutex_unlock(&state_mutex);
					stop_play_midi_file_events();
					do_play();
					pthread_mutex_lock(&state_mutex);
				}
				else
				{
					player_state = _MIDI_PLAYER_STATE_INIT_STOP;
					pthread_mutex_unlock(&state_mutex);
					stop_playing();
					break;
				}
			}

			pthread_mutex_unlock(&state_mutex);
			break;
		}

		case _MIDI_PLAYER_STATE_INIT_PAUSE: {
			pthread_mutex_lock(&state_mutex);
			pause_start_time_msec = get_current_time_ms();
			prev_pulse_time = current_pulse_time;
			paused = true;
			player_state = _MIDI_PLAYER_STATE_PAUSED;
			pthread_mutex_unlock(&state_mutex);

			stop_play_midi_file_events();
			break;
		}
		}

		usleep(100000);
	}

	return 0;
}
