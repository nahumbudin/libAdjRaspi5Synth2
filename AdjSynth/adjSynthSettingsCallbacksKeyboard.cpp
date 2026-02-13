/**
*	@file		adjSynthSettingsCallbacksKeyboard.cpp
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.3
*						1. Code refactoring and notaion.
*
*	@brief		Callback to handle Keboard Settings
*	settings
*
*	History:\n
*
*	version	1.2	5-Feb-2021
*					1. Code refactoring and notaion.
*	version	1.1	27-12-2019
*	version 1.0	15_Nov-2019 First version
*
*/

#include "adjSynth.h"

int set_keyboard_portamento_cb(int port, int prog)
{
	AdjSynth::get_instance()->kbd1->set_portamento_time(port);
	return 0;
}

int set_keyboard_sensetivity_cb(int sens, int prog)
{
	AdjSynth::get_instance()->kbd1->set_sensitivity(sens);
	return 0;
}

int set_keyboard_sensetivity_low_cb(int sens, int prog)
{
	AdjSynth::get_instance()->kbd1->set_low_sensitivity(sens);
	return 0;
}

int set_keyboard_split_point_cb(int splt, int prog)
{
	AdjSynth::get_instance()->kbd1->set_split_point(splt);
	return 0;
}

int set_keyboard_polyphonic_mode_cb(int mod, int prog)
{
	AdjSynth::get_instance()->poly_mode = mod;
	return 0;
}

int set_keyboard_portamento_enable_state_cb(bool en, int prog)
{
	if (en)
	{
		AdjSynth::get_instance()->kbd1->enable_portamento();
	}
	else
	{
		AdjSynth::get_instance()->kbd1->disable_portamento();
	}
	return 0;
}

