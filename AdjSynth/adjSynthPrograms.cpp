/**
*	@file		adjSynthPrograms.cpp
*	@author		Nahum Budin
*	@date		28-Sep-2025
*	@version	1.0
*	
*	The adjSynth Program implements a preset voice settings, in a similar way of a MIDI program.
*	In this way, the adjSynth voices may be dynamically allocated and set with various "sounds" presets.
*	
*	Unlike in the previous version (adjSynthProgram), in this version concept, a program is just a set
*	of preset parameters that is used to setup a general purpose voice object with the program preset
*	parameters.
*	
*	The adjSynth programs collection is a MIDI bank that provides 19 programs (see hereafter), but unlike
*	common MIDI banks, each program does not represent a fix "sound" preset, and may be dynamiclly
*	loaded with any valid preset.
*
*	In this version, there are 19 programs (0-18):
*		0-15	MIDI-mapping mode programs used each for a MIDI channel 1-16
*				Settings are loaded with presets and cannot be addited
*		16		Sketch 1 - settings can be changed online
*		17		Sketch 2 - settings can be changed online
*		18		Sketch 3 - settings can be changed online
*		
*	Programs 16-18 are allocated to sketchs - a mode that provides an online dynamic setting
*	capabilities of the program preset parameters. The user can dynamiclly select the active
*	sketch out of the 3.
*	
*	The MIDI Mapper can allocate any available MIDI channel (1-16) to the active sketch program.
*
*
*/

#include "adjSynthPrograms.h"

AdjSynthPrograms::AdjSynthPrograms(
	int prog_num,
	int wt_size,
	_settings_params_t *preset_params,
	AudioPolyMixer *audio_poly_mixer)
{
	if ((prog_num >= 0) && (prog_num < _SYNTH_MAX_NUM_OF_PROGRAMS))
	{
		program_num = prog_num;
	}
	else
	{
		printf("fatal error - Program Constructor - illegal program number!");
		exit(1);
	}

	if (preset_params != nullptr)
	{
		active_preset_params = preset_params;
		program_settings_manager = new Settings(active_preset_params);
	}
	else
	{
		printf("fatal error - Program Constructor - no settings params!");
		exit(1);
	}

	if (audio_poly_mixer != NULL)
	{
		parent_audio_poly_mixer = audio_poly_mixer;
	}
	else
	{
		printf("fatal error - Program Constructor - no audio poly mixer!");
		exit(1);
	}

	program_wavetable = new Wavetable();
	program_wavetable->size = wt_size;
	program_wavetable->samples = (float *)malloc(wt_size * sizeof(float));

	synth_pad_creator = new SynthPADcreator(program_wavetable, program_wavetable->size);
	program_wavetable->base_freq =
		synth_pad_creator->set_base_frequency(program_wavetable, _PAD_DEFAULT_BASE_NOTE);
	synth_pad_creator->generate_wavetable(program_wavetable);

	mso_wtab = new DSP_MorphingSinusOscWTAB();
	mso_wtab->calc_segments_lengths(&mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->base_waveform_tab, &mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	mso_wtab->calc_segments_lengths(&mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->morphed_waveform_tab, &mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);

	for (int v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		assigned_voices[v] = _VOICE_NOT_ASSIGNED;
	}
}
AdjSynthPrograms::~AdjSynthPrograms()
{
	delete[] program_wavetable->samples;
	delete program_wavetable;

	delete[] mso_wtab->base_waveform_tab;
	delete[] mso_wtab->morphed_waveform_tab;
}

/* Get the program number */
int AdjSynthPrograms::get_program_num()
{
	return program_num;
}

/* Get the active preset parameters */
_settings_params_t *AdjSynthPrograms::get_active_program_preset_params()
{
	return active_preset_params;
}

/* Set the active preset parameters */
int AdjSynthPrograms::set_active_program_preset_params_no_update(_settings_params_t *preset_params)
{
	if (preset_params == nullptr)
	{
		return -1;
	}
	
	active_preset_params = preset_params;
	return 0;
}

/* Set the active preset parameters and update all the currentlly assigned voices */
void AdjSynthPrograms::set_program_preset_params(_settings_params_t *preset_params)
{
	int v;

	if (preset_params == nullptr)
	{
		return;
	}
	
	active_preset_params = preset_params;
	// Update all assigned voices with the new preset parameters
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		SynthVoice *voice = AdjSynth::get_instance()->synth_voice[v];
			
		if (voice)
		{
			voice->set_voice_params(active_preset_params,
				ModSynth::get_instance()->adj_synth->get_active_common_params(), "all");
		}
	}
}

