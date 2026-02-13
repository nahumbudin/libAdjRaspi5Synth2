/**
* @file			fluidSynth.h
*	@author		Nahum Budin
*	@date		19-Jul-2024
*	@version	1.0
*	
*	@brief		FluidSynth API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <string>
#include <vector>

#include "types.h"
#include "../Settings/settings.h"

#define _AFS_REVERB_ENABLED true
#define _AFS_REVERB_DISABLED false

#define _AFS_CHORUS_ENABLED true
#define _AFS_CHORUS_DISABLED false

#define _AFS_CHORUS_WAVEFORM_SINE 0
#define _AFS_CHORUS_WAVEFORM_TRIANGLE 1

#define _NUM_OF_FLUID_PRESETS 3


/**
*   @brief  Sets the Fluid synth volume level
*   @param  volume (0-200)
*   @return void
*/
void mod_synth_set_fluid_synth_volume(int vol);

/**
*   @brief  Sets the Fluid synth reverb room size
*   @param  room size (0-120)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_room_size(int val);

/**
*   @brief  Sets the Fluid synth reverb damp
*   @param  damp level (0-100)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_damp(int val);

/**
*   @brief  Sets the Fluid synth reverb width
*   @param  width level (0-100)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_width(int val);

/**
*   @brief  Sets the Fluid synth reverb level
*   @param  reverb level (0-100)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_level(int val);

/**
*   @brief  Sets the Fluid synth chorus number of stages
*   @param  number of stages (0-99)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_number(int val);

/**
*   @brief  Sets the Fluid synth chorus level
*   @param  chorus level (0-99)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_level(int val);

/**
*   @brief  Sets the Fluid synth chorus speed (Hz)
*   @param  chorus level (30-500)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_speed(int val);

/**
*   @brief  Sets the Fluid synth chorus depth
*   @param  depth (0-210)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_depth(int val);

/**
*   @brief  Sets the Fluid synth waveform
*   @param  waveform _FLUID_CHORUS_MOD_SINE (0) or _FLUID_CHORUS_MOD_TRIANGLE (1)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_waveform(int val);

/**
*   @brief  Enable the Fluid synth reverb effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_enable_reverb();

/**
*   @brief  Disable the Fluid synth reverb effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_disable_reverb();

/**
*   @brief  Enable the Fluid synth chorus effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_enable_chorus();

/**
*   @brief  Disable the Fluid synth chorus effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_disable_chorus();

/**
*  @brief  Return the current fluid synth volume set value
*  @param  non
*  @return the the current fluid synth volume set value
*/
int mod_synth_get_active_fluid_synth_volume();

/**
*   @brief  Return the current fluid synth reverb room size
*   @param  non
*   @return the the current fluid synth reverb room size
*/
int mod_synth_get_active_fluid_synth_reverb_room_size();

/**
*   @brief  Return the current fluid synth reverb damp
*   @param  non
*   @return the the current fluid synth reverb damp level
*/
int mod_synth_get_active_fluid_synth_reverb_damp();

/**
*   @brief  Return the current fluid synth reverb width
*   @param  non
*   @return the the current fluid synth reverb width level
*/
int mod_synth_get_active_fluid_synth_reverb_width();

/**
*   @brief  Return the current fluid synth reverb level
*   @param  non
*   @return the the current fluid synth level
*/
int mod_synth_get_active_fluid_synth_reverb_level();

/**
*   @brief  Return the current fluid synth chorus number
*   @param  non
*   @return the the current fluid synth chorus number
*/
int mod_synth_get_active_fluid_synth_chorus_number();

/**
*   @brief  Return the current fluid synth chorus level
*   @param  non
*   @return the the current fluid synth chorus level
*/
int mod_synth_get_active_fluid_synth_chorus_level();

/**
*   @brief  Return the current fluid synth chorus speed
*   @param  non
*   @return the the current fluid synth chorus speed
*/
int mod_synth_get_active_fluid_synth_chorus_speed();

/**
*   @brief  Return the current fluid synth chorus depth
*   @param  non
*   @return the the current fluid synth chorus depth
*/
int mod_synth_get_active_fluid_synth_chorus_depth();

/**
*   @brief  Return the current fluid synth chorus modulation waveform
*   @param  non
*   @return the the current fluid synth chorus modulation waveform 0-sine; 1-triangle
*/
int mod_synth_get_active_fluid_synth_chorus_waveform();

/**
*   @brief  Return the current fluid synth reverb activation state
*   @param  non
*   @return the the current fluid synth reverb activation state
*/
bool mod_synth_get_active_fluid_synth_reverb_activation_state();

/**
*   @brief  Return the current fluid synth chorus activation state
*   @param  non
*   @return the the current fluid synth chorus activation state
*/
bool mod_synth_get_active_fluid_synth_chorus_activation_state();

