/**
*	@file		midiStream.cpp
*	@author		Nahum Budin
*	@date		11-May-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*					2. Adding more AUX controls SYSEX messages
*
*	@brief		Handle midi streams and definitions
*
*  Based on Music Open Lab Library.
*	Copyright AdjHeart Nahum Budin May 2017 and
*	Teensyduino Core Library AudioStream Audio-blocks concept
*
*	History:\n
*
*	version 1.0		13-Oct-2019:
*		First version
*		1. Added mutex to control blocks allocation operations.
*	version 1.1		3-Feb-2021
*		1. Code refactoring and notaion.
*
*/

#include <unistd.h>
#include <cstring>

#include "midiStream.h"
#include "../misc/priorities.h"

/* A global memory pool of midi messages data blocks */
midi_stream_mssg_block_t* MidiStream::midi_stream_memory_pool;
uint32_t MidiStream::midi_stream_memory_pool_available_mask[_MIDI_STREAM_MESSAGES_POOL_NUM_OF_MASKS];

/* A global memory pool of midi raw data blocks */
raw_data_mssg_block_t* MidiStream::raw_data_mssgs_memory_pool;
uint32_t MidiStream::raw_data_mssgs_memory_pool_available_mask[_RAWDATA_MESSAGES_POOL_NUM_OF_MASKS];

bool MidiStream::update_enable = false;
/* Holds pointers to MidiStream objects that should start the update process (stage 0)*/
MidiStream* MidiStream::first_update[_MAX_STAGES_NUM];

bool MidiStream::thread_is_running = false;
bool MidiStream::update_in_progress = false;
pthread_t MidiStream::midi_thread;

std::mutex MidiStream::allocations_manage_mutex;

/**
*   @brief  Create a connection (input and output numbers are 0).
*   @param  source			 source midi block
*	@param	destination		 destination midi block
*   @return 0 if done
*/
MidiConnection::MidiConnection(MidiStream* source, MidiStream* destination)
{
	src = source;
	dst = destination;
	src_index = 0;
	dest_index = 0;
	next_dest = NULL;

	connect();
}

/**
*   @brief  Create a connection.
*   @param  source			 source midi block
*	@param	sourceOutput	 source midi block output number
*	@param	destination		 destination midi block
*	@param	destinationInput destination midi block input number
*   @return 0 if done
*/
MidiConnection::MidiConnection(MidiStream* source, unsigned char source_output,
	MidiStream* destination, unsigned char destination_input)
{
	src = source;
	dst = destination;
	src_index = source_output;
	dest_index = destination_input;
	next_dest = NULL;

	connect();
}

/**
*   @brief  Create a connection between the connection source and
*			connection destination midi sream objects.
*   @param  none
*   @return void
*/
void MidiConnection::connect(void)
{
	MidiConnection* p;

	if (dest_index > dst->num_inputs)
	{
		return;
	}

	MidiStream::update_stop();
	p = src->destination_list;
	if (p == NULL)
	{
		// 1st
		src->destination_list = this;
	}
	else if (src->num_outputs > 1)
	{
		while (p->next_dest) 
			p = p->next_dest;

		p->next_dest = this;
	}
	src->active = true;
	dst->active = true;
	MidiStream::update_setup();
}

/**
*   @brief  Creates and initializes a midi stream object instance.
*			This object reads MIDI data from inputs queues, process it
*			during the update cycle at a given stage, and may pass the 
*			results to another midi stream block for further processing.
*   @param  ninput		number of inputs
*	@param	iqueue		a pointer to a pointer of input queues (array)
*	@param	stage		update stage number
*   @return 0 if done
*/
MidiStream::MidiStream(uint8_t ninput, midi_stream_mssg_block_t** iqueue, uint8_t stage)
{
	num_inputs = ninput;
	stage_num = stage;
	input_queue = iqueue;

	if (stage_num > _MAX_STAGES_NUM)
	{
		stage_num = _MAX_STAGES_NUM;
	}

	active = false;
	destination_list = NULL;

	for (int i = 0; i < num_inputs; i++)
	{
		input_queue[i] = NULL;
	}
	// add to a simple list, for update_all
	// TODO: replace with a std::list?
	// TODO: replace with a proper data flow analysis in update_all
	if (first_update[stage_num] == NULL)
	{
		// 1st block
		first_update[stage_num] = this;
	}
	else
	{
		MidiStream* p;
		// go until the end of the list
		for (p = first_update[stage_num]; p->next_update[stage_num]; p = p->next_update[stage_num])
			;

		p->next_update[stage_num] = this;
	}

	next_update[stage_num] = NULL;

	num_outputs = _MAX_REF_COUNT;
}

