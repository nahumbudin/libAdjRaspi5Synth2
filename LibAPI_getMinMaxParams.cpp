/**
*	@file		LibAPI_getMinMaxParams.cpp
*	@author		Nahum Budin
*	@date		26-Sep-2025
*	@version	1.3
*					1. Rename patch to preset parameters.
*
*	@brief		Get settings parameters min and max values (range)
*
*	History: 
*				version 1.2	15-Oct-2024	Rename patch to preset parameters.
*				version 1.1	9-Feb-2021	Code refactoring and notaion
*				version 1.0	17-jan-2020	1st version
*
*/

#include "modSynth.h"
#include "./LibAPI_settingsManager.h"

int mod_synth_get_osc_detune_max_octave() { return _OSC_DETUNE_MAX_OCTAVE; }
int mod_synth_get_osc_detune_min_octave() { return _OSC_DETUNE_MIN_OCTAVE; }
int mod_synth_get_osc_detune_max_semitones() { return _OSC_DETUNE_MAX_SEMITONES; }
int mod_synth_get_osc_detune_min_semitones() { return _OSC_DETUNE_MIN_SEMITONES; }
float mod_synth_get_osc_detune_max_cents() { return _OSC_DETUNE_MAX_CENTS; }
float mod_synth_get_osc_detune_min_cents() { return _OSC_DETUNE_MIN_CENTS; }
int mod_synth_get_osc1_num_of_drawbars() { return _NUM_OF_UNISON_LEVEL_SLIDERS; }

float mod_synth_get_karplus_strong_max_string_damping() { return _KARPLUS_STRONG_MAX_STRING_DAMPING; }
float mod_synth_get_karplus_strong_min_string_damping() { return _KARPLUS_STRONG_MIN_STRING_DAMPING; }
float mod_synth_get_karplus_strong_max_pluck_damping() { return _KARPLUS_STRONG_MAX_PLUCK_DAMPING; }
float mod_synth_get_karplus_strong_min_pluck_damping() { return _KARPLUS_STRONG_MIN_PLUCK_DAMPING; }
float mod_synth_get_karplus_strong_max_decay() { return _KARPLUS_STRONG_MAX_DECAY; }
float mod_synth_get_karplus_strong_min_decay() { return _KARPLUS_STRONG_MIN_DECAY; }
float mod_synth_get_karplus_strong_max_string_damping_variations() { return _KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION; }
float mod_synth_get_karplus_strong_min_string_damping_variations() { return _KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION; }
float mod_synth_get_karplus_strong_max_pluck_damping_variations() { return _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION; }
float mod_synth_get_karplus_strong_min_pluck_damping_variations() { return _KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION; }

int mod_synth_get_synthesizer_max_num_of_polyphonic_voices() { return _SYNTH_MAX_NUM_OF_VOICES; }
int mod_synth_get_synthesizer_max_num_of_programs() { return _SYNTH_MAX_NUM_OF_PROGRAMS; }

int mod_synth_get_mso_table_length() { return ModSynth::get_instance()->adj_synth->mso_wtab->get_wavetable_length(); }
float *mod_synth_get_mso_morphed_lookup_table()
{
	return AdjSynth::get_instance()->synth_program[AdjSynth::get_instance()->get_active_sketch()]->mso_wtab->morphed_waveform_tab->get_wtab_ptr(); 
	//programsMsoLUT[0]->morphedWaveformLut->getLUTptr(); }
}

float mod_synth_get_filter_min_center_freq() { return _FILTER_MIN_CENTER_FREQ; }
float mod_synth_get_filter_max_center_freq() { return (float)ModSynth::get_instance()->get_sample_rate() / 6.7f; }
float mod_synth_get_filter_min_Q() { return _FILTER_MIN_Q; }
float mod_synth_get_filter_max_Q() { return _FILTER_MAX_Q; }

float mod_synth_get_adsr_max_attack_time_sec() { return ADSR_MAX_ATTACK_TIME_SEC; }
float mod_synth_get_adsr_max_decay_time_sec() { return ADSR_MAX_DECAY_TIME_SEC; }
float mod_synth_get_adsr_max_release_time_sec() { return ADSR_MAX_RELEASE_TIME_SEC; }
float mod_synth_get_adsr_max_forced_release_time_sec() { return ADSR_FORCED_RELEASE_TIME_SEC; }

float mod_synth_get_lfo_min_frequency() { return _MOD_LFO_MIN_FREQ; }
float mod_synth_get_lfo_max_frequency() { return _MOD_LFO_MAX_FREQ; }
float mod_synth_get_portamento_max_time_sec() { return _PORTAMENTO_MAX_TIME_SEC; }


