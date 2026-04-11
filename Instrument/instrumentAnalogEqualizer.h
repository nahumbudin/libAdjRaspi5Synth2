/**
* @file		instrumentAnalogEqualizer.h
*	@author		Nahum Budin
*	@date		11-Apr-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements an analog band equalizer instrument.
*	
*	History:\n
*	
*/

#pragma once

#define _NUM_ANALOG_EQUALIZER_PRESETS 10

#include "instrument.h"

class InstrumentAnalogEqualizer : public Instrument
{
public:
	
	InstrumentAnalogEqualizer();
		
	~InstrumentAnalogEqualizer();
	
	int set_default_preset_parameters(_settings_params_t *params);
	
	/**
	*   @brief  Save the active Analog Equalizer preset parameters as XML file
	*   @param  path preset XML file full path
	*   @return 0 if done
*   */
	int save_analog_equalizer_presets_file(string path);
	
	/**
*   @brief  Open a preset parameters XML file and set it as the Analog Equalizer active preset parameters
*   @param  path settings XML file full path.
*   @param  preset a pointer to the active preset
*   @param	summary  a pointer to a string that will hold the preset description.
*   @return 0 if done
*/
	int open_analog_equalizer_presets_file(
		string path,
		_settings_params_t *preset,
		string *summary);
	
	int set_analog_equalizer_settings(_settings_params_t *preset, _settings_params_t *global_preset);
	
	int generate_analog_equalizer_prest_summary(_settings_params_t *preset, string *summary);
	
	void set_active_preset(int preset_num);
	
	int get_active_preset_num();
	int get_active_preset_params(_settings_params_t *preset);
	int set_active_preset_params(_settings_params_t *preset);
	int get_number_of_presets();
	
	int collect_analog_equalizer_preset_parms(_settings_params_t *params);
	
	void register_analog_equalizer_channel_gui_update_callback(func_ptr_void_void_t ptr);
	
	void update_analog_equalizer_gui();
	
	string presets_summary_str[_NUM_ANALOG_EQUALIZER_PRESETS] = { "" };
	
private:
	
	
	InstrumentAnalogEqualizer *instrument_analog_equalizer_instance;
	
	
	void analog_equalizer_channel_gui_update_callback();
	
	int active_preset;
	_settings_params_t presets[_NUM_ANALOG_EQUALIZER_PRESETS];
	
	static func_ptr_void_void_t analog_equalizer_channel_gui_update_callback_ptr;
	
	
};
