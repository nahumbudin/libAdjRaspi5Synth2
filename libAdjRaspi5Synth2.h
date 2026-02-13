/**
* @file		libAdjRaspi5Synth_1.h
*	@author		Nahum Budin
*	@date		8-May-2024
*	@version	1.0
*
*
*	History:\n
*
*	
*
*	@brief		A raspberry Pi multi-core polyphonic music synthesizer library api.
*
*	\mainpage AdjHeart Polyphonic Synthesizer
*
*	This library was writen to run over a Raspberry Pi 5B.\n
*	The librarry implements a modular synthesizer \n
* 
* 
*	It provides a full API supporting all features incluuding callback functions for updates and events initiation.
*
*/

#pragma once


#include "modSynth.h"
#include "./LibAPI/types.h"

#include <cstdint>

// #ifdef __cplusplus
// extern "C" {
// #endif





void callback_message_id(int);


// #ifdef __cplusplus
// }
// #endif

