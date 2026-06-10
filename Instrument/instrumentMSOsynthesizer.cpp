/**
* @file		instrumentMSOsynthesizer.cpp
*	@author		Nahum Budin
*	@date		30-May-2026
*	@version	1.0	Initial release
*					
*	@brief		Implements a MSO synthesizer instrument.
*	
*	History:\n
*	
*/

#include "instrumentMSOsynthesizer.h"

InstrumentMSOsynthesizer::InstrumentMSOsynthesizer(AdjSynth *adj_synth, _settings_params_t *external_settings)
	: Instrument(_INSTRUMENT_NAME_MSO_SYNTH_STR_KEY, true, true, false,
					NULL, NULL, adj_synth, external_settings, _INSTRUMENT_TYPE_PLAYING)
{
	adjheart_synth = adj_synth;
	
	alsa_midi_sequencer_events_handler->set_instrument(this);
	
	init_analog_synth_default_settings(active_settings_params);
	// Adjust the specific settings parameters that are relevant to the MSO synthesizer instrument
	init_specific_mso_synthesizer_default_settings(active_settings_params);
	instrument_settings_manager->set_all_params_type(active_settings_params, "mso-synthesizer-preset");

	// Enable the MSO.
	AdjSynth::get_instance()->mso_event_bool(
		_MSO_1_EVENT,
		_MSO_ENABLE,
		true,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);

	// Set Default Level.
	AdjSynth::get_instance()->mso_event_int(
		_MSO_1_EVENT,
		_MSO_FILTER_SEND_1,
		40,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);

	AdjSynth::get_instance()->mso_event_int(
		_MSO_1_EVENT,
		_MSO_FILTER_SEND_2,
		0,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);

	AdjSynth::get_instance()->mso_event_int(
		_MSO_1_EVENT,
		_MSO_AMP_MOD_ENV_LEVEL,
		100,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
}

InstrumentMSOsynthesizer::~InstrumentMSOsynthesizer()
{
}

void InstrumentMSOsynthesizer::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_on(channel, note, velocity, 0, _MSO_SYNTH_PROGRAM_23);
}

void InstrumentMSOsynthesizer::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	adjheart_synth->midi_play_note_off(channel, note, 0, 0, _MSO_SYNTH_PROGRAM_23);
}

void InstrumentMSOsynthesizer::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentMSOsynthesizer::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentMSOsynthesizer::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
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
			_MSO_SYNTH_PROGRAM_23);
	}
	else if (num == _MIDI_EVENT_CHANNEL_BALANCE_BYTE_2)
	{
		//AdjSynth::get_instance()->amp_event();

		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_MSO_SYNTH_PROGRAM_23);
	}
	else if (num == _MIDI_EVENT_CHANNEL_SEND_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_SEND + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_MSO_SYNTH_PROGRAM_23);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_LEVEL_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_MSO_SYNTH_PROGRAM_23);
	}
	else if (num == _MIDI_EVENT_CHANNEL_PAN_MOD_LFO_SELECT_BYTE_2)
	{
		adjheart_synth->polyphonic_mixer_event(
			_POLYPHONIC_MIXER_EVENT,
			_MIXER_CHAN_1_PAN_MOD_LFO + channel,
			val,
			adjheart_synth->get_active_common_params(),
			_MSO_SYNTH_PROGRAM_23);
	}
}

void InstrumentMSOsynthesizer::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentMSOsynthesizer::sysex_handler(uint8_t *message, int len)
{
}

int InstrumentMSOsynthesizer::set_default_settings_parameters(_settings_params_t *params, int prog)
{

	return 0;
}

int InstrumentMSOsynthesizer::set_instrument_settings(_settings_params_t *preset)
{
	return 0;
}

int InstrumentMSOsynthesizer::events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{
	return 0;
}

int InstrumentMSOsynthesizer::events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{
	return 0;
}
int InstrumentMSOsynthesizer::events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{
	return 0;
}
int InstrumentMSOsynthesizer::events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{
	return 0;
}

