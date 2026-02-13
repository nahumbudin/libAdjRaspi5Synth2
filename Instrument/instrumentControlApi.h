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

#pragma once

#include "../Settings/settings.h"

class InstrumentControlApi
{
  public:
	InstrumentControlApi(_settings_params_t *setting_params);
	~InstrumentControlApi();

	virtual int settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program = -1);
	virtual int settings_events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program = -1);
	virtual int settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program = -1);
	virtual int settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program = -1);
};
