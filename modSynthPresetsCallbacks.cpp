/**
*	@file		modSynthPresetsCallbacks.cpp
*	@author		Nahum Budin
*	@date		18-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*
*	@brief		Callbacks to handle setting Mod Synthesizer preset parameters
*	settings
*
*	History:\n
*				version	1.1	06-Feb-2021	Code refactoring and notaion
*				version 1.0	02-05-2020	First version
*
*/

#include <string>

#include "modSynth.h"
#include "modSynthPatch.h"

#include "./Instrument/instrumentFluidSynth.h"


// Temp holding preset parameters
mod_synth_preset_t synth_presets;

mod_synth_preset_t *get_synth_temp_preset_params() { return &synth_presets; }

int set_mixer_channel_synth_type_cb(int type, int mixer_chan, int channel) 
{
	//	ModSynth::get_instance()->set_midi_channel_synth(mixerdChan, type);
	synth_presets.chennels_presets[mixer_chan].synth_type = type;
	return 0;
}

int set_mixer_channel_midi_program_cb(int prog, int mixer_chan, int channel)
{
	//int bank = ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_channel_program(mixerdChan);		
	//ModSynth::get_instance()->get_fluid_synth()->select_fluid_synth_program(mixerdChan, bank, prog);
	synth_presets.chennels_presets[mixer_chan].fluid_prorgram_presets.midi_program = prog;
	return 0;
}

int set_mixer_channel_midi_program_name_cb(std::string prog, int mixer_chan, int channel)
{
	synth_presets.chennels_presets[mixer_chan].fluid_prorgram_presets.program_name = prog;
	return 0;
}

int set_mixer_channel_midi_bank_cb(int bank, int mixerChan, int channel)
{
	synth_presets.chennels_presets[mixerChan].fluid_prorgram_presets.midi_bank = bank;
	return 0;
}

int set_mixer_channel_midi_sound_font_cb(std::string sf, int mixer_chan, int channel)
{
	int sfid, res;
	string sf_path = std::string(_FLUID_DEFAULT_SOUNDFONT_DIR) + std::string("/") + sf;
	
	if (ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->
					loaded_soundfont_map.find(sf_path) == 
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->loaded_soundfont_map.end())
	{
		// soundfont not loadded yet - load it
		sfid = ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->
						load_fluid_synth_sound_font(sf_path, false);
		// TODO: update settings?
	}
	else
	{
		// soundfont already loaded
		sfid = ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->
						loaded_soundfont_map[sf_path];
	}
	
	synth_presets.chennels_presets[mixer_chan].fluid_prorgram_presets.midi_sound_font_file_name = sf_path;
	synth_presets.chennels_presets[mixer_chan].fluid_prorgram_presets.sound_font_id = sfid;	
	
	return 0;
}

int set_mixer_channel_adj_synth_patch_cb(std::string pf, int mixer_chan, int channel)
{
	string pfpath = std::string(_PATCHES_FILES_DEFAULT_DIR) + std::string("/") + pf;
	
	synth_presets.chennels_presets[mixer_chan].adj_synth_patch_name = pf;
	synth_presets.chennels_presets[mixer_chan].adj_synth_patch_path = pfpath;
	return 0;
}

int set_mixer_channel_level_cb(int level, int mixer_chan, int channel)
{
	synth_presets.chennels_presets[mixer_chan].mixer_presets.channel_level = level;
	return 0;
}

int set_mixer_channel_pan_cb(int pan, int mixer_chan, int channel)
{
	synth_presets.chennels_presets[mixer_chan].mixer_presets.channel_pan = pan;
	return 0;
}

int set_mixer_channel_send_cb(int send, int mixer_chan, int channel)
{
	synth_presets.chennels_presets[mixer_chan].mixer_presets.channel_send = send;
	return 0;
}

int set_fluid_synth_volume_callback(int vol, int dummy)
{
	get_synth_temp_preset_params()->fluid_volume = vol;
	return 0;
}

int set_fluid_synth_reverb_enable_callback(bool state, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.reverb_enabled = state;
	return 0;
}

int set_fluid_synth_reverb_room_size_callback(double size, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.reverb_room = size;
	return 0;
}

int set_fluid_synth_reverb_damp_callback(double damp, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.reverb_damp = damp;
	return 0;
}

int set_fluid_synth_reverb_width_callback(double width, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.reverb_width = width;
	return 0;
}

int set_fluid_synth_reverb_level_callback(double level, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.reverb_level = level;
	return 0;
}


int set_fluid_synth_chorus_enable_callback(bool state, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.chorus_enabled = state;
	return 0;
}


int set_fluid_synth_chorus_n_callback(int N, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.chorus_n = N;
	return 0;
}

int set_fluid_synth_chorus_level_callback(double level, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.chorus_level = level;
	return 0;
}

int set_fluid_synth_chorus_speed_callback(double speed, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.chorus_speed = speed;
	return 0;
}

int set_fluid_synth_chorus_depth_callback(double depth, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.chorus_depth = depth;
	return 0;
}

int set_fluid_synth_chorus_waveform_callback(int waveform, int dummy)
{
	get_synth_temp_preset_params()->fluid_preset.chorus_waveform = waveform;
	return 0;
}


int set_equilizer_band_31_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_31_level = level;
	return 0;
}

int set_equilizer_band_62_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_62_level = level;
	return 0;
}

int set_equilizer_band_125_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_125_level = level;
	return 0;
}

int set_equilizer_band_250_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_250_level = level;
	return 0;
}

int set_equilizer_band_500_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_500_level = level;
	return 0;
}

int set_equilizer_band_1k_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_1K_level = level;
	return 0;
}

int set_equilizer_band_2k_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_2K_level = level;
	return 0;
}

int set_equilizer_band_4k_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_4K_level = level;
	return 0;
}

int set_equilizer_band_8k_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_8K_level = level;
	return 0;
}

int set_equilizer_band_16k_level_cb(int level, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_16K_level = level;
	return 0;
}

int set_equilizer_preset_cb(int pre, int dummy)
{
	get_synth_temp_preset_params()->adj_preset.equalizer_preset = pre;
	return 0;
}




