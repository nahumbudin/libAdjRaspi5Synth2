/**
*	@file		adjSynthPrograms.h
*	@author		Nahum Budin
*	@date		28-Sep-2025
*	@version	1.0
*	
*	The adjSynth Program implements a preset voice settings, in a similar way of a MIDI program.
*	In this way, the adjSynth voices may be dynamically allocated and set with various "sounds" presets.
*
*	However, unlike a common MIDI implementation, the adjSynth programs are directly linked to a MIDI channel.
*	This means that each MIDI channel (1-16) is mapped to a specific program (0-15), unlike the common MIDI,
*	where a program change message may change the program assigned to a MIDI channel.
*	
*	Thus, to change the sound preset of a MIDI channel/program, the user must change the program preset parameters.
*	
*	Unlike in the previous version (adjSynthProgram), in this version concept, a program is just a set
*	of preset parameters that is used to setup each allocated voice object with the program preset parameters,
*	when it is allocated following a NOTE-ON command on a specific MIDI channel.
*	In this way, there is no need for a predefined set of voices per program, and any voice can be allocated to any program.
*	
*	In this version, there are 19 programs (0-18):
*		0-15	MIDI-mapping mode programs used each for a MIDI channel 1-16
*				Settings are loaded with presets and cannot be edited.
*				
*	To create new sound presets for programs 0-15, the user should use the sketch programs (16-18) to create and edit online new presets,
*	and then save the created presets to files. The user may then load the created preset files.
*	
*		16		Sketch 1 - settings can be changed online
*		17		Sketch 2 - settings can be changed online
*		18		Sketch 3 - settings can be changed online
*
*/

#pragma once

#include <vector>
#include <memory>

#include "../Settings/settings.h"
#include "adjSynthVoice.h"
#include "../Audio/audioPolyMixer.h"

#define	_VOICE_NOT_ASSIGNED		0
#define	_VOICE_ASSIGNED			1

class AudioPolyMixer;

class AdjSynthPrograms
{
  public:
	AdjSynthPrograms(
		int prog_num = -1,
		int wt_size = _PAD_DEFAULT_WAVETABLE_SIZE,
		_settings_params_t *preset_params = nullptr,
		AudioPolyMixer *audio_poly_mixer = NULL);
	~AdjSynthPrograms();
	
	/* Get the program number */
	int get_program_num();

	/* Get the active preset parameters */
	_settings_params_t *get_active_program_preset_params();
	/* Set the active preset parameters without updating all the currentlly assigned voices*/
	int set_active_program_preset_params_no_update(_settings_params_t *preset_params);
	/* Set the active preset parameters and update all the currentlly assigned voices*/
	void set_program_preset_params(_settings_params_t *preset_params);
	
	/* Assign a voice with the program preset parameters */
	int assign_voice_with_preset_program_params(SynthVoice *voice, int voice_num);
	
	/* Deallocate a voice */
	int deallocate_voice_from_program(int voice_num);
	
	/* Get the voice number that plays a given not. */
	int get_voice_num_playing_note(int note);

	/* Refresh all program assigned voices with a new preset params */
	int refresh_all_program_voices_with_preset_params(_settings_params_t *preset_params);

	/* Read program preset file */
	int read_program_preset_file(string path = "", string type = "");

	/* Set all the program voices polly mixer gain1 level */
	int set_program_voices_poly_mixer_gain_1_level_int(int level);
	int set_program_voices_poly_mixer_gain_1_level_float(float level);

	/* Set all the program voices polly mixer gain2 level */
	int set_program_voices_poly_mixer_gain_2_level_int(int level);
	int set_program_voices_poly_mixer_gain_2_level_float(float level);
	
	/* Set all the program voices polly mixer pan1 level */
	int set_program_voices_poly_mixer_pan_1_int(int pan);
	int set_program_voices_poly_mixer_pan_1_float(float pan);
	
	/* Set all the program voices polly mixer pan2 level */
	int set_program_voices_poly_mixer_pan_2_int(int pan);
	int set_program_voices_poly_mixer_pan_2_float(int pan);

	/* Set all the program voices polly mixer send1 level */
	int set_program_voices_poly_mixer_send_1_int(int send);
	int set_program_voices_poly_mixer_send_1_float(float send);

	/* Set all the program voices polly mixer send2 level */
	int set_program_voices_poly_mixer_send_2_int(int send);
	int set_program_voices_poly_mixer_send_2_float(float send);
	
	/* Get a voice object. */
	SynthVoice *get_voice(int voice_num);

	/* Used by the MSO a wave table */
	DSP_MorphingSinusOscWTAB *mso_wtab = NULL;

	/* synthPAD wavetable handling */
	SynthPADcreator *synth_pad_creator = NULL;
	Wavetable *program_wavetable = NULL;

	/* Holds the program preset parameters */
	_settings_params_t *active_preset_params;

	Settings *program_settings_manager = NULL;
	
	/* Holds the indexes of the assigned voices. */
	//std::vector<std::unique_ptr<int>> assigned_voices_vector;
	int assigned_voices[_SYNTH_MAX_NUM_OF_VOICES];
	

  private:
	/* Mange the program settings */
	

	/* Holds the program number. */
	int program_num;
	
	/* Holds the indexes of the assigned voices. */
	//std::vector<std::unique_ptr<int>> assigned_voices_vector;
	//int assigned_voices[_SYNTH_MAX_NUM_OF_VOICES];

	/* Holds the program voices polly mixer gain1 level */
	float program_voices_output_gain1;
	/* Holds the program voices polly mixer gain2 level */
	float program_voices_output_gain2;
	/* Holds the program voices polly mixer pan1 level */
	float program_voices_output_pan1;
	/* Holds the program voices polly mixer pan2 level */
	float program_voices_output_pan2;
	/* Holds the program voices polly mixer send1 level */
	float program_voices_output_send1;
	/* Holds the program voices polly mixer send2 level */
	float program_voices_output_send2;

	/* Holds the paresnt AdjSynth audio poly mixer instance */
	AudioPolyMixer	*parent_audio_poly_mixer;
};


