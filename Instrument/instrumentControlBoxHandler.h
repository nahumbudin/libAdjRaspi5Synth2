/**
* @file		instrumentControlBoxHandler.h
*	@author		Nahum Budin
*	@date		7-Jul-2024
*	@version	1.0	Initial release
*					
*	@brief		Provides an events handler to handle the control box events.
*	
*	History:\n
*	
*/

#pragma once

#include "instrument.h"

class InstrumentControlBoxEventsHandler : public Instrument
{
  public:
	InstrumentControlBoxEventsHandler();
	~InstrumentControlBoxEventsHandler();

	void register_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr);
	void unregister_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr);

	
  private:
	list<func_ptr_void_int_uint16_t> callback_ptr_control_box_event_update_ui_list;

	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);

	void control_box_events_handler(int event_id, uint16_t event_value);

	int sliders_input_value_min[_NUM_OF_CONTROL_BOX_SLIDERS];
	int sliders_input_value_max[_NUM_OF_CONTROL_BOX_SLIDERS];

	std::mutex midiout_mutex; // Protects midiout from concurrent access
};
