/**
 * @file		audioRecording.cpp
 * @author		Nahum Budin (With Github Copilot)
 * @date		16-Jan-2026
 * @version	1.0
 *
 * @brief		MP3 audio recording implementation using LAME encoder
 */

#include "audioRecording.h"
#include "../LibAPI/audio.h"
#include <cstring>
#include <iostream>

/**
 * @brief Constructor
 */
AudioRecording::AudioRecording(int buffer_size)
	: recording_active(false)
	, recording_mode(_RECORDING_MODE_STEREO)
	, lame_flags(nullptr)
	, lame_sample_rate(44100)
	, pcm_buffer_left(nullptr)
	, pcm_buffer_right(nullptr)
	, pcm_buffer_size(buffer_size)
	, pcm_samples_accumulated(0)
	, mp3_buffer(nullptr)
	, mp3_buffer_size(0)
{
	// Buffers will be allocated when recording starts
}

/**
 * @brief Destructor
 */
AudioRecording::~AudioRecording()
{
	if (recording_active)
	{
		stop_recording();
	}
	cleanup_resources();
}

/**
 * @brief Start MP3 recording
 */
int AudioRecording::start_recording(int mode, const std::string &path, int bitrate, int sample_rate)
{
	if (recording_active)
	{
		std::cerr << "AudioRecording: Recording already in progress" << std::endl;
		return -1;
	}
    
	// Validate mode
	if (mode < _RECORDING_MODE_STEREO || mode > _RECORDING_MODE_RIGHT)
	{
		std::cerr << "AudioRecording: Invalid recording mode: " << mode << std::endl;
		return -3;
	}
    
	recording_mode = mode;
	recording_file_path = path;
	lame_sample_rate = sample_rate;
    
	// Open output file
	mp3_file.open(path, std::ios::binary | std::ios::trunc);
	if (!mp3_file.is_open())
	{
		std::cerr << "AudioRecording: Failed to open file: " << path << std::endl;
		return -2;
	}
    
	// Initialize LAME encoder
	lame_flags = lame_init();
	if (lame_flags == nullptr)
	{
		std::cerr << "AudioRecording: Failed to initialize LAME encoder" << std::endl;
		mp3_file.close();
		return -4;
	}
    
	// Configure LAME
	lame_set_in_samplerate(lame_flags, sample_rate);
	lame_set_out_samplerate(lame_flags, sample_rate);
	lame_set_brate(lame_flags, bitrate);
	lame_set_quality(lame_flags, 2); // 0=best (slow), 9=worst (fast), 2=high quality
    
	if (mode == _RECORDING_MODE_STEREO)
	{
		lame_set_num_channels(lame_flags, 2);
		lame_set_mode(lame_flags, STEREO);
	}
	else
	{
		lame_set_num_channels(lame_flags, 1);
		lame_set_mode(lame_flags, MONO);
	}
    
	// Initialize encoder with parameters
	if (lame_init_params(lame_flags) < 0)
	{
		std::cerr << "AudioRecording: Failed to set LAME parameters" << std::endl;
		lame_close(lame_flags);
		lame_flags = nullptr;
		mp3_file.close();
		return -5;
	}
    
	// Allocate PCM buffers
	pcm_buffer_left = new float[pcm_buffer_size];
	pcm_buffer_right = new float[pcm_buffer_size];
	std::memset(pcm_buffer_left, 0, pcm_buffer_size * sizeof(float));
	std::memset(pcm_buffer_right, 0, pcm_buffer_size * sizeof(float));
    
	// Allocate MP3 buffer (LAME recommends 1.25 * samples + 7200)
	mp3_buffer_size = static_cast<int>(1.25 * pcm_buffer_size + 7200);
	mp3_buffer = new unsigned char[mp3_buffer_size];
    
	pcm_samples_accumulated = 0;
    
	// Start recording
	recording_active = true;
    
	std::cout << "AudioRecording: Started - " << path 
	          << " (" << bitrate << " kbps, " << sample_rate << " Hz, "
	          << (mode == _RECORDING_MODE_STEREO ? "stereo" : 
	              mode == _RECORDING_MODE_LEFT ? "left" : "right") 
	<< ")" << std::endl;
    
	return 0;
}

