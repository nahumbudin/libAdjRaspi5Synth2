/**
*	@file		audioBlock.cpp
*	@author		Nahum Budin
*	@date		30-Sep-2024
*	@version	1.1 
*					1. Code refactoring and notaion.
*					
*	@version	1.1	29-Jan-2021	Code refactoring and notaion
*				1.0	4-Nov-2019	(13/6/2018 No control blocks)
*
*	@brief		Audio blocks and connections .
*	
****************** Audio Block *********************
*
*	The Audio-block is the basic object that generates or process a single audio-samples block,
*	each containing up to _AUDIO_MAX_BUF_SIZE (2048) samples. The default block size is
*	_DEFAULT_BLOCK_SIZE (512).
*	
*	The Audio-block may either generate a new audio-samples block, or process input audio-samples blocks
*	and produce a new processed audio-samples block. 
*	
*	This processing is performed by each Audio-block
*	specific void update(void) function (see more details hereafter).
*	
*	The audio processing comprises a singele or multiple chains of Audio-blockes.
*	
*	A chain of Audio-blocks comprises Audio-blocks that are linked by Audio-connections (see hereafter),
*	where the output of each Audio-block may be fed into the next single or a multiple Audio-blocks inputs.
*	
*	Each such chain may be allocated to one of the polyphonic voices objects adjSynthVoice (see adjSynthVoive.h),
*	or, for example, to the common audio processing path (e.g. reverb, equilizer, and output-mixer - see
*	adjSynth.h).
*	
*	The order of Audio-blocks chain updates is set by using a linked-list, where each element points at the next 
*	Audio-block of the chian. 
*	
*	Each linked list is owned by the Audio-block parent adjSynthVoice object (see adjSynthVoive.h), in the case of
*	a polyphonic voice, or by the common audio effect module, as described above.
*	
*	This parent object provides the Audio-block with a pointer to a pointer of the 1st element of the list 
*	AudioBlockFloat **audio_first_update_ptr.
*	
*	The chain linked list is created in the following way: When a new Audio-block object is created, 
*		- if the audio_first_update_ptr is NULL (empty), the created Audio-block becomes the 1st object
*		  in the list (chain) by settin the audio_first_update_ptr to a pointer to the object.
*		- if the audio_first_update_ptr is not a NULL (not empty), the Audio-block becomes the last object
*		  in the list by setting the currently last Audio-block AudioBlockFloat *audio_next_update pointer
*		  to a pointer to the object pointer.
*	
*	When an Audio-blocks chain executes the update cycle, it starts by activating the update function of
*	the 1st item in the list, pointed by the audio_first_update_ptr, then the next item pointed by the
*	audio_next_update pointer of the 1st element, then the element pointed by the audio_next_update pointer
*	of the 2nd element, and so on.
*	
*	In each audio processing cycle, all the Audio-block chains are executed and generates the next audio-samplea
*	output block.	
*
*	Each audio processing cycle may be divided into stages enumarated _AUDIO_STAGE_0 (0) to _AUDIO_STAGE_7 (7),
*	where each Audio-block chain is assigned with a specific stage-number.
*	
*	On each audio processing cycle, the Audio-blocks processing update function is performed
*	in the order of stages, starting by all the Audio-block chains that are assigned to Stage_0, following by
*	Stage_1, and so on, untill al the Audio-blocks is peformung their update processing, stage by stage.
*	
*	Stages assignment is implementd by assigning each stage chain a dedicated audio_first_update_ptr pointer
*	of the AudioBlockFloat *audio_first_update[_MAX_STAGE_NUM] pointers array (see adjSynthVoice.h).
*	
*	Note: the update cycle starts by executing the polyphonic voices chains by calling the 
*	callback_audio_voice_update(int voice_num) of all the active voices, followed by activating the common
*	audio task by calling the callback_audio_update_cycle_end_tasks(int param)  (see adjSynth.cpp).*
*	
*
*
*	***************** Audio Connection *********************
*	
*	The Audio-connection (AudioConnectionFloat) connects between Audio-blocsks outputs to Audio-blocks inputs.
*	
*	A connection is created when calling the connect() function.
*	
*	The connect function creates a linked list of destination Audio-blocks that the source Audio-object 
*	output should be transfered to their inputs using the Audio-block 
*	transmit_audio_block(audio_block_float_mono_t *block, uint16_t index) function.
*/

