/**
* @file			types.h
*	@author		Nahum Budin
*	@date		22-Sep-2025
*	@version	1.1
*					1. Refactoring rename modules to instruments
*					2. Add AdjSynrh panic function
*	
*	@brief		Provides the synthesizer high level API
*	
*	History:
*				version 1.0  6-Jul-2024	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <string>
#include <list>

#include "../LibAPI/types.h"
#include "../Settings/settings.h"

using namespace std;

class Instrument;

/**
*   @brief  Initializes the synthesizer engine.
*			Must be called first before any other call.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init();

/**
*   @brief  Initializes the Bluetooth services.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_bt_services();

/**
*   @brief  Deinitializes the Bluetooth services.
*			terminate al BT related threads
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_bt_services();

/**
*   @brief  Initializes the external MIDI interface service.
*   @param  int port_num  serial port number
*   @return 0 if done
*/
int mod_synth_init_ext_midi_services(int port_num);

/**
*   @brief  Deinitializes the external MIDI interface service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_ext_midi_services();

/**
*   @brief  Initializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_midi_services();

/**
*   @brief  Deinitializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_midi_services();

/**
*   @brief  Closing and cleanup when application goes down.
*			Must be called whenever terminating the application.
*   @param  none
*   @return void
*/
void mod_synth_on_exit();

/**
*   @brief  Returns the synthesizer maximum number of polyiphonic voices.
*   @param  none
*   @return int	maximum number of polyiphonic voices.
*/
int mod_synth_get_synthesizer_max_num_of_polyphonic_voices();
/**
*   @brief  Returns the synthesizer enabled number of polyiphonic voices.
*   @param  none
*   @return int	enabled number of polyiphonic voices.
*/
int mod_synth_get_synthesizer_num_of_polyphonic_voices();
/**
*   @brief  Returns the synthesizer maximum number of programs.
*   @param  none
*   @return int	maximum number of programs.
*/
int mod_synth_get_synthesizer_max_num_of_programs();
/**
*   @brief  Returns the synthesizer enabled number of programs.
*   @param  none
*   @return int	enabled number of programs.
*/
int mod_synth_get_synthesizer_num_of_programs();

/**
*   @brief  Returns the number of the CPU cores.
*   @param  none
*   @return int	number of the CPU cores (1, 2, 3, 4...).
*/
int mod_synth_get_number_of_cores();


/**
*   @brief  Returns the total (all cores) CPU utilization.
*   @param  none
*   @return int	the total (all cores) CPU utilization in precetages (0 to 100).
*/
int mod_synth_get_cpu_utilization();

/**
*   @brief  Adds an instrument.
*   @param  string	instrument name string
*   @param	pointer to the instrument instance
*   @return 0 if done
*/
int mod_synth_add_instrument(string ins_name, Instrument *instrument=NULL);

/**
*   @brief  Removes an instrument.
*   @param  string	instrument name string
*   @return 0 if done
*/
int mod_synth_remove_instrument(string ins_name);

/**
*   @brief  Adds an active instrument.
*   en_instruments_ids_t instrument instrument id
*   @return 0 if done
*/
int mod_synth_add_active_instrument(en_instruments_ids_t mod);

/**
*   @brief  Removes an active instrument.
*   @param  en_instruments_ids_t instrument id
*   @return 0 if done
*/
int mod_synth_remove_active_instrument(en_instruments_ids_t mod);

/**
*   @brief  Allocates an instrument to a midi channel.
*   @param	int midi channel 0-15
*   @param  en_instruments_ids_t instrument id
*   @return 0 if done
*/
int mod_synth_allocate_midi_channel_synth(int ch, en_instruments_ids_t synth);

/**
*   @brief  Return the allocated midi channel instrument.
*   @param	int midi channel 0-15
*   @return en_instruments_ids_t instrument id
*/
en_instruments_ids_t mod_synth_get_allocated_midi_channel_synth(int ch);




/**
*   @brief  Sets the active sketch num
*   @param  ask	the active sketch num _SKETCH_1 to _SKETCH_3
*   @return void
*/
void mod_synth_set_active_sketch(int ask);

/**
*   @brief  Retruns the active sketch num
*   @param  none
*   @return the active sketch num _SKETCH_1 to _SKETCH_3
*/
int mod_synth_get_active_sketch();

/**
*   @brief  Copy source sketch params to a destination sketch params
*   @param  int srcsk	sorce sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @param  int destsk	destination sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @return 0 if done
*/
int mod_synth_copy_sketch(int srcsk, int destsk);

/**
*   @brief  Sets the master volume (affects all voices).
*   @param  int vol	master volume (0-100)
*   @return 0 if done
*/
void mod_synth_set_master_volume(int vol);

/**
*   @brief  Returns the master volume (affects all voices).
*   @param  none
*   @return int master volume (0-100)
*/
int mod_synth_get_master_volume();


/**
*   @brief  Initiates an amplifier-mixer related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h 
*   @param  int ampid	target amp-mixer: _AMP_CH1_EVENT, _AMP_CH2_EVENT
*	@param	int eventid	specific event code:\n
*				_AMP_LEVEL, _AMP_PAN, _AMP_PAN_MOD_LFO, _AMP_PAN_MOD_LFO_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_AMP_LEVEL: 0-100;\n
*				_AMP_PAN: 0-100; 0 -> Left   50 -> Mid   100- -> Right\n	
*				_AMP_PAN_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_AMP_PAN_MOD_LFO_LEVEL: 0-100\n
*
*   @return 0
*/
int mod_synth_amp_event_int(int ampid, int eventid, int val);

/**
*   @brief Initiates an audio related event with integer value.
*   All available parameters values are defined in LibAPI/synthesizer.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_MODE, _AUDIO_SAMPLE_RATE, _AUDIO_BLOCK_SIZE\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_MODE : _JACK_MODE_MANUAL, _JACK_MODE_AUTO\n
*		_AUDIO_JACK_SAMPLE_RATE : _JACK_SAMPLE_RATE_44, _JACK_SAMPLE_RATE_48\n
*		_AUDIO_JACK_BLOCK_SIZE : _JACK_BLOCK_SIZE_256, _JACK_BLOCK_SIZE_512, _JACK_BLOCK_SIZE_1024\n
*/

int mod_synth_audio_event_int(int audid, int eventid, int val);

/**
*   @brief Initiates an audio related event with boolean value.
*   All available parameters values are defined in defs.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_AUTO_START, _AUDIO_JACK_AUTO_CONNECT\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_AUTO_START : _JACK_AUTO_START_DIS, _JACK_AUTO_START_EN\n
*		_AUDIO_JACK_AUTO_CONNECTT : _JACK_AUTO_CONNECT_DIS, _JACK_AUTO_CONNECT_EN\n
*/
int mod_synth_audio_event_bool(int audid, int eventid, bool val);

/**
*   @brief  Initiates a distortion effect related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int distid	target distortion: _DISTORTION_1_EVENT, _DISTORTION_2_EVENT
*	@param	int eventid	specific event code:\n
*				_DISTORTION_DRIVE, _DISTORTION_RANGE, _DISTORTION_BLEND\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_DISTORTION_DRIVE: 0-100\n
*				_DISTORTION_RANGE: 0-100\n
*				_DISTORTION_BLEND: 0-100\n
*
*   @return 0
*/
int mod_synth_distortion_event_int(int distid, int eventid, int val);

/**
*   @brief  Initiates a distortion effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int distid	target distortion: _DISTORTION_1_EVENT, _DISTORTION_2_EVENT
*	@param	int eventid	specific event code:\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN: true - enabled; false - disabled\n
*   @return 0
*/
int mod_synth_distortion_event_bool(int distid, int eventid, bool val);


/**
*   @brief  Initiates a 10 bands equilizer related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int beqid	target equilizer: _BAND_EQUALIZER_EVENT
*	@param	int eventid	specific event code:\n
*				_BAND_EQUALIZER_BAND_31_LEVEL, _BAND_EQUALIZER_BAND_62_LEVEL\n
*				_BAND_EQUALIZER_BAND_125_LEVEL, _BAND_EQUALIZER_BAND_250_LEVEL\n
*				_BAND_EQUALIZER_BAND_500_LEVEL, _BAND_EQUALIZER_BAND_1K_LEVEL\n
*				_BAND_EQUALIZER_BAND_2K_LEVEL, _BAND_EQUALIZER_BAND_4K_LEVEL\n
*				_BAND_EQUALIZER_BAND_8K_LEVEL, _BAND_EQUALIZER_BAND_16K_LEVEL\n
*				_BAND_EQUILIZER_PRESET, _BAND_EQUALIZER_EVENT\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_BAND_EQUALIZER_BAND_xxx_LEVEL: 0-40; 0 -> -20db   40 -> +20db\n
*				_BAND_EQUILIZER_PRESET: not implemented\n
*				_BAND_EQUALIZER_EVENT: 1;
*
*   @return 0
*/
int mod_synth_band_equilizer_event(int beqid, int eventid, int val);

