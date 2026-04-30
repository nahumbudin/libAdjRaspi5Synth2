/**
* @file		patch.cpp
*	@author		Nahum Budin
*	@date		22-Sep-2025
*	@version	1.1
*					1. Code refactoring rename modules to instruments
*					2. Implementation: init connections before opening instruments as some instruments initialization are utilizing MIDI messages
*	
*	@brief		Modular synthersizer patches handling.
*
*	History: 
*				Ver 1.0  29-Aug-2024	Initial
*/

#include <filesystem>
#include <iostream>
#include <list>

#include "patches.h"

#include "../modSynth.h"
#include "../Instrument/instrumentsManager.h"
#include "../Instrument/instrumentFluidSynth.h"
#include "../Instrument/instrumentAnalogSynth.h"
#include "../Instrument/instrumentMidiMixer.h"
#include "../Instrument/instrumentAnalogReverbration.h"
#include "../Instrument/instrumentMidiMapper.h"
#include "../Instrument/instrumentHammondOrgan.h"
#include "../utils/utils.h"
#include "../LibAPI/connections.h"
#include "../LibAPI/defines.h"
#include "../LibAPI/fluidSynth.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"


using namespace rapidjson;
using namespace std;


PatchsHandler *PatchsHandler::patch_handler_instance = NULL;

PatchsHandler::PatchsHandler(){

	callback_get_active_instruments_names_list_ptr = NULL;
}

PatchsHandler::~PatchsHandler()
{
	
	
}

PatchsHandler *PatchsHandler::get_patchs_handler_instance()
{
	if (patch_handler_instance == NULL)
	{
		patch_handler_instance = new PatchsHandler();
	}

	return patch_handler_instance;
}

