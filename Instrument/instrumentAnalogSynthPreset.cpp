/**
* @file		instrumentAnalogSynthPreset.cpp
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
*				  instruments.
*				
*	History:\n
*		
*/

#include "instrumentAnalogSynthPreset.h"
#include "../LibAPI/synthesizer.h"
#include "../LibAPI/defines.h"

InstrumentAnalogSynthPresetManager* InstrumentAnalogSynthPresetManager::instrument_analog_synth_preset_manager_instance = nullptr;

InstrumentAnalogSynthPreset::InstrumentAnalogSynthPreset(AdjSynth *adj_synth, int midi_program)
	: Instrument(_INSTRUMENT_NAME_ANALOG_SYNTH_PRESET_STR_KEY + std::string("-") + std::to_string(midi_program - _ANALOG_SYNTH_PRESET_1_PROGRAM_30 + 1), 
					true, true, false,
					NULL, NULL, adj_synth, NULL, _INSTRUMENT_TYPE_PLAYING), 
		midi_program(midi_program)
{
	adjheart_synth = adj_synth;
	
	alsa_midi_sequencer_events_handler->set_instrument(this);
	
	init_analog_synth_default_settings(active_settings_params);
	
	instrument_settings_manager->set_all_params_type(active_settings_params, "adj_synth_settings_params");
}

InstrumentAnalogSynthPreset::~InstrumentAnalogSynthPreset()
{
}

int InstrumentAnalogSynthPreset::get_midi_program()
{
	return midi_program;
}

// Set all analog synth parameters to default values for the Hammond Organ program.
int InstrumentAnalogSynthPreset::init_analog_synth_default_settings(_settings_params_t *params)
{
	int res = 0;
	
	res = adjheart_synth->set_default_preset_parameters_amp(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_distortion(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_filter(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_kps(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_modulators(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_mso(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_noise(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_pad(params, midi_program);
	res |= adjheart_synth->set_default_preset_parameters_vco(params, midi_program);
	
	return res;
}

void InstrumentAnalogSynthPreset::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_on(channel, note, velocity, 0, midi_program);
}

void InstrumentAnalogSynthPreset::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_off(channel, note, 0, 0, midi_program);
}

void InstrumentAnalogSynthPreset::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentAnalogSynthPreset::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentAnalogSynthPreset::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	int res = 0;
	
	// All Notes/Sounds Off handler (ignore channel) TODO: channel
	if ((num == _MIDI_ALL_SOUNDS_OFF) || (num == _MIDI_ALL_NOTES_OFF))
	{
		for (int v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			adjheart_synth->synth_voice[v]->audio_voice->set_inactive();
			adjheart_synth->synth_voice[v]->audio_voice->reset_wait_for_not_active();
		}
	}
	else if (num == _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2)
	{
		//AdjSynth::get_instance()->audio_out->set_master_volume(val);
		//AdjSynth::get_instance()->set
		
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT, 
			_MIXER_CHAN_1_LEVEL + channel,
			val,
			adjheart_synth->get_active_common_params(),
			midi_program);
	}
	else if (num == _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2)
	{
		//AdjSynth::get_instance()->amp_event();

		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN + channel,
			val,
			adjheart_synth->get_active_common_params(),
			midi_program);
	}
	else if (num == _MIDI_EVENT_CHANNEL_SEND_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_SEND + channel,
			val,
			adjheart_synth->get_active_common_params(),
			midi_program);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
			val,
			adjheart_synth->get_active_common_params(),
			midi_program);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO + channel,
			val,
			adjheart_synth->get_active_common_params(),
			midi_program);
	}
	
}

void InstrumentAnalogSynthPreset::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentAnalogSynthPreset::sysex_handler(uint8_t *message, int len)
{
}


InstrumentAnalogSynthPresetManager* InstrumentAnalogSynthPresetManager::get_instance(AdjSynth *adj_synth)
{
	if (instrument_analog_synth_preset_manager_instance == nullptr)
	{
		instrument_analog_synth_preset_manager_instance = new InstrumentAnalogSynthPresetManager(adj_synth);
	}
	return instrument_analog_synth_preset_manager_instance;
}

InstrumentAnalogSynthPresetManager::InstrumentAnalogSynthPresetManager(AdjSynth *adj_synth)
{
	// Create the preset module instruments and add them to the instrument manager
	for (int i = 0; i < _MAX_NUM_OF_ANALOG_PRESET_INSTRUMENTS; i++)
	{
		preset_instruments[i] = new InstrumentAnalogSynthPreset(adj_synth, _ANALOG_SYNTH_PRESET_1_PROGRAM_30 + i);
	}
}

int InstrumentAnalogSynthPresetManager::get_preset_instrument_midi_program_num(int inst_num)
{
	if (inst_num < 0 || inst_num >= _MAX_NUM_OF_ANALOG_PRESET_INSTRUMENTS)
	{
		return -1; // Invalid instrument number
	}
	
	return preset_instruments[inst_num]->get_midi_program();
}
InstrumentAnalogSynthPreset* InstrumentAnalogSynthPresetManager::get_preset_instrument(int inst_num)
{
	if (inst_num < 0 || inst_num >= _MAX_NUM_OF_ANALOG_PRESET_INSTRUMENTS)
	{
		return nullptr;
	}
	
	return preset_instruments[inst_num];
}
