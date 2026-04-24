/**
* @file		instrumentAnalogReverbration.h
*	@author		Nahum Budin
*	@date		17-Apr-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements an Analog Reverberation instrument.
*	
*	History:\n
*	
*/

#pragma once

#include "instrument.h"

#define _NUM_ANALOG_REVERBERATION_PRESETS 10

// Settings callback functions prototypes
int set_reverb3m_preset_cb(int pset, int prog);
int set_reverb_room_size_cb(int rs, int prog);
int set_reverb_damp_cb(int dmp, int prog);
int set_reverb_wet_cb(int wt, int prog);
int set_reverb_dry_cb(int dry, int prog);
int set_reverb_width_cb(int wd, int prog);
int set_reverb_mode_cb(int md, int prog);
int set_reverb3m_enable_state_cb(bool en, int prog);
int set_reverb_enable_state_cb(bool en, int prog);

class InstrumentAnalogReverbration : public Instrument
{	
public:
	InstrumentAnalogReverbration();
	~InstrumentAnalogReverbration();
	
	int set_default_settings_parameters();
	
	int set_instrument_settings(_settings_params_t *preset);
	
	void set_active_preset(int preset_num);
	
	int get_active_preset_num();
	int get_active_preset_params(_settings_params_t *preset);
	int set_active_preset_params(_settings_params_t *preset);
	int get_number_of_presets();
	
	
	int api_settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program = -1);
	int api_settings_events_handler(int moduleid, int paramid, float val, _settings_params_t *params, int program = -1);
	int api_settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program = -1);
	int api_settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program = -1);
	
	void update_analog_reverberation_gui();
	
	void register_update_gui_callback(func_ptr_void_void_t ptr);
	
private:
	
	InstrumentAnalogReverbration *instrument_analog_reverbration_instance;
	
	int active_preset;
	
	_settings_params_t analog_reverberation_presets[_NUM_ANALOG_REVERBERATION_PRESETS];
	
	static func_ptr_void_void_t update_gui_callback_ptr;
	
};
