/**
* @file		midiHandler.h
*	@author		Nahum Budin
*	@date		13-Oct-2019
*	@version	1.0
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

#pragma once

#include "midiStream.h"

#define _MIDI_HANDLER_NUM_OF_INPUTS 1 + _NUM_OF_MIDI_STREAMS

void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
void note_off(uint8_t channel, uint8_t note);
void change_program(uint8_t channel, uint8_t program);
void channel_pressure(uint8_t channel, uint8_t key, uint8_t val);
void controller_event(uint8_t channel, uint8_t num, uint8_t val);
void pitch_bend(uint8_t channel, int pitch);
void sysex_command(uint8_t *msg, uint8_t len);

class MidiHandler : public MidiStream
{

  public:
	MidiHandler(uint8_t stage = 0);

	void connect_bt_midi();
	void disconnect_bt_midi();

	//	void setActiveInput(int in);

	virtual void update(void);

	inline void registerCallbackNoteOff(void (*fptr)(uint8_t channel, uint8_t note));
	inline void registerCallbackNoteOn(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity));
	inline void registerCallbackPolyAftertouch(void (*fptr)(uint8_t channel, uint8_t note, uint8_t pressure));
	inline void registerCallbackChannelControl(void (*fptr)(uint8_t channel, uint8_t controller, uint8_t value));
	inline void registerCallbackChangeProgram(void (*fptr)(uint8_t channel, uint8_t program));
	inline void registerCallbackChannelPressure(void (*fptr)(uint8_t channel, uint8_t pressure));
	inline void registerCallbackPitchbend(void (*fptr)(uint8_t channel, int pitch));
	inline void registerCallbackSysexMssg(void (*fptr)(uint8_t *msg, uint8_t length));

  private:
	volatile bool update_responsibility;

	void (*callback_note_off)(uint8_t ch, uint8_t note);
	void (*callback_note_on)(uint8_t ch, uint8_t note, uint8_t vel);
	void (*callback_poly_aftertouch)(uint8_t ch, uint8_t note, uint8_t pres);
	void (*callback_channel_control)(uint8_t ch, uint8_t controller, uint8_t value);
	void (*callback_change_program)(uint8_t ch, uint8_t prog);
	void (*callback_channel_pressure)(uint8_t ch, uint8_t pres);
	void (*callback_pitch_bend)(uint8_t ch, int pitch);
	void (*callback_sysex_message)(uint8_t *msg, uint8_t len);

	void call_midi_handlers(uint8_t *msg, uint8_t len);
	void process_midi_command(midi_stream_mssg_block_t *mssg);

	//midiStream_mssg_block_t *inputQueueArray[8];
	midi_stream_mssg_block_t *input_queue_array[_MIDI_HANDLER_NUM_OF_INPUTS];
	raw_data_mssg_block_t *raw_data_mssg;
	uint8_t midi_parser_state;

	/* If true bt midi input is connected to the synthesizer midi input,
	*	disconnected otherwise (default value) */
	bool connected;

	//int activeInput;
};

