/**
 * @file		settings.h
 *	@author		Nahum Budin
 *	@date		23-Sep-2025
 *	@version	1.2
 *					1. Refactoring.
 *					2. Comments
 *					3. Adding a mutex to each settings structure to be used instead
 *						of the global mutex.
*	
*	@brief		Instruments and common settings.
*
*	History: see h file\n
*		ver. 1.1  28-Jun-2024
*					1. A parent class to each individual instrument son.
*					2. File handling is common and is managed by the modSynth settings object.
*					3. Replacing param type field from int to string
*		ver. 1.0  5-Oct-2019  modSynthSettings.h
*/

/**
*	Here we handle the AdjHeart instruments sysnthesizer settings parameters.
*	Each instrument or common resources has an instance of a settings object.
*	Each settings parameter is identified by a unique string key (name).
*	There are 4 types of parameters values: string, integer, float (double) and boolean.
*	The integer and float parameters also include their minimum and maximum values.
*	Each parameter includes a pointer to a set-value callback function.
*	The parameters are handeled as map that holds the parameter unique key name (string) and the parameter structure.
*	Referencing a parameter is done by its name (key).
*	The same parameter can be set for a each polyphonic voice parameter instance.
*/

#include <cstring>

#include "../modSynth.h"
#include "settings.h"
//#include "../utils/utils.h"

/* A nutex to control asynchronous settings operation request execution */
//std::mutex Settings::settings_manage_mutex;

/* Settings version */
uint32_t Settings::settings_version = 250923; // 2025-09-23

/**
 * @brief Creates a settings class object instance
 *
 * @param settings_params a pointer to the settings parameters object (mandatory parameter)
 * @return none
 */
Settings::Settings(_settings_params_t *settings_params)
{
	if (settings_params == NULL)
	{
		printf("fatal error - fluid synth - no settings params");
		exit(1);
	}

	active_settings_params = settings_params;

	settings_version = get_settings_version();
}

Settings::~Settings()
{
	active_settings_params->string_parameters_map.clear();
	active_settings_params->int_parameters_map.clear();
	active_settings_params->float_parameters_map.clear();
	active_settings_params->bool_parameters_map.clear();
}

/**
  * @brief Returns the active settings parameters
  *
  * @param none
  * @return the active settings prameters
  */
_settings_params_t *Settings::get_active_settings_parameters()
{
	return active_settings_params;
}

/**
*	@brief	Deep copy settings parameters from a given source to a destination.
*
*	@param	destination_params	destination _setting_params_t
*	@param	source_params		source _setting_params_t
*	@return void
*/
void Settings::settings_params_deep_copy(_settings_params_t *destination_params,
										 _settings_params_t *source_params)
{
	destination_params = (_settings_params_t *)malloc(sizeof(*source_params));
	memcpy(destination_params, source_params, sizeof(*source_params));
}

/**
 * @brief Returns the settings version.
 *
 * @param none
 * @return settings version
 */
uint32_t Settings::get_settings_version()
{
	return settings_version;
}

/**
 * @brief Returns the specified string setting param structure.
 *
 * @param settings	all settings object (mandatory parameter)
 * @param name		the setting's name (mandatory parameter)
 * @param param		pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t Settings::get_string_param(_settings_params_t *settings, string name,
										  _settings_str_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, _settings_str_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

	//	settings_manage_mutex.lock(); TODO: check if needed for read operations

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->string_parameters_map.find(name);
	if (iter != _settings->string_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		res = _SETTINGS_KEY_FOUND;
	}

	//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief Returns the specified integer setting param structure.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name (mandatory parameter)
 * @param param pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t Settings::get_int_param(_settings_params_t *settings, string name,
									   _settings_int_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, _settings_int_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

	//	settings_manage_mutex.lock();

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->int_parameters_map.find(name);
	if (iter != _settings->int_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->max_val = iter->second.max_val;
		param->min_val = iter->second.min_val;
		param->limits_set = iter->second.limits_set;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		
		res = _SETTINGS_KEY_FOUND;
	}

	//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief Return the specified float (double) setting param structure.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name (mandatory parameter)
 * @param param pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t Settings::get_float_param(_settings_params_t *settings, string name,
										 _settings_float_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, _settings_float_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

	//	settings_manage_mutex.lock();

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->float_parameters_map.find(name);
	if (iter != _settings->float_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->max_val = iter->second.max_val;
		param->min_val = iter->second.min_val;
		param->limits_set = iter->second.limits_set;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		res = _SETTINGS_KEY_FOUND;
	}

	//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief Returns the specified boolean setting param structure.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name (mandatory parameter)
 * @param param pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t Settings::get_bool_param(_settings_params_t *settings, string name,
										_settings_bool_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, _settings_bool_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

	//	settings_manage_mutex.lock();

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->bool_parameters_map.find(name);
	if (iter != _settings->bool_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		res = _SETTINGS_KEY_FOUND;
	}

	//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief	Set the specified string setting param.
 *			If no param with specified name is found a new param is created.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param type describes the settings paramaters, for example, "fluid_synth_settings"
 * @param callback an update function for the settings
 * @param block_start start index for block settings
 * @param block_stop stop index for block settings
 * @param block_callback a block update function for the settings
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
 *					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
 *					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise 
 */

