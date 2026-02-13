/**
*	@file		midiAlsaQclient.h
*	@author		Nahum Budin
*	@date		11-May-2024
*	@version	1.3
*					1. Minor code refactoring .
*
*	@brief		Recives data from the alsa midi channels rx queue and send it as a raw data
*				blocks to connected midi stream objects during the update process.
*
*	History:\n
*		version 1.0		9-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Dec 2017)
* 
*		version 1.1		12-Feb-2021
*							1. Code refactoring and notaion.
*							2. Adding rx queue as a paramter
*
*/

#pragma once

#include <cstdint>

#include "midiStream.h"
#include "../ALSA/alsaMidi.h"
#include "../utils/safeQueues.h"

class MidiAlsaQclientIn : public MidiStream
{
public:
	MidiAlsaQclientIn(SafeQueue<alsa_data_t*>* rx_q = nullptr, uint8_t stage = 0);

	virtual void update(void);

private:

	SafeQueue<alsa_data_t*>* rx_queue;

};