/**
*   @brief  Initiates a filter related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int filtid	target filter: _FILTER_1_EVENT, _FILTER_2_EVENT
*	@param	int eventid	specific event code:\n
*				_FILTER_FREQ, _FILTER_OCT, _FILTER_Q\n
*				_FILTER_KBD_TRACK, _FILTER_BAND\n
*				_FILTER_FREQ_MOD_LFO, _FILTER_FREQ_MOD_LFO_LEVEL\n
*				_FILTER_FREQ_MOD_ENV, _FILTER_FREQ_MOD_ENV_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_FILTER_FREQ: 0-100; 0->getFilterMinCenterFreq(); 100-> getFilterMaxCenterFreq()\n
*				_FILTER_OCT: 0-100; 0->0.f; 100->6.9999f\n
*				_FILTER_Q: 0-100: 0->getFilterMinQ();  100->getFilterMaxQ()\n
*				_FILTER_KBD_TRACK: 0-100\n				
*				_FILTER_BAND:\n
\verbatim
_FILTER_BAND_LPF, _FILTER_BAND_HPF, _FILTER_BAND_BPF, _FILTER_BAND_PASS_ALL\n
\endverbatim
*				FILTER_FREQ_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_FILTER_FREQ_MOD_LFO_LEVEL: 0-100\n
*				_FILTER_FREQ_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_FILTER_FREQ_MOD_ENV_LEVEL: 0-100\n
*
*   @return 0
*/
int mod_synth_filter_event_int(int filtid, int eventid, int val);

/**
*   @brief  Initiates keyboard related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int kbid	target keyboard: _KBD_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KBD_PORTAMENTO_LEVEL, _KBD_SENSITIVITY_LEVEL, _KBD_LOW_SENSITIVITY_LEVEL\n
*				_KBD_SPLIT_POINT\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_KBD_PORTAMENTO_LEVEL: 1-100\n
*				_KBD_SENSITIVITY_LEVEL: 0-100\n
*				_KBD_LOW_SENSITIVITY_LEVEL: 0-100\n
*				_KBD_SPLIT_POINT: _KBD_SPLIT_POINT_NONE, _KBD_SPLIT_POINT_C2, _KBD_SPLIT_POINT_C3\n
*				_KBD_SPLIT_POINT_C4, _KBD_SPLIT_POINT_C5\n
*
*   @return 0
*/
int mod_synth_kbd_event_int(int kbid, int eventid, int val);


/**
*   @brief  Initiates a Karplus-Strong-String generator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int karpid	target generator: _KARPLUS_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM\n
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE\n
*				_KARPLUS_STRONG_STRING_DAMPING, _KARPLUS_STRONG_STRING_DAMPING_VARIATION\n
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM:\n
\verbatim
	_KARPLUS_STRONG_EXCITATION_WHITE_NOISE, _KARPLUS_STRONG_EXCITATION_PINK_NOISE
	_KARPLUS_STRONG_EXCITATION_BROWN_NOISE, _KARPLUS_STRONG_EXCITATION_SINECHIRP
	_KARPLUS_STRONG_EXCITATION_DECAYEDSINE, _KARPLUS_STRONG_EXCITATION_SAWTOOTH_WAVE
	_KARPLUS_STRONG_EXCITATION_SQUARE_WAVE\n
\endverbatim
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE:\n
\verbatim
	_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT, _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING: 0-100;\n 
\verbatim
	0->getKarplusStrongMinStringDumping() 
	100 -> getKarplusStrongMaxStringDumping()
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING_VARIATION: 0-100\n
\verbatim
	0 -> getKarplusStrongMinStringDumpingVariations() 
	100 -> getKarplusStrongMaxStringDumpingVariations()
\endverbatim
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY: 0-100:\n 
\verbatim
	0 -> getKarplusStrongMinDecay()  
	100 -> getKarplusStrongMaxDecay()
\endverbatim
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS: 0-100\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2: 0-100\n
*
*   @return void
*/
int mod_synth_karplus_event_int(int karlplusid, int eventid, int val);

/**
*   @brief  Initiates a Karplus-Strong-String generator related event with boolean value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int karpid	target generator: _KARPLUS_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KARPLUS_STRONG_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				true, false\n
*				
*   @return 0
*/
int mod_synth_karplus_event_bool(int karlplusid, int eventid, bool val);

/**
*   @brief  Initiates a modulator (LFO/ADSR) related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int beqid	target modulator:
\verbatim
	_LFO_1_EVENT, _LFO_2_EVENT, _LFO_3_EVENT, _LFO_4_EVENT, _LFO_5_EVENT
	_ENV_1_EVENT, _ENV_2_EVENT, _ENV_3_EVENT, _ENV_4_EVENT, _ENV_5_EVENT
\endverbatim
*	@param	int modid	specific event code:\n
*				_MOD_ADSR_ATTACK, _MOD_ADSR_DECAY, _MOD_ADSR_SUSTAIN, _MOD_ADSR_RELEASE\n
*				_MOD_LFO_WAVEFORM, _MOD_LFO_RATE, _MOD_LFO_SYMMETRY\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_MOD_ADSR_ATTACK: 0-100; 0 -> 0   100 -> getAdsrMaxAttackTimeSec()\n
*				_MOD_ADSR_DECAY: 0-100; 0 -> 0   100 -> getAdsrMaxDecayTimeSec()\n
*				_MOD_ADSR_SUSTAIN: 0-100; 0 -> 0   100 -> getAdsrMaxSustainTimeSec()\n
*				_MOD_ADSR_RELEASE: 0-100; 0 -> 0   100 -> getAdsrMaxReleaseTimeSec()\n
*				_MOD_LFO_WAVEFORM:\n
\verbatim
	_OSC_WAVEFORM_SINE, _OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_PULSE
	_OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAMPHOLD
\endverbatim
*				_MOD_LFO_RATE: 0-100; 0 -> getLFOminFrequency(); 100 -> getLFOmaxFrequency()\n
*				_MOD_LFO_SYMMETRY: 5-95
*   @return 0
*/
int mod_synth_modulator_event_int(int modid, int eventid, int val);

/**
*   @brief  Initiates a Morphed Sinus Oscilator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int msoid	target mso : _MSO_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MSO_SEGMENT_A_POSITION, _MSO_SEGMENT_B_POSITION, _MSO_SEGMENT_C_POSITION\n
*				_MSO_SEGMENT_D_POSITION, _MSO_SEGMENT_E_POSITION, _MSO_SEGMENT_F_POSITION
*				_MSO_SYMETRY

*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_MSO_SEGMENT_x_POSITION : 0 - _MSO_WAVEFORM_LENGTH - 1 (511)
*				_MSO_SYMETRY : 0 - 100

*				MSO_SEND_1, _NOISE_SEND_2: 0-100\n
*				MSO_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				MSO_AMP_MOD_LFO_LEVEL: 0-100\n

*				MSO_AMP_MOD_ENV:\n
\verbatim
_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, ENV_5
\endverbatim
*				_MSO_AMP_MOD_ENV_LEVEL: 0-100\n
*
*   @return void
*/
int mod_synth_mso_event_int(int msoid, int eventid, int val);

/**
*   @brief  Initiates a Morphed Sinus Oscilator related event with boolean value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int msoid	target mso : _MSO_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MSO_ENABLE
*	@param	boolean val event parameter value true / false:\n*
*	@param	_settings_params_t params	active settings params
*	@param	int program	program number
*   @return void
*/
int mod_synth_mso_event_bool(int msoid, int eventid, bool val);

/**
*   @brief  Initiates a Noise generator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int noiseid	target noise generator: _NOISE_1_EVENT
*	@param	int eventid	specific event code:\n
*				_NOISE_COLOR, _NOISE_SEND_1, _NOISE_SEND_2\n
*				_NOISE_AMP_MOD_LFO, _NOISE_AMP_MOD_ENV
*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_NOISE_COLOR\n
\verbatim
	_WHITE_NOISE, _PINK_NOISE, _BROWN_NOISE
\endverbatim
*				_NOISE_SEND_1, _NOISE_SEND_2: 0-100\n
*				NOISE_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				NOISE_AMP_MOD_LFO_LEVEL: 0-100\n
*				NOISE_AMP_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_NOISE_AMP_MOD_ENV_LEVEL: 0-100\n		
*
*	@param	int program	program number
*   @return 0
*/
int mod_synth_noise_event_int(int noiseid, int eventid, int val);

/**
*   @brief  Initiates a Noise generator related event with booleam value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int noiseid	target noise generator: _NOISE_1_EVENT
*	@param	int eventid	specific event code: _NOISE_ENABLE\n	
*
*	@param	bool val event parameter value  true or false (enable/disable)
*	@param	int program	program number
*   @return 0
*/
int mod_synth_noise_event_bool(int noiseid, int eventid, bool val);

