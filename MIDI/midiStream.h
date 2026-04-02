/**
 *	@file		midiStream.h
 *	@author		Nahum Budin
 *	@date		13-Oct-2025
 *	@version	1.3
 *					1. Add all notes/sounds off commands.
 *					2. Added mutexes to update_all and thread creation.
 *
 *	@brief		Handle midi streams and definitions
 *
 *  Based on Music Open Lab Library.
 *	Copyright AdjHeart Nahum Budin May 2017 and
 *	Teensyduino Core Library AudioStream Audio-blocks concept
 *
 *	History:\n
 *
 *
 *		1. Added mutex to control blocks allocation operations.
 *	version 1.1		3-Feb-2021	 Code refactoring and notaion.
 *	version 1.0		13-Oct-2019: First version
 *	version	1.2
 *					1. Code refactoring and notaion.
 *					2. Adding more AUX controls SYSEX messages
 *					3. Added mutexes to update_all and thread creation.
*
*/

#pragma once

#include <cstdlib>
#include <stdint.h>
#include <cstdint>
#include "mutex"

#include "../commonDefs.h"

// Must be a multiple of 32
#define _MAX_MIDI_STREAM_MESSAGES_POOL_SIZE		256
#if ((_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE & 0x1f) != 0)
Error : must be n * 32
#endif
// Must be no more than 512
#if (_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE > 512)
Error : must be less than 512
#endif
// Used for allocation 32 bits masks
#define _MIDI_STREAM_MESSAGES_POOL_NUM_OF_MASKS (_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE >> 5) 

// Max MIDI STREAM message length - must be a power of 2
#define _MIDI_STREAM_MESSAGE_LEN				512		
#if ((_MIDI_STREAM_MESSAGE_LEN-1) & _MIDI_STREAM_MESSAGE_LEN)
Error : must be a power of 2
#endif
// Must be no more than 512
#if (_MIDI_STREAM_MESSAGE_LEN > 512)
Error : must be no more than 512
#endif

// 3 Bytes are used for header
#define _MIDI_MSSG_MAX_LEN (_MIDI_STREAM_MESSAGE_LEN - 3)
#define _RAWDATA_MSSG_MAX_LEN _MIDI_MSSG_MAX_LEN

// Raw data MIDI messages pool - Must be a multiple of 32
#define _MAX_RAWDATA_MSSGS_POOL_SIZE			256		
//#if ((_MAX_RAWDATA_MSSGS_POOL_SIZE-1) & _MAX_RAWDATA_MSSGS_POOL_SIZE)
#if ((_MAX_RAWDATA_MSSGS_POOL_SIZE & 0x1f) != 0)
Error : must be n * 32
#endif
// Must be no more than 512
#if (_MAX_RAWDATA_MSSGS_POOL_SIZE > 512)
Error : must be no more than 512
#endif
// Used for allocation 32 bits masks
#define _RAWDATA_MESSAGES_POOL_NUM_OF_MASKS (_MAX_RAWDATA_MSSGS_POOL_SIZE >> 5) 

// Max number of objects that can own a message (allocate data struct)
// Must be no more than 7
#define _MAX_REF_COUNT 7
#if (_MAX_REF_COUNT > 7)
Error : Must be no more than 7
#endif

// Max number of midi processing stages - Must be no more than 8
#define _MAX_STAGES_NUM 8
#if (_MAX_STAGES_NUM > 8)
Error : Must be no more than 8
#endif

// Number of simultaneous handeled streams
#define _NUM_OF_MIDI_STREAMS					_MAX_NUM_OF_MIDI_CONNECTIONS

#define _MIDI_NOTE_OFF							0x80
#define _MIDI_NOTE_ON							0x90
#define _MIDI_AFTERTOUCH						0xA0
#define _MIDI_CHAN_CONROL						0xB0
#define _MIDI_PROGRAM_CHANGE					0xC0
#define _MIDI_CHAN_PRESURE						0xD0
#define _MIDI_PITCH_BEND						0xE0
#define _MIDI_SYSEX_START						0xF0
#define _MIDI_SYSEX_END							0xF7
#define _MIDI_RESET								0xFF

// TODO: temp
#define _MIDI_TILTUNE_SYSEX_VENDOR_ID_0			0
#define _MIDI_TILTUNE_SYSEX_VENDOR_ID_1			0x35	// Not official! 
#define _MIDI_TILTUNE_SYSEX_VENDOR_ID_2			0x56