#include <mutex>
#include <cstring>

#include "audioBlock.h"
#include "../utils/utils.h"
#include "../LibAPI/audio.h"

extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

// Set default values 
//unsigned int AudioBlockFloat::period_time;
volatile bool AudioBlockFloat::update_enable = false;
volatile bool AudioBlockFloat::update_in_progress = false;

/* A global memory pool of audio blocks */
audio_block_float_mono_t *AudioBlockFloat::audio_data_blocks_memory_pool;

uint32_t AudioBlockFloat::audio_data_blocks_memory_pool_available_mask[_AUDIO_BLOCKS_MESSAGES_POOL_NUM_OF_MASKS];
uint16_t AudioBlockFloat::audio_data_blocks_memory_pool_first_mask;

uint16_t AudioBlockFloat::audio_block_size = _AUDIO_MAX_BUF_SIZE;

/**
*   @brief  Creates and initializes an audio-block object instance.
*   @param  num_of_aud_inputs		number of inputs
*	@param	audio_queues			a pointer to a pointer of input queues (array)
*	@param	audio_first_update_ptr	a pointer to an audio block object instance
*									that is the first in the update chain
*	@param	stage					update stage number
*   @return 0 if done
*/
AudioBlockFloat::AudioBlockFloat(
	uint16_t num_of_aud_inputs, 
	audio_block_float_mono_t **audio_queues,
	AudioBlockFloat **audio_first_update_ptr,
	uint16_t stage)
{
	//	audioDriverType = _AUDIO_ALSA;
	
	//	period_time = _ALSA_PERIOD_TIME_USEC;
	num_of_audio_inputs = num_of_aud_inputs;
	stage_num = stage;
	audio_input_queue = audio_queues;
	audio_first_update = audio_first_update_ptr;
		
	active = false;

	audio_destination_list = NULL;
	
	for (int i = 0; i < num_of_audio_inputs; i++) 
	{
		audio_input_queue[i] = NULL;
	}
	
	// add to the end of a simple list, for update_all
	if (*audio_first_update == NULL)
	{
		// 1st in the list
		*audio_first_update = this;
	} 
	else 
	{
		// look for list end
		AudioBlockFloat *p;
		// loop until end
		for (p = *audio_first_update; p->audio_next_update; p = p->audio_next_update) ;
		// Add
		p->audio_next_update = this;
	}
	
	audio_next_update = NULL;
}

AudioConnectionFloat *AudioBlockFloat::get_audio_destination_list() 
{ 
	return audio_destination_list; 
}

void AudioBlockFloat::clear_audio_destinations_list() 
{
	audio_destination_list = NULL; 
}

/**
*   @brief  Enable updates
*   @param  none
*   @return void
*/
void AudioBlockFloat::enable_updates() 
{ 
	update_enable = true; 
}

/**
*   @brief  Disable updates
*   @param  none
*   @return void
*/
void AudioBlockFloat::disable_updates() 
{ 
	update_enable = false; 
}

/**
*   @brief  Return stage number
*   @param  none
*   @return stage number
*/
int AudioBlockFloat::get_stage() { return stage_num; }

/**
*   @brief  Set up the pool of audio data blocks
*			and place them all onto the free list
*   @param  data	a pointer to a audio_block_float_mono_t objects pool (array)
*	@param	num		number of blocks to be created
*	@param	size	block size
*   @return void
*/
void AudioBlockFloat::initialize_audio_memory(audio_block_float_mono_t *blocks, uint16_t num, uint16_t size)
{
	unsigned int i;

	if (num > _MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE)
	{
		num = _MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE;
		fprintf(stderr, "Init audio memory error: trying to init %i blocks, max %i were allocated\n", num, _MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE);
	}
	
	audio_block_size = size;
	if (!is_valid_audio_block_size(audio_block_size))
	{
		audio_block_size = _DEFAULT_BLOCK_SIZE;
	}

	update_stop();
	
	for (i = 0; i < num; i++) 	
	{
		if (blocks[i].data != NULL) 
		{
			delete[] blocks[i].data;
		}
		
		blocks[i].data = new float[size];
	}

	audio_data_blocks_memory_pool = blocks;
	audio_data_blocks_memory_pool_first_mask = 0;
	
	// Clear all masks indicating block availability 
	for (i = 0; i < _AUDIO_BLOCKS_MESSAGES_POOL_NUM_OF_MASKS; i++) 
	{
		audio_data_blocks_memory_pool_available_mask[i] = 0;
	}

	// Set all masks to "1" ("free")
	for (i = 0; i < num; i++) 
	{
		audio_data_blocks_memory_pool_available_mask[i >> 5] |= (1 << (i & 0x1F));
	}

	// Add index num to each block
	for (i = 0; i < num; i++) 
	{
		blocks[i].memory_pool_index = i;
	}
	
	update_start();
}

