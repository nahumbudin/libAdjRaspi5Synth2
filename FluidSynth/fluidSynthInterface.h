/**
* @file		fluidSynthInterface.h
*	@author		Nahum Budin
*	@date		30-6-2024
*	@version	2.0
*					1. Update to FluidSynth Ver 2.35
*					2. Code refactoring
*
*	@brief		FluidSynth handling.
*	
*	@History
*		Ver 1.0 9-Oct-2019
*/

#pragma once

#include <stdint.h>
#include <string>
#include <map>

#include <fluidsynth.h>

#include "../LibAPI/libAdjRaspi5SynthAPI.h"

#include "../Settings/settings.h"

#include "../Instrument/instrumentFluidSynth.h"


//#include "synthUtils.h"

//#include "../LibAPI/libAdjHeartModSynth_1.h"

using namespace std;

#define _FLUID_OK 1
#define _FLUID_FAILED -1
#define _FLUID_BAD_PARAMS -2

typedef int fluid_res_t;

class InstrumentFluidSynth;


class FluidSynthInterface
{
  public:
	~FluidSynthInterface();

	static FluidSynthInterface *get_instance(InstrumentFluidSynth *fluid_instrument);
	static FluidSynthInterface *get_instance();

	fluid_settings_t *get_fluid_settings();
	fluid_synth_t *get_fluid_synth();

	fluid_res_t init_default_settings();

	fluid_res_t initialize_fluid_synthesizer();
	fluid_res_t deinitialize_fluid_synthesizer();

	// Send midi events
	fluid_res_t fluid_synth_note_on_event(int channel, int note, int velocity);
	fluid_res_t fluid_synth_note_off_event(int channel, int note);
	fluid_res_t fluid_synth_controller_event(uint8_t channel, uint8_t num, uint8_t val);
	fluid_res_t fluid_synth_pitch_bend_event(uint8_t channel, int pitch);
	fluid_res_t fluid_synth_program_change_event(uint8_t channel, int program);
	fluid_res_t fluid_synth_channel_pressure_event(int chan, int val);

	fluid_res_t set_fluid_synth_audio_driver(string driver);
	fluid_res_t set_fluid_synth_audio_jack_id(string id);
	fluid_res_t set_fluid_synth_audio_jack_autoconnect(bool auto_connect);
	fluid_res_t set_fluid_synth_audio_period_size(int size);
	fluid_res_t set_fluid_synth_audio_num_of_periods(int periods);

	fluid_res_t select_fluid_synth_midi_bank(string bank);

	fluid_res_t load_fluid_synth_sound_font(string path, bool reset_presets);

	fluid_res_t select_fluid_synth_program(int channel, unsigned int bank_num, int program);

	fluid_res_t set_fluid_synth_gain(double gain);

	fluid_res_t enable_fluid_synth_reverb();
	fluid_res_t disable_fluid_synth_reverb();

	fluid_res_t set_fluid_synth_reverb_room_size(double size);
	fluid_res_t set_fluid_synth_reverb_damp(double damp);
	fluid_res_t set_fluid_synth_reverb_width(double eidth);
	fluid_res_t set_fluid_synth_reverb_level(double level);
	fluid_res_t set_fluid_synth_reverb_params();

	fluid_res_t set_fluid_synth_reverb_enabled_value(bool en);
	fluid_res_t set_fluid_synth_reverb_room_size_value(double size);
	fluid_res_t set_fluid_synth_reverb_damp_value(double damp);
	fluid_res_t set_fluid_synth_reverb_width_value(double eidth);
	fluid_res_t set_fluid_synth_reverb_level_value(double level);

	fluid_res_t enable_fluid_synth_chorus();
	fluid_res_t disable_fluid_synth_chorus();

	fluid_res_t set_fluid_synth_chorus_N(int N);
	fluid_res_t set_fluid_synth_chorus_level(double level);
	fluid_res_t set_fluid_synth_chorus_speed(double speed);
	fluid_res_t set_fluid_synth_chorus_depth(double depth);
	fluid_res_t set_fluid_synth_chorus_waveform(int wform);
	fluid_res_t set_fluid_synth_chorus_params();

	fluid_res_t set_fluid_synth_chorus_enabled_value(bool en);
	fluid_res_t set_fluid_synth_chorus_N_value(int N);
	fluid_res_t set_fluid_synth_chorus_level_value(double level);
	fluid_res_t set_fluid_synth_chorus_speed_value(double speed);
	fluid_res_t set_fluid_synth_chorus_depth_value(double depth);
	fluid_res_t set_fluid_synth_chorus_waveform_value(int wform);

	fluid_res_t set_fluid_synth_soundfont_file(string path);
	int load_and_set_fluid_synth_soundfont(string path);

	fluid_res_t set_fluid_synth_program_select(int chan, unsigned int sfid,
											   unsigned int bank, unsigned int program);

	int get_fluid_synth_channel_sfont_id(int chan);
	int get_fluid_synth_channel_bank(int chan);
	int get_fluid_synth_channel_program(int chan);
	std::string get_fluid_synth_channel_preset_name(int chan);
	std::string get_fluid_synth_channel_preset_soundfont_name(int chan);
	std::string get_fluid_synth_soundfont_id_name(int sfid);

	int get_num_of_fluid_synth_loaded_soundFonts_files();
	int get_fluid_synth_sound_font_presets(int sfnum, vector<_soundfont_presets_data_t> *presets);
	int get_fluid_synth_loaded_sound_fonts_presets(vector<_soundfont_presets_data_t> *presets);

	int fluid_synth_event(int settingId, int value);

	void fluid_synth_panic_action();

	fluid_res_t set_callbacks();

	std::map<string, int> loaded_soundfont_map;

  private:
	FluidSynthInterface(InstrumentFluidSynth *fluid_inst);

	static FluidSynthInterface *fluid_synth_int_instance;
	_settings_params_t **active_settings_params;

	InstrumentFluidSynth *parent_instrument;

	fluid_settings_t *settings;
	fluid_synth_t *synth = NULL;
	fluid_midi_driver_t *mdriver;
	fluid_audio_driver_t *adriver;

	int sound_font_id;
	const string fluid_default_gm_sound_font_file = "/usr/share/sounds/sf2/FluidR3_GM.sf2";
};

string strip_settings_name_prefix(string name);

// FluidSynth settings callbacks
int set_fluid_synth_audio_driver_callback(string driver, int dummy = 0); // dummy: used for program number for AdjSynth settings
int set_fluid_synth_audio_jack_id_callback(string id, int dummy = 0);
int set_fluid_synth_audio_jack_autoconnect_callback(bool auto_connect, int dummy = 0);
int set_fluid_synth_audio_period_size_callback(int size, int dummy = 0);
int set_fluid_synth_audio_num_of_periods_callback(int periods, int dummy = 0);

int select_fluid_synth_midi_bank_callback(string bank, int dummy = 0);

int handle_midi_event_callback(void *data, fluid_midi_event_t *event);

int set_fluid_synth_gain_callback(double gain, int dummy);

int set_fluid_synth_reverb_active_callback(bool state, int dummy = 0);
int get_fluid_synth_reverb_active_callback();

int set_fluid_synth_chorus_active_callback(bool state, int dummy = 0);
int get_fluid_synth_chorus_active_callback();