/**
*   @brief  Initiates a PAD related event with integer value (affects all voices).
*			http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int padid	target PAD: _PAD_1_EVENT
*	@param	int eventid	specific event code:\n 
*				_PAD_DETUNE_OCTAVE, _PAD_DETUNE_SEMITONES, _PAD_DETUNE_CENTS\n 
*				_PAD_HARMONY_LEVEL_1, _PAD_HARMONY_LEVEL_2, _PAD_HARMONY_LEVEL_3\n
*				_PAD_HARMONY_LEVEL_4, _PAD_HARMONY_LEVEL_5, _PAD_HARMONY_LEVEL_6\n
*				_PAD_HARMONY_LEVEL_7, _PAD_HARMONY_LEVEL_8, _PAD_DETUNE\n
*				_PAD_FREQ_MOD_LFO, _PAD_FREQ_MOD_LFO_LEVEL\n
*				_PAD_AMP_MOD_LFO, _PAD_AMP_MOD_LFO_LEVEL\n
*				_PAD_QUALITY, _PAD_SHAPE\n
*				_PAD_BASE_NOTE, _PAD_BASE_WIDTH\n
*				_PAD_GENERATE, _PAD_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_PAD_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
*				_PAD_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
*				_PAD_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
*				_PAD_FILTER_SEND_1, _PAD_FILTER_SEND_2: 0-100\n
*				_PAD_FREQ_MOD_LFO, _PAD_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_PAD_FREQ_MOD_LFO_LEVEL, _PAD_AMP_MOD_LFO_LEVEL: 0-100\n
*				_PAD_FREQ_MOD_ENV, _PAD_AMP_MOD_ENV:\n
\verbatim
_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, _ENV_5
\endverbatim
*				_PAD_FREQ_MOD_ENV_LEVEL, _PAD_AMP_MOD_ENV_LEVEL: 0-100\n
*				_PAD_QUALITY:\n 
\verbatim
	_PAD_QUALITY_32K, _PAD_QUALITY_64K, _PAD_QUALITY_128K
	_PAD_QUALITY_256K, _PAD_QUALITY_512K, _PAD_QUALITY_1024K
\endverbatim
*				_PAD_SHAPE:\n
\verbatim
_PAD_SHAPE_RECTANGULAR, _PAD_SHAPE_GAUSSIAN, _PAD_SHAPE_DOUBLE_EXP
\endverbatim
*				_PAD_BASE_NOTE:\n
\verbatim
_PAD_BASE_NOTE_C2, _PAD_BASE_NOTE_G2, _PAD_BASE_NOTE_C3, _PAD_BASE_NOTE_G3
_PAD_BASE_NOTE_C4, _PAD_BASE_NOTE_G4, _PAD_BASE_NOTE_C5, _PAD_BASE_NOTE_G5
_PAD_BASE_NOTE_C6, _PAD_BASE_NOTE_G6
\endverbatim
*				_PAD_BASE_WIDTH: 0-100\n
*				_PAD_GENERATE: (any value)
*				_PAD_DETUNE: 0-100\n
*				_PAD_ENABLE: false, true
*	@param	_settings_params_t params	active settings params
*	@param int program	program number
*
*   @return 0
*/
int mod_synth_pad_event_int(int padid, int eventid, int val);

/**
*   @brief  Initiates a PAD related event with bool value (affects all voices).
*			http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int padid	target PAD: _PAD_1_EVENT
*	@param	int eventid	specific event code: _PAD_ENABLE\n 
*	@param	int val event parameter value true/fale:\n
*
*   @return 0
*/
int mod_synth_pad_event_bool(int padid, int eventid, bool val);

/**
*   @brief  Initiates a reverbration effect related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ROOM_SIZE, _REVERB_DAMP, _REVERB_WET, _REVERB_DRY\n
*				_REVERB_WIDTH, _REVERB_MODE, _REVERB_PRESET
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ROOM_SIZE: 10-98\n
*				_REVERB_DAMP: 0-100\n
*				_REVERB_WET: 0-100\n
*				_REVERB_DRY: 0-100\n
*				_REVERB_WIDTH: 1-100\n
*				_REVERB_MODE: 0-50
*				_REVERB_PRESET: enum sf_reverb_preset (defined in LibAPI/synthesizer.h)\n
*
*   @return 0
*/
int mod_synth_reverb_event_int(int revid, int eventid, int val);

/**
*   @brief  Initiates a reverbration effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE: true - enabled; false - disabled\n
*
*   @return 0
*/
int mod_synth_reverb_event_bool(int revid, int eventid, bool val);

/**
*   @brief  Initiates a VCO related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_PARAM_WAVEFORM, _OSC_PWM_SYMMETRY\n
*				_OSC_DETUNE_OCTAVE, _OSC_DETUNE_SEMITONES, _OSC_DETUNE_CENTS\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2\n
*				_OSC_UNISON_MODE, _OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE  (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_1, _OSC_UNISON_LEVEL_2, _OSC_UNISON_LEVEL_3 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_4, _OSC_UNISON_LEVEL_5, _OSC_UNISON_LEVEL_6 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_7, _OSC_UNISON_LEVEL_8, _OSC_UNISON_LEVEL_9 (valid only for OSC 1)\n
*				_OSC_HAMMOND_PERCUSION_MODE									  (valid only for OSC 1)\n
*				_OSC_FREQ_MOD_LFO, _OSC_FREQ_MOD_LFO_LEVEL\n
*				_OSC_PWM_MOD_LFO, _OSC_PWM_MOD_LFO_LEVEL\n
*				_OSC_AMP_MOD_LFO, _OSC_AMP_MOD_LFO_LEVEL\n
*				_OSC_FREQ_MOD_ENV, _OSC_FREQ_MOD_ENV_LEVEL\n
*				_OSC_PWM_MOD_ENV, _OSC_PWM_MOD_ENV_LEVEL\n
*				_OSC_AMP_MOD_ENV, _OSC_AMP_MOD_ENV_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_OSC_PARAM_WAVEFORM:\n
\verbatim
				_OSC_WAVEFORM_SINE, _OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_PULSE
				_OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAMPHOLD
\endverbatim
*				_OSC_PWM_SYMMETRY: 5-95\n
*				_OSC_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
*				_OSC_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
*				_OSC_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2: 0-100\n
*				_OSC_UNISON_MODE:\n
\verbatim
	_OSC_UNISON_MODE_12345678, _OSC_UNISON_MODE_HAMMOND, _OSC_UNISON_MODE_OCTAVES
	_OSC_UNISON_MODE_C_CHORD, _OSC_UNISON_MODE_Cm_CHORD
	_OSC_UNISON_MODE_C7_CHORD, _OSC_UNISON_MODE_Cm7_CHORD
\endverbatim
*				_OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE: 0-99\n
*				_OSC_UNISON_LEVEL_1 - _OSC_UNISON_LEVEL_9: 0-100\n
*				_OSC_HAMMOND_PERCUSION_MODE:\n
\verbatim
	_HAMMOND_PERCUSION_MODE_OFF, _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST, _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_NORM_FAST, _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST, _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_NORM_FAST
\endverbatim
*				_OSC_FREQ_MOD_LFO, _OSC_PWM_MOD_LFO, _OSC_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_OSC_FREQ_MOD_LFO_LEVEL, _OSC_PWM_MOD_LFO_LEVEL, _OSC_AMP_MOD_LFO_LEVEL: 0-100\n
*				_OSC_FREQ_MOD_ENV, _OSC_PWM_MOD_ENV, _OSC_AMP_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_OSC_FREQ_MOD_ENV_LEVEL, _OSC_PWM_MOD_ENV_LEVEL, _OSC_AMP_MOD_ENV_LEVEL: 0-100\n
*
*   @return 0 is done.
*/
int mod_synth_vco_event_int(int vcoid, int eventid, int val);

/**
*   @brief  Initiates a VCO related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_1_UNISON_SQUARE				(valid only for OSC 1)\n
*				_OSC_SYNC							(valid only for OSC 2)\n
*				_OSC_ENABLE
*	@param	bool val event parameter value  true or false (enable/disable)

*   @return void
*/
int mod_synth_vco_event_bool(int vcoid, int eventid, bool val);


/**
	*   @brief  Enables OSC1.
	*   @param  none
	*   @return void
	*/
void mod_synth_enable_osc1();

/**
*   @brief  Disables OSC1.
*   @param  none
*   @return void
*/
void mod_synth_disable_osc1();

/**
*   @brief  Enables OSC2.
*   @param  none
*   @return void
*/
void mod_synth_enable_osc2();

/**
*   @brief  Disables OSC2.
*   @param  none
*   @return void
*/
void mod_synth_disable_osc2();

/**
	*   @brief  Enables Noise.
	*   @param  none
	*   @return void
	*/
