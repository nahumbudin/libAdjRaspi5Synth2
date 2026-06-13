/**
 * @file		audioJackRecording.h
 * @author		Nahum Budin (With Github Copilot)
 * @date		12-June-2026
 * @version	1.0
 *
 * @brief		Record mixed audio from JACK input ports (captures all JACK clients)
 */

#pragma once

#include "audioRecording.h"
#include <jack/jack.h>

class AudioJackRecording : public AudioRecording
{
public:
	AudioJackRecording(int buffer_size = 512);
	~AudioJackRecording();
    
	/**
	 * @brief Initialize JACK input client for recording
	 * @return 0 if success, negative on error
	 */
	int init_jack_recording_client();
    
	/**
	 * @brief Close JACK recording client
	 * @return 0 if success
	 */
	int close_jack_recording_client();
    
	/**
	 * @brief JACK process callback (called by JACK server)
	 */
	static int jack_recording_process_callback(jack_nframes_t nframes, void *arg);

private:
	jack_client_t *jack_client;
	jack_port_t *jack_input_port_left;
	jack_port_t *jack_input_port_right;
	bool jack_initialized;
};
