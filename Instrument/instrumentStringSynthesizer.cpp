/**
* @file		instrumentStringSynth.cpp
*	@author		Nahum Budin
*	@date		1-May-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements a Karplus-Strong String Synth instrument.
*	
*	History:\n
*	
*/

#include "instrumentStringSynth.h"

InstrumentStringSynth::InstrumentStringSynth(AdjSynth *adj_synth, _settings_params_t *external_settings)
	: Instrument(_INSTRUMENT_NAME_KARPLUS_STRONG_STRING_SYNTH_STR_KEY, true, true, false, // MIDI in, audio out, no MIDI out
					NULL, NULL, adj_synth, external_settings, _INSTRUMENT_TYPE_PLAYING)
{
	adjheart_synth = adj_synth;
	
	alsa_midi_sequencer_events_handler->set_instrument(this);

	set_default_settings_parameters(active_settings_params);
	
	instrument_settings_manager->set_all_params_type(active_settings_params, "string-synth-preset");
	
	
}

InstrumentStringSynth::~InstrumentStringSynth()
{
}

void InstrumentStringSynth::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	int play_note = note;
	
	if (detune_octave >= _OSC_DETUNE_MIN_OCTAVE && detune_octave <= _OSC_DETUNE_MAX_OCTAVE)
	{
		play_note += (detune_octave * 12);
	}
	
	if (detune_semitones >= _OSC_DETUNE_MIN_SEMITONES && detune_semitones <= _OSC_DETUNE_MAX_SEMITONES)
	{
		play_note += detune_semitones;
	}
	
	if (play_note < 0)
	{
		play_note = 0;
	}
	else if (play_note > 127)
	{
		play_note = 127;
	}
	
	adjheart_synth->midi_play_note_on(channel, play_note, velocity, 0, _STRING_SYNTH_PROGRAM_21);
}

void InstrumentStringSynth::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	int play_note = note;
	
	if (detune_octave >= _OSC_DETUNE_MIN_OCTAVE && detune_octave <= _OSC_DETUNE_MAX_OCTAVE)
	{
		play_note += (detune_octave * 12);
	}
	
	if (detune_semitones >= _OSC_DETUNE_MIN_SEMITONES && detune_semitones <= _OSC_DETUNE_MAX_SEMITONES)
	{
		play_note += detune_semitones;
	}
	
	if (play_note < 0)
	{
		play_note = 0;
	}
	else if (play_note > 127)
	{
		play_note = 127;
	}
	
	adjheart_synth->midi_play_note_off(channel, play_note, 0, 0, _STRING_SYNTH_PROGRAM_21);
}

void InstrumentStringSynth::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentStringSynth::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentStringSynth::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
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
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT, 
			_MIXER_CHAN_1_LEVEL + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_STRING_SYNTH_PROGRAM_21);
	}
	else if (num == _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2)
	{
		//AdjSynth::get_instance()->amp_event();

		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_STRING_SYNTH_PROGRAM_21);
	}
	else if (num == _MIDI_EVENT_CHANNEL_SEND_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_SEND + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_STRING_SYNTH_PROGRAM_21);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_STRING_SYNTH_PROGRAM_21);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_STRING_SYNTH_PROGRAM_21);
	}
	
}

void InstrumentStringSynth::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentStringSynth::sysex_handler(uint8_t *message, int len)
{
}

int InstrumentStringSynth::set_default_settings_parameters(_settings_params_t *params, int prog)
{
	int res = 0;
	
	res = instrument_settings_manager->set_bool_param_value(
		params,
		"adjsynth.karplus_synth.enabled",
		true,
		_SET_VALUE,
		prog);
	
	return res;
}

void InstrumentStringSynth::set_tune_offset_octave(int octave)
{
	if (octave < _OSC_DETUNE_MIN_OCTAVE || octave > _OSC_DETUNE_MAX_OCTAVE)
	{
		return;
	}
	
	detune_octave = octave;
}

void InstrumentStringSynth::set_tune_offset_semitones(int semitones)
{
	if (semitones < _OSC_DETUNE_MIN_SEMITONES || semitones > _OSC_DETUNE_MAX_SEMITONES)
	{
		return;
	}
	
	detune_semitones = semitones;
}

int InstrumentStringSynth::get_tune_offset_octave()
{
	return detune_octave;
}

int InstrumentStringSynth::get_tune_offset_semitones()
{
	return detune_semitones;
}