settings_res_t Settings::set_string_param(_settings_params_t *settings,
										  string name,
										  string value,
										  string type,
										  string_param_update_callback_t callback,
										  int block_start,
										  int block_stop,
										  string_block_param_update_callback_t block_callback,
										  uint16_t set_mask,
										  int program)
{
	settings_res_t res = _SETTINGS_FAILED;

	_settings_str_param_t param;
	_settings_str_param_t new_param;

	// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);

	//settings_manage_mutex.lock();
	ModSynth::settings_handler_mutex.lock();

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_string_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add new
		param.key = name;

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				param.value = value;
			}
		else
		{
			param.value = "";
		}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				param.type = type;
			}
		else
		{
			param.type = "param_type_none";
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				param.setup_callback = callback;
				param.callback_set = true;
			}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				param.block_start_index = block_start;
			}
		else
		{
			param.block_start_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				param.block_stop_index = block_stop;
			}
		else
		{
			param.block_stop_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				param.block_setup_callback = block_callback;
				param.block_callback_set = true;
			}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->string_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = param.key;

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				new_param.value = value;
			}
		else
		{
			new_param.value = param.value;
		}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				new_param.type = type;
			}
		else
		{
			new_param.type = param.type;
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				// A new callback function - update
				new_param.setup_callback = callback;
				new_param.callback_set = true;
			}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				new_param.block_start_index = block_start;
			}
		else
		{
			new_param.block_start_index = param.block_start_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				new_param.block_stop_index = block_stop;
			}
		else
		{
			new_param.block_stop_index = param.block_stop_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				new_param.block_setup_callback = block_callback;
				new_param.block_callback_set = true;
			}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}

		_settings->string_parameters_map[name] = new_param;
	}

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) &&
		(_settings->string_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
		_settings->string_parameters_map[name].setup_callback(value, program);
	}

	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) &&
		(_settings->string_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if (_settings->string_parameters_map[name].block_start_index >= 0 &&
			(_settings->string_parameters_map[name].block_stop_index >=
			 _settings->string_parameters_map[name].block_start_index))
		{
			for (int i = _settings->string_parameters_map[name].block_start_index;
				 i <= _settings->string_parameters_map[name].block_stop_index; i++)
			{
				_settings->string_parameters_map[name].block_setup_callback(value, i, program);
			}
		}
	}

	//settings_manage_mutex.unlock();
	ModSynth::settings_handler_mutex.unlock();

	return _SETTINGS_OK;
}

