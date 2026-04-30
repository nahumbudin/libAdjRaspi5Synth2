/**
* @file		instrumentHammondOrgan.cpp
*	@author		Nahum Budin
*	@date		26-06-2024
*	@version	1.0	Initial release
*					
*	@brief		Implements an Hammond Organ instrument.
*	
*	History:\n
*	
*/

#include "instrumentHammondOrgan.h"
#include "../LibAPI/synthesizer.h"

InstrumentHammondOrgan::InstrumentHammondOrgan(AdjSynth *adj_synth, _settings_params_t *external_settings)
	: Instrument(_INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY, true, true, false,
					NULL, NULL, adj_synth, external_settings, _INSTRUMENT_TYPE_PLAYING)
{
	adjheart_synth = adj_synth;
	
	for (int i = 1; i < 9; i++)
	{
		drawbar_levels[i] = 0;
	}
	
	drawbar_levels[0] =  8;
	square_wave_enable = false;
	percussion_mode = _HAMMOND_ORGAN_PERCUSION_MODE_OFF;
	percussion_soft = true;
	leslie_speed = 50;
	leslie_level = 0;
	send_filter_1 = 40;
	send_filter_2 = 40;
	
	// Use LFO 6 for the Leslie effect modulation. TODO: allocate a dedicated LFO?
	// Set for all allocated MIDI channels.
	for (int chan = 0; chan < 16; chan++)
	{
		if (get_active_midi_channels())
		{
			mod_synth_midi_mixer_set_channel_pan_mod_lfo(chan, _LFO_6);
		}
	}	
	
	alsa_midi_sequencer_events_handler->set_instrument(this);
	
	init_analog_synth_default_settings(active_settings_params);
	//instrument_settings_manager->set_all_params_type(active_settings_params, "hammond-organ-preset");
	// Adjust the specific settings parameters that are relevant to the Hammond Organ instrument
	init_specific_hammond_organ_default_settings(active_settings_params);
	instrument_settings_manager->set_all_params_type(active_settings_params, "hammond-organ-preset");
}

InstrumentHammondOrgan::~InstrumentHammondOrgan()
{
}

void InstrumentHammondOrgan::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_on(channel, note, velocity, 0, _HAMMOND_ORGAN_PROGRAM_20);
}

void InstrumentHammondOrgan::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_off(channel, note, 0, 0, _HAMMOND_ORGAN_PROGRAM_20);
}

void InstrumentHammondOrgan::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentHammondOrgan::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentHammondOrgan::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
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
			_HAMMOND_ORGAN_PROGRAM_20);
	}
	else if (num == _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2)
	{
		//AdjSynth::get_instance()->amp_event();

		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_HAMMOND_ORGAN_PROGRAM_20);
	}
	else if (num == _MIDI_EVENT_CHANNEL_SEND_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_SEND + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_HAMMOND_ORGAN_PROGRAM_20);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_HAMMOND_ORGAN_PROGRAM_20);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_HAMMOND_ORGAN_PROGRAM_20);
	}
	
}

void InstrumentHammondOrgan::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentHammondOrgan::sysex_handler(uint8_t *message, int len)
{
}

