/**
*	@file		midiFileTimeSignature.h
*	@author		Nahum Budin
*	@date		7-Aug-2024
*	@version	1.0
*
*	@brief		Handles the MIDI time signature.
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#include <stdio.h>

#include "midiFileTimeSignature.h"

/** Creates a new time signature, with the given numerator,
 * denominator, pulses per quarter note, and tempo.
 */
MidiFileTimeSignature::MidiFileTimeSignature(int numera, int denomina, 
											 int quarter_not, int temp)
{
	if (numera <= 0 || denomina <= 0 || quarter_not <= 0)
	{
		printf("Invalid time signature");
	}
	else
	{
		numerator = numera;
		/* Midi File gives wrong time signature sometimes */
		if (numerator == 5)
		{
			numerator = 4;
		}
		
		denominator = denomina;
		quarter_note = quarter_not;
		tempo = temp;

		int beat;
		if (denominator < 4)
			beat = quarter_note * 2;
		else
			beat = quarter_note / (denominator / 4);

		measure = numerator * beat;
	}
}

/** Get the numerator of the time signature */
int MidiFileTimeSignature::get_numerator()
{
	return numerator;
}

/** Get the denominator of the time signature */
int MidiFileTimeSignature::get_denominator()
{
	return denominator;
}

/** Get the number of pulses per quarter note */
int MidiFileTimeSignature::get_quarter()
{
	return quarter_note;
}

/** Get the number of pulses per measure */
int MidiFileTimeSignature::get_mesure()
{
	return measure;
}

/** Get the number of microseconds per quarter note */
int MidiFileTimeSignature::get_tempo()
{
	return tempo;
}

/** Return which measure the given time (in pulses) belongs to. */
int MidiFileTimeSignature::get_measure_num(int time)
{
	return time / measure;
}

/** Given a duration in pulses, return the closest note duration. */
int MidiFileTimeSignature::get_note_duration(int pulses)
{
	int whole = quarter_note * 4;

	/**
         1       = 32/32
         3/4     = 24/32
         1/2     = 16/32
         3/8     = 12/32
         1/4     =  8/32
         3/16    =  6/32
         1/8     =  4/32 =    8/64
         triplet         = 5.33/64
         1/16    =  2/32 =    4/64
         1/32    =  1/32 =    2/64
         **/

	if (pulses >= 28 * whole / 32)
		return midi_file_note_duration_t::whole;
	else if (pulses >= 20 * whole / 32)
		return midi_file_note_duration_t::dotted_half;
	else if (pulses >= 14 * whole / 32)
		return midi_file_note_duration_t::half;
	else if (pulses >= 10 * whole / 32)
		return midi_file_note_duration_t::dotted_quarter;
	else if (pulses >= 7 * whole / 32)
		return midi_file_note_duration_t::quarter;
	else if (pulses >= 5 * whole / 32)
		return midi_file_note_duration_t::dotted_eighth;
	else if (pulses >= 6 * whole / 64)
		return midi_file_note_duration_t::eighth;
	else if (pulses >= 5 * whole / 64)
		return midi_file_note_duration_t::triplet;
	else if (pulses >= 3 * whole / 64)
		return midi_file_note_duration_t::sixteenth;
	else
		return midi_file_note_duration_t::thirty_second;
}

/** Converts a note duration into a stem duration.  Dotted durations
  * are converted into their non-dotted equivalents.
  */
int MidiFileTimeSignature::get_stem_duration(midi_file_note_duration_t dur)
{
	if (dur == midi_file_note_duration_t::dotted_half)
		return midi_file_note_duration_t::half;
	else if (dur == midi_file_note_duration_t::dotted_quarter)
		return midi_file_note_duration_t::quarter;
	else if (dur == midi_file_note_duration_t::dotted_eighth)
		return midi_file_note_duration_t::eighth;
	else
		return dur;
}

int MidiFileTimeSignature::duration_to_time(midi_file_note_duration_t dur)
{
	const int eighth = quarter_note / 2;
	const int sixteenth = eighth / 2;

	switch (dur)
	{
		case midi_file_note_duration_t::whole:
			return quarter_note * 4;

		case midi_file_note_duration_t::dotted_half:
			return quarter_note * 3;

		case midi_file_note_duration_t::half:
			return quarter_note * 2;

		case midi_file_note_duration_t::dotted_quarter:
			return 3 * eighth;

		case midi_file_note_duration_t::quarter:
			return quarter_note;

		case midi_file_note_duration_t::dotted_eighth:
			return 3 * sixteenth;

		case midi_file_note_duration_t::eighth:
			return eighth;

		case midi_file_note_duration_t::triplet:
			return quarter_note / 3;

		case midi_file_note_duration_t::sixteenth:
			return sixteenth;

		case midi_file_note_duration_t::thirty_second:
			return sixteenth / 2;
			
		default:
			return 0;
	}
}

std::string MidiFileTimeSignature::time_to_string()
{
	char str[256];

	sprintf(str, "TimeSignature=%1$i/%2$i quarter=%3$i tempo=%4$i",
						 numerator, denominator, quarter_note, tempo);

	std::string time_str(str);

	return time_str;
}
