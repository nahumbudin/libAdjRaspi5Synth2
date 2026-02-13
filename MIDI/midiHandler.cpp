/**
* @file		midiHandler.cpp
*	@author		Nahum Budin
*	@date		25-Jun-2024
*	@version	1.1
*					1. Code refactoring
*					2. Removing call to modSynth
*
*	@brief		The midi handler is a mid-stream object
*				that executes the parssed midi commands
*
*  Based on Music Open Lab Library.
*	Copyright AdjHeart Nahum Budin May 2017 and
*
*	History:\n
*
*	version 1.0		13-Oct-2019:
*		First version
*
*/

#include "midiHandler.h"

#include <stdio.h>
#include <stdint.h>

//#include "../synthesizer/modSynth.h"

#include "midiHandler.h"

static int ncount = 0;

void note_on(uint8_t channel, uint8_t note, uint8_t velocity)
{
	if (velocity == 0)
	{
		note_off(channel, note);
	}
	else
	{
		ncount++;

		fprintf(stderr, "handler on %i  %i\n", note, ncount);
		// ModSynth::get_instance()->noteOn(channel, note, velocity);
	}
	//	AdjSynth::get_instance()->midi_play_note_on(channel, note, velocity);
	//	ModSynth::get_instance()->get_fluid_synth()->fluid_synth_note_on_event(channel, note, velocity);
}

void note_off(uint8_t channel, uint8_t note)
{
	ncount--;

	fprintf(stderr, "handler off %i  %i\n", note, ncount);
	// ModSynth::get_instance()->noteOff(channel, note);
	//	AdjSynth::get_instance()->midi_play_note_off(channel, note, 0);
	//	ModSynth::get_instance()->get_fluid_synth()->fluid_synth_note_off_event(channel, note);
}

void change_program(uint8_t channel, uint8_t program)
{

	fprintf(stderr, "handler channel %i\n", channel);
	// ModSynth::get_instance()->changeProgram(channel, program);
	// callback_midi_program_change_event(channel, program);
	//	ModSynth::get_instance()->get_fluid_synth()->fluid_synth_program_change_event(channel, program);
}

void channel_pressure(uint8_t channel, uint8_t key, uint8_t val)
{
	// ModSynth::get_instance()->channelPressure(channel, val);
	//	ModSynth::get_instance()->get_fluid_synth()->fluid_synth_after_touch(channel, key, val);
}

void controller_event(uint8_t channel, uint8_t num, uint8_t val)
{
	fprintf(stderr, "handler conroller channel %i  number %i  value %i\n", channel, num, val);
	// ModSynth::get_instance()->controllerEvent(channel, num, val);

	//	ModSynth::get_instance()->get_fluid_synth()->fluid_synth_controller_event(channel, num, val);
}

void pitch_bend(uint8_t channel, int pitch)
{
	fprintf(stderr, "pitch bend channel %i  pitch %i\n", channel, pitch);
	// ModSynth::get_instance()->pitch_bend(channel, pitch);
	//	ModSynth::get_instance()->get_fluid_synth()->fluid_synth_pitch_bend_event(channel, pitch);
}

void sysex_command(uint8_t *msg, uint8_t len)
{
	uint8_t val1, val2;

	//	fprintf(stderr, "sysex command: ");
	//	for (int i = 0; i < len; i++)
	//	{
	//		fprintf(stderr, "%x ", *(msg + i));
	//	}
	//	fprintf(stderr, "\n");

	// Look for AdjHeart sysex messages
	if ((*(msg + 1) == _MIDI_TILTUNE_SYSEX_VENDOR_ID_0) &&
		(*(msg + 2) == _MIDI_TILTUNE_SYSEX_VENDOR_ID_1) &&
		(*(msg + 3) == _MIDI_TILTUNE_SYSEX_VENDOR_ID_2))
	{
		val1 = *(msg + 5);
		val2 = *(msg + 6);

		if (*(msg + 4) == _MIDI_SYSEX_AUX_KNOB_1_MESSAGE_ID)
		{
			fprintf(stderr, "Knob: %i\n", val1);
		}
		else if (*(msg + 4) == _MIDI_SYSEX_AUX_KNOB_1_SW_MESSAGE_ID)
		{
			fprintf(stderr, "Knob-switch: %i\n", val1);
		}
		else if (*(msg + 4) == _MIDI_SYSEX_AUX_SLIDER_1_MESSAGE_ID)
		{
			fprintf(stderr, "Slider: %i\n", (val1 & 0x7f) + (val2 << 7));
		}
	}
}

