/**
* @file		instrumentFluidSynth.cpp
*	@author		Nahum Budin
*	@date		7-Oct-2025
*	@version	1.1	
*					1. "Manua" copy of settings structure (mutx added)
*					
*	@brief		Implements a FluidSynth SoundFont synthesizer.
*	
*	History:\n
*	
*		version 1.0	24-June-2024	Initial
*	
*/

#include <string>

#include "instrumentFluidSynth.h"
#include "../utils/xmlFiles.h"
#include "../LibAPI/synthesizer.h"

//FluidSynthInterface *InstrumentFluidSynth::fluid_synth_int_instance = NULL;

InstrumentFluidSynth::InstrumentFluidSynth()
	: Instrument(_INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY, true, true, false)
{

	int res = 0;
		
	fluid_synth_int_instance = NULL;
	callback_ptr_update_fluid_synth_preset_text = NULL;

	alsa_midi_sequencer_events_handler->set_instrument(this);

	fluid_synth_int_instance = FluidSynthInterface::get_instance(this);
	/* Intilize the FluidSynth and active_settings_params */
	fluid_synth_int_instance->initialize_fluid_synthesizer();
	/*  Copy to presets */
	for (int p = 0; p < _NUM_OF_FLUID_PRESETS; p++)
	{
		//presets[p] = *active_settings_params;
		
		presets[p].settings_type = "fluid-synth-preset";
		presets[p].name = "Preset " + to_string(p + 1);

		presets[p].version = active_settings_params->version;

		presets[p].bool_parameters_map = active_settings_params->bool_parameters_map;
		presets[p].float_parameters_map = active_settings_params->float_parameters_map;
		presets[p].int_parameters_map = active_settings_params->int_parameters_map;
		presets[p].string_parameters_map = active_settings_params->string_parameters_map;
	}

	active_preset = 0;

	active_settings_params = &presets[0];
	
	res++; // for debug 
}

InstrumentFluidSynth::~InstrumentFluidSynth()
{
	
}

FluidSynthInterface *InstrumentFluidSynth::get_fluid_synth_interface()
{
	return fluid_synth_int_instance;
}

/**
*   @brief  Register a callback function that updates the UI FluidSynth settings name text display.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
void InstrumentFluidSynth::register_callback_update_preset_text(func_ptr_void_string_t ptr)
{
	callback_ptr_update_fluid_synth_preset_text = ptr;
}

/**
*   @brief  Activate a callback function that updates the UI FluidSynth settings name text display\n
*   @param  None\n
*   @return None
*/
void InstrumentFluidSynth::activate_ui_update_preset_text_callback(std::string text)
{
	if (callback_ptr_update_fluid_synth_preset_text)
	{
		callback_ptr_update_fluid_synth_preset_text(text);
	}
}

void InstrumentFluidSynth::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	fluid_synth_int_instance->fluid_synth_note_on_event(channel, note, velocity);
}

void InstrumentFluidSynth::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	fluid_synth_int_instance->fluid_synth_note_off_event(channel, note);
}

void InstrumentFluidSynth::change_program_handler(uint8_t channel, uint8_t program)
{
	fluid_synth_int_instance->fluid_synth_program_change_event(channel, program);
}

void InstrumentFluidSynth::channel_pressure_handler(uint8_t channel, uint8_t val)
{
	fluid_synth_int_instance->fluid_synth_channel_pressure_event(channel, val);
}

void InstrumentFluidSynth::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	fluid_synth_int_instance->fluid_synth_controller_event(channel, num, val);
}

void InstrumentFluidSynth::pitch_bend_handler(uint8_t channel, int pitch)
{
	fluid_synth_int_instance->fluid_synth_pitch_bend_event(channel, pitch);
}

void InstrumentFluidSynth::sysex_handler(uint8_t *message, int len)
{
	
}

/**
*   @brief  Sets the Fluid synth volume level
*   @param  volume (0-200)
*   @return void
*/
void InstrumentFluidSynth::set_volume(int vol)
{
	// 0-1.0
// TODO:
	float master_volume = 1.f;
	
	double gain = (double)(vol * master_volume) / 200.f; // 200 * 100 / 20000 = 1.0
	
	fluid_res_t res = fluid_synth_int_instance->set_fluid_synth_gain(gain);

	// set gain also verifies range
	if (res == _SETTINGS_OK)
	{
		//res = fluid_synth_int_instance->set_fluid_synth_gain(gain);
	}
}

