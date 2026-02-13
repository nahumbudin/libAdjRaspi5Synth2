/**
* @file		instrumentHammondAnalogSynth.h
*	@author		Nahum Budin
*	@date		24-09-2025
*	@version	1.1	
*					1. Adding a pointer to a AdjSynth object
*					
*	@brief		Implements an Analog Synthesizer instrument.
*	
*	History:\n
*		version 1.0		12-8-2024: First version
*	
*/

#pragma once

#include "instrument.h"
#include "../Jack/jackConnections.h"


class InstrumentAnalogSynth : public Instrument
{
public:
	InstrumentAnalogSynth(AdjSynth *adj_synth);

	~InstrumentAnalogSynth();
	
	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);

	int set_default_settings_parameters(_settings_params_t *params, int prog = -1);

	/**
*   @brief  Save the active Analog Synth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
	int save_analog_synth_settings_file(string path);

	/**
*   @brief  Open settings parameters XML file and set it as the Analog Synth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
	int open_analog_synth_settings_file(string path);

	int collect_analog_synth_preset_parms(_settings_params_t *params);

	int save_analog_synth_preset_file(string path);

	int set_analog_synth_left_jack_output_connection(s_jack_connection_t connection);
	int set_analog_synth_right_jack_output_connection(s_jack_connection_t connection);

	s_jack_connection_t get_analog_synth_left_jack_output_connection();
	s_jack_connection_t get_analog_synth_right_jack_output_connection();

  private:
	/* Holds the stereo L and R Jack outputs connections */
	s_jack_connection_t jack_out_connection_left, jack_out_connection_right;
	  
	
};