void mod_synth_enable_noise();

/**
*   @brief  Disables Noise.
*   @param  none
*   @return void
*/
void mod_synth_disable_noise();

/**
*   @brief  Enables Karplus.
*   @param  none
*   @return void
*/
void mod_synth_enable_karplus();

/**
*   @brief  Disables Karplus.
*   @param  none
*   @return void
*/
void mod_synth_disable_karplus();


/**
*   @brief  Enables Morphed Sinus Osc.
*   @param  none
*   @return void
*/
void mod_synth_enable_mso();

/**
*   @brief  Disables Morphed Sinus Osc.
*   @param  none
*   @return void
*/
void mod_synth_disable_mso();

/**
*   @brief  Enables PDAsynthesizer.
*   @param  none
*   @return void
*/
void mod_synth_enable_pad_synth();

/**
*   @brief  Disables PDAsynthesizer.
*   @param  none
*   @return void
*/
void mod_synth_disable_pad_synth();


/**
*   @brief  Activate FluidSynth panic cleanup
*   @param  none
*   @return none
*/
void mod_synth_panic_action();

/**
 *   @brief  Activate AdjSynth panic cleanup
 *   @param  none
 *   @return none
 */
void mod_synth_adj_synt_panic_action();




/**
	*   @brief  Returns the value of the active patch AMP 1 level.
	*   @param  none
	*   @return int	 the value of the active patch AMP 1 level.
	*/
int mod_synth_get_active_ch_1_level();
/**
*   @brief  Returns the value of the active patch AMP 1 pan.
*   @param  none
*   @return int	 the value of the active patch AMP 1 pan.
*	[ 0 -> Left  50 -> mid   100 -> Right]
*/
int mod_synth_get_active_ch_1_pan();
/**
*   @brief  Returns the value of the active patch AMP1 pan modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch  AMP1 pan modulation LFO number.
*/
int mod_synth_get_active_ch_1_pan_mod_lfo();
/**
*   @brief  Returns the value of the active patch AMP 2 pan modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch  AMP 2 pan modulation LFO level.
*/
int mod_synth_get_active_ch_1_pan_mod_lfo_level();	
/**
*   @brief  Returns the value of the active  AMP 1 send.
*   @param  none
*   @return int	 the value of the active h AMP 1 sendn. 0-100
*/
int mod_synth_get_active_ch_1_send();

/**
*   @brief  Returns the value of the active patch AMP 2 level.
*   @param  none
*   @return int	 the value of the active patch AMP 2 level.
*/
int mod_synth_get_active_ch_2_level();
/**
*   @brief  Returns the value of the active patch AMP 2 pan.
*   @param  none
*   @return int	 the value of the active patch AMP 2 pan.
*	[ 0 -> Left  50 -> mid   100 -> Right]
*/
int mod_synth_get_active_ch_2_pan();
/**
*   @brief  Returns the value of the active patch AMP 2 pan modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch  AMP 2 pan modulation LFO number.
*/
int mod_synth_get_active_ch_2_pan_mod_lfo();
/**
*   @brief  Returns the value of the active patch AMP 1 pan modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch  AMP 1 pan modulation LFO level.
*/
int mod_synth_get_active_ch_2_pan_mod_lfo_level();
/**
	*   @brief  Returns the value of the active  AMP 2 send.
	*   @param  none
	*   @return int	 the value of the active h AMP 2 send.0-100
	*/
int mod_synth_get_active_ch_2_send();

/**
*   @brief  Returns the value of the active patch distortion enable state.
*   @param  none
*   @return bool	 the value of the active patch distortion enable state.
*/
bool mod_synth_get_active_distortion_enable_state();
/**
*   @brief  Returns the value of the active patch distortion auto gain enable state.
*   @param  none
*   @return bool	 the value of the active patch distortion auto gain enable state.
*/
bool mod_synth_get_active_distortion_auto_gain_state();
/**
*   @brief  Returns the value of the active patch distortion 1 drive level.
*   @param  none
*   @return int	 the value of the active patch distortion 1 drive level.
*/
int mod_synth_get_active_distortion_1_drive();
/**
*   @brief  Returns the value of the active patch distortion 1 range level.
*   @param  none
*   @return int	 the value of the active patch distortion 1 range level.
*/
int mod_synth_get_active_distortion_1_range();
/**
*   @brief  Returns the value of the active patch distortion 1 blend level.
*   @param  none
*   @return int	 the value of the active patch distortion 1 blend level.
*/
int mod_synth_get_active_distortion_1_blend();
/**
*   @brief  Returns the value of the active patch distortion 2 drive level.
*   @param  none
*   @return int	 the value of the active patch distortion 2 drive level.
*/
int mod_synth_get_active_distortion_2_drive();
/**
*   @brief  Returns the value of the active patch distortion 2 range level.
*   @param  none
*   @return int	 the value of the active patch distortion 2 range level.
*/
int mod_synth_get_active_distortion_2_range();
/**
*   @brief  Returns the value of the active patch distortion 2 blend level.
*   @param  none
*   @return int	 the value of the active patch distortion 2 blend level.
*/
int mod_synth_get_active_distortion_2_blend();

/**
*   @brief  Returns the value of the active patch band-equilizer 31Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 31Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band31_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 62Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 62Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band62_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 125Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 125Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band125_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 250Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 250Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band250_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 500Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 500Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band500_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 1KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 1KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band1k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 2KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 2KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band2k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 4KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 4KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band4k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 8KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 8KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band8k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 16KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 16KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band16k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer selected preset (not implemented).
*   @param  none
*   @return int	 the value of the active patch band-equilizer selected preset.
*/
int mod_synth_get_active_equilizer_preset();

/**
*   @brief  Returns the value of the active patch filter 1 frequency.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency.
*	[freq Hz = getFilterMinCenterFreq() + (getFilterMaxCenterFreq() - getFilterMinCenterFreq()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_1_freq();
/**
*   @brief  Returns the value of the active patch filter 1 Octave.
*   @param  none
*   @return int	 the value of the active patch filter 1 octave.
*	[octave = (float)ret value * 6.9999f / 100.0f]
*/
int mod_synth_get_active_filter_1_oct();
/**
*   @brief  Returns the value of the active patch filter 1 Q.
*   @param  none
*   @return int	 the value of the active patch filter 1 Q.
*	[Q = getFilterMinQ() + (getFilterMaxCenterQ() - getFilterMinCenterQ()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_1_q();
/**
*   @brief  Returns the value of the active patch filter 1 keyboard tracking.
*   @param  none
*   @return int	 the value of the active patch filter 1 keyboard tracking.
*/
int mod_synth_get_active_filter_1_kbd_track();
/**
*   @brief  Returns the value of the active patch filter 1  band.
*   @param  none
*   @return int	 the value of the active patch filter 1 band (see defs.h).
*/
int mod_synth_get_active_filter_1_band();

/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation LFO number.
*/
int mod_synth_get_active_filter_1_Freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation LFO level.
*/
int mod_synth_get_active_filter_1_Freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation ENV.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation ENV.
*/
int mod_synth_get_active_filter_1_Freq_mod_env();
/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation ENV level.
*/
int mod_synth_get_active_filter_1_Freq_mod_env_level();

/**
*   @brief  Returns the value of the active patch filter 2 frequency.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency.
*	[freq Hz = getFilterMinCenterFreq() + (getFilterMaxCenterFreq() - getFilterMinCenterFreq()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_2_freq();
/**
*   @brief  Returns the value of the active patch filter 2 Octave.
*   @param  none
*   @return int	 the value of the active patch filter 2 octave.
*	[octave = (float)ret value * 6.9999f / 100.0f]
*/
int mod_synth_get_active_filter_2_oct();
/**
*   @brief  Returns the value of the active patch filter 2 Q.
*   @param  none
*   @return int	 the value of the active patch filter 2 Q.
*	[Q = getFilterMinQ() + (getFilterMaxCenterQ() - getFilterMinCenterQ()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_2_q();
/**
*   @brief  Returns the value of the active patch filter 2 keyboard tracking.
*   @param  none
*   @return int	 the value of the active patch filter 2 keyboard tracking.
*/
int mod_synth_get_active_filter_2_kbd_track();
/**
*   @brief  Returns the value of the active patch filter 2  band.
*   @param  none
*   @return int	 the value of the active patch filter 2 band (see defs.h).
*/
int mod_synth_get_active_filter_2_band();
/**
*   @brief  Returns the state of the active patch filter 2  tracking filter 1.
*   @param  none
*   @return int	 the state of the active patch filter 2  tracking filter 1.
*/
bool mod_synth_get_active_filter_2_Trac_filter_1_state();

/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation LFO number.
*/
int mod_synth_get_active_filter_2_Freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation LFO level.
*/
int mod_synth_get_active_filter_2_Freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation ENV.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation ENV.
*/
int mod_synth_get_active_filter_2_Freq_mod_env();
/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation ENV level.
*/
int mod_synth_get_active_filter_2_Freq_mod_env_level();