#define _MIDISTREAM_HEADER_TYPE_RAW				0		// Raw data message
#define _MIDISTREAM_HEADER_TYPE_MIDI			1		// General MIDI single message
#define _MIDISTREAM_HEADER_TYPE_MIDI_LONG		2		// MIDI Sysex message
#define _MIDISTREAM_HEADER_TYPE_MIDI_MULTI		3		// Multipple MIDI messages
#define _MIDISTREAM_HEADER_TYPE_RESERVED_1		4
#define _MIDISTREAM_HEADER_TYPE_RESERVED_2		5
#define _MIDISTREAM_HEADER_TYPE_RESERVED_3		6
#define _MIDISTREAM_HEADER_TYPE_RESERVED_4		7

#define _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER	0xF5

#define _MIDI_PARSER_STATE_IDLE					0
#define _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS	1
#define _MIDI_PARSER_STATE_WAIT_FOR_COMAND_BYTE	2
#define _MIDI_PARSER_STATE_COMMAND_IN_PROGRESS	3

#define _MIDI_PARSER_STATE_ERROR				-1

#define _MIDI_STAGE_0							0		// Stages defines processing order
#define _MIDI_STAGE_1							1		//  within the MIDI update process pipeline
#define _MIDI_STAGE_2							2
#define _MIDI_STAGE_3							3
#define _MIDI_STAGE_4							4
#define _MIDI_STAGE_5							5
#define _MIDI_STAGE_6							6
#define _MIDI_STAGE_7							7

// #define _MIDI_SYSEX_RASPI_OFF_MESSAGE_ID_1		0x21
// #define _MIDI_SYSEX_RASPI_OFF_MESSAGE_ID_2		0x2e

#define _MIDI_SYSEX_AUX_KNOB_1_MESSAGE_ID		0x30
#define _MIDI_SYSEX_AUX_KNOB_2_MESSAGE_ID		0x31
#define _MIDI_SYSEX_AUX_KNOB_3_MESSAGE_ID		0x32
#define _MIDI_SYSEX_AUX_KNOB_4_MESSAGE_ID		0x33
#define _MIDI_SYSEX_AUX_KNOB_5_MESSAGE_ID		0x34
#define _MIDI_SYSEX_AUX_KNOB_6_MESSAGE_ID		0x35
#define _MIDI_SYSEX_AUX_KNOB_7_MESSAGE_ID		0x36
#define _MIDI_SYSEX_AUX_KNOB_8_MESSAGE_ID		0x37
#define _MIDI_SYSEX_AUX_KNOB_9_MESSAGE_ID		0x38
#define _MIDI_SYSEX_AUX_KNOB_10_MESSAGE_ID		0x39
#define _MIDI_SYSEX_AUX_KNOB_11_MESSAGE_ID		0x3a
#define _MIDI_SYSEX_AUX_KNOB_12_MESSAGE_ID		0x3b
#define _MIDI_SYSEX_AUX_KNOB_13_MESSAGE_ID		0x3c
#define _MIDI_SYSEX_AUX_KNOB_14_MESSAGE_ID		0x3d
#define _MIDI_SYSEX_AUX_KNOB_15_MESSAGE_ID		0x3e
#define _MIDI_SYSEX_AUX_KNOB_16_MESSAGE_ID		0x3f

#define _MIDI_SYSEX_AUX_KNOB_1_SW_MESSAGE_ID	0x40
#define _MIDI_SYSEX_AUX_KNOB_2_SW_MESSAGE_ID	0x41
#define _MIDI_SYSEX_AUX_KNOB_3_SW_MESSAGE_ID	0x42
#define _MIDI_SYSEX_AUX_KNOB_4_SW_MESSAGE_ID	0x43
#define _MIDI_SYSEX_AUX_KNOB_5_SW_MESSAGE_ID	0x44
#define _MIDI_SYSEX_AUX_KNOB_6_SW_MESSAGE_ID	0x45
#define _MIDI_SYSEX_AUX_KNOB_7_SW_MESSAGE_ID	0x46
#define _MIDI_SYSEX_AUX_KNOB_8_SW_MESSAGE_ID	0x47
#define _MIDI_SYSEX_AUX_KNOB_9_SW_MESSAGE_ID	0x48
#define _MIDI_SYSEX_AUX_KNOB_10_SW_MESSAGE_ID	0x49
#define _MIDI_SYSEX_AUX_KNOB_11_SW_MESSAGE_ID	0x4a
#define _MIDI_SYSEX_AUX_KNOB_12_SW_MESSAGE_ID	0x4b
#define _MIDI_SYSEX_AUX_KNOB_13_SW_MESSAGE_ID	0x4c
#define _MIDI_SYSEX_AUX_KNOB_14_SW_MESSAGE_ID	0x4d
#define _MIDI_SYSEX_AUX_KNOB_15_SW_MESSAGE_ID	0x4e
#define _MIDI_SYSEX_AUX_KNOB_16_SW_MESSAGE_ID	0x4f