/**
*   @brief Save patch parametrs as a json file.
*   @param  file path	patch file full path string
*	@return 0 if done; -1 otherwise 
*	
* 	{
*		"name": "patch name string",
*		"patches" : [
*			{
*				"name": "instrument  1 name string id",
*				"settings_file: "instrument 1 settings.html file name",
*				"connections": [
*					{ "midi input connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					},
*					{ "midi output connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					},
*					{ "jack audio output connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					},
*					{ "jack audio input connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					} ],
*			},
*			{
*				"name": "instrument  2 name string id",
*				"settings_file: "instrument 2 settings.html file name",
*				"connections": [
*				
*			}]
*				
*	}
*/
int PatchsHandler::save_patch_file(std::string file_path)
{
	vector<string> instruments_names;
	instruments_names.reserve(_MAX_NUM_OF_INSTRUMENTS);
	instruments_names.resize(_MAX_NUM_OF_INSTRUMENTS);

	list<std::string> connected_to_midi_clients_names_list;
	
	int instrument_num = 1;
	int client_number;
	string json_str;
	list<string>::iterator names_it;

	s_jack_connection_t jack_connection;

	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();

	if (callback_get_active_instruments_names_list_ptr != NULL)
	{
		// Get a list of only the active (oppened) instruments
		instruments_names = callback_get_active_instruments_names_list_ptr();
	}

	// Create and save a current settings file for each instrument 
	// instrument name + "-settings.html"
	filesystem::path fullpath(file_path);
	// `remove_filename()` does not alter `fullpath`
	string path_without_filename = fullpath.remove_filename();

	if (instruments_names.size() > 0)
	{
		StringBuffer s;
		/* JSON writer */
		Writer<StringBuffer> writer(s);

		// Create and store all active instrument settings files
		create_active_instruments_settings_files(instruments_names, path_without_filename);

		// Between StartObject()/EndObject(),
		writer.StartObject();

		writer.Key("patch_name");
		writer.String(filesystem::path(file_path).stem().c_str());

		writer.Key("instruments");
		writer.StartArray();

		for (string instrument_name : instruments_names)
		{
			// Get instrument Midi Input connections
			client_number = mod_synth_get_midi_client_connection_num(instrument_name);
			connected_to_midi_clients_names_list.clear();
			mod_synth_get_midi_client_connected_to_clients_names_list(client_number, true,
																	  &connected_to_midi_clients_names_list);

			writer.StartObject();

			writer.Key("instrument_name");
			writer.String(instrument_name.c_str());

			writer.Key("settings_file");
			writer.String((path_without_filename + instrument_name.c_str() + "-settings.html").c_str());

			writer.Key("connections");
			writer.StartArray();

			writer.StartObject();
			writer.Key("midi_input_connections");
			writer.StartArray();

			if (connected_to_midi_clients_names_list.size() > 0)
			{
				names_it = connected_to_midi_clients_names_list.begin();
				for (int i = 0; i < connected_to_midi_clients_names_list.size(); i++)
				{
					writer.String(names_it->c_str());
					advance(names_it, 1);
				}
			}

			writer.EndArray(); // midi_input_connections
			writer.EndObject();

			writer.StartObject();
			writer.Key("midi_output_connections");
			writer.StartArray();

			// TODO:

			writer.EndArray(); // midi_output_connections
			writer.EndObject();

			writer.StartObject();
			writer.Key("jack_audio_input_connections");
			writer.StartArray();

			// TODO:

			writer.EndArray(); // jack_audio_input_connections
			writer.EndObject();			

			writer.StartObject();
			writer.Key("jack_audio_right_output_connections");
			writer.StartArray();

			//jack_connection =
			//	ModSynth::get_instance()->get_analog_synth()->get_analog_synth_right_jack_output_connection();

			if (instrument_name == _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY)
			{
				jack_connection =
					ModSynth::get_instance()->get_analog_synth()->get_analog_synth_right_jack_output_connection();
			}
			else if (instrument_name == _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY)
			{
				jack_connection =
					ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_right_jack_output_connection();
			}
			else
			{
				jack_connection.in_client_name = "";
				jack_connection.in_client_port_name = "";
				jack_connection.out_client_name = "";
				jack_connection.out_client_port_name = "";			
			}

			writer.String(jack_connection.in_client_name.c_str());
			writer.String(jack_connection.in_client_port_name.c_str());
			writer.String(jack_connection.out_client_name.c_str());
			writer.String(jack_connection.out_client_port_name.c_str());

			writer.EndArray(); // Jack Out Right
			writer.EndObject();

			writer.StartObject();
			writer.Key("jack_audio_left_output_connections");
			writer.StartArray();

			//jack_connection =
			//	ModSynth::get_instance()->get_analog_synth()->get_analog_synth_left_jack_output_connection();

			if (instrument_name == _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY)
			{
				jack_connection =
					ModSynth::get_instance()->get_analog_synth()->get_analog_synth_left_jack_output_connection();
			}
			else if (instrument_name == _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY)
			{
				jack_connection =
					ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_left_jack_output_connection();
			}
			else
			{
				jack_connection.in_client_name = "";
				jack_connection.in_client_port_name = "";
				jack_connection.out_client_name = "";
				jack_connection.out_client_port_name = "";
			}

			writer.String(jack_connection.in_client_name.c_str());
			writer.String(jack_connection.in_client_port_name.c_str());
			writer.String(jack_connection.out_client_name.c_str());
			writer.String(jack_connection.out_client_port_name.c_str());

			writer.EndArray(); // Jack Out
			writer.EndObject();

			writer.EndArray(); // connections

			writer.EndObject();

			instrument_num++;
		}

		writer.EndArray();

		writer.EndObject();
		json_str = s.GetString();

		return write_text_file(file_path, json_str);
	}

	return -1;
}