/**
	*   @brief  Returns the value of the active patch Karplus-strong generator excitation waveform.
	*   @param  none
	*   @return int	 the value of the active patch Karplus-strong generator excitation waveform (see defs.h).
	*/
int mod_synth_get_active_karplus_excitation_waveform_type();

/**
	*   @brief  Returns the value of the active patch Karplus-strong generator excitation waveform variations level.
	*   @param  none
	*   @return int	 the value of the active patch Karplus-strong generator excitation waveform variations level(see defs.h).
	*/
int mod_synth_get_active_karplus_excitation_waveform_variations();

/**
*   @brief  Returns the value of the active patch Karplus-strong generator string damping calculation mode.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator string damping calculation mode (see defs.h).
*/
int mod_synth_get_active_karplus_string_damping_calculation_mode();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator string damping.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator string damping.\n
*	[damping = getKarplusStrongMinStringDamping()+ret value/100.0f*(getKarplusStrongMinStringDamping()-getKarplusStrongMaxStringDamping())]
*/
int mod_synth_get_active_karplus_string_damping();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator pluck damping.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator pluck damping.\n
*	[damping = getKarplusStrongMinPluckDamping()+ret value/100.0f*(getKarplusStrongMinPluckDamping()-getKarplusStrongMaxPluckDamping())]
*/
int mod_synth_get_active_karplus_pluck_damping();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-on decay factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-on decay factor.\n
*	[decay = getKarplusStrongMinStringDecay()+ret value/100.0f*(getKarplusStrongMinStringDecay()-getKarplusStrongMaxStringDecay())]
*/
int mod_synth_get_active_karplus_string_on_decay();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-off decay factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-off decay factor.\n
*	[decay = getKarplusStrongMinStringDecay()+ret value/100.0f*(getKarplusStrongMinStringDecay()-getKarplusStrongMaxStringDecay())]
*/
int mod_synth_get_active_karplus_string_off_decay();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-on string damping variations factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-on string damping variations factor.\n
*	[string damping variation factor = \n
*	getKarplusStrongMinStringDampingVariations()+ret value/100.0f*(getKarplusStrongMaxStringDampingVariations()-getKarplusStrongMinStringDampingVariations())]
*/
int mod_synth_get_active_karplus_string_damping_variation();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-on pluck damping variations factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-on pluck damping variations factor.\n
*	[pluckdamping variation factor = \n
*	getKarplusStrongMinPluckDampingVariations()+ret value/100.0f*(getKarplusStrongMaxPluckDampingVariations()-getKarplusStrongMinPluckDampingVariations())]
*/
int mod_synth_get_active_karplus_pluck_damping_variation();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator send filter 1 level.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator send filter 1 level.
*/
int mod_synth_get_active_karplus_send_filter_1();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator send filter 2 level.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator send filter 2 level.
*/
int mod_synth_get_active_karplus_send_filter_2();







/**
*   @brief  Returns the value of the active patch portamento time.
*   @param  none
*   @return int	 the value of the active patch portamento time.
*/
int mod_synth_get_active_keyboard_portamento_level();
/**
*   @brief  Returns the value of the active patch portamento enable state.
*   @param  none
*   @return bool	 the value of the active patch portamento enable state.
*/
bool mod_synth_get_active_keyboard_portamento_enabled_state();
	
/**
*   @brief  Returns the value of the active patch keyboard sensitivity.\n
*	Affects all keyboard if no keyboard-split is set.\n
*	Affects only upper keyboard-split when keyboard-split is set.
*   @param  none
*   @return int	 the value of the active patch keyboard sensitivity.
*/
int mod_synth_get_active_keyboard_sensitivity();
/**
*   @brief  Returns the value of the active patch low-keyboard sensitivity.\n
*	Has no effect when keyboard-split is not set.\n
*	Affects only lower keyboard-split when keyboard-split is set.
*   @param  none
*   @return int	 the value of the active patch lower-keyboard sensitivity.
*/
int mod_synth_get_active_keyboard_sensitivity_low();
/**
*   @brief  Returns the value of the active patch keyboard split point.\n
*   @param  none
*   @return int	 the value of the active patch keyboard split point.
*/
int mod_synth_get_active_keyboard_split_point();
/**
*   @brief  Returns the value of the active patch keyboard poly mode.
*   @param  none
*   @return bool	 the value of the active patch keyboard poly mode (see defs.h).
*/
int mod_synth_get_active_keyboard_poly_mode();

/**
*   @brief  Returns the value of the active Midi mixer setting parameters.
*   @param  paramid	parameter id: _MIXER_CHAN_n_LEVEL, _MIXER_CHAN_n_PAN, _MIXER_CHAN_n_SEND (n:1-16)
*   @return int	 the value of the active patch band-equilizer selected preset.
*/
int mod_synth_get_active_midi_mixer_setting_param(int paramid);

/**
	*   @brief  Returns the value of the active patch ADSR 1 attack time.
	*   @param  none
	*   @return int	 the value of the active patch ADSR 1 attack time.
	*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
	*/
int mod_synth_get_active_env_mod_1_attack();
/**
*   @brief  Returns the value of the active patch ADSR 1 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 1 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_1_decay();
/**
*   @brief  Returns the value of the active patch ADSR 1 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 1 sustain level.
*/
int mod_synth_get_active_env_mod_1_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 1 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 1 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_1_release();
	
/**
*   @brief  Returns the value of the active patch ADSR 2 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_2_attack();
/**
*   @brief  Returns the value of the active patch ADSR 2 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_2_decay();
/**
*   @brief  Returns the value of the active patch ADSR 2 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 sustain level.
*/
int mod_synth_get_active_env_mod_2_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 2 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_2_release();
	
/**
*   @brief  Returns the value of the active patch ADSR 3 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_3_attack();
/**
*   @brief  Returns the value of the active patch ADSR 3 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_3_decay();
/**
*   @brief  Returns the value of the active patch ADSR 3 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 sustain level.
*/
int mod_synth_get_active_env_mod_3_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 3 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_3_release();

/**
*   @brief  Returns the value of the active patch ADSR 4 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_4_attack();
/**
*   @brief  Returns the value of the active patch ADSR 4 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_4_decay();
/**
*   @brief  Returns the value of the active patch ADSR 4 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 sustain level.
*/
int mod_synth_get_active_env_mod_4_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 4 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_4_release();

/**
*   @brief  Returns the value of the active patch ADSR 5 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_5_attack();
/**
*   @brief  Returns the value of the active patch ADSR 5 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_5_decay();
/**
*   @brief  Returns the value of the active patch ADSR 5 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 sustain level.
*/
int mod_synth_get_active_env_mod_5_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 5 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_5_release();

/**
*   @brief  Returns the value of the active patch ADSR 6 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 6 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_6_attack();
/**
*   @brief  Returns the value of the active patch ADSR 6 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 6 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_6_decay();
/**
*   @brief  Returns the value of the active patch ADSR 6 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 6 sustain level.
*/
int mod_synth_get_active_env_mod_6_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 6 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 6 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_6_release();

/**
*   @brief  Returns the value of the active patch LFO 1 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 1 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_1_waveform();
/**
*   @brief  Returns the value of the active patch LFO 1 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 1 symetry in %.
*/
int mod_synth_get_active_lfo_mod_1_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 1 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 1 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_1_rate();
	
/**
*   @brief  Returns the value of the active patch LFO 2 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 2 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_2_waveform();
/**
*   @brief  Returns the value of the active patch LFO 2 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 2 symetry in %.
*/
int mod_synth_get_active_lfo_mod_2_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 2 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 2 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_2_rate();
	
/**
*   @brief  Returns the value of the active patch LFO 3 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 1 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_3_waveform();
/**
*   @brief  Returns the value of the active patch LFO 3 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 3 symetry in %.
*/
int mod_synth_get_active_lfo_mod_3_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 3 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 3 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_3_rate();

/**
*   @brief  Returns the value of the active patch LFO 4 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 4 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_4_waveform();
/**
*   @brief  Returns the value of the active patch LFO 4 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 4 symetry in %.
*/
int mod_synth_get_active_lfo_mod_4_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 4 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 4 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_4_rate();

/**
*   @brief  Returns the value of the active patch LFO 5 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 5 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_5_waveform();
/**
*   @brief  Returns the value of the active patch LFO 5 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 5 symetry in %.
*/
int mod_synth_get_active_lfo_mod_5_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 5 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 5 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_5_rate();

/**
*   @brief  Returns the value of the active patch LFO 6 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 6 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_6_waveform();
/**
*   @brief  Returns the value of the active patch LFO 6 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 6 symetry in %.
*/
int mod_synth_get_active_lfo_mod_6_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 6 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 6 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_6_rate();

