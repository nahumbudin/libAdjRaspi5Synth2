/**
* @file			types.h
*	@author		Nahum Budin
*	@date		22-Sep-205
*	@version	1.1
*					1. Refactoring rename modules to instruments
*	
*	@brief		Types definitions
*	
*	History:
*				Ver 1.0  6-Jul-2024	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "../MIDI/midiFileEvent.h"  // TODO:

/* Types Definitions */
using namespace std;

/* used for holding soundfont presets */
typedef struct _soundfont_presets_data
{
	int sound_font_id;
	char *presetname;
	int banknum;
	int program;
} _soundfont_presets_data_t;

/* Holds instruments IDs */
typedef enum
{
	none_instrument_id = -1,
	fluid_synth,
	adj_hammond_organ,
	adj_analog_synth,
	adj_karplusstrong_string_synth,
	adj_morphed_sin_synth,
	adj_pad_synth,
	adj_midi_player,
	adj_reverb_effect,
	adj_distortion_effect,
	adj_graphic_equilizer,
	midi_mixer,
	adj_midi_mapper,
	adj_ext_midi_interface,
	adj_keyboard_control

} en_instruments_ids_t;

/* Holds instruments types */
typedef enum
{
	none_instrument_type = -1,
	synth,
	effect,
	player,
	control,
	interface,
	keyboard
	
} en_instruments_types_t;

typedef enum 
{
	_SF_REVERB_PRESET_DEFAULT,
	_SF_REVERB_PRESET_SMALLHALL1,
	_SF_REVERB_PRESET_SMALLHALL2,
	_SF_REVERB_PRESET_MEDIUMHALL1,
	_SF_REVERB_PRESET_MEDIUMHALL2,
	_SF_REVERB_PRESET_LARGEHALL1,
	_SF_REVERB_PRESET_LARGEHALL2,
	_SF_REVERB_PRESET_SMALLROOM1,
	_SF_REVERB_PRESET_SMALLROOM2,
	_SF_REVERB_PRESET_MEDIUMROOM1,
	_SF_REVERB_PRESET_MEDIUMROOM2,
	_SF_REVERB_PRESET_LARGEROOM1,
	_SF_REVERB_PRESET_LARGEROOM2,
	_SF_REVERB_PRESET_MEDIUMER1,
	_SF_REVERB_PRESET_MEDIUMER2,
	_SF_REVERB_PRESET_PLATEHIGH,
	_SF_REVERB_PRESET_PLATELOW,
	_SF_REVERB_PRESET_LONGREVERB1,
	_SF_REVERB_PRESET_LONGREVERB2
} sf_reverb_preset;

/** The possible note durations */
typedef enum midi_file_note_duration{

	thirty_second,
	sixteenth,
	triplet,
	eighth,
	dotted_eighth,
	quarter,
	dotted_quarter,
	half,
	dotted_half,
	whole} midi_file_note_duration_t;

/* Callback and functions pointers */

/* void foo(void) function ptr */
typedef void (*func_ptr_void_void_t)(void);
/* void foo(bool) function pointer */
typedef void (*func_ptr_void_bool_t)(bool);
/* void foo(int) function pointer */
typedef void (*func_ptr_void_int_t)(int);
/* void foo(std::string) function pointer */
typedef void (*func_ptr_void_string_t)(std::string);
/* void foo(int, uint16_t) function pointer */
typedef void (*func_ptr_void_int_uint16_t)(int, uint16_t);
/* void foo(uint8_t, uint8_t) function pointer */
typedef void (*func_ptr_void_uint8_t_uint8_t)(uint8_t, uint8_t);
/* void foo(int, int) function pointer */
typedef void (*func_ptr_void_int_int_t)(int, int);

/* void foo(int, bool) function pointer */
typedef void (*func_ptr_void_int_bool_t)(int, bool);

/* void foo(std::vector<MidiFileEvent>, int) function pointer */
typedef void (*func_ptr_void_midi_events_vector_int)(std::vector<MidiFileEvent>, int);

/* void foo(std::vector<int>) function pointer */
typedef void (*func_ptr_void_int_vector_t)(std::vector<int>);

/* std::vector<std::string> foo(void) function pointer */
typedef std::vector<std::string>(*func_ptr_vector_std_string_void_t)(void);

/* void foo(en_instruments_ids_t) function pointer */
typedef void(*func_ptr_void_en_instruments_ids_t_t)(en_instruments_ids_t);

