/**
* @file		instrument.cpp
*	@author		Nahum Budin
*	@date		11-Apr-2026
*	@version	2.0	
*					1. Specific instruments settings are handled by each instrument.
*					2. 
*					   
*					
*	@brief		The basic music generating object, e.g., analog-synthesizer,
*				FluidSynth SoundFont synthesizer, organ, etc.
*	
*	History:\n
*				versio 1.1	  24-09-2025
*					1. Adding a pointer to a AdjSynth object
*					2. Splitting the settings parameters to two groups: active_preset_settings_params and active_common_settings_params,
*					3. Splitting active_preset_settings_params to two groups: active_program_settings_params and active_common_settings_params, 
*					   used for the AdjSynth setting params and adding a active_fluid_synth_settings_params for the FluidSynth settings params
*				version 1.0		24-06-2024: First version
*	
*/

#include "instrument.h"
#include "../utils/xmlFiles.h"
#include "../MIDI/midiStream.h"

/* Holds the BT input client name. */
std::string AlsaMidiSysControl::bt_client_in_name = "";
/* Holds the control box input client name.*/
std::string AlsaMidiSysControl::control_box_client_in_name = "";
/* Holds the control box external MIDI interface input client name. */
std::string AlsaMidiSysControl::control_box_xt_midi_in_client_name = "";
/* Holds the MIDI playerr input client name. */
std::string AlsaMidiSysControl::midi_player_client_in_name = "";
/* Holds the MIDI mapper input clent name. */
std::string AlsaMidiSysControl::midi_mapper_client_in_name = "";

/**
*   @brief  Creates an Instrument object
*   @param  string				name				the instrument name
*   @param	bool				with_midi_in		if true, the instrument have a midi in interface
*   @param	bool				with_audio_out		if true, the instrument have an audio output interface
*	@param	bool				with_midi_out		if true, the instrument have a midi output port
*	@param	AlsaMidiSysControl	alse_control		a pointer to an ALSA control object
*	@param	string				alsa_client_in_name	the name of the ALSA input client.
*	@param	AdjSynth			*adj_synth			a pointer to the main AdjSynth object
*   @return 0 if done
*/
Instrument::Instrument(std::string name, bool with_midi_in,
					   bool with_audio_out, bool with_midi_out,
					   AlsaMidiSysControl *alsa_control,
					   std::string *alsa_client_in_name,
					   AdjSynth *adj_synth)
{
	instrument_name = name;

	midi_in_enable = with_midi_in;
	midi_out_enable = with_midi_out;
	audio_out_enable = with_audio_out;

	//	instrument_connections_control = new InstrumentConnectionsControl();

	if (midi_in_enable)
	{
		alsa_connections = AlsaMidiSysControl::get_instance();
	}

	if (audio_out_enable)
	{
		// Curenly ALSA audio is not supported TODO:
		jack_connections = JackConnections::get_instance();
	}

	if (midi_out_enable)
	{
		const char *portname = "virtual";
		int status;

		mode = SND_RAWMIDI_SYNC;

		if ((status = snd_rawmidi_open(NULL, &midiout, portname, mode)) < 0)
		{
			printf("Problem opening %s Alsa output: %s", name.c_str(), snd_strerror(status));
			//		exit(1);
		}
		else
		{			
			if ((alsa_control != NULL) && (alsa_client_in_name != NULL))
			{
				// Scan for current ALSA In Instrument client
				alsa_control->refresh_alsa_clients_data();
				// The last client name is of theis instrumet just created client.
				int last_input_client_num = alsa_control->get_num_of_input_midi_clients() - 1;
				alsa_control->get_midi_input_client_name_string(last_input_client_num,
																alsa_client_in_name);
			}
		}
	}

	init();
}

Instrument::~Instrument()
{
	
}