/**
*   @brief  Set up the pool of midiStream data blocks
*			and place them all onto the free list
*   @param  data	a pointer to a midi_stream_mssg_block_t objects pool (array)
*	@param	num		number of blocks to be created
*   @return void
*/
void MidiStream::initialize_midi_stream_memory(midi_stream_mssg_block_t* data, unsigned int num)
{
	unsigned int i;

	//	if (num > _MAX_MIDI_STREAM_MESSAGES_POOL_SIZE) num = _MAX_MIDI_STREAM_MESSAGES_POOL_SIZE;
	update_stop();
	midi_stream_memory_pool = data;
	for (i = 0; i < _MIDI_STREAM_MESSAGES_POOL_NUM_OF_MASKS; i++)
	{
		midi_stream_memory_pool_available_mask[i] = 0;
	}
	for (i = 0; i < num; i++)
	{
		// >> 5 : masks are 32 bits each
		midi_stream_memory_pool_available_mask[i >> 5] |= (1 << (i & 0x1F));
	}
	for (i = 0; i < num; i++)
	{
		data[i].memory_pool_index = i;
	}
	update_setup();
}


/**
*   @brief  Allocate 1 midi data block.
*			If successful, the caller is the only owner of this new block.
*   @param  none
*   @return a pointer to the allocated midi_stream_mssg_block_t block
*/
midi_stream_mssg_block_t* MidiStream::allocate_midi_stream_block(void)
{
	uint32_t n, index, avail;
	uint32_t* p;
	midi_stream_mssg_block_t* block;
	//	uint8_t used;

	// mutex-get like
	update_stop();
	allocations_manage_mutex.lock();

	p = midi_stream_memory_pool_available_mask;
	index = 0;
	do
	{
		avail = *(p + index);
		if (avail) break;
		else index++;
	} while (index < _MIDI_STREAM_MESSAGES_POOL_NUM_OF_MASKS);

	if (index >= _MIDI_STREAM_MESSAGES_POOL_NUM_OF_MASKS)
	{
		// No free block
		// mutex - release like
		update_setup();
		allocations_manage_mutex.unlock();
		//Serial.println("alloc:null");
		return NULL;
	}
	// Get the number of leading zeros.
	n = __builtin_clz(avail);
	// Mark used by setting to zero
	*(p + index) = avail & ~(0x80000000 >> n);

	block = midi_stream_memory_pool + ((index << 5) + (/*31 -*/ n));
	block->ref_count = 1;
	//	if (used > memory_used_max) memory_used_max = used;

	// mutex-release like
	update_setup();
	allocations_manage_mutex.unlock();

	//	printf("Allocate midi block %i\n", ++test_count);

	return block;
}

/**
*   @brief  Release ownership of a midi stream data block.
*			If no other streams have ownership, the block is
*			returned to the free pool.
*   @param  block	a pointer to the to be released block (midi_stream_mssg_block_t)
*   @return void
*/
void MidiStream::release_midi_stream_block(midi_stream_mssg_block_t* block)
{
	// mutex-get like
	update_stop();
	allocations_manage_mutex.lock();

	// Mark free by setting the bit to 1
	uint32_t mask = (0x80000000 >> (/*31 -*/ (block->memory_pool_index & 0x1F)));
	uint32_t index = block->memory_pool_index >> 5;

	if (block->ref_count > 1)
	{
		// One less owner
		block->ref_count--;
	}
	else
	{
		// No more users - release
		midi_stream_memory_pool_available_mask[index] |= mask;
		//		memory_used--;
		//		printf("Release midi block %i\n", ++test_count);
	}

	// mutex - release like
	update_setup();
	allocations_manage_mutex.unlock();
}


