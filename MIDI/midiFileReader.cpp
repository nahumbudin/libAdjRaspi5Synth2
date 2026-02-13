/**
*	@file		midiFileReader.cpp
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

#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include "midiFileReader.h"
#include "midiFileException.h"

/** Creates a new MidiFileReader from the given data */
//MidiFileReader::MidiFileReader(std::list<uint8_t> data)
MidiFileReader::MidiFileReader(std::vector<uint8_t> data)
{
	data_length = data.size();
	parse_offset = 0;
	data_bytes = data;

	//std::list<uint8_t>::iterator it = data.begin();

	/* Convert the data list into a vector */
	//for (int i = 0; i < data_length; i++)
	//{
	//	data_bytes.push_back(*it);
	//	it++;
	//}
}

/** Checks that the given number of bytes doesn't exceed the file size, if exceeding, return -1 */
void MidiFileReader::check_read(int amount)
{
	if (parse_offset + amount > data_length)
	{
		throw new MidiFileException("File is truncated", parse_offset);
	}
}

/** Reads the next byte in the file, but don't increment the parse offset */
uint8_t MidiFileReader::peek_byte()
{
	check_read(1);
	
	return data_bytes.at(parse_offset);
}

/** Reads the next byte in the file, and increments the parse offset */
uint8_t MidiFileReader::read_byte()
{
	check_read(1);

	uint8_t byte =  data_bytes.at(parse_offset);
	parse_offset++;

	return byte;
}

/** Read the given number of bytes from the file */
std::vector<uint8_t> MidiFileReader::read_bytes(int amount)
{
	std::vector<uint8_t> result(amount);
	
	if (amount == 0)
	{
		result.push_back(0);
		return result;
	}

	check_read(amount);
	for (int i = 0; i < amount; i++)
	{
		result.at(i) = data_bytes.at(i + parse_offset);
	}
	parse_offset += amount;
	
	return result;
}

/** Read a 16-bit short from the file */
int MidiFileReader::read_short()
{
	check_read(2);
	
	int x = ((data_bytes.at(parse_offset) & 0xFF) << 8) |
			(data_bytes.at(parse_offset + 1) & 0xFF);
	
	parse_offset += 2;
	
	return x;
}

/** Read a 32-bit short from the file */
int MidiFileReader::read_int()
{
	check_read(4);

	int x = ((data_bytes.at(parse_offset) & 0xFF) << 24) |
			((data_bytes.at(parse_offset + 1) & 0xFF) << 16) |
			((data_bytes.at(parse_offset + 2) & 0xFF) << 8) |
			((data_bytes.at(parse_offset + 3) & 0xFF));

	parse_offset += 4;

	return x;
}

/** Read a String with the given length */
std::string MidiFileReader::read_string(int len)
{
	char chars[len];

	check_read(len);

	for (int i = 0; i < len; i++)
	{
		chars[i] = data_bytes.at(parse_offset + i);
	}

	parse_offset += len;

	std::string result(chars, len);

	return result;
}

/** Read a variable-length integer (1 to 4 bytes). 
 * The integer ends when we encounter a byte that doesn't have the 8th bit set
 * (a byte less than 0x80).
*/
int MidiFileReader::read_var_len()
{
	int result = 0;
	uint8_t b;

	b = read_byte();
	result = (int)(b & 0x7f);

	for (int i = 0; i < 3; i++)
	{
		if ((b & 0x80) != 0)
		{
			b = read_byte();
			result = (int)((result << 7) + (b & 0x7f));
		}
		else
		{
			break;
		}
	}
	
	return (int)result;
}

/** Skip over the given number of bytes */
void MidiFileReader::skip(int amount)
{
	check_read(amount);
	parse_offset += amount;
}

/** Return the current parse offset */
int MidiFileReader::get_offset()
{
	return parse_offset;
}

/** Return the raw midi file byte data */
std::vector<uint8_t> MidiFileReader::get_data()
{
	return data_bytes;
}