/**
*   @brief Load patch parametrs as a json file.
*   @param  file path	patch file full path string
*	@return 0 if done; -1 otherwise 
*/	
int PatchsHandler::load_patch_file(std::string file_path)
{
	vector<char> file_data;
	int res;
	
	/* A JSON document */
	Document document;
	
	string patch_name;
	string instrument_name;

	/* list of active instruments */
	vector<string> active_instruments_names; 
	/* list of instruments settings files in the same order of the instruments above */
	vector<string> settings_files;
	/* lists of list of midi input intrefaces names.
	 * Each active instrument is represented as a vector.
	 * An empty vector represents no midi input interfaces. */	
	vector<vector<string>> midi_input_connections; 

	string interface;
	vector<string> midi_input_interfaces;

	s_jack_connection_t jack_connection;
	vector<s_jack_connection_t> jack_right_output_connections_vector;
	vector<s_jack_connection_t> jack_left_output_connections_vector;

	res = read_text_file(file_path, &file_data);
	if ((res == 0) && (file_data.size() > 0))
	{
		char json[file_data.size() + 1];

		for (int i = 0; i < file_data.size(); i++)
		{
			json[i] = file_data.at(i);
		}

		// Termination
		json[file_data.size()] = 0;

		// Try parsing the JSON text
		if (document.Parse(json).HasParseError())
		{
			return -1;
		}
		
		// Document is a JSON value that represents the root of DOM (Document Object Model). 
		// Root (top level element) can be either an object or array.
		assert(document.IsObject()); 

		for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
		{	
			// iterate through document
			// make object value
			const Value &obj_name = document[itr->name.GetString()]; 
			// printf("name %s\n", itr->name.GetString());
			if (strcmp(itr->name.GetString(), "patch_name") == 0)
			{
				patch_name = itr->value.GetString();	
			}
			else if (strcmp(itr->name.GetString(), "instruments") == 0)
			{
				// instruments array
				for (SizeType i = 0; i < itr->value.Size(); i++)
				{
					if (itr->value[i].IsObject())
					{
						midi_input_interfaces.clear();

						//jack_right_output_connections_vector.clear();
						//jack_left_output_connections_vector.clear();
						
						const Value &m = itr->value[i];
						for (auto &v : m.GetObject())
						{ 
							// iterate through instruments array objects
							if (m[v.name.GetString()].IsString())
							{
								if (strcmp(v.name.GetString(), "instrument_name") == 0)
								{
									active_instruments_names.push_back(v.value.GetString());
								}
								else if (strcmp(v.name.GetString(), "settings_file") == 0)
								{
									settings_files.push_back(v.value.GetString());
								}
							}
							
							if (m[v.name.GetString()].IsArray())
							{
								// Connections array
								const Value &a = m[v.name.GetString()];
								for (SizeType j = 0; j < a.Size(); j++) {
									if(a[j].IsObject()) 
									{
										for (auto &c : a[j].GetObject())
										{
											string name = c.name.GetString();
											if (c.value.IsArray())
											{
												const Value &k = c.value;
												for (SizeType t = 0; t < k.Size(); t++) 
												{
													if (name == "midi_input_connections")
													{
														interface = k[t].GetString();
														midi_input_interfaces.push_back(interface);
													}
													else if (name == "midi_output_connections")
													{
														; // TODO:
													}
													else if (name == "jack_audio_input_connections")
													{
														; // TODO:
													}
													else if (name == "jack_audio_right_output_connections")
													{
														switch (t)
														{
															case 0:
																jack_connection.in_client_name = k[t].GetString();
																break;
															case 1:
																jack_connection.in_client_port_name = k[t].GetString();
																break;
															case 2:
																jack_connection.out_client_name = k[t].GetString();
																break;
															case 3:
																jack_connection.out_client_port_name = k[t].GetString();
																jack_right_output_connections_vector.push_back(jack_connection);	
															break;
														}
														
														
													}
													else if (name == "jack_audio_left_output_connections")
													{
														switch (t)
														{
															case 0:
																jack_connection.in_client_name = k[t].GetString();
																break;
															case 1:
																jack_connection.in_client_port_name = k[t].GetString();
																break;
															case 2:
																jack_connection.out_client_name = k[t].GetString();
																break;
															case 3:
																jack_connection.out_client_port_name = k[t].GetString();
																jack_left_output_connections_vector.push_back(jack_connection);	
																break;
														}
													}
												} 
											}
										}
									}	
								} // end of iterating on connections
							}
						}
					}

					midi_input_connections.push_back(midi_input_interfaces);
					
				} // end of iterations over each module
				
			} // end of modules object	
		}

		if (active_instruments_names.size() > 0)
		{
			// Open and activate the instruments and set the connections
			implement_patch(
				active_instruments_names, 
				settings_files, 
				midi_input_connections, 
				jack_right_output_connections_vector,
				jack_left_output_connections_vector,
				file_path);
		}

		return active_instruments_names.size();
	}

	return -1;
}

