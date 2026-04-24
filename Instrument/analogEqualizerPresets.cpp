/**
*	@file		AnalogEqualizerPresets.cpp
*	@author		Nahum Budin
*	@date		11-Apr-2026
*	@version	1.0
*					1. Initial version.
*
*	@brief		Used for collecting and hndling the Analog Equalizer preset params
*
*	History:\n
*
*/

#include "instrumentAnalogEqualizer.h"

#include "../utils/xmlFiles.h"

int InstrumentAnalogEqualizer::set_default_preset_parameters(_settings_params_t *params)
{
	
	return 0;
}

/**
*   @brief  Save the active Analog Equalizer preset parameters as XML file
*   @param  path preset XML file full path
*   @return 0 if done
*   */
int InstrumentAnalogEqualizer::save_analog_equalizer_presets_file(string path)
{
	_settings_params_t preset_temp;
	
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);

	XML_files *xml_files = new XML_files();

	int res = 0;

	res = collect_analog_equalizer_preset_parms(&preset_temp);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return -1;
	}

	preset_temp.name = xml_files->get_xml_file_name(path);
	preset_temp.settings_type = "analog-equalizer-preset";

	res = instrument_settings_manager->write_settings_file(
		&preset_temp,
		instrument_settings_manager->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		"analog-equalizer-preset");

	return res;
}

/**
*   @brief  Open a preset parameters XML file and set it as the Analog Equalizer active preset parameters
*   @param  path settings XML file full path.
*   @param  preset a pointer to the active preset
*   @param	summary  a pointer to a string that will hold the preset description (TBD).
*   @return 0 if done
*/
int InstrumentAnalogEqualizer::open_analog_equalizer_presets_file(
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
		"analog-equalizer-preset");

	if (res == _SETTINGS_OK)
	{
		if (preset != NULL)
		{
			set_analog_equalizer_settings(active_settings_params, 
				ModSynth::get_instance()->adj_synth->get_active_common_params());
			// generate_analog_equalizer_prest_summary(&presets[active_preset], summary[active_preset);
			
		}
	}

	return res;	
}

/**
*   @brief  Sets all the Analog Equalizer preset params.
*   @param	preset	a pointer to the analog equalizer instrument presets parameters struct read from the preset XML file
*   @param	global_preset	a pointer to the mod synth common presets parameters struct
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if presets is NULL is NULL
*/
int InstrumentAnalogEqualizer::set_analog_equalizer_settings(_settings_params_t *preset, _settings_params_t *global_preset)
{
	int res = 0;
	
	
	_settings_int_param_t int_param;

	return_val_if_true(preset == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_31_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_31_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_62_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_62_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_125_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_125_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_250_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_250_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_500_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_500_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_1k_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_1k_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_2k_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_2k_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_4k_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_4k_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_8k_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_8k_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	res |= instrument_settings_manager->get_int_param(
			preset,
		"adjsynth.equilizer.band_16k_level",
		&int_param);
	
	if (res == _SETTINGS_KEY_FOUND)
	{
		res |= instrument_settings_manager->set_int_param(
		global_preset,
			"adjsynth.equilizer.band_16k_level",
			int_param.value,
			0,
			0,
			"analog-equalizer-preset",
			NULL,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_TYPE,
			-1); // no program (common resource)
	}
	
	return res;
}

int InstrumentAnalogEqualizer::generate_analog_equalizer_prest_summary(_settings_params_t *preset, string *summary)
{
	
	return 0;
}

int InstrumentAnalogEqualizer::collect_analog_equalizer_preset_parms(_settings_params_t *params)
{
	int res = 0;
	
	int channel;
	char keyString[128];

	_settings_int_param_t int_param;

	params->settings_type = "analog-equalizer-preset";
	
	
	
	
	
	return res;
	
}