/**
*   @brief  Returns an active patch MSO lookup table segment point.
*   @param  int  requested point (0-5)
*   @return int	the an active patch MSO lookup table segment point.
*/
int mod_synth_get_active_mso_table_segment_point(int point);
/**
*   @brief  Returns an active patch MSO symetry value.
*   @param  none
*   @return int	the an active patch MSO symetry value.
*/
int mod_synth_get_active_mso_symetry();
/**
*   @brief  Returns the value of the active patch MSO pwm %.
*   @param  none
*   @return int	 the value of the active patch MSO pwm %.
*/
int mod_synth_get_active_mso_pwm_percents();
/**
*   @brief  Returns the value of the active patch MSO detune octave value.
*   @param  none
*   @return int	 the value of the active patch MSO detune octave value\n.
*	[octave detune = return value + getOscDetuneMinOctave()]
*/
int mod_synth_get_active_mso_detune_octave();
/**
*   @brief  Returns the value of the active patch MSO detune semitones value.
*   @param  none
*   @return int	 the value of the active patch MSO detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_mso_detune_semitones();
/**
*   @brief  Returns the value of the active patch MSO detune cents value.
*   @param  none
*   @return int	 the value of the active patch MSO detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_mso_detune_cents();
/**
*   @brief  Returns the value of the active patch MSO send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch MSO send level to filter 1.
*/
int mod_synth_get_active_mso_send_filter_1();
/**
*   @brief  Returns the value of the active patch MSO send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch MSO send level to filter 2.
*/
int mod_synth_get_active_mso_send_filter_2();
/**
*   @brief  Returns the value of the active patch MSO sync on OSC1 state.
*   @param  none
*   @return bool	 the value of the active patch MSO sync on OSC1 state.
*/
//bool getActiveMso1SyncIsOnState();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation LFO number.
*/
int mod_synth_get_active_mso_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation LFO level.
*/
int mod_synth_get_active_mso_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation ENV number.
*/
int mod_synth_get_active_mso_freq_mod_env();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation ENV level.
*/
int mod_synth_get_active_mso_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation LFO number.
*/
int mod_synth_get_active_mso_pwm_mod_lfo();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation LFO level.
*/
int mod_synth_get_active_mso_pwm_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation ENV number.
*/
int mod_synth_get_active_mso_pwm_mod_env();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation ENV level.
*/
int mod_synth_get_active_mso_pwm_mod_env_level();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation LFO number.
*/
int mod_synth_get_active_mso_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation LFO level.
*/
int mod_synth_get_active_mso_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation ENV number.
*/
int mod_synth_get_active_mso_amp_mod_env();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation ENV level.
*/
int mod_synth_get_active_mso_amp_mod_env_level();

/**
*   @brief  Returns the value of the active patch Noise color.
*   @param  none
*   @return int	 the value of the active patch  Noise color (see defs.h).
*/
int mod_synth_get_active_noise_color();
/**
*   @brief  Returns the value of the active patch Noise send filter 1 level.
*   @param  none
*   @return int	 the value of the active patch  Noise send filter 1 level.
*/
int mod_synth_get_active_noise_send_filter_1();
/**
*   @brief  Returns the value of the active patch Noise send filter 2 level.
*   @param  none
*   @return int	 the value of the active patch  Noise send filter 2 level.
*/
int mod_synth_get_active_noise_send_filter_2();

/**
*   @brief  Returns the value of the active patch Noise amp modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation LFO number.
*/
int mod_synth_get_active_noise_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch Noise amp modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation LFO level.
*/
int mod_synth_get_active_noise_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch Noise amp modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation ENV number.
*/
int mod_synth_get_active_noise_amp_mod_env();
/**
*   @brief  Returns the value of the active patch Noise amp modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation ENV level.
*/
int mod_synth_get_active_noise_amp_mod_env_level();

/**
	*   @brief  Returns the value of the active patch PAD detune octave value.
	*   @param  none
	*   @return int	 the value of the active patch PAD detune octave value\n.
	*	[octave detune = return value + getOscDetuneMinOctave()]
	*/
int mod_synth_get_active_pad_detune_octave();
/**
*   @brief  Returns the value of the active patch PAD detune semitones value.
*   @param  none
*   @return int	 the value of the active patch PAD detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_pad_detune_semitones();
/**
*   @brief  Returns the value of the active patch PAD detune cents value.
*   @param  none
*   @return int	 the value of the active patch PAD detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_pad_detune_cents();
/**
*   @brief  Returns the value of the active patch PAD send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch PAD send level to filter 1.
*/
int mod_synth_get_active_pad_send_filter_1();
/**
*   @brief  Returns the value of the active patch PAD send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch PAD send level to filter 2.
*/
int mod_synth_get_active_pad_send_filter_2();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation LFO number.
*/
int mod_synth_get_active_pad_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation LFO level.
*/
int mod_synth_get_active_pad_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation ENV number.
*/
int mod_synth_get_active_pad_freq_mod_env();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation ENV level.
*/
int mod_synth_get_active_pad_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation LFO number.
*/
int mod_synth_get_active_pad_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation LFO level.
*/
int mod_synth_get_active_pad_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation ENV number.
*/
int mod_synth_get_active_pad_amp_mod_env();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation ENV level.
*/
int mod_synth_get_active_pad_amp_mod_env_level();
/**
*   @brief  Returns the value of the active patch PAD quality factor.
*   @param  none
*   @return int	 the value of the active patch PAD quality factor.
*/
int mod_synth_get_active_pad_quality();
/**
*   @brief  Returns the value of the active patch PAD shape.
*   @param  none
*   @return int	 the value of the active patch PAD shape.
*/
int mod_synth_get_active_pad_shape();
/**
*   @brief  Returns the value of the active patch PAD shape-cutoff mode.
*   @param  none
*   @return int	 the value of the active patch PAD shape-cutoff mode.
*/
int mod_synth_get_active_pad_shape_cutoff();
/**
*   @brief  Returns the value of the active patch PAD base note.
*   @param  none
*   @return int	 the value of the active patch PAD base note.
*/
int mod_synth_get_active_pad_base_note();
/**
*   @brief  Returns the value of the active patch PAD base width.
*   @param  none
*   @return int	 the value of the active patch PAD abase width.
*/
int mod_synth_get_active_pad_base_width();
/**
*   @brief  Returns the value of the active patch PAD harmony level.
*   @param  int harmony: 0-7
*   @return int	 the value of the active patch PAD harmony level.
*/
int mod_synth_get_active_pad_harmony_level(int harmony);
/**
*   @brief  Returns the value of the active PAD harmonic-detune level.
*   @param  none
*   @return int	 the value of the active patch PAD harmonic-detune level.
*/
int mod_synth_get_active_pad_harmony_detune();

/**
*   @brief  Returns the value of the active patch reverbration enable state.
*   @param  none
*   @return bool	 the value of the active patch reverbration enable state.
*/
bool mod_synth_get_active_reverb_enable_state();
/**
*   @brief  Returns the value of the active patch reverbration room size value.
*   @param  none
*   @return int	 the value of the active patch reverbration room size value.
*/
int mod_synth_get_active_reverb_room_size();
/**
*   @brief  Returns the value of the active patch reverbration damp value.
*   @param  none
*   @return int	 the value of the active patch reverbration damp value.
*/
int mod_synth_get_active_reverb_damp();
/**
*   @brief  Returns the value of the active patch reverbration wet value.
*   @param  none
*   @return int	 the value of the active patch reverbration wet value.
*/
int mod_synth_get_active_reverb_wet();
/**
*   @brief  Returns the value of the active patch reverbration dry value.
*   @param  none
*   @return int	 the value of the active patch reverbration dry value.
*/
int mod_synth_get_active_reverb_dry();
/**
*   @brief  Returns the value of the active patch reverbration width value.
*   @param  none
*   @return int	 the value of the active patch reverbration width value.
*/
int mod_synth_get_active_reverb_width();
/**
*   @brief  Returns the value of the active patch reverbration mode value.
*   @param  none
*   @return int	 the value of the active patch reverbration mode value.
*/
int mod_synth_get_active_reverb_mode();

/**
*   @brief  Returns the value of the active patch 3M reverbration enable state.
*   @param  none
*   @return bool	 the value of the active patch 3M reverbration enable state.
*/
bool mod_synth_get_active_reverb_3m_enable_state();
/**
*   @brief  Returns the value of the active patch 3M reverbration preset.
*   @param  none
*   @return int	 the value of the active patch 3M reverbration preset (see defs.h).
*/
int mod_synth_get_active_reverb_3m_preset();
/**
*   @brief  Returns the value of the active patch distortion selected preset (not implemented).
*   @param  none
*   @return int	 the value of the active patch distortion preset.
*/
int mod_synth_get_active_reverb_3m_preset();