/**
*   @brief  Sets the Fluid synth reverb room size
*   @param  room size (0-120)
*   @return void
*/
void InstrumentFluidSynth::set_reverb_room_size(int val)
{
	// 0-1.0
	int res;
	double fsize = (float)(val) / 120.f;

	res = fluid_synth_int_instance->set_fluid_synth_reverb_room_size_value(fsize);
	res |= fluid_synth_int_instance->set_fluid_synth_reverb_params();
}

/**
*   @brief  Sets the Fluid synth reverb damp
*   @param  damp level (0-100)
*   @return void
*/
void InstrumentFluidSynth::set_reverb_damp(int val)
{
	int res = 0;

	// 0-1.0
	double fdamp = (float)(val) / 100.f;

	res = fluid_synth_int_instance->set_fluid_synth_reverb_damp_value(fdamp);
	res |= fluid_synth_int_instance->set_fluid_synth_reverb_params();
}

/**
*   @brief  Sets the Fluid synth reverb width
*   @param  width level (0-100)
*   @return void
*/
void InstrumentFluidSynth::set_reverb_width(int val)
{
	int res = 0;

	// 0-1.0 (not 0-100.0)
	double fwidth = (float)(val) / 100.0;

	res = fluid_synth_int_instance->set_fluid_synth_reverb_width_value(fwidth);
	res |= fluid_synth_int_instance->set_fluid_synth_reverb_params();
}

/**
*   @brief  Sets the Fluid synth reverb level
*   @param  reverb level (0-100)
*   @return void
*/
void InstrumentFluidSynth::set_reverb_level(int val)
{
	int res = 0;

	// 0-1.0
	double flevel = (float)(val) / 100.f;

	res = fluid_synth_int_instance->set_fluid_synth_reverb_level_value(flevel);
	res |= fluid_synth_int_instance->set_fluid_synth_reverb_params();
}

/**
*   @brief  Sets the Fluid synth chorus number of stages
*   @param  number of stages (0-99)
*   @return void
*/
void InstrumentFluidSynth::set_chorus_number(int val)
{
	int res = 0;

	// number of stages 0-99
	res = fluid_synth_int_instance->set_fluid_synth_chorus_N_value(val);
	res |= fluid_synth_int_instance->set_fluid_synth_chorus_params();
}

/**
*   @brief  Sets the Fluid synth chorus level
*   @param  chorus level (0-99)
*   @return void
*/
void InstrumentFluidSynth::set_chorus_level(int val)
{
	int res = 0;

	// 0-10.0
	double flevel = (float)(val) / 10.f;

	res = fluid_synth_int_instance->set_fluid_synth_chorus_level_value(flevel);
	res |= fluid_synth_int_instance->set_fluid_synth_chorus_params();
}

/**
*   @brief  Sets the Fluid synth chorus speed (Hz)
*   @param  chorus level (30-500)
*   @return void
*/
void InstrumentFluidSynth::set_chorus_speed(int val)
{
	int res = 0;

	// 0.29-5.0
	double fspeed = (float)(val) / 100.f;

	res = fluid_synth_int_instance->set_fluid_synth_chorus_speed_value(fspeed);
	res |= fluid_synth_int_instance->set_fluid_synth_chorus_params();
}

/**
*   @brief  Sets the Fluid synth chorus depth
*   @param  depth (0-210)
*   @return void
*/
void InstrumentFluidSynth::set_chorus_depth(int val)
{
	int res = 0;

	// 0-21.0
	double fdepth = (float)(val) / 10.f;

	res = fluid_synth_int_instance->set_fluid_synth_chorus_depth_value(fdepth);
	res |= fluid_synth_int_instance->set_fluid_synth_chorus_params();
}

/**
*   @brief  Sets the Fluid synth waveform
*   @param  waveform _FLUID_CHORUS_MOD_SINE (0) or _FLUID_CHORUS_MOD_TRIANGLE (1)
*   @return void
*/
void InstrumentFluidSynth::set_chorus_waveform(int val)
{
	int res = 0;

	res = fluid_synth_int_instance->set_fluid_synth_chorus_waveform_value(val);
	res |= fluid_synth_int_instance->set_fluid_synth_chorus_params();
}