#define _MIDI_SYSEX_AUX_SLIDER_1_MESSAGE_ID		0x50
#define _MIDI_SYSEX_AUX_SLIDER_2_MESSAGE_ID		0x51
#define _MIDI_SYSEX_AUX_SLIDER_3_MESSAGE_ID		0x52
#define _MIDI_SYSEX_AUX_SLIDER_4_MESSAGE_ID		0x53
#define _MIDI_SYSEX_AUX_SLIDER_5_MESSAGE_ID		0x54
#define _MIDI_SYSEX_AUX_SLIDER_6_MESSAGE_ID		0x55
#define _MIDI_SYSEX_AUX_SLIDER_7_MESSAGE_ID		0x56
#define _MIDI_SYSEX_AUX_SLIDER_8_MESSAGE_ID		0x57
#define _MIDI_SYSEX_AUX_SLIDER_9_MESSAGE_ID		0x58
#define _MIDI_SYSEX_AUX_SLIDER_10_MESSAGE_ID	0x59
#define _MIDI_SYSEX_AUX_SLIDER_11_MESSAGE_ID	0x5a
#define _MIDI_SYSEX_AUX_SLIDER_12_MESSAGE_ID	0x5b
#define _MIDI_SYSEX_AUX_SLIDER_13_MESSAGE_ID	0x5c
#define _MIDI_SYSEX_AUX_SLIDER_14_MESSAGE_ID	0x5d
#define _MIDI_SYSEX_AUX_SLIDER_15_MESSAGE_ID	0x5e
#define _MIDI_SYSEX_AUX_SLIDER_16_MESSAGE_ID	0x5f


#define _MIDI_SYSEX_AUX_PUSHBUTTON_1_MESSAGE_ID		0x60
#define _MIDI_SYSEX_AUX_PUSHBUTTON_2_MESSAGE_ID		0x61
#define _MIDI_SYSEX_AUX_PUSHBUTTON_3_MESSAGE_ID		0x62
#define _MIDI_SYSEX_AUX_PUSHBUTTON_4_MESSAGE_ID		0x63
#define _MIDI_SYSEX_AUX_PUSHBUTTON_5_MESSAGE_ID		0x64
#define _MIDI_SYSEX_AUX_PUSHBUTTON_6_MESSAGE_ID		0x65
#define _MIDI_SYSEX_AUX_PUSHBUTTON_7_MESSAGE_ID		0x66
#define _MIDI_SYSEX_AUX_PUSHBUTTON_8_MESSAGE_ID		0x67
#define _MIDI_SYSEX_AUX_PUSHBUTTON_9_MESSAGE_ID		0x68
#define _MIDI_SYSEX_AUX_PUSHBUTTON_10_MESSAGE_ID	0x69
#define _MIDI_SYSEX_AUX_PUSHBUTTON_11_MESSAGE_ID	0x6a
#define _MIDI_SYSEX_AUX_PUSHBUTTON_12_MESSAGE_ID	0x6b
#define _MIDI_SYSEX_AUX_PUSHBUTTON_13_MESSAGE_ID	0x6c
#define _MIDI_SYSEX_AUX_PUSHBUTTON_14_MESSAGE_ID	0x6d
#define _MIDI_SYSEX_AUX_PUSHBUTTON_15_MESSAGE_ID	0x6e
#define _MIDI_SYSEX_AUX_PUSHBUTTON_16_MESSAGE_ID	0x6f

#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_1_MESSAGE_ID		0x70
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_2_MESSAGE_ID		0x71
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_3_MESSAGE_ID		0x72
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_4_MESSAGE_ID		0x73
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_5_MESSAGE_ID		0x74
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_6_MESSAGE_ID		0x75
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_7_MESSAGE_ID		0x76
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_8_MESSAGE_ID		0x77
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_9_MESSAGE_ID		0x78
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_10_MESSAGE_ID		0x79
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_11_MESSAGE_ID		0x7a
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_12_MESSAGE_ID		0x7b
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_13_MESSAGE_ID		0x7c
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_14_MESSAGE_ID		0x7d
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_15_MESSAGE_ID		0x7e
#define _MIDI_SYSEX_AUX_FUNC_PUSHBUTTON_16_MESSAGE_ID		0x7f