/**
*   @brief  Select a SoundFont file for the fluid synth
*   @param  path full path of the sound font file
*   @return void
*/
void mod_synth_set_fluid_synth_soundfont(string path);

/**
* Sets the FluidSynth soundfont file settings value, load and activate it.
*
* @param none
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
void mod_synth_load_and_set_fluid_synth_soundfont(string path);

/**
*   @brief  Select an instrument on a MIDI channel by SoundFont ID, bank and program numbers.
*   @param  chan MIDI channel 0-15
*	@param	sfid ID of loaded sondfone
*	@param	bank bank number
*	@param	program MIDI program number
*   @return 0 on successes -1 otherwise
*/
int mod_synth_set_fluid_synth_select_program(int chan,
											 unsigned int sfid,
											 unsigned int bank,
											 unsigned int program);

/**
*   @brief  Get the name of the soundfont id
*   @param  int sfid
*   @return  soundfont name
*/
std::string mod_synth_get_fluid_synth_soundfont_id_name(int sfid);

/**
*   @brief  Get the presets of all fluid soundfont files.
*	@param	presets a pointer to a _soundfont_presets_data_t vector to return
*			the specified sound font presets data
*   @return 0 if OK
*/
int mod_synth_get_fluid_synth_loaded_soundfonts_presets(std::vector<_soundfont_presets_data_t> *presets);

/**
*   @brief  Get the current fluid synth channel bank
*   @param  int chan
*   @return bank
*/
int mod_synth_get_fluid_synth_channel_bank(int chan);

/**
*   @brief  Get the current fluid synth channel program
*   @param  int chan
*   @return program
*/
int mod_synth_get_fluid_synth_channel_program(int chan);

/**
*   @brief  Get the current fluid synth channel preset name
*   @param  int chan
*   @return preset name
*/
std::string mod_synth_get_fluid_synth_channel_preset_name(int chan);

/**
*   @brief  Get the current fluid synth channel sfont id
*   @param  int chan
*   @return sfont id
*/
int mod_synth_get_fluid_synth_channel_sfont_id(int chan);

/**
*   @brief  Get the current fluid synth channel sfont name
*   @param  int chan
*   @return sfont id
*/
std::string mod_synth_get_fluid_synth_channel_preset_soundfont_name(int chan);

/**
*   @brief  Select an instrument on a MIDI channel by SoundFont ID, bank and program numbers.
*   @param  chan MIDI channel 0-15
*	@param	sfid ID of loaded sondfone
*	@param	bank bank number
*	@param	program MIDI program number
*   @return 0 on successes -1 otherwise
*/
int mod_synth_set_fluid_synth_program_select(int chan,
											 unsigned int sfid,
											 unsigned int bank,
											 unsigned int program);

/**
*   @brief  Collects the active FluidSynth parameters into a settings parameters struct. \n
*   @param  a pointer to a settingg struct. \n
*   @return 0 - OK; -1 Error.
*/
int mod_synth_collect_fluid_synth_preset_parms(_settings_params_t *params);

/**
*   @brief  Save a preset parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_fluid_synth_preset_file(string path);

/**
*   @brief  Open and load preset parameters XML file and set it as the selected FluidSynth preset parameters
*   @param  path preset XML file full path
*   @param	presetNum selected preset id (0 to _MAX_NUM_OF_PRESETS)
*   @return 0 if done
*/
int mod_synth_open_fluid_synth_preset_file(string path, int preset_num);

/**
*   @brief  Register a callback function that initiates a GUI update of FluidSynth module\n
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )\n
*   @return None
*/
void mod_synth_fluid_synth_register_ui_update_callback(func_ptr_void_void_t ptr);

/**
*   @brief  Activate a callback function that initiates a GUI update of the FluidSynth module\n
*   @param  None\n
*   @return None
*/
void mod_synth_fluid_synth_activate_ui_update_callback();

/**
*   @brief  Register a callback function that updates the UI FluidSynth preset name text display.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
void mod_synth_fluid_synth_register_callback_update_preset_text(func_ptr_void_string_t ptr);

/**
*   @brief  Activate a callback function that updates the UI FluidSynth preset name text display\n
*   @param  None\n
*   @return None
*/
void mod_synth_fluid_synth_activate_ui_update_preset_text_callback(std::string text);

/**
*   @brief  Sets the active FluidSynth preset number\n
*   @param  preset num\n
*   @return None
*/
void mod_synth_fluid_synth_set_active_preset(int preset_num);

/**
*   @brief  Gets the active FluidSynth preset number\n
*   @param  None\n
*   @return preset num
*/
int mod_synth_fluid_synth_get_active_preset();
