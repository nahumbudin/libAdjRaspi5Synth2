/**
*	@file		midiMixerPresets.cpp
*	@author		Nahum Budin
*	@date		6-Apr-2026
*	@version	1.0
*					1. Initial version.
*
*	@brief		Used for collecting and hndling the Midi Mixer preset params
*
*	History:\n
*
*/

#include "instrumentMidiMixer.h"
#include "instrumentMidiMapper.h"
#include "instrumentAnalogSynth.h"

#include "../utils/xmlFiles.h"


int InstrumentMidiMixer::set_default_settings_parameters()
{
	
	int res = 0, channel; 
	char keyString[128];
	
	return_val_if_true(active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	
	for (channel = 0; channel < 16; channel++)
	{
		sprintf(keyString, "adjsynth.mixer_channel_%i.level", channel + 1);
		res |= instrument_settings_manager->set_int_param
			(active_settings_params,
			string(keyString),
			80,
			100,
			0,
			"midi-mixer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);

		sprintf(keyString, "adjsynth.mixer_channel_%i.pan", channel + 1);
		res |= instrument_settings_manager->set_int_param
			(active_settings_params,
			string(keyString),
			50,
			100,
			0,
			"midi-mixer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);

		sprintf(keyString, "adjsynth.mixer_channel_%i.send", channel + 1);
		res |= instrument_settings_manager->set_int_param
			(active_settings_params,
			string(keyString),
			0,
			100,
			0,
			"midi-mixer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);
		
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo", channel + 1);
		res |= instrument_settings_manager->set_int_param
			(active_settings_params,
			string(keyString),
			_LFO_NONE,
			_LFO_6,
			_LFO_NONE,
			"midi-mixer-preset",
			NULL, 
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);
		
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo_level", channel + 1);
		res |= instrument_settings_manager->set_int_param
			(active_settings_params,
			string(keyString),
			0,
			100,
			0,
			"midi-mixer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);
	}
	
	return res;
}
	
/**
*   @brief  Save the active Midi Mixer preset parameters as XML file
*   @param  path preset XML file full path
*   @return 0 if done
*   */
int InstrumentMidiMixer::save_midi_mixer_presets_file(string path)
{
	_settings_params_t preset_temp;
	
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);

	XML_files *xml_files = new XML_files();

	int res = 0;

	res = collect_midi_mixer_preset_parms(&preset_temp);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return -1;
	}

	preset_temp.name = xml_files->get_xml_file_name(path);
	preset_temp.settings_type = "midi-mixer-preset";

	res = instrument_settings_manager->write_settings_file(
		&preset_temp,
		instrument_settings_manager->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		"midi-mixer-preset");

	return res;
}

/**
 *   @brief  Open a preset parameters XML file and set it as the Midi Mixer active preset parameters
 *   @param  path settings XML file full path.
 *   @param  preset a pointer to the active preset
 *   @param	summary  a pointer to a string that will hold the preset description (TBD).
 *   @return 0 if done
 *
 *	// 13-Apr-2026 Note: Not in use. Use Instrument::read_instrument_settings()
*/
int InstrumentMidiMixer::open_midi_mixer_presets_file(
	string path,
	_settings_params_t *preset,
	string *summary)
{
	int res = -1;

	return_val_if_true(preset == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);

	res = instrument_settings_manager->read_settings_file(
		preset,
		path,
		"midi-mixer-preset");

	if (res == _SETTINGS_OK)
	{
		if (preset != NULL)
		{
			set_instrument_settings(active_settings_params); // Set the midi instrument params
			
			update_midi_mixer_gui();
			// generate_midi_mixer_prest_summary(&presets[active_preset], summary[active_preset);
			
		}
	}

	return res;	
}

/**
*   @brief  Sets all the Midi Mixer preset params (execute the setting callbacks).
*   @param	preset	a pointer to the midi mixer instrument presets parameters struct read from the preset file
*   @param	global_preset	a pointer to the mod synth common presets parameters struct
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if presets is NULL is NULL
*/
int InstrumentMidiMixer::set_instrument_settings(_settings_params_t *preset)
{
	int res = 0;
	int channel;
	char keyString[128];
	
	_settings_int_param_t int_param;

	return_val_if_true(active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);

	for (channel = 0; channel < 16; channel++)
	{		
		sprintf(keyString, "adjsynth.mixer_channel_%i.level", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			preset,
			string(keyString),
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			set_channel_volume(channel, int_param.value);
			
			AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
				_MIXER_CHAN_1_LEVEL + channel,
				int_param.value,
				AdjSynth::get_instance()->get_active_common_params(),
				AdjSynth::get_instance()->get_active_sketch());
		}		
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			preset,
			string(keyString),
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{		
			set_channel_pan(channel, int_param.value);
			
			AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
				_MIXER_CHAN_1_PAN + channel,
				int_param.value,
				AdjSynth::get_instance()->get_active_common_params(),
				AdjSynth::get_instance()->get_active_sketch());
		}
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.send", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			preset,
			string(keyString),
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{			
			set_channel_send(channel, int_param.value);
			
			AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
				_MIXER_CHAN_1_SEND + channel,
				int_param.value,
				AdjSynth::get_instance()->get_active_common_params(),
				AdjSynth::get_instance()->get_active_sketch());
		}
		
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			preset,
			string(keyString),
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{		
			set_channel_pan_mod_lfo(channel, int_param.value);
			
			AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
				_MIXER_CHAN_1_PAN_MOD_LFO + channel,
				int_param.value,
				AdjSynth::get_instance()->get_active_common_params(),
				AdjSynth::get_instance()->get_active_sketch());
		}
	
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo_level", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			preset,
			string(keyString),
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{		
			set_channel_pan_mod_lfo_level(channel, int_param.value);
			
			AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
				_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
				int_param.value,
				AdjSynth::get_instance()->get_active_common_params(),
				AdjSynth::get_instance()->get_active_sketch());
		}
	}

	return res;
}
	
