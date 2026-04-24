/**
* @file			libAdjRaspi5synthAPI.h
*	@author		Nahum Budin
*	@date		21-Apr-2026
*	@version	1.1
*					1. Added Hammond Organ API.
*
*
*	History:\n
*	
*			ver 1.0  8-May-2024 Initial
*
*	
*
*	@brief		A raspberry Pi multi-core polyphonic music synthesizer library api.
*
*	This library was writen to run over a Raspberry Pi 5B.\n
*	The librarry implements a modular synthesizer \n
* 
* 
*	It provides a full API supporting all features incluuding callback functions for updates and events initiation.
*
*/

#pragma once

#include "types.h"
#include "defaults.h"
#include "gui.h"
#include "audio.h"
#include "defines.h"
#include "midi.h"
#include "modSynthAPI.h"
#include "controlBox.h"
#include "connections.h"
#include "fluidSynth.h"
#include "settingsApi.h"
#include "patch.h"
#include "synthesizer.h"
#include "utils.h"
#include "hammondOrgan.h"

