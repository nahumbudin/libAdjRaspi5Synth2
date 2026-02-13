/**
* @file		gui.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		Provides the GUI API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include "types.h"

/* UI Callback Functions Registration */

/**
*   @brief  Register a callback function that initiates a full GUI update.
*			May handle multiple registrations.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(uint16_t) )
*   @return void
*/
void register_callback_update_ui(func_ptr_void_void_t ptr);

/**
*   @brief  Register a callback function that initiates a full GUI update.
*			Following a control box activity.
*			May handle multiple registrations.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(uint16_t) )
*   @return void
*/
void mod_synth_register_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr);

/**
*   @brief  Unregister a callback function that initiates a full GUI update.
* 			Following a control box activity.
*			May handle multiple registrations.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(uint16_t) )
*   @return void
*/
void mod_synth_unregister_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr);

/**
*   @brief  Register a callback function that update the midi player progress bar.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(int) )
*   @return void
*/
void mod_synth_register_midi_player_potision_update_callback(func_ptr_void_int_t ptr);

/**
*   @brief  Register a callback function that update the midi player song total playing time min:sec.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(int, int) )
*   @return void
*/
void mod_synth_register_midi_player_total_song_playing_time_update_callback(func_ptr_void_int_int_t ptr);

/**
*   @brief  Register a callback function that update the midi player song playing time min:sec.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(int, int) )
*   @return void
*/
void mod_synth_register_midi_player_song_playing_time_update_callback(func_ptr_void_int_int_t ptr);

/**
*   @brief  Register a callback function that update the midi player remaining song playing time min:sec.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(int, int) )
*   @return void
*/
void mod_synth_register_midi_player_song_remaining_playing_time_update_callback(func_ptr_void_int_int_t ptr);

/**
*   @brief  Register a callback function that update the midi mixer channels volume.
*   @param  func_ptr_void_int_int_t ptr  a pointer to the callback function ( void func(int, int) ) (chan, vol)
*   @return void
*/
void mod_synth_register_midi_mixer_channel_volume_update_callback(func_ptr_void_int_int_t ptr);

/**
*   @brief  Register a callback function that update the midi mixer channels pans.
*   @param  func_ptr_void_int_int_t ptr  a pointer to the callback function ( void func(int, int) ) (chan, vol)
*   @return void
*/
void mod_synth_register_midi_mixer_channel_pan_update_callback(func_ptr_void_int_int_t ptr);

/**
*   @brief  Register a callback function that update the midi mixer channels static volume state.
*   @param  func_ptr_void_int_bool_t ptr  a pointer to the callback function ( void func(int, bool) ) (chan, state)
*   @return void
*/
void mod_synth_register_midi_mixer_channel_static_volume_update_callback(func_ptr_void_int_bool_t ptr);


/**
*   @brief  Register a callback function that initiates a GUI update of the OSC1 Unison mode.
*   @param  funcPtrVoidInt ptr  a pointer to the callback function ( void func(int) )\n
*	Callback int parameter indicates the unison mode, that provides this mode labels.
*   @return void
*/
void mod_synth_register_set_osc_1_unison_mode_callback(func_ptr_void_int_t ptr);

/* Activation function */
void mod_synth_callback_set_osc_1_unison_mode(int mode);
