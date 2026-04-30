/**
* @file		defines.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		Provides the general constant DEFINES definitions and values
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#define _MAX_NUM_OF_INSTRUMENTS									256

#define _INSTRUMENT_TYPE_PLAYING								0
#define _INSTRUMENT_TYPE_EFFECT									1
#define _INSTRUMENT_TYPE_CONTROL								2

#define _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY					"Adj-Fluid-Synth"
#define _INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY					"Adj-Hammond-Organ"
#define _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY					"Adj-Analog-Synth"
#define _INSTRUMENT_NAME_KARPLUS_STRONG_STRING_SYNTH_STR_KEY	"Karplus-Strong-Strings-Synth"
#define _INSTRUMENT_NAME_MORPHED_SINUS_SYNTH_STR_KEY			"Morphed-Sinus-Synth"
#define _INSTRUMENT_NAME_PADSYNTH_SYNTH_STR_KEY					"Adj-PAD-Synth"


#define _INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY			"Adj-Midi-Player"
#define _INSTRUMENT_NAME_MIDI_MIXER_STR_KEY				"Midi-Mixer"
#define _INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY			"Adj-Midi-Mapper"

#define _INSTRUMENT_NAME_REVERB_STR_KEY					"Adj-Reverb"
#define _INSTRUMENT_NAME_DISTORTION_STR_KEY				"Adj-Distortion"
#define _INSTRUMENT_NAME_GRAPHIC_EQUALIZER_STR_KEY		"Graphic-Equilizer"

#define _INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY	"Control-Box-Handler"
#define _INSTRUMENT_NAME_EXT_MIDI_INT_CONTROL_STR_KEY	"External-MIDI-Interface-Control"
#define _INSTRUMENT_NAME_KEYBOARD_CONTROL_STR_KEY		"Keyboard-Control"

#define _INSTRUMENT_JACK_OUTPUT_NAME_FLUID_SYNTH_STR	_INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY
#define _INSTRUMENT_JACK_OUTPUT_NAME_HAMMOND_ORGAN_STR _INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY

#define _ALSA_NAME_CLIENT_BT_STR						"Bluetooth-MIDI"
#define _ALSA_NAME_CLIENT_CONTROL_BOX_STR				"Control-Box"
#define _ALSA_NAME_CLIENT_CONTROL_BOX_EXT_MIDI_STR		"Control-Box-Ext-MIDI"
#define _ALSA_NAME_CLIENT_MIDI_PLAYER_STR				_INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY
#define _ALSA_NAME_CLIENT_MIDI_MAPPER_STR				_INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY

#define _NUM_OF_CONTROL_BOX_SLIDERS						12
#define _NUM_OF_CONTROL_BOX_KNOBS						12
#define _NUM_OF_CONTROL_BOX_PUSHBUTTONS					12

#define _FLUID_CHORUS_MOD_SINE							0
#define _FLUID_CHORUS_MOD_TRIANGLE						1

#define _MIDI_PLAYER_STATE_STOPPED						0
#define _MIDI_PLAYER_STATE_PLAYING						1
#define _MIDI_PLAYER_STATE_PAUSED						2
#define _MIDI_PLAYER_STATE_INIT_STOP					3
#define _MIDI_PLAYER_STATE_INIT_PAUSE					4
#define _MIDI_PLAYER_STATE_PLAYNG_SYNC					6

#define _MAX_NUM_OF_MIDI_CLIENTS						128

#define _I2C_BUS_NUMBER										1

#define _I2C_SLAVE_MAIN_CONTROL_DEVICE_ADDRESS				99
#define _I2C_SLAVE_MAIN_CONTROL_DEVICE_DATA_REG_ADDRESS		0x72
#define _I2C_SLAVE_MAIN_CONTROL_DEVICE_DATA_LEN_REG_ADDRESS	0x73
#define _I2C_SLAVE_EXT_MIDI_INT_RX_DATA_REG_ADDRESS			0x74
#define _I2C_SLAVE_EXT_MIDI_INT_RX_DATA_LEN_REG_ADDRESS		0x75

#define _I2C_SLAVE_CONTROL_BOARD_ADDRESS					98
#define _I2C_SLAVE_CONTROL_BOARD_DATA_REG_ADDRESS			0x31
#define _I2C_SLAVE_CONTROL_BOARD_DATA_LEN_REG_ADDRESS		0x30

// Writing to this register will turn the control device control board Yellow LED on.
#define _REG_YELLOW_LED_ON									0x20
// Writing to this register will turn the control device control board Yellow LED off.
#define _REG_YELLOW_LED_OFF									0x21
// Writing to this register will turn the control device control board Green LED on.
#define _REG_GREEN_LED_ON									0x22
// Writing to this register will turn the control device control board Green LED off.
#define _REG_GREEN_LED_OFF									0x23

// Assuming the system using i2c interface number 1 to communicate with the audio board.
#define _I2C_INTERFACE_NUMBER_MAIN_CONTROL	2
#define _I2C_INTERFACE_NUMBER_CONTROL_BOARD 3