/* Assign a voice with the program preset parameters */
int AdjSynthPrograms::assign_voice_with_preset_program_params(SynthVoice *voice, int voice_num)
{
	static bool first_time_done = false;
	
	if (voice == nullptr)
	{
		return -1;
	}
	
	if ((voice_num < 0) || (voice_num >= _SYNTH_MAX_NUM_OF_VOICES))
	{
		return -2;
	}

	voice->set_allocated_program(program_num);

	// Set the voice with the program preset parameters values
	voice->set_voice_params(active_preset_params, 
		ModSynth::get_instance()->adj_synth->get_active_common_params(), "all");
	
	// Set the MSO wavetable
	voice->dsp_voice->mso_1->set_wavetable(mso_wtab);
	// Set the PAD wavetable
	voice->set_pad_wave_table(program_wavetable);

	// Add the voice to the assigned voices list
	assigned_voices[voice_num] = _VOICE_ASSIGNED;

	return 0;
}

/* Deallocate a voice */
int AdjSynthPrograms::	deallocate_voice_from_program(int voice_num)
{
	int res = -2; // voice not found
	int v;

	if ((voice_num < 0) || (voice_num >= _SYNTH_MAX_NUM_OF_VOICES))
	{
		return -1;
	}

	if (assigned_voices[voice_num] == _VOICE_ASSIGNED)
	{
		// Remove assigned_voice;
		assigned_voices[voice_num] = _VOICE_NOT_ASSIGNED;
		res = 0;
	}

	// Clean up all residual notes that were waiting for not active state
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if ((assigned_voices[voice_num] == _VOICE_ASSIGNED) &&
			!AdjSynth::get_instance()->synth_voice[voice_num]->audio_voice->is_voice_wait_for_not_active() &&
			!AdjSynth::get_instance()->synth_voice[voice_num]->audio_voice->is_voice_active())
		{
			assigned_voices[v] = _VOICE_NOT_ASSIGNED;
		}
	}

	return res;
}

/* Get the voice number that plays a given note (assuming only one exists! TODO: */
int AdjSynthPrograms::get_voice_num_playing_note(int note)
{
	SynthVoice *voice;
	int res = -1;
	int v;

	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
			voice = AdjSynth::get_instance()->synth_voice[v];

			if (voice)
			{
				if (voice->audio_voice->get_note() == note)
				{
					res = v;
				}
				else if (!AdjSynth::get_instance()->synth_voice[v]->audio_voice->is_voice_wait_for_not_active() &&
						 !AdjSynth::get_instance()->synth_voice[v]->audio_voice->is_voice_active())
				{
					// Clean up all residual notes that were waiting for not active state
					assigned_voices[v] = _VOICE_NOT_ASSIGNED;
				}
			}
		}
	}

	return res;
}

/* Refresh all program assigned voices with a new preset params */
int AdjSynthPrograms::refresh_all_program_voices_with_preset_params(_settings_params_t *preset_params)
{
	int v;
	SynthVoice *voice;

	active_preset_params = preset_params;
	// Update all assigned voices with the new preset parameters
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
			voice = AdjSynth::get_instance()->synth_voice[v];
			if (voice)
			{
				voice->set_voice_params(active_preset_params, 
					ModSynth::get_instance()->adj_synth->get_active_common_params(), "all");
			}
		}
	}
	
	return 0;
}

/* Read program preset file 
 * @param params settings parameters structure
 * @param path full path of the file
 * @param type describes the settings paramaters, for example, "fluid_synth_settings"
 * @param channel	set param of midi channel
 * @return #_SETTINGS_OK if read sucessfully, #_SETTINGS_FAILED otherwise
 */
int AdjSynthPrograms::read_program_preset_file(string path, string type)
{
	settings_res_t res;

	res = program_settings_manager->read_settings_file(active_preset_params ,path, type);

	return res;
}

/* Set all the program voices polly mixer gain1 level */
int AdjSynthPrograms::set_program_voices_poly_mixer_gain_1_level_int(int level)
{
	int v;

	if ((level < 0) || (level > 100))
	{
		return -1;
	}
	
	// Set the program voices output gain1 level
	program_voices_output_gain1 = (float)level / 100.0f;
	
	// Update all assigned voices with the new gain level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_gain1_level(v, program_voices_output_gain1);
		}
	}
	
	return 0;
}

int AdjSynthPrograms::set_program_voices_poly_mixer_gain_1_level_float(float level)
{
	int v;

	if ((level < 0.0f) || (level > 1.0f))
	{
		return -1;
	}
	
	// Set the program voices output gain1 level
	program_voices_output_gain1 = level;
	
	// Update all assigned voices with the new gain level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_gain1_level(v, program_voices_output_gain1);
		}
	}

	return 0;
}

/* Set all the program voices polly mixer gain2 level */
int AdjSynthPrograms::set_program_voices_poly_mixer_gain_2_level_int(int level)
{
	int v;

	if ((level < 0) || (level > 100))
	{
		return -1;
	}
	
	// Set the program voices output gain2 level
	program_voices_output_gain2 = (float)level / 100.0f;
	
	// Update all assigned voices with the new gain level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_gain2_level(v, program_voices_output_gain2);
		}
	}

	return 0;
}
int AdjSynthPrograms::set_program_voices_poly_mixer_gain_2_level_float(float level)
{
	int v;
	
	if ((level < 0.0f) || (level > 1.0f))
	{
		return -1;
	}
	
	// Set the program voices output gain2 level
	program_voices_output_gain2 = level;
	
	// Update all assigned voices with the new gain level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_gain2_level(v, program_voices_output_gain2);
		}
	}
	return 0;
}

