/**
*	@file		instrumenyMidiPlayerPlaybackTread.cpp
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
	
	clear_all_playing_notes_callback_ptr = clr_all_playing_notes_callback_ptr;
	midi_change_program_callback_ptr = midi_change_prog_callback_ptr;
	midi_change_channel_volume_callback_ptr = midi_change_chan_volume_callback_ptr;
	send_midi_events_vector_callback_ptr = send_midi_events_vec_ptr;
	midi_player_potision_update_callback_ptr = midi_player_pos_update_callback_ptr;
	midi_player_playing_time_update_callback_ptr = midi_player_play_time_update_callback_ptr;
	midi_player_file_remaining_time_update_callback_ptr = midi_player_file_rem_time_update_callback_ptr;
	midi_player_file_total_time_update_callback_ptr = midi_player_tot_play_time_update_callback_ptr;

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

void MidiPlaybackThread::set_playback_speed(float spd)
{
	speed = spd	;
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
	player_state = _MIDI_PLAYER_STATE_STOPPED;
	start_pulse_time = 0;
	current_pulse_time = 0;
	prev_pulse_time = 0;
	stop_play_midi_file_events();
	stop_update_thread(); // TODO: ?
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
	// Verify that the playback thread is not allready running
	if (playback_thread_is_running)
	{
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
	//	err = errno; // for debug
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting MIDI Playback thread realtime prio\n");
	}
	// Set the control flag
	playback_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&playback_thread_id, &tattr, playback_thread, (void *)100);
	char thname[] = {"midi_playback_thread"};
	pthread_setname_np(playback_thread_id, &thname[0]);
}

void MidiPlaybackThread::stop_playback_thread()
{
	playback_thread_is_running = false;
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
				/* Calculate elapsed playing time from start-playing time (ref as zero) */
				actual_play_time_msec = (long)(get_current_time_ms() - start_playing_time_ms - pause_duration_time_msec);
				current_pulse_time = actual_play_time_msec * pulses_per_msec;
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
	// Verify that the update thread is not allready running
	if (update_thread_is_running)
	{
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
	//	err = errno; // for debug
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting MIDI Playback Update thread realtime prio\n");
	}
	// Set the control flag
	update_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&update_thread_id, &tattr, update_thread, (void *)100);
	char thname[] = {"midi_playback_update_thread"};
	pthread_setname_np(update_thread_id, &thname[0]);
}

void MidiPlaybackThread::stop_update_thread()
{
	update_thread_is_running = false;
}

bool MidiPlaybackThread::is_update_thread_running()
{
	return update_thread_is_running;
}

void *MidiPlaybackThread::update_thread(void *thread_id)
{
	/* The periodic update.
     *  If the MIDI file is still playing, update the current_pulse_time.
     *  If a stop or pause has been initiated (by someone clicking
     *  the stop or pause button), than stop the timer.
     */
	long lmsec = 0;

	while (update_thread_is_running)
	{

		if (midi_file == NULL)
		{
			player_state = _MIDI_PLAYER_STATE_STOPPED;
			update_thread_is_running = false;
			return 0;
		}
		else
		{
			lmsec = (long)(get_current_time_ms() - (start_playing_time_ms + pause_duration_time_msec));
		}

		switch (player_state)
		{
			case _MIDI_PLAYER_STATE_STOPPED:
			case _MIDI_PLAYER_STATE_PAUSED:
				/* This case should never happen */
				break;

			case _MIDI_PLAYER_STATE_INIT_STOP:
				break;

			case _MIDI_PLAYER_STATE_PLAYING:
				/* Update progress */
				pulses_per_msec = pulses_per_msec * speed;
				prev_pulse_time = current_pulse_time;

				/* Update progress bar */
				file_position = (int)(current_pulse_time / midi_file->get_total_pulses() * 100);

				if (midi_player_potision_update_callback_ptr != NULL)
				{
					midi_player_potision_update_callback_ptr(file_position);
				}

				/* Update Playing time */
				file_playing_time_seconds = ((int)lmsec / 1000) % 60;
				file_playing_time_minutes = (((int)lmsec / 1000) - file_playing_time_seconds) / 60;
			
				printf("Playing Time: %i : %i\n", file_playing_time_minutes, file_playing_time_seconds);
			
				if (midi_player_playing_time_update_callback_ptr != NULL)
				{
					midi_player_playing_time_update_callback_ptr(file_playing_time_minutes, file_playing_time_seconds);
				}
				/* Update Remaining Playing time */
				remaining_file_play_msec = (int)(midi_file->get_total_pulses() / pulses_per_msec);
				remaining_file_play_sec = (int)(((remaining_file_play_msec - lmsec) / 1000) % 60);
				remaining_file_play_min = (int)((((remaining_file_play_msec - lmsec) / 1000) - remaining_file_play_sec) / 60);

				if (midi_player_file_remaining_time_update_callback_ptr != NULL)
				{
					midi_player_file_remaining_time_update_callback_ptr(remaining_file_play_min, remaining_file_play_sec);
				}
				/* Update Total  Playing time */

				file_total_playing_time_seconds = (int)((remaining_file_play_msec / 1000) % 60);
				file_total_playing_time_minutes = (int)(((remaining_file_play_msec / 1000) - file_total_playing_time_seconds) / 60);

				if (midi_player_file_total_time_update_callback_ptr != NULL)
				{
					midi_player_file_total_time_update_callback_ptr(file_total_playing_time_minutes, file_total_playing_time_seconds);
				}
				
				/* Stop or loop if we've reached the end of the song */

				if (current_pulse_time > midi_file->get_total_pulses())
				{
					/* Enf of file */
					if (auto_loop_back_on)
					{
						stop_play_midi_file_events();
						do_play();
					}
					else
					{
						player_state = _MIDI_PLAYER_STATE_INIT_STOP;
						stop_playing();
						break;
					}
				}
			
				break;

			case _MIDI_PLAYER_STATE_INIT_PAUSE:

				pause_start_time_msec = get_current_time_ms();

				/* Player sends MIDI commands out through BT serial service */
				// TODO:  stop playing MIDI events
				stop_play_midi_file_events();

				prev_pulse_time = current_pulse_time;

				paused = true;
				player_state = _MIDI_PLAYER_STATE_PAUSED;

				break;
		}

		usleep(100000);
		//printf("Update thread timer\n");
	}

	return 0;
}
