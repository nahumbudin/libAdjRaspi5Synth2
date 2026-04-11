/**
* @file		instrumentMidiMixer.h
*	@author		Nahum Budin
*	@date		6-Apr-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements a MIDI Mixer instrument.
*	
*	History:\n
*	
*/

#pragma once

#include "instrument.h"

#define _NUM_MIDI_MIXER_PRESETS 10

class InstrumentMidiMixer : public Instrument
{
public:
	
	InstrumentMidiMixer();
		
	~InstrumentMidiMixer();
	
	int set_default_preset_parameters(_settings_params_t *params);
	
	/**
	*   @brief  Save the active Midi Mixer preset parameters as XML file
	*   @param  path preset XML file full path
	*   @return 0 if done
*   */
	int save_midi_mixer_presets_file(string path);
	
	/**
*   @brief  Open a preset parameters XML file and set it as the Midi Mixer active preset parameters
*   @param  path settings XML file full path.
*   @param  preset a pointer to the active preset
*   @param	summary  a pointer to a string that will hold the preset description.
*   @return 0 if done
*/
	int open_midi_mixer_presets_file(
		string path,
		_settings_params_t *preset,
		string *summary);
	
	int set_midi_mixer_settings(_settings_params_t *preset, _settings_params_t *global_preset);
	
	int generate_midi_mixer_prest_summary(_settings_params_t *preset, string *summary);
	
	void set_active_preset(int preset_num);
	
	int get_active_preset_num();
	int get_active_preset_params(_settings_params_t *preset);
	int set_active_preset_params(_settings_params_t *preset);
	int get_number_of_presets();
	
	int collect_midi_mixer_preset_parms(_settings_params_t *params);
	
	void register_midi_mixer_channel_volume_update_callback(func_ptr_void_int_int_t ptr);
	void register_midi_mixer_channel_send_update_callback(func_ptr_void_int_int_t ptr);
	void register_midi_mixer_channel_pan_update_callback(func_ptr_void_int_int_t ptr);
	void register_midi_mixer_channel_pan_mod_lfo_update_callback(func_ptr_void_int_int_t ptr);
	void register_midi_mixer_channel_pan_mod_lfo_level_update_callback(func_ptr_void_int_int_t ptr);
	
	void update_midi_mixer_gui();
	
	string presets_summary_str[_NUM_MIDI_MIXER_PRESETS] = { "" };
	
private:
	
	
	InstrumentMidiMixer *instrument_midi_mixer_instance;
	
	
	void midi_mixer_channel_volume_update_callback(int channel, int value);
	void midi_mixer_channel_send_update_callback(int channel, int value);
	void midi_mixer_channel_pan_update_callback(int channel, int value);
	void midi_mixer_channel_pan_mod_lfo_update_callback(int channel, int value);
	void midi_mixer_channel_pan_mod_lfo_level_update_callback(int channel, int value);
	
	int active_preset;
	_settings_params_t presets[_NUM_MIDI_MIXER_PRESETS];
	
	static func_ptr_void_int_int_t midi_mixer_channel_volume_update_callback_ptr;
	static func_ptr_void_int_int_t midi_mixer_channel_send_update_callback_ptr;
	static func_ptr_void_int_int_t midi_mixer_channel_pan_update_callback_ptr;
	static func_ptr_void_int_int_t midi_mixer_channel_pan_mod_lfo_update_callback_ptr;
	static func_ptr_void_int_int_t midi_mixer_channel_pan_mod_lfo_level_update_callback_ptr;
	
	
};
