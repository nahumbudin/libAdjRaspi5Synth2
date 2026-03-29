/**
*	@file		modSynthPatch.cpp
*	@author		Nahum Budin
*	@date		24-Sep-2025
*	@version	1.3
*					1. Code refactoring renaming preset to patche (presets are used for settings).
*
*	@brief		Used for saving and restoring a full setup of the synthesizer as a patch
*
*	History:\n
*			version 1.2	17-Oct-2024:	Code refactoring and notaion
*			version	1.1	6-Feb_2021	Code refactoring and notaion
*			version 1.0	30-4-2020:	First version
*/

#include "modSynthPatch.h"
#include "./Instrument/instrumentFluidSynth.h"

#include "modSynth.h"

mod_synth_preset_t ModSynthPatches::synth_presets[_NUM_OF_PRESETS];
string ModSynthPatches::preset_summary_text[_NUM_OF_PRESETS] = { "" };
int ModSynthPatches::active_preset = 0;


/**
*   @brief  Initilizes the presets data..
*   @param	none
*   @return void
*/
void ModSynthPatches::init()
{
	for (int i = 0; i < _NUM_OF_PRESETS; i++)
	{
		for (int c = 0; c < 16; c++)
		{
			synth_presets[i].chennels_presets[c].synth_type = _MIDI_CHAN_ASSIGNED_SYNTH_NONE;
		}
	}
}

