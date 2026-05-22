/**
* @file		instrumentPADsynthesizer.cpp
*	@author		Nahum Budin
*	@date		26-06-2024
*	@version	1.0	Initial release
*					
*	@brief		Implements a PAD synthesizer instrument.
*	
*	History:\n
*	
*/

#include "instrumentPADsynthesizer.h"

InstrumentPADsynthesizer::InstrumentPADsynthesizer(AdjSynth *adj_synth, _settings_params_t *external_settings)
	: Instrument(_INSTRUMENT_NAME_PAD_SYNTH_STR_KEY, true, true, false,
					NULL, NULL, adj_synth, external_settings, _INSTRUMENT_TYPE_PLAYING)
{
	adjheart_synth = adj_synth;
	
	alsa_midi_sequencer_events_handler->set_instrument(this);
	
	init_analog_synth_default_settings(active_settings_params);
	// Adjust the specific settings parameters that are relevant to the PAD synthesizer instrument
	init_specific_pad_synthesizer_default_settings(active_settings_params);
	instrument_settings_manager->set_all_params_type(active_settings_params, "pad-synthesizer-preset");

	// Enable the PAD.
	AdjSynth::get_instance()->pad_event_bool(
		_PAD_SYNTH_EVENT,
		_PAD_ENABLE,
		true,
		this->active_settings_params,
		_PAD_SYNTH_PROGRAM_22);
	
	// Set ADSR_1 and LFO_1 as the amp modulators.
	AdjSynth::get_instance()->pad_event_int(
		_PAD_SYNTH_EVENT,
		_PAD_AMP_MOD_LFO,
		_LFO_1,
		this->active_settings_params,
		_PAD_SYNTH_PROGRAM_22);
	
	AdjSynth::get_instance()->pad_event_int(
		_PAD_SYNTH_EVENT,
		_PAD_AMP_MOD_ENV,
		_ENV_1,
		this->active_settings_params,
		_PAD_SYNTH_PROGRAM_22);
}

InstrumentPADsynthesizer::~InstrumentPADsynthesizer()
{
}

void InstrumentPADsynthesizer::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_on(channel, note, velocity, 0, _PAD_SYNTH_PROGRAM_22);
}

void InstrumentPADsynthesizer::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_off(channel, note, 0, 0, _PAD_SYNTH_PROGRAM_22);
}

void InstrumentPADsynthesizer::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentPADsynthesizer::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentPADsynthesizer::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
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
			_PAD_SYNTH_PROGRAM_22);
	}
	else if (num == _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2)
	{
		//AdjSynth::get_instance()->amp_event();

		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_PAD_SYNTH_PROGRAM_22);
	}
	else if (num == _MIDI_EVENT_CHANNEL_SEND_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_SEND + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_PAD_SYNTH_PROGRAM_22);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_PAD_SYNTH_PROGRAM_22);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_PAD_SYNTH_PROGRAM_22);
	}
}

void InstrumentPADsynthesizer::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentPADsynthesizer::sysex_handler(uint8_t *message, int len)
{
}

int InstrumentPADsynthesizer::set_default_settings_parameters(_settings_params_t *params, int prog)
{

	return 0;
}

int InstrumentPADsynthesizer::set_instrument_settings(_settings_params_t *preset)
{
	return 0;
}

int InstrumentPADsynthesizer::events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{
	return 0;
}

int InstrumentPADsynthesizer::events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{
	return 0;
}
int InstrumentPADsynthesizer::events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	return 0;
}
int InstrumentPADsynthesizer::events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{
	return 0;
}

int InstrumentPADsynthesizer::get_int_parameter(int instID, int param_id)
{
	if (instID != _PAD_SYNTH_EVENT)
	{
		return -1;
	}
	
	int res = -1;
	_settings_int_param_t int_param;

	if (param_id == _MOD_ADSR_ATTACK)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.attack",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_ADSR_DECAY)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.decay",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_ADSR_SUSTAIN)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.sustain",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_ADSR_RELEASE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.release",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_LFO_RATE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_1.rate",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_LFO_SYMMETRY)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_1.symmetry",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_DETUNE_OCTAVE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.tune_offset_octave",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_DETUNE_CENTS)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.tune_offset_cents",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_DETUNE_SEMITONES)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.tune_offset_semitones",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_FILTER_SEND_1)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.send_filter_1",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_FILTER_SEND_2)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.send_filter_2",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_AMP_MOD_LFO_LEVEL)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.amp_modulation_lfo_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_LFO_RATE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_1.rate",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_LFO_WAVEFORM)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_1.waveform",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_LFO_SYMMETRY)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_1.symmetry",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_AMP_MOD_ENV_LEVEL)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.amp_modulation_env_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_ADSR_ATTACK)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.attack",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_ADSR_DECAY)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.decay",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_ADSR_SUSTAIN)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.sustain",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MOD_ADSR_RELEASE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_1.release",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_QUALITY)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.quality",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_SHAPE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.shape",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_SHAPE_CUTOFF)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.shape_cutoff",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_BASE_NOTE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.base_note",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_BASE_WIDTH)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.base_width",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_1)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_0",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_2)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_1",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_3)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_2",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_4)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_3",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_5)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_4",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_6)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_5",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_7)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_6",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_8)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_7",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_9)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_8",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_HARMONY_LEVEL_10)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_level_9",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _PAD_DETUNE)
	{
		
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.pad_synth.harmonies_detune",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}

	return -1;
}

int InstrumentPADsynthesizer::init_analog_synth_default_settings(_settings_params_t *params)
{
	return 0;
}

int InstrumentPADsynthesizer::init_specific_pad_synthesizer_default_settings(_settings_params_t *params)
{
	return 0;
}
