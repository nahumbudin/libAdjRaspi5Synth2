/**
*	@file		analogSynthPresets.cpp
*	@author		Nahum Budin
*	@date		23-Sep-2025
*	@version	1.1
*					1. Convert an adjust from fluidSynthPresets.
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

#include "adjSynth.h"
#include "../Instrument/instrumentAnalogSynth.h"
#include "../utils/xmlFiles.h"
//#include "settings.h"



int InstrumentAnalogSynth::set_default_settings_parameters(_settings_params_t *preset_params, 
															_settings_params_t *global_params)
{
	int res = 0;

	//res = adjheart_synth->set_default_preset_parameters(params, 0);

	
	res = adjheart_synth->set_default_preset_parameters_amp(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_distortion(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_filter(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_kps(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_modulators(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_mso(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_noise(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_pad(preset_params, 0);
	res |= adjheart_synth->set_default_preset_parameters_vco(preset_params, 0);
	
	
	res |= adjheart_synth->set_default_settings_parameters_equalizer(global_params);
	res |= adjheart_synth->set_default_settings_parameters_keyboard(global_params);
	//res |= adjheart_synth->set_default_settings_parameters_mixer(global_params); // Set in the midi mixer instrument initialization.
	res |= adjheart_synth->set_default_settings_parameters_reverb(global_params);

	jack_out_connection_left.in_client_name = "";
	jack_out_connection_left.in_client_port_name = "";
	jack_out_connection_left.out_client_name = "";
	jack_out_connection_left.out_client_port_name = "";
	
	jack_out_connection_right.in_client_name = "";
	jack_out_connection_right.in_client_port_name = "";
	jack_out_connection_right.out_client_name = "";
	jack_out_connection_right.out_client_port_name = "";

	return res;
}

	/**
*   @brief  Save the active Analog Synth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
	int InstrumentAnalogSynth::save_analog_synth_settings_file(string path)
	{
		int res;

		return_val_if_true(instrument_settings_manager == NULL, _SETTINGS_BAD_PARAMETERS);

		res = instrument_settings_manager->write_settings_file(
			active_settings_params,
			instrument_settings_manager->get_settings_version(),
			"",
			path,
			"adj_synth_settings_params");
		return res;
	}

/**
*   @brief  Open settings parameters XML file and set it as the Analog Synth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
int InstrumentAnalogSynth::open_analog_synth_settings_file(string path)
{
	return_val_if_true(instrument_settings_manager == NULL, _SETTINGS_BAD_PARAMETERS);

	XML_files *xml_files = new XML_files();

	int res = instrument_settings_manager->read_settings_file(active_settings_params, path, "adj_synth_settings_params");
	

	if (res == 0)
	{
		

	}
		return 0;
}

int InstrumentAnalogSynth::collect_analog_synth_preset_parms(_settings_params_t *params)
{
	
	return 0;
}

int InstrumentAnalogSynth::save_analog_synth_preset_file(string path)
{

	return 0;
}