/**
*   @brief  Allocate 1 audio data block.
*			If successful, the caller is the only owner of this new block.
*   @param  none
*   @return a pointer to the allocated audio_block_float_mono_t block
*/
audio_block_float_mono_t * AudioBlockFloat::allocate_audio_block(void)
{
	uint32_t n, index, avail;
	uint32_t *p, *end;
	audio_block_float_mono_t *block;

	p = audio_data_blocks_memory_pool_available_mask;
	end = p + _AUDIO_BLOCKS_MESSAGES_POOL_NUM_OF_MASKS;
	
	update_stop();
	
	index = audio_data_blocks_memory_pool_first_mask;  // = 0 ?
	p += index;
	while (1) 
	{
		if (p >= end) 
		{
			// All masks were checked - no available block
			update_start();
			return NULL;
		}
		avail = *p;
		if (avail)
		{
			// Current mask is non all zero - there are available free blocks
			break;
		}
		// Goto next mask
		index++;
		p++;
	}
	
	// Mark as used
	// n will be the number of leading zero bits
	n = __builtin_clz(avail);
	// Clear the 1st non zero bit - mark as used
	avail &= ~(0x80000000 >> n);
	*p = avail;
	if (!avail) 
	{
		// No more available blocks in this mask - 
		// next time start searching from the next following mask
		index++;
		audio_data_blocks_memory_pool_first_mask = index;
	}
	// Index holds the mask number
	index = p - audio_data_blocks_memory_pool_available_mask;
	// Avilable block = (mask number X 32) + bit number
	block = audio_data_blocks_memory_pool + ((index << 5) + (31 - n));
	// Will be counted down by each receiving audio block to indicate id still in use
	block->ref_count = 1;

	//	printf("allocate block %i\n", block->memory_pool_index);
	
	update_start();
	
	return block;
}


/**
*   @brief  Release ownership of a data block.
*			If no other streams have ownership, the block is
*			returned to the free pool
*   @param  block	a pointer to the to be released block (audio_block_float_mono_t)
*   @return void
*/
void AudioBlockFloat::release_audio_block(audio_block_float_mono_t *block)
{
	//if (block == NULL) return;
	uint32_t mask = (0x80000000 >> (31 - (block->memory_pool_index & 0x1F)));
	uint32_t index = block->memory_pool_index >> 5;

	update_stop();
	
	// Release only if nor in use (multiple receiver)
	if (block->ref_count > 1) 
	{
		block->ref_count--;
	}
	else 
	{
		audio_data_blocks_memory_pool_available_mask[index] |= mask;
		if (index < audio_data_blocks_memory_pool_first_mask)
		{
			audio_data_blocks_memory_pool_first_mask = index;
		}
	}

	update_start();
}

/**
*   @brief  Transmit an audio data block
*			to all streams that connect to an output.  The block
*			becomes owned by all the recepients, but also is still
*			owned by this object.  Normally, a block must be released
*			by the caller after it is transmitted.  This allows the
*			caller to transmit the same block to more than 1 outputs,
*			and then release it once after all transmit calls.
*   @param  block a pointer to the transmitted block audio_block_float_mono_t block 
*			(audio_block_float_mono_t)
*	@param	index	output num
*   @return void
*/
void AudioBlockFloat::transmit_audio_block(audio_block_float_mono_t *block, uint16_t index)
{
	for (volatile AudioConnectionFloat *c = audio_destination_list; c != NULL; c = c->next_dest) 
	{
		if (c->src_index == index) 
		{
			if (c->dst->audio_input_queue[c->dest_index] == NULL) 
			{
				c->dst->audio_input_queue[c->dest_index] = block;
				block->ref_count++;
			}
		}
	}
}


