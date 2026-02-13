/**
*	@file		synthKeyboard.cpp
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	1.1 29-Jan-2021
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*				1.0		9-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 15, 2018)
*
*	@brief		Keyboard handling
*/

#include <mutex>

#include "synthKeyboard.h"
#include "../Settings/settings.h"
#include "../utils/utils.h"
#include "../LibAPI/synthesizer.h"
#include "../MIDI/midiStream.h" // for tune_frequencies_PGM[]

// Mutex to handle busy/not-busy voices marking (Synthesizer.cpp) 
extern pthread_mutex_t voice_busy_mutex;

/**
*   @brief  Create and initialize a SynthKeyboard object instance.
*   @param	none
*   @return none
*/
SynthKeyboard::SynthKeyboard(int samp_rate, int block_size)
{
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
	
	portamento_enabled = false;
	portamento_time = 0.0f;
	set_sensitivity(50);
	set_low_sensitivity(50);
	set_split_point(_KBD_SPLIT_POINT_NONE);
	note_freq = 440.0f;
	actual_freq = 440.0f;	
	gettimeofday(&start_time, NULL);
}

SynthKeyboard::~SynthKeyboard()
{
	
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int SynthKeyboard::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int SynthKeyboard::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int SynthKeyboard::set_audio_block_size(int size)
{
	int res = 0;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;				
		res = audio_block_size;
	}
	else
	{
		res = -1;
	}
	
	return res;
}
	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int SynthKeyboard::get_audio_block_size() { return audio_block_size; }


/**
*   @brief  Set portamento time.
*   @param	porta	portamento time (sec) 0.0 to PORTAMENTO_MAX_TIME_SEC
*   @return void
*/
void SynthKeyboard::set_portamento_time(float porta)
{
	portamento_time = porta;
	
	if (portamento_time < 0.0f)
	{	
		portamento_time = 0.0f;
	}
	else if (portamento_time > _PORTAMENTO_MAX_TIME_SEC)
	{	
		portamento_time = _PORTAMENTO_MAX_TIME_SEC;
	}
	
	// no (divide by) zero time
	porata_div = 1.0f - (float)((float)audio_block_size / (portamento_time + 0.004f) / (float)sample_rate);
}

/**
*   @brief  Set portamento time (int).
*   @param	porta	portamento time (sec) 0 to 100
*					(converted to 0.0 to PORTAMENTO_MAX_TIME_SEC
*					using log10 scale)
*   @return void
*/
void SynthKeyboard::set_portamento_time(int porta)
{
	set_portamento_time(Utils::calc_log_scale_100_float(0.0f, _PORTAMENTO_MAX_TIME_SEC, 10.0, porta));
}

/**
*   @brief  Return portamento time.
*   @param	none	
*   @return portamento time (sec) 0.0 to PORTAMENTO_MAX_TIME_SEC
*/
float SynthKeyboard::get_portamento_level() { return portamento_time; }

/**
*   @brief  Set keyboard sensitivity (full kbd when no split, high part when split enabled).
*   @param	sens	0 to 100
*   @return void
*/
void SynthKeyboard::set_sensitivity(int sens)
{
	sensitivity = 100 - sens; // (0 - low sensitivity; 100 - high sensitivity)
	if (sensitivity < 0) 
	{
		sensitivity = 0;
	}
	else if (sensitivity > 100) 
	{
		sensitivity = 100;
	}
	
	sensitivity_offset = (float)sensitivity * 1.2f; // (0.0-120.0)
	sensitivity_slope = (127.0 - sensitivity_offset) / 127.0f;	
}

/**
*   @brief  Return keyboard sensitivity (full kbd when no split, high part when split enabled).
*   @param	none
*   @return keyboard sensitivity 0-100
*/
int SynthKeyboard::get_sensitivity() 
{ 
	return sensitivity; 
}


/**
*   @brief  Set keyboard low part sensitivity (when split enabled).
*   @param	sens	0 to 100
*   @return void
*/
void SynthKeyboard::set_low_sensitivity(int sens)
{
	low_sensitivity = 100 - sens; // (0 - low sensitivity; 100 - high sensitivity)
	if (low_sensitivity < 0) 
	{
		low_sensitivity = 0;
	}
	else if (low_sensitivity > 100) 
	{
		low_sensitivity = 100;
	}

	low_sensitivity_offset = (float)low_sensitivity * 1.2f; // (0.0-120.0)
	low_sensitivity_slope = (127.0 - low_sensitivity_offset) / 127.0f;
}

