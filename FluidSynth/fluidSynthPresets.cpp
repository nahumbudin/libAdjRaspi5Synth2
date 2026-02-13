/**
*	@file		fluidSynthPresets.cpp
*	@author		Nahum Budin
*	@date		23-Sep-2025
*	@version	1.1
*					1. Code refactoring and notaion.
*					2. 
*
*	@brief		Used for collecting preset params
*
*	History:\n
*		version 1.2		28-Jul-2024: Code refactoring and notaion.
*		Based on modSynthCollectPresetParams.h
*		version 1.1		7-Feb-2021
*		version 1.0		30-4-2020: 	First version
*
*/

#include <algorithm>

#include "../Instrument/instrumentFluidSynth.h"
#include "settings.h"
#include "../utils/xmlFiles.h"

class InstrumentFluidSynth;

int InstrumentFluidSynth::collect_fluid_synth_preset_parms(_settings_params_t *params)
{

	int res = 0;
	int result = 0;

	_settings_int_param_t int_param;
	_settings_float_param_t float_param;
	_settings_bool_param_t bool_param;

	int int_val;
	float float_val;
	bool bool_val;

	params->settings_type = "fluid-synth-preset";
	
	for (int i = 0; i < 16; i++)
	{
		res |= instrument_settings->set_int_param(
			params,
			"fluid.synth.ch" + std::to_string(i + 1) + ".midi_program",
			get_fluid_synth_interface()->get_fluid_synth_channel_program(i),
			0,
			0,
			"fluid-synth-param",
			NULL,
			-1,
			-1,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // FluidSynth has no Mod Synth Programs

		res |= instrument_settings->set_string_param(
			params,
			"fluid.synth.ch" + std::to_string(i + 1) + ".midi_program_name",
			get_fluid_synth_interface()->get_fluid_synth_channel_preset_name(i),
			"fluid-synth-param",
			NULL,
			-1,
			-1,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1);

		res |= instrument_settings->set_int_param(
			params,
			"fluid.synth.ch" + std::to_string(i + 1) + ".midi_bank",
			get_fluid_synth_interface()->get_fluid_synth_channel_bank(i),
			0,
			0,
			"fluid-synth-param",
			NULL,
			-1,
			-1,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1);

		res |= instrument_settings->set_string_param(
			params,
			"fluid.synth.ch" + std::to_string(i + 1) + ".midi_sound_font",
			get_fluid_synth_interface()->get_fluid_synth_channel_preset_soundfont_name(i),
			"fluid-synth-param",
			NULL,
			-1,
			-1,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1);

		res |= instrument_settings->set_int_param(
			params,
			"fluid.synth.ch" + std::to_string(i + 1) + ".midi_sound_font_id",
			get_fluid_synth_interface()->get_fluid_synth_channel_sfont_id(i),
			0,
			0,
			"fluid-synth-param",
			NULL,
			-1,
			-1,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1);
	}

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.volume", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0.f;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.volume",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_bool_param(active_settings_params, "fluid.synth.reverb.active", &bool_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		bool_val = bool_param.value;
	}
	else
	{
		bool_val = false;
	}

	res |= instrument_settings->set_bool_param(
		params,
		"fluid.synth.reverb.active",
		bool_val,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.reverb.room-size", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.reverb.room-size",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.reverb.damp", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.reverb.damp",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.reverb.width", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.reverb.width",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.reverb.level", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.reverb.level",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_bool_param(active_settings_params, "fluid.synth.chorus.active", &bool_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		bool_val = bool_param.value;
	}
	else
	{
		bool_val = false;
	}

	res |= instrument_settings->set_bool_param(
		params,
		"fluid.synth.chorus.enabled",
		bool_val,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_int_param(active_settings_params, "fluid.synth.chorus.nr", &int_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		int_val = int_param.value;
	}
	else
	{
		int_val = 0;
	}

	res |= instrument_settings->set_int_param(
		params,
		"fluid_synth.chorus.nr",
		int_val,
		0,
		0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.chorus.level", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.chorus.level",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.chorus.speed", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.chorus.speed",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_float_param(active_settings_params, "fluid.synth.chorus.depth", &float_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		float_val = float_param.value;
	}
	else
	{
		float_val = 0;
	}

	res |= instrument_settings->set_float_param(
		params,
		"fluid.synth.chorus.depth",
		float_val,
		0.0,
		0.0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	result = instrument_settings->get_int_param(active_settings_params, "fluid.synth.chorus.waveform", &int_param);
	if (result == _SETTINGS_KEY_FOUND)
	{
		int_val = int_param.value;
	}
	else
	{
		int_val = 0;
	}

	res |= instrument_settings->set_int_param(
		params,
		"fluid.synth.chorus.waveform",
		int_val,
		0,
		0,
		"fluid-synth-param",
		NULL,
		-1,
		-1,
		NULL,
		_SET_VALUE | _SET_TYPE,
		-1);

	return res;
}

/**
*   @brief  Save a preset parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int InstrumentFluidSynth::save_fluid_synth_preset_file(string path)
{
	_settings_params_t preset_temp;
	
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);

	XML_files *xml_files = new XML_files();

	int res = 0;

	res = collect_fluid_synth_preset_parms(&preset_temp);
	if (res != 0)
	{
		return -1;
	}

	preset_temp.name = xml_files->get_xml_file_name(path);
	preset_temp.settings_type = "fluid-synth-preset";

	res = instrument_settings->write_settings_file(
		&preset_temp,
		instrument_settings->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		"fluid_settings_params");

	return res;
}

/**
*   @brief  Open a preset parameters XML file and set it as the FluidSynth active preset parameters
*   @param  path settings XML file full path.
*   @param  preset a pointer to the active preset
*   @param	summary  a pointer to a string that will hold the preset description.
*   @return 0 if done
*/
int InstrumentFluidSynth::open_fluid_synth_preset_file(
	string path,
	_settings_params_t *preset,
	std::string *summary)
{
	int res = -1;

	return_val_if_true(preset == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);

	res = instrument_settings->read_settings_file(
		preset,
		path,
		"fluid-synth-preset");

	if (res == _SETTINGS_OK)
	{
		if (preset != NULL)
		{
			collect_fluid_synth_prest_channels_data(preset, &channels_presets[active_preset][0]);
			load_fluid_synth_preset_soundfonts(&channels_presets[active_preset][0]);
			set_fluid_synth_settings(active_settings_params, &channels_presets[active_preset][0]);
			generate_fluid_synth_prest_summary(&channels_presets[active_preset][0], summary);
			
		}
	}

	return res;	
}

/**
*   @brief  Collects the channels data from a FluidSynth preset params struct.
*   @param	preset	a pointer to a _settings_params_t presets parameters struct
*   @param	channels_presets   a pointer to an array of channels data structs
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if presets is NULL is NULL
*/
int InstrumentFluidSynth::collect_fluid_synth_prest_channels_data(
			_settings_params_t *params,
			fluid_channel_preset_t *channels_presets)
{
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(channels_presets == NULL, _SETTINGS_BAD_PARAMETERS);

	for (int ch = 0; ch < 16; ch++)
	{
		std::string channel_prefix = "fluid.synth.ch" + std::to_string(ch + 1) + ".midi_";
		
		(channels_presets + ch)->channel_num = ch;
		(channels_presets + ch)->midi_program = params->int_parameters_map[channel_prefix + "program"].value;
		(channels_presets + ch)->midi_bank = params->int_parameters_map[channel_prefix + "bank"].value;
		(channels_presets + ch)->soundfont_id = params->int_parameters_map[channel_prefix + "sound_font_id"].value;
		(channels_presets + ch)->program_name = params->string_parameters_map[channel_prefix + "program_name"].value;
		(channels_presets + ch)->midi_soundfont_file_name = params->string_parameters_map[channel_prefix + "sound_font"].value;
	}

	return 0;
}

/**
*   @brief  Generates a summary text string of the preset fluid programs names.
*   @param	preset	a pointer to the 1st fluid_channel_preset_t presets object[16]
*   @param	summary	a pointer to a std::string that will hold the summary text
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if either params is NULL
*/
int InstrumentFluidSynth::generate_fluid_synth_prest_summary(fluid_channel_preset_t *preset, std::string *summary)
{
	return_val_if_true(preset == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(summary == NULL, _SETTINGS_BAD_PARAMETERS);

	std::string sum_text = "";

	for (int chan = 0; chan < 16; chan++)
	{

		if (chan != 0)
		{
			sum_text += "\n";
		}

		sum_text += "Ch" + std::to_string(chan + 1) + std::string(" : ") + (preset + chan)->program_name;
	}

	*summary = sum_text;

	return 0;
}

/**
*   @brief  Loads all the FluidSynth preset used soundfonts.
*   @param	channels_presets   a pointer to an array of channels data structs
*   @param	channels_presets   a pointer to an array of channels data structs
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if presets is NULL is NULL
*/
int InstrumentFluidSynth::load_fluid_synth_preset_soundfonts(fluid_channel_preset_t *channels_presets)
{
	list<std::string> soundfonts_names;
	list<int> soundfonts_ids;
	std::string path = _FLUID_DEFAULT_SOUNDFONT_DIR;
	int res = 0;

	return_val_if_true(channels_presets == NULL, _SETTINGS_BAD_PARAMETERS);

	soundfonts_names.push_back(channels_presets->midi_soundfont_file_name);
	soundfonts_ids.push_back(channels_presets->soundfont_id);

	for (int ch = 1; ch < 16; ch++)
	{
		if (std::find(soundfonts_names.begin(), soundfonts_names.end(), 
			(channels_presets + ch)->midi_soundfont_file_name) == soundfonts_names.end())
		{
			soundfonts_names.push_back((channels_presets + ch)->midi_soundfont_file_name);
			soundfonts_ids.push_back((channels_presets + ch)->soundfont_id);
		}
	}
	
	// Load soundfonts
	list<std::string>::iterator it = soundfonts_names.begin();
	for (int s = 0; s < soundfonts_names.size(); s++)
	{
		res |= fluid_synth_int_instance->load_and_set_fluid_synth_soundfont(
			path + "/" + *it);
		it++;
	}

	return res;
}

/**
*   @brief  Sets all the FluidSynth preset params.
*   @param	preset	a pointer to a _settings_params_t presets parameters struct
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if presets is NULL is NULL
*/
int InstrumentFluidSynth::set_fluid_synth_settings(_settings_params_t *preset,
												   fluid_channel_preset_t *channels_presets)
{
	int res = 0;
	_settings_float_param_t float_param;
	_settings_int_param_t int_param;
	_settings_bool_param_t bool_param;
	_settings_str_param_t staring_param;

	return_val_if_true(preset == NULL, _SETTINGS_BAD_PARAMETERS);

	for (int chan = 0; chan < 16; chan++)
	{
		res |= fluid_synth_int_instance->set_fluid_synth_program_select(
			chan,
			fluid_synth_int_instance->loaded_soundfont_map[(channels_presets + chan)->midi_soundfont_file_name],
			(channels_presets + chan)->midi_bank,
			(channels_presets + chan)->midi_program);
	}

	if (res == 0)
	{
		instrument_settings->get_float_param(preset, "fluid.synth.volume", &float_param);
		set_volume((int)(float_param.value * 200));

		
		instrument_settings->get_bool_param(preset, "fluid.synth.reverb.active", &bool_param);
		if (bool_param.value)
		{
			enable_reverb();
		}
		else
		{
			disable_reverb();
		}

		instrument_settings->get_float_param(preset, "fluid.synth.reverb.room-size", &float_param);
		set_reverb_room_size((int)(float_param.value * 120));

		instrument_settings->get_float_param(preset, "fluid.synth.reverb.damp", &float_param);
		set_reverb_damp((int)(float_param.value * 100));

		instrument_settings->get_float_param(preset, "fluid.synth.reverb.width", &float_param);
		set_reverb_width((int)(float_param.value * 100));

		instrument_settings->get_float_param(preset, "fluid.synth.reverb.level", &float_param);
		set_reverb_level((int)(float_param.value * 100));

		instrument_settings->get_bool_param(preset, "fluid.synth.chorus.active", &bool_param);
		if (bool_param.value)
		{
			enable_chorus();
		}
		else
		{
			disable_chorus();
		}

		instrument_settings->get_int_param(preset, "fluid.synth.chorus.nr", &int_param);
		set_chorus_number((int)(int_param.value * 10));

		instrument_settings->get_float_param(preset, "fluid.synth.chorus.level", &float_param);
		set_chorus_level((int)(float_param.value * 100));

		instrument_settings->get_float_param(preset, "fluid.synth.chorus.speed", &float_param);
		set_chorus_speed((int)(float_param.value * 100));

		instrument_settings->get_float_param(preset, "fluid.synth.chorus.depth", &float_param);
		set_chorus_depth((int)(float_param.value * 10));

		instrument_settings->get_int_param(preset, "fluid.synth.chorus.waveform", &int_param);
		set_chorus_waveform(int_param.value);		
	}

	return res;
}
