/**
* @file		instrumentsManager.h
*	@author		Nahum Budin
*	@date		22-Sep-2025
*	@version	1.1
*					1. Code refactoring rename modules to instruments
*					
*	@brief		Manages active instruments
*	
*	History:
*				Ver1.0	10-Jul-2024		1.0	Initial release
*\n
*	
*/

#pragma once

#include <map>
#include <string>

#include "instrument.h"

using namespace std;

class InstrumentsManager
{
  public:
	static InstrumentsManager *get_instance();
	
	~InstrumentsManager();

	int add_instrument(string ins_name, Instrument *inst=NULL);
	int remove_instrument(string ins_name);
	Instrument *get_instrument(string ins_name);

	void close_instrument_pannel_id(en_instruments_ids_t inst_id);
	void close_instrument_pannel_name(string inst_name);
	void open_instrument_pannel_name(string inst_name);
	
	int set_instrument_active_midi_channels_mask(string instrument_key_str, uint16_t mask);
	uint16_t get_instrument_active_midi_channels_mask(string instrument_key_str);

	en_instruments_types_t get_instrument_type(en_instruments_ids_t inst_id);

	void add_active_instrument(en_instruments_ids_t inst);
	void remove_active_instrument(en_instruments_ids_t inst);

	int allocate_midi_channel_synth(int ch, en_instruments_ids_t synth);
	en_instruments_ids_t get_allocated_midi_channel_synth(int ch);

	void register_callback_close_instrument_pannel_id(func_ptr_void_en_instruments_ids_t_t ptr);
	void register_callback_close_instrument_pannel_name(func_ptr_void_string_t ptr);
	void register_callback_open_instrument_pannel_name(func_ptr_void_string_t ptr);

  private:
	InstrumentsManager();

	static InstrumentsManager *instruments_manager_instance;

	map<string, Instrument*> map_active_instruments;
	map<string, string> map_instruments_names;
	map<en_instruments_ids_t, en_instruments_types_t> map_instruments_type;

	vector<en_instruments_ids_t> active_instruments;

	en_instruments_ids_t midi_channels_allocated_synth[16] = {en_instruments_ids_t::none_instrument_id };

	func_ptr_void_en_instruments_ids_t_t callback_close_instrument_pannel_id_ptr;
	func_ptr_void_string_t callback_close_instrument_pannel_name_ptr;
	func_ptr_void_string_t callback_open_instrument_pannel_name_ptr;
};