int InstrumentMidiMixer::generate_midi_mixer_prest_summary(_settings_params_t *preset, string *summary)
{
	
	return 0;
}

int InstrumentMidiMixer::collect_midi_mixer_preset_parms(_settings_params_t *params)
{
	int res = 0;
	
	int channel;
	char keyString[128];

	_settings_int_param_t int_param;

	params->settings_type = "midi-mixer-preset";
	
	for (int channel = 0; channel < 16; channel++)
	{
		sprintf(keyString, "adjsynth.mixer_channel_%i.level", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_common_params(),
			string(keyString),
			 &int_param);

		if (res == _SETTINGS_KEY_FOUND)
		{
			res |= instrument_settings_manager->set_int_param(
				params,
				string(keyString),
				int_param.value,
				0, 0,
				"midi-mixer-preset",
				NULL,
				 0,
				 0,
				NULL,
				_SET_VALUE | _SET_TYPE,
				0); 
		}		
			
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_common_params(),
			string(keyString),
			&int_param);

		if (res == _SETTINGS_KEY_FOUND)
		{
			res |= instrument_settings_manager->set_int_param(
				params,
				string(keyString),
				int_param.value,
				0,
				0,
				"midi-mixer-preset",
				NULL,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_TYPE,
				0); 
		}		
		
		sprintf(keyString, "adjsynth.mixer_channel_%i.send", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_common_params(),
			string(keyString),
			&int_param);

		if (res == _SETTINGS_KEY_FOUND)
		{
			res |= instrument_settings_manager->set_int_param(
				params,
				string(keyString),
				int_param.value,
				0,
				0,
				"midi-mixer-preset",
				NULL,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_TYPE,
				0); 
		}
		
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_common_params(),
			string(keyString),
			&int_param);

		if (res == _SETTINGS_KEY_FOUND)
		{
			res |= instrument_settings_manager->set_int_param(
				params,
				string(keyString),
				int_param.value,
				0,
				0,
				"midi-mixer-preset",
				NULL,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_TYPE,
				0); 
		}				
		
		sprintf(keyString, "adjsynth.mixer_channel_%i.pan_mod_lfo_level", channel + 1);
		
		res |= instrument_settings_manager->get_int_param(
			ModSynth::get_instance()->adj_synth->get_active_common_params(),
			string(keyString),
			&int_param);

		if (res == _SETTINGS_KEY_FOUND)
		{
			res |= instrument_settings_manager->set_int_param(
				params,
				string(keyString),
				int_param.value,
				0,
				0,
				"midi-mixer-preset",
				NULL,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_TYPE,
				0); 
		}		
	}
	
	return res;
}

	
void InstrumentMidiMixer::set_active_preset(int preset_num)
{
	
	
}
	
int InstrumentMidiMixer::get_active_preset_num()
{
	
	return active_preset;
}
	
int InstrumentMidiMixer::get_active_preset_params(_settings_params_t *preset)
{
	
	return 0;
}
	
int InstrumentMidiMixer::set_active_preset_params(_settings_params_t *preset)
{
	
	return 0;
}
	
int InstrumentMidiMixer::get_number_of_presets()
{
	
	return _NUM_MIDI_MIXER_PRESETS;
}


// Presets handling callback functions


int InstrumentMidiMixer::set_channel_volume(int channel, int value)
{
	ModSynth::get_instance()->midi_mapper->set_midi_channel_volume(channel, value);
	return 0;
}

int InstrumentMidiMixer::set_channel_send(int channel, int value)
{
	ModSynth::get_instance()->midi_mapper->set_midi_channel_send(channel, value);
	return 0;
}

int InstrumentMidiMixer::set_channel_pan(int channel, int value)
{
	ModSynth::get_instance()->midi_mapper->set_midi_channel_pan(channel, value);
	return 0;
}

int InstrumentMidiMixer::set_channel_pan_mod_lfo(int channel, int value)
{
	ModSynth::get_instance()->midi_mapper->set_midi_channel_pan_mod_lfo(channel, value);
	return 0;
}

int InstrumentMidiMixer::set_channel_pan_mod_lfo_level(int channel, int value)
{
	ModSynth::get_instance()->midi_mapper->set_midi_channel_pan_mod_lfo_level(channel, value);
	return 0;
}
	
