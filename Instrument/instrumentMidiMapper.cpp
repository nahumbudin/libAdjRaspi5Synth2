/**
 *	@file		instrumenyMidiMapper.cpp
 *	@author		Nahum Budin
 *	@date		27-Jan-2026
 *	@version	1.1
 *					1. Added MIDI mixer Pan modulation LFO level and selection control handling
 *					2. Added MIDI mixer Send control handling
 *					3. Added mutex to protect all snd_rawmidi_write() calls.
*
*	@brief		MIDI mapper instrument.
*				It processes incoming MIDI events and generates control events for the synth. 
*				It can be used to filter or modify events prior to sending them to the synthesizer modules. 
*				In default state it is transparent and simply passes all MIDI events.
*				
*				Major capabilities:
*				
*					- Capture messages and activate callbacks (e.g. change program).
*					- Block messages (e.g. block channel pressure or aftertouch/pressure commands).
*					- Split the keyboard (e.g note < N -> ch1; note >= N -> ch2)
*					- Set channel volume (e.g. scale chanel notes velocity by 0.5)
*
*	History:\n
*			version 1.0	23-Aug-2024	Initial
*
*	Based on my Android TilTune Player Java Project 
*
*/

#include "instrumentMidiMapper.h"
#include "../LibAPI/midi.h"

InstrumentMidiMapper *InstrumentMidiMapper::instrument_midi_mapper_instance = NULL;

func_ptr_void_int_int_t InstrumentMidiMapper::midi_channel_volume_control_command_trap_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiMapper::midi_channel_change_program_command_trap_callback_ptr = NULL;
func_ptr_void_int_bool_t InstrumentMidiMapper::midi_channel_active_indication_callback_ptr = NULL;

InstrumentMidiMapper::InstrumentMidiMapper(AlsaMidiSysControl *alsa_control,
										   std::string *alsa_client_in_name)
	: Instrument(_INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY, true, false, true,		// midi in, no audio out, midi out
				 alsa_control, alsa_client_in_name, AdjSynth::get_instance(), NULL, _INSTRUMENT_TYPE_CONTROL)
{

	keyboard_split_note = 64;

	for (int ch = 0; ch < 16; ch++)
	{
		midi_channel_volume_scale[ch] = 1.0f; // Future option TODO:?
		midi_channel_block_channel_volume_commands[ch] = false;
	}

	instrument_midi_mapper_instance = this;
	/* Midi mapper input handles all channels */
	this->alsa_midi_sequencer_events_handler->set_active_midi_channels(0xffff);
	
	this->alsa_midi_sequencer_events_handler->update_active_midi_channels_on_channel_change = true;
};

/**
*	@brief	Set a MIDI mixer channel volume
*	@param	int		channel 0-15
*	@param	int		scale 0-100
*	@return void
*/
void InstrumentMidiMapper::set_midi_channel_volume(int chan, int vol)
{
	uint8_t bytes[3];
	int status;
	
	if ((chan < 16) && (chan >= 0) && (vol <= 100) && (vol >= 0))
	{
		float vol_scale = (float)vol / 100.0f * 127.f;
		midi_channel_volume_scale[chan] = vol_scale;

		bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (chan & 0x0f);
		bytes[1] = _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2;
		bytes[2] = (int)vol_scale;

		std::lock_guard<std::mutex> lock(midiout_mutex);
		if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
		{
			printf("Problem writing midi mapper change channel volume to MIDI output: %s", snd_strerror(status));
		}
	}
}

/**
*	@brief	Returns a MIDI mixer channel volume
*	@param	int		channel 0-15
*	@return vol 0-100
*/
int InstrumentMidiMapper::get_midi_channel_volume(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return (int)(midi_channel_volume_scale[chan] / 127.f * 100);
	}
	
	return 0;
}

/**
*	@brief	Set a MIDI mixer channel pan
*	@param	int		channel 0-15
*	@param	int		pan-0 - (left)  -  100 (right)
*	@return void
*/
void InstrumentMidiMapper::set_midi_channel_pan(int chan, int pan) 
{
	uint8_t bytes[3];
	int status;
	
	if ((chan < 16) && (chan >= 0) && (pan <= 100) && (pan >= 0))
	{
		float pan_scale = ((float)pan / 100.0f) * 127.f;
		midi_channel_pan_scale[chan] = pan_scale;
		
		bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (chan & 0x0f);
		bytes[1] = _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2;
		bytes[2] = (int)pan_scale;

		std::lock_guard<std::mutex> lock(midiout_mutex);
		if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
		{
			printf("Problem writing midi mapper change channel pan to MIDI output: %s", snd_strerror(status));
		}
	}
}
int InstrumentMidiMapper::get_midi_channel_pan(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return (int)(midi_channel_pan_scale[chan] / 127.f * 100);
	}
	return 0;
}

void InstrumentMidiMapper::set_midi_channel_send(int chan, int snd)
{
	uint8_t bytes[3];
	int status;
	
	if ((chan < 16) && (chan >= 0) && (snd <= 100) && (snd >= 0))
	{
		float snd_scale = (float)snd / 100.0f * 127.f;
		midi_channel_send_scale[chan] = snd_scale;

		bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (chan & 0x0f);
		bytes[1] = _MIDI_EVENT_CHANNEL_SEND_BYTE_2;
		bytes[2] = (int)snd_scale;

		std::lock_guard<std::mutex> lock(midiout_mutex);
		if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
		{
			printf("Problem writing midi mapper change channel send to MIDI output: %s", snd_strerror(status));
		}
	}
}