/**
*   @brief  Returns the the active patch OSC1 enable state.
*   @param  none
*   @return bool	 the active patch OSC1 enable state.
*/
bool mod_synth_get_active_osc1_enable_state();
/**
*   @brief  Returns the the active patch OSC2 enable state.
*   @param  none
*   @return bool	 the active patch OSC2 enable state.
*/
bool mod_synth_get_active_osc2_enable_state();
/**
*   @brief  Returns the the active patch Noise enable state.
*   @param  none
*   @return bool	 the active patch Noise enable state.
*/
bool mod_synth_get_active_noise_enable_state();
/**
*   @brief  Returns the the active patch Karplus enable state.
*   @param  none
*   @return bool	 the active patch Karplus enable state.
*/
bool mod_synth_get_active_karplus_enable_state();
/**
*   @brief  Returns the the active patch MSO enable state.
*   @param  none
*   @return bool	 the active patch MSO enable state.
*/
bool mod_synth_get_active_mso_enable_state();
/**
*   @brief  Returns the the active patch PAD enable state.
*   @param  none
*   @return bool	 the active patch PAD enable state.
*/
bool mod_synth_get_active_pad_enable_state();

/**
*   @brief  Returns the value of the active patch OSC1 waveform.
*   @param  none
*   @return int	 the value of the active patch OSC1 waveform (see defs.h).
*/
int mod_synth_get_active_osc1_waveform();
/**
*   @brief  Returns the value of the active patch OSC1 pwm %.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm %.
*/
int mod_synth_get_active_osc1_pwm_percents();
/**
*   @brief  Returns the value of the active patch OSC1 detune octave value.
*   @param  none
*   @return int	 the value of the active patch OSC1 detune octave value\n.
*	[octave detune = return value + getOscDetuneMinOctave()]
*/
int mod_synth_get_active_osc1_detune_octave();
/**
*   @brief  Returns the value of the active patch OSC1 detune semitones value.
*   @param  none
*   @return int	 the value of the active patch OSC1 detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_osc1_detune_semitones();
/**
*   @brief  Returns the value of the active patch OSC1 detune cents value.
*   @param  none
*   @return int	 the value of the active patch OSC1 detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_osc1_detune_cents();
/**
*   @brief  Returns the value of the active patch OSC1 send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch OSC1 send level to filter 1.
*/
int mod_synth_get_active_osc1_send_filter_1();
/**
*   @brief  Returns the value of the active patch OSC1 send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch OSC1 send level to filter 2.
*/
int mod_synth_get_active_osc1_send_filter_2();
/**
*   @brief  Returns the value of the active patch OSC1 unison mode.
*   @param  none
*   @return int	 the value of the active patch OSC1 unison mode (see defs.h).
*/
int mod_synth_get_active_osc1_unison_mode();
/**
*   @brief  Returns the value of the active patch OSC1 hammond-organ mode.
*   @param  none
*   @return int	 the value of the active patch OSC1 hammond-organ mode (see defs.h).
*/
int mod_synth_get_active_osc1_hammond_mode();
/**
*   @brief  Returns the value of the active patch OSC1 unison harmony level.
*   @param  int harnum		0-7 (0-8 in Hamond unisom mode)
*   @return int	 the value of the active patch OSC1 hammond-organ mode (see defs.h).
*/
int mod_synth_get_active_osc1_harmonies_levels(int harnum);
/**
*   @brief  Returns the value of the active patch OSC1 harmonies detune level.
*   @param  none
*   @return int	 the value of the active patch OSC1 harmonies detune level.
*/
int mod_synth_get_active_osc1_harmonies_detune_factor();
/**
*   @brief  Returns the value of the active patch OSC1 harmonies distortion level.
*   @param  none
*   @return int	 the value of the active patch OSC1 harmonies distortion level.
*/
int mod_synth_get_active_osc1_harmonies_dist_factor();

/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation LFO number.
*/
int mod_synth_get_active_osc1_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation LFO level.
*/
int mod_synth_get_active_osc1_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation ENV number.
*/
int mod_synth_get_active_osc1_freq_mod_env();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation ENV level.
*/
int mod_synth_get_active_osc1_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation LFO number.
*/
int mod_synth_get_active_osc1_pwm_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation LFO level.
*/
int mod_synth_get_active_osc1_pwm_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation ENV number.
*/
int mod_synth_get_active_osc1_pwm_mod_env();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation ENV level.
*/
int mod_synth_get_active_osc1_pwm_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation LFO number.
*/
int mod_synth_get_active_osc1_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation LFO level.
*/
int mod_synth_get_active_osc1_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation ENV number.
*/
int mod_synth_get_active_osc1_amp_mod_env();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation ENV level.
*/
int mod_synth_get_active_osc1_amp_mod_env_level();

/**
*   @brief  Returns the value of the active patch OSC2 waveform.
*   @param  none
*   @return int	 the value of the active patch OSC2 waveform (see defs.h).
*/
int mod_synth_get_active_osc2_waveform();
/**
*   @brief  Returns the value of the active patch OSC2 pwm %.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm %.
*/
int mod_synth_get_active_osc2_pwm_percents();
/**
*   @brief  Returns the value of the active patch OSC2 detune octave value.
*   @param  none
*   @return int	 the value of the active patch OSC2 detune octave value\n.
*	[octave detune = return value + getOscDetuneMinOctave()]
*/
int mod_synth_get_active_osc2_detune_octave();
/**
*   @brief  Returns the value of the active patch OSC2 detune semitones value.
*   @param  none
*   @return int	 the value of the active patch OSC2 detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_osc2_detune_semitones();
/**
*   @brief  Returns the value of the active patch OSC2 detune cents value.
*   @param  none
*   @return int	 the value of the active patch OSC2 detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_osc2_detune_cents();
/**
*   @brief  Returns the value of the active patch OSC2 send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch OSC2 send level to filter 1.
*/
int mod_synth_get_active_osc2_send_filter_1();
/**
*   @brief  Returns the value of the active patch OSC2 send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch OSC2 send level to filter 2.
*/
int mod_synth_get_active_osc2_send_filter_2();
/**
*   @brief  Returns the value of the active patch OSC2 sync on OSC1 state.
*   @param  none
*   @return bool	 the value of the active patch OSC2 sync on OSC1 state.
*/
bool mod_synth_get_active_osc2_sync_is_on_state();
/**
*   @brief  Returns the value of the active patch OSC2 fixed tone state (Not implemented).
*   @param  none
*   @return bool	 the value of the active patch OSC2 ixed tone state.
*/
bool mod_synth_get_active_osc2_fix_tone_is_on_state();

/**
*   @brief  Returns the value of the active patch OSC2 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC2 frequency modulation LFO number.
*/
int mod_synth_get_active_osc2_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation LFO level.
*/
int mod_synth_get_active_osc2_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC2 frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC2 frequency modulation ENV number.
*/
int mod_synth_get_active_osc2_freq_mod_env();
/**
*   @brief  Returns the value of the active patch OSC2 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC2 frequency modulation ENV level.
*/
int mod_synth_get_active_osc2_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation LFO number.
*/
int mod_synth_get_active_osc2_pwm_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation LFO level.
*/
int mod_synth_get_active_osc2_pwm_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation ENV number.
*/
int mod_synth_get_active_osc2_pwm_mod_env();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation ENV level.
*/
int mod_synth_get_active_osc2_pwm_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation LFO number.
*/
int mod_synth_get_active_osc2_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation LFO level.
*/
int mod_synth_get_active_osc2_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation ENV number.
*/
int mod_synth_get_active_osc2_amp_mod_env();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation ENV level.
*/
int mod_synth_get_active_osc2_amp_mod_env_level();





/**
*   @brief  Returns the maximum value of oscilator octave detune.
*   @param  none
*   @return int	the maximum value of oscilator octave detune.
*/
int mod_synth_get_osc_detune_max_octave();
/**
*   @brief  Returns the minimum value of oscilator octave detune.
*   @param  none
*   @return int	the minimum value of oscilator octave detune.
*/
int mod_synth_get_osc_detune_min_octave();
/**
*   @brief  Returns the maximum value of oscilator semitones detune.
*   @param  none
*   @return int	the maximum value of oscilator semitones detune.
*/
int mod_synth_get_osc_detune_max_semitones();
/**
*   @brief  Returns the minimum value of oscilator semitones detune.
*   @param  none
*   @return int	the maximum value of oscilator Semitones detune.
*/
int mod_synth_get_osc_detune_min_semitones();
/**
*   @brief  Returns the maximum value of oscilator cents detune.
*   @param  none
*   @return float	the maximum value of oscilator cents detune.
*/
float mod_synth_get_osc_detune_max_cents();
/**
*   @brief  Returns the minimum value of oscilator cents detune.
*   @param  none
*   @return float	the minimum value of oscilator cents detune.
*/
float mod_synth_get_osc_detune_min_cents();
/**
*   @brief  Returns the number of OSC1 drawbars.
*   @param  none
*   @return int	 the number of OSC1 drawbars.
*/
int mod_synth_get_osc1_num_of_drawbars();
/**
*   @brief  Returns the maximum value of ADSR attack time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR attack time in seconds.
*/
float mod_synth_get_adsr_max_attack_time_sec();
/**
*   @brief  Returns the maximum value of ADSR fecay time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR decay time in seconds.
*/
float mod_synth_get_adsr_max_decay_time_sec();
/**
*   @brief  Returns the maximum value of ADSR release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR release time in seconds.
*/
float mod_synth_get_adsr_max_release_time_sec();
/**
*   @brief  Returns the maximum value of ADSR forced-release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR forced-release time in seconds.
*/
float mod_synth_get_adsr_max_forced_release_time_sec();

