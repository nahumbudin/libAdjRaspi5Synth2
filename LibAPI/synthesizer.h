/**
* @file			oscilator.h
*	@author		Nahum Budin
*	@date		24-Sep-2025
*	@version	1.1
*					1. Code refactoring and notaion presetes renamed to patches (presets are used for settings)
*	
*	History:\n
*		version 1.0		13-Sep-2024: First version
*	
*	@brief		Provides the Oscilators related API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#define _SYNTH_MAX_NUM_OF_VOICES			48
#if (_SYNTH_MAX_NUM_OF_CORE_VOICES > 64)
Error - must be no more than 64
#endif

#define _SYNTH_MAX_NUM_OF_CORES				8
#if (_SYNTH_MAX_NUM_OF_CORES > 8)
		Error -
		must be no more than 8
#endif
	
#define _SYNTH_MAX_NUM_OF_PROGRAMS		_PROGRAM_18 + 1
#if (_SYNTH_MAX_NUM_OF_PROGRAMS > _PROGRAM_18 + 1)
	Error -
	must be no more than PROGRAM_18 + 1
#endif

#define _NUM_OF_PATCHES						5

#define _NUM_OF_PRESETS						5
	
#define	_NUM_OF_HARMONIES					9
	
#define	_NUM_OF_SKETCHES					3

#define _OSC_MAX_FREQUENCY					12543.8539514160f	// MIDI G11
#define _OSC_MIN_FREQUENCY					0.1f				// LFO 8.1757989156f	// MIDI C1

#define _OSC_WAVEFORM_SINE							0
#define _OSC_WAVEFORM_SQUARE						1
#define _OSC_WAVEFORM_PULSE							2
#define _OSC_WAVEFORM_TRIANGLE						3
#define _OSC_WAVEFORM_SAMPHOLD						4

#define _OSC_DETUNE_MAX_OCTAVE						6
#define _OSC_DETUNE_MIN_OCTAVE						-6
#define _OSC_DETUNE_MAX_SEMITONES					11
#define _OSC_DETUNE_MIN_SEMITONES					-11
#define _OSC_DETUNE_MAX_CENTS						7.5f
#define _OSC_DETUNE_MIN_CENTS						-7.5f
#define _OSC_DETUNE_CENTS_FACTORIAL					0.25f
	
#define _NUM_OF_VCOS								2

#define	_NUM_OF_HARMONIES							9



#define _NUM_OF_HARMONIZING_FREQUENCIES				13
#define _MAX_HARMONIES_DISTORTION_FACTOR			3.0f
#define _MIN_HARMONIES_DISTORTION_FACTOR			1.0f
#define _MAX_HARMONIES_DETUNE_FACTOR				0.08f
#define _MIN_HARMONIES_DETUNE_FACTOR				0.0f

#define _KARPLUS_STRONG_STATE_STEADY_OUTPUT				0
#define _KARPLUS_STRONG_STATE_STEADY_BEGIN_NEXT			1
#define _KARPLUS_STRONG_STATE_PLAYING					2

#define _KARPLUS_STRONG_MIN_FREQ						27.5f			// MIDI A0
#define _KARPLUS_STRONG_MAX_FREQ						4186.0f			// MIDI C8

#define _KARPLUS_STRONG_MAX_PLUCK_DAMPING				0.9f
#define _KARPLUS_STRONG_MIN_PLUCK_DAMPING				0.1f

#define _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION		0.5f
#define _KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION		0.0f

#define _KARPLUS_STRONG_MAX_STRING_DAMPING				0.7f
#define _KARPLUS_STRONG_MIN_STRING_DAMPING				0.1f

#define _KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION	0.5f
#define _KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION	0.0f

#define _KARPLUS_STRONG_MAX_CHARACTER_VARIATION			1.0f
#define _KARPLUS_STRONG_MIN_CHARACTER_VARIATION			0.0f

#define _KARPLUS_STRONG_MAX_DECAY						0.999f
#define _KARPLUS_STRONG_MIN_DECAY						0.9f
	
	



#define _WHITE_NOISE				0
#define _PINK_NOISE					1
#define _BROWN_NOISE				2

#define _DISTORTION_MAX_DRIVE		1.f
#define _DISTORTION_MIN_DRIVE		0.f
#define _DISTORTION_MAX_RANGE		100.f
#define _DISTORTION_MIN_RANGE		1.f
#define _DISTORTION_MAX_BLEND		1.f
#define _DISTORTION_MIN_BLEND		0.f

#define _FILTER_MIN_CENTER_FREQ		20.0f
#define _FILTER_MIN_Q				0.7f
#define _FILTER_MAX_Q				5.0f

#define _FILTER_BAND_LPF			0
#define _FILTER_BAND_HPF			1
#define _FILTER_BAND_BPF			2
#define _FILTER_BAND_PASS_ALL		3

#define _CONTROL_SUB_SAMPLING		16
	

		
#define SYNC_ON						true
#define SYNC_OFF					false
#define TRACK_ON					true
#define TRACK_OFF					false								1
	
	
	

#define _MOD_LFO_MIN_FREQ			0.1f
#define _MOD_LFO_MAX_FREQ			10.0f

#define _NUM_OF_LFOS				6
#define _NUM_OF_LFO_DELAY_OPTIONS	5
#define _LFO_NONE					0
#define _LFO_1						1
#define _LFO_2						2
#define _LFO_3						3
#define _LFO_4						4
#define _LFO_5						5
#define _LFO_6						6
#define _LFO_SELECTED				7 // Used for the GUI common LFO controls frame

#define _LFO_1_DELAYED_500MS		7
#define _LFO_2_DELAYED_500MS		8
#define _LFO_3_DELAYED_500MS		9
#define _LFO_4_DELAYED_500MS		10
#define _LFO_5_DELAYED_500MS		11
#define _LFO_6_DELAYED_500MS		12

#define _LFO_1_DELAYED_1000MS		13
#define _LFO_2_DELAYED_1000MS		14
#define _LFO_3_DELAYED_1000MS		15
#define _LFO_4_DELAYED_1000MS		16
#define _LFO_5_DELAYED_1000MS		17
#define _LFO_6_DELAYED_1000MS		18

#define _LFO_1_DELAYED_1500MS		19
#define _LFO_2_DELAYED_1500MS		20
#define _LFO_3_DELAYED_1500MS		21
#define _LFO_4_DELAYED_1500MS		22
#define _LFO_5_DELAYED_1500MS		23
#define _LFO_6_DELAYED_1500MS		24

#define _LFO_1_DELAYED_2000MS		25
#define _LFO_2_DELAYED_2000MS		26
#define _LFO_3_DELAYED_2000MS		27
#define _LFO_4_DELAYED_2000MS		28
#define _LFO_5_DELAYED_2000MS		29
#define _LFO_6_DELAYED_2000MS		30


#define _NUM_OF_ADSRS				6
#define _ENV_NONE					0
#define _ENV_1						1
#define _ENV_2						2
#define _ENV_3						3
#define _ENV_4						4
#define _ENV_5						5
#define _ENV_6						6
#define _ENV_SELECTED				7 // Used for the GUI common ADSR controls frame

// Maximum Attack time in seconds
#define ADSR_MAX_ATTACK_TIME_SEC	2.0f	
// Maximum Decay time in seconds 
#define ADSR_MAX_DECAY_TIME_SEC		5.0f	
// Maximum Release time in seconds
#define ADSR_MAX_RELEASE_TIME_SEC	10.0f
	
#define _MSO_POINT_MAX_VALUE		2046
#define _MSO_POINT_MIN_VALUE		1
#define _MSO_MAX_SYMETRY			95
#define _MSO_MIN_SYMETRY			5
	
#define _PROGRAM_0					0
#define _PROGRAM_1					1
#define _PROGRAM_2					2
#define _PROGRAM_3					3
#define _PROGRAM_4					4
#define _PROGRAM_5					5
#define _PROGRAM_6					6
#define _PROGRAM_7					7
#define _PROGRAM_8					8
#define _PROGRAM_9					9
#define _PROGRAM_10					10
#define _PROGRAM_11					11
#define _PROGRAM_12					12
#define _PROGRAM_13					13
#define _PROGRAM_14					14
#define _PROGRAM_15					15
#define _PROGRAM_16					16		// Scetch 1
#define _PROGRAM_17					17
#define _PROGRAM_18					18
	
#define _SYNTH_MAX_NUM_OF_PROGRAMS		_PROGRAM_18 + 1
#if (_SYNTH_MAX_NUM_OF_PROGRAMS > _PROGRAM_18 + 1)
	Error -
	must be no more than PROGRAM_18 + 1
#endif
		
#define _SYNTH_NUM_OF_PROGRAMS						_SYNTH_MAX_NUM_OF_PROGRAMS

#define _MIDI_MIXER_CHAN_1							_PROGRAM_0
#define _MIDI_MIXER_CHAN_2							_PROGRAM_1
#define _MIDI_MIXER_CHAN_3							_PROGRAM_2
#define _MIDI_MIXER_CHAN_4							_PROGRAM_3
#define _MIDI_MIXER_CHAN_5							_PROGRAM_4
#define _MIDI_MIXER_CHAN_6							_PROGRAM_5
#define _MIDI_MIXER_CHAN_7							_PROGRAM_6
#define _MIDI_MIXER_CHAN_8							_PROGRAM_7
#define _MIDI_MIXER_CHAN_9							_PROGRAM_8
#define _MIDI_MIXER_CHAN_10							_PROGRAM_9
#define _MIDI_MIXER_CHAN_11							_PROGRAM_10
#define _MIDI_MIXER_CHAN_12							_PROGRAM_11
#define _MIDI_MIXER_CHAN_13							_PROGRAM_12
#define _MIDI_MIXER_CHAN_14							_PROGRAM_13
#define _MIDI_MIXER_CHAN_15							_PROGRAM_14
#define _MIDI_MIXER_CHAN_16							_PROGRAM_15
#define _SKETCH_PROGRAM_1							_PROGRAM_16
#define _SKETCH_PROGRAM_2							_PROGRAM_17
#define _SKETCH_PROGRAM_3							_PROGRAM_18
	
#define _NUM_OF_UNISON_LEVEL_SLIDERS				9
	
#define _OSC_1_EVENT								0
#define _OSC_2_EVENT								1

#define _LFO_1_EVENT								2
#define _LFO_2_EVENT								3
#define _LFO_3_EVENT								4
#define _LFO_4_EVENT								5
#define _LFO_5_EVENT								6
#define _LFO_6_EVENT								7
	
#define _ENV_1_EVENT								8
#define _ENV_2_EVENT								9
#define _ENV_3_EVENT								10
#define _ENV_4_EVENT								11
#define _ENV_5_EVENT								12
#define _ENV_6_EVENT								13

#define _ENABLE										((bool)true)
#define _DISABLE									((bool)false)
		
#define _FILTER_1_EVENT								13
#define _FILTER_2_EVENT								14

#define _AMP_CH1_EVENT								15
#define _AMP_CH2_EVENT								16

#define _KBD_1_EVENT								20

#define _NOISE_1_EVENT								25

#define _KARPLUS_1_EVENT							30

#define _REVERB_EVENT								35

#define _DISTORTION_1_EVENT							40
#define _DISTORTION_2_EVENT							41

#define _BAND_EQUALIZER_EVENT						45

#define _MSO_1_EVENT								50

#define _MIDI_MAPPING_EVENT							55

#define _PAD_1_EVENT								60

#define _SEQUENCER_1_EVENT							65

#define _RECORDER_1_EVENT							70

#define	_MIDI_MIXER_1_EVENT							75
	
#define _FLUID_SYNTH_1_EVENT						80
	
#define _MOD_SYNTH_EVENT							85

#define _AUDIO_EVENT_1								90
	
#define _POLYPHONIC_MIXER_EVENT						95

#define _OSC_PARAM_WAVEFORM							110		
	
#define _OSC_WAVEFORM_SINE							0
#define _OSC_WAVEFORM_SQUARE						1
#define _OSC_WAVEFORM_PULSE							2
#define _OSC_WAVEFORM_TRIANGLE						3
#define _OSC_WAVEFORM_SAMPHOLD						4
		
#define _OSC_PWM_SYMMETRY							120
#define _OSC_DETUNE_OCTAVE							130
#define _OSC_DETUNE_SEMITONES						131
#define _OSC_DETUNE_CENTS							132
#define _OSC_FILTER_SEND_1							140
#define _OSC_FILTER_SEND_2							150
#define _OSC_UNISON_MODE							160
	
#define _OSC_UNISON_MODE_12345678					0
#define _OSC_UNISON_MODE_HAMMOND					1
#define _OSC_UNISON_MODE_OCTAVES					2
#define _OSC_UNISON_MODE_C_CHORD					3
#define _OSC_UNISON_MODE_Cm_CHORD					4
#define _OSC_UNISON_MODE_C7_CHORD					5
#define _OSC_UNISON_MODE_Cm7_CHORD					6
	
#define _OSC_UNISON_LEVEL_1							170
#define _OSC_UNISON_LEVEL_2							171
#define _OSC_UNISON_LEVEL_3							172
#define _OSC_UNISON_LEVEL_4							173
#define _OSC_UNISON_LEVEL_5							174
#define _OSC_UNISON_LEVEL_6							175
#define _OSC_UNISON_LEVEL_7							176
#define _OSC_UNISON_LEVEL_8							177
#define _OSC_UNISON_LEVEL_9							181
#define _OSC_UNISON_DISTORTION						178
#define _OSC_UNISON_DETUNE							179
#define _OSC_SYNC									180
#define _OSC_FIX_TONE								190
#define _OSC_TRACK_KBD								200
#define _OSC_1_UNISON_SQUARE						210
#define _OSC_FREQ_MOD_LFO							220
#define _OSC_FREQ_MOD_LFO_LEVEL						221
#define _OSC_FREQ_MOD_ENV							222
#define _OSC_FREQ_MOD_ENV_LEVEL						223
#define _OSC_PWM_MOD_LFO							224
#define _OSC_PWM_MOD_LFO_LEVEL						225
#define _OSC_PWM_MOD_ENV							226
#define _OSC_PWM_MOD_ENV_LEVEL						227
#define _OSC_AMP_MOD_LFO							228
#define _OSC_AMP_MOD_LFO_LEVEL						229
#define _OSC_AMP_MOD_ENV							230
#define _OSC_AMP_MOD_ENV_LEVEL						231
#define _OSC_ENABLE									232
#define _OSC_HAMMOND_PERCUSION_MODE					240
	
#define _HAMMOND_PERCUSION_MODE_OFF					0
#define _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW		1
#define _HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST		2
#define _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW		3
#define _HAMMOND_PERCUSION_MODE_2ND_NORM_FAST		4
#define _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW		5
#define _HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST		6
#define _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW		7
#define _HAMMOND_PERCUSION_MODE_3RD_NORM_FAST		8
	
#define _MOD_ADSR_ATTACK							300
#define _MOD_ADSR_DECAY								301
#define _MOD_ADSR_SUSTAIN							302
#define _MOD_ADSR_RELEASE							303
#define _MOD_LFO_WAVEFORM							310
#define _MOD_LFO_RATE								311
#define _MOD_LFO_SYMMETRY							312	
	
#define _FILTER_FREQ								400
#define _FILTER_OCT									401
#define _FILTER_Q									402
#define _FILTER_KBD_FREQ							403
#define _FILTER_KBD_TRACK							404
#define _FILTER_BAND								405
#define _FILTER_2_TRACK_FILT_1						406
#define _FILTER_FREQ_MOD_LFO						407
#define _FILTER_FREQ_MOD_LFO_LEVEL					408
#define _FILTER_FREQ_MOD_ENV						409
#define _FILTER_FREQ_MOD_ENV_LEVEL					410
	
#define _FILTER_BAND_LPF							0
#define _FILTER_BAND_HPF							1
#define _FILTER_BAND_BPF							2
#define _FILTER_BAND_PASS_ALL						3	
	
	
	
#define _AMP_LEVEL									500
#define _AMP_PAN									501
#define _AMP_ENV_MOD_ENABLE							502
#define _AMP_PAN_MOD_LFO							503
#define _AMP_PAN_MOD_LFO_LEVEL						504
#define _AMP_FIXED_LEVELS							505
#define _AMP_SEND									506
	
#define _AMP_FIXED_LEVELS_DISABLE					0
#define _AMP_FIXED_LEVELS_ENABLE					1
	
#define _MIXER_CHAN_1_LEVEL							510
#define _MIXER_CHAN_2_LEVEL							511
#define _MIXER_CHAN_3_LEVEL							512
#define _MIXER_CHAN_4_LEVEL							513
#define _MIXER_CHAN_5_LEVEL							514
#define _MIXER_CHAN_6_LEVEL							515
#define _MIXER_CHAN_7_LEVEL							516
#define _MIXER_CHAN_8_LEVEL							517
#define _MIXER_CHAN_9_LEVEL							518
#define _MIXER_CHAN_10_LEVEL						519
#define _MIXER_CHAN_11_LEVEL						520
#define _MIXER_CHAN_12_LEVEL						521
#define _MIXER_CHAN_13_LEVEL						522
#define _MIXER_CHAN_14_LEVEL						523
#define _MIXER_CHAN_15_LEVEL						524
#define _MIXER_CHAN_16_LEVEL						525
#define _MIXER_ALL_LEVEL							526

#define _MIXER_CHAN_1_PAN							530
#define _MIXER_CHAN_2_PAN							531
#define _MIXER_CHAN_3_PAN							532
#define _MIXER_CHAN_4_PAN							533
#define _MIXER_CHAN_5_PAN							534
#define _MIXER_CHAN_6_PAN							535
#define _MIXER_CHAN_7_PAN							536
#define _MIXER_CHAN_8_PAN							537
#define _MIXER_CHAN_9_PAN							538
#define _MIXER_CHAN_10_PAN							539
#define _MIXER_CHAN_11_PAN							540
#define _MIXER_CHAN_12_PAN							541
#define _MIXER_CHAN_13_PAN							542
#define _MIXER_CHAN_14_PAN							543
#define _MIXER_CHAN_15_PAN							544
#define _MIXER_CHAN_16_PAN							545
#define _MIXER_ALL_PAN								546
	
#define _MIXER_CHAN_1_SEND							550
#define _MIXER_CHAN_2_SEND							551
#define _MIXER_CHAN_3_SEND							552
#define _MIXER_CHAN_4_SEND							553
#define _MIXER_CHAN_5_SEND							554
#define _MIXER_CHAN_6_SEND							555
#define _MIXER_CHAN_7_SEND							556
#define _MIXER_CHAN_8_SEND							557
#define _MIXER_CHAN_9_SEND							558
#define _MIXER_CHAN_10_SEND							559
#define _MIXER_CHAN_11_SEND							560
#define _MIXER_CHAN_12_SEND							561
#define _MIXER_CHAN_13_SEND							562
#define _MIXER_CHAN_14_SEND							563
#define _MIXER_CHAN_15_SEND							564
#define _MIXER_CHAN_16_SEND							565
#define _MIXER_ALL_SEND								566

#define _MIXER_CHAN_1_PAN_MOD_LFO					570
#define _MIXER_CHAN_2_PAN_MOD_LFO					571
#define _MIXER_CHAN_3_PAN_MOD_LFO					572
#define _MIXER_CHAN_4_PAN_MOD_LFO					573
#define _MIXER_CHAN_5_PAN_MOD_LFO					574
#define _MIXER_CHAN_6_PAN_MOD_LFO					575
#define _MIXER_CHAN_7_PAN_MOD_LFO					576
#define _MIXER_CHAN_8_PAN_MOD_LFO					577
#define _MIXER_CHAN_9_PAN_MOD_LFO					578
#define _MIXER_CHAN_10_PAN_MOD_LFO					579
#define _MIXER_CHAN_11_PAN_MOD_LFO					580
#define _MIXER_CHAN_12_PAN_MOD_LFO					581
#define _MIXER_CHAN_13_PAN_MOD_LFO					582
#define _MIXER_CHAN_14_PAN_MOD_LFO					583
#define _MIXER_CHAN_15_PAN_MOD_LFO					584
#define _MIXER_CHAN_16_PAN_MOD_LFO					585
#define _MIXER_ALL_PAN_MOD_LFO						586
	
#define _MIXER_CHAN_1_PAN_MOD_LFO_LEVEL				587
#define _MIXER_CHAN_2_PAN_MOD_LFO_LEVEL				588
#define _MIXER_CHAN_3_PAN_MOD_LFO_LEVEL				589
#define _MIXER_CHAN_4_PAN_MOD_LFO_LEVEL				590
#define _MIXER_CHAN_5_PAN_MOD_LFO_LEVEL				591
#define _MIXER_CHAN_6_PAN_MOD_LFO_LEVEL				592
#define _MIXER_CHAN_7_PAN_MOD_LFO_LEVEL				593
#define _MIXER_CHAN_8_PAN_MOD_LFO_LEVEL				594
#define _MIXER_CHAN_9_PAN_MOD_LFO_LEVEL				595
#define _MIXER_CHAN_10_PAN_MOD_LFO_LEVEL			596
#define _MIXER_CHAN_11_PAN_MOD_LFO_LEVEL			597
#define _MIXER_CHAN_12_PAN_MOD_LFO_LEVEL			598
#define _MIXER_CHAN_13_PAN_MOD_LFO_LEVEL			599
#define _MIXER_CHAN_14_PAN_MOD_LFO_LEVEL			600
#define _MIXER_CHAN_15_PAN_MOD_LFO_LEVEL			601
#define _MIXER_CHAN_16_PAN_MOD_LFO_LEVEL			602
#define _MIXER_ALL_PAN_MOD_LFO_LEVEL				603
	
	
#define _KBD_PORTAMENTO_LEVEL						610
#define _KBD_PORTAMENTO_ENABLE						611
#define _KBD_SENSITIVITY_LEVEL						612
#define _KBD_POLY_MODE_LIMIT						613
#define _KBD_POLY_MODE_FIFO							614
#define _KBD_POLY_MODE_REUSE						615
#define _KBD_LOW_SENSITIVITY_LEVEL					616
#define _KBD_SPLIT_POINT							617

#define _KBD_SPLIT_POINT_NONE						0
#define _KBD_SPLIT_POINT_C2							1
#define _KBD_SPLIT_POINT_C3							2
#define _KBD_SPLIT_POINT_C4							3
#define _KBD_SPLIT_POINT_C5							4
	
	
#define _NOISE_COLOR								700		
	
#define _WHITE_NOISE								0
#define _PINK_NOISE									1
#define _BROWN_NOISE								2
		
#define _NOISE_SEND_1								701
#define _NOISE_SEND_2								702
#define _NOISE_AMP_MOD_LFO							703
#define _NOISE_AMP_MOD_LFO_LEVEL					704
#define _NOISE_AMP_MOD_ENV							705
#define _NOISE_AMP_MOD_ENV_LEVEL					706
#define _NOISE_ENABLE								707
	
	
#define _KARPLUS_STRONG_EXCITATION_WAVEFORM			800	
	
#define _KARPLUS_STRONG_EXCITATION_WHITE_NOISE		0
#define _KARPLUS_STRONG_EXCITATION_PINK_NOISE		1
#define _KARPLUS_STRONG_EXCITATION_BROWN_NOISE		2
#define _KARPLUS_STRONG_EXCITATION_SINECHIRP		3
#define _KARPLUS_STRONG_EXCITATION_DECAYEDSINE		4
#define _KARPLUS_STRONG_EXCITATION_SAWTOOTH_WAVE	5
#define _KARPLUS_STRONG_EXCITATION_SQUARE_WAVE		6
	
		
#define _KARPLUS_STRONG_STRING_DUMP_CALC_MODE		801	
	
#define _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT	0
#define _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC	1
		
#define _KARPLUS_STRONG_STRING_DAMPING				802
#define _KARPLUS_STRONG_STRING_DAMPING_VARIATION	803
#define _KARPLUS_STRONG_PLUCK_DAMPING				804
#define _KARPLUS_STRONG_PLUCK_DAMPING_VARIATION		805
#define _KARPLUS_STRONG_ON_DECAY					806
#define _KARPLUS_STRONG_OFF_DECAY					807
#define _KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS 808
#define _KARPLUS_STRONG_ENABLE						809	
	
#define _KARPLUS_STRONG_SEND_1						812
#define _KARPLUS_STRONG_SEND_2						813
	
#define _SYNTH_VOICE_1								0
	
	
#define _REVERB_ROOM_SIZE							900
#define _REVERB_ENABLE								901
#define _REVERB_DAMP								902
#define _REVERB_WET									903
#define _REVERB_DRY									904
#define _REVERB_WIDTH								905
#define _REVERB_MODE								906
#define _REVERB_PRESET								907
#define _REVERB3M_ENABLE							908

#define _REVERP_PRESET_CUSTOM						0
	
#define _PLAY_MODE									1000
#define _PLAY_MODE_POLY								0
#define _PLAY_MODE_SOLO								1
#define _PLAY_MODE_MIDI								2
	
#define _ENABLE_DISTORTION							1100
#define _DISTORTION_AUTO_GAIN						1101
#define _DISTORTION_DRIVE							1102
#define _DISTORTION_RANGE							1103
#define _DISTORTION_BLEND							1104

	
	
#define _BAND_EQUALIZER_BAND_31_LEVEL				1200
#define _BAND_EQUALIZER_BAND_62_LEVEL				1201
#define _BAND_EQUALIZER_BAND_125_LEVEL				1202
#define _BAND_EQUALIZER_BAND_250_LEVEL				1203
#define _BAND_EQUALIZER_BAND_500_LEVEL				1204
#define _BAND_EQUALIZER_BAND_1K_LEVEL				1205
#define _BAND_EQUALIZER_BAND_2K_LEVEL				1206
#define _BAND_EQUALIZER_BAND_4K_LEVEL				1207
#define _BAND_EQUALIZER_BAND_8K_LEVEL				1208
#define _BAND_EQUALIZER_BAND_16K_LEVEL				1209

#define _BAND_EQUALIZER_PRESET						1220		// 0?
#define _BAND_EQUALIZER_SET_ALL_ZERO				1221		// 1?
	
	
#define _MSO_SEGMENT_A_POSITION						1250
#define _MSO_SEGMENT_B_POSITION						1251
#define _MSO_SEGMENT_C_POSITION						1252
#define _MSO_SEGMENT_D_POSITION						1253
#define _MSO_SEGMENT_E_POSITION						1254
#define _MSO_SEGMENT_F_POSITION						1255

#define _MSO_SYMETRY								1260

#define _MSO_CALC_BASE_LUT							1270
#define _MSO_CALC_MORPHED_LUT						1271

#define _MSO_DETUNE_OCTAVE							1280
#define _MSO_DETUNE_SEMITONES						1281
#define _MSO_DETUNE_CENTS							1282
#define _MSO_FILTER_SEND_1							1283
#define _MSO_FILTER_SEND_2							1284
#define _MSO_ENABLE									1285

#define _MSO_FREQ_MOD_LFO							1290
#define _MSO_FREQ_MOD_LFO_LEVEL						1291
#define _MSO_FREQ_MOD_ENV							1292
#define _MSO_FREQ_MOD_ENV_LEVEL						1293
#define _MSO_PWM_MOD_LFO							1294
#define _MSO_PWM_MOD_LFO_LEVEL						1295
#define _MSO_PWM_MOD_ENV							1296
#define _MSO_PWM_MOD_ENV_LEVEL						1297
#define _MSO_AMP_MOD_LFO							1298
#define _MSO_AMP_MOD_LFO_LEVEL						1299
#define _MSO_AMP_MOD_ENV							1300
#define _MSO_AMP_MOD_ENV_LEVEL						1301
	
	
#define _PAD_DETUNE_OCTAVE							1500
#define _PAD_DETUNE_SEMITONES						1501
#define _PAD_DETUNE_CENTS							1502
#define _PAD_FILTER_SEND_1							1503
#define _PAD_FILTER_SEND_2							1504
#define _PAD_ENABLE									1505

#define _PAD_FREQ_MOD_LFO							1506
#define _PAD_FREQ_MOD_LFO_LEVEL						1507
#define _PAD_FREQ_MOD_ENV							1508
#define _PAD_FREQ_MOD_ENV_LEVEL						1509
#define _PAD_AMP_MOD_LFO							1510
#define _PAD_AMP_MOD_LFO_LEVEL						1511
#define _PAD_AMP_MOD_ENV							1512
#define _PAD_AMP_MOD_ENV_LEVEL						1513
		
#define _PAD_HARMONY_LEVEL_1						1520
#define _PAD_HARMONY_LEVEL_2						1521
#define _PAD_HARMONY_LEVEL_3						1522
#define _PAD_HARMONY_LEVEL_4						1523
#define _PAD_HARMONY_LEVEL_5						1524
#define _PAD_HARMONY_LEVEL_6						1525
#define _PAD_HARMONY_LEVEL_7						1526
#define _PAD_HARMONY_LEVEL_8						1527
#define _PAD_HARMONY_LEVEL_9						1528
#define _PAD_HARMONY_LEVEL_10						1529

#define _PAD_QUALITY								1530

#define _PAD_QUALITY_32K							0
#define _PAD_QUALITY_64K							1
#define _PAD_QUALITY_128K							2
#define _PAD_QUALITY_256K							3
#define _PAD_QUALITY_512K							4
#define _PAD_QUALITY_1024K							5

#define _PAD_SHAPE									1531		
		
#define _PAD_SHAPE_RECTANGULAR						0
#define _PAD_SHAPE_GAUSSIAN							1
#define _PAD_SHAPE_DOUBLE_EXP						2
		
#define _PAD_BASE_NOTE								1532

#define _PAD_BASE_NOTE_C2							0
#define _PAD_BASE_NOTE_G2							1
#define _PAD_BASE_NOTE_C3							2
#define _PAD_BASE_NOTE_G3							3
#define _PAD_BASE_NOTE_C4							4
#define _PAD_BASE_NOTE_G4							5
#define _PAD_BASE_NOTE_C5							6
#define _PAD_BASE_NOTE_G5							7
#define _PAD_BASE_NOTE_C6							8
#define _PAD_BASE_NOTE_G6							9
		
#define _PAD_BASE_WIDTH								1533
#define _PAD_GENERATE								1534
#define _PAD_DETUNE									1535

#define _PAD_SHAPE_CUTOFF							1540		
		
#define _PAD_SHAPE_CUTOFF_FULL						0
#define _PAD_SHAPE_CUTOFF_UPPER						1
#define _PAD_SHAPE_CUTOFF_LOWER						2
		
#define _SEQUENCER_START_RECORDING					1600
#define _SEQUENCER_STOP_RECORDING					1601


	
#define _AUDIO_DRIVER								2110
#define _AUDIO_SAMPLE_RATE							2120
#define _AUDIO_BLOCK_SIZE							2130

#define	_AUDIO_JACK_MODE							2200
#define _AUDIO_JACK_AUTO_START						2130
#define _AUDIO_JACK_AUTO_CONNECT					2140
	
	
	
	
#define _FLUID_SYNTH_VOLUME							10000
#define _FLUID_SYNTH_REVERB_ROOM_SIZE				10010
#define _FLUID_SYNTH_REVERB_DAMP					10020
#define _FLUID_SYNTH_REVERB_WIDTH					10030
#define _FLUID_SYNTH_REVERB_LEVEL					10040
#define _FLUID_SYNTH_REVERB_ENABLE					10050

#define _FLUID_SYNTH_CHORUS_NUMBER					10100
#define _FLUID_SYNTH_CHORUS_LEVEL					10110
#define _FLUID_SYNTH_CHORUS_SPEED					10120
#define _FLUID_SYNTH_CHORUS_DEPTH					10130
#define _FLUID_SYNTH_CHORUS_WAVEFORM				10140
#define _FLUID_SYNTH_CHORUS_ENABLE					10150
	
	
