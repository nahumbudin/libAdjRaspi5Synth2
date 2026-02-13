/**
*	@file		instrumenyMidiPlayer.cpp
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

#include "instrumentMidiPlayer.h"
#include "../Misc/priorities.h"
#include "../LibAPI/defines.h"
#include "../MIDI/midiConversions.h"

bool InstrumentMidiPlayer::file_initialized;
int InstrumentMidiPlayer::playback_Volume;
int InstrumentMidiPlayer::playback_relative_speed;
std::string InstrumentMidiPlayer::file_title;
std::string InstrumentMidiPlayer::file_path;
MidiFile *InstrumentMidiPlayer::midi_file;
double InstrumentMidiPlayer::pulses_per_msec = 100; // TODO:
bool InstrumentMidiPlayer::notes_on[16][128];
vector<uint8_t> InstrumentMidiPlayer::file_data_bytes;

func_ptr_void_void_t InstrumentMidiPlayer::clear_all_playing_notes_callback_ptr = NULL;
func_ptr_void_uint8_t_uint8_t InstrumentMidiPlayer::midi_change_program_callback_ptr = NULL;
func_ptr_void_uint8_t_uint8_t InstrumentMidiPlayer::midi_change_channel_volume_callback_ptr = NULL;
func_ptr_void_midi_events_vector_int InstrumentMidiPlayer::send_midi_events_vector_callback_ptr = NULL;
func_ptr_void_int_t InstrumentMidiPlayer::midi_player_potision_update_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiPlayer::midi_player_total_playing_time_update_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiPlayer::midi_player_playing_time_update_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiPlayer::midi_player_file_remaining_time_update_callback_ptr = NULL;

InstrumentMidiPlayer *InstrumentMidiPlayer::instrument_midi_player_instance = NULL;

void send_midi_commands_wrapper(std::vector<MidiFileEvent> events, int vol){
	
	InstrumentMidiPlayer::get_instrument_midi_player_instance()->send_midi_commands(events, vol);
}

InstrumentMidiPlayer::InstrumentMidiPlayer(AlsaMidiSysControl *alsa_control,
										   std::string *alsa_client_in_name)
	: Instrument(_INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY, false, false, true,
				 alsa_control, alsa_client_in_name)
{

	instrument_midi_player_instance = this;

	midi_file = NULL;

	file_initialized = false;
	playback_Volume = 75;
	playback_relative_speed = 100;

	//player_speed = 1.0f;
}

InstrumentMidiPlayer::~InstrumentMidiPlayer()
{
	terminate();
}

void InstrumentMidiPlayer::terminate()
{
	// Stop threads.
	if (midi_playback_threads != NULL)
	{
		if (midi_playback_threads->is_player_playing())
		{
			midi_playback_threads->stop_playing();
		}

		//delete midi_playback_thread;
	}
}

InstrumentMidiPlayer *InstrumentMidiPlayer::get_instrument_midi_player_instance()
{
	return instrument_midi_player_instance;
}

void InstrumentMidiPlayer::register_clear_all_playing_notes_callback(func_ptr_void_void_t ptr)
{
	clear_all_playing_notes_callback_ptr = ptr;
}

void InstrumentMidiPlayer::register_midi_change_program_callback(func_ptr_void_uint8_t_uint8_t ptr)
{
	midi_change_program_callback_ptr = ptr;
}

void InstrumentMidiPlayer::register_midi_change_channel_volume_callback(func_ptr_void_uint8_t_uint8_t ptr)
{
	midi_change_channel_volume_callback_ptr = ptr;
}

void InstrumentMidiPlayer::register_send_midi_events_vector_callback(func_ptr_void_midi_events_vector_int ptr)
{
	send_midi_events_vector_callback_ptr = ptr;
}

void InstrumentMidiPlayer::register_midi_player_potision_update_callback(func_ptr_void_int_t ptr)
{
	midi_player_potision_update_callback_ptr = ptr;
}

void InstrumentMidiPlayer::register_midi_player_total_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_player_total_playing_time_update_callback_ptr = ptr;
}

void InstrumentMidiPlayer::register_midi_player_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_player_playing_time_update_callback_ptr = ptr;
}

void InstrumentMidiPlayer::register_midi_player_song_remaining_time_update_callback(func_ptr_void_int_int_t ptr)
{
	midi_player_file_remaining_time_update_callback_ptr = ptr;
}

/* Used for initiating a new file */
int InstrumentMidiPlayer::init_new_file(std::string path, std::string title)
{
	file_title = title;
	file_path = path;

	file_data_bytes.clear();
	midi_file = new MidiFile(path);
	midi_file->open_midi_file(path, &file_data_bytes);

	/* Terminate all running playback threads. */
	terminate();

	if (!file_data_bytes.empty() && file_data_bytes.size() >= 6 && has_midi_header(file_data_bytes))
	{
		/* Build a new MidiFile from the file data */
		if (midi_file != NULL)
		{
			delete midi_file;
		}
		midi_file = new MidiFile(file_data_bytes, path, false, false);

		double inverse_tempo = 1.0 / midi_file->get_time_signature()->get_tempo();
		double inverse_tempo_scaled = inverse_tempo *  100.0 /*playerOptions.tempo */ / 100.0;
		double tempo_usq = (int)(1.0 / inverse_tempo_scaled);
		pulses_per_msec = midi_file->get_time_signature()->get_quarter() * (1000.0 / tempo_usq);
		midi_playback_threads->set_pulses_per_ms(pulses_per_msec);

		//int result = processFile(bFileDataBytes, strFileTitle, strFilePath);

		// Intilize the player
		//initializePlayer(bFileDataBytes, strFileTitle, strFilePath,
		//					playerOptions.autoScale, playerOptions.autoDrumsCount);

		init_new_playback_thread();
		
		return 0;
	}
	else
	{
		printf("MIDI Player - MIDI File ERROR");
	}

	return -1;
}

