/**
* @file		midiParser.h
*	@author		Nahum Budin
*	@date		25-Jun-2024
*	@version	1.1
*					1. Code refactoring.
*
*	@brief	The midi parser is a midi-stream object with a single input 
*			that parsses midi data streams into midi events.
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

#pragma once

#include "midiStream.h"

/**
* @brief	The midi parser is a midi-stream object with a single input 
*			that parsses midi data streams into midi events.
*/
class MidiParser : public MidiStream
{

  public:
	MidiParser(uint8_t stage = 0) : MidiStream(1, input_queue_array, stage)
	{
		update_setup();

		num_outputs = 1;
		midi_parser_state = _MIDI_PARSER_STATE_IDLE;
		midi_buff_in = 0;
		midi_buff_out = 0;
		midi_command_candidate_index = 0;
	}
	virtual void update(void);

	/* How many bytes in MIDI commands. Last is unknown for Sysex */
	static constexpr uint8_t midi_commands_length[8] = {3, 3, 3, 3, 2, 2, 3, 0};

  private:
	raw_data_mssg_block_t *mdata;
	uint8_t midi_parser_state;
	midi_stream_mssg_block_t *input_queue_array[1];

	// Holds incoming data
	uint8_t midi_in_buffer[_MIDI_STREAM_MESSAGE_LEN * 4];
	// Points to next input byte buffer index
	volatile unsigned int midi_buff_in;
	// Points to next output byte buffer index
	volatile unsigned int midi_buff_out;
	// A mask for buffer length bitset
	volatile const unsigned int buff_length_mask = sizeof(midi_in_buffer) - 1;
	// Holds a command candidate data when collected
	uint8_t midi_command[_MIDI_MSSG_MAX_LEN];
	// Holds a pointer to midi command candidate byte index
	volatile uint8_t midi_command_candidate_index;

	int parse(midi_stream_mssg_block_t *block);
};
