/**
* @file		instrumentAnalogReverbration.cpp
*	@author		Nahum Budin
*	@date		17-Apr-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements an Analog Reverberation instrument.
*	
*	History:\n
*	
*/

#include "instrumentAnalogReverbration.h"

func_ptr_void_void_t InstrumentAnalogReverbration::update_gui_callback_ptr = NULL;

InstrumentAnalogReverbration::InstrumentAnalogReverbration()
	: Instrument(_INSTRUMENT_NAME_REVERB_STR_KEY, false, false, false, 
				 NULL, NULL)
{
	
	active_preset = 0;
	
	instrument_analog_reverbration_instance = this;
	
	// Set the settings parameters of the common resources (not program specific) to their default values.
	set_default_settings_parameters();
}

InstrumentAnalogReverbration::~InstrumentAnalogReverbration()
{
}

void InstrumentAnalogReverbration::update_analog_reverberation_gui()
{
	if (update_gui_callback_ptr)
	{
		update_gui_callback_ptr();
	}
}

/**
*   @brief  Initiates a reverbration effect related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ROOM_SIZE, _REVERB_DAMP, _REVERB_WET, _REVERB_DRY\n
*				_REVERB_WIDTH, _REVERB_MODE, _REVERB_PRESET
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ROOM_SIZE: 10-98\n
*				_REVERB_DAMP: 0-100\n
*				_REVERB_WET: 0-100\n
*				_REVERB_DRY: 0-100\n
*				_REVERB_WIDTH: 1-100\n
*				_REVERB_MODE: 0-50
*				_REVERB_PRESET: enum sf_reverb_preset (defined in LibAPI/synthesizer.h)\n
*
*   @return 0
*/
int InstrumentAnalogReverbration::api_settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{
	int res = 0;
	
	if (params == NULL)
	{
		return _SETTINGS_BAD_PARAMETERS;
	}
	
	if (moduleid != _REVERB_EVENT)
	{
		return _SETTINGS_BAD_PARAMETERS;
	}
	
	if (paramid == _REVERB_PRESET)
	{
		res = instrument_settings_manager->set_int_param_value
				(params,
				"adjsynth.reverb3m.preset",
				val,
				_EXEC_CALLBACK,
				-1); // -1: no program (common resource)
	}
	else if (paramid == _REVERB_ROOM_SIZE)
	{
		res = instrument_settings_manager->set_int_param_value
				(params,
				"adjsynth.reverb.room_size",
				val,
				_EXEC_CALLBACK,
				-1);
	}
	else if (paramid == _REVERB_DAMP)
	{
		res = instrument_settings_manager->set_int_param_value
				(params,
				"adjsynth.reverb.damp",
				val,
				_EXEC_CALLBACK,
				-1);
	}
	else if (paramid == _REVERB_WET)
	{
		res = instrument_settings_manager->set_int_param_value
				(params,
				"adjsynth.reverb.wet",
				val,
				_EXEC_CALLBACK,
				-1);
	}
	else if (paramid == _REVERB_DRY)
	{
		res = instrument_settings_manager->set_int_param_value
				(params,
			"adjsynth.reverb.dry",
			val,
			_EXEC_CALLBACK,
			-1);
	}
	else if (paramid == _REVERB_WIDTH)
	{
		instrument_settings_manager->set_int_param_value
				(params,
				"adjsynth.reverb.width",
				val,
				_EXEC_CALLBACK,
				-1);
	}
	else if (paramid == _REVERB_MODE)
	{
		res = instrument_settings_manager->set_int_param_value
				(params,
				"adjsynth.reverb.mode",
				val,
				_EXEC_CALLBACK,
				-1);
	}
	else {
		
	}	
	
	return res;
}
int InstrumentAnalogReverbration::api_settings_events_handler(int moduleid, int paramid, float val, _settings_params_t *params, int program)
{
	int res = 0;
	
	
	return res;
}

/**
*   @brief  Initiates a reverbration effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE: true - enabled; false - disabled\n
*
*   @return 0
*/
int InstrumentAnalogReverbration::api_settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	int res = 0;
	
	switch (paramid)
	{
	case _REVERB_ENABLE:
		res = instrument_settings_manager->set_bool_param_value
					(params,
				"adjsynth.reverb.enable_state",
				val,
				_EXEC_CALLBACK,
				-1);
		
		// Only 1 reverb active - disable the other reverb state
		if (val)
		{
			res = instrument_settings_manager->set_bool_param_value
					(params,
					"adjsynth.reverb3m.enable_state",
					false,
					_EXEC_CALLBACK,
					-1);	
		}
		break;

	case _REVERB3M_ENABLE:
		res = instrument_settings_manager->set_bool_param_value
					(params,
				"adjsynth.reverb3m.enable_state",
				val,
				_EXEC_CALLBACK,
				-1);
		
		// Only 1 reverb active
		if (val)
		{
			res = instrument_settings_manager->set_bool_param_value
					(params,
					"adjsynth.reverb.enable_state",
					false,
					_EXEC_CALLBACK,
					-1);	
		}

		break;
	}

	return res;
}
int InstrumentAnalogReverbration::api_settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{
	int res = 0;
	
	
	return res;
}

void InstrumentAnalogReverbration::register_update_gui_callback(func_ptr_void_void_t ptr)
{
	update_gui_callback_ptr = ptr;
}