/**
*   @brief  Enable the Fluid synth reverb effect
*   @param  none
*   @return void
*/
void InstrumentFluidSynth::enable_reverb()
{
	fluid_synth_int_instance->enable_fluid_synth_reverb();
}

/**
*   @brief  Disable the Fluid synth reverb effect
*   @param  none
*   @return void
*/
void InstrumentFluidSynth::disable_reverb()
{
	fluid_synth_int_instance->disable_fluid_synth_reverb();
}

/**
*   @brief  Enable the Fluid synth chorus effect
*   @param  none
*   @return void
*/
void InstrumentFluidSynth::enable_chorus()
{
	fluid_synth_int_instance->enable_fluid_synth_chorus();
}

/**
*   @brief  Disable the Fluid synth chorus effect
*   @param  none
*   @return void
*/
void InstrumentFluidSynth::disable_chorus()
{
	fluid_synth_int_instance->disable_fluid_synth_chorus();
}

/**
*   @brief  Select an instrument on a MIDI channel by SoundFont ID, bank and program numbers.
*   @param  chan MIDI channel 0-15
*	@param	sfid ID of loaded sondfone
*	@param	bank bank number
*	@param	program MIDI program number
*   @return 0 on successes -1 otherwise
*/
int InstrumentFluidSynth::set_fluid_synth_program_select(int chan, unsigned int sfid,
								   unsigned int bank, unsigned int program)
{
	int res;
	std::string preset_prefix = "fluid.synth.ch" + std::to_string(chan + 1) + ".midi_";
	std::string preset_name;
	std::string sfont_name;

	res = instrument_settings->set_int_param_value(
		active_settings_params,
		preset_prefix + "program",
		program,
		_SET_VALUE,
		-1);

	res |= instrument_settings->set_int_param_value(
		active_settings_params,
		preset_prefix + "bank",
		bank,
		_SET_VALUE,
		-1);

	res |= instrument_settings->set_int_param_value(
		active_settings_params,
		preset_prefix + "sound_font_id",
		sfid,
		_SET_VALUE,
		-1);	

	res |= fluid_synth_int_instance->set_fluid_synth_program_select(chan, sfid, bank, program);

	preset_name = fluid_synth_int_instance->get_fluid_synth_channel_preset_name(chan);
	sfont_name = fluid_synth_int_instance->get_fluid_synth_channel_preset_soundfont_name(chan);

	res |= instrument_settings->set_string_param_value(
		active_settings_params,
		preset_prefix + "program_name",
		preset_name,
		_SET_VALUE,
		-1);

	res |= instrument_settings->set_string_param_value(
		active_settings_params,
		preset_prefix + "sound_font",
		sfont_name,
		_SET_VALUE,
		-1);
	
	return res;
}

/**
*   @brief  Select a SoundFont file for the fluid synth
*   @param  path full path of the sound font file
*   @return void
*/
void InstrumentFluidSynth::set_soundfont(string path)
{
	int res;
	res = fluid_synth_int_instance->set_fluid_synth_soundfont_file(path);
	
}

/**
* Sets the FluidSynth soundfont file settings value, load and activate it.
*
* @param none
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
void InstrumentFluidSynth::load_and_set_fluid_synth_soundfont(string path)
{
	int res;
	res = fluid_synth_int_instance->load_and_set_fluid_synth_soundfont(path);
}

/**
*   @brief  Select an instrument on a MIDI channel by SoundFont ID, bank and program numbers.
*   @param  chan MIDI channel 0-15
*	@param	sfid ID of loaded sondfone
*	@param	bank bank number
*	@param	program MIDI program number
*   @return 0 on successes -1 otherwise
*/
int InstrumentFluidSynth::select_program(int chan,
										 unsigned int sfid,
										 unsigned int bank,
										 unsigned int program)
{

	return 0;
}

int InstrumentFluidSynth::set_default_settings_parameters(_settings_params_t *params, int prog)
{

	return 0;
}

