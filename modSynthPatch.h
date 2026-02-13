/**
*	@file		modSynthPatch.h
*	@author		Nahum Budin
*	@date		24-Sep-2025
*	@version	1.3
*					1. Code refactoring renaming preset to patches.
*
*	@brief		Used for saving and restoring a full setup of the synthesizer as a Patch
*
*	History:\n
*			version 1.2	17-Oct-2024:	Code refactoring and notaion
*			version	1.1	6-Feb_2021	Code refactoring and notaion
*			version 1.0	30-4-2020:	First version
*/

#pragma once

#include <string>

#include "./LibAPI/synthesizer.h"

typedef struct fluid_program_preset
{
	int channel_num;
	int midi_program;
	int midi_bank;
	int sound_font_id;
	std::string program_name;
	std::string midi_sound_font_file_name;
} fluid_program_preset_t;

typedef struct mixer_preset
{
	int channel_num;
	int channel_level;
	int channel_pan;
	int channel_send;
} mixer_preset_t;

typedef struct channel_preset {
	int channel_num;
	uint16_t synth_type;	
	fluid_program_preset_t fluid_prorgram_presets;	
	mixer_preset_t mixer_presets;	
	std::string adj_synth_patch_name;
	std::string adj_synth_patch_path;
} channel_preset_t;

typedef struct fluid_efects_preset {
	bool reverb_enabled;
	float reverb_room;
	float reverb_damp;
	float reverb_width;
	float reverb_level;
	bool chorus_enabled;
	int chorus_n;
	float chorus_level;
	float chorus_speed;
	float chorus_depth;
	int chorus_waveform;
} fluid_efects_preset_t;

typedef struct adj_efects_preset {
	bool reverb_enabled;
	int reverb_room_size;
	int reverb_damp;
	int reverb_wet;
	int reverb_dry;
	int reverb_width;
	int reverb_mode;

	bool reverb3m_enabled;
	int reverb3m_preset;

	int equalizer_31_level;
	int equalizer_62_level;
	int equalizer_125_level;
	int equalizer_250_level;
	int equalizer_500_level;
	int equalizer_1K_level;
	int equalizer_2K_level;
	int equalizer_4K_level;
	int equalizer_8K_level;
	int equalizer_16K_level;
	int equalizer_preset;
} adj_efects_preset_t;

typedef struct mod_synth_preset {
	std::string name;
	int master_volume;
	int fluid_volume;	
	channel_preset_t chennels_presets[_PROGRAM_16];
	fluid_efects_preset_t fluid_preset;
	adj_efects_preset_t adj_preset;
} mod_synth_preset_t;


mod_synth_preset_t *get_synth_temp_preset_params();



//Callbacks to set preset settings

int set_audio_driver_type_cb(int drive, int dummy);
int set_audio_block_size_cb(int size, int dummy);
int set_audio_sample_rate_cb(int rate, int dummy);

// Note - we are using the block callback option to target each 
//        midi-mixer channel
int set_mixer_channel_synth_type_cb(int type, int mixer_chan, int channel);
int set_mixer_channel_midi_program_cb(int prog, int mixer_chan, int channel);
int set_mixer_channel_midi_program_name_cb(std::string prog, int mixer_chan, int channel);
int set_mixer_channel_midi_bank_cb(int prog, int mixer_chan, int channel);
int set_mixer_channel_midi_sound_font_cb(std::string sf, int mixer_chan, int channel);
int set_mixer_channel_adj_synth_patch_cb(std::string pf, int mixer_chan, int channel);
int set_mixer_channel_level_cb(int level, int mixer_chan, int channel);
int set_mixer_channel_pan_cb(int pan, int mixer_chan, int channel);
int set_mixer_channel_send_cb(int send, int mixer_chan, int channel);


int set_fluid_synth_volume_callback(int vol, int dummy = 0);

int set_fluid_synth_reverb_enable_callback(bool state, int dummy = 0);
int set_fluid_synth_reverb_room_size_callback(double size, int dummy = 0);
int set_fluid_synth_reverb_damp_callback(double damp, int dummy = 0);
int set_fluid_synth_reverb_width_callback(double width, int dummy = 0);
int set_fluid_synth_reverb_level_callback(double level, int dummy = 0);

int set_fluid_synth_chorus_enable_callback(bool state, int dummy = 0);
int set_fluid_synth_chorus_n_callback(int N, int dummy = 0);
int set_fluid_synth_chorus_level_callback(double level, int dummy = 0);
int set_fluid_synth_chorus_speed_callback(double speed, int dummy = 0);
int set_fluid_synth_chorus_depth_callback(double depth, int dummy = 0);
int set_fluid_synth_chorus_waveform_callback(int waveform, int dummy = 0);

int set_equilizer_band_31_level_cb(int level, int dummy = 0);
int set_equilizer_band_62_level_cb(int level, int dummy = 0);
int set_equilizer_band_125_level_cb(int level, int dummy = 0);
int set_equilizer_band_250_level_cb(int level, int dummy = 0);
int set_equilizer_band_500_level_cb(int level, int dummy = 0);
int set_equilizer_band_1k_level_cb(int level, int dummy = 0);
int set_equilizer_band_2k_level_cb(int level, int dummy = 0);
int set_equilizer_band_4k_level_cb(int level, int dummy = 0);
int set_equilizer_band_8k_level_cb(int level, int dummy = 0);
int set_equilizer_band_16k_level_cb(int level, int dummy = 0);
int set_equilizer_preset_cb(int pre, int dummy = 0);

class ModSynthPatches 
{
public:
	
	static void init();
	
	static int generate_summary(mod_synth_preset_t *preset = NULL, std::string *summary = NULL);
	
	static int set_synth_settings(mod_synth_preset_t *preset = NULL);
	
	static mod_synth_preset_t *get_synth_preset_params(int presetNum);
	
	static std::string *get_synth_preset_summary_text(int presetNum);
	
	static void copy_presets(mod_synth_preset_t *src, mod_synth_preset_t *dest);
	
	static void set_active_preset(int preset);
	static int get_active_preset();
	
private:
	static mod_synth_preset_t synth_presets[_NUM_OF_PRESETS];
	
	static std::string preset_summary_text[_NUM_OF_PRESETS];
	
	static int active_preset;
};
