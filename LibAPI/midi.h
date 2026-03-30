/**
 * @file			midi.h
 *	@author		Nahum Budin
 *	@date		21-Dec-2025
 *	@version	1.1
 *					1. Increase the number of max midi devices and ports to 32
 *					2. Added MIDI mixer Pan modulation LFO level and selection control definitions
 *					3. Added MIDI mixer Send control definition
 *					4. Added MIDI Player Backward and Forward control and auto loop back playing control.
 *					5. Added MIDI Player playback volume control
 *
 *	@brief		Provides the MIDI related API
 *
 *	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
 *
 *	History:
 *		version 1.0		6-Jul-2024
 */

#pragma once

#include <string>
#include <vector>

#include "../LibAPI/types.h"

#define MAX_NUM_OF_MODULES 7

#define _MAX_NUM_OF_MIDI_DEVICES 32
#define _MAX_NUM_OF_MIDI_DEVICE_PORTS 32
#define _MIDI_DEVICE_INPUT 0  // an input midi device (synth)
#define _MIDI_DEVICE_OUTPUT 1 // an output midi device (keyboard)

#define _MIDI_EXT_INTERFACE_SERIAL_PORT_NUM 22 // ttyAMA0

#define _MIDI_MAPPING_MODE 1400

#define _MIDI_MAPPING_MODE_SKETCH 0
#define _MIDI_MAPPING_MODE_MAPPING 1
#define _DEFAULT_MIDI_MAPPING_MODE _MIDI_MAPPING_MODE_SKETCH // _MIDI_MAPPING_MODE_MAPPING

#define _MIDI_EVENT_NOTE_OFF 0x80
#define _MIDI_EVENT_NOTE_ON 0x90
#define _MIDI_EVENT_KEY_PRESSURE 0xA0
#define _MIDI_EVENT_CONTROL_CHANGE 0xB0
#define _MIDI_EVENT_PROGRAM_CHANGE 0xC0
#define _MIDI_EVENT_CHANNEL_PRESSURE 0xD0
#define _MIDI_EVENT_PITCH_BEND 0xE0
#define _MIDI_META_SYSEX_1 0xF0
#define _MIDI_META_SYSEX_2 0xF7

#define _MIDI_ALL_SOUNDS_OFF 0x78
#define _MIDI_ALL_NOTES_OFF	0x7B

#define _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2 0x07
#define _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2 0x08

#define _MIDI_EVENT_CHANNEL_SEND_BYTE_2					0x66	// Non standard Control Change for channel send level
#define _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2	0x67	// Non standard Control Change for channel pan modulation LFO level
#define _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2	0x68	// Non standard Control Change for channel pan modulation LFO selection

#define _MIDI_META_EVENT 0xFF
#define _MIDI_META_EVENT_SEQUENCE 0x00
#define _MIDI_META_EVENT_TEXT 0x01
#define _MIDI_META_EVENT_COPYRIGHT 0x02
#define _MIDI_META_EVENT_SEQUENCE_NAME 0x03
#define _MIDI_META_EVENT_INSTRUMENT 0x04
#define _MIDI_META_EVENT_LYRIC 0x05
#define _MIDI_META_EVENT_MARKER 0x06
#define _MIDI_META_EVENT_END_OF_TRACK 0x2F
#define _MIDI_META_EVENT_LYRICS 0x05
#define _MIDI_META_EVENT_TEMPO 0x51
#define _MIDI_META_EVENT_SMPTE_OFFSET 0x54
#define _MIDI_META_EVENT_TIME_SIGNATURE 0x58
#define _MIDI_META_EVENT_KEY_SIGNATURE 0x59

/* The Program Change event gives the instrument that should
     * be used for a particular channel.  The following table
     * maps each instrument number (0 thru 128) to an instrument
     * name.
     */

static std::string midi_instruments_names[129] = {
	"Acoustic Grand Piano",
	"Bright Acoustic Piano",
	"Electric Grand Piano",
	"Honky-tonk Piano",
	"Electric Piano 1",
	"Electric Piano 2",
	"Harpsichord",
	"Clavi",
	"Celesta",
	"Glockenspiel",
	"Music Box",
	"Vibraphone",
	"Marimba",
	"Xylophone",
	"Tubular Bells",
	"Dulcimer",
	"Drawbar Organ",
	"Percussive Organ",
	"Rock Organ",
	"Church Organ",
	"Reed Organ",
	"Accordion",
	"Harmonica",
	"Tango Accordion",
	"Acoustic Guitar (nylon)",
	"Acoustic Guitar (steel)",
	"Electric Guitar (jazz)",
	"Electric Guitar (clean)",
	"Electric Guitar (muted)",
	"Overdriven Guitar",
	"Distortion Guitar",
	"Guitar harmonics",
	"Acoustic Bass",
	"Electric Bass (finger)",
	"Electric Bass (pick)",
	"Fretless Bass",
	"Slap Bass 1",
	"Slap Bass 2",
	"Synth Bass 1",
	"Synth Bass 2",
	"Violin",
	"Viola",
	"Cello",
	"Contrabass",
	"Tremolo Strings",
	"Pizzicato Strings",
	"Orchestral Harp",
	"Timpani",
	"String Ensemble 1",
	"String Ensemble 2",
	"SynthStrings 1",
	"SynthStrings 2",
	"Choir Aahs",
	"Voice Oohs",
	"Synth Voice",
	"Orchestra Hit",
	"Trumpet",
	"Trombone",
	"Tuba",
	"Muted Trumpet",
	"French Horn",
	"Brass Section",
	"SynthBrass 1",
	"SynthBrass 2",
	"Soprano Sax",
	"Alto Sax",
	"Tenor Sax",
	"Baritone Sax",
	"Oboe",
	"English Horn",
	"Bassoon",
	"Clarinet",
	"Piccolo",
	"Flute",
	"Recorder",
	"Pan Flute",
	"Blown Bottle",
	"Shakuhachi",
	"Whistle",
	"Ocarina",
	"Lead 1 (square)",
	"Lead 2 (sawtooth)",
	"Lead 3 (calliope)",
	"Lead 4 (chiff)",
	"Lead 5 (charang)",
	"Lead 6 (voice)",
	"Lead 7 (fifths)",
	"Lead 8 (bass + lead)",
	"Pad 1 (new age)",
	"Pad 2 (warm)",
	"Pad 3 (polysynth)",
	"Pad 4 (choir)",
	"Pad 5 (bowed)",
	"Pad 6 (metallic)",
	"Pad 7 (halo)",
	"Pad 8 (sweep)",
	"FX 1 (rain)",
	"FX 2 (soundtrack)",
	"FX 3 (crystal)",
	"FX 4 (atmosphere)",
	"FX 5 (brightness)",
	"FX 6 (goblins)",
	"FX 7 (echoes)",
	"FX 8 (sci-fi)",
	"Sitar",
	"Banjo",
	"Shamisen",
	"Koto",
	"Kalimba",
	"Bag pipe",
	"Fiddle",
	"Shanai",
	"Tinkle Bell",
	"Agogo",
	"Steel Drums",
	"Woodblock",
	"Taiko Drum",
	"Melodic Tom",
	"Synth Drum",
	"Reverse Cymbal",
	"Guitar Fret Noise",
	"Breath Noise",
	"Seashore",
	"Bird Tweet",
	"Telephone Ring",
	"Helicopter",
	"Applause",
	"Gunshot",
	"Percussion"};