/* Set all the program voices polly mixer pan1 level */
int AdjSynthPrograms::set_program_voices_poly_mixer_pan_1_int(int pan)
{
	int v;
	
	if ((pan < -100) || (pan > 100))
	{
		return -1;
	}
	
	// Set the program voices output pan1 level
	program_voices_output_pan1 = (float)pan / 100.0f;
	
	// Update all assigned voices with the new pan level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_pan1(v, program_voices_output_pan1);
		}
	}

	return 0;
}
int AdjSynthPrograms::set_program_voices_poly_mixer_pan_1_float(float pan)
{
	int v;

	if ((pan < -1.0f) || (pan > 1.0f))
	{
		return -1;
	}
	
	// Set the program voices output pan1 level
	program_voices_output_pan1 = pan;
	
	// Update all assigned voices with the new pan level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_pan1(v, program_voices_output_pan1);
		}
	}
	return 0;
}

/* Set all the program voices polly mixer pan2 level */
int AdjSynthPrograms::set_program_voices_poly_mixer_pan_2_int(int pan)
{
	int v;

	if ((pan < -100) || (pan > 100))
	{
		return -1;
	}
	
	// Set the program voices output pan2 level
	program_voices_output_pan2 = (float)pan / 100.0f;
	
	// Update all assigned voices with the new pan level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_pan2(v, program_voices_output_pan2);
		}
	}
	return 0;
}
int AdjSynthPrograms::set_program_voices_poly_mixer_pan_2_float(int pan)
{
	int v;
	
	if ((pan < -1.0f) || (pan > 1.0f))
	{
		return -1;
	}
	
	// Set the program voices output pan2 level
	program_voices_output_pan2 = pan;
	
	// Update all assigned voices with the new pan level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_pan2(v, program_voices_output_pan2);
		}
	}
	return 0;
}

/* Set all the program voices polly mixer send1 level */
int AdjSynthPrograms::set_program_voices_poly_mixer_send_1_int(int send)
{
	int v;

	if ((send < 0) || (send > 100))
	{
		return -1;
	}
	
	// Set the program voices output send1 level
	program_voices_output_send1 = (float)send / 100.0f;
	
	// Update all assigned voices with the new send level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_send1_level(v, program_voices_output_send1);
		}
	}
	
	return 0;
}
int AdjSynthPrograms::set_program_voices_poly_mixer_send_1_float(float send)
{
	int v;

	if ((send < 0.0f) || (send > 1.0f))
	{
		return -1;
	}
	
	// Set the program voices output send1 level
	program_voices_output_send1 = send;
	
	// Update all assigned voices with the new send level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_send1_level(v, program_voices_output_send1);
		}
	}
	
	return 0;
}

/* Set all the program voices polly mixer send2 level */
int AdjSynthPrograms::set_program_voices_poly_mixer_send_2_int(int send)
{
	int v;
	
	if ((send < 0) || (send > 100))
	{
		return -1;
	}
	
	// Set the program voices output send2 level
	program_voices_output_send2 = (float)send / 100.0f;
	
	// Update all assigned voices with the new send level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_send2_level(v, program_voices_output_send2);
		}
	}
	
	return 0;
}
int AdjSynthPrograms::set_program_voices_poly_mixer_send_2_float(float send)
{
	int v;
	
	if ((send < 0.0f) || (send > 1.0f))
	{
		return -1;
	}
	
	// Set the program voices output send2 level
	program_voices_output_send2 = send;
	
	// Update all assigned voices with the new send level
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
//			parent_audio_poly_mixer->set_voice_send2_level(v, program_voices_output_send2);
		}
	}
	return 0;
}

/* Get a voice object. Rtuens NULL if the voice is not assigned to the program. */
SynthVoice *AdjSynthPrograms::get_voice(int voice_num)
{
	SynthVoice *voice = NULL;
	int v;

	if ((voice_num < 0) || (voice_num >= _SYNTH_MAX_NUM_OF_VOICES))
	{
		return NULL;
	}

	// Check if the voice is assigned to this program
	for (v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
	{
		// Clean up all residual notes that were waiting for not active state
		if (!AdjSynth::get_instance()->synth_voice[v]->audio_voice->is_voice_wait_for_not_active() &&
			!AdjSynth::get_instance()->synth_voice[v]->audio_voice->is_voice_active())
		{
			assigned_voices[v] = _VOICE_NOT_ASSIGNED;
		}

		if (assigned_voices[v] == _VOICE_ASSIGNED)
		{
			voice = AdjSynth::get_instance()->synth_voice[voice_num];
		}
	}

	return voice;
}