/**
*   @brief  Generates a summary text string of the preset fluid programs and/or AdjSynth patches names.
*   @param	preset	a pointer to a mod_synth_preset_t presets object
*   @param	summary	a pointer to a std::string that will hold the summary text
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if either params is NULL
*/
int ModSynthPatches::generate_summary(mod_synth_preset_t *preset, std::string *summary)
{
	return_val_if_true(preset == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(summary == NULL, _SETTINGS_BAD_PARAMETERS);
	
	std::string sum_text = "";
	
	for (int chan = 0; chan < 16; chan++)
	{
		if (preset->chennels_presets[chan].synth_type == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
		{
			if (chan != 0)
			{
				sum_text += "\n";
			}
			sum_text += "Ch" + std::to_string(chan + 1) + std::string(" FS: ") + preset->chennels_presets[chan].fluid_prorgram_presets.program_name;
		}
		else if (preset->chennels_presets[chan].synth_type == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
		{
			if (chan != 0)
			{
				sum_text += "\n";
			}
			sum_text += "Ch" + std::to_string(chan + 1) + std::string(" AS: ") + preset->chennels_presets[chan].adj_synth_patch_name;
		}
	}
	
	*summary = sum_text;
	
	return 0;
}

/**
*   @brief  Sets all the preset params.
*   @param	preset	a pointer to a mod_synth_preset_t presets object
*   @return 0 if OK; _SETTINGS_BAD_PARAMETERS (-4) if presets is NULL is NULL
*/
int ModSynthPatches::set_synth_settings(mod_synth_preset_t *preset) 
{
	return_val_if_true(preset == NULL, _SETTINGS_BAD_PARAMETERS);
	
	bool valid_setting = false;
	
	for (int chan = 0; chan < 16; chan++)
	{
		ModSynth::get_instance()->set_midi_channel_synth(chan, preset->chennels_presets[chan].synth_type);
				
		if (preset->chennels_presets[chan].synth_type == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
		{
			ModSynth::get_instance()->get_fluid_synth()->set_fluid_synth_program_select(
				chan,
				preset->chennels_presets[chan].fluid_prorgram_presets.sound_font_id,
				preset->chennels_presets[chan].fluid_prorgram_presets.midi_bank,
				preset->chennels_presets[chan].fluid_prorgram_presets.midi_program);
			
			valid_setting = true;
		}
		else if (preset->chennels_presets[chan].synth_type == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
		{
			int res;
	
			if (preset->chennels_presets[chan].adj_synth_patch_path != "")
			{
						
				res = mod_synth_open_adj_synth_patch_file(preset->chennels_presets[chan].adj_synth_patch_path, chan);
				// TODO: if res
				valid_setting = true;
			}	
		}
		
		if (valid_setting)
		{		
			ModSynth::get_instance()->adj_synth->midi_mixer_event(
					_MIDI_MIXER_1_EVENT,
				_MIXER_CHAN_1_LEVEL + chan, 
				preset->chennels_presets[chan].mixer_presets.channel_level,
				ModSynth::get_instance()->adj_synth->get_active_settings_params());

			ModSynth::get_instance()->adj_synth->midi_mixer_event(_MIDI_MIXER_1_EVENT,
				_MIXER_CHAN_1_PAN + chan,
				preset->chennels_presets[chan].mixer_presets.channel_pan,
				ModSynth::get_instance()->adj_synth->get_active_settings_params());

			ModSynth::get_instance()->adj_synth->midi_mixer_event(_MIDI_MIXER_1_EVENT,
				_MIXER_CHAN_1_SEND + chan,
				preset->chennels_presets[chan].mixer_presets.channel_send,
				ModSynth::get_instance()->adj_synth->get_active_settings_params());
		}	
				
	}
			
	if (valid_setting)
	{
		ModSynth::get_instance()->set_fluid_synth_volume(preset->fluid_volume);
				
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_reverb_enabled_value(preset->fluid_preset.reverb_enabled);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_reverb_damp_value(preset->fluid_preset.reverb_damp);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_reverb_level_value(preset->fluid_preset.reverb_level);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_reverb_room_size_value(preset->fluid_preset.reverb_room);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_reverb_width_value(preset->fluid_preset.reverb_width);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_reverb_params();
				
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_chorus_enabled_value(preset->fluid_preset.chorus_enabled);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_chorus_depth_value(preset->fluid_preset.chorus_depth);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_chorus_level_value(preset->fluid_preset.chorus_level);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_chorus_N_value(preset->fluid_preset.chorus_n);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_chorus_speed_value(preset->fluid_preset.chorus_speed);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_chorus_waveform_value(preset->fluid_preset.chorus_waveform);
		ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->set_fluid_synth_chorus_params();
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_31_LEVEL,
			preset->adj_preset.equalizer_31_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_62_LEVEL,
			preset->adj_preset.equalizer_62_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_125_LEVEL,
			preset->adj_preset.equalizer_125_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_250_LEVEL,
			preset->adj_preset.equalizer_250_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_500_LEVEL,
			preset->adj_preset.equalizer_500_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_1K_LEVEL,
			preset->adj_preset.equalizer_1K_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_2K_LEVEL,
			preset->adj_preset.equalizer_2K_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_4K_LEVEL,
			preset->adj_preset.equalizer_4K_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_8K_LEVEL,
			preset->adj_preset.equalizer_8K_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
				
		ModSynth::get_instance()->adj_synth->band_equalizer_event(
			_BAND_EQUALIZER_EVENT, 
			_BAND_EQUALIZER_BAND_16K_LEVEL,
			preset->adj_preset.equalizer_16K_level,
			ModSynth::get_instance()->adj_synth->get_active_settings_params());
	}
	
	return 0;
}

/**
*   @brief  Returns a preset params.
*   @param	preset_num		requester preset number (0-4)
*   @return a pointer to a mod_synth_preset_t presets object if OK; NULL if param out of range
*/
mod_synth_preset_t *ModSynthPatches::get_synth_preset_params(int preset_num)
{
	if ((preset_num < 0) || (preset_num >= _NUM_OF_PRESETS))
	{
		return NULL;
	}
	else
	{
		return &synth_presets[preset_num];
	}
}

/**
*   @brief  Returns a preset summary text.
*   @param	preset_num		requester preset number (0-4)
*   @return a preset summart string text if OK; NULL if param out of range
*/
string *ModSynthPatches::get_synth_preset_summary_text(int preset_num)
{
	if ((preset_num < 0) || (preset_num >= _NUM_OF_PRESETS))
	{
		return NULL;
	}
	else
	{
		return &preset_summary_text[preset_num];
	}
}

/**
*   @brief  Copy a soutce preset object conten into a destination preset object.
*   @param	src		a pointer to the source preset
*   @param	dest	a pointer to the destination preset
*   @return void
*/
void ModSynthPatches::copy_presets(mod_synth_preset_t *src, mod_synth_preset_t *dest)
{
	if ((src == NULL) || (dest == NULL))
	{
		return;
	}
	
	dest->name = src->name;
	dest->master_volume = src->master_volume;
	dest->fluid_volume = src->fluid_volume;
	
	dest->adj_preset.equalizer_31_level = src->adj_preset.equalizer_31_level;
	dest->adj_preset.equalizer_62_level = src->adj_preset.equalizer_62_level;
	dest->adj_preset.equalizer_125_level = src->adj_preset.equalizer_125_level;
	dest->adj_preset.equalizer_250_level = src->adj_preset.equalizer_250_level;
	dest->adj_preset.equalizer_500_level = src->adj_preset.equalizer_500_level;
	dest->adj_preset.equalizer_1K_level = src->adj_preset.equalizer_1K_level;
	dest->adj_preset.equalizer_2K_level = src->adj_preset.equalizer_2K_level;
	dest->adj_preset.equalizer_4K_level = src->adj_preset.equalizer_4K_level;
	dest->adj_preset.equalizer_8K_level = src->adj_preset.equalizer_8K_level;
	dest->adj_preset.equalizer_16K_level = src->adj_preset.equalizer_16K_level;	
	dest->adj_preset.equalizer_preset = src->adj_preset.equalizer_preset;
	
	dest->adj_preset.reverb3m_enabled = src->adj_preset.reverb3m_enabled;
	dest->adj_preset.reverb3m_preset = src->adj_preset.reverb3m_preset;
	
	dest->adj_preset.reverb_damp = src->adj_preset.reverb_damp;
	dest->adj_preset.reverb_dry = src->adj_preset.reverb_dry;
	dest->adj_preset.reverb_dry = src->adj_preset.reverb_dry;
	dest->adj_preset.reverb_enabled = src->adj_preset.reverb_enabled;
	dest->adj_preset.reverb_mode = src->adj_preset.reverb_mode;
	dest->adj_preset.reverb_room_size = src->adj_preset.reverb_room_size;
	dest->adj_preset.reverb_wet = src->adj_preset.reverb_wet;
	dest->adj_preset.reverb_width = src->adj_preset.reverb_width;
	
	for (int i = 0; i < _PROGRAM_16; i++)
	{
		dest->chennels_presets[i].adj_synth_patch_name = src->chennels_presets[i].adj_synth_patch_name;
		dest->chennels_presets[i].adj_synth_patch_path = src->chennels_presets[i].adj_synth_patch_path;
		dest->chennels_presets[i].channel_num = src->chennels_presets[i].channel_num;
		dest->chennels_presets[i].synth_type = src->chennels_presets[i].synth_type;
		
		dest->chennels_presets[i].fluid_prorgram_presets.channel_num = src->chennels_presets[i].fluid_prorgram_presets.channel_num;
		dest->chennels_presets[i].fluid_prorgram_presets.midi_bank = src->chennels_presets[i].fluid_prorgram_presets.midi_bank;
		dest->chennels_presets[i].fluid_prorgram_presets.midi_program = src->chennels_presets[i].fluid_prorgram_presets.midi_program;
		dest->chennels_presets[i].fluid_prorgram_presets.midi_sound_font_file_name = 
			src->chennels_presets[i].fluid_prorgram_presets.midi_sound_font_file_name;
		dest->chennels_presets[i].fluid_prorgram_presets.program_name = src->chennels_presets[i].fluid_prorgram_presets.program_name;
		dest->chennels_presets[i].fluid_prorgram_presets.sound_font_id = src->chennels_presets[i].fluid_prorgram_presets.sound_font_id;
		
		dest->chennels_presets[i].mixer_presets.channel_level = src->chennels_presets[i].mixer_presets.channel_level;
		dest->chennels_presets[i].mixer_presets.channel_pan = src->chennels_presets[i].mixer_presets.channel_pan;
		dest->chennels_presets[i].mixer_presets.channel_send = src->chennels_presets[i].mixer_presets.channel_send;
		dest->chennels_presets[i].mixer_presets.channel_num = src->chennels_presets[i].mixer_presets.channel_num;
	}	
}

/**
*   @brief  Selects the active preset .
*   @param	preset	requester preset number (0-4)
*   @return void
*/
void ModSynthPatches::set_active_preset(int preset)
{
	if ((preset >= 0) && (preset < _NUM_OF_PRESETS))
	{
		active_preset = preset;
	}
}

/**
*   @brief  Returns the active preset .
*   @param	non
*   @return active preset
*/
int ModSynthPatches::get_active_preset() { return active_preset; }