/**
*   @brief  Set up the pool ofraw data blocks
*			and place them all onto the free list
*   @param  data	a pointer to a raw_data_mssg_block_t objects pool (array)
*	@param	num		number of blocks to be created
*   @return void
*/
void MidiStream::initialize_rawdata_mssgs_memory(raw_data_mssg_block_t* data, unsigned int num)
{
	unsigned int i;

	//	if (num > MAX_RAWDATA_MSSGS_POOL_SIZE) num = MAX_RAWDATA_MSSGS_POOL_SIZE;
	allocations_manage_mutex.lock();
	update_stop();
	raw_data_mssgs_memory_pool = data;
	for (i = 0; i < _RAWDATA_MESSAGES_POOL_NUM_OF_MASKS; i++)
	{
		raw_data_mssgs_memory_pool_available_mask[i] = 0;
	}

	for (i = 0; i < num; i++)
	{
		// >> 5 : masks are 32 bits each
		raw_data_mssgs_memory_pool_available_mask[i >> 5] |= (1 << (i & 0x1F));
	}

	for (i = 0; i < num; i++)
	{
		data[i].memory_pool_index = i;
	}

	update_setup();
	allocations_manage_mutex.unlock();
}

/**
*   @brief  Allocate 1 midi raw data block.
*			If successful, the caller is the only owner of this new block.
*   @param  none
*   @return a pointer to the allocated raw_data_mssg_block_t block
*/
raw_data_mssg_block_t* MidiStream::allocate_raw_data_mssg_block(void)
{

	uint32_t n, index, avail;
	uint32_t* p;
	raw_data_mssg_block_t* block;

	update_stop();
	allocations_manage_mutex.lock();
	p = raw_data_mssgs_memory_pool_available_mask;
	index = 0;
	do
	{
		avail = *(p + index);
		if (avail) break;
		else index++;
	} while (index < _RAWDATA_MESSAGES_POOL_NUM_OF_MASKS);

	if (index >= _RAWDATA_MESSAGES_POOL_NUM_OF_MASKS)
	{
		// No free block
		update_setup();
		allocations_manage_mutex.unlock();
		//Serial.println("alloc:null");
		return NULL;
	}

	n = __builtin_clz(avail);
	*(p + index) = avail & ~(0x80000000 >> n);
	block = raw_data_mssgs_memory_pool + ((index << 5) + (/*31 -*/ n));
	block->ref_count = 1;

	update_setup();
	allocations_manage_mutex.unlock();

	//	printf("Allocate raw block %i\n", ++test_count);

	return block;
}

/**
*   @brief  Release ownership of a midi stream raw data block.
*			If no other streams have ownership, the block is
*			returned to the free pool.
*   @param  block	a pointer to the to be released block (raw_data_mssg_block_t)
*   @return void
*/
void MidiStream::release_raw_data_mssg_block(raw_data_mssg_block_t* block)
{
	if (!block)
	{
		return;
	}

	update_stop();
	allocations_manage_mutex.lock();

	uint32_t mask = (0x80000000 >> (/*31 -*/ (block->memory_pool_index & 0x1F)));
	uint32_t index = block->memory_pool_index >> 5;

	if (block->ref_count > 1)
	{
		block->ref_count--;
	}
	else
	{
		raw_data_mssgs_memory_pool_available_mask[index] |= mask;
		//		printf("Release raw block %i\n", ++test_count);
	}

	update_setup();
	allocations_manage_mutex.unlock();
}

/**
*   @brief  Transmit a midi message data block to all streams that connect to an output.
*			The block becomes owned by all the recepients, but also is still
*			owned by this object.  Normally, a block must be released  by the caller after
*			it's transmitted.  This allows the caller to transmit the same block to more than 1 output,
*			but no more than 7 outputs and then release it once after all transmit calls.
*   @param  block a pointer to the transmitted block midi_stream_mssg_block_t block
*			(midi_stream_mssg_block_t)
*	@param	index	output num
*   @return void
*/
void MidiStream::transmit(midi_stream_mssg_block_t* block, unsigned char index)
{
	for (MidiConnection* c = destination_list; c != NULL; c = c->next_dest)
	{
		if (c->src_index == index)
		{
			if ((c->dst->input_queue[c->dest_index] == NULL) &&
				(block->ref_count < _MAX_REF_COUNT)) {
				c->dst->input_queue[c->dest_index] = block;
				block->ref_count++;
				// Serial.print("tx ref cnt: "); Serial.println(block->ref_count);
			}
		}
	}
}

