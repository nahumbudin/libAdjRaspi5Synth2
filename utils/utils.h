/**
*	@file		utils.h
*	@author		Nahum Budin
*	@date		11-May-2024
*	@version	1.1 
*					1. Update includes
*
*	@brief		Utilities.
* 
*	History:\n
* 
*	version	1.0		31-Oct-2019
*	(revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*/

#pragma once

#include <vector>

using namespace std;

#include <math.h>
#include <stdint.h>
#include <string>
#include <cstdint>

#define PI 3.1415926536f

//Random number generator
typedef uint32_t prng_t;
extern prng_t prng_state;  // defined at utils.cpp

class Utils
{
public:

	static float calc_log_scale_100_float(float min, float max, float base, int val);
	static int calc_log_scale_100_int(int min, int max, int base, int val);

	static std::string execute_system_command_getstd_out(std::string cmd);
	
	static pid_t get_process_id(char *process_name);
};


#define return_if_true(cond) \
if(cond) \
    return

#define return_val_if_true(cond, val) \
 return_if_true(cond) (val)

#define return_if_fail(cond) \
if(cond) \
    ; \
else \
    return

#define return_val_if_fail(cond, val) \
 return_if_fail(cond) (val)

/**
* Random numbers generator (0.0f..1.0f)
*/
// Portable Pseudo-Random Number Generator
#ifndef INT32_MAX
#define INT32_MAX      (2147483647)
#endif
#define RND (prng() / (INT32_MAX * 1.0f))

inline prng_t prng_r(prng_t& p)
{
	return p = p * 1103515245 + 12345;
}

inline prng_t prng(void)
{
	return prng_r(prng_state) & 0x7fffffff;
}

inline void sprng(prng_t p)
{
	prng_state = p;
}

bool is_valid_audio_driver(int driver);
bool is_valid_sample_rate(int samp_rate);
bool is_valid_audio_block_size(int size);
bool is_valid_midi_mapping_mode(int mode);
bool is_valid_voice_state(int state);
bool is_valid_pad_harmonies_envelope_shape(int shape);
bool is_valid_pad_harmonies_envelope_shape_cutoff(int shcutof);
unsigned long calc_period_time_us(int samp_rate, int block_size);

long get_current_time_ms();

int write_text_file(string full_path, string file_data);
int read_text_file(string full_path, vector<char> *file_data);

