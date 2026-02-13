/**
*	@file		prioritiesr.h
*	@author		Nahum Budin
*	@date		23_Jan-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*
*	@History	30-Oct-2019	1.0
*
*	@brief		Envelope generator.
*/

#pragma once

#define _THREAD_PRIORITY_MIDI_IN		9 // Highest
#define _THREAD_PRIORITY_MIDI_CTLB		9
#define _THREAD_PRIORITY_MIDI_PLAYBACK  9
#define _THREAD_PRIORITY_SERIAL_PORT	10
#define _THREAD_PRIORITY_I2C_INTERFACE	11

#define _THREAD_PRIORITY_CHANGE_CONTROL	12 
#define _THREAD_PRIORITY_UPDATE_TIMER	13		  
#define _THREAD_PRIORITY_UPDATE			14
#define _THREAD_PRIORITY_JACK			15
#define _THREAD_PRIORITY_ALSA			16

#define _THREAD_PRIORITY_MIDI_STREAM	21
#define _THREAD_PRIORITY_BT_CONNECTED	22
#define _THREAD_PRIORITY_BT_MAIN		23