/**
*   @brief  Returns the minimum value of filter frequency in Hz.
*   @param  none
*   @return float	the minimum value of filter frequency in Hz.
*/
float mod_synth_get_filter_min_center_freq();
/**
*   @brief  Returns the maximum value of filter frequency in Hz.
*   @param  none
*   @return float	the maximum value of filter frequency in Hz.
*/
float mod_synth_get_filter_max_center_freq();
/**
*   @brief  Returns the minimum value of filter Q.
*   @param  none
*   @return float	the minimum value of filter Q.
*/
float mod_synth_get_filter_min_Q();
/**
*   @brief  Returns the maximum value of filter Q.
*   @param  none
*   @return float	the maximum value of filter Q.
*/
float mod_synth_get_filter_max_Q();
/**
*   @brief  Returns the maximum value of the Karplus-Strong generator String Damping factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong generator String Damping factor.
*/
float mod_synth_get_karplus_strong_max_string_damping();
/**
*   @brief  Returns the minimum value of the Karplus-Strong generator String Damping factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong generator String Damping factor.
*/
float mod_synth_get_karplus_strong_min_string_damping();
/**
*   @brief  Returns the maximum value of the Karplus-Strong generator Pluck Damping factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong generator Pluck Damping factor.
*/
float mod_synth_get_karplus_strong_max_pluck_damping();
/**
*   @brief  Returns the minimum value of the Karplus-Strong generator Pluck Damping factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong generator Pluck Damping factor.
*/
float mod_synth_get_karplus_strong_min_pluck_damping();
/**
*   @brief  Returns the maximum value of the Karplus-Strong generator decay time.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong generator decay time.
*/
float mod_synth_get_karplus_strong_max_decay();
/**
*   @brief  Returns the minimum value of the Karplus-Strong generator decay time.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong generator decay time.
*/
float mod_synth_get_karplus_strong_min_decay();
/**
*   @brief  Returns the maximum value of the Karplus-Strong String damping variations factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong String damping variations factor.
*/
float mod_synth_get_karplus_strong_max_pluck_damping_variations();
/**
*   @brief  Returns the minimum value of the Karplus-Strong Pluck damping variations factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong Pluck damping variations factor.
*/
float mod_synth_get_karplus_strong_min_pluck_damping_variations();

/**
*   @brief  Returns the maximum value of the Karplus-Strong Pluck damping variations factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong Pluck damping variations factor.
*/
float mod_synth_get_karplus_strong_max_string_damping_variations();
/**
*   @brief  Returns the minimum value of the Karplus-Strong String damping variations factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong String damping variations factor.
*/
float mod_synth_get_karplus_strong_min_string_damping_variations();


/**
*   @brief  Returns a pointer to the PAD synth base harmony shape vector.
*   @param  None.
*   @return float*  a pointer to the PAD synth base harmony shape vector
*/
float *mod_synth_get_pad_base_harmony_profile();
/**
*   @brief  Returns the PAD synth base harmony shape vector number of elements.
*   @param  None.
*   @return int  the PAD synth base harmony shape vector  number of elements.
*/
int mod_synth_get_pad_base_harmony_profile_size();
/**
*   @brief  Returns a pointer to the PAD synth spectrum vector.
*   @param  None.
*   @return float*  a pointer to the PAD synth spectrum vector
*/
float *mod_synth_get_pad_spectrum();
/**
*   @brief  Returns the PAD synth spectrum vector number of elements.
*   @param  None.
*   @return int  the PAD synth spectrum vector  number of elements.
*/
int mod_synth_get_pad_spectrum_size();







/**
*   @brief  Returns the synthesizer maximum number of polyiphonic voices.
*   @param  none
*   @return int	maximum number of polyiphonic voices.
*/
int mod_synth_get_synthesizer_max_num_of_polyphonic_voices();

/**
*   @brief  Returns the synthesizer enabled number of programs.
*   @param  none
*   @return int	enabled number of programs.
*/
int mod_synth_get_synthesizer_max_num_of_programs();

/**
	*   @brief  Returns the minimum value of filter frequency in Hz.
	*   @param  none
	*   @return float	the minimum value of filter frequency in Hz.
	*/
float mod_synth_get_filter_min_center_freq();
/**
*   @brief  Returns the maximum value of filter frequency in Hz.
*   @param  none
*   @return float	the maximum value of filter frequency in Hz.
*/
float mod_synth_get_filter_max_center_freq();
/**
*   @brief  Returns the minimum value of filter Q.
*   @param  none
*   @return float	the minimum value of filter Q.
*/
float mod_synth_get_filter_min_Q();
/**
*   @brief  Returns the maximum value of filter Q.
*   @param  none
*   @return float	the maximum value of filter Q.
*/
float mod_synth_get_filter_max_Q();
/**
	*   @brief  Returns the maximum value of ADSR attack time in seconds.
	*   @param  none
	*   @return float	the maximum value of ADSR attack time in seconds.
	*/
float mod_synth_get_adsr_max_attack_time_sec();
/**
*   @brief  Returns the maximum value of ADSR fecay time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR decay time in seconds.
*/
float mod_synth_get_adsr_max_decay_time_sec();
/**
*   @brief  Returns the maximum value of ADSR release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR release time in seconds.
*/
float mod_synth_get_adsr_max_release_time_sec();
/**
*   @brief  Returns the maximum value of ADSR forced-release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR forced-release time in seconds.
*/
float mod_synth_get_adsr_max_forced_release_time_sec();

/**
*   @brief  Returns the minimum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the minimum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_min_frequency();
/**
*   @brief  Returns the maximum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the maximum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_max_frequency();

/**
*   @brief  Returns the MSO lookup table length.
*   @param  none
*   @return int	the the MSO lokkup table length.
*/
int mod_synth_get_mso_table_length();
/**
*   @brief  Returns a pointer to the the MSO lookup table.
*   @param  none
*   @return float*	ta pointer to the the MSO lookup table.
*/
float *mod_synth_get_mso_morphed_lookup_table();

/**
*   @brief  Returns the maximum value of the keyboard portamento time in seconds.
*   @param  none
*   @return float	the minimum value of the keyboard portamento time in seconds.
*/
float mod_synth_get_portamento_max_time_sec();

/**
*   @brief  Returns the minimum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the minimum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_min_frequency();
/**
*   @brief  Returns the maximum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the maximum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_max_frequency();

/**
*   @brief  Returns the instrument type.
*   @param  en_instruments_ids_t  instrument id enum  LibAPI/types.h
*   @return en_instruments_types instrument type en_instruments_types::none_instrument_type if key string not found.
*/
en_instruments_types_t mod_synth_get_instrument_type(en_instruments_ids_t inst_id);








/**
*   @brief  Register a callback function that initiates a full GUI update.
*			May handle multiple registrations.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
//void mod_synth_register_callback_update_ui(func_ptr_void_void_t ptr);

/**
*   @brief  Register a callback function that closes an instrument pannel on the main window.
*   @param  func_ptr_void_en_instruments_ids_t_t  a pointer to the callback function 
*													( void func(en_instruments_ids_t) ) (instrument id)
*   @return void
*/
void mod_synth_register_callback_wrapper_close_instrument_pannel_id(func_ptr_void_en_instruments_ids_t_t ptr);

/**
*   @brief  Register a callback function that closes an instrument on the main window.
*   @param  func_ptr_void_string_t  a pointer to the callback function 
*													( void func(std::string) ) (instrument name)
*   @return void
*/
void mod_synth_register_callback_wrapper_close_instrument_pannel_name(func_ptr_void_string_t ptr);

/**
*   @brief  Register a callback function that opens an instrument on the main window.
*   @param  func_ptr_void_string_t  a pointer to the callback function 
*													( void func(std::string) ) (instrument name)
*   @return void
*/
void mod_synth_register_callback_wrapper_open_instrument_pannel_name(func_ptr_void_string_t ptr);
