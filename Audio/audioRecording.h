/**
 * @file		audioRecording.h
 * @author		Nahum Budin (With Github Copilot)
 * @date		16-Jan-2026
 * @version	1.0
 *
 * @brief		MP3 audio recording using LAME encoder
 *              Records mixed audio output to MP3 file
 */

#pragma once

#include <lame/lame.h>
#include <string>
#include <fstream>
#include <atomic>

class AudioRecording
{
public:
	/**
	 * @brief Constructor
	 * @param buffer_size Size of PCM buffer (typically audio block size)
	 */
	AudioRecording(int buffer_size = 512);
    
	/**
	 * @brief Destructor - ensures recording is stopped and resources freed
	 */
	~AudioRecording();
    
	/**
	 * @brief Start MP3 recording
	 * @param mode Recording mode: _RECORDING_MODE_LEFT, _RECORDING_MODE_RIGHT, _RECORDING_MODE_STEREO
	 * @param path Output file path (e.g., "/tmp/recording.mp3")
	 * @param bitrate MP3 bitrate in kbps (default 192)
	 * @param sample_rate Sample rate in Hz (default 44100)
	 * @return 0 if success, negative error code otherwise
	 *         -1: already recording
	 *         -2: file open failed
	 *         -3: invalid mode
	 *         -4: LAME init failed
	 *         -5: LAME parameter setup failed
	 */
	int start_recording(int mode, const std::string &path, int bitrate = 192, int sample_rate = 44100);
    
	/**
	 * @brief Stop MP3 recording
	 * @return 0 if success, -1 if not recording
	 */
	int stop_recording();
    
	/**
	 * @brief Record audio samples (call from audio processing thread)
	 * @param left_channel Pointer to left channel samples
	 * @param right_channel Pointer to right channel samples
	 * @param num_samples Number of samples per channel
	 * @return 0 if success, -1 if not recording
	 */
	int record_samples(const float *left_channel, const float *right_channel, int num_samples);
    
	/**
	 * @brief Check if currently recording
	 * @return true if recording, false otherwise
	 */
	bool is_recording() const;
    
	/**
	 * @brief Get current recording file path
	 * @return File path string, empty if not recording
	 */
	std::string get_recording_path() const;
    
	/**
	 * @brief Get current recording mode
	 * @return Recording mode constant
	 */
	int get_recording_mode() const;

private:
	// Internal methods
	void encode_and_write_mp3_chunk();
	void flush_mp3_encoder();
	void cleanup_resources();
    
	// Recording state
	std::atomic<bool> recording_active;
	int recording_mode;
	std::string recording_file_path;
    
	// LAME encoder
	lame_global_flags *lame_flags;
	int lame_sample_rate;
    
	// File output
	std::ofstream mp3_file;
    
	// PCM buffers
	float *pcm_buffer_left;
	float *pcm_buffer_right;
	int pcm_buffer_size;
	int pcm_samples_accumulated;
    
	// MP3 output buffer
	unsigned char *mp3_buffer;
	int mp3_buffer_size;
};