MidiHandler::MidiHandler(uint8_t stage)
	: MidiStream(_MIDI_HANDLER_NUM_OF_INPUTS,
				 input_queue_array, stage)
{

	// Take responsibility for updates?

	//	activeInput = _MIDI_HANDLER_MIDI_INPUT;
	update_setup();

	num_outputs = 1;
	midi_parser_state = _MIDI_PARSER_STATE_IDLE;

	disconnect_bt_midi();

	registerCallbackNoteOn(&note_on);
	registerCallbackNoteOff(&note_off);
	registerCallbackChangeProgram(&change_program);

	registerCallbackPolyAftertouch(&channel_pressure);
	registerCallbackChannelControl(&controller_event);
	registerCallbackPitchbend(&pitch_bend);

	registerCallbackSysexMssg(&sysex_command);
}

/**
*   @brief  Enable direct transfer of bluetooth midi events.
*   @param  none
*   @return void
*/
void MidiHandler::connect_bt_midi()
{
	connected = true;
}

/**
*   @brief  Disable direct transfer of bluetooth midi events.
*   @param  none
*   @return void
*/
void MidiHandler::disconnect_bt_midi()
{
	connected = false;
}

/*
void MidiHandler::setActiveInput(int in)
{ 
	activeInput = in;
	if (activeInput < 0)
		activeInput = 0;
	else if (activeInput > _MIDI_HANDLER_NUM_OF_INPUTS - 1)
		activeInput = _MIDI_HANDLER_NUM_OF_INPUTS - 1;
}
*/

/**
*   @brief  The block update processing function.
*   @param  none
*   @return void
*/
void MidiHandler::update()
{

	int stream; // , numOfInputs;

	midi_stream_mssg_block_t *rx_block, *tmp_block;

	volatile static int midi_mssg_index = 0;
	volatile static int index, i;

	for (stream = 0; stream < _MIDI_HANDLER_NUM_OF_INPUTS; stream++)
	{
		// Get input buffer
		rx_block = receive_read_only(stream);
		if (rx_block)
		{
			// New data received
			if (rx_block->header_type == _MIDISTREAM_HEADER_TYPE_MIDI_MULTI)
			{
				// Multipple midi commands seperated by a delimitr (0xF5)
				////				if (rx_block->data_t.dataptr->len > 0)
				if (rx_block->header_length > 0)
				{
					tmp_block = allocate_midi_stream_block();
					if (tmp_block)
					{
						tmp_block->header_type = _MIDISTREAM_HEADER_TYPE_MIDI;
						index = 0;
						////						while (index < rx_block->data_t.dataptr->len)
						while (index < rx_block->header_length)
						{
							i = 0;
							////							while ((rx_block->data_t.dataptr->bytes[index] != _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER) &&
							////								(index < rx_block->data_t.dataptr->len))
							while ((rx_block->data_t.bytes[index] != _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER) &&
								   (index < rx_block->header_length))
							{
								////								tmp_block->data_t.bytes[i++] = rx_block->data_t.dataptr->bytes[index++];
								tmp_block->data_t.bytes[i++] = rx_block->data_t.bytes[index++];
							}
							////							if (rx_block->data_t.dataptr->bytes[index] == _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER)
							if (rx_block->data_t.bytes[index] == _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER)
							{
								tmp_block->header_length = index;
								// skip delimiter
								index++;
								process_midi_command(tmp_block);
								release_midi_stream_block(tmp_block);
							}
						}
					}
				}
			}
			else if (rx_block->header_type == _MIDISTREAM_HEADER_TYPE_MIDI)
			{
				// A single MIDI command
				//			fprintf(stderr, "midi");
				process_midi_command(rx_block);
			}

			////			if (rx_block->ref_count <= 1)
			////				// Last use of MIDI stream block
			release_raw_data_mssg_block(raw_data_mssg);
			release_midi_stream_block(rx_block);
		}
		else
		{
			// Error no block
			//		fprintf(stderr, "Midi handler no rx block: \n");
		}
	}
}

