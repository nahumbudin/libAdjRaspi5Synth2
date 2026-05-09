/**
* @file			stringSynthesizer.h
*	@author		Nahum Budin
*	@date		1-May-2026
*	@version	1.0
*	
*	@brief		Provides the String Synthesizer Instrument related API
*	
*/

#pragma once

#define _KPS_DETUNE_OCTAVE							130
#define _KPS_DETUNE_SEMITONES						131
#define _KPS_DISTORTION_DRIVE						132
#define _KPS_DISTORTION_BLEND						133
#define _KPS_DISTORTION_RANGE						134
#define _KPS_DISTORTION_ACTIVE						135
#define _KPS_DISTORTION_AUTO_GAIN_ENABLE			136

int mod_synth_set_string_synth_param_value(int mod_id, int param_id, int value);
int mod_synth_get_string_synth_param_value(int mod_id, int param_id);