// Table of midi note frequencies 
//   Generated from Excel by =ROUND(440/32*(2^((x-9)/12)),0) for 0<x<128

const float tune_frequencies_PGM[128] =
{
	  8.1758,    8.6620,     9.1770,    9.7227,    10.3009,    10.9134,    11.5623,   12.2499,
	 12.9783,   13.7500,    14.5676,   15.4339,    16.3516,    17.3239,    18.3540,   19.4454,
	 20.6017,   21.8268,    23.1247,   24.4997,    25.9565,    27.5000,    29.1352,   30.8677,
	 32.7032,   34.6478,    36.7081,   38.8909,    41.2034,    43.6535,    46.2493,   48.9994,
	 51.9131,   55.0000,    58.2705,   61.7354,    65.4064,    69.2957,    73.4162,    77.7817,
	 82.4069,   87.3071,    92.4986,   97.9989,   103.8262,   110.0000,   116.5409,   123.4708,
	130.8128,  138.5913,   146.8324,  155.5635,   164.8138,   174.6141,   184.9972,   195.9977,
	207.6523,  220.0000,   233.0819,  246.9417,   261.6256,   277.1826,   293.6648,   311.1270,
	329.6276,  349.2282,   369.9944,  391.9954,   415.3047,   440.0000,   466.1638,   493.8833,
	523.2511,  554.3653,   587.3295,  622.2540,   659.2551,   698.4565,   739.9888,   783.9909,
	830.6094,  880.0000,   932.3275,  987.7666,  1046.5023,  1108.7305,  1174.6591,  1244.5079,
	1318.5102, 1396.9129, 1479.9777, 1567.9817,  1661.2188,  1760.0000,  1864.6550,  1975.5332,
	2093.0045, 2217.4610, 2349.3181, 2489.0159,  2637.0205,  2793.8259,  2959.9554,  3135.9635,
	3322.4376, 3520.0000, 3729.3101, 3951.0664,  4186.0090,  4434.9221,  4698.6363,  4978.0317,
	5274.0409, 5587.6517, 5919.9108, 6271.9270,  6644.8752,  7040.0000,  7458.6202,  7902.1328,
	8372.0181, 8869.8442, 9397.2726, 9956.0635, 10548.0818, 11175.3034, 11839.8215, 12543.8540
};

class MidiStream;
class MidiConnection;

/* Data structure used for raw data */
typedef struct
{
	uint8_t ref_count; 				// This block is referenced by ref_count streams.
									// Any referenced stream decrements it by 1 when used.
									// It can be relased back to pool only when zeroed.
	uint8_t memory_pool_index; 		// Block number
	uint8_t len;
	uint8_t bytes[_RAWDATA_MSSG_MAX_LEN];
} raw_data_mssg_block_t;

/* Data structure used for data handling by MidiStream objects */
typedef	struct
{
	// Block common
	uint8_t ref_count; 				// This block is referenced by ref_count streams.
									// Any referenced stream decrements it by 1 when used.
									// It can be relased back to pool only when zeroed.
	uint8_t memory_pool_index; 		// Block number

	uint8_t header_type; 			// MIDISTREAM_HEADER_TYPE_BULK, MIDISTREAM_HEADER_TYPE_MIDI_SINGLE....
	uint8_t header_length; 			// Number of valid bytes
	uint8_t header_reseved1;
	uint8_t header_reseved2;

	// Block data bytes: may be refernced in various ways
	union
	{
		// Used for MIDI type messages refernces
		struct mssg
		{
			uint8_t command;
			uint8_t byte2;
			uint8_t byte3;
			uint8_t byte4;
			uint8_t byte5;
		} midimssg_t;

		// Used for direct data refernce
		uint8_t bytes[_MIDI_MSSG_MAX_LEN]; 
	} data_t;

} midi_stream_mssg_block_t;

/* Used for connecting and transferring mid-blocks between midi-stream objects */
class MidiConnection {
public:
	MidiConnection(MidiStream* source, MidiStream* destination);

	MidiConnection(MidiStream* source,
		unsigned char source_output_num,
		MidiStream* destination,
		unsigned char destination_input_num);

	friend class MidiStream;

protected:
	void connect(void);