/**
*   @brief  Receive a midi message data block from an input.  The block's data
*			may be shared with other streams, so it must not be written over
*	@param	index	input num
*   @return midi block
*/
midi_stream_mssg_block_t* MidiStream::receive_read_only(unsigned int index)
{
	midi_stream_mssg_block_t* in;

	if (index >= num_inputs)
	{
		return NULL;
	}
	// Get the input block
	in = input_queue[index];
	// Clear the input Q entry
	input_queue[index] = NULL;
	return in;
}

/**
*   @brief  Receive a midi message data block from an input.  The block will not
*			be shared, so its contents may be changed.
*	@param	index	input num
*   @return audio block
*/
midi_stream_mssg_block_t* MidiStream::receive_writable(unsigned int index)
{
	midi_stream_mssg_block_t* in, * p;

	if (index >= num_inputs)
	{
		return NULL;
	}

	in = input_queue[index];
	input_queue[index] = NULL;
	if (in && in->ref_count > 1)
	{
		p = allocate_midi_stream_block();
		if (p) memcpy(p, in, sizeof(midi_stream_mssg_block_t));
		release_midi_stream_block(in);
		in = p;
	}
	return in;
}


/**
*   @brief  Enables the update process.
*   @param  none
*   @return void
*/
void MidiStream::update_setup(void)
{

	MidiStream::update_enable = true;
}

/**
*   @brief  Disables the update process.
*   @param  none
*   @return void
*/
void MidiStream::update_stop(void)
{

	MidiStream::update_enable = false;
}

/**
*   @brief  Returns the block stage number.
*   @param  none
*   @return stage number
*/
uint8_t MidiStream::get_stage_num()
{
	return stage_num;
}

/**
*   @brief  Debug printing of update list.
*   @param  none
*   @return void
*/
void MidiStream::dbg_print_update_list(void)
{
	MidiStream* p;

	for (uint8_t stage = 0; stage < _MAX_STAGES_NUM; stage++)
	{
		fprintf(stderr, "pipe %i : ", stage);
		for (p = MidiStream::first_update[stage]; p; p = p->next_update[stage])
		{
			if (p == NULL)
			{
				fprintf(stderr, "NULL");
			}
			else
			{
				fprintf(stderr, "%x-->", (uintptr_t)p);
			}
		}
		fprintf(stderr, "\n");
	}
}

/**
*   @brief  Start the update process thread.
*   @param  none
*   @return void
*/
void MidiStream::start_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_STREAM;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// setting the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	//	err = errno;
		//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting MIDI Stream thread realtime prio\n");
	}

	thread_is_running = true;
	pthread_create(&midi_thread, &tattr, run_midi, (void*)11);
}

/**
*   @brief  Stop the update process thread.
*   @param  none
*   @return void
*/
void MidiStream::stop_thread()
{
	thread_is_running = false;
}

/**
* Initiate and update process of all the blocks
*/
void MidiStream::update_all(void) {

	MidiStream* p;
	uint8_t stage;

	update_in_progress = true;
	for (stage = 0; stage < _MAX_STAGES_NUM; stage++)
	{
		// Blocks are updated in sequential stages order - Update all blocks in this stage
		for (p = MidiStream::first_update[stage]; p; p = p->next_update[stage])
		{
			if (p->active) {
				p->update();
			}
		}
	}
	update_in_progress = false;

	//	fprintf(stderr, "MIDI Stream thread update timer\n");
}

/**
 * Check if update is in in progress
 *

 * @return true if update is in progress, false otherwise
 */
bool MidiStream::update_is_in_progress()
{
	return update_in_progress;
}

/**
*	Midi timer function that periodically (5msec) initiates a MIDI update process
*/
void* MidiStream::run_midi(void* threadid)
{
	fprintf(stderr, "MIDI Stream thread started\n");

	int* tid = (int*)threadid;
	tid++;

	pthread_t thId = pthread_self();

	pthread_setname_np(thId, "midithread");

	while (thread_is_running)
	{
		if (update_enable)
		{
			update_all();
		}

		usleep(1000); // (10000);
	}

	return 0;
}