/**
*   @brief  Receive an audio block from an input.  The block's data
*			may be shared with other streams, so it must not be written
*	@param	index	input num
*   @return audio block
*/
audio_block_float_mono_t * AudioBlockFloat::receive_audio_block_read_only(uint16_t index)
{
	audio_block_float_mono_t *in;

	if (index >= num_of_audio_inputs)
	{
		return NULL;
	}

	in = audio_input_queue[index];
	audio_input_queue[index] = NULL;
	return in;
}

/**
*   @brief  Receive an audio block from an input.  The block's data
*			will not be shared with other streams, 
*			so its contents may be changed.
*	@param	index	input num
*   @return audio block
*/
audio_block_float_mono_t * AudioBlockFloat::receive_audio_block_writable(uint16_t index)
{
	audio_block_float_mono_t *in, *p;

	if (index >= num_of_audio_inputs)
	{
		return NULL;
	}

	in = audio_input_queue[index];
	audio_input_queue[index] = NULL;
	if (in && in->ref_count > 1) 
	{
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		p = allocate_audio_block();
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		if (p)
		{
			memcpy(p->data, in->data, sizeof(*p->data)); // <<<<<<<<<<<<<<<<
		}
		in->ref_count--;
		in = p;
	}

	return in;
}

/**
*   @brief  Free all used blocks
*	@param	none
*   @return void
*/
void AudioBlockFloat::free_used_audio_blocks()
{
	int i;
	// Clear all masks indicating block availability 
	for (i = 0; i < _AUDIO_BLOCKS_MESSAGES_POOL_NUM_OF_MASKS; i++) 
	{
		audio_data_blocks_memory_pool_available_mask[i] = 0;
	}
	// Set all masks to "1"
	for (i = 0; i < _MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE; i++) 
	{
		audio_data_blocks_memory_pool_available_mask[i >> 5] |= (1 << (i & 0x1F));
	}
}

/**
*   @brief  Eenable updates 
*	@param	none
*   @return void
*/
void AudioBlockFloat::update_start(void) {
	
	AudioBlockFloat::update_enable = true;
}

/**
*   @brief  Disable updates
*	@param	none
*   @return void
*/
void AudioBlockFloat::update_stop(void) {
	
	AudioBlockFloat::update_enable = false;
}

/**
*   @brief  Set ALSA period time (uSec)
*	@param	ptu	ALSA period time (uSec)
*   @return void
*/
//int AudioBlockFloat::setPeriodTimeUs(unsigned long ptu) {
	
//	period_time = ptu;
	
//	return 0;
//}

/**
*   @brief  Generate a random number in given range
*	@param	min		random number minimum vale
*	@param	max		random number maximum vale
*   @return void
*/
int16_t AudioBlockFloat::random(int16_t min, int16_t max)
{
	return min + (max - min)*(rand() % 0x7fff);
}

/**
*   @brief  Return the update in progress state
*	@param	min		non
*   @return true if update process is in progress, false otherwise
*/
bool AudioBlockFloat::update_is_in_progress() 
{ 
	return update_in_progress;
}

/* A global memory pool of audio connections */
AudioConnectionFloat *AudioConnectionsManagerFloat::connections[_MAX_NUM_OF_AUDIO_CONNECTIONS];

AudioConnectionFloat::AudioConnectionFloat()
{
	connected = false;
	disconnect = false;
}

/**
*   @brief  Create a connection.
*   @param  source			 source audio block
*	@param	sourceOutput	 source audio block output number
*	@param	destination		 destination audio block
*	@param	destinationInput destination audio block input number
*   @return 0 if done
*/
void AudioConnectionFloat::connect(
	AudioBlockFloat *source,
	uint16_t sourceOutput,
	AudioBlockFloat *destination,
	uint16_t destinationInput) {
	src = source;
	dst = destination;
	src_index = sourceOutput;
	dest_index = destinationInput;
	next_dest = NULL;

	AudioConnectionFloat *p;

	if (dest_index > dst->num_of_audio_inputs)
	{
		return;
	}

	// Create a linked list of destinations
	p = src->audio_destination_list;
	if (p == NULL)
	{
		src->audio_destination_list = this;
	}
	else
	{
		while (p->next_dest)
		{
			// find list end
			p = p->next_dest;
		}
		p->next_dest = this;
	}

	src->active = true;
	dst->active = true;
	connected = true;
}