int InstrumentMidiPlayer::init_new_playback_thread()
{
	register_send_midi_events_vector_callback(&send_midi_commands_wrapper); // TODO: here?

	midi_playback_threads = new MidiPlaybackThread(
		midi_file,
		clear_all_playing_notes_callback_ptr,
		midi_change_program_callback_ptr,
		midi_change_channel_volume_callback_ptr,
		send_midi_events_vector_callback_ptr,
		midi_player_potision_update_callback_ptr,
		midi_player_total_playing_time_update_callback_ptr,
		midi_player_playing_time_update_callback_ptr,
		midi_player_file_remaining_time_update_callback_ptr);

	return 0;
}

void InstrumentMidiPlayer::play()
{
	midi_playback_threads->start_playing();
}

void InstrumentMidiPlayer::pause()
{
	midi_playback_threads->pause_playing();
	set_off_all_on_notes();
	send_all_notes_off_command();
	send_all_sounds_off_command();
}

void InstrumentMidiPlayer::stop()
{
	midi_playback_threads->stop_playing();
	set_off_all_on_notes();
	send_all_notes_off_command();
	send_all_sounds_off_command();
}

/* Set all played on notes */
void InstrumentMidiPlayer::set_off_all_on_notes()
{
	uint8_t command[3];
	int status;
	command[2] = 0;

	for (int c = 0; c < 16; c++)
	{
		for (int n = 0; n < 128; n++)
		{
			if (notes_on[c][n])
			{
				command[0] = ((_MIDI_EVENT_NOTE_OFF << 4) & 0xf0) + c;
				command[1] = n;

				if ((status = snd_rawmidi_write(midiout, command, 3)) < 0)
				{
					printf("Problem writing midi player note off to MIDI output: %s", snd_strerror(status));
					//					exit(1);
				}
			}
		}
	}
}

void InstrumentMidiPlayer::send_all_notes_off_command()
{
	uint8_t command[3];
	int status;

	for (int c = 0; c < 16; c++)
	{
		command[0] = _MIDI_EVENT_CONTROL_CHANGE + c;
		command[1] = _MIDI_ALL_NOTES_OFF;
		command[2] = 0;

		if ((status = snd_rawmidi_write(midiout, command, 3)) < 0)
		{
			printf("Problem writing midi all note off to MIDI output: %s", snd_strerror(status));
			//					exit(1);
		}
	}
}

void InstrumentMidiPlayer::send_all_sounds_off_command()
{
	uint8_t command[3];
	int status;

	for (int c = 0; c < 16; c++)
	{
		command[0] = _MIDI_EVENT_CONTROL_CHANGE + c;
		command[1] = _MIDI_ALL_SOUNDS_OFF;
		command[2] = 0;

		if ((status = snd_rawmidi_write(midiout, command, 3)) < 0)
		{
			printf("Problem writing midi all sounds off to MIDI output: %s", snd_strerror(status));
			//					exit(1);
		}
	}
}

void InstrumentMidiPlayer::send_midi_commands(std::vector<MidiFileEvent> events, int vol)
{
	int status;
	uint8_t bytes[512] = {0};
	bool res;
	std::vector<uint8_t> bytes_vector;

	/* Go over all commands */
	for (MidiFileEvent event : events)
	{
		if ((event.event_command == _MIDI_EVENT_NOTE_OFF) ||
			(event.event_command == _MIDI_EVENT_NOTE_ON))
		{
			/* Mark notes ON; clear marked notes OFF - clera remaining notes ON when stop/pause */
			if ((event.channel < 16) && (event.channel >= 0) &&
				(event.note_number < 128) && (event.note_number > 0))
			{
				if ((event.event_command == _MIDI_EVENT_NOTE_ON) && (event.velocity != 0))
				{
					notes_on[event.channel][event.note_number] = true;
				}
				else
				{
					notes_on[event.channel][event.note_number] = false;
				}
			}
		}

		res = convert_midi_event_to_mssg(event, &bytes_vector, playback_Volume);

		if (res && (bytes_vector.size() > 0))
		{
			for (int i = 0; i < bytes_vector.size(); i++)
			{
				bytes[i] = bytes_vector.at(i);
			}

			if ((status = snd_rawmidi_write(midiout, bytes, bytes_vector.size())) < 0)
			{
				printf("Problem writing midi player data to MIDI output: %s", snd_strerror(status));
				//					exit(1);
			}
		}
	}

	

	// printf("Playback num of commands: %i\n", (int)events.size());
}

/*Returns true if the data starts with the header MTrk */
bool InstrumentMidiPlayer::has_midi_header(vector<uint8_t> data)
{
	if ((data.at(0) == 'M') && (data.at(1) == 'T') &&
		(data.at(2) == 'h') && (data.at(3) == 'd'))
	{
		return true;
	}
	else
	{
		return false;
	}
}







