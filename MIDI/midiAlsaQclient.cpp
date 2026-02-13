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

#include "midiAlsaQclient.h"
//#include "../alsa/alsaMidi.h"
#include "midiStream.h"
#include "../utils/safeQueues.h"

MidiAlsaQclientIn::MidiAlsaQclientIn(SafeQueue<alsa_data_t*>* rx_q, uint8_t stage)
	: MidiStream(0, NULL, stage)
{
	rx_queue = rx_q;
	update_setup(); //active = true;
}


void MidiAlsaQclientIn::update(void) // TODO: add connection number to select input_queue[connection] (temp connection = 0)
{
	midi_stream_mssg_block_t* block;
	raw_data_mssg_block_t* data;
	int count, last_count;

	alsa_data_t* rx_data = NULL;
	bool timeout, blocking = false;

	// Available channel data
	block = allocate_midi_stream_block();
	if (block) {
		data = allocate_raw_data_mssg_block();
		if (data)
		{
			block->header_type = _MIDISTREAM_HEADER_TYPE_RAW;
			block->header_reseved1 = 0;
			block->header_reseved2 = 0;
			// Get all data from Q
			count = 0;
			last_count = 0;		// used for single message multiple reads
			rx_data = rx_queue->dequeue(0, &timeout, &blocking);  //// Non blocking get from Q
			while (rx_data != NULL)
			{
				for (count; count < (last_count + rx_data->length) && count < sizeof(rx_data->data); count++)
				{
					data->bytes[count] = rx_data->data[count - last_count];
				}
				last_count = count;
				delete rx_data;
				rx_data = rx_queue->dequeue(0, &timeout, &blocking);
			}

			if (count > 0)
			{
				data->len = count;

				for (int i = 0; i < count; i++)
				{
					block->data_t.bytes[i] = data->bytes[i];
				}
				block->header_length = count;
				transmit(block, 0);
			}
			//			if (block->ref_count <= 1)
			//			{
							// Last use of MIDI stream block
			release_raw_data_mssg_block(data);
			//			}
			release_midi_stream_block(block);
		}
		else
		{
			// No block available
			fprintf(stderr, "midi ALAS Q client: no raw block: \n");
			if (block)
			{
				release_midi_stream_block(block);
			}
		}
	}
}