	MidiStream* src;
	MidiStream* dst;
	volatile unsigned char src_index;
	volatile unsigned char dest_index;
	MidiConnection* next_dest;
};

/* Allocates a pool of n MIDI stream messages */
#define allocate_midi_stream_messages_memory_pool(num) ({ \
	if (num > _MAX_MIDI_STREAM_MESSAGES_POOL_SIZE) { \
		static midi_stream_mssg_block_t data[_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE]; \
		MidiStream::initialize_midi_stream_memory(data, _MAX_MIDI_STREAM_MESSAGES_POOL_SIZE); \
	} \
	else { \
		static midi_stream_mssg_block_t data[num]; \
		MidiStream::initialize_midi_stream_memory(data, num); \
	} \
} )

/* Allocates a pool of n Raw data stream messages */
#define allocate_raw_data_mssgs_memory_pool(num) ({ \
	if (num > _MAX_RAWDATA_MSSGS_POOL_SIZE) { \
		static raw_data_mssg_block_t data[_MAX_RAWDATA_MSSGS_POOL_SIZE]; \
		MidiStream::initialize_rawdata_mssgs_memory(data, _MAX_RAWDATA_MSSGS_POOL_SIZE); \
	} \
	else { \
		static raw_data_mssg_block_t data[num]; \
		MidiStream::initialize_rawdata_mssgs_memory(data, num); \
	} \
} )


class MidiStream {

public:
	MidiStream(uint8_t ninput, midi_stream_mssg_block_t** iqueue, uint8_t stage = 0);
	virtual ~MidiStream();

	static void initialize_midi_stream_memory(midi_stream_mssg_block_t* data, unsigned int num);
	static void initialize_rawdata_mssgs_memory(raw_data_mssg_block_t* data, unsigned int num);

	static void start_thread();
	static void stop_thread();

	static void update_setup(void);
	static void update_stop(void);

	uint8_t get_stage_num();
	static void dbg_print_update_list(void);

	static void update_all(void);
	static bool update_is_in_progress();

	static std::mutex update_all_mutex;
	static std::mutex thread_creation_mutex;

	//	friend class MidiOutputPrint;

protected:
	volatile bool active;
	volatile uint8_t num_inputs;
	volatile uint8_t num_outputs;
	volatile uint8_t stage_num;

	static midi_stream_mssg_block_t* allocate_midi_stream_block(void);
	static void release_midi_stream_block(midi_stream_mssg_block_t* block);
	static raw_data_mssg_block_t* allocate_raw_data_mssg_block(void);
	static void release_raw_data_mssg_block(raw_data_mssg_block_t* block);
	void transmit(midi_stream_mssg_block_t* block, unsigned char index = 0);
	midi_stream_mssg_block_t* receive_read_only(unsigned int index = 0);
	midi_stream_mssg_block_t* receive_writable(unsigned int index = 0);

	friend class MidiConnection;

private:
	/* Perform block processing */
	virtual void update(void) = 0;
	/* Midi timer function */
	static void* run_midi(void* threadid);

	/* Main thread running midi loop */
	static pthread_t midi_thread;

	/* holds a linked list of objects to be propagate data through */
	MidiConnection* destination_list;
	/* for update_all - holds the 1st object in the propagate chain */
	static MidiStream* first_update[_MAX_STAGES_NUM];   // for update_all
	/* for update_all */
	MidiStream* next_update[_MAX_STAGES_NUM];   // for update_all
	/* Input queues */
	midi_stream_mssg_block_t** input_queue;
	/* When true update process is enables */
	static bool update_enable;
	/* A memory pool of midi blocks
	 When true, the midi timer thread is running */
	static bool thread_is_running;
	/* When true update process is in progress */
	static bool update_in_progress;

	static midi_stream_mssg_block_t* midi_stream_memory_pool;
	/* Used for marking used/free pool resources */
	static uint32_t midi_stream_memory_pool_available_mask[_MIDI_STREAM_MESSAGES_POOL_NUM_OF_MASKS];
	//	static uint16_t midi_stream_blocks_memory_pool_first_mask;
	static raw_data_mssg_block_t* raw_data_mssgs_memory_pool;
	/* Used for marking used/free pool resources */
	static uint32_t raw_data_mssgs_memory_pool_available_mask[_RAWDATA_MESSAGES_POOL_NUM_OF_MASKS];
	//	static uint16_t rawdataMssgs_blocks_memory_pool_first_mask;

	/* Mutex to handle allocations opperations */
	static std::mutex allocations_manage_mutex;
};

