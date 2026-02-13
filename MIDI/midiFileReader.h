/**
*	@file		midiFileReader.h
*	@author		Nahum Budin
*	@date		8-Aug-2024
*	@version	1.0
*
*	@brief		MIDI files low level reading.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#pragma once

/* @class MidiFileReader
 * The MidiFileReader is used to read low-level binary data from a midi file.
 * This class can do the following:
 *
 * - Peek at the next byte in the file.
 * - Read a byte
 * - Read a 16-bit big endian short
 * - Read a 32-bit big endian int
 * - Read a fixed length ascii string (not null terminated)
 * - Read a "variable length" integer.  .
 * - Skip ahead a given number of bytes
 * - Return the current offset.
 */

#include <stdint.h>
#include <list>
#include <vector>
#include <string>

#include "midiFileEvent.h"

class MidiFileReader
{
	
  public:
	MidiFileReader(std::vector<uint8_t> data);
	//MidiFileReader(std::list<uint8_t> data);

	uint8_t peek_byte();
	uint8_t read_byte();
	std::vector<uint8_t> read_bytes(int amount);
	int read_short();
	int read_int();
	std::string read_string(int len);
	int read_var_len();
	void skip(int amount);
	int get_offset();
	std::vector<uint8_t> get_data();


  private:
	void check_read(int amount);
	
	
	int data_length;
	int parse_offset;
	std::vector<uint8_t> data_bytes;
};