/**
 * @brief Stop MP3 recording
 */
int AudioRecording::stop_recording()
{
	std::lock_guard<std::mutex> lock(recording_mutex);

	if (!recording_active)
	{
		std::cerr << "AudioRecording: No recording in progress" << std::endl;
		return -1;
	}

	// Set flag immediately to prevent re-entry
	recording_active = false;

	// Flush any remaining samples
	flush_mp3_encoder();

	// Write final MP3 frames and VBR tag
	if (lame_flags != nullptr)
	{
		unsigned char mp3_finalize_buffer[7200];
		int final_bytes = lame_encode_flush(lame_flags, mp3_finalize_buffer, sizeof(mp3_finalize_buffer));
		if (final_bytes > 0)
		{
			mp3_file.write(reinterpret_cast<char *>(mp3_finalize_buffer), final_bytes);
		}

		lame_close(lame_flags);
		lame_flags = nullptr;
	}

	// Close file
	if (mp3_file.is_open())
	{
		mp3_file.close();
	}

	std::cout << "AudioRecording: Stopped - " << recording_file_path << std::endl;

	cleanup_resources();

	return 0;
}

/**
 * @brief Record audio samples
 */
int AudioRecording::record_samples(const float *left_channel, const float *right_channel, int num_samples)
{
	std::lock_guard<std::mutex> lock(recording_mutex);
	
	if (!recording_active)
	{
		return -1;
	}

	if (left_channel == nullptr || right_channel == nullptr || num_samples <= 0)
	{
		return -1;
	}

	// Safety check: validate buffer is allocated
	if (pcm_buffer_left == nullptr || pcm_buffer_right == nullptr)
	{
		std::cerr << "AudioRecording: PCM buffers not allocated" << std::endl;
		return -1;
	}

	// Check if we have room in buffer
	if (pcm_samples_accumulated + num_samples > pcm_buffer_size)
	{
		// Encode what we have first
		encode_and_write_mp3_chunk();

		// Verify the counter was actually reset
		if (pcm_samples_accumulated != 0)
		{
			std::cerr << "AudioRecording: WARNING - pcm_samples_accumulated not reset after encode, forcing reset" << std::endl;
			pcm_samples_accumulated = 0;
		}
	}

	// Additional safety: ensure we won't overflow even after the check above
	if (pcm_samples_accumulated + num_samples > pcm_buffer_size)
	{
		std::cerr << "AudioRecording: ERROR - Would overflow buffer (" 
				  << pcm_samples_accumulated << " + " << num_samples 
				  << " > " << pcm_buffer_size << "), dropping samples" << std::endl;
		return -1;
	}

	// Copy samples to PCM buffer
	std::memcpy(&pcm_buffer_left[pcm_samples_accumulated], left_channel, num_samples * sizeof(float));
	std::memcpy(&pcm_buffer_right[pcm_samples_accumulated], right_channel, num_samples * sizeof(float));

	pcm_samples_accumulated += num_samples;

	// Encode if buffer is full
	if (pcm_samples_accumulated >= pcm_buffer_size)
	{
		encode_and_write_mp3_chunk();
	}

	return 0;
}

/**
 * @brief Check if currently recording
 */
bool AudioRecording::is_recording() const
{
	return recording_active.load();
}

/**
 * @brief Get current recording file path
 */
std::string AudioRecording::get_recording_path() const
{
	return recording_file_path;
}

/**
 * @brief Set remote MP3 file path
 * @param path Remote file path
 */
void AudioRecording::set_remote_mp3_file_path(const std::string &path)
{
	remote_mp3_file_path = path;
}