/**
*   @brief  process a single or delomited separated multi parssed midi commad.
*   @param  mssg a block of the parssed midi straem (a single or a delimitted multi midi commands).
*   @return void
*/
void MidiHandler::process_midi_command(midi_stream_mssg_block_t *mssg)
{
	static int midi_mssg_index = 0;
	static int index;
	uint8_t midi_bytes[_MIDI_STREAM_MESSAGE_LEN];

	if ((mssg->data_t.midimssg_t.command != _MIDI_SYSEX_START) &&
		(midi_parser_state == _MIDI_PARSER_STATE_IDLE))
	{
		// New none Sysex MIDI command - handle it
		call_midi_handlers(&mssg->data_t.midimssg_t.command, (uint8_t)sizeof(mssg->data_t));
	}
	else if ((mssg->data_t.midimssg_t.command == _MIDI_SYSEX_START) &&
			 (midi_parser_state == _MIDI_PARSER_STATE_IDLE))
	{
		// New Sysex MIDI command
		midi_mssg_index = 0;
		////		raw_data_mssg = allocateRawDataMssgBlock();
		////		if (raw_data_mssg)
		{
			//	fprintf(stderr, "sys start");
			midi_parser_state = _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS;
			while ((midi_mssg_index < sizeof(mssg->data_t)) &&
				   (mssg->data_t.bytes[midi_mssg_index] != _MIDI_SYSEX_END))
			{
				// Collect Sysex bytes until all block data bytes have been read
				//  or Sysex message end
				////				raw_data_mssg->bytes[midi_mssg_index] = mssg->data_t.bytes[midi_mssg_index];
				midi_bytes[midi_mssg_index] = mssg->data_t.bytes[midi_mssg_index];
				midi_mssg_index++;
			}
			if (mssg->data_t.bytes[midi_mssg_index] == _MIDI_SYSEX_END)
			{
				// End of Sysex message (thats a very short Sysex message. Is it possible?)
				////				raw_data_mssg->bytes[midi_mssg_index++] = _MIDI_SYSEX_END;
				midi_bytes[midi_mssg_index++] = _MIDI_SYSEX_END;
				//				raw_data_mssg->len = midi_mssg_index;
				// Handle it
				////				callMidiHandlers(&raw_data_mssg->bytes[0], midi_mssg_index);
				call_midi_handlers(&midi_bytes[0], midi_mssg_index);
				midi_parser_state = _MIDI_PARSER_STATE_IDLE;
				midi_mssg_index = 0;
			}
		}
	}
	else if (midi_parser_state == _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS)
	{
		// Sysex MIDI command additional data
		////		if (raw_data_mssg)
		{
			//	fprintf(stderr, "mssg index: %i\n", midiMssgIndex);
			index = 0;
			while ((index < sizeof(mssg->data_t)) &&
				   (midi_mssg_index < _MIDI_MSSG_MAX_LEN) &&
				   (mssg->data_t.bytes[index] != _MIDI_SYSEX_END))
			{
				// Collect Sysex bytes until all block's data bytes have been read
				//  or Sysex message end or message is too long
				////				raw_data_mssg->bytes[midi_mssg_index] = mssg->data_t.bytes[index];
				midi_bytes[midi_mssg_index] = mssg->data_t.bytes[index];
				midi_mssg_index++;
				index++;
			}
			if (midi_mssg_index >= _MIDI_MSSG_MAX_LEN)
			{
				// Too long message - discard
				midi_parser_state = _MIDI_PARSER_STATE_IDLE;
				midi_mssg_index = 0;
			}
			else if (mssg->data_t.bytes[index] == _MIDI_SYSEX_END)
			{
				// End of Sysex message
				//	fprintf(stderr, ""sys end");
				////				raw_data_mssg->bytes[midi_mssg_index++] = _MIDI_SYSEX_END;
				midi_bytes[midi_mssg_index++] = _MIDI_SYSEX_END;
				////				raw_data_mssg->len = midi_mssg_index;
				////				callMidiHandlers(&raw_data_mssg->bytes[0], midi_mssg_index);
				call_midi_handlers(&midi_bytes[0], midi_mssg_index);
				midi_parser_state = _MIDI_PARSER_STATE_IDLE;
				midi_mssg_index = 0;
			}
		}
	}
}

