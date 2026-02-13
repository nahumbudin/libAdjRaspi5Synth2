/**
* @file		connections.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		Provides the ALSA and JackAudio connections API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

/**
*   @brief  Returns a list of the active ALSA MIDI input clients.
*			(use only port 0)
*   @param  none
*   @return 0 if done; -1 if params out of range;
*/
std::list<std::string> mod_synth_get_midi_input_client_name_strings();

/**
*   @brief  Connects an ALSA MIDI input client to an instrument MIDI input ALSA port.
*			(use only port 0)
*   @param  std::string instrument_name a constant string presenting the instrument name
*   @param  connect		if true connect; if false disconnect
*   @parm	use_index: if true
*	@param  in_client	the input client index on the detected midi-clients list.
*   @param  in_port		the input client port index on the detected midi-clients list.
*   @parm	use_index: if false
*	@param  in_client	the input client number.
*   @param  in_port		the input client port number.
*   
*   @return 0 if done
*/
int mod_synth_connect_midi_clients(std::string instrument_name, int in_client, int in_port, bool connect, bool use_index=true);

/**
*	@brief	Refresh all ALSA clients and connections data
*	@param	none
*	@return 0
*/
int mod_synth_refresh_alsa_clients_data();

/**
*   @brief  Returns the client num in the connections list.
*   @param  client name
*	@return con_num		connection num (0 to num_of_connections-1). 
*/
int mod_synth_get_midi_client_connection_num(std::string instrument_name);

/**
*   @brief  Returns the selected MIDI iput client id.
*   @param  string	client name.
*   @param	bool	replace	if true, use virtual clients given name (default),
*							if false, use virtual clients system given name
*	@return void
*/
int mod_synth_get_midi_input_client_id(string name, bool replace = true);


/**
*   @brief  Returns a list of clients numbers (@connection) that connects to a client.
*   @param  cln_num	the connected to client number
*	@param	pointer to alist of clients numbers (@connection) that connects to a client. 
*	@return	num of clients found
*/
int mod_synth_get_midi_client_connected_to_clients_numbers_list(int cln_num, std::list<int> *cln_lst);

/**
*   @brief  Returns a list of clients names that connects to a client.
*   @param  cln_num	the connected to client number
*	@param	pointer to alist of clients names that connects to a client. 
*	@return	num of clients found
*/
int mod_synth_get_midi_client_connected_to_clients_names_list(int cln_num, bool replace,
															  std::list<std::string> *cln_lst);


/**
*   @brief  Returns the number of jack outputs.
*   @param  none
*   @return 0 if done
*/
int mod_synth_get_num_of_output_jack_clients();

/**
*   @brief  Returns the name of JACK output client.
*   @param  cln client index.
*   @return int the number of an JACK output client name; "" if param out of range.
*/
std::string mod_synth_get_jack_output_client_name(int cln);

/**
*   @brief  Returns the number of JACK output client ports.
*   @param  cln client index.
*   @return int the number of an JACK output clients ports; -1 if param out of range.
*/
int mod_synth_get_num_of_jack_output_client_ports(int cln);

/**
*   @brief  Returns the name of JACK output client port.
*   @param  cln client index.
*   @param	prt port index
*   @return int the number of an JACK output client name; "" if param out of range.
*/
std::string mod_synth_get_jack_output_client_port_name(int cln, int prt);

/**
*   @brief  Returns the number of jack inputs.
*   @param  none
*   @return 0 if done
*/
int mod_synth_get_num_of_input_jack_clients();

/**
*   @brief  Returns the name of JACK intput client.
*   @param  cln client index.
*   @return int the number of an JACK input client name; "" if param out of range.
*/
std::string mod_synth_get_jack_input_client_name(int cln);

/**
*   @brief  Returns the number of JACK input client ports.
*   @param  cln client index.
*   @return int the number of an JACK input clients ports; -1 if param out of range.
*/
int mod_synth_get_num_of_jack_input_client_ports(int cln);

/**
*   @brief  Returns the name of JACK input client port.
*   @param  cln client index.
*   @param	prt port index
*   @return int the number of an JACK input client name; "" if param out of range.
*/
std::string mod_synth_get_jack_input_client_port_name(int cln, int prt);

/**
*   @brief  Connects Jack input and output clients/ports.
*   @param  in_client_name			input client name
*   @param  in_client_port name		input client port name
*   @param  out_client_name			output client name
*   @param  out_client_port name	output client port name
*   @param  connect		if true connect; if false disconnect
*	@return 0 if done; -1 otherwise 
*/
int mod_synth_connect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name,
	bool connect);

/**
*	@brief	Refresh all JACK clients and connections connections data
*	@param	none
*	@return 0
*/
int mod_synth_refresh_jack_clients_data();

/**
*	@brief	Returns a list of Jack input <"Client:Port> that connects to the given output client:port 
*	@param	client name
*	@param	client port name
*	@param	pointer to a list of strings
*	@return number of connections in list.
*/
int mod_synth_get_jack_input_connections(std::string out_client_name, std::string out_client_port_name,
										 std::list<std::string> *outputs_list);
