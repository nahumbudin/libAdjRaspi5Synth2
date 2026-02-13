/**
* @file		instrumentFluidSynth.h
*	@author		Nahum Budin
*	@date		24-06-2024
*	@version	1.0	Initial release
*					
*	@brief		Implements a FluidSynth SoundFont synthesizer.
*	
*	History:\n
*	
*/

#pragma once

#include "fluidsynth.h"

#include "instrument.h"
#include "../FluidSynth/fluidSynthInterface.h"
#include "../Settings/settings.h"

using namespace std;

class FluidSynthInterface;

typedef struct fluid_channel_preset
{
	int channel_num;
	int midi_program;
	int midi_bank;
	int soundfont_id;
	string program_name;
	string midi_soundfont_file_name;
} fluid_channel_preset_t;

class InstrumentFluidSynth : public Instrument
{
public:
	InstrumentFluidSynth();
	
	~InstrumentFluidSynth();

	FluidSynthInterface *get_fluid_synth_interface();

	void register_callback_update_preset_text(func_ptr_void_string_t ptr);
	void activate_ui_update_preset_text_callback(string text);

	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);

	int set_fluid_synth_program_select(int chan, unsigned int sfid,
									   unsigned int bank, unsigned int program);

	void set_soundfont(string path);

	void load_and_set_fluid_synth_soundfont(string path);

	int select_program(int chan, unsigned int sfid,
					    unsigned int bank, unsigned int program);

	int set_default_settings_parameters(_settings_params_t *params, int prog = -1);

	/**
*   @brief  Save the active FluidSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
	int save_fluid_synth_settings_file(string path);

	/**
*   @brief  Open settings parameters XML file and set it as the FluidSynth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
	int open_fluid_synth_settings_file(string path);

	int get_fluid_synth_channel_bank(int chan);

	int collect_fluid_synth_preset_parms(_settings_params_t *params);

	int save_fluid_synth_preset_file(string path);

	int open_fluid_synth_preset_file(
		string path = "",
		_settings_params_t *preset = NULL,
		std::string *summary = NULL);

	int collect_fluid_synth_prest_channels_data(_settings_params_t *params,
												fluid_channel_preset_t *channels_presets);

	int generate_fluid_synth_prest_summary(fluid_channel_preset_t *preset, string *summary);

	int load_fluid_synth_preset_soundfonts(fluid_channel_preset_t *channels_presets);

	int set_fluid_synth_settings(_settings_params_t *preset,
								 fluid_channel_preset_t *channels_presets);

	void set_active_preset(int preset_num);

	int events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, float val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program = -1);
	int events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program = -1);

	int active_preset;
	_settings_params_t presets[_NUM_OF_FLUID_PRESETS];
	string presets_summary_str[_NUM_OF_FLUID_PRESETS] = {""};

	fluid_channel_preset_t channels_presets[_NUM_OF_FLUID_PRESETS][16];

  private:
	FluidSynthInterface *fluid_synth_int_instance;

	func_ptr_void_string_t callback_ptr_update_fluid_synth_preset_text;
	
	void set_volume(int vol);
	void set_reverb_room_size(int val);
	void set_reverb_damp(int val);
	void set_reverb_width(int val);
	void set_reverb_level(int val);

	void set_chorus_number(int val);
	void set_chorus_level(int val);
	void set_chorus_speed(int val);
	void set_chorus_depth(int val);
	void set_chorus_waveform(int val);

	void enable_reverb();
	void disable_reverb();

	void enable_chorus();
	void disable_chorus();
};

