/**
* @file		instrumentsManager.cpp
*	@author		Nahum Budin
*	@date		22-Sep-2025
*	@version	1.1	
*					Code refactoring rename modules to instruments
*					
*	@brief		Manages active instruments
*	
*	History:
*				Ver 1.0 	10-Jul-2024	Initial release	
*\n
*	
*/

#include <stddef.h>

#include "instrumentsManager.h"
#include "instrumentFluidSynth.h"
#include "instrumentAnalogSynth.h"
#include "instrumentHammondOrgan.h"
#include "instrumentMidiMixer.h"
#include "../modSynth.h"

class ModSynth;

InstrumentsManager *InstrumentsManager::instruments_manager_instance = NULL;

InstrumentsManager::InstrumentsManager(){

	callback_close_instrument_pannel_id_ptr = NULL;
	callback_close_instrument_pannel_name_ptr = NULL;
	
	for (int i = 0; i < 16; i++)
	{
		midi_channels_allocated_synth[i] = en_instruments_ids_t::none_instrument_id;
	}
		
	map_instruments_names[_INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY] = _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY;				
	map_instruments_names[_INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY] = "Hammond Organ";				
	map_instruments_names[_INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY] = "Anlog Synth";				
	map_instruments_names[_INSTRUMENT_NAME_KARPLUS_STRONG_STRING_SYNTH_STR_KEY] = "Karplus Strong String Synth";
	map_instruments_names[_INSTRUMENT_NAME_MORPHED_SINUS_SYNTH_STR_KEY] = "Morphed Sinus Synth";		
	map_instruments_names[_INSTRUMENT_NAME_PADSYNTH_SYNTH_STR_KEY] = "PAD Synth";
	map_instruments_names[_INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY] = "MIDI Player";
	map_instruments_names[_INSTRUMENT_NAME_MIDI_MIXER_STR_KEY] = "MIDI Mixer";	
	map_instruments_names[_INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY] = "MIDI_CHANNELS Mapper";
	map_instruments_names[_INSTRUMENT_NAME_REVERB_STR_KEY] = "Reverb Effect";			
	map_instruments_names[_INSTRUMENT_NAME_DISTORTION_STR_KEY] = "Distortion Effect";		
	map_instruments_names[_INSTRUMENT_NAME_GRAPHIC_EQUALIZER_STR_KEY] = "Graphic Band Equalizer";
	map_instruments_names[_INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY] = "Control Box";	
	map_instruments_names[_INSTRUMENT_NAME_EXT_MIDI_INT_CONTROL_STR_KEY] = "Ext. MIDI Int.";	
	map_instruments_names[_INSTRUMENT_NAME_KEYBOARD_CONTROL_STR_KEY] = "Keyboard Control";

	map_instruments_type[en_instruments_ids_t::fluid_synth] = en_instruments_types_t::synth;
	map_instruments_type[en_instruments_ids_t::adj_analog_synth] = en_instruments_types_t::synth;
	map_instruments_type[en_instruments_ids_t::adj_hammond_organ] = en_instruments_types_t::synth;
	map_instruments_type[en_instruments_ids_t::adj_karplusstrong_string_synth] = en_instruments_types_t::synth;
	map_instruments_type[en_instruments_ids_t::adj_morphed_sin_synth] = en_instruments_types_t::synth;
	map_instruments_type[en_instruments_ids_t::adj_pad_synth] = en_instruments_types_t::synth;
	map_instruments_type[en_instruments_ids_t::adj_midi_player] = en_instruments_types_t::player;
	map_instruments_type[en_instruments_ids_t::adj_midi_mapper] = en_instruments_types_t::control;
	map_instruments_type[en_instruments_ids_t::midi_mixer] = en_instruments_types_t::control;
	map_instruments_type[en_instruments_ids_t::adj_distortion_effect] = en_instruments_types_t::effect;
	map_instruments_type[en_instruments_ids_t::adj_graphic_equilizer] = en_instruments_types_t::effect;
	map_instruments_type[en_instruments_ids_t::adj_reverb_effect] = en_instruments_types_t::effect;
	map_instruments_type[en_instruments_ids_t::adj_ext_midi_interface] = en_instruments_types_t::interface;
	map_instruments_type[en_instruments_ids_t::adj_keyboard_control] = en_instruments_types_t::keyboard;
	
	
}

InstrumentsManager::~InstrumentsManager()
{
	
	
}

InstrumentsManager *InstrumentsManager::get_instance()
{
	if (instruments_manager_instance == NULL)
	{
		instruments_manager_instance = new InstrumentsManager();
	}

	return instruments_manager_instance;
}

