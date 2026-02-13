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

#pragma once

#include "../LibAPI/types.h"

/* @class TimeSignature
 * 
 * The TimeSignature class represents
 * - The time signature of the song, such as 4/4, 3/4, or 6/8 time, and
 * - The number of pulses per quarter note
 * - The number of microseconds per quarter note
 *
 * In midi files, all time is measured in "pulses".  Each note has
 * a start time (measured in pulses), and a duration (measured in
 * pulses).  This class is used mainly to convert pulse durations
 * (like 120, 240, etc) into note durations (half, quarter, eighth, etc).
 */

class MidiFileTimeSignature
{
  public:
	MidiFileTimeSignature(int numera, int denomina, int quarter_not, int temp);

	int get_numerator();
	int get_denominator();
	int get_quarter();
	int get_mesure();
	int get_tempo();

	int get_measure_num(int time);
	int get_note_duration(int pulses);
	int get_stem_duration(midi_file_note_duration_t dur);
	int duration_to_time(midi_file_note_duration_t dur);
	std::string time_to_string();

  private:
	/** Numerator of the time signature */
	int numerator;
	/** Denominator of the time signature */
	int denominator;
	/** Number of pulses per quarter note */
	int quarter_note;
	/** Number of pulses per measure */
	int measure;
	/** Number of microseconds per quarter note */
	int tempo;
	
};


