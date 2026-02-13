/**
* @file			audio.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		Provides the Audio related API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#define _AUDIO_JACK 0
#define _AUDIO_ALSA 1
#define _DEFAULT_AUDIO_DRIVER _AUDIO_JACK

#define _SAMPLE_RATE_44 44100
#define _SAMPLE_RATE_48 48000
#define _DEFAULT_SAMPLE_RATE _SAMPLE_RATE_44

#define _AUDIO_BLOCK_SIZE_256 256
#define _AUDIO_BLOCK_SIZE_512 512
#define _AUDIO_BLOCK_SIZE_1024 1024
#define _DEFAULT_BLOCK_SIZE _AUDIO_BLOCK_SIZE_512

#define _MAX_NUM_OF_JACK_AUDIO_OUT_PORTS 2
#define _MAX_NUM_OF_JACK_AUDIO_IN_PORTS 2
#define _MAX_NUM_OF_JACK_MIDI_IN_PORTS 4

#define _MAX_NUM_OF_JACK_CLIENTS 16
#define _MAX_NUM_OF_JACK_CLIENT_PORTS 16
#define _JACK_CLIENT_INPUT 0
#define _JACK_CLIENT_OUTPUT 1

/**
*   @brief  Starts the audio service.
*			Must be called after intializtion.
*   @param  none
*   @return 0 if done
*/
int mod_synth_start_audio();

/**
*   @brief  Stops the audio service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_stop_audio();