int InstrumentsManager::add_instrument(string ins_name, Instrument *inst)
{
	Instrument *instrument = inst;
	
	if (instrument == NULL)
	{
		//TODO: find base on name?
		//if (ins_name = )
	}

	if (map_active_instruments.find(ins_name) == map_active_instruments.end())
	{
		/* Instrument not found - add it*/
		map_active_instruments.insert({ ins_name, instrument });
		
		return 0;
	}
	/* Instrument already exists */
	return -1;
}

int InstrumentsManager::remove_instrument(string ins_name){
	
	map_active_instruments.extract(ins_name);

	return 0;
}

Instrument *InstrumentsManager::get_instrument(std::string ins_name)
{
	if (map_active_instruments.find(ins_name) != map_active_instruments.end())
	{
		/* Instrument found */
		return map_active_instruments[ins_name];
	}
	/* Not Found*/
	return NULL;
}

void InstrumentsManager::close_instrument_pannel_id(en_instruments_ids_t inst_id)
{
	if (callback_close_instrument_pannel_id_ptr != NULL)
	{
		callback_close_instrument_pannel_id_ptr(inst_id);
	}
}

void InstrumentsManager::close_instrument_pannel_name(string inst_name)
{
	if (callback_close_instrument_pannel_name_ptr != NULL)
	{
		callback_close_instrument_pannel_name_ptr(inst_name);
	}
}

void InstrumentsManager::open_instrument_pannel_name(string inst_name)
{
	if (callback_open_instrument_pannel_name_ptr != NULL)
	{
		callback_open_instrument_pannel_name_ptr(inst_name);
	}
}
int InstrumentsManager::set_instrument_active_midi_channels_mask(string instrument_key_str, uint16_t mask)
{
	Instrument *instrument = get_instrument(instrument_key_str);
	
	
	if (instrument != NULL)
	{
		instrument->set_active_midi_channels(mask);
		
		return 0;
	}
	
	return -1;
}

uint16_t InstrumentsManager::get_instrument_active_midi_channels_mask(string instrument_key_str)
{
	Instrument *instrument = get_instrument(instrument_key_str);
	
	
	if (instrument != NULL)
	{
		return instrument->get_active_midi_channels();
	}
	
	return 0;
}

en_instruments_types_t InstrumentsManager::get_instrument_type(en_instruments_ids_t inst_id)
{
	if (map_instruments_type.find(inst_id) == map_instruments_type.end())
	{
		/* Instrument not found */
		return en_instruments_types_t::none_instrument_type;
	}
	
	return map_instruments_type[inst_id];
}

void InstrumentsManager::add_active_instrument(en_instruments_ids_t inst)
{
	bool exist = false;

	for (en_instruments_ids_t act_inst : active_instruments)
	{
		if (act_inst == inst)
		{
			exist = true;
			break;
		}
	}
	
	if (!exist)
	{
		active_instruments.push_back(inst);

		if (map_instruments_type[inst] == en_instruments_types_t::synth)
		{
			/* A synthesizer module added */
			// TODO:
			
		}
	}
}

void InstrumentsManager::remove_active_instrument(en_instruments_ids_t inst)
{
	int i = 0;
	bool exist = false;

	for (en_instruments_ids_t act_inst : active_instruments)
	{
		if (act_inst == inst)
		{
			exist = true;
			break;
		}
		 
		i++;
	}
	
	if (exist)
	{
		active_instruments.erase(active_instruments.begin() + i);

		if (map_instruments_type[inst] == en_instruments_types_t::synth)
		{
			/* A synthesizer module removed */
			// TODO:
		}
	}
}

