/**
* @file			patch.h
*	@author		Nahum Budin
*	@date		22-Sep-2025
*	@version	1.1
*					1. Code refactoring rename modules to instruments
*					2. Implementation: init connections before opening instruments as some instruments initialization are utilizing MIDI messages
*					3. Add Presets handling.
*	
*	@brief		Provides the Patches handling related API
*	
*	History:
*				Ver1.0	30-Aug-2024		Initial
*	
*/

#pragma once

#include "types.h"

int mod_synth_save_patch_file(std::string file_path);
int mod_synth_load_patch_file(std::string file_path);

void mod_synth_register_callback_get_active_instruments_names_list(func_ptr_vector_std_string_void_t ptr);

int mod_synth_load_hammond_preset_file(std::string path);
int mod_synth_save_hammond_preset_file(std::string path);

int mod_synth_load_string_synthesizer_preset_file(std::string path);
int mod_synth_save_string_synthesizer_preset_file(std::string path);

int mod_synth_load_pad_synthesizer_preset_file(std::string path);
int mod_synth_save_pad_synthesizer_preset_file(std::string path);

int mod_synth_load_keyboard_mapper_preset_file(std::string path);
int mod_synth_save_keyboard_mapper_preset_file(std::string path);

int mod_synth_load_patch_preset_synthesizer_preset_file(std::string path, int preset_index);