/**
*   @brief  Return keyboard low part sensitivity.
*   @param	none
*   @return keyboard low part sensitivity 0-100
*/
int SynthKeyboard::get_low_sensitivity() 
{
	return low_sensitivity; 
}

/**
*   @brief  Set keyboard split point.
*   @param	sp	split point (midi note) _KBD_SPLIT_POINT_NONE, _KBD_SPLIT_POINT_C2 - C5
*   @return void
*/
void SynthKeyboard::set_split_point(int sp)
{
	if ((sp >= _KBD_SPLIT_POINT_NONE) && (sp <= _KBD_SPLIT_POINT_C5))
	{	
		split_point = sp;
	}
}

/**
*   @brief Return keyboard split point.
*   @param	none	
*   @return split point (midi note) _KBD_SPLIT_POINT_NONE, _KBD_SPLIT_POINT_C2 - C5
*/
int SynthKeyboard::get_split_point() 
{
	return split_point;
}

/**
*   @brief Return scaled velocity (full or high part) based on high sensitivity setting).
*   @param	vel		original velocity	
*   @return scaled velocity
*/
int SynthKeyboard::get_scaled_velocity(int vel)
{
	int res;
	
	if (vel < 0)
	{	
		res = 0;
	}
	else if (vel > 127)
	{	
		res = 127;
	}
	else
	{
		res = (int)((float)vel * sensitivity_slope + sensitivity_offset - 0.5f);
	}
	
	return res;
}

/**
*   @brief Return scaled velocity (low part) based on loe sensitivity setting).
*   @param	vel		original velocity	
*   @return scaled velocity
*/
int SynthKeyboard::get_low_scaled_velocity(int vel)
{
	int res;
	
	if (vel < 0)
	{	
		res =  0;
	}
	else if (vel > 127)
	{	
		res = 127;
	}
	else
	{
		res = (int)((float)vel * low_sensitivity_slope + low_sensitivity_offset - 0.5f);
	}
	
	return res;
}

/**
*   @brief Enable portamento
*   @param	none	
*   @return void
*/
void SynthKeyboard::enable_portamento() 
{ 
	portamento_enabled = true; 
}

/**
*   @brief Disable portamento
*   @param	none	
*   @return void
*/
void SynthKeyboard::disable_portamento() 
{ 
	portamento_enabled = false; 
}

/**
*   @brief Return portamento state
*   @param	none	
*   @return true if portamento enabled; false otherwise
*/
bool SynthKeyboard::portamento_is_enabled() { return portamento_enabled; }

/**
*   @brief Return playd note frequency (actual)
*   @param	none	
*   @return current playd note frequency (Hz)
*/
float SynthKeyboard::get_note_frequency()
{
	if (portamento_enabled)
	{	
		return actual_freq;
	}
	else
	{	
		return note_freq;
	}
}

/**
*   @brief Update playd note frequency (gliding as time passes)
*   @param	none	
*   @return void
*/
void SynthKeyboard::update_actual_frequency()
{
	if (note_diff > 0)
	{
		// Glide up
		actual_freq /= porata_div;
		if (actual_freq > note_freq)
		{
			actual_freq = note_freq;
			note_diff = 0;
		}
	}
	else if (note_diff < 0)
	{
		// Glide down
		actual_freq *= porata_div;
		if (actual_freq < note_freq)
		{
			actual_freq = note_freq;
			note_diff = 0;
		}
	}
	else
	{
		// Same note - no glide
		actual_freq = note_freq;
	}
}

/**
*   @brief Start a midi note on portamento process
*   @param	channel		midi channel
*   @param	byte2		note num (byte 2 of midi-on event)
*   @param	byte3		note velocity (byte 3 of midi-on event)
*   @return void
*/
void  SynthKeyboard::midi_play_note_on(uint8_t channel, uint8_t byte2, uint8_t byte3)
{
	note_diff = byte2 - prev_note_ind;
	prev_note_ind = byte2;
	note_freq = tune_frequencies_PGM[byte2];
}

void  SynthKeyboard::midi_play_note_off(uint8_t channel, uint8_t byte2, uint8_t byte3)
{
	// no effect on portamento!?
}