int InstrumentMSOsynthesizer::get_int_parameter(int instID, int param_id)
{
	if (instID != _MSO_SYNTH_EVENT)
	{
		return -1;
	}
	
	int res = -1;
	_settings_int_param_t int_param;
	
	if (param_id == _MSO_DETUNE_OCTAVE)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.tune_offset_oct",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_DETUNE_CENTS)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.tune_offset_cents",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_DETUNE_SEMITONES)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.tune_offset_semitones",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_SEND_1)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.send_filter_1",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SEGMENT_A_POSITION)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.segment_position_a",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SEGMENT_B_POSITION)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.segment_position_b",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SEGMENT_C_POSITION)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.segment_position_c",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SEGMENT_D_POSITION)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.segment_position_d",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SEGMENT_E_POSITION)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.segment_position_e",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SEGMENT_F_POSITION)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.segment_position_f",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYMETRY)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.symmetry",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_LFO)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.freq_modulation_lfo_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}	
	else if (param_id == _MSO_FREQ_MOD_LFO_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.freq_modulation_lfo_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_LFO_RATE)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_3.rate",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_LFO_SYMMETRY)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_3.symmetry",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_LFO_WAVEFORM)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_3.waveform",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_ENV)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.freq_modulation_env_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_ENV_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.freq_modulation_env_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_ENV_ATTACK)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_3.attack",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_ENV_DECAY)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_3.decay",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_ENV_SUSTAIN)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_3.sustain",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FREQ_MOD_ENV_RELEASE)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_3.release",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_PWM_MOD_LFO)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.pwm_modulation_lfo_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_PWM_MOD_LFO_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.pwm_modulation_lfo_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYM_MOD_LFO_RATE)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_2.rate",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYM_MOD_LFO_SYMMETRY)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_2.symmetry",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYM_MOD_LFO_WAVEFORM)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_2.waveform",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_PWM_MOD_ENV)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.pwm_modulation_env_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_PWM_MOD_ENV_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.pwm_modulation_env_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYM_MOD_ENV_ATTACK)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_2.attack",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYM_MOD_ENV_DECAY)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_2.decay",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYM_MOD_ENV_SUSTAIN)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_2.sustain",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_SYM_MOD_ENV_RELEASE)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_2.release",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}	
	else if (param_id == _MSO_AMP_MOD_LFO)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.amp_modulation_lfo_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_AMP_MOD_LFO_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.amp_modulation_lfo_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_AMP_MOD_LFO_RATE)
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
	else if (param_id == _MSO_AMP_MOD_LFO_SYMMETRY)
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
	else if (param_id == _MSO_AMP_MOD_LFO_WAVEFORM)
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
	else if (param_id == _MSO_AMP_MOD_ENV)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.amp_modulation_env_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_AMP_MOD_ENV_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.mso_synth.amp_modulation_env_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_AMP_MOD_ENV_ATTACK)
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
	else if (param_id == _MSO_AMP_MOD_ENV_DECAY)
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
	else if (param_id == _MSO_AMP_MOD_ENV_SUSTAIN)
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
	else if (param_id == _MSO_AMP_MOD_ENV_RELEASE)
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
	else if (param_id == _FILTER_FREQ)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.frequency",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_Q)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.q",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_OCT)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.octave",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_KBD_TRACK)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.keyboard_track",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_BAND)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.band",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_FREQ_MOD_LFO)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.freq_modulation_lfo_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_FREQ_MOD_LFO_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.freq_modulation_lfo_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_FREQ_MOD_LFO_RATE)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_4.rate",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_FREQ_MOD_LFO_SYMMETRY)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_4.symmetry",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_FREQ_MOD_LFO_WAVEFORM)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.lfo_4.waveform",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_FREQ_MOD_ENV)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.freq_modulation_env_num",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _FILTER_FREQ_MOD_ENV_LEVEL)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.filter1.freq_modulation_env_level",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_FREQ_MOD_ENV_ATTACK)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_4.attack",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_FREQ_MOD_ENV_DECAY)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_4.decay",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_FREQ_MOD_ENV_SUSTAIN)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_4.sustain",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else if (param_id == _MSO_FILTER_FREQ_MOD_ENV_RELEASE)
	{
		res = instrument_settings_manager->get_int_param(
			active_settings_params,
			"adjsynth.env_4.release",
			&int_param);
		
		if (res == _SETTINGS_KEY_FOUND)
		{
			return int_param.value;
		}
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int InstrumentMSOsynthesizer::init_analog_synth_default_settings(_settings_params_t *params)
{
	return 0;
}

int InstrumentMSOsynthesizer::init_specific_mso_synthesizer_default_settings(_settings_params_t *params)
{
	
	
	// Alocate LFOs and Envelopes for modulation sources, and set the default modulation routings and levels for the 
	// MSO synthesizer instrument
	
	// Set ADSRs and LFOs as the: 1 amp modulators, 2 pwm/sym modulators, 3 frequency modulators, and 4 filter frequency modulators.
	AdjSynth::get_instance()->mso_event_int(
		_MSO_SYNTH_EVENT,
		_MSO_AMP_MOD_LFO,
		_LFO_1,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	AdjSynth::get_instance()->mso_event_int(
		_MSO_SYNTH_EVENT,
		_MSO_AMP_MOD_ENV,
		_ENV_1,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	AdjSynth::get_instance()->mso_event_int(
		_MSO_SYNTH_EVENT,
		_MSO_PWM_MOD_LFO,
		_LFO_2,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	AdjSynth::get_instance()->mso_event_int(
		_MSO_SYNTH_EVENT,
		_MSO_PWM_MOD_ENV,
		_ENV_2,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	AdjSynth::get_instance()->mso_event_int(
		_MSO_SYNTH_EVENT,
		_MSO_FREQ_MOD_LFO,
		_LFO_3,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	AdjSynth::get_instance()->mso_event_int(
		_MSO_SYNTH_EVENT,
		_MSO_FREQ_MOD_ENV,
		_ENV_3,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	AdjSynth::get_instance()->filter_event(
		_FILTER_1_EVENT,
		_FILTER_FREQ_MOD_LFO,
		_LFO_4,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	AdjSynth::get_instance()->filter_event(
		_FILTER_1_EVENT,
		_FILTER_FREQ_MOD_ENV,
		_ENV_4,
		this->active_settings_params,
		_MSO_SYNTH_PROGRAM_23);
	
	return 0;
}