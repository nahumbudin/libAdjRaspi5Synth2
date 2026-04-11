/**
* @file		instrumentAnalogEqualizer.cpp
*	@author		Nahum Budin
*	@date		11-Apr-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements an analog band equalizer instrument.
*	
*	History:\n
*	
*/

#include "instrumentAnalogEqualizer.h"

func_ptr_void_void_t InstrumentAnalogEqualizer::analog_equalizer_channel_gui_update_callback_ptr = NULL;

InstrumentAnalogEqualizer::InstrumentAnalogEqualizer()
	: Instrument(_INSTRUMENT_NAME_GRAPHIC_EQUALIZER_STR_KEY, false, false, false) // No MIDI input, no audio output, no MIDI output
{	
	active_preset = 0;
	
	
	
	instrument_analog_equalizer_instance = this;

	
}
		
InstrumentAnalogEqualizer::~InstrumentAnalogEqualizer()
{
	
}

void InstrumentAnalogEqualizer::update_analog_equalizer_gui()
{
	// Activate the GUI update callback to update the GUI with the current preset parameters values.
	// The GUI update call back will query the current preset parameters values and update the GUI accordingly.
	if (analog_equalizer_channel_gui_update_callback_ptr != NULL)
	{
		analog_equalizer_channel_gui_update_callback_ptr();
	}
}

void InstrumentAnalogEqualizer::register_analog_equalizer_channel_gui_update_callback(func_ptr_void_void_t ptr)
{
	analog_equalizer_channel_gui_update_callback_ptr = ptr;
}

void InstrumentAnalogEqualizer::analog_equalizer_channel_gui_update_callback()
{
	if (analog_equalizer_channel_gui_update_callback_ptr != NULL)
	{
		analog_equalizer_channel_gui_update_callback_ptr();
	}
}