void PatchsHandler::register_callback_get_active_instruments_names_list(func_ptr_vector_std_string_void_t ptr)
{
	callback_get_active_instruments_names_list_ptr = ptr;
}

std::string PatchsHandler::get_jack_client_name_for_instrument(const std::string &instrument_name)
{
	if (instrument_name == _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY)
	{
		return "Adj-Analog-Synth";
	}
	else if (instrument_name == _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY)
	{
		return "Adj-Fluid-Synth";
	}
	else
	{
		return "";
	}
}

int PatchsHandler::close_current_oppened_instruments()
{
	vector<string> instruments_names;

	// Get a list of only the active (oppened) instruments
	instruments_names = callback_get_active_instruments_names_list_ptr();

	for (int m = 0; m < instruments_names.size(); m++)
	{
		// Disconnect Connections
		// TODO: call next function?
		
		// Close instruments
		ModSynth::get_instance()->instruments_manager->close_instrument_pannel_name(instruments_names.at(m));
	}

	return 0;
}


int PatchsHandler::disconnect_current_oppened_instruments_midi_in_connections()
{
	vector<string> instruments_names;
	list<int> connected_to_midi_clients_numbers_list;
	int module_num = 1;
	int client_id;
	int client_number;

	// Refresh all connections data.
	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();

	if (callback_get_active_instruments_names_list_ptr != NULL)
	{
		// Get a list of only the active (oppened) instruments
		instruments_names = callback_get_active_instruments_names_list_ptr();
	}

	if (instruments_names.size() > 0)
	{
		for (const string& instrument_name : instruments_names)
		{
			// Get module Midi Input connections
			int client_number = mod_synth_get_midi_client_connection_num(instrument_name);
			connected_to_midi_clients_numbers_list.clear();
			mod_synth_get_midi_client_connected_to_clients_numbers_list(client_number,
																	  &connected_to_midi_clients_numbers_list);

			if (connected_to_midi_clients_numbers_list.size() > 0)
			{
				for (int client_num : connected_to_midi_clients_numbers_list)
				{
					// disconnect, use client num
					mod_synth_connect_midi_clients(instrument_name, client_num, 0, false, false); 
				}
			}

			// Midi outputs
			
			// TODO:
		}
	}
}

/**
*   @brief Create settings HTML files of all active instruments.
*   @param  inst_name			a vector of instruments names strings
*   @param	patch_file_path		the patch directory path
*	@return 0 if done; -1 otherwise 
*/	
int PatchsHandler::create_active_instruments_settings_files(vector<string> inst_names, string patch_file_path)
{
	string settings_file_path;
	

	for (string instrument_name : inst_names)
	{
		settings_file_path = patch_file_path + instrument_name + "-settings.html";
		
		if (instrument_name == _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY)
		{
			//settings_file_path = patch_file_path + instrument_name + "-settings.html";
			mod_synth_save_fluid_synth_preset_file(settings_file_path);
		}
		else if (instrument_name == _INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY)
		{
			mod_synth_save_hammond_organ_patch_file(settings_file_path);
		}
		else if (instrument_name == _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY)
		{
			//settings_file_path = patch_file_path + instrument_name + "-settings.html";
			mod_synth_save_adj_synth_patch_file(settings_file_path);
		}
		else if (instrument_name == _INSTRUMENT_NAME_KARPLUS_STRONG_STRING_SYNTH_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_MORPHED_SINUS_SYNTH_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_PADSYNTH_SYNTH_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_MIDI_MIXER_STR_KEY)
		{
			mod_synth_save_midi_mixer_patch_file(settings_file_path);
		}
		else if (instrument_name == _INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_REVERB_STR_KEY)
		{
			mod_synth_save_analog_reverberation_patch_file(settings_file_path);
		}
		else if (instrument_name == _INSTRUMENT_NAME_DISTORTION_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_GRAPHIC_EQUALIZER_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_EXT_MIDI_INT_CONTROL_STR_KEY)
		{
			// TODO:
		}
		else if (instrument_name == _INSTRUMENT_NAME_KEYBOARD_CONTROL_STR_KEY)
		{
			// TODO:
		}
	}

	return 0;
}

