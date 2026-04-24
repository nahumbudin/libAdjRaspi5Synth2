/**
*	@file		analogReverberationPresets.cpp
*	@author		Nahum Budin
*	@date		17-Apr-2026
*	@version	1.0
*					1. Initial version.
*
*	@brief		Used for collecting and hndling the Analog Reverberation preset params
*
*	History:\n
*
*/

#include "instrumentAnalogReverbration.h"

int InstrumentAnalogReverbration::set_default_settings_parameters()
{
	int res = 0;
	
	return_val_if_true(active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = instrument_settings_manager->set_bool_param
		(active_settings_params,
		"adjsynth.reverb3m.enable_state",
		false,
		"analog-reverb-preset",
		set_reverb3m_enable_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		-1); // no program (common resource)
	
	res |= instrument_settings_manager->set_bool_param
				(active_settings_params,
		"adjsynth.reverb.enable_state",
		false,
		"analog-reverb-preset",
		set_reverb_enable_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb3m.preset",
		_SF_REVERB_PRESET_DEFAULT,
		_SF_REVERB_PRESET_LONGREVERB2,
		_SF_REVERB_PRESET_DEFAULT,
		"analog-reverb-preset",
		set_reverb3m_preset_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.room_size",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_room_size_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.damp",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_damp_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.wet",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_wet_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.dry",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_dry_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.mode",
		0,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_mode_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
		_SET_TYPE | _SET_CALLBACK,
		-1);
		
	return res;
}

int InstrumentAnalogReverbration::set_instrument_settings(_settings_params_t *preset)
{
	int res = 0;
	
	return_val_if_true(active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);	
	
	res = instrument_settings_manager->set_bool_param
		(active_settings_params,
		"adjsynth.reverb3m.enable_state",
		false,
		"analog-reverb-preset",
		set_reverb3m_enable_state_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK, // Ignore the values, only execute the callback function with current settings values.
		-1); // no program (common resource)
	
	res |= instrument_settings_manager->set_bool_param
				(active_settings_params,
		"adjsynth.reverb.enable_state",
		false,
		"analog-reverb-preset",
		set_reverb_enable_state_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb3m.preset",
		_SF_REVERB_PRESET_DEFAULT,
		_SF_REVERB_PRESET_LONGREVERB2,
		_SF_REVERB_PRESET_DEFAULT,
		"analog-reverb-preset",
		set_reverb3m_preset_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.room_size",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_room_size_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.damp",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_damp_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.wet",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_wet_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.dry",
		50,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_dry_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK,
		-1);
	
	res |= instrument_settings_manager->set_int_param
		(active_settings_params,
		"adjsynth.reverb.mode",
		0,
		100,
		0,
		"analog-reverb-preset",
		set_reverb_mode_cb,
		0,
		0,
		NULL,
		_EXEC_CALLBACK,
		-1);
		
	return res;
}

void InstrumentAnalogReverbration::set_active_preset(int preset_num)
{
	
	
}
	
int InstrumentAnalogReverbration::get_active_preset_num()
{
	
	return active_preset;
}
	
int InstrumentAnalogReverbration::get_active_preset_params(_settings_params_t *preset)
{
	
	return 0;
}
	
int InstrumentAnalogReverbration::set_active_preset_params(_settings_params_t *preset)
{
	
	return 0;
}
	
int InstrumentAnalogReverbration::get_number_of_presets()
{
	
	return _NUM_ANALOG_REVERBERATION_PRESETS;
}

// Settings callback functions implementations

int set_reverb3m_preset_cb(int pset, int prog)
{
	sf_presetreverb(&AdjSynth::get_instance()->audio_reverb->rv3m, 
		AdjSynth::get_instance()->get_sample_rate(), 
		(sf_reverb_preset)pset);
	return 0;
}

int set_reverb_room_size_cb(int rs, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_room_size(rs);
	return 0;
}

int set_reverb_damp_cb(int dmp, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_damp(dmp);
	return 0;
}

int set_reverb_wet_cb(int wt, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_wet(wt);
	return 0;
}

int set_reverb_dry_cb(int dry, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_dry(dry);
	return 0;
}

int set_reverb_width_cb(int wd, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_width(wd);
	return 0;
}

int set_reverb_mode_cb(int md, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_mode(md);
	return 0;
}

int set_reverb3m_enable_state_cb(bool en, int prog)
{
	if (en)
	{
		AdjSynth::get_instance()->audio_reverb->rev3m_enable();
	}
	else
	{
		AdjSynth::get_instance()->audio_reverb->rev3m_disable();
	}
	return 0;
}

int set_reverb_enable_state_cb(bool en, int prog)
{
	if (en)
	{
		AdjSynth::get_instance()->audio_reverb->rev_enable();
	}
	else
	{
		AdjSynth::get_instance()->audio_reverb->rev_disable();
	}
	return 0;
}