int InstrumentStringSynth::events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{
	if (moduleid != _STRING_SYNTH_EVENT)
	{
		return -1;
	}
	
	int res = 0;
	
	switch (paramid)
	{	
	case _KARPLUS_STRONG_EXCITATION_WAVEFORM:
			
		if (val < _KARPLUS_STRONG_EXCITATION_WHITE_NOISE || val > _KARPLUS_STRONG_EXCITATION_SQUARE_WAVE)
			{
				return -1;
			}
			
			res |= instrument_settings_manager->set_int_param_value(
				params,
				"adjsynth.karplus_synth.excitation_waveform_type",
				val,
				_SET_VALUE,
				_STRING_SYNTH_PROGRAM_21);
			
			// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
					"adjsynth.karplus_synth.excitation_waveform_type");
			
		break;
		
	case _KARPLUS_STRONG_STRING_DUMP_CALC_MODE:
		if (val < _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT || val > _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.string_damping_calculation_mode",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
					"adjsynth.karplus_synth.string_damping_calculation_mode");
		
		break;
		
	case _KARPLUS_STRONG_STRING_DAMPING:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.string_damping",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.string_damping");
		
		break;
		
	case _KARPLUS_STRONG_STRING_DAMPING_VARIATION:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.string_damping_variations",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.string_damping_variations");
		
		break;
		
	case _KARPLUS_STRONG_ON_DECAY:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.on_decay",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.on_decay");
		
		break;
		
	case _KARPLUS_STRONG_OFF_DECAY:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.off_decay",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.off_decay");
		
		break;
		
	case _KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.excitation_waveform_variations",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.excitation_waveform_variations");
		
		break;
		
	case _KARPLUS_STRONG_PLUCK_DAMPING:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.pluck_damping",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.pluck_damping");
		
		break;
		
	case _KARPLUS_STRONG_PLUCK_DAMPING_VARIATION:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.pluck_damping_variations",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.pluck_damping_variations");
		
		break;

	case _KARPLUS_STRONG_CHARACTER_VARIATIONS:
		if (val < 0 || val > 100)
		{
			return -1;
		}

		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.character_variations",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);

		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21,
			"adjsynth.karplus_synth.character_variations");

	case _KARPLUS_STRONG_SEND_1:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.send_filter_1",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.send_filter_1");
		
		break;
		
	case _KARPLUS_STRONG_SEND_2:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.karplus_synth.send_filter_2",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.karplus_synth.send_filter_2");
		
		break;
		
	case _KPS_DETUNE_OCTAVE:
		
		val += _OSC_DETUNE_MIN_OCTAVE;
		
		if (val < _OSC_DETUNE_MIN_OCTAVE || val > _OSC_DETUNE_MAX_OCTAVE)
		{
			return -1;
		}
		
		detune_octave = val;
		
		res |= instrument_settings_manager->set_int_param_value(
		params,
			"adjsynth.karplus_synth.tune_offset_oct",
			detune_octave,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, "adjsynth.karplus_synth.tune_offset_oct");
		
		
		break;
		
	case _KPS_DETUNE_SEMITONES:
		val += _OSC_DETUNE_MIN_SEMITONES;
		
		if (val < _OSC_DETUNE_MIN_SEMITONES || val > _OSC_DETUNE_MAX_SEMITONES)
		{
			return -1;
		}
		
		detune_semitones = val;
		
		res |= instrument_settings_manager->set_int_param_value(
		params,
			"adjsynth.karplus_synth.tune_offset_semitones",
			detune_semitones,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, "adjsynth.karplus_synth.tune_offset_semitones");
		
		break;
		
		case _KPS_DISTORTION_DRIVE:
		if (val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.distortion_1.drive",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
		"adjsynth.distortion_1.drive");
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.distortion_2.drive",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.distortion_2.drive");
		
		break;
		
		case _KPS_DISTORTION_RANGE :
		if(val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.distortion_1.range",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.distortion_1.range");
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.distortion_2.range",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.distortion_2.range");
		
		break;
		
		case _KPS_DISTORTION_BLEND :
		if(val < 0 || val > 100)
		{
			return -1;
		}
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.distortion_1.blend",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.distortion_1.blend");
		
		res |= instrument_settings_manager->set_int_param_value(
			params,
			"adjsynth.distortion_2.blend",
			val,
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.distortion_2.blend");
		
		break;
		
		case _KPS_DISTORTION_ACTIVE :
		
		res |= instrument_settings_manager->set_bool_param_value(
			params,
			"adjsynth.distortion.enabled",
			(val > 0 ? true : false),
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.distortion.enabled");
		
		break;
		
	case _KPS_DISTORTION_AUTO_GAIN_ENABLE :
		
		res |= instrument_settings_manager->set_bool_param_value(
			params,
			"adjsynth.distortion.auto_gain_enabled",
			(val > 0 ? true : false),
			_SET_VALUE,
			_STRING_SYNTH_PROGRAM_21);
		
		// Set active playing voice settings
		res |= adjheart_synth->update_program_voices_parameter(_STRING_SYNTH_PROGRAM_21, 
			"adjsynth.distortion.auto_gain_enabled");
		
		break;
		
	default:
		return -1;
	}
	
	return res;
}