/**
*   @brief  activate callback functions to execute a single or delomited separated 
*			multi parssed midi commad.
*   @param  mssg a block of the parssed midi straem single midi command bytes.
*			len the mssg number of bytes
*   @return void
*/
void MidiHandler::call_midi_handlers(uint8_t *msg, uint8_t len)
{

	if (msg && connected)
	{
		uint8_t command = *msg & 0xf0;
		uint8_t channel = *msg & 0x0f;
		uint8_t byte2 = *(msg + 1);
		uint8_t byte3;
		int pitch = 0x2000; // Center pitch

		switch (command)
		{

			//			fprintf(stderr, "command: %x\n", command);

		case _MIDI_NOTE_OFF:
			//			fprintf(stderr, "Off command: \n");

			byte3 = *(msg + 2);

			if (callback_note_off)
				(*callback_note_off)(channel, byte2);
			break;

		case _MIDI_NOTE_ON:
			//			fprintf(stderr, "On command: \n");

			byte3 = *(msg + 2);

			if (callback_note_on)
				(*callback_note_on)(channel, byte2, byte3);
			break;

		case _MIDI_AFTERTOUCH:
			byte3 = *(msg + 2);
			if (callback_poly_aftertouch)
				(*callback_poly_aftertouch)(channel, byte2, byte3);
			break;

		case _MIDI_CHAN_CONROL:
			byte3 = *(msg + 2);
			if (callback_channel_control)
				(*callback_channel_control)(channel, byte2, byte3);
			break;

		case _MIDI_PROGRAM_CHANGE:
			if (callback_change_program)
				(*callback_change_program)(channel, byte2);
			break;

		case _MIDI_CHAN_PRESURE:
			if (callback_channel_pressure)
				(*callback_channel_pressure)(channel, byte2);
			break;

		case _MIDI_PITCH_BEND:
			byte3 = *(msg + 2);
			pitch = (int)((byte2 & 0x7f) + ((byte3 & 0x7f) << 7));
			if (callback_pitch_bend)
				(*callback_pitch_bend)(channel, pitch);
			break;

		case _MIDI_SYSEX_START:
			if (callback_sysex_message)
				(*callback_sysex_message)(msg, len);
			break;

		default:
			// Error
			break;
		}
	}
}

inline void MidiHandler::registerCallbackNoteOff(void (*fptr)(uint8_t channel, uint8_t note))
{
	callback_note_off = fptr;
};

inline void MidiHandler::registerCallbackNoteOn(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity))
{
	callback_note_on = fptr;
};

inline void MidiHandler::registerCallbackPolyAftertouch(void (*fptr)(uint8_t channel, uint8_t note, uint8_t pressure))
{
	callback_poly_aftertouch = fptr;
};

inline void MidiHandler::registerCallbackChannelControl(void (*fptr)(uint8_t channel, uint8_t controller, uint8_t value))
{
	callback_channel_control = fptr;
};

inline void MidiHandler::registerCallbackChangeProgram(void (*fptr)(uint8_t channel, uint8_t program))
{
	callback_change_program = fptr;
};

inline void MidiHandler::registerCallbackChannelPressure(void (*fptr)(uint8_t channel, uint8_t pressure))
{
	callback_channel_pressure = fptr;
};

inline void MidiHandler::registerCallbackPitchbend(void (*fptr)(uint8_t channel, int pitch))
{
	callback_pitch_bend = fptr;
};

inline void MidiHandler::registerCallbackSysexMssg(void (*fptr)(uint8_t *msg, uint8_t length))
{
	callback_sysex_message = fptr;
};