/**
 * @brief	Set the specified int setting param.
 *			If no param with specified name is found a new param is created.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new int value
 * @param max_val the param maximum valid value
 * @param min_val the param minimum valid value
 * @param type describes the settings paramaters, for example, "fluid_synth_settings"
 * @param callback an update function for the settings
 * @param block_start start index for block settings
 * @param block_stop stop index for block settings
 * @param block_callback a block update function for the settings											 
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
 *					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
 *					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::set_int_param(_settings_params_t *settings, string name,
									   int value,
									   int max_val, int min_val,
									   string type,
									   int_param_update_callback_t callback,
									   int block_start,
									   int block_stop,
									   int_block_param_update_callback_t block_callback,
									   uint16_t set_mask,
									   int program)
{
	settings_res_t res = _SETTINGS_FAILED;
	_settings_int_param_t param;
	_settings_int_param_t new_param;

	// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);

	//settings_manage_mutex.lock();
	ModSynth::settings_handler_mutex.lock();

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_int_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add it
		param.key = name;

		if
			_CHECK_MASK(set_mask, _SET_MAX_VAL)
			{
				param.max_val = max_val;
				param.limits_set = true;
			}

		if
			_CHECK_MASK(set_mask, _SET_MIN_VAL)
			{
				param.min_val = min_val;
				param.limits_set = true;
			}

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				if (!param.limits_set)
				{
					param.value = value;
				}
				else
				{
					if ((value < param.min_val) || (value > param.max_val))
					{
						// out of rang - abbort
						//settings_manage_mutex.unlock();
						ModSynth::settings_handler_mutex.unlock();
						return _SETTINGS_PARAM_OUT_OF_RANGE;
					}
					else
					{
						param.value = value;
					}
				}
			}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				param.type = type;
			}
		else
		{
			param.type = "param_type_none";
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				param.setup_callback = callback;
				param.callback_set = true;
			}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				param.block_start_index = block_start;
			}
		else
		{
			param.block_start_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				param.block_stop_index = block_stop;
			}
		else
		{
			param.block_stop_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				param.block_setup_callback = block_callback;
				param.block_callback_set = true;
			}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->int_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = param.key;

		if
			_CHECK_MASK(set_mask, _SET_MAX_VAL)
			{
				new_param.max_val = max_val;
				new_param.limits_set = true;
			}
		else
		{
			new_param.max_val = param.max_val;
			new_param.limits_set = param.limits_set;
		}

		if
			_CHECK_MASK(set_mask, _SET_MIN_VAL)
			{
				new_param.min_val = min_val;
				new_param.limits_set = true;
			}
		else
		{
			new_param.min_val = param.min_val;
			new_param.limits_set = param.limits_set;
		}

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				if (!param.limits_set)
				{
					new_param.value = value;
					//settings->params_mutex.unlock();
				}
				else
				{
					if ((value < new_param.min_val) || (value > new_param.max_val))
					{
						// out of rang - abbort
						ModSynth::settings_handler_mutex.unlock();
						return _SETTINGS_PARAM_OUT_OF_RANGE;
					}
					else
					{
						new_param.value = value;
					}
				}
			}
		else
		{
			new_param.value = param.value;
		}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				new_param.type = type;
			}
		else
		{
			new_param.type = param.type;
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				new_param.setup_callback = callback;
				new_param.callback_set = true;
			}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				new_param.block_start_index = block_start;
			}
		else
		{
			new_param.block_start_index = param.block_start_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				new_param.block_stop_index = block_stop;
			}
		else
		{
			new_param.block_stop_index = param.block_stop_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				new_param.block_setup_callback = block_callback;
				new_param.block_callback_set = true;
			}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}

		_settings->int_parameters_map[name] = new_param;
	}

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) &&
		(_settings->int_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
		_settings->int_parameters_map[name].setup_callback(value, program);
	}

	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) &&
		(_settings->int_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if ((_settings->int_parameters_map[name].block_start_index >= 0) &&
			(_settings->int_parameters_map[name].block_stop_index >=
			 _settings->int_parameters_map[name].block_start_index))
		{
			for (int i = _settings->int_parameters_map[name].block_start_index;
				 i <= _settings->int_parameters_map[name].block_stop_index; i++)
			{
				_settings->int_parameters_map[name].block_setup_callback(value, i, program);
			}
		}
	}

	//settings_manage_mutex.unlock();
	ModSynth::settings_handler_mutex.unlock();

	return _SETTINGS_OK;
}

/**
 * Set the specified float (double) setting param.
 *	If no param with specified name is found a new param is created.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new float (double) value
 * @param max_val the param maximum valid value
 * @param min_val the param minimum valid value
 * @param type describes the settings paramaters, for example, "fluid_synth_settings"
 * @param callback an update function for the settings
 * @param block_start start index for block settings
 * @param block_stop stop index for block settings
 * @param block_callback a block update function for the settings
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
 *					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
 *					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::set_float_param(_settings_params_t *settings, string name,
										 double value,
										 double max_val, double min_val,
										 std::string type,
										 float_param_update_callback_t callback,
										 int block_start,
										 int block_stop,
										 float_block_param_update_callback_t block_callback,
										 uint16_t set_mask,
										 int program)
{
	settings_res_t res = _SETTINGS_FAILED;
	_settings_float_param_t param;
	_settings_float_param_t new_param;

	// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);

	//settings_manage_mutex.lock();
	ModSynth::settings_handler_mutex.lock();

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_float_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add it
		param.key = name;

		if
			_CHECK_MASK(set_mask, _SET_MAX_VAL)
			{
				param.max_val = max_val;
				param.limits_set = true;
			}

		if
			_CHECK_MASK(set_mask, _SET_MIN_VAL)
			{
				param.min_val = min_val;
				param.limits_set = true;
			}

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				if (!param.limits_set)
				{
					param.value = value;
				}
				else
				{
					if ((value < param.min_val) || (value > param.max_val))
					{
						// out of rang - abbort
						//settings_manage_mutex.unlock();
						ModSynth::settings_handler_mutex.unlock();
						
						return _SETTINGS_PARAM_OUT_OF_RANGE;
					}
					else
					{
						param.value = value;
					}
				}
			}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				param.type = type;
			}
		else
		{
			param.type = "parm_type_none";
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				param.setup_callback = callback;
				param.callback_set = true;
			}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				param.block_start_index = block_start;
			}
		else
		{
			param.block_start_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				param.block_stop_index = block_stop;
			}
		else
		{
			param.block_stop_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				param.block_setup_callback = block_callback;
				param.block_callback_set = true;
			}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->float_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = name;

		if
			_CHECK_MASK(set_mask, _SET_MAX_VAL)
			{
				new_param.max_val = max_val;
				new_param.limits_set = true;
			}
		else
		{
			new_param.max_val = param.max_val;
			new_param.limits_set = param.limits_set;
		}

		if
			_CHECK_MASK(set_mask, _SET_MIN_VAL)
			{
				new_param.min_val = min_val;
				new_param.limits_set = true;
			}
		else
		{
			new_param.min_val = param.min_val;
			new_param.limits_set = param.limits_set;
		}

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				if (!param.limits_set)
				{
					new_param.value = value;
				}
				else
				{
					if ((value < new_param.min_val) || (value > new_param.max_val))
					{
						// out of rang - abbort
						//settings_manage_mutex.unlock();
						ModSynth::settings_handler_mutex.unlock();
						
						return _SETTINGS_PARAM_OUT_OF_RANGE;
					}
					else
					{
						new_param.value = value;
					}
				}
			}
		else
		{
			new_param.value = param.value;
		}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				new_param.type = type;
			}
		else
		{
			new_param.type = param.type;
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				new_param.setup_callback = callback;
				new_param.callback_set = true;
			}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				new_param.block_start_index = block_start;
			}
		else
		{
			new_param.block_start_index = param.block_start_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				new_param.block_stop_index = block_stop;
			}
		else
		{
			new_param.block_stop_index = param.block_stop_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				new_param.block_setup_callback = block_callback;
				new_param.block_callback_set = true;
			}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}

		_settings->float_parameters_map[name] = new_param;
	}

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) &&
		(_settings->float_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
		_settings->float_parameters_map[name].setup_callback(value, program);
	}

	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) &&
		(_settings->float_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if ((_settings->float_parameters_map[name].block_start_index >= 0) &&
			(_settings->float_parameters_map[name].block_stop_index >=
			 _settings->float_parameters_map[name].block_start_index))
		{
			for (int i = _settings->float_parameters_map[name].block_start_index;
				 i <= _settings->float_parameters_map[name].block_stop_index; i++)
			{
				_settings->float_parameters_map[name].block_setup_callback(value, i, program);
			}
		}
	}

	//settings_manage_mutex.unlock();
	ModSynth::settings_handler_mutex.unlock();

	return _SETTINGS_OK;
}

/**
* Set the specified boolean setting param.
*	If no param with specified name is found a new param is created.
*
* @param settings all settings structure (mandatory parameter)
* @param name the setting's name  (mandatory parameter)
* @param value the param new boolean
* @param type describes the settings paramaters, for example, "fluid_synth_settings"
* @param callback an update function for the settings
* @param block_start start index for block settings
* @param block_stop stop index for block settings
* @param block_callback a block update function for the settings
* @param _set_mask a bit map that indicates what parameter's fields should be set or update
*					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
*					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
*					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
* @param prog	program number
* @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
*/
settings_res_t Settings::set_bool_param(_settings_params_t *settings, string name,
										bool value,
										string type,
										bool_param_update_callback_t callback,
										int block_start,
										int block_stop,
										bool_block_param_update_callback_t block_callback,
										uint16_t set_mask,
										int program)
{
	settings_res_t res = _SETTINGS_FAILED;
	_settings_bool_param_t param;
	_settings_bool_param_t new_param;

	// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);

	//settings_manage_mutex.lock();
	ModSynth::settings_handler_mutex.lock();

	_settings_params_t *_settings;

	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_bool_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add new
		param.key = name;

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				param.value = value;
			}
		else
		{
			param.value = false;
		}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				param.type = type;
			}
		else
		{
			param.type = "param_type_none";
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				param.setup_callback = callback;
				param.callback_set = true;
			}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				param.block_start_index = block_start;
			}
		else
		{
			param.block_start_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				param.block_stop_index = block_stop;
			}
		else
		{
			param.block_stop_index = -1;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				param.block_setup_callback = block_callback;
				param.block_callback_set = true;
			}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->bool_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = param.key;

		if
			_CHECK_MASK(set_mask, _SET_VALUE)
			{
				new_param.value = value;
			}
		else
		{
			new_param.value = param.value;
		}

		if
			_CHECK_MASK(set_mask, _SET_TYPE)
			{
				new_param.type = type;
			}
		else
		{
			new_param.type = param.type;
		}

		if
			_CHECK_MASK(set_mask, _SET_CALLBACK)
			{
				// A new callback function - update
				new_param.setup_callback = callback;
				new_param.callback_set = true;
			}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}
		if
			_CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
			{
				new_param.block_start_index = block_start;
			}
		else
		{
			new_param.block_start_index = param.block_start_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
			{
				new_param.block_stop_index = block_stop;
			}
		else
		{
			new_param.block_stop_index = param.block_stop_index;
		}

		if
			_CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
			{
				new_param.block_setup_callback = block_callback;
				new_param.block_callback_set = true;
			}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}

		_settings->bool_parameters_map[name] = new_param;
	}

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) &&
		(_settings->bool_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
		_settings->bool_parameters_map[name].setup_callback(value, program);
	}

	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) &&
		(_settings->bool_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if ((_settings->bool_parameters_map[name].block_start_index >= 0) &&
			(_settings->bool_parameters_map[name].block_stop_index >=
			 _settings->bool_parameters_map[name].block_start_index))
		{
			for (int i = _settings->bool_parameters_map[name].block_start_index;
				 i <= _settings->bool_parameters_map[name].block_stop_index; i++)
			{
				_settings->bool_parameters_map[name].block_setup_callback(value, i, program);
			}
		}
	}

	//settings_manage_mutex.unlock();
	ModSynth::settings_handler_mutex.unlock();

	return _SETTINGS_OK;
}