/**
*   @brief Open all patch instruments and set all connections.
*   @param  active_instruments	a vector of all active instruments names strings
*   @param	setting_files		a vector of all active instruments settings files
*   @param	midi_in_connections a vector of all active instruments midi in connections
*   @param	file_path			the patch directory path
*	@return 0 if done; -1 otherwise 
*	
*	TODO: add other connections
*/
int PatchsHandler::implement_patch(vector<string> active_instruments, vector<string> settings_files,
								   vector<vector<string>> midi_in_connections,
								   vector<s_jack_connection_t> jack_right_out_connections_vector,
								   vector<s_jack_connection_t> jack_left_out_connections_vector,
								   string file_path)
{
	string settings_file_path;
	vector<string> midi_input_connections;
	string midi_in_connection;
	int client_num;
	int res = 0;
	_settings_int_param_t value;
	
	/* Set connections BEFORE or AFTER ? all modules are oppened as some instruments initialization are utilizing MIDI messages */
	
	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();

	for (int m = 0; m < active_instruments.size(); m++)
	{
		// Midi In Connections
		midi_input_connections = midi_in_connections.at(m);
		if (midi_input_connections.size() > 0)
		{
			for (int c = 0; c < midi_input_connections.size(); c++)
			{
				midi_in_connection = midi_input_connections.at(c);
				// replace virtual clients names
				client_num = mod_synth_get_midi_input_client_id(midi_in_connection, true); 
				// connect, use index
				mod_synth_connect_midi_clients(active_instruments.at(m), client_num, 0, true, true); 
			}
		}

		// Jack Audio Out Connections
		if (jack_right_out_connections_vector.size() > 0)
		{
			for (s_jack_connection_t jack_connection : jack_right_out_connections_vector)
			{
				mod_synth_connect_jack_connection(
					jack_connection.in_client_name,
					jack_connection.in_client_port_name,
					jack_connection.out_client_name,
					jack_connection.out_client_port_name,
					true); // Conect
			}
		}

		if (jack_left_out_connections_vector.size() > 0)
		{
			for (s_jack_connection_t jack_connection : jack_left_out_connections_vector)
			{
				mod_synth_connect_jack_connection(
					jack_connection.in_client_name,
					jack_connection.in_client_port_name,
					jack_connection.out_client_name,
					jack_connection.out_client_port_name,
					true); // Conect
			}
		}
	}

	for (int m = 0; m < active_instruments.size(); m++)
	{
		// Open the instruments pannels.
		ModSynth::get_instance()->instruments_manager->open_instrument_pannel_name(active_instruments.at(m));
		// Handle settings files
		if (settings_files.at(m) != "")
		{
			settings_file_path = filesystem::path(file_path).parent_path().c_str();
			settings_file_path += "/" + active_instruments.at(m) + "-settings.xml";

			if (active_instruments.at(m) == _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY)
			{
				ModSynth::get_instance()->get_fluid_synth()->open_fluid_synth_preset_file(
					settings_file_path,
					ModSynth::get_instance()->get_fluid_synth()->active_settings_params,
					ModSynth::get_instance()->get_fluid_synth()->presets_summary_str);
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY)
			{
				ModSynth::get_instance()->get_analog_synth()->instrument_settings_manager->read_settings_file(
					AdjSynth::get_instance()->get_active_preset_params(),
					settings_file_path,
					_ADJ_SYNTH_PRESET_PARAMS,
					AdjSynth::get_instance()->get_active_sketch()); 			
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY)
			{
				ModSynth::get_instance()->get_hammond_organ()->instrument_settings_manager->read_settings_file(
					ModSynth::get_instance()->get_hammond_organ()->active_settings_params,
					settings_file_path,
					"hammond-organ-preset",
					_HAMMOND_ORGAN_PROGRAM_20);
				
				// Update the Leslie speed and level.
				// Go over all allocated MIDI channels.
				for (int chan = 0; chan < 16; chan++)
				{
					if (ModSynth::get_instance()->get_hammond_organ()->get_active_midi_channels() & (1 << chan))
					{
						res = ModSynth::get_instance()->get_hammond_organ()->instrument_settings_manager->get_int_param(
							ModSynth::get_instance()->get_hammond_organ()->active_settings_params,
							"adjsynth.hammond.organ_leslie_speed",
							&value);
						
						if (res == _SETTINGS_KEY_FOUND)
						{
							mod_synth_midi_mixer_set_channel_pan_mod_lfo(chan, _LFO_6);
							mod_synth_modulator_event_int(_LFO_6_EVENT, _MOD_LFO_RATE, value.value);
						}
						
						res = ModSynth::get_instance()->get_hammond_organ()->instrument_settings_manager->get_int_param(
							ModSynth::get_instance()->get_hammond_organ()->active_settings_params,
							"adjsynth.hammond.organ_leslie_level",
							&value);
						
						if (res == _SETTINGS_KEY_FOUND)
						{
		
							mod_synth_midi_mixer_set_channel_pan_mod_level(chan, value.value);
						}
					}
				}
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_KARPLUS_STRONG_STRING_SYNTH_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_MORPHED_SINUS_SYNTH_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_PADSYNTH_SYNTH_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_MIDI_MIXER_STR_KEY)
			{
				//ModSynth::get_instance()->get_midi_mixer()->open_midi_mixer_presets_file(
				//	settings_file_path,
				//	ModSynth::get_instance()->get_midi_mixer()->active_settings_params,
				//	&ModSynth::get_instance()->get_midi_mixer()->presets_summary_str[0]);
				
				ModSynth::get_instance()->get_midi_mixer()->read_instrument_settings(
					ModSynth::get_instance()->get_midi_mixer()->active_settings_params,
					settings_file_path,
					"midi-mixer-preset");				
				
				// Set the midi instrument params
				ModSynth::get_instance()->get_midi_mixer()->set_instrument_settings(ModSynth::get_instance()->get_midi_mixer()->active_settings_params); 
	
				ModSynth::get_instance()->get_midi_mixer()->update_midi_mixer_gui();
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_REVERB_STR_KEY)
			{
				ModSynth::get_instance()->get_analog_reverberation()->read_instrument_settings(
					ModSynth::get_instance()->get_analog_reverberation()->active_settings_params,
					settings_file_path,
					"analog-reverb-preset");
				
				//ModSynth::get_instance()->get_analog_reverberation()->set_instrument_settings(
				//	ModSynth::get_instance()->get_analog_reverberation()->active_settings_params);
				
				ModSynth::get_instance()->get_analog_reverberation()->update_analog_reverberation_gui();
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_DISTORTION_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_GRAPHIC_EQUALIZER_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_EXT_MIDI_INT_CONTROL_STR_KEY)
			{
				
			}
			else if (active_instruments.at(m) == _INSTRUMENT_NAME_KEYBOARD_CONTROL_STR_KEY)
			{
				//ModSynth::get_instance()->get_keyboard_control()->open_adj_synth_patch_file(settings_file_path);
			}
		}
	}
	
	return 0;
}
