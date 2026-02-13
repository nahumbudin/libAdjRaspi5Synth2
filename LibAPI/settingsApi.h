/**
* @file			settingsApi.h
*	@author		Nahum Budin
*	@date		25-Jul-2024
*	@version	1.0
*	
*	@brief		Settings API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <string>

#define _SAVE_FLUID_SETTINGS (1 << 1)

#define _READ_FLUID_SETTINGS _SAVE_FLUID_SETTINGS

/**
*   @brief  Save the active FluidSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_fluid_synth_settings_file(std::string path);

/**
*   @brief  Open settings parameters XML file and set it as the FluidSynth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_open_fluid_synth_settings_file(std::string path);

/**
*   @brief  Save the active AdjSynth patch parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_adj_synth_patch_file(string path);

/**
*   @brief  Open settings parameters XML file and set it as the AdjSynth active patch parameters
*   @param  path settings XML file full path
*	@param	channel	midi channel (1-16, 0 for active patch).
*   @return 0 if done
*/
int mod_synth_open_adj_synth_patch_file(std::string path, int channel);

/**
*   @brief  Gets a program patch name
*   @param  prog	program num (1-16)
*   @return program patch name
*/
std::string mod_synth_get_program_patch_name(int prog);

/**
*   @brief  Save the active AdjSynth settingd parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_adj_synth_settings_file(string path);

/**
*   @brief  Open settings parameters XML file and set it as the AdjSynth active settings parameters
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_open_adj_synth_settings_file(std::string path);

/**
*   @brief  Save the active AdjSynth general settingd parameters as XML file
*   @param  path general settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_mod_synth_general_settings_file(string path);

/**
*   @brief  Open general settings parameters XML file and set it as the AdjSynth active general settings parameters
*   @param  path general settings XML file full path
*   @return 0 if done
*/
int mod_synth_open_mod_synth_general_settings_file(std::string path);

/**
*   @brief  Save the active ModSynth preset parameters as XML file
*   @param  path preset XML file full path
*   @return 0 if done
*/
int mod_synth_save_mod_synth_preset_file(string path);

/**
*   @brief  Open and load preset parameters XML file and set it as the selected ModSynth preset parameters
*   @param  path preset XML file full path
*   @param	presetNum selected preset id (0 to _MAX_NUM_OF_PRESETS)
*   @return 0 if done
*/
int mod_synth_open_mod_synth_preset_file(std::string path, int presetNum);

/**
*   @brief  Set the active ModSynth preset 
*   @param  presetNum	selected preset (0 to _MAX_NUM_OF_PRESETS)
*   @return none
*/
void mod_synth_set_mod_synth_active_preset_num(int presetNum);

/**
*   @brief  Get the active ModSynth preset 
*   @param  None
*   @return 0 if done
*/
int mod_synth_get_mod_synth_active_preset_num();

/**
*   @brief  Get the preset summary string
*   @param  presetNum	selected preset (0 to _MAX_NUM_OF_PRESETS)
*   @return summary string text
*/
string *mod_synth_get_preset_summary_string(int presetNum);
