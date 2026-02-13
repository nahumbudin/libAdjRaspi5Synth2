/**
*	@file		midiConversions.h
*	@author		Nahum Budin
*	@date		17-Aug-2024
*	@version	1.0
*
*	@brief		Convert MIDI events to bytes.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#pragma once

#include <vector>
#include <stddef.h>

#include "midiFileEvent.h"

bool convert_midi_event_to_mssg(MidiFileEvent event,std::vector<uint8_t> *bytes=NULL, int vol = 100);