/**
 * @brief Get remote MP3 file path
 * @return Remote file path string
 */
std::string AudioRecording::get_remote_mp3_file_path() const
{
	return remote_mp3_file_path;
}

/**
 * @brief Get current recording mode
 */
int AudioRecording::get_recording_mode() const
{
	return recording_mode;
}

/**
 * @brief Encode accumulated PCM samples and write to MP3 file
 */
void AudioRecording::encode_and_write_mp3_chunk()
{
	if (!recording_active || lame_flags == nullptr || pcm_samples_accumulated == 0 || !mp3_file.is_open())
	{
		return;
	}

	// Store the sample count before encoding
	int samples_to_encode = pcm_samples_accumulated;

	// CRITICAL: Reset the counter BEFORE encoding to prevent buffer overflow
	// if this function is called recursively or if there's any issue during encoding
	pcm_samples_accumulated = 0;

	int mp3_bytes = 0;

	if (recording_mode == _RECORDING_MODE_STEREO)
	{
		// Encode stereo - 2 separate channels
		mp3_bytes = lame_encode_buffer_ieee_float(
			lame_flags,
			pcm_buffer_left,
			pcm_buffer_right,
			samples_to_encode,
			mp3_buffer,
			mp3_buffer_size);
	}
	else if (recording_mode == _RECORDING_MODE_LEFT)
	{
		// Encode left channel only
		mp3_bytes = lame_encode_buffer_ieee_float(
			lame_flags,
			pcm_buffer_left,
			nullptr,
			samples_to_encode,
			mp3_buffer,
			mp3_buffer_size);
	}
	else if (recording_mode == _RECORDING_MODE_RIGHT)
	{
		// Encode right channel only
		mp3_bytes = lame_encode_buffer_ieee_float(
			lame_flags,
			pcm_buffer_right,
			nullptr,
			samples_to_encode,
			mp3_buffer,
			mp3_buffer_size);
	}
	else if (recording_mode == _RECORDING_MODE_MONO_MIX)
	{
		// ⚠️ NEW: Mix L+R to mono
		// Create temporary buffer with mixed samples
		float *mixed_buffer = new float[samples_to_encode];
		for (int i = 0; i < samples_to_encode; i++)
		{
			mixed_buffer[i] = (pcm_buffer_left[i] + pcm_buffer_right[i]) * 0.5f;
		}

		// Encode mixed mono
		mp3_bytes = lame_encode_buffer_ieee_float(
			lame_flags,
			mixed_buffer,
			nullptr,  // Mono
			samples_to_encode,
			mp3_buffer,
			mp3_buffer_size);

		delete[] mixed_buffer;
	}

	if (mp3_bytes < 0)
	{
		std::cerr << "AudioRecording: LAME encoding error: " << mp3_bytes << std::endl;
		// Counter already reset - samples are lost but we prevent crash
	}
	else if (mp3_bytes > 0)
	{
		mp3_file.write(reinterpret_cast<char*>(mp3_buffer), mp3_bytes);
	}
}

/**
 * @brief Flush remaining samples from encoder
 */
void AudioRecording::flush_mp3_encoder()
{
	if (pcm_samples_accumulated > 0)
	{
		encode_and_write_mp3_chunk();
	}
}

/**
 * @brief Clean up allocated resources
 */
void AudioRecording::cleanup_resources()
{
	if (pcm_buffer_left != nullptr)
	{
		delete[] pcm_buffer_left;
		pcm_buffer_left = nullptr;
	}
    
	if (pcm_buffer_right != nullptr)
	{
		delete[] pcm_buffer_right;
		pcm_buffer_right = nullptr;
	}
    
	if (mp3_buffer != nullptr)
	{
		delete[] mp3_buffer;
		mp3_buffer = nullptr;
	}
    
	pcm_samples_accumulated = 0;
}