int InstrumentStringSynth::events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{
	return 0;
}

int InstrumentStringSynth::events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	return 0;
}

int InstrumentStringSynth::events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{
	return 0;
}

int InstrumentStringSynth::get_param_value(int paramid)
{
	int res = 0;
	_settings_int_param_t val;
	_settings_bool_param_t bool_val;
	
	switch (paramid)
	{
		case _KPS_DETUNE_OCTAVE:
		return detune_octave;
		break;
		
	case _KPS_DETUNE_SEMITONES:
		return detune_semitones;
		break;
		
	case _KARPLUS_STRONG_EXCITATION_WAVEFORM:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.excitation_waveform_type",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return val.value;
		}
		break;
		
		case _KARPLUS_STRONG_STRING_DUMP_CALC_MODE:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.string_damping_calculation_mode",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return val.value;
		}
		break;
		
		case _KARPLUS_STRONG_STRING_DAMPING:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.string_damping",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return val.value;
		}
		break;
		
		case _KARPLUS_STRONG_STRING_DAMPING_VARIATION:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.string_damping_variations",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
			{
			return val.value;
		}
		
		break;
		
		case _KARPLUS_STRONG_ON_DECAY:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.on_decay",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return val.value;
		}
		
		break;
		
		case _KARPLUS_STRONG_OFF_DECAY:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.off_decay",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return val.value;
		}
		
		break;
		
		case _KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.excitation_waveform_variations",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
			{
			return val.value;
		}
		
		break;
		
		case _KARPLUS_STRONG_PLUCK_DAMPING:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.pluck_damping",
			&val);

			if (res == _SETTINGS_KEY_FOUND)
			{
				return val.value;
			}
		
		break;
		
		case _KARPLUS_STRONG_PLUCK_DAMPING_VARIATION:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.pluck_damping_variations",
			&val);

			if (res == _SETTINGS_KEY_FOUND)
			{
				return val.value;
			}
		
		break;
		
		case _KARPLUS_STRONG_CHARACTER_VARIATIONS:
			res = instrument_settings_manager->get_int_param(
				active_settings_params,
				"adjsynth.karplus_synth.character_variations",
				&val);
			
			if (res == _SETTINGS_KEY_FOUND)
			{
				return val.value;
			}

			break;

		case _KARPLUS_STRONG_SEND_1:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.send_filter_1",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
			{
			return val.value;
		}
		
		case _KARPLUS_STRONG_SEND_2:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.karplus_synth.send_filter_2",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return val.value;
		}
		
		break;
		
		case _KPS_DISTORTION_DRIVE:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.distortion.drive",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
			{
			return val.value;
		}
		
		break;
		
		case _KPS_DISTORTION_RANGE:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.distortion.range",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
			{
			return val.value;
		}
		
		break;
		
		case _KPS_DISTORTION_BLEND:
			res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.distortion.blend",
			&val);
		
		if (res == _SETTINGS_KEY_FOUND)
			{
			return val.value;
		}
		
		break;
		
		case _KPS_DISTORTION_ACTIVE:
			res = instrument_settings_manager->get_bool_param(
			active_settings_params,
			"adjsynth.distortion.enabled",
			&bool_val);	
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return bool_val.value ? 1 : 0;
		}
		
		break;
		
		case _KPS_DISTORTION_AUTO_GAIN_ENABLE:
			res = instrument_settings_manager->get_bool_param(
			active_settings_params,
			"adjsynth.distortion.auto_gain_enabled",
			&bool_val);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return bool_val.value ? 1 : 0;
		}
		
		break;
	}
	
	return 0;
}