int Instrument::init()
{
	if (midi_in_enable)
	{
		alsa_midi_sequencer_input_client = new AlsaMidiSequencerInputClient(instrument_name, &alsa_rx_queue);
		alsa_connections->refresh_alsa_clients_data();
		alsa_input_client_id = alsa_connections->get_midi_output_client_id(instrument_name);

		alsa_midi_sequencer_events_handler = new AlsaMidiSeqencerEventsHandler(0, &alsa_rx_queue);
		alsa_midi_sequencer_events_handler->set_instrument(this);
	}

	//active_preset_settings_params = new _settings_params_t(); // TODO: presets or active_settings_param?
	//active_common_settings_params = new _settings_params_t(); // TODO: presets or active_settings_param?
	
	active_settings_params = new _settings_params_t(); 

	//instrument_settings = new Settings(active_preset_settings_params); // TODO: presets or active_settings_param?
	instrument_settings_manager = new Settings(active_settings_params);

	return 0;
}

/**
*   @brief  Save the active instrument preset parameters as XML file
*   @param  path preset XML file path. The file name is derived from the instrument name and the path, 
*			for example: /home/user/presets/Adj-Synth-Settings.xml
*	@type	type is used to identify the settings type in the XML file, for example "midi-mixer-settings", "adj-synth-settings", etc.
*   @return 0 if done
*   */
int Instrument::save_instrument_active_settings(string path, string type)
{
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);
	
	int res = 0;
	string settings_file_full_path;
	
	settings_file_full_path = path + "/" + instrument_name + "-settings.html";

	res = instrument_settings_manager->write_settings_file(
		active_settings_params,
		instrument_settings_manager->get_settings_version(),
		instrument_name + "-settings",
		path,
		type);
	
	return res;
}

/**
*   @brief  Read instrument settings parameters from an XML file
*   @param  path preset XML file path. The file name is derived from the instrument name and the path, 
*			for example: /home/user/presets/Adj-Synth-Settings.xml
*	@type	type is used to identify the settings type in the XML file, for example "midi-mixer-settings", "adj-synth-settings", etc.
*   @return 0 if done
*   */
int Instrument::read_instrument_settings(_settings_params_t *preset, string path, string type)
{
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);
	
	int res = 0;
	string settings_file_full_path;
	
	settings_file_full_path = path + "/" + instrument_name + "-settings";

	res = instrument_settings_manager->read_settings_file(
		preset,
		settings_file_full_path,
		type, 
		0); // no channel

	return res;
}

string Instrument::get_instrument_name()
{
	return instrument_name;
}

void Instrument::register_ui_update_callback(func_ptr_void_void_t ptr)
{
	update_ui_callback_ptr = ptr;
}

void Instrument::activate_ui_update_callback()
{
	if (update_ui_callback_ptr)
	{
		update_ui_callback_ptr();
	}
}

void Instrument::set_active_midi_channels(uint16_t act_chans){

	if (midi_in_enable)
	{
		// A bit map that indicates which channel is allocated to this instrument.
		alsa_midi_sequencer_events_handler->set_active_midi_channels(act_chans);
	}
}

uint16_t Instrument::get_active_midi_channels()
{
	if (midi_in_enable)
	{
		return alsa_midi_sequencer_events_handler->get_active_midi_channels();
	}
	else
	{
		return 0;
	}
}

/************************************ 
 * Virtual interfaces
 ************************************/

void Instrument::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void Instrument::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void Instrument::change_program_handler(uint8_t channel, uint8_t program)
{
}

void Instrument::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void Instrument::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	
}

void Instrument::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void Instrument::sysex_handler(uint8_t *message, int len)
{
}

void Instrument::control_box_events_handler(int event_id, uint16_t event_value)
{
	
}

int Instrument::set_default_settings_parameters()
{

	return 0;
}

int Instrument::set_instrument_settings()
{
	
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	int val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with float value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	double val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, float val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with bool value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	bool val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with string value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	string val event parameter value 
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{

	return 0;
}
