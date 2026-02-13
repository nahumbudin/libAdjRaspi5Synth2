/**
* @file		JackConfigurationFile.h
*	@author		Nahum Budin
*	@date		2-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*					
*
*	@brief		Jack Audio configuration handling.
*	
*	History:13-Feb-2021	1.1
*					1. Code refactoring and notaion.
*					2. Move from GUI level to lib level	
*		
*			13-Apr-2020	1.0	1st version
*	
*	Example of file:
*	
*	#This is the AdjModSynth JACK Audio configuration file.
*	#Last update: April 13 2020.
*
*	#Do not change the header lines that start with !
*	
*	#Coments lines starts with # and are ignored.
*	
*	!Processes
*	#Current version supports up to 5 processes
*	#JMETERS is a JACK Audio level meters application.
*	jmeters -t din -c 2 L R
*	!Processes_end
*	
*	!Audio
*	#Set the QjackCtl audio connectios using the jack_connect command
*	adjheart_fluid_synth:left jmeters:in-1
*	adjheart_fluid_synth:right jmeters:in-2
*	AdjHeartSynth_out:AdjHeartSynthL_out jmeters:in-1
*	AdjHeartSynth_out:AdjHeartSynthR_out jmeters:in-2
*	adjheart_fluid_synth:left AdjHeartSynth_in:AdjHeartSynthL_in
*	adjheart_fluid_synth:right AdjHeartSynth_in:AdjHeartSynthR_in
*	AdjHeartSynth_out:AdjHeartSynthL_out AdjHeartSynth_in:AdjHeartSynthL_in
*	AdjHeartSynth_out:AdjHeartSynthR_out AdjHeartSynth_in:AdjHeartSynthR_in
*	
*	!Audio_end
*	
*	!Alsa_midi
*	#Set the ALSA MIDI connections using the aconnect command
*	#Novation Impulse
*	24:0 130:0
*	#USB MIDI converter HC35
*	20:0 130:0
*	!Alsa_midi_end
*
*
*/

#include <string>
#include <fstream>      // std::ifstream
#include <vector>
#include <unistd.h>
#include <stdlib.h>

#include "JackConfigurationFile.h"
#include "jackAudioClients.h"

JackConfigurationFile::JackConfigurationFile(std::string path)
{
	file_path = path;
	file_is_read = false;
}

int JackConfigurationFile::read_configuration_file()
{
	std::ifstream config_file;
	char c;
	
	if (file_path == "")
	{
		fprintf(stderr, "Jack configuration file Error - missing jack configuration file path");
		return -1;
	}
	else
	{
		config_file.open(file_path, std::ifstream::in);
		int err = errno;
		if (!config_file.is_open())
		{
			fprintf(stderr, "Jack configuration file Error - Can't open %s Jack config file for read!\n", file_path.c_str());
			return -2;
		}

		while (config_file.get(c)) 
		{
			file_data += c;			
		}
		config_file.close();	
		file_is_read = true;
		
		return 0;
	}
}

int JackConfigurationFile::get_config_text_lines()
{
	std::string text_line;
	int index;
	char c = 0;
	
	if (!file_is_read || file_data.size() == 0)
	{
		fprintf(stderr, "Jack configuration file Error - file not read yet or empty");
		return -1;
	}
	else
	{
		index = 0;
	
		while (index < file_data.size()) 
		{
			// Start collecting configuration file text lines
			c = file_data.c_str()[index++];
			while (c != 10) // <LF>
			{
				if (c != 13) 
				{ 
					// not a <CR>	
					text_line += c;
				}
				c = file_data.c_str()[index++];
			}
			// EOL
			if ((*(text_line.begin()) != _COMMENT_START) && (text_line.size() > 0))
			{
				// not a commented text line - add it
				config_text_lines.push_back(text_line);
			}
			
			text_line.clear();
		}
		
		return 0;
	}
}

int JackConfigurationFile::get_section_text_lines(std::vector<std::string> *lines, 
	std::string start_header, 
	std::string end_header)
{
	bool section_start = false;
	
	if ((lines == NULL) || (start_header == "") || (end_header == ""))
	{
		return -1;
	}
	
	for (std::string line : config_text_lines)
	{
		// go over al lines
		if (!section_start)
		{
			// look for requested section header line
			if (line.compare(start_header) == 0)
			{
				// start segment header found
				section_start = true;
			}
		}
		else
		{
			// section line collection is in progress
			if (line.compare(end_header) != 0)
			{
				// not end of section yet - add line
				lines->push_back(line);
			}
			else
			{
				// end of section - done.
				section_start = false;
				break;
			}
		}		
	}
		
	return 0;
}

int JackConfigurationFile::execute_configuration()
{	
	
	int res;
	std::vector<std::string> config_lines;
	int i;
	
	// Read the configuration file text
	res = read_configuration_file();
	if (res != 0)
	{
		// error reading file
		return res;
	}
	
	// get the configuration file non commented text lines
	res = get_config_text_lines();
	if (res != 0)
	{
		return res;
	}	
	
	// Get the "Processes" section lines
	res = get_section_text_lines(&config_lines, 
		_PROCESSES_SECTION_SRART_HEADER, 
		_PROCESSES_SECTION_END_HEADER);
	if (res != 0)
	{
		return res;
	}
	// Execute the processes comand lines: "start(process)
	for (i = 0; ((i < _MAX_NUM_OF_PROCESSES) && (i < config_lines.size())); i++)
	{
		system((config_lines.at(i) + " &").c_str()); // run in background; do not wait
		// Let it role...
		usleep(500000);
	}	
	
	config_lines.clear();
	// Look for the audio connections command lines
	res = get_section_text_lines(&config_lines, 
		_AUDIO_SECTION_SRART_HEADER, 
		_AUDIO_SECTION_END_HEADER);
	if (res != 0)
	{
		return res;
	}
	// Execute the jack_connect commands: "system(jack_connect param)"
	for (i = 0; i < config_lines.size(); i++)
	{
		system(("jack_connect " + config_lines.at(i)).c_str());
	}
	
	if (get_jack_auto_connect_midi_state())
	{
		config_lines.clear();
		// Look for the ALSA MIDI connections command lines
	
		res = get_section_text_lines(&config_lines, 
			_ALSA_MIDI_SECTION_SRART_HEADER, 
			_ALSA_MIDI_SECTION_END_HEADER);
		if (res != 0)
		{
			return res;
		}
		// Execute the aconnect ALSA MIDI connection command lines: "system("connect param")
		for (i = 0; i < config_lines.size(); i++)
		{
			system(("aconnect " + config_lines.at(i)).c_str());
		}
	}
	
	return 0;
}
