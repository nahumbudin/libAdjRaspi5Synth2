/**
*	@file		audioBlock.h
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
*	***************** Audio Block *********************
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
*	Each linked list is owned the Audio-block parent adjSynthVoice object (see adjSynthVoive.h), in the case of
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

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "audioCommons.h"

// Create a pool of audio blocks to avoid memory allocation in runtime
#define AllocateAudioMemoryBlocksFloatPool(num, block_size) ({ \
	static audio_block_float_mono_t blocks[num]; \
	AudioBlockFloat::initialize_audio_memory(blocks, num, block_size); \
})
	

// Must be a multiple of 32
#define _MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE 1024
#if ((_MAX_AUDIO_SBLOCKS_MESSAGES_POOL_SIZE & 0x1f) != 0)
Error : must be n * 32
#endif
	// Must be no more than 2048
#if (_MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE > 2048)
	Error : must be less than 2048
#endif
	// Used for allocation 32 bits masks
#define _AUDIO_BLOCKS_MESSAGES_POOL_NUM_OF_MASKS (_MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE >> 5) 

#define _MAX_NUM_OF_AUDIO_CONNECTIONS			2048

#define _CONNECTION_OK							0
#define _CONNECTION_NOT_CONNECTED				-1
#define _NULL_CONNECTION						-2
#define _CONNECTION_LIST_EMPTY					-3
	
	// Must be no more than 8
#define _MAX_STAGE_NUM 8
#if (_MAX_STAGE_NUM > 8)
	Error : Must be no more than 8
#endif

#define _AUDIO_STAGE_0									0 
#define _AUDIO_STAGE_1									1 
#define _AUDIO_STAGE_2									2 
#define _AUDIO_STAGE_3									3 
#define _AUDIO_STAGE_4									4 
#define _AUDIO_STAGE_5									5 
#define _AUDIO_STAGE_6									6 
#define _AUDIO_STAGE_7									7 

class AudioConnectionFloat;

class AudioBlockFloat 
{
public:
	
	AudioBlockFloat(
		uint16_t num_of_aud_inputs,
		audio_block_float_mono_t **audio_queues,
		AudioBlockFloat **audio_first_update_ptr,
		uint16_t stage = 0); 
	
	AudioConnectionFloat *get_audio_destination_list(); 
	/*{ 
		return audio_destination_list; 
	}*/
	
	int get_stage();
	
	static void enable_updates();
	static void disable_updates();
	
	static bool update_is_in_progress();
	
	static void initialize_audio_memory(audio_block_float_mono_t *blocks, uint16_t num, uint16_t block_size);	
	
	static int16_t random(int16_t min, int16_t max);	

	/* Performs block processing */
	virtual void update(void) = 0;

	// for update_all
	AudioBlockFloat *audio_next_update;
	// for update_all - holds the 1st object in the propagate chain
	/*static*/ AudioBlockFloat **audio_first_update;
		
protected:
	static audio_block_float_mono_t * allocate_audio_block(void);	
	static void release_audio_block(audio_block_float_mono_t * block);	
	void transmit_audio_block(audio_block_float_mono_t *block, uint16_t index = 0);	
	audio_block_float_mono_t * receive_audio_block_read_only(uint16_t index = 0);	
	audio_block_float_mono_t * receive_audio_block_writable(uint16_t index = 0);	
	
	static void free_used_audio_blocks();
	
	void clear_audio_destinations_list(); // {audio_destination_list = NULL; } 
	
	static void update_start(void);
	static void update_stop(void);
	
	//	// Used audio driver: alsa or jack audio
	//	int audio_driver_type;
		// Number of audio inputs 
	uint16_t num_of_audio_inputs;
	// Number of audio outputs */
	uint16_t num_of_audio_outputs;
	// object is associated with this stage num
	volatile uint16_t stage_num;
	// Object is run on this core
	//volatile uint8_t core_num;
	
	bool active;
		
private:
	
	// holds a linked list of objects to be propagate audio data through
	AudioConnectionFloat *audio_destination_list; 
	// Input queues
	audio_block_float_mono **audio_input_queue;
	// When true update process is enables
	volatile static bool update_enable;
	volatile static bool update_in_progress;
	// A global memory pool of audio blocks
	static audio_block_float_mono_t *audio_data_blocks_memory_pool;
	static uint32_t audio_data_blocks_memory_pool_available_mask[];
	static uint16_t audio_data_blocks_memory_pool_first_mask;
	static uint16_t audio_block_size;
	
	friend class AudioConnectionFloat;
	friend class AudioConnectionsManagerFloat;
};


class AudioConnectionFloat
{
public:
	AudioConnectionFloat();

	void connect(AudioBlockFloat *source, AudioBlockFloat *destination);
	void connect(AudioBlockFloat *source,
		uint16_t source_output,
		AudioBlockFloat *destination,
		uint16_t destination_input);
	uint32_t get_connection_id();

	bool is_connected();
	void disconnect_this();
	bool to_disconnect();
	void was_disconnected();

protected:
	AudioBlockFloat *src;
	AudioBlockFloat *dst;
	uint16_t src_index;
	uint16_t dest_index;
	AudioConnectionFloat *next_dest;

	friend class AudioBlockFloat;
	friend class AudioConnectionsManagerFloat;

private:
	uint32_t id;
	bool connected;
	bool disconnect;

}
;

class AudioConnectionsManagerFloat
{
public:
	AudioConnectionsManagerFloat();

	AudioConnectionFloat *get_audio_connection();
	int delete_audio_connection(uint16_t id);

private:
	static AudioConnectionFloat *connections[_MAX_NUM_OF_AUDIO_CONNECTIONS];
};

