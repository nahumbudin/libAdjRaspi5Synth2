/**
 *	@file		modSynth.h
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.1
 *					1. Code refactoring rename patches to preset parameters.
 *					2. Adding support in both old and new MIDI program objects.
 *					3. Definning a global settings handling mutex
 *					4. Adding I2C interface support
 *
 *	@brief		This is the main modular synthesizer libraray object.
 *
 *	History:\n
 *
 *	version 1.0		11-May-2024: First version
 *
 *	Based on libAdjHeartModSynth_2 library
*
*/

#pragma once

#include <cstddef>
#include <cstdint>

#include "./AdjSynth/adjSynth.h"

#include "./AdjSynth/adjSynthPolyphony.h"

#include "./ALSA/alsaBtClientOutput.h"
#include "./ALSA/controlBoxClientAlsaOutput.h"
#include "./ALSA/controlBoxExtMidiInClientAlsaOutput.h"
#include "./ALSA/alsaMidiSystemControl.h"

#include "./Jack/jackConnections.h"

#include "./MIDI/midiExtInterface.h"

#include "./Settings/settings.h"

#include "./Patches/patches.h"

#include "./I2C/i2cInterface.h"

//#include "./Instrument/instrumentFluidSynth.h"
//#include "./Instrument/instrumentHammondOrgan.h"

//#include "./AdjSynth/adjSynth.h"

enum mod_synth_type
{
	SYNTH_TYPE_FLUID,
	SYNTH_TYPE_ADDITIVE,
	SYNTH_TYPE_KARPLUS, // Karplus Strong String
	SYNTH_TYPE_MSO, // Modified Sinus 
	SYNTH_TYPE_PAD				// PAD Synth http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm#c_implementation	
};

class InstrumentsManager;
class InstrumentFluidSynth;
class InstrumentHammondOrgan;
class InstrumentAnalogSynth;
class InstrumentMidiPlayer;
class InstrumentMidiMapper;
class InstrumentControlBoxEventsHandler;

class AdjSynth;

void callback_audio_update_cycle_start_tasks_wrapper(int param);

class ModSynth
{
public:
	~ModSynth();
	static ModSynth *get_instance();

	int init();
	
	void on_exit();	

	void synth_panic_action();
	
	void start_cheack_cpu_utilization_thread();
	void stop_cheack_cpu_utilization_thread();
	int mod_synth_get_cpu_utilization();
	
	int init_midi_ext_interface(int ser_port_num);
	int deinit_midi_ext_interface();
	
	int collect_mod_synth_preset_parms(_settings_params_t *params);
	int set_default_preset_parameters(_settings_params_t *params);


	AlsaBtClientOutput *get_bt_alsa_out();

	InstrumentFluidSynth *get_fluid_synth();
	InstrumentAnalogSynth *get_analog_synth();
	InstrumentHammondOrgan *get_hammond_organ();
	InstrumentMidiPlayer *get_midi_player();
	InstrumentMidiMapper *get_midi_mapper();
	
	int set_adj_synth_default_settings(_settings_params_t *params);
	int set_default_general_settings_parameters(_settings_params_t *params);
	int set_default_settings_parameters_audio(_settings_params_t *params);
	
	
	int save_adj_synth_patch_file(string path, Settings *settings, _settings_params_t *params);
	int open_adj_synth_patch_file(string path, Settings *settings, _settings_params_t *params, int channel);
	int save_adj_synth_patch_file(string path);
	int open_adj_synth_patch_file(string path, int channel);
	
	int save_adj_synth_settings_file(string path, Settings *settings, _settings_params_t *params);
	int open_adj_synth_settings_file(string path, Settings *settings, _settings_params_t *params);
	int save_mod_synth_general_settings_file(string path);
	int open_mod_synth_general_settings_file(string path);
	
	void set_master_volume(int vol);
	int get_master_volume();

	void set_fluid_synth_volume(int vol);
	int get_fluid_synth_volume();
	
	int set_sample_rate(int samp_rate, bool restart_audio);
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size, bool restart_audio);
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	int set_audio_driver_type(int driver, bool restart_audio);
	int set_audio_driver_type(int driver);
	int get_audio_driver_type();
	
	int start_audio();
	int stop_audio();
	
	int get_midi_channel_synth(int chan);
	void set_midi_channel_synth(int chan, int synth);

	void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off(uint8_t channel, uint8_t note);
	void change_program(uint8_t channel, uint8_t program);
	void channel_pressure(uint8_t channel, uint8_t val);
	void controller_event(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend(uint8_t channel, int pitch);
	
	void update_tasks(int voc);	

	AlsaMidiSysControl *alsa_midi_system_control;

	JackConnections *jack_connections;

	InstrumentControlBoxEventsHandler *control_box_events_handler;

	InstrumentsManager *instruments_manager;
	
	InstrumentMidiPlayer *midi_player;

	InstrumentMidiMapper *midi_mapper;

	PatchsHandler *patches_handler;
	
	AdjSynth *adj_synth;
	
	Settings *general_settings_manager;
	Settings *fluid_synth_settings_manager;
	
	static int cpu_utilization;

	static std::mutex settings_handler_mutex;

	_settings_params_t preset_temp;
	std::string preset_temp_summary_text;

  private:
	
	ModSynth();

	static ModSynth *mod_synth;

	AlsaBtClientOutput *bt_alsa_out;
	ControlBoxClientAlsaOutput *control_box_alsa_out;
	ControlBoxExtMidiInClientAlsaOutput *control_box_ext_midi_in_alsa_out;

	I2Cinterface *i2c_interface_main_control;
	I2Cinterface *i2c_interface_control_board;
	;

	MidiExtInterface *midi_ext_interface;
	
	InstrumentFluidSynth *fluid_synth;
	InstrumentHammondOrgan *hammond_organ;
	InstrumentAnalogSynth *analog_synth;
	
	/* The modular synthesizer master volume level (0-100) */
	int master_volume;
	/* The fluid synthesizer  volume level (0-100) */
	int fluid_synth_volume;

	pthread_t cheack_cpu_utilization_thread_id;	
	
	/* Holds a bit mask for each MIDI channel 0-15, sketc 1-3 (16-18) indicating assigned synthesizer
	   Channel 0 is the main edditing channel */
	uint16_t midi_channel_synth[_SKETCH_PROGRAM_3 + 1];
	
	// ModSynth default settings directory
	string mod_synth_general_settings_file_path_name;
	
	
	/* Holds the general (global) settings parameters */
	_settings_params_t active_general_synth_settings_params;
	
	/* Holds the FluidSynth settings parameters */
	_settings_params_t active_fluid_synth_settings_params;
	
	int audio_driver, sample_rate, audio_block_size;

};

// Thread checking cpu utilization
void *cheack_cpu_utilization_thread(void *arg);

