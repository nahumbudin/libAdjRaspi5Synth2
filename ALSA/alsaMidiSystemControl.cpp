/**
*	@file		alsaMidiSystemControl.h
*	@author		Nahum Budin
*	@date		7-Jul-2024
*	@version	1.1
*					1.	Multiple instances - not a singleton
*					2.  Code refactoring
*	
*	@brief		Use system "aconnect" commands to scan and control ALSA midi connections.		
*				
*	History:\n
*	
*		version 1.0		6-Mar-2021
*/

#include "alsaMidiSystemControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "../utils/utils.h"
#include "../LibAPI/defines.h"
#include "alsaMidiSystemControl.h"

AlsaMidiSysControl *AlsaMidiSysControl::alsa_midi_sys_control_instance = NULL;

AlsaMidiSysControl *AlsaMidiSysControl::get_instance()
{
	if (alsa_midi_sys_control_instance == NULL)
	{
		alsa_midi_sys_control_instance = new AlsaMidiSysControl();
	}

	return alsa_midi_sys_control_instance;
}

AlsaMidiSysControl::AlsaMidiSysControl()
{
	// int res;

	//refresh_alsa_clients_data();
}

/**
*	@brief	Initilizes clients data
*	@param	none
*	@return 0
*/
int AlsaMidiSysControl::init_clients_data()
{
	input_clients_data.data_type = _CLIENTS_TYPE_DATA_INPUT;
	input_clients_data.num_of_clients = 0;
	input_clients_data.currently_processed_client_index = 0;
	input_clients_data.currently_processed_port_index = 0;
	input_clients_data.currently_processed_connection_index = 0;
	input_clients_data.clients_text_data = "";
	input_clients_data.clients_data_text_lines_list.clear();
	input_clients_data.connected_to_list.clear();

	output_clients_data.data_type = _CLIENTS_TYPE_DATA_OUTPUT;
	output_clients_data.num_of_clients = 0;
	output_clients_data.currently_processed_client_index = 0;
	output_clients_data.currently_processed_port_index = 0;
	output_clients_data.currently_processed_connection_index = 0;
	output_clients_data.clients_text_data = "";
	output_clients_data.clients_data_text_lines_list.clear();
	output_clients_data.connected_to_list.clear();

	clients_connections_data.data_type = _CLIENTS_TYPE_DATA_CONNECTIONS;
	clients_connections_data.num_of_clients = 0;
	clients_connections_data.currently_processed_client_index = 0;
	clients_connections_data.currently_processed_port_index = 0;
	clients_connections_data.currently_processed_connection_index = 0;
	clients_connections_data.clients_text_data = "";
	clients_connections_data.clients_data_text_lines_list.clear();
	clients_connections_data.connected_to_list.clear();

	for (int cl = 0; cl < max_num_of_alsa_clients; cl++)
	{
		input_clients_data.clients_num[cl] = -1;
		input_clients_data.clients_name[cl] = "";
		input_clients_data.clients_type[cl] = "";
		input_clients_data.clients_num_of_ports[cl] = 0;

		output_clients_data.clients_num[cl] = -1;
		output_clients_data.clients_name[cl] = "";
		output_clients_data.clients_type[cl] = "";
		output_clients_data.clients_num_of_ports[cl] = 0;

		clients_connections_data.clients_num[cl] = -1;
		clients_connections_data.clients_name[cl] = "";
		clients_connections_data.clients_type[cl] = "";
		clients_connections_data.clients_num_of_ports[cl] = 0;

		for (int pr = 0; pr < max_num_of_alsa_ports; pr++)
		{
			input_clients_data.clients_ports[cl][pr] = -1;
			input_clients_data.clients_ports_names[cl][pr] = "";

			output_clients_data.clients_ports[cl][pr] = -1;
			output_clients_data.clients_ports_names[cl][pr] = "";

			clients_connections_data.clients_ports[cl][pr] = -1;
			clients_connections_data.clients_ports_names[cl][pr] = "";
		}
	}

	clients_connections_data.connected_to_list.clear();

	return 0;
}

/**
*	@brief	Refresh all ALSA clients and connections data
*	@param	none
*	@return 0
*/
int AlsaMidiSysControl::refresh_alsa_clients_data()
{
	init_clients_data();

	get_alsa_clients_data();
	parse_alsa_clients_data_text_lines();
	parse_clients_data_text_lines_types();
	parse_alsa_clients_data();
	return 0;
}

/**
*	@brief	Activate aconnect commands (-i, -o, -l), captures stdout response strings.
*	@param	none
*	@return 0
*/
int AlsaMidiSysControl::get_alsa_clients_data()
{
	input_clients_data.clients_text_data = Utils::execute_system_command_getstd_out("aconnect -i");
	output_clients_data.clients_text_data = Utils::execute_system_command_getstd_out("aconnect -o");
	clients_connections_data.clients_text_data = Utils::execute_system_command_getstd_out("aconnect -l");
	return 0;
}

