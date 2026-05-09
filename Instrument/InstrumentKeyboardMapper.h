/**
*	@file		instrumentKeyboardMapper.h
*	@author		Nahum Budin
*	@date		5-May-2026
*	@version	1.0
*					1. Initial version.
*					
*	@brief		MIDI keyboard mapper instrument.
*				It mapps incoming MIDI events generated from the midi keyboard it connects to.
*				It can be used to split the keyboard into 2 zones, and assign each zone keys to a different MIDI channel,
*					regrdless of the keyboard assigned MIDI channel.
*				Each zone can be individually transposed by a specified number of octaves.
*
*	History:\n
*
*/

#pragma once

#include "instrument.h"

class InstrumentKeyboardMapper : public Instrument
{
  public:
	
	InstrumentKeyboardMapper(AlsaMidiSysControl *alsa_control = NULL,
							   string *alsa_client_in_name = NULL);
	~InstrumentKeyboardMapper();
	static InstrumentKeyboardMapper *get_instrument_keyboard_mapper_instance();
	
	void set_zone_1_midi_chan(int chan);
	int get_zone_1_midi_chan();
	
	void set_zone_1_transpose(int octave);
	int get_zone_1_transpose();
	
	void set_zone_2_midi_chan(int chan);
	int get_zone_2_midi_chan();
	
	void set_zone_2_transpose(int octave);
	int get_zone_2_transpose();
	
	void set_split_point_note(int note);
	int get_split_point_note();
	
	void set_bypass_on(bool bypass);
	bool get_bypass_on();
	
	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);
	void control_box_events_handler(int event_id, uint16_t event_value);
	
  private:
	
	static InstrumentKeyboardMapper *instrument_keyboard_mapper_instance;
	std::mutex midiout_mutex; // Protects midiout from concurrent access
	
	int zone_1_low_note;
	int zone_1_high_note;
	int zone_1_midi_chan;
	int zone_1_transpose;
	
	int zone_2_low_note;
	int zone_2_high_note;
	int zone_2_midi_chan;
	int zone_2_transpose;
	
	int split_point_note;
	bool bypass_on;
	
};