/**
*   @brief  Save the active FluidSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int InstrumentFluidSynth::save_fluid_synth_settings_file(string path)
{
	int res;

	return_val_if_true(instrument_settings == NULL, _SETTINGS_BAD_PARAMETERS);

	res = instrument_settings->write_settings_file(
		active_settings_params,
		instrument_settings->get_settings_version(),
		"",
		path,
		"fluid_settings_params");
	return res;
}

/**
*   @brief  Open FluidSynth settings parameters XML file and set it as the FluidSynth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
int InstrumentFluidSynth::open_fluid_synth_settings_file(string path)
{
	return_val_if_true(instrument_settings == NULL, _SETTINGS_BAD_PARAMETERS);

	XML_files *xml_files = new XML_files();

	int res = instrument_settings->read_settings_file(active_settings_params, path, "fluid_synth_settings");
	fluid_synth_int_instance->set_fluid_synth_chorus_params();
	fluid_synth_int_instance->set_fluid_synth_reverb_params();
	
	if (res == 0)
	{
		activate_ui_update_callback();
		activate_ui_update_preset_text_callback(xml_files->remove_file_extention(xml_files->get_xml_file_name(path)));
	}

	return 0;
}

void InstrumentFluidSynth::set_active_preset(int preset_num)
{
	active_preset = preset_num;

	active_settings_params = &presets[active_preset];

	collect_fluid_synth_prest_channels_data(
		&presets[active_preset], &channels_presets[active_preset][0]);

	set_fluid_synth_settings(active_settings_params, 
							 &channels_presets[active_preset][0]);
}

int InstrumentFluidSynth::get_fluid_synth_channel_bank(int chan)
{
	return fluid_synth_int_instance->get_fluid_synth_channel_bank(chan);
}

int InstrumentFluidSynth::events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{
	if (moduleid == _FLUID_SYNTH_1_EVENT)
	{
		if (paramid == _FLUID_SYNTH_VOLUME)
		{
			set_volume(val);
		}
		else if (paramid == _FLUID_SYNTH_REVERB_ROOM_SIZE)
		{
			set_reverb_room_size(val);
		}
		else if (paramid == _FLUID_SYNTH_REVERB_DAMP)
		{
			set_reverb_damp(val);
		}
		else if (paramid == _FLUID_SYNTH_REVERB_WIDTH)
		{
			set_reverb_width(val);
		}
		else if (paramid == _FLUID_SYNTH_REVERB_LEVEL)
		{
			set_reverb_level(val);
		}
		else if (paramid == _FLUID_SYNTH_CHORUS_NUMBER)
		{
			set_chorus_number(val);
		}
		else if (paramid == _FLUID_SYNTH_CHORUS_LEVEL)
		{
			set_chorus_level(val);
		}
		else if (paramid == _FLUID_SYNTH_CHORUS_SPEED)
		{
			set_chorus_speed(val);
		}
		else if (paramid == _FLUID_SYNTH_CHORUS_DEPTH)
		{
			set_chorus_depth(val);
		}
		else if (paramid == _FLUID_SYNTH_CHORUS_WAVEFORM)
		{
			set_chorus_waveform(val);
		}
		else
		{
			return -2;
		}		
		
		return 0;
	}
	
	return -1;
}

int InstrumentFluidSynth::events_handler(int moduleid, int paramid, float val, _settings_params_t *params, int program)
{
	if (moduleid == _FLUID_SYNTH_1_EVENT)
	{
		return 0;
	}
	
	return -1;
}

int InstrumentFluidSynth::events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	if (moduleid == _FLUID_SYNTH_1_EVENT)
	{
		if (paramid == _FLUID_SYNTH_REVERB_ENABLE)
		{
			if (val)
			{
				enable_reverb();
			}
			else
			{
				disable_reverb();
			}
		}
		else if (paramid == _FLUID_SYNTH_CHORUS_ENABLE)
		{
			if (val)
			{
				enable_chorus();
			}
			else
			{
				disable_chorus();
			}
		}
		else
		{
			return -2;
		}
		
		return 0;
	}
	
	return -1;
}

int InstrumentFluidSynth::events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{
	if (moduleid == _FLUID_SYNTH_1_EVENT)
	{
		return 0;
	}
	
	return -1;
}