int InstrumentMidiMapper::get_midi_channel_send(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return (int)(midi_channel_send_scale[chan] / 127.f * 100);
	}
	
	return 0;
}

void InstrumentMidiMapper::set_midi_channel_pan_mod_lfo(int chan, int lfo)
{
	uint8_t bytes[3];
	int status;

	if ((chan < 16) && (chan >= 0) && (lfo <= _NUM_OF_LFOS) && (_LFO_NONE >= 0))
	{
		midi_channel_pan_mod_lfo[chan] = lfo;

		bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (chan & 0x0f);
		bytes[1] = _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2;
		bytes[2] = (int)lfo;

		std::lock_guard<std::mutex> lock(midiout_mutex);
		if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
		{
			printf("Problem writing midi mapper change channel volume to MIDI output: %s", snd_strerror(status));
		}
	}
}

int InstrumentMidiMapper::get_midi_channel_pan_mod_lfo(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return midi_channel_pan_mod_lfo[chan];
	}
	
	return 0;
}

void InstrumentMidiMapper::set_midi_channel_pan_mod_lfo_level(int chan, int lev)
{
	uint8_t bytes[3];
	int status;

	if ((chan < 16) && (chan >= 0) && (lev <= 100) && (lev >= 0))
	{
		midi_channel_pan_mod_lfo_level[chan] = lev;

		bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (chan & 0x0f);
		bytes[1] = _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2;
		bytes[2] = (int)lev;

		std::lock_guard<std::mutex> lock(midiout_mutex);
		if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
		{
			printf("Problem writing midi mapper change channel volume to MIDI output: %s", snd_strerror(status));
		}
	}
}

int InstrumentMidiMapper::get_midi_channel_pan_mod_lfo_level(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return midi_channel_pan_mod_lfo_level[chan];
	}
	
	return 0;
}

void InstrumentMidiMapper::enable_midi_channel_block_volume_commands(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		midi_channel_block_channel_volume_commands[chan] = true;
	}
}

void InstrumentMidiMapper::disable_midi_channel_block_volume_commands(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		midi_channel_block_channel_volume_commands[chan] = false;
	}
}

bool InstrumentMidiMapper::get_midi_channel_block_volume_commands_state(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return midi_channel_block_channel_volume_commands[chan];
	}
	else
	{
		return false;
	}
}

void InstrumentMidiMapper::set_keyboard_split_note(int note)
{
	if ((note < 128) && (note >= 0))
	{
		keyboard_split_note = note;
	}
}

int InstrumentMidiMapper::get_keyboard_split_note()
{
	return keyboard_split_note;
}

void InstrumentMidiMapper::register_midi_channel_volume_control_command_trap_callback(func_ptr_void_int_int_t ptr)
{
	midi_channel_volume_control_command_trap_callback_ptr = ptr;
}

void InstrumentMidiMapper::register_midi_channel_change_program_command_trap_callback(func_ptr_void_int_int_t ptr)
{
	midi_channel_change_program_command_trap_callback_ptr = ptr;
}

void InstrumentMidiMapper::register_midi_channel_active_indication_update_callback(func_ptr_void_int_bool_t ptr)
{
	midi_channel_active_indication_callback_ptr = ptr;
}

void InstrumentMidiMapper::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	uint8_t bytes[3];
	int status;

	bytes[0] = _MIDI_EVENT_NOTE_ON + (channel & 0x0f);
	bytes[1] = note;
	// bytes[2] = (int)(velocity * midi_channel_volume_scale[channel]);
	bytes[2] = velocity;

	std::lock_guard<std::mutex> lock(midiout_mutex);
	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper note on data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::note_off_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	uint8_t bytes[3];
	int status;

	bytes[0] = _MIDI_EVENT_NOTE_OFF + (channel & 0x0f);
	bytes[1] = num;
	bytes[2] = val;

	std::lock_guard<std::mutex> lock(midiout_mutex);
	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper note off data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::change_program_handler(uint8_t channel, uint8_t program)
{
	uint8_t bytes[2];
	int status;

	bytes[0] = _MIDI_EVENT_PROGRAM_CHANGE + (channel & 0x0f);
	bytes[1] = program;

	if (midi_channel_change_program_command_trap_callback_ptr != NULL)
	{
		midi_channel_change_program_command_trap_callback_ptr(channel, program);
	}

	if ((status = snd_rawmidi_write(midiout, bytes, 2)) < 0)
	{
		printf("Problem writing midi mapper change program data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::channel_pressure_handler(uint8_t channel, uint8_t val)
{
	
}

void InstrumentMidiMapper::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	uint8_t bytes[3];
	int status;
	int scaled_volume;

	/* Check if channel volume control commands should be blocked. */
	if (num == _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2)
	{
		/* Trap volume commands */
		if ((midi_channel_volume_control_command_trap_callback_ptr != NULL) &&
			!midi_channel_block_channel_volume_commands[channel])
		{
			scaled_volume = (int)((val * 100) / 127);
			midi_channel_volume_control_command_trap_callback_ptr(channel, scaled_volume);
		}
		
		//if (midi_channel_block_channel_volume_commands[channel])
		//{
		//	/* Block - drop the command */
		//	return;
		//}
	}

	bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (channel & 0x0f);
	bytes[1] = num;
	bytes[2] = val;

	std::lock_guard<std::mutex> lock(midiout_mutex);
	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper change control data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::pitch_bend_handler(uint8_t channel, int pitch)
{
	
}

void InstrumentMidiMapper::sysex_handler(uint8_t *message, int len)
{
	
}

void InstrumentMidiMapper::control_box_events_handler(int event_id, uint16_t event_value)
{
	
}
