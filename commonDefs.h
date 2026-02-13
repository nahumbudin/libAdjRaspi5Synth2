
/**
* @file		commonDefs.h
* @author		Nahum Budin
* @date		11-May-2024
* @version	1.0
*
*@brief		Holds common definitions.
*
*Based on Music Open Lab Library.
* Copyright AdjHeart Nahum Budin August 2018
*
*History
* version 1.0		11-May-2024:
*First version
*/


#pragma once

#include "../libAdjRaspi5Synth2/LibAPI/libAdjRaspi5SynthAPI.h"

#define _MAX_NUM_OF_MIDI_CONNECTIONS		8
#if (_MAX_NUM_OF_MIDI_CONNECTIONS > _MAX_NUM_OF_MIDI_DEVICES)
error max num of midi connections cannot be higher than max num off midi devices
#endif

#define CHECK_BIT(var, pos) (((var) >> (pos)) & 1)
	
#define _MIDI_CHAN_ASSIGNED_SYNTH_NONE					0
#define _MIDI_CHAN_ASSIGNED_SYNTH_FLUID					1
#define _MIDI_CHAN_ASSIGNED_SYNTH_ADJ					2
	
	
typedef struct voice_resource
{
	voice_resource()
		: id(-1)
		, used(false)
		, pending(false)
		, note(-1)
		, timestamp(0) {}
	int id;
	bool used;
	bool pending;
	int note;
	uint64_t timestamp;
} voice_resource_t;