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

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <fstream>

#include "utils.h"

#include "../libAdjRaspi5Synth2.h"
#include "../commonDefs.h"
#include "../Audio/audioCommons.h"

using namespace std;

// For the random generator
prng_t prng_state = 0x1234;

/**
*	@brief Returns a logarithmic scaled float number
*	@parma float min:	min value of output range
*	@parma float max:	max value of output range
*	@parma float base:	log base (e.g. base = 10 -> 10^val)
*	@parma int val:		input number in the range 0 to 100
*	@return: float		a log scaled number in the range of min to max
*/
float Utils::calc_log_scale_100_float(float min, float max, float base, int val)
{
	int in = val;
	float logVal;

	if (in < 0)
	{
		in = 0;
	}
	else if (in > 100)
	{
		in = 100;
	}

	if (val > 0)
	{
		logVal = (float)(min + (max - min) * pow((double)base, (double)in / 50.0) / 100.0);
	}
	else
	{
		logVal = min;
	}

	return logVal;
}

/**
*	@brief Returns a logarithmic scaled integer number
*	@parma int min:		min value of output range
*	@parma int max:		max value of output range
*	@parma int base:	log base (e.g. base = 10 -> 10^val)
*	@parma int val:		input number in the range 0 to 100
*	@return: float		a log scaled number in the range of min to max
*/
int Utils::calc_log_scale_100_int(int min, int max, int base, int val)
{
	int in = val;

	if (in < 0)
	{
		in = 0;
	}
	else if (in > 100)
	{
		in = 100;
	}

	return (int)(min + (max - min) * pow((double)base, (double)in / 50.0) / 100.0);
}

/**
*	@brief	Runs a system command using system("cmd") and returns redirested output
*	@param	cmd command
*	@return redirected command captured output
*/
std::string Utils::execute_system_command_getstd_out(std::string cmd)
{
	std::string data;
	FILE* stream;
	const int max_buffer = 2048;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");
	if (stream)
	{
		while (!feof(stream))
		{
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		}

		pclose(stream);
	}

	return data;


	return data;
}

pid_t Utils::get_process_id(char* process_name)
{
	char buf[512];
	char command[128];
	pid_t pid;
	
	sprintf(command, "pidof - s %s", process_name);
	
	FILE *cmd_pipe = popen(command, "r");

	fgets(buf, 512, cmd_pipe);
	pid = strtoul(buf, NULL, 10);

	pclose(cmd_pipe);
	
	return pid;
}

/**
*	@brief	returns true if the given audio driver type value is a valid type
*	@param	int driver driver-type
*	@return true if a valid audio-driver value
*/
bool is_valid_audio_driver(int driver)
{
	return (driver == _AUDIO_JACK) || (driver == _AUDIO_ALSA);
}

/**
*	@brief	returns true if the given sample rate value is a valid sample-rate
*	@param	int samp_rate	sample-rate
*	@return true if a valid sample-rate value
*/
bool is_valid_sample_rate(int samp_rate)
{
	return (samp_rate == _SAMPLE_RATE_44) || (samp_rate == _SAMPLE_RATE_48);
}

/**
*	@brief	returns true if the given audio block size value is a valid size
*	@param	int block_size	bloc-size
*	@return true if a valid block-size value
*/
bool is_valid_audio_block_size(int size)
{
	return ((size == _AUDIO_BLOCK_SIZE_256) ||
		(size == _AUDIO_BLOCK_SIZE_512) ||
		(size == _AUDIO_BLOCK_SIZE_1024)) &&
		(size <= _AUDIO_MAX_BUF_SIZE);
}

/**
*	@brief	returns true if the given midi mapping mode value is a valid mode
*	@param	mode	mapping mode
*	@return true if a valid block-size value
*/
bool is_valid_midi_mapping_mode(int mode)
{
	return ((mode == _MIDI_MAPPING_MODE_SKETCH) ||
		(mode == _MIDI_MAPPING_MODE_MAPPING));
}

/**
*	@brief	returns true if the given PAD harmonies envelope shape is a valid shape
*	@param	shap	envelope shape
*	@return true if a valid envelope shape
*/
bool is_valid_pad_harmonies_envelope_shape(int shape)
{
	return ((shape == _PAD_SHAPE_RECTANGULAR) ||
		(shape == _PAD_SHAPE_GAUSSIAN) ||
		(shape == _PAD_SHAPE_DOUBLE_EXP));
}

/**
*	@brief	returns true if the given PAD harmonies envelope shape cutoff is a valid cutoff
*	@param	shap	envelope shape cutoof
*	@return true if a valid envelope shape
*/
bool is_valid_pad_harmonies_envelope_shape_cutoff(int shcutof)
{
	return ((shcutof == _PAD_SHAPE_CUTOFF_FULL) ||
		(shcutof == _PAD_SHAPE_CUTOFF_UPPER) ||
		(shcutof == _PAD_SHAPE_CUTOFF_LOWER));
}

/**
*	@brief	returns the period tim [us] based on the sample-rate and block-size
*	@param	samp_rat	sample-rate
*	@param	block_size	block-size
*	@return period-time [us]
*/
unsigned long calc_period_time_us(int samp_rate, int block_size)
{
	if (samp_rate == 0)
	{
		fprintf(stderr, "CommonDefs: sample rate value is zero.");
		sleep(3);
		exit(1);
	}

	return ((unsigned long)block_size * 1000000) / samp_rate + 0.5;

}

long get_current_time_ms()
{
	struct timeval tp;

	gettimeofday(&tp, NULL);

	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}


int write_text_file(string full_path, string file_data)
{
	if ((full_path == "") || (file_data == ""))
		return -1;

	ofstream patch_file; //(full_path.c_str(), std::ofstream::binary);
	patch_file.open(full_path, ofstream::out);
	int err = errno;
	if (!patch_file.is_open())
	{
		fprintf(stderr, "Can't open %s XML patch file for write!\n", full_path.c_str());
		return -2;
	}

	patch_file << file_data;

	patch_file.close();

	return 0;
}

int read_text_file(string full_path, vector<char> *file_data)
{
	if (full_path == "")
		return -1;

	std::ifstream in_file;

	in_file.open(full_path, ofstream::in);

	// get size of file
	in_file.seekg(0, in_file.end);
	long size = in_file.tellg();
	in_file.seekg(0);

	// allocate memory for file content
	char *buffer = new char[size];

	in_file.read(buffer, size);

	for (int i = 0; i < size; i++)
	{
		file_data->push_back(buffer[i]);
	}

	// release dynamically-allocated memory
	delete[] buffer;
	in_file.close();	
	
	return 0;
}


