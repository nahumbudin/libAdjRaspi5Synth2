/**
* @file		settingsFiles.cpp
*	@author		Nahum Budin
*	@date		23-Sep-2025
*	@version	1.2
*					1. Code refactoring
*					2. Handlig all instruments settings parameters
*	
*	@brief		Synthesizer settings files handling.
*
*	History see h file:\n
*		ver 1.1 29-Jun-2024 Code refactoring (handling onlu FluidSynth params)
*		ver 1.0 5-Oct-2019 Initial 	modSynthSettingsFiles.cpp
*
*/

#include "settings.h"
#include "../utils/xmlFiles.h"

/**
 * Saves settings parameter as an XML file.
 *
 * @param params settings parameters structure
 * @version parameters version
 * @param name parameters set name (for example, patch name)
 * @param path full path of the file
 * @param type describes the settings paramaters, for example, "fluid_synth_settings"
 * @return #_SETTINGS_OK if saveed sucessfully, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::write_settings_file(_settings_params_t *params,
										   uint32_t version, string name,
										   string path, string type)
{
	settings_res_t res = _SETTINGS_OK;


	// Verify mandatory params
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(version == 0, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	
	XML_files *xml_files = new XML_files();
	vector<string> xmlfilestring;
	std::string file_name, file_name_no_ext;
	std::string type_str;

	// Set type string
	type_str = type;
	if (type_str == "")
	{
		type_str = "other";
	}
	
	// Build file headers
	file_name = xml_files->remove_file_extention(xml_files->get_xml_file_name(path));
	return_val_if_true(file_name == "", _SETTINGS_BAD_PATH);

	file_name_no_ext = xml_files->remove_file_extention(file_name);
	return_val_if_true(file_name_no_ext == "", _SETTINGS_BAD_PATH);

	xmlfilestring.push_back(xml_files->comment("AdjHeart Modular Synthesizer Parameters File"));
	xmlfilestring.push_back(xml_files->start_tag(type_str, 0, true));

	xmlfilestring.push_back(xml_files->string_val_element("string_param", "name", file_name, 4));
	xmlfilestring.push_back(xml_files->int_val_element("int_param", "version", version, 4));

	// Unmark the define below to enable saving all settings parametrs.
	// Leave marked if only settings value parameter should be saved (default)
	//#define _SAVE_ALL_PARAMTERS_
	


	// Go over all string parameters
	for (std::map<std::string, _settings_str_param_t>::iterator param = params->string_parameters_map.begin();
		 param != params->string_parameters_map.end(); ++param)
	{
		// Verify param type
		if (param->second.type == type)
		{
			xmlfilestring.push_back(xml_files->string_val_element(
				"string_param",
				param->first,
				param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param", param->first + ".callback_set",
																  param->second.callback_set, 4));
			}

			if (param->second.block_callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param",
																  param->first + ".block_callback_set",
																  param->second.block_callback_set,
																  4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_start",
																 param->second.block_start_index,
																 4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_stop",
																 param->second.block_stop_index,
																 4));
			}
#endif
		}
	}

	// Go over all integer parameters
	for (std::map<std::string, _settings_int_param_t>::iterator param = params->int_parameters_map.begin();
		 param != params->int_parameters_map.end(); ++param)
	{
		if (param->second.type == type)
		{
			xmlfilestring.push_back(xml_files->int_val_element("int_param", param->first,
															 param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.limits_set)
			{
				xmlfilestring.push_back(xml_files->intValElement("int_param", param->first + ".max",
																 param->second.max_val, 4));

				xmlfilestring.push_back(xml_files->intValElement("int_param", param->first + ".min",
																 param->second.min_val, 4));

				xmlfilestring.push_back(xml_files->boolValElement("bool_param", param->first + ".limits_set",
																  param->second.limits_set, 4));
			}

			if (param->second.callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param", param->first + ".callback_set",
																  param->second.callback_set, 4));
			}

			if (param->second.block_callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param",
																  param->first + ".block_callback_set",
																  param->second.block_callback_set,
																  4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_start",
																 param->second.block_start_index,
																 4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_stop",
																 param->second.block_stop_index,
																 4));
			}
#endif
		}
	}

	// Go over all float parameters
	for (std::map<std::string, _settings_float_param_t>::iterator param = params->float_parameters_map.begin();
		 param != params->float_parameters_map.end(); ++param)
	{
		if (param->second.type == type)
		{
			xmlfilestring.push_back(xml_files->float_val_element("float_param", param->first,
															     param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.limits_set)
			{
				xmlfilestring.push_back(xml_files->floatValElement("float_param", param->first + ".max",
																   param->second.max_val, 4));

				xmlfilestring.push_back(xml_files->floatValElement("float_param", param->first + ".min",
																   param->second.min_val, 4));
				xmlfilestring.push_back(xml_files->boolValElement("bool_param", param->first + ".limits_set",
																  param->second.limits_set, 4));
			}

			if (param->second.callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param", param->first + ".callback_set",
																  param->second.callback_set, 4));
			}

			if (param->second.block_callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param",
																  param->first + ".block_callback_set",
																  param->second.block_callback_set,
																  4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_start",
																 param->second.block_start_index,
																 4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_stop",
																 param->second.block_stop_index,
																 4));
			}
#endif
		}
	}

	// Go over all boolean parameters
	for (std::map<std::string, _settings_bool_param_t>::iterator param = params->bool_parameters_map.begin();
		 param != params->bool_parameters_map.end(); ++param)
	{
		if (param->second.type == type)
		{
			xmlfilestring.push_back(xml_files->bool_val_element("bool_param", param->first,
															    param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param", param->first + ".callback_set",
																  param->second.callback_set, 4));
			}

			if (param->second.block_callback_set)
			{
				xmlfilestring.push_back(xml_files->boolValElement("bool_param",
																  param->first + ".block_callback_set",
																  param->second.block_callback_set,
																  4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_start",
																 param->second.block_start_index,
																 4));

				xmlfilestring.push_back(xml_files->intValElement("int_param",
																 param->first + ".block_stop",
																 param->second.block_stop_index,
																 4));
			}
#endif
		}
	}

	xmlfilestring.push_back(xml_files->end_tag(type_str, 0));

	res = xml_files->write_xml_file(xml_files->remove_file_extention(path).c_str(), &xmlfilestring);

	return res;
}

/**
 * Reads settings parameter from an XML file.
 *
 * @param params settings parameters structure
 * @param path full path of the file
 * @param type describes the settings paramaters, for example, "fluid_synth_settings"
 * @param channel	set param of midi channel
 * @return #_SETTINGS_OK if read sucessfully, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::read_settings_file(_settings_params_t *params,
											string path, string type,
											int channel)
{
	XML_files *xml_files = new XML_files();
	std::string readPatchFileData;
	settings_res_t res, res_1;
	vector<std::string> intParams, boolParams, floatParams, stringParams;
	std::string param_name;
	std::string string_param_value;
	int int_param_value;
	float float_param_value;
	bool bool_param_value;
	int id, ivalue, count;
	_settings_str_param_t str_param;

	// Verify mandatory params
	return_val_if_true(params == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(type == "", _SETTINGS_BAD_PARAMETERS);

	res = xml_files->read_xml_file(xml_files->remove_file_extention(path).c_str(), &readPatchFileData);
	if (res != _SETTINGS_OK)
	{
		return _SETTINGS_READ_FILE_ERROR;
	}

	if (xml_files->element_exist(readPatchFileData, type))
	{
		intParams = xml_files->get_int_params(readPatchFileData);
		boolParams = xml_files->get_bool_params(readPatchFileData);
		floatParams = xml_files->get_float_params(readPatchFileData);
		stringParams = xml_files->get_string_params(readPatchFileData);
	}
	else
	{
		return _SETTINGS_READ_FILE_ERROR;
	}

	// Parse string parameters
	count = 0;
	for (auto &element : stringParams)
	{
		param_name = xml_files->get_element_name(element);
		string_param_value = xml_files->get_string_element_value(element);
		if (!param_name.empty())
		{
			set_string_param_value(params,
								   param_name,
								   string_param_value,
								   _EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
								   channel);
		}
	}

	res_1 = get_string_param(params, "name", &str_param);
	if (res_1 == _SETTINGS_KEY_FOUND)
	{
		params->name = str_param.value;
	}

	// Parse integer parameters
	count = 0;
	for (auto &element : intParams)
	{
		param_name = xml_files->get_element_name(element);
		int_param_value = xml_files->get_int_element_value(element);
		if (!param_name.empty())
		{
			set_int_param_value(params,
								param_name,
								int_param_value,
								_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
								channel);
		}
	}

	// Parse float parameters
	count = 0;
	for (auto &element : floatParams)
	{
		param_name = xml_files->get_element_name(element);
		float_param_value = xml_files->get_float_element_value(element);
		if (!param_name.empty())
		{
			set_float_param_value(params,
								  param_name,
								  float_param_value,
								  _EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
								  channel);
		}
	}

	// Parse boolean parameters
	count = 0;
	for (auto &element : boolParams)
	{
		param_name = xml_files->get_element_name(element);
		bool_param_value = xml_files->get_bool_element_value(element);
		if (!param_name.empty())
		{
			set_bool_param_value(params,
								 param_name,
								 bool_param_value,
								 _EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
								 channel);
		}
	}

	return res;
}