/**
*   @brief  Create a connection (input and output numbers are 0).
*   @param  source			 source audio block
*	@param	destination		 destination audio block
*   @return 0 if done
*/
void AudioConnectionFloat::connect(AudioBlockFloat *source, AudioBlockFloat *destination) {

	connect(source, 0, destination, 0);
}

/**
*   @brief  Return connection id.
*   @param  none
*   @return connection id number
*/
uint32_t AudioConnectionFloat::get_connection_id()
{
	return id;
}

/**
*   @brief  Return the connection state.
*   @param  none
*   @return true if connected, false otherwise
*/
bool AudioConnectionFloat::is_connected()
{
	return connected;
}

/**
*   @brief  Set disconnect flag true.
*   @param  none
*   @return void
*/
void AudioConnectionFloat::disconnect_this()
{
	disconnect = true;
}

/**
*   @brief  Set disconnect flag false.
*   @param  none
*   @return void
*/
void AudioConnectionFloat::was_disconnected()
{
	disconnect = false;
}

/**
*   @brief  Return the disconnect flag status.
*   @param  none
*   @return true - this connection should be disconnected, otherwise not.
*/
bool AudioConnectionFloat::to_disconnect() { return disconnect; }


AudioConnectionsManagerFloat::AudioConnectionsManagerFloat()
{
	int i;

	for (i = 0; i < _MAX_NUM_OF_AUDIO_CONNECTIONS; i++)
	{
		connections[i] = NULL;
	}
}

/**
*   @brief  Return a free connection out of the pool.
*   @param  none
*   @return true - a connection (AudioConnectionFloat).
*/
AudioConnectionFloat *AudioConnectionsManagerFloat::get_audio_connection()
{
	int i;
	AudioConnectionFloat *connection = NULL;

	for (i = 0; i < _MAX_NUM_OF_AUDIO_CONNECTIONS; i++)
	{
		if (connections[i] == NULL)
		{
			// Creates connections on demmand
			connections[i] = new AudioConnectionFloat();
			connection = connections[i];
			connection->id = i;
			break;
		}
	}

	return connection;
}

/**
*   @brief  Delete connection.
*   @param  id	connection id.
*   @return 0 if done, -1 if id is out of range
*			-2 if connection does not exist,
*			-3 if connection source audio object destinations list is empty
*/
int AudioConnectionsManagerFloat::delete_audio_connection(uint16_t id)
{
	AudioConnectionFloat *srcConnectiosList, *p, *prev;

	if ((id < 0) || (id > _MAX_NUM_OF_AUDIO_CONNECTIONS))
	{
		return -1;
	}

	if (connections[id] == NULL)
	{
		return -2;
	}

	// Get given connection source audio object destinations list
	srcConnectiosList = connections[id]->src->audio_destination_list;
	if (srcConnectiosList == NULL)
	{
		return -3;
	}
	// Look for the given connection id
	p = srcConnectiosList;
	prev = NULL;
	while ((p->next_dest) && (p->get_connection_id() != id))
	{
		prev = p;
		p = p->next_dest;
	}

	if ((p != NULL) && (p->get_connection_id() == id))
	{
		// connection found;
		if (p->next_dest == NULL)
		{
			// Last - delete the link to it
			if (prev != NULL)
			{
				// last and not the first dest in list was removed
				prev->next_dest = NULL;
			}
			else
			{
				// last and the 1st dest in list was removed (empty list)
				srcConnectiosList->src->audio_destination_list = NULL;
			}
		}
		else
		{
			// Not the last - remove
			if (prev != NULL)
			{
				// not the first dest in list was removed
				prev->next_dest = p->next_dest;
			}
			else
			{
				// the 1st dest in list was removed
				*srcConnectiosList = *srcConnectiosList->next_dest;
			}
		}
		// Free
		connections[id] = NULL;
	}

	return 0;
}

