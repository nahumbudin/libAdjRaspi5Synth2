/**
*	@file		midiFileException.h
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

#pragma once

#include <string>

/* @class MidiFileException
 * A MidiFileException is thrown when an error occurs
 * while parsing the Midi File.  The constructore takes
 * the file offset (in bytes) where the error occurred,
 * and a string describing the error.
 */
class MidiFileException
{
  public:
	MidiFileException(std::string text, int offset);
};
