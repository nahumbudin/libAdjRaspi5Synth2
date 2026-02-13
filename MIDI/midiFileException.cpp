/**
*	@file		midiFileException.cpp
*	@author		Nahum Budin
*	@date		10-Aug-2024
*	@version	1.0
*
*	@brief		MIDI files execptions handling.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*/

#include <stdexcept>

#include "midiFileException.h"

MidiFileException::MidiFileException(std::string text, int offset)
{
	throw(text + " at offset " + std::to_string(offset));
}