/**
 * @brief	Set the specified string setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value callback is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::set_string_param_value(_settings_params_t *settings,
												string name,
												string value,
												uint16_t set_mask,
												int program)
{
	return set_string_param(settings, name, value, "", NULL, 0, 0, NULL, set_mask | _SET_VALUE, program);
}

/**
 * @brief	Set the specified integer setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value callback is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::set_int_param_value(_settings_params_t *settings,
											 string name,
											 int value,
											 uint16_t set_mask,
											 int program)
{
	return set_int_param(settings, name, value, 0, 0, "", NULL, 0, 0, NULL, set_mask | _SET_VALUE, program);
}

/**
 * @brief	Set the specified float setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value callback is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::set_float_param_value(_settings_params_t *settings,
											   string name,
											   double value,
											   uint16_t set_mask,
											   int program)
{
	return set_float_param(settings, name, value, 0, 0, "", NULL, 0, 0, NULL, set_mask | _SET_VALUE, program);
}

/**
 * @brief	Set the specified boolean setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value call back is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t Settings::set_bool_param_value(_settings_params_t *settings,
											  string name,
											  bool value,
											  uint16_t set_mask,
											  int program)
{
	return set_bool_param(settings, name, value, "", NULL, 0, 0, NULL, set_mask | _SET_VALUE, program);
}

/**
 *	Returns the index of map key value
 */
int Settings::look_for_key(std::string key, std::map<std::string, int> paramsMap)
{
	std::map<std::string, int>::iterator it;

	it = paramsMap.find(key);
	if (it != paramsMap.end())
		return it->second;
	else
		return -1;
}

