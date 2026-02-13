/**
* @file			types.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		Types definitions
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <stdint.h>

/* Types Definitions */
using namespace std;

/* used for holding soundfont presets */
typedef struct _soundfont_presets_data
{
	int soundFontId;
	char *presetname;
	int banknum;
	int program;
} _soundfont_presets_data_t;

/* Holds moduls IDs */
typedef enum
{
	none = -1,
	fluid_synth,
	adj_hammond_organ,
	adj_analog_synth,
	adj_karplusstrong_string_synth,
	adj_morphed_sin_synth,
	adj_pad_synth,
	adj_reverb_effect,
	adj_distortion_effect,
	adj_graphic_equilizer,
	adj_output_mixer,
	adj_midi_mapper,
	adj_ext_midi_interface,
	adj_keyboard_control,

} en_modules_ids_t;

/* Callback and functions pointers */

/* void foo(bool) function pointer */
typedef void (*func_ptr_void_bool_t)(bool);

/* void foo(int, uint16_t) function pointer */
typedef void (*func_ptr_void_int_uint16_t)(int, uint16_t);
