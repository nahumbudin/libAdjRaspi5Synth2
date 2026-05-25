/**
* @file		instrumentAnalogSynthPreset.h
*	@author		Nahum Budin
*	@date		22-May-2026
*	@version	1.0	
*					1. Initial release
*					
*	@brief		- The InstrumentAnalogSynthPreset implements an Analog Synthesizer instrument that uses a prest 
*	              file and has no GUI controls, other than selecting and loading the preset file, generated 
*	              using the full version of the Analog Synthesizer.
*				- This is usueful for selecting fix presets like programs in a General MIDI sound module.
*				- Each Preset Module is assigned with a unique MIDI program outside the 0-15 range,
*				  like the other instruments, and may be mapped to a MIDI channel (0-15) using the 
*				  MIDI Mapper instrument.
*				- Unlike other instruments, the preset module instrument may have multiple instances,
*				  each assigned to a different MIDI program, and each with a different preset file.
*				- All the instances of the preset module instrument are initiated in advance during the 
*				  initialization of the AdjSynth.
*				- The InstrumentAnalogSynthPresetManager class is responsible for managing the preset module
*				  instruments, including allocating and deallocating them.
*				
*	History:\n
*		
*/

#pragma once

#include "instrument.h"
#include "../Jack/jackConnections.h"

class InstrumentAnalogSynthPreset : public Instrument
{
public:
	InstrumentAnalogSynthPreset(AdjSynth *adj_synth, int midi_program = -1);

	~InstrumentAnalogSynthPreset();
	
	int get_midi_program();
	
	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);
	
	int init_analog_synth_default_settings(_settings_params_t *params);
	
	//int set_instrument_settings(_settings_params_t *preset);

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
	
	int midi_program;
};

class InstrumentAnalogSynthPresetManager
{
public:
	
	~InstrumentAnalogSynthPresetManager();
	
	static InstrumentAnalogSynthPresetManager* get_instance(AdjSynth *adj_synth);
	
	int get_preset_instrument_midi_program_num(int inst_num);
	
	InstrumentAnalogSynthPreset* get_preset_instrument(int inst_num);
	
private:
	
	InstrumentAnalogSynthPresetManager(AdjSynth *adj_synth);
	
	static InstrumentAnalogSynthPresetManager* instrument_analog_synth_preset_manager_instance;
	
	InstrumentAnalogSynthPreset* preset_instruments[_MAX_NUM_OF_ANALOG_PRESET_INSTRUMENTS];
};
