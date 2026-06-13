/**
 * @file		audioJackRecording.h
 * @author		Nahum Budin (With Github Copilot)
 * @date		12-June-2026
 * @version	1.0
 *
 * @brief		Record mixed audio from JACK input ports (captures all JACK clients)
 */

#include "audioJackRecording.h"
#include <iostream>
#include <cstring>

AudioJackRecording::AudioJackRecording(int buffer_size)
	: AudioRecording(buffer_size)
	, jack_client(nullptr)
	, jack_input_port_left(nullptr)
	, jack_input_port_right(nullptr)
	, jack_initialized(false)
{
}

AudioJackRecording::~AudioJackRecording()
{
	close_jack_recording_client();
}

int AudioJackRecording::init_jack_recording_client()
{
	if (jack_initialized)
	{
		return 0;  // Already initialized
	}
    
	// Open JACK client
	jack_status_t status;
	jack_client = jack_client_open("AdjSynthRecorder", JackNullOption, &status);
    
	if (jack_client == nullptr)
	{
		std::cerr << "AudioJackRecording: Failed to open JACK client" << std::endl;
		return -1;
	}
    
	// Register input ports
	jack_input_port_left = jack_port_register(jack_client,
		"record_in_L",
		JACK_DEFAULT_AUDIO_TYPE,
		JackPortIsInput,
		0);
	jack_input_port_right = jack_port_register(jack_client,
		"record_in_R",
		JACK_DEFAULT_AUDIO_TYPE,
		JackPortIsInput,
		0);
    
	if (!jack_input_port_left || !jack_input_port_right)
	{
		std::cerr << "AudioJackRecording: Failed to register JACK ports" << std::endl;
		jack_client_close(jack_client);
		jack_client = nullptr;
		return -2;
	}
    
	// Set process callback
	jack_set_process_callback(jack_client, jack_recording_process_callback, this);
    
	// Activate client
	if (jack_activate(jack_client) != 0)
	{
		std::cerr << "AudioJackRecording: Failed to activate JACK client" << std::endl;
		jack_client_close(jack_client);
		jack_client = nullptr;
		return -3;
	}
    
	// Auto-connect to system playback (master output)
	const std::string recorder_in_L = std::string("AdjSynthRecorder:") + 
                                       jack_port_short_name(jack_input_port_left);
	const std::string recorder_in_R = std::string("AdjSynthRecorder:") + 
	                                   jack_port_short_name(jack_input_port_right);
    
	// Define exact source ports
	struct PortConnection
	{
		const char *source;
		const char *destination;
	};
    
	const PortConnection connections[] = {
		// AdjSynth outputs
		{ "AdjHeartSynth_out:AdjHeartSynthL_out", recorder_in_L.c_str() },
		{ "AdjHeartSynth_out:AdjHeartSynthR_out", recorder_in_R.c_str() },
		// FluidSynth outputs
		{ "Adj-Fluid-Synth:left", recorder_in_L.c_str() },
		{ "Adj-Fluid-Synth:right", recorder_in_R.c_str() }
	};
    
	// Make all connections
	for (const auto &conn : connections)
	{
		int result = jack_connect(jack_client, conn.source, conn.destination);
        
		if (result == 0)
		{
			std::cout << "AudioJackRecording: ✓ Connected " 
			          << conn.source << " -> " << conn.destination << std::endl;
		}
		else if (result == EEXIST)
		{
			std::cout << "AudioJackRecording: ⚠ Already connected: " 
			          << conn.source << std::endl;
		}
		else
		{
			std::cerr << "AudioJackRecording: ✗ Failed to connect: " 
			          << conn.source << " (error: " << result << ")" << std::endl;
		}
	}
    
	jack_initialized = true;
	std::cout << "AudioJackRecording: JACK client initialized and connected" << std::endl;
    
	return 0;
}

int AudioJackRecording::close_jack_recording_client()
{
	if (jack_client)
	{
		jack_deactivate(jack_client);
		jack_client_close(jack_client);
		jack_client = nullptr;
	}
    
	jack_initialized = false;
	return 0;
}

int AudioJackRecording::jack_recording_process_callback(jack_nframes_t nframes, void *arg)
{
	AudioJackRecording *recorder = static_cast<AudioJackRecording*>(arg);
    
	if (!recorder->is_recording())
	{
		return 0;
	}
    
	// Get input buffers from JACK
	jack_default_audio_sample_t *in_left = 
	    (jack_default_audio_sample_t*)jack_port_get_buffer(recorder->jack_input_port_left, nframes);
	jack_default_audio_sample_t *in_right = 
	    (jack_default_audio_sample_t*)jack_port_get_buffer(recorder->jack_input_port_right, nframes);
    
	// Record samples (JACK uses float samples, so direct pass)
	recorder->record_samples(in_left, in_right, nframes);
    
	return 0;
}