/**
*	@brief	Parse the data text string into lines terminated by \n
*	@param	data_text pointer to a string that holds the data to be parsed.
*	@return a vector of text lines strings
*/
std::vector<s_alsa_text_line_t> AlsaMidiSysControl::parse_data_text_lines(std::string *data_text)
{
	std::vector<s_alsa_text_line_t> lines;
	int new_position = 0, last_position = 0;
	s_alsa_text_line_t line;

	line.client = -1;
	line.port = -1;
	line.type = -1;

	while (last_position < data_text->length())
	{
		new_position = data_text->find_first_of("\n", last_position);
		if (new_position == std::string::npos)
		{
			// no more \n
			break;
		}
		else
		{
			line.text_line = data_text->substr(last_position, new_position - last_position);
			lines.push_back(line);
			last_position = new_position + 1;
		}
	}

	return lines;
}

/**
*	@brief	Parses all the captured stdout responses strings into lines.
*	@param	none
*	@return 0
*/
int AlsaMidiSysControl::parse_alsa_clients_data_text_lines()
{
	input_clients_data.clients_data_text_lines_list = parse_data_text_lines(&input_clients_data.clients_text_data);
	output_clients_data.clients_data_text_lines_list = parse_data_text_lines(&output_clients_data.clients_text_data);
	clients_connections_data.clients_data_text_lines_list = parse_data_text_lines(&clients_connections_data.clients_text_data);
	return 0;
}

/**
*	@brief	Parses a text line and get its type (client, port connection).
*	@param	text_line_data	pointer to a s_text_line_t structure
*	@return line type; -1 if illegale
*/
int AlsaMidiSysControl::parse_text_line_type(s_alsa_text_line_t *text_line_data)
{
	if ((text_line_data == NULL) || (text_line_data->text_line.size() == 0))
	{
		return -1;
	}

	text_line_data->type = -1;

	if (text_line_data->text_line.at(0) == 'c')
	{
		// client line
		text_line_data->type = _LINE_TYPE_CLIENT;
	}
	else if (first_non_space_char_is_digit(&text_line_data->text_line))
	{
		// port line
		text_line_data->type = _LINE_TYPE_PORT;
	}
	else if (text_line_data->text_line.at(0) == '\t')
	{
		// connection line
		text_line_data->type = _LINE_TYPE_CONNECTIONS;
	}

	return text_line_data->type;
}

/**
*	@brief	Parses all text lines and get there types (client, port connection).
*	@param	clients_data	a pointer to a s_clients_data_t structure
*	@return 0 if done; else -1
*/
int AlsaMidiSysControl::parse_text_lines_types(s_alsa_clients_data_t *clients_data)
{
	int line_index = 0;

	if ((clients_data == NULL) || (clients_data->clients_data_text_lines_list.size() == 0))
	{
		return -1;
	}

	while (line_index < clients_data->clients_data_text_lines_list.size())
	{
		parse_text_line_type(&clients_data->clients_data_text_lines_list.at(line_index++));
	}

	return 0;
}

/**
*	@brief	Parses all captured clients data text lines and get there types (client, port connection).
*	@param	none
*	@return 0 if done; else -1
*/
int AlsaMidiSysControl::parse_clients_data_text_lines_types()
{
	int res = 0;

	res = parse_text_lines_types(&input_clients_data);
	res |= parse_text_lines_types(&output_clients_data);
	res |= parse_text_lines_types(&clients_connections_data);

	return res;
}

/**
*	@brief	Parses and process a "client" text line. client n: 'name' [type=t]
*	@param	text_line	pointer to a text line string
*	@param	clients_data	pointer to a s_clients_data_t structure
*	@return 0 if a valid line; else -1
*/
int AlsaMidiSysControl::process_client_text_line(
	std::string *text_line,
	s_alsa_clients_data_t *clients_data)
{
	int pos, start, end;
	std::string num_str, name_str, type_str;

	if ((clients_data == NULL) || (text_line == NULL) || (text_line->size() == 0))
	{
		return -1;
	}

	// Parse client num
	end = text_line->find(":", 7); // 7: 'client 'n
	if (end == std::string::npos)
	{
		return -1;
	}

	num_str = text_line->substr(7, end - 7);
	if (num_str.length() == 0)
	{
		return -1;
	}

	clients_data->clients_num[clients_data->currently_processed_client_index] = std::stoi(num_str);

	end += 1;

	// Parse client name
	start = text_line->find("'", end);
	if (start == std::string::npos)
	{
		return -1;
	}
	start += 1; // 'name

	end = text_line->find("'", start);
	if (end == std::string::npos)
	{
		return -1;
	}

	name_str = text_line->substr(start, end - start);
	if (name_str.length() == 0)
	{
		return -1;
	}
	clients_data->clients_name[clients_data->currently_processed_client_index] = name_str;

	end += 1;

	// Parse client type
	start = text_line->find("[type=", end);
	if (start == std::string::npos)
	{
		return -1;
	}

	start += 6; // "[type=" 6 chars

	end = text_line->find("]", start);
	if (end == std::string::npos)
	{
		return -1;
	}

	type_str = text_line->substr(start, end - start);
	if (type_str.length() == 0)
	{
		return -1;
	}

	clients_data->clients_type[clients_data->currently_processed_client_index] = type_str;
	// last will remain; start from 0
	clients_data->num_of_clients = clients_data->currently_processed_client_index + 1;

	return 0;
}

/**
*	@brief	Parses and process a "port" text line. "  n 'name'"
*	@param	text_line 	pointer to a text line string
*	@param	clients_data	pointer to a s_clients_data_t structure
*	@return 0 if a valid line; else -1
*/
int AlsaMidiSysControl::process_port_text_line(
	std::string *text_line,
	s_alsa_clients_data_t *clients_data)
{
	int pos, start, end;
	std::string num_str, name_str;

	if ((clients_data == NULL) || (text_line == NULL) || (text_line->size() == 0))
	{
		return -1;
	}

	// Parse port num
	pos = text_line->find_first_of("0123456789", 0);
	if (pos == std::string::npos)
	{
		return -1;
	}

	end = text_line->find_first_of(" ", pos);
	if (end == std::string::npos)
	{
		return -1;
	}

	num_str = text_line->substr(pos, end - pos);
	clients_data->clients_ports[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index] = std::stoi(num_str);

	// Parse port name
	start = text_line->find("'", end);
	if (start == std::string::npos)
	{
		return -1;
	}
	start += 1; // 'name

	end = text_line->find("'", start);
	if (end == std::string::npos)
	{
		return -1;
	}

	name_str = text_line->substr(start, end - start);
	if (name_str.length() == 0)
	{
		return -1;
	}
	clients_data->clients_ports_names[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index] = name_str;
	// last will remain; start from 0
	clients_data->clients_num_of_ports[clients_data->currently_processed_client_index] = clients_data->currently_processed_port_index + 1;

	return 0;
}

/**
*	@brief	Parses and process a "Connecting To" text line. "Connecting To: c:p, c:p, c:p"
*	@param	text_line 	pointer to a text line string
*	@param	clients_data	pointer to a s_clients_data_t structure
*	@return 0 if a valid line; else -1
*/
int AlsaMidiSysControl::process_connecting_to_text_line(
	std::string *text_line,
	s_alsa_clients_data_t *clients_data)
{
	s_alsa_clients_connection_t connection;
	int colons_label_pos, colons_connection_pos, comma_pos, end, last_pos;
	std::string client_num_str, port_num_str;

	if ((clients_data == NULL) || (text_line == NULL) || (text_line->size() == 0))
	{
		// Bad data
		return -1;
	}

	// Un set
	connection.connected_client = -1;
	connection.connected_port = -1;
	connection.connecting_to_client_num = -1;
	connection.connecting_to_port_num = -1;

	// find the label ending colons place "Connecting To:"
	colons_label_pos = text_line->find_first_of(":", 0);
	if (colons_label_pos == std::string::npos)
	{
		// No valid label
		return -1;
	}

	// Look for multiple connections (seperated by commas)
	comma_pos = text_line->find_first_of(",", colons_label_pos + 1);
	if (comma_pos != std::string::npos)
	{
		// A comma was found; multiple connections: 1st connection
		colons_connection_pos = text_line->find_first_of(":", colons_label_pos + 1);
		if (colons_connection_pos == std::string::npos)
		{
			return -1;
		}
		client_num_str = text_line->substr(colons_label_pos + 2, colons_connection_pos - colons_label_pos - 2); // ": cc:"
		if (client_num_str.length() == 0)
		{
			return -1;
		}
		else
		{
			connection.connecting_to_client_num = stoi(client_num_str);
		}
		// Parse port num
		end = comma_pos - 1;
		port_num_str = text_line->substr(colons_connection_pos + 1, end - colons_connection_pos);
		if (port_num_str.length() == 0)
		{
			return -1;
		}
		else
		{
			connection.connecting_to_port_num = stoi(port_num_str);
		}

		connection.connected_client = clients_data->clients_num[clients_data->currently_processed_client_index];
		connection.connected_port = clients_data->clients_ports[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index];
		clients_data->connected_to_list.push_back(connection);

		last_pos = comma_pos;

		// Look for additional multiple connections
		while (true)
		{
			comma_pos = text_line->find_first_of(",", last_pos + 1);
			if (comma_pos == std::string::npos)
			{
				// No more commas: exit and look for last connection
				break;
			}
			else
			{
				// More connections
				colons_connection_pos = text_line->find_first_of(":", last_pos + 1);
				if (colons_connection_pos == std::string::npos)
				{
					return -1;
				}
				// Parse client num
				client_num_str = text_line->substr(last_pos + 2, colons_connection_pos - last_pos - 2); // ", cc:"
				if (client_num_str.length() == 0)
				{
					return -1;
				}
				else
				{
					connection.connecting_to_client_num = stoi(client_num_str);
				}
				last_pos = comma_pos;
				// Parse port num
				end = comma_pos - 1;
				port_num_str = text_line->substr(colons_connection_pos + 1, end - colons_connection_pos); // :pp,
				if (port_num_str.length() == 0)
				{
					return -1;
				}
				else
				{
					connection.connecting_to_port_num = stoi(port_num_str);
				}

				connection.connected_client = clients_data->clients_num[clients_data->currently_processed_client_index];
				connection.connected_port = clients_data->clients_ports[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index];

				clients_data->connected_to_list.push_back(connection);
			}
		} // while

		// Look for the last connection
		colons_connection_pos = text_line->find_first_of(":", last_pos);
		if (colons_connection_pos == std::string::npos)
		{
			return -1;
		}
		// Parse client num
		client_num_str = text_line->substr(last_pos + 2, colons_connection_pos - last_pos - 2); // ": cc:"
		if (client_num_str.length() == 0)
		{
			return -1;
		}
		else
		{
			connection.connecting_to_client_num = stoi(client_num_str);
		}
		// Parse port num
		end = text_line->length() - 1;
		port_num_str = text_line->substr(colons_connection_pos + 1, end - colons_connection_pos);
		if (port_num_str.length() == 0)
		{
			return -1;
		}
		else
		{
			connection.connecting_to_port_num = stoi(port_num_str);
		}

		connection.connected_client = clients_data->clients_num[clients_data->currently_processed_client_index];
		connection.connected_port = clients_data->clients_ports[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index];

		clients_data->connected_to_list.push_back(connection);
	}
	else
	{
		// no commas - no multiple connections; look for a single connection
		colons_connection_pos = text_line->find_first_of(":", colons_label_pos + 1); // 2nd colons c:p
		if (colons_connection_pos == std::string::npos)
		{
			// error - no connection
			return -1;
		}
		// Parse client num
		client_num_str = text_line->substr(colons_label_pos + 2, colons_connection_pos - colons_label_pos - 2); // ": cc:"
		if (client_num_str.length() == 0)
		{
			return -1;
		}
		else
		{
			connection.connecting_to_client_num = stoi(client_num_str);
		}
		// Parse port num
		end = text_line->length() - 1;
		port_num_str = text_line->substr(colons_connection_pos + 1, end - colons_connection_pos);
		if (port_num_str.length() == 0)
		{
			return -1;
		}
		else
		{
			connection.connecting_to_port_num = stoi(port_num_str);
		}

		connection.connected_client = clients_data->clients_num[clients_data->currently_processed_client_index];
		connection.connected_port = clients_data->clients_ports[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index];

		clients_data->connected_to_list.push_back(connection);

		return 0;
	}

	return 0;
}

/**
*	@brief	Parses and process a "Connected From" text line.
*	@param	text_line 	pointer to a text line string
*	@param	clients_data	pointer to a s_clients_data_t structure
*	@return 0 if a valid line; else -1
*/
int AlsaMidiSysControl::process_connected_from_text_line(
	std::string *text_line,
	s_alsa_clients_data_t *clients_data)
{
	return 0;
}

/**
*	@brief	Check if a text line starts with leading spaces folowed by digits (port line).
*	@param	text_line 	pointer to a text line string
*	@return true if a text line starts with leading spaces folowed by digits; else false 
*/
bool AlsaMidiSysControl::first_non_space_char_is_digit(std::string *text_line)
{
	int pos1, pos2;

	if (text_line->length() == 0)
	{
		// Empty text line
		return false;
	}
	else
	{
		// line may start with leading spaces
		pos1 = text_line->find_first_not_of(" ", 0);
		if (pos1 == std::string::npos)
		{
			// Text line has no spaces
			return false;
		}
		else
		{
			// look for a digit
			pos2 = text_line->find_first_of("0123456789", 0);
			if ((pos1 == pos2) && (pos2 < 10)) // 10?
			{
				// 1st non-space char is a digit and no too many leading spaces
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

/**
*   @brief  Process a text line based on its type.
*   @param  line_num  line number
*   @param	clients_data	pointer to a s_clients_data_t structure
*   @return line type; -1 otherwise.
*/
int AlsaMidiSysControl::process_text_line(
	int line_num,
	s_alsa_clients_data_t *clients_data)
{
	int res, pos;

	if (clients_data == NULL)
	{
		return -1;
	}

	if (clients_data->clients_data_text_lines_list.at(line_num).text_line.length() == 0)
	{
		return -1;
	}

	// Find line type
	if (clients_data->clients_data_text_lines_list.at(line_num).type == _LINE_TYPE_CLIENT)
	{
		// client line
		clients_data->currently_processed_client_index++; // starts at -1
		res = process_client_text_line(&clients_data->clients_data_text_lines_list.at(line_num).text_line, clients_data);
		if (res == 0)
		{
			clients_data->currently_processed_port_index = -1; // Init
		}
		else
		{
			// Not a valid client line;
			if (clients_data->currently_processed_client_index > 0)
			{
				clients_data->currently_processed_client_index = -1;
			}
		}
	}
	else if (clients_data->clients_data_text_lines_list.at(line_num).type == _LINE_TYPE_PORT)
	{
		// port line
		clients_data->currently_processed_port_index++;
		res = process_port_text_line(&clients_data->clients_data_text_lines_list.at(line_num).text_line, clients_data);
		if (res == 0)
		{
			//
		}
		else
		{
			// Not a valid port line;
			if (clients_data->currently_processed_port_index > 0)
			{
				clients_data->currently_processed_port_index = -1;
			}
		}
	}
	else if (clients_data->clients_data_text_lines_list.at(line_num).type == _LINE_TYPE_CONNECTIONS)
	{
		// connecting to/connected from line
		pos = clients_data->clients_data_text_lines_list.at(line_num).text_line.find("Connecting", 1);
		if (pos != std::string::npos)
		{
			// "Connecting To" line
			res = process_connecting_to_text_line(&clients_data->clients_data_text_lines_list.at(line_num).text_line, clients_data);
			if (res == 0)
			{
				clients_data->currently_processed_connection_index++;
			}
		}
		else if (clients_data->clients_data_text_lines_list.at(line_num).text_line.find_first_of("Connected", 1) != std::string::npos)
		{
			// Connected From line - ignore
			//res = process_connected_from_text_line(text_line, clients_data);
		}
	}
	else
	{
		// A unknown line type
		return -1;
	}

	return 0;
}

/**
*	@brief	Parses clients data.
*	@param	none
*	@return num of clients if ok; -1 if error
*/
int AlsaMidiSysControl::parse_alsa_client_data(s_alsa_clients_data_t *clients_data)
{
	//	std::string parssed_line;
	int line_index = 0;
	int res;

	if (clients_data == NULL)
	{
		return -1;
	}

	clients_data->currently_processed_client_index = -1;
	clients_data->currently_processed_port_index = -1;
	clients_data->currently_processed_connection_index = -1;

	while (line_index < clients_data->clients_data_text_lines_list.size())
	{
		res = process_text_line(line_index, clients_data);
		if (res < 0)
		{
			// error
			break;
		}
		else
		{
			clients_data->clients_data_text_lines_list.at(line_index).client = clients_data->currently_processed_client_index;
			clients_data->clients_data_text_lines_list.at(line_index).port = clients_data->currently_processed_port_index;
			line_index++;
		}
	}

	return clients_data->num_of_clients;
}

/**
*	@brief	Parses the data for all clients.
*	@param	none
*	@return 0 ok; -1 if error
*/
int AlsaMidiSysControl::parse_alsa_clients_data()
{
	int res;

	res = parse_alsa_client_data(&input_clients_data);
	res |= parse_alsa_client_data(&output_clients_data);
	res |= parse_alsa_client_data(&clients_connections_data);
	return res;
}

/**
*   @brief  Returns the number of detected input MIDI clientes.
*   @param  none.
*   @return int the number of detected input MIDI clients.
*/
int AlsaMidiSysControl::get_num_of_input_midi_clients()
{
	return input_clients_data.num_of_clients;
}

/**
*   @brief  Returns the number of detected output MIDI clients.
*   @param  none.
*   @return int the number of detected output MIDI clients.
*/
int AlsaMidiSysControl::get_num_of_output_midi_clients()
{
	return output_clients_data.num_of_clients;
}

/**
*   @brief  Returns the number of an input MIDI client ports.
*   @param  cln input client index.
*   @return int the number of an input MIDI clients ports; -1 if param out of range.
*/
int AlsaMidiSysControl::get_num_of_input_midi_client_ports(int cln)
{
	if ((cln >= 0) && (cln < input_clients_data.num_of_clients))
	{
		return input_clients_data.clients_num_of_ports[cln];
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Returns the number of an output MIDI client ports.
*   @param  cln output client index.
*   @return int the number of an output MIDI clients ports; ; -1 if param out of range.
*/
int AlsaMidiSysControl::get_num_of_output_midi_client_ports(int cln)
{
	if ((cln >= 0) && (cln < output_clients_data.num_of_clients))
	{
		return output_clients_data.clients_num_of_ports[cln];
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Returns the selected MIDI input client name string.
*   @param  int cln input device id.
*   @param  name std::string* pointer to the selected MIDI input client name string.
*   @param	bool replace   if true, replaces the virtual client names with the actual names
*	@return void
*/
void AlsaMidiSysControl::get_midi_input_client_name_string(int cln, std::string *name, bool replace)
{
	char cln_name[256];

	if ((cln >= 0) && (cln < input_clients_data.num_of_clients))
	{
		sprintf(cln_name, "%i:%s", input_clients_data.clients_num[cln], input_clients_data.clients_name[cln].c_str());
		*name = std::string(cln_name);
	}
	else
	{
		*name = "";
	}

	if (replace)
	{
		if ((bt_client_in_name != "") &&
			(*name == bt_client_in_name))
		{
			*name = _ALSA_NAME_CLIENT_BT_STR; // (" + *name + ") ";
		}
		else if ((control_box_client_in_name != "") &&
				 (*name == control_box_client_in_name))
		{
			*name = _ALSA_NAME_CLIENT_CONTROL_BOX_STR; // (" + *name + ")";
		}
		else if ((control_box_xt_midi_in_client_name != "") &&
				 (*name == control_box_xt_midi_in_client_name))
		{
			*name = _ALSA_NAME_CLIENT_CONTROL_BOX_EXT_MIDI_STR; // (" + *name + ")";
		}
		else if ((midi_player_client_in_name != "") &&
				 (*name == midi_player_client_in_name))
		{
			*name = _ALSA_NAME_CLIENT_MIDI_PLAYER_STR;
		}
		else if ((midi_mapper_client_in_name != "") &&
				 (*name == midi_mapper_client_in_name))
		{
			*name = _ALSA_NAME_CLIENT_MIDI_MAPPER_STR;
		}
	}
}

/**
*   @brief  Returns the selected MIDI output client name string.
*   @param  int cln output device id.
*   @param  name char* pointer to the selected MIDI output client name string.
*	@return void
*/
void AlsaMidiSysControl::get_midi_output_client_name_string(int cln, std::string *name)
{
	char cln_name[256];

	if ((cln >= 0) && (cln < output_clients_data.num_of_clients))
	{
		sprintf(cln_name, "%i:%s", output_clients_data.clients_num[cln], output_clients_data.clients_name[cln].c_str());
		*name = std::string(cln_name);
	}
	else
	{
		*name = "";
	}
}

/**
*   @brief  Returns the number of MIDI in clients and an ordered list of the clients name.
*   @param  std::list<std::string>* a pointer to the list of names
*   @return int number of clients
*	
*/


int AlsaMidiSysControl::get_alsa_midi_in_clients_names_list(std::list<std::string> *names_list)
{
	int num_of_clients = get_num_of_input_midi_clients();
	std::string name;

	if ((names_list == NULL) || (num_of_clients <= 0))
	{
		return -1;
	}

	for (int i = 0; i < num_of_clients; i++)
	{
		get_midi_input_client_name_string(i, &name);
		names_list->push_back(name);
	}

	return num_of_clients;
}



/**
*   @brief  Returns the selected MIDI iput client id.
*   @param  string	client name.
*   @param	bool	replace	if true, use virtual clients given name (default),
*							if false, use virtual clients system given name
*	@return void
*/
int AlsaMidiSysControl::get_midi_input_client_id(std::string name, bool replace)
{
	int count;
	std::string client_name = name;
	bool replaced = false;

	if (replace)
	{
		if (name == _ALSA_NAME_CLIENT_BT_STR)
		{
			client_name = bt_client_in_name;
			replaced = true;
		}
		else if (name == _ALSA_NAME_CLIENT_CONTROL_BOX_STR)
		{
			client_name = control_box_client_in_name;
			replaced = true;
		}
		else if (client_name == _ALSA_NAME_CLIENT_CONTROL_BOX_EXT_MIDI_STR)
		{
			client_name = control_box_xt_midi_in_client_name;
			replaced = true;
		}
		else if (client_name == _ALSA_NAME_CLIENT_MIDI_PLAYER_STR)
		{
			client_name = midi_player_client_in_name;
			replaced = true;
		}
		else if (client_name == _ALSA_NAME_CLIENT_MIDI_MAPPER_STR)
		{
			client_name = midi_mapper_client_in_name;
			replaced = true;
		}
		
		
		/* Remove prefix */
		if (replaced)
		{
			int loc = client_name.find_first_of(":");
			if (loc != std::string::npos)
			{
				std::string removed_name = client_name.substr(loc + 1);
				client_name = removed_name;
			}
		}
	}

	for (count = 0; count < input_clients_data.num_of_clients; count++)
	{
		if (input_clients_data.clients_name[count] == client_name)
		{
			break;
		}
	}

	if (count < input_clients_data.num_of_clients)
	{
		return count;
	}

	return -1;
}

/**
*   @brief  Returns the selected MIDI output client id.
*   @param  string	client name.
*	@return void
*/
int AlsaMidiSysControl::get_midi_output_client_id(std::string name)
{
	int count;

	for (count = 0; count < output_clients_data.num_of_clients; count++)
	{
		if (output_clients_data.clients_name[count] == name)
		{
			break;
		}
	}

	if (count < output_clients_data.num_of_clients)
	{
		return count;
	}

	return -1;
}



/**
*   @brief  Returns the selected MIDI input client port name string.
*   @param  int cln input client id.
*   @param	int port port number
*   @param	replace	if true, replace virtual clients names
*   @param  name std::string* pointer to the selected MIDI input client port name string.
*	@return void
*/
void AlsaMidiSysControl::get_midi_input_client_port_name_string(int cln, int port, std::string *name)
{
	char port_name[256];

	if ((cln >= 0) && (cln < input_clients_data.num_of_clients))
	{
		if (input_clients_data.clients_num_of_ports[cln] > 0)
		{
			if ((port >= 0 && (port < input_clients_data.clients_num_of_ports[cln])))
			{
				sprintf(port_name, "%i:%s",
						input_clients_data.clients_ports[cln][port],
						input_clients_data.clients_ports_names[cln][port].c_str());
				*name = std::string(port_name);
			}
		}
	}
	else
	{
		*name = "";
	}
}

/**
*   @brief  Returns the selected MIDI output client port name string.
*   @param  int cln output client id.
*   @param	int port port number
*   @param  name std::string* pointer to the selected MIDI output client port name string.
*	@return void
*/
void AlsaMidiSysControl::get_midi_output_client_port_name_string(int cln, int port, std::string *name)
{
	char port_name[256];

	if ((cln >= 0) && (cln < output_clients_data.num_of_clients))
	{
		if (output_clients_data.clients_num_of_ports[cln] > 0)
		{
			if ((port >= 0 && (port < output_clients_data.clients_num_of_ports[cln])))
			{
				sprintf(port_name,
						"%i:%s",
						output_clients_data.clients_ports[cln][port],
						output_clients_data.clients_ports_names[cln][port].c_str());
				*name = std::string(port_name);
			}
		}
	}
	else
	{
		*name = "";
	}
}

/**
*   @brief  Connect an input MIDI client to a MIDI output client.
*   @para use_index: if true
*	@param  in_client	the input client index on the detected midi-clients list.
*   @param  in_port		the input client port index on the detected midi-clients list.
*	@param  out_client	the output client index on the detected midi-clients list.
*   @param  out_port	the output client port index on the detected midi-clients list.
*   @para use_index: if false
*	@param  in_client	the input client number.
*   @param  in_port		the input client port number.
*	@param  out_client	the output client number.
*   @param  out_port	the output client port number.
*   
*   @return 0 if done; -1 if params out of range; 
*/
int AlsaMidiSysControl::connect_midi_clients(int in_client, int in_port, int out_client, int out_port, bool use_index)
{
	char command[128];

	int in_client_num, in_client_port_num, out_client_num, out_client_port_num;

	if (use_index)
	{
		if ((in_client >= 0) && (in_client < input_clients_data.num_of_clients) &&
			(out_client >= 0) && (out_client < output_clients_data.num_of_clients) &&
			(in_port >= 0) && (in_port < input_clients_data.clients_num_of_ports[in_client]) &&
			(out_port >= 0) && (out_port < output_clients_data.clients_num_of_ports[out_client]))
		{
			in_client_num = input_clients_data.clients_num[in_client];
			in_client_port_num = input_clients_data.clients_ports[in_client][in_port];
			out_client_num = output_clients_data.clients_num[out_client];
			out_client_port_num = output_clients_data.clients_ports[out_client][out_port];
		}
		else
		{
			return -1;
		}
	}
	else
	{
		in_client_num = in_client;
		in_client_port_num = in_port;
		out_client_num = out_client;
		out_client_port_num = out_port;
	}

	sprintf(command, "aconnect %i:%i %i:%i",
			in_client_num, in_client_port_num, out_client_num, out_client_port_num);

	system(command);

	return 0;
}

/**
*   @brief  Disconnect an input MIDI client from a MIDI output client.
*   @para use_index: if true
*	@param  in_client	the input client index on the detected midi-clients list.
*   @param  in_port		the input client port index on the detected midi-clients list.
*	@param  out_client	the output client index on the detected midi-clients list.
*   @param  out_port	the output client port index on the detected midi-clients list.
*   @para use_index: if false
*	@param  in_client	the input client number.
*   @param  in_port		the input client port number.
*	@param  out_client	the output client number.
*   @param  out_port	the output client port number.
*   
*   @return 0 if done; -1 if params out of range; 
*/
int AlsaMidiSysControl::disconnect_midi_clients(int in_client, int in_port, int out_client, int out_port, bool use_index)
{
	char command[128];

	int in_client_num, in_client_port_num, out_client_num, out_client_port_num;

	if (use_index)
	{
		if ((in_client >= 0) && (in_client < input_clients_data.num_of_clients) &&
			(out_client >= 0) && (out_client < output_clients_data.num_of_clients) &&
			(in_port >= 0) && (in_port < input_clients_data.clients_num_of_ports[in_client]) &&
			(out_port >= 0) && (out_port < output_clients_data.clients_num_of_ports[out_client]))
		{
			in_client_num = input_clients_data.clients_num[in_client];
			in_client_port_num = input_clients_data.clients_ports[in_client][in_port];
			out_client_num = output_clients_data.clients_num[out_client];
			out_client_port_num = output_clients_data.clients_ports[out_client][out_port];
		}
		else
		{
			return -1;
		}
	}
	else
	{
		in_client_num = in_client;
		in_client_port_num = in_port;
		out_client_num = out_client;
		out_client_port_num = out_port;
	}

	sprintf(command, "aconnect -d %i:%i %i:%i",
			in_client_num, in_client_port_num, out_client_num, out_client_port_num);

	system(command);

	return 0;
}

/**
*   @brief  Disconnect all connected midi clients connections.
*   @param  none.
*	@return 0
*/
int AlsaMidiSysControl::disconnect_all_midi_clients()
{
	return system("aconnect -x");
}

/**
*   @brief  Returns the number of midi clients (connected) connections.
*   @param  none.
*	@return the number of midi clients (connected) connections
*/
int AlsaMidiSysControl::get_num_of_midi_clients_connections()
{
	return clients_connections_data.connected_to_list.size();
}

/**
*   @brief  Returns the connected (src) client number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connected-client (kbd) client number 
*/
int AlsaMidiSysControl::get_midi_client_connection_connected_client_num(int con_num)
{
	if ((con_num < 0) || (con_num >= clients_connections_data.connected_to_list.size()))
	{
		return -1;
	}
	else
	{
		return clients_connections_data.connected_to_list.at(con_num).connected_client;
	}
}

/**
*   @brief  Returns the connected (src) port number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connected-client (kbd) port number 
*/
int AlsaMidiSysControl::get_midi_client_connection_connected_port_num(int con_num)
{
	if ((con_num < 0) || (con_num >= clients_connections_data.connected_to_list.size()))
	{
		return -1;
	}
	else
	{
		return clients_connections_data.connected_to_list.at(con_num).connected_port;
	}
}

/**
*   @brief  Returns the connecting to (target) client number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connecting to (target) client number. 
*/
int AlsaMidiSysControl::get_midi_client_connection_connecting_to_client_num(int con_num)
{
	if ((con_num < 0) || (con_num >= clients_connections_data.connected_to_list.size()))
	{
		return -1;
	}
	else
	{
		return clients_connections_data.connected_to_list.at(con_num).connecting_to_client_num;
	}
}

/**
*   @brief  Returns the connecting to (target) port number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connecting to (target) port number. 
*/
int AlsaMidiSysControl::get_midi_client_connection_connecting_to_port_num(int con_num)
{
	if ((con_num < 0) || (con_num >= clients_connections_data.connected_to_list.size()))
	{
		return -1;
	}
	else
	{
		return clients_connections_data.connected_to_list.at(con_num).connecting_to_port_num;
	}
}

/**
*   @brief  Returns the client id in the connections list.
*   @param  client name
*	@return con_num		connection num (0 to num_of_connections-1). 
*/
int AlsaMidiSysControl::get_midi_client_connection_id(std::string name)
{
	int num_of_connections = get_num_of_midi_clients_connections();
	bool client_found = false;
	int cln;

	if (name == "")
	{
		return -1;
	}
	
	for (cln = 0; cln < num_of_connections; cln++)
	{
		if (name == clients_connections_data.clients_name[cln])
		{
			client_found = true;
			break;
		}
	}

	if (client_found)
	{
		return cln;
	}

	return -1;
}

/**
*   @brief  Returns the client num in the connections list.
*   @param  client name
*	@return client number (usually the 1xx). 
*/
int AlsaMidiSysControl::get_midi_client_connection_num(std::string name)
{
	int num_of_connections = get_num_of_midi_clients_connections();
	bool client_found = false;
	int cln, clnid;

	if (name == "")
	{
		return -1;
	}

	/* Find the client line number in the connections table base on the client name. */
	clnid = get_midi_client_connection_id(name);

	if (clnid < 0)
	{
		/* Not found */
		return -1;
	}
	else
	{
		/* Get the client number. */
		return clients_connections_data.clients_num[clnid];
	}

	return -1;
}

/**
*   @brief  Returns a list of clients numbers (@connection) that connects to a client.
*   @param  cln_num	the connected to client number
*	@param	pointer to alist of clients numbers (@connection) that connects to a client. 
*	@return	num of clients found
*/
int AlsaMidiSysControl::get_midi_client_connected_to_clients_numbers_list(int cln_num, std::list<int> *cln_lst)
{
	int i;
	s_alsa_clients_connection_t clients_connection;


	for (i = 0; i < clients_connections_data.connected_to_list.size(); i++) // connected_to_list is a <vector>
	{
		clients_connection = clients_connections_data.connected_to_list.at(i);

		if (clients_connection.connecting_to_client_num == cln_num)
		{
			cln_lst->push_back(clients_connection.connected_client);
		}
	}

	return cln_lst->size();
}

/**
*   @brief  Returns a list of clients names that connects to a client.
*   @param  cln_num	the connected to client number
*	@param	pointer to alist of clients names that connects to a client.
*	@param  replace if true, virtual clients ames are replaced with the application names.
*	@return	num of clients found
*/
int AlsaMidiSysControl::get_midi_client_connected_to_clients_names_list(int cln_num, bool replace,
																		std::list<std::string> *cln_lst)
{
	std::list<int> con_to;
	int cl, cn;
	std::string cl_name;

	get_midi_client_connected_to_clients_numbers_list(cln_num, &con_to);

	std::list<int>::iterator it_cln = con_to.begin();

	for (cl = 0; cl < con_to.size(); cl++)
	{
		for (cn = 0; cn < clients_connections_data.num_of_clients; cn++)
		{

			if (clients_connections_data.clients_num[cn] == *it_cln)
			{
				cl_name = clients_connections_data.clients_name[cn];
				
				if (replace)
				{
					if ((bt_client_in_name != "") &&
						(cl_name == strip_client_name_prefix(bt_client_in_name)))
					{
						cl_name = _ALSA_NAME_CLIENT_BT_STR; // (" + *name + ") ";
					}
					else if ((control_box_client_in_name != "") &&
							 (cl_name == strip_client_name_prefix(control_box_client_in_name)))
					{
						cl_name = _ALSA_NAME_CLIENT_CONTROL_BOX_STR; // (" + *name + ")";
					}
					else if ((control_box_xt_midi_in_client_name != "") &&
							 (cl_name == strip_client_name_prefix(control_box_xt_midi_in_client_name)))
					{
						cl_name = _ALSA_NAME_CLIENT_CONTROL_BOX_EXT_MIDI_STR; // (" + *name + ")";
					}
					else if ((midi_player_client_in_name != "") &&
							 (cl_name == strip_client_name_prefix(midi_player_client_in_name)))
					{
						cl_name = _ALSA_NAME_CLIENT_MIDI_PLAYER_STR; // (" + *name + ")";
					}
					else if ((midi_mapper_client_in_name != "") &&
							 (cl_name == strip_client_name_prefix(midi_mapper_client_in_name)))
					{
						cl_name = _ALSA_NAME_CLIENT_MIDI_MAPPER_STR; // (" + *name + ")";
					}
				}

				cln_lst->push_back(cl_name);				
				
				break;
			}			
		}

		it_cln++;
	}	

	return cln_lst->size();
}

/**
 * Strips the client name prefix (prefix:name -> name)
 *
 * @param name the full client name 
 * @return the stripped name
 */
std::string AlsaMidiSysControl::strip_client_name_prefix(std::string name)
{
	int prefix_end = name.find_first_of(':');

	if (prefix_end != std::string::npos)
	{
		return name.substr(prefix_end + 1, name.length() - 1);
	}

	return name;
}