/* End Instruments */

/**
*	@brief	Set the active MIDI channel of aninstrument
*	@param	string		instrument_name		instrument id string
*	@param	uint16_t	active_midi_ch_mask	16 bits bitmask   msb ch16 ch15......ch2 ch1  lsb
*	@return 0
*/
int mod_synth_set_instrument_active_midi_channels(std::string instrument_name, uint16_t active_midi_ch_mask);

/**
*	@brief	Get the active MIDI channel of aninstrument
*	@param	string		instrument_name		instrument id string	
*	@return active_midi_ch_mask	16 bits bitmask   msb ch16 ch15......ch2 ch1  lsb
*/
uint16_t mod_synth_get_instrument_active_midi_channels(std::string instrument_name);

/**
*	@brief	Reads a MIDI file.
*	@param	string	full path to the midi file	
*	@return 0 if done.
*/
int mod_synth_open_midi_file(std::string path);

/* MIDI Playback */
void mod_synth_midi_player_play();
void mod_synth_midi_player_pause();
void mod_synth_midi_player_stop();
void mod_synth_midi_player_backward();
void mod_synth_midi_player_forward();

void mod_synth_midi_player_set_auto_loop_back(bool state);
bool mod_synth_midi_player_get_auto_loop_back();

void mod_synth_midi_player_set_playback_volume(int vol);
int mod_synth_midi_player_get_playback_volume();


/**
*	@brief	Set a MIDI mixer channel volume
*	@param	int		channel 0-15
*	@param	int		volume 0-100
*	@return void
*/
void mod_synth_midi_mixer_set_channel_volume(int chan, int vol);

/**
*	@brief	Set a MIDI mixer channel pan
*	@param	int		channel 0-15
*	@param	int		pan -50 (left) -  +50 (right)
*	@return void
*/
void mod_synth_midi_mixer_set_channel_pan(int chan, int pan);

/**
 *	@brief	Set a MIDI mixer channel pan modulation level
 *	@param	int		channel 0-15
 *	@param	int		0-100
 *	@return void
 */
void mod_synth_midi_mixer_set_channel_pan_mod_level(int chan, int lfo);

/**
 *	@brief	Set a MIDI mixer channel pan modulation LFO number
 *	@param	int		channel 0-15
 *	@param	int		lfo 0-none 1-6
 *	@return void
 */
void mod_synth_midi_mixer_set_channel_pan_mod_lfo(int chan, int lfo);

/**
 *	@brief	Set a MIDI mixer channel send level
 *	@param	int		channel 0-15
 *	@param	int		snd 0-100
 *	@return void
 */
void mod_synth_midi_mixer_set_channel_send_level(int chan, int snd);

/**
*	@brief	Set a MIDI mixer channel static volume
*			When true, the volume level is not affected by channel volume commands.
*	@param	int		channel 0-15
*	@param	bool	if true, block volume commands (static volume)
*	@return void
*/
void mod_synth_midi_mixer_set_channel_static_volume(int chan, bool state);

/* Midi Mapper */

/**
*	@brief	Register a function to notify that a channel volume command was trapped
*	@param	int		a pionter to a void foo(ini chan, int vol) function. (volume is scaled 0-127 -> 0-100)
*	@return void
*/
void mod_synth_register_midi_channel_volume_control_command_trapped_callback(func_ptr_void_int_int_t ptr);

/**
*	@brief	Register a function to notify that a channel change program command was trapped
*	@param	int		a pionter to a void foo(ini chan, int prog) function.
*	@return void
*/
void mod_synth_register_midi_channel_change_program_command_trapped_callback(func_ptr_void_int_int_t ptr);

/**
*   @brief  Register a callback to notify that a midi mixer channels active indications.
*   @param  func_ptr_void_int_bool_t ptr  a pointer to the callback function ( void func(int, bool) ) (chan, state)
*   @return void
*/
void mod_synth_register_midi_channel_active_indication_update_callback(func_ptr_void_int_bool_t ptr);
