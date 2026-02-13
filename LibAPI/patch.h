/**
* @file			patch.h
*	@author		Nahum Budin
*	@date		22-Sep-2025
*	@version	1.1
*					1. Code refactoring rename modules to instruments
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


