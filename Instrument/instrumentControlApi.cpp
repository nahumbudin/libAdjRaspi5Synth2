/**
* @file		instrumentControlApi.h
*	@author		Nahum Budin
*	@date		5-Jul-2024
*	@version	1.0	Initial release
*					
*	@brief		Provides the instrument control API
*				
*	
*	History:\n
*	
*/

#include "instrumentControlApi.h"

InstrumentControlApi::InstrumentControlApi(_settings_params_t *setting_params)
{
	
	
}

InstrumentControlApi::~InstrumentControlApi()
{
	
	
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	int val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int InstrumentControlApi::settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	double val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int InstrumentControlApi::settings_events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	bool val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int InstrumentControlApi::settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	string val event parameter value 
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int InstrumentControlApi::settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{

	return 0;
}