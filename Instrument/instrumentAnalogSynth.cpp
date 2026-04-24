/**
* @file		instrumentHammondAnalogSynth.cpp
*	@author		Nahum Budin
*	@date		24-09-2025
*	@version	1.1	
*					1. Adding a pointer to a AdjSynth object
*					
*	@brief		Implements an Analog Synthesizer instrument.
*	
*	History:\n
*		version 1.0		12-8-2024: First version
*	
*/

#include "instrumentAnalogSynth.h"
#include "../AdjSynth/adjSynth.h"

InstrumentAnalogSynth::InstrumentAnalogSynth(AdjSynth *adj_synth)
	: Instrument(_INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY, true, true, false, 
				 NULL, NULL, adj_synth)
{
	adjheart_synth = adj_synth;
	alsa_midi_sequencer_events_handler->set_instrument(this);

	active_settings_params->name = "Analog Synth Params";
	active_settings_params->settings_type = _ADJ_SYNTH_PRESET_PARAMS;
	active_settings_params->version = instrument_settings_manager->get_settings_version();

	// Preset params are set when a voice is assigned to a program, so we don't set them here.
	
	// Set the settings parameters of the common resources (not program specific) to their default values.
	set_default_settings_parameters(active_settings_params, 
									AdjSynth::get_instance()->get_active_common_params());
}

InstrumentAnalogSynth::~InstrumentAnalogSynth()
{
}

void InstrumentAnalogSynth::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	AdjSynth::get_instance()->midi_play_note_on(channel, note, velocity, 0, adjheart_synth->get_active_sketch());
}

void InstrumentAnalogSynth::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	AdjSynth::get_instance()->midi_play_note_off(channel, note, 0, 0, adjheart_synth->get_active_sketch());
}

void InstrumentAnalogSynth::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentAnalogSynth::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentAnalogSynth::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	int res = 0;
	
	// All Notes/Sounds Off handler (ignore channel) TODO: channel
	if ((num == _MIDI_ALL_SOUNDS_OFF) || (num == _MIDI_ALL_NOTES_OFF))
	{
		for (int v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			AdjSynth::get_instance()->synth_voice[v]->audio_voice->set_inactive();
			AdjSynth::get_instance()->synth_voice[v]->audio_voice->reset_wait_for_not_active();
		}
	}
	else if (num == _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2)
	{
		//AdjSynth::get_instance()->audio_out->set_master_volume(val);
		//AdjSynth::get_instance()->set
		
		AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT, 
			_MIXER_CHAN_1_LEVEL + channel,
			val,
			AdjSynth::get_instance()->get_active_common_params(),
			AdjSynth::get_instance()->get_active_sketch());
	}
	else if (num == _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2)
	{
		//AdjSynth::get_instance()->amp_event();

		AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN + channel,
			val,
			AdjSynth::get_instance()->get_active_common_params(),
			AdjSynth::get_instance()->get_active_sketch());
	}
	else if (num == _MIDI_EVENT_CHANNEL_SEND_BYTE_2)
	{
		AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_SEND + channel,
			val,
			AdjSynth::get_instance()->get_active_common_params(),
			AdjSynth::get_instance()->get_active_sketch());
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2)
	{
		AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
			val,
			AdjSynth::get_instance()->get_active_common_params(),
			AdjSynth::get_instance()->get_active_sketch());
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2)
	{
		AdjSynth::get_instance()->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO + channel,
			val,
			AdjSynth::get_instance()->get_active_common_params(),
			AdjSynth::get_instance()->get_active_sketch());
	}
}

void InstrumentAnalogSynth::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentAnalogSynth::sysex_handler(uint8_t *message, int len)
{
}

int InstrumentAnalogSynth::set_analog_synth_left_jack_output_connection(s_jack_connection_t connection)
{
	jack_out_connection_left.in_client_name = connection.in_client_name;
	jack_out_connection_left.in_client_port_name = connection.in_client_port_name;
	jack_out_connection_left.out_client_name = connection.out_client_name;
	jack_out_connection_left.out_client_port_name = connection.out_client_port_name;

	return 0;
}

int InstrumentAnalogSynth::set_analog_synth_right_jack_output_connection(s_jack_connection_t connection)
{
	jack_out_connection_right.in_client_name = connection.in_client_name;
	jack_out_connection_right.in_client_port_name = connection.in_client_port_name;
	jack_out_connection_right.out_client_name = connection.out_client_name;
	jack_out_connection_right.out_client_port_name = connection.out_client_port_name;

	return 0;
}

s_jack_connection_t InstrumentAnalogSynth::get_analog_synth_left_jack_output_connection()
{

	s_jack_connection_t connection;
	connection.in_client_name = jack_out_connection_left.in_client_name;
	connection.in_client_port_name = jack_out_connection_left.in_client_port_name;
	connection.out_client_name = jack_out_connection_left.out_client_name;
	connection.out_client_port_name = jack_out_connection_left.out_client_port_name;

	return connection;
}

s_jack_connection_t InstrumentAnalogSynth::get_analog_synth_right_jack_output_connection()
{
	s_jack_connection_t connection;

	connection.in_client_name = jack_out_connection_right.in_client_name;
	connection.in_client_port_name = jack_out_connection_right.in_client_port_name;
	connection.out_client_name = jack_out_connection_right.out_client_name;
	connection.out_client_port_name = jack_out_connection_right.out_client_port_name;

	return connection;
}




