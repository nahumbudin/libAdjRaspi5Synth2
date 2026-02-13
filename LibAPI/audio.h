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

#define _LEFT								0
#define _RIGHT								1		
#define _SEND_LEFT							2
#define _SEND_RIGHT							3

#define _AUDIO_JACK							0
#define _AUDIO_ALSA							1
#define _DEFAULT_AUDIO_DRIVER				_AUDIO_JACK

#define _SAMPLE_RATE_44						44100
#define _SAMPLE_RATE_48						48000
#define _DEFAULT_SAMPLE_RATE				_SAMPLE_RATE_44

#define _AUDIO_BLOCK_SIZE_256				256
#define _AUDIO_BLOCK_SIZE_512				512
#define _AUDIO_BLOCK_SIZE_1024				1024
#define _DEFAULT_BLOCK_SIZE					_AUDIO_BLOCK_SIZE_512

#define _DEFAULT_ALSA_AUDIO_DEVICE			{ "plughw:1,0" }
#define _DEFAULT_ALSA_PERIOD_TIME_USEC		10000		
#define _DEFAULT_ALSA_AUD_PERIOD_TIME_USEC	((_DEFAULT_BLOCK_SIZE*1000000)/_DEFAULT_SAMPLE_RATE + 0.5)

#define _DEFAULT_JACK_AUD_PERIOD_TIME_USEC	((_DEFAULT_BLOCK_SIZE*1000000)/_DEFAULT_SAMPLE_RATE + 0.5)

#define _MAX_NUM_OF_JACK_AUDIO_OUT_PORTS	2
#define _MAX_NUM_OF_JACK_AUDIO_IN_PORTS		2
#define _MAX_NUM_OF_JACK_MIDI_IN_PORTS		4

#define _MAX_NUM_OF_JACK_CLIENTS			64
#define _MAX_NUM_OF_JACK_CLIENT_PORTS		64
#define _MAX_NUM_OF_GUI_JACK_CLIENTS		8
#define _MAX_NUM_OF_GUI_JACK_CLIENT_PORTS	8
#define _JACK_CLIENT_INPUT					0
#define _JACK_CLIENT_OUTPUT					1

#define _JACK_MODE_APP_CONTROL				0	// application sets JACK params
#define _JACK_MODE_SERVER_CONTROL			1	// JACK server sets application params
#define _DEFAULT_JACK_MODE					_JACK_MODE_SERVER_CONTROL
	
#define _JACK_AUTO_START_DIS				false
#define _JACK_AUTO_START_EN					true
#define _DEFAULT_JACK_AUTO_START			_JACK_AUTO_START_EN

#define _JACK_AUTO_CONNECT_AUDIO_DIS		false
#define _JACK_AUTO_CONNECT_AUDIO_EN			true
#define _DEFAULT_JACK_AUTO_CONNECT_AUDIO	_JACK_AUTO_CONNECT_AUDIO_EN
	
#define _JACK_AUTO_CONNECT_MIDI_DIS			false
#define _JACK_AUTO_CONNECT_MIDI_EN			true
#define _DEFAULT_JACK_AUTO_CONNECT_MIDI		_JACK_AUTO_CONNECT_MIDI_DIS

#define _MESSAGE_JACK_SERV_OUTPUT_NOT_RUNNING		1800
#define _MESSAGE_JACK_SERV_OUTPUT_RUNNING			1801
#define _MESSAGE_JACK_SERV_INPUT_NOT_RUNNING		1802
#define _MESSAGE_JACK_SERV_INPUT_RUNNING			1803


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