int InstrumentHammondOrgan::set_default_settings_parameters(_settings_params_t *params, int prog)
{

	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{
	if (moduleid != _HAMMOND_ORGAN_EVENT)
	{
		return -1;
	}
	
	int res = 0;
	int level;
	char keyString[128];
	
	switch (paramid)
	{
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_1:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_2:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_3:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_4:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_5:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_6:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_7:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_8:
	case _HAMMOND_ORGAN_DRAWBAR_LEVEL_9:
		if (val < _HAMMOND_ORGAN_DRAWBAR_LEVEL_MIN || val > _HAMMOND_ORGAN_DRAWBAR_LEVEL_MAX)
		{
			return -1;
		}
		
		drawbar_levels[paramid - _HAMMOND_ORGAN_DRAWBAR_LEVEL_1] = val;
		
		// Update active settings parameters
		level = int(float(val) * 100.0 / _HAMMOND_ORGAN_DRAWBAR_LEVEL_MAX); // 0-8 to 0-100
		if (level < 0)
		{
			level = 0;
		}
		else if (level > 100)
		{
			level = 100;
		}
		
		sprintf(keyString, "adjsynth.osc1.unison_level_%i", paramid - _HAMMOND_ORGAN_DRAWBAR_LEVEL_1 + 1);
		res |= instrument_settings_manager->set_int_param_value(
			params,
			string(keyString),
			level,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, keyString);
		
		break;
		
	case _HAMMOND_ORGAN_DETUNE_OCTAVE:
		
		val += _OSC_DETUNE_MIN_OCTAVE;
		
		if (val < _OSC_DETUNE_MIN_OCTAVE || val > _OSC_DETUNE_MAX_OCTAVE)
		{
			return -1;
		}
		
		detune_octave = val;
		
		res |= instrument_settings_manager->set_int_param_value(
		params,
			"adjsynth.osc1.tune_offset_oct",
			detune_octave,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc1.tune_offset_oct");
		
		break;
		
	case _HAMMOND_ORGAN_DETUNE_SEMITONES:
		
		val += _OSC_DETUNE_MIN_SEMITONES;
		
		if (val < _OSC_DETUNE_MIN_SEMITONES || val > _OSC_DETUNE_MAX_SEMITONES)
		{
			return -1;
		}
		
		detune_semitones = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.tune_offset_semitones",
			detune_semitones,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc1.tune_offset_semitones");
		
		break;
		
	case _HAMMOND_ORGAN_DETUNE_CENTS:
		
		val += _OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL; // 0.25 steps
		
		if (val < _OSC_DETUNE_MIN_CENTS || val > _OSC_DETUNE_MAX_CENTS)
		{
			return -1;
		}
		
		detune_cents = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.tune_offset_cents",
			detune_cents,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc1.tune_offset_cents");
		
		break;
		
		
	case _HAMMOND_ORGAN_PERCUSSION_MODE:
		
		if (val < _HAMMOND_ORGAN_PERCUSION_MODE_OFF || val > _HAMMOND_ORGAN_PERCUSION_MODE_3RD_NORM_FAST)
		{
			return -1;
		}
		
		percussion_mode = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.hammond_percussion_mode",
			percussion_mode,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		adjheart_synth->set_hammond_percusion_mode(val, params, _HAMMOND_ORGAN_PROGRAM_20);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc1.hammond_percussion_mode");
		
		break;
		
	case _HAMMOND_ORGAN_LESLIE_SPEED:
		
			if (val < 0 || val > 100)
			{
				return -1;
			}
			
			leslie_speed = val;
		
		// Refresh if cahnnels allocation was changed.
		for (int chan = 0; chan < 16; chan++)
		{
			if (get_active_midi_channels() & (1 << chan))
			{
				mod_synth_midi_mixer_set_channel_pan_mod_lfo(chan, _LFO_6);
				mod_synth_modulator_event_int(_LFO_6_EVENT, _MOD_LFO_RATE, leslie_speed);
			}
		}	
			
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.hammond.organ_leslie_speed",
			leslie_speed,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	case _HAMMOND_ORGAN_LESLIE_LEVEL:
		
		if (val < 0 || val > 100)
		{
			return -1;
		}
			
		leslie_level = val;
		
		// Refresh if cahnnels allocation was changed.
		for (int chan = 0; chan < 16; chan++)
		{
			if (get_active_midi_channels() & (1 << chan))
			{
				mod_synth_midi_mixer_set_channel_pan_mod_lfo(chan, _LFO_6);
				mod_synth_midi_mixer_set_channel_pan_mod_level(chan, leslie_level);
			}
		}	
			
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.hammond.organ_leslie_level",
			leslie_level,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	case _HAMMOND_ORGAN_SEND_FILTER_1:
		
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		send_filter_1 = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.send_filter_1",
			send_filter_1,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc1.send_filter_1");
		
		// If the percussion mode is active, VCO2 send filter 1 should be relative to send filter 1.
		int vco2_send_filter_1;
		if (percussion_mode != _HAMMOND_ORGAN_PERCUSION_MODE_OFF)
		{
			if (percussion_soft)
			{
				vco2_send_filter_1 = send_filter_1 / 3;
			}
			else
			{
				vco2_send_filter_1 = send_filter_1;
			}
			
			res |= instrument_settings_manager->set_int_param_value(
				params,
				"adjsynth.osc2.send_filter_1",
				vco2_send_filter_1,
				_SET_VALUE,
				_HAMMOND_ORGAN_PROGRAM_20);
			
			// Set active playing voice settings
			res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc2.send_filter_1");
		}
		
		break;
		
	case _HAMMOND_ORGAN_SEND_FILTER_2:
		
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		send_filter_2 = val;
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.osc1.send_filter_2",
			send_filter_2,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc1.send_filter_2");
		
		// If the percussion mode is active, VCO2 send filter 2 should be relative to send filter 2.
		int vco2_send_filter_2;
		if (percussion_mode != _HAMMOND_ORGAN_PERCUSION_MODE_OFF)
		{
			if (percussion_soft)
			{
				vco2_send_filter_2 = send_filter_2 / 3;
			}
			else
			{
				vco2_send_filter_2 = send_filter_2;
			}
			
			res |= instrument_settings_manager->set_int_param_value(
				params,
				"adjsynth.osc2.send_filter_2",
				vco2_send_filter_2,
				_SET_VALUE,
				_HAMMOND_ORGAN_PROGRAM_20);
			
			// Set active playing voice settings
			res |= adjheart_synth->update_program_voices_parameter(_HAMMOND_ORGAN_PROGRAM_20, "adjsynth.osc2.send_filter_2");
		}
		
		break;
		
	default:
		return -1;
		
	}
	
	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{

	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	if (moduleid != _HAMMOND_ORGAN_EVENT)
	{
		return -1;
	}
	
	int res = 0;
	
	switch (paramid)
	{
		
	case _HAMMOND_ORGAN_SQUARE_WAVE_ENABLE:
		
		square_wave_enable = val;
		
		res |= instrument_settings_manager->set_bool_param_value(
			params,
			"adjsynth.osc1.unison_square_wave",
			square_wave_enable,
			_SET_VALUE,
			_HAMMOND_ORGAN_PROGRAM_20);
		
		break;
		
	default:
		return -1;
		
	}
	
	return 0;
}

int InstrumentHammondOrgan::events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{

	return 0;
}

int InstrumentHammondOrgan::set_drawbar_level(int drawbar_num, int level)
{
	if (drawbar_num < 1 || drawbar_num > 9 || 
		level < _HAMMOND_ORGAN_DRAWBAR_LEVEL_MIN || 
		level > _HAMMOND_ORGAN_DRAWBAR_LEVEL_MAX)
	{
		return -1;
	}
	
	drawbar_levels[drawbar_num - 1] = level;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_octave_detune(int detune)
{
	if (detune < _OSC_DETUNE_MIN_OCTAVE || detune > _OSC_DETUNE_MAX_OCTAVE)
	{
		return -1;
	}
	
	detune_octave = detune;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_semitones_detune(int detune)
{
	if (detune < _OSC_DETUNE_MIN_SEMITONES || detune > _OSC_DETUNE_MAX_SEMITONES)
	{
		return -1;
	}
	
	detune_semitones = detune;	
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_cents_detune(int detune)
{
	if (detune < _OSC_DETUNE_MIN_CENTS || detune > _OSC_DETUNE_MAX_CENTS)
	{
		return -1;
	}
	
	detune_cents = detune;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_square_wave_enable(bool state)
{
	square_wave_enable = state;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_percussion_mode(int mode)
{
	if (mode < _HAMMOND_ORGAN_PERCUSION_MODE_OFF || mode > _HAMMOND_ORGAN_PERCUSION_MODE_3RD_NORM_FAST)
	{
		return -1;
	}
	
	percussion_mode = mode;
	
	switch (percussion_mode)
	{
	case _HAMMOND_ORGAN_PERCUSION_MODE_2ND_SOFT_SLOW:
	case _HAMMOND_ORGAN_PERCUSION_MODE_2ND_SOFT_FAST:
	case _HAMMOND_ORGAN_PERCUSION_MODE_3RD_SOFT_SLOW:
	case _HAMMOND_ORGAN_PERCUSION_MODE_3RD_SOFT_FAST:
		percussion_soft = true;
		break;
		
	default:
		percussion_soft = false;
		break;
	}
	
	// Update active settings parameters
	
	return 0;
}

// The leselie rotating speaker effect is simulated using the mixer pan modilation.
	
int InstrumentHammondOrgan::set_leslie_speed(int speed)
{
	if (speed < 0 || speed > 100)
	{
		return -1;
	}
	
	leslie_speed = speed;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_leslie_level(int level)
{
	if (level < 0 || level > 100)
	{
		return -1;
	}
	
	leslie_level = level;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_send_filter_1(int level)
{
	if (level < 0 || level > 100)
	{
		return -1;
	}
	
	send_filter_1 = level;
	
	// Update active settings parameters
	
	return 0;
}
	
int InstrumentHammondOrgan::set_send_filter_2(int level)
{
	if (level < 0 || level > 100)
	{
		return -1;
	}
	
	send_filter_2 = level;
	
	// Update active settings parameters
	
	return 0;
}