int InstrumentsManager::allocate_midi_channel_synth(int ch, en_instruments_ids_t synth)
{
	en_instruments_ids_t last_connected;
	int res = 0;
	_settings_int_param_t int_param;
	char key_string[128];
	
	fprintf(stderr, "Allocate MIDI channel %i to instrument %i\n", ch, synth);
	
	if ((ch < 0) || (ch > 15))
	{
		return -1;
	}
	
	uint16_t channels_on_mask = (uint16_t)1 << ch;
	uint16_t channels_off_mask = ~channels_on_mask;
	uint16_t channels;
	
	/* Disconnect previous channel's instrument */
	last_connected = midi_channels_allocated_synth[ch];

	if (last_connected == en_instruments_ids_t::fluid_synth)
	{
		if (ModSynth::get_instance()->get_fluid_synth() != NULL)
		{
			// Clear the current allocated midi channel bit
			channels = ModSynth::get_instance()->get_fluid_synth()->
				alsa_midi_sequencer_events_handler->get_active_midi_channels() & channels_off_mask;
			
			ModSynth::get_instance()->get_fluid_synth()->
				alsa_midi_sequencer_events_handler->set_active_midi_channels(channels);
		}
	}
	else if (last_connected == en_instruments_ids_t::adj_analog_synth)
	{
		if (ModSynth::get_instance()->get_analog_synth() != NULL)
		{
			channels = ModSynth::get_instance()->get_analog_synth()->
				alsa_midi_sequencer_events_handler->get_active_midi_channels() & channels_off_mask;
			
			ModSynth::get_instance()->get_analog_synth()->
				alsa_midi_sequencer_events_handler->set_active_midi_channels(channels);
		}
	}
	else if (last_connected == en_instruments_ids_t::adj_hammond_organ)
	{
		if (ModSynth::get_instance()->get_hammond_organ() != NULL)
		{
			channels = ModSynth::get_instance()->get_hammond_organ()->
				alsa_midi_sequencer_events_handler->get_active_midi_channels() & channels_off_mask;
			
			ModSynth::get_instance()->get_hammond_organ()->
				alsa_midi_sequencer_events_handler->set_active_midi_channels(channels);
		}
	}
	
	// Add the new channel to the other selected instruments
	
	/* Connect the new channel */
	if (synth == en_instruments_ids_t::fluid_synth)
	{
		if (ModSynth::get_instance()->get_fluid_synth() != NULL)
		{
			channels = ModSynth::get_instance()->get_fluid_synth()->
				alsa_midi_sequencer_events_handler->get_active_midi_channels() | channels_on_mask;
			
			ModSynth::get_instance()->get_fluid_synth()->
				alsa_midi_sequencer_events_handler->set_active_midi_channels(channels);
			// Set the new allocated instrument id in the channels allocation map
			midi_channels_allocated_synth[ch] = synth;
		}
	}
	if (synth == en_instruments_ids_t::adj_analog_synth)
	{
		if (ModSynth::get_instance()->get_analog_synth() != NULL)
		{
			channels = ModSynth::get_instance()->get_analog_synth()->
				alsa_midi_sequencer_events_handler->get_active_midi_channels() | channels_on_mask;
			
			ModSynth::get_instance()->get_analog_synth()->
				alsa_midi_sequencer_events_handler->set_active_midi_channels(channels);
			midi_channels_allocated_synth[ch] = synth;
			
			// Can we set the MIDI Mixer channel settings here (Others will be set when allocating a voiceprogram]?
			
			sprintf(key_string, "adjsynth.mixer_channel_%i.level", ch + 1);
			res = ModSynth::get_instance()->get_midi_mixer()->instrument_settings_manager->get_int_param(
				ModSynth::get_instance()->get_midi_mixer()->active_settings_params,
				key_string,
				&int_param);
			
			if (res == _SETTINGS_KEY_FOUND)
			{		
				AdjSynth::get_instance()->polyphonic_mixer_event(
					_POLYPHONIC_MIXER_EVENT, 
					_MIXER_CHAN_1_LEVEL + ch,
					int_param.value,
					ModSynth::get_instance()->get_midi_mixer()->active_settings_params,
					AdjSynth::get_instance()->get_active_sketch()); // <<< TODO:
			}
			
			sprintf(key_string, "adjsynth.mixer_channel_%i.send", ch + 1);
			res = ModSynth::get_instance()->get_midi_mixer()->instrument_settings_manager->get_int_param(
				ModSynth::get_instance()->get_midi_mixer()->active_settings_params,
				key_string,
				&int_param);
			
			if (res == _SETTINGS_KEY_FOUND)
			{		
				AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT, 
					_MIXER_CHAN_1_SEND + ch,
					int_param.value,
					ModSynth::get_instance()->get_midi_mixer()->active_settings_params,
					AdjSynth::get_instance()->get_active_sketch());
			}
			
			
		
		}
	}
	else if (synth == en_instruments_ids_t::adj_hammond_organ)
	{
		if (ModSynth::get_instance()->get_hammond_organ() != NULL)
		{
			channels = ModSynth::get_instance()->get_hammond_organ()->
				alsa_midi_sequencer_events_handler->get_active_midi_channels() | channels_on_mask;
			
			ModSynth::get_instance()->get_hammond_organ()->
				alsa_midi_sequencer_events_handler->set_active_midi_channels(channels);
			midi_channels_allocated_synth[ch] = synth;
		}
	}
	
	return 0;
}
en_instruments_ids_t InstrumentsManager::get_allocated_midi_channel_synth(int ch)
{
	if ((ch < 0) || (ch > 15))
	{
		return en_instruments_ids_t::none_instrument_id;
	}
	
	return midi_channels_allocated_synth[ch];
}

void InstrumentsManager::register_callback_close_instrument_pannel_id(func_ptr_void_en_instruments_ids_t_t ptr)
{
	callback_close_instrument_pannel_id_ptr = ptr;
}

void InstrumentsManager::register_callback_close_instrument_pannel_name(func_ptr_void_string_t ptr)
{
	callback_close_instrument_pannel_name_ptr = ptr;
}

void InstrumentsManager::register_callback_open_instrument_pannel_name(func_ptr_void_string_t ptr)
{
	callback_open_instrument_pannel_name_ptr = ptr;
}
