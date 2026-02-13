/**
*	@file		jackConnections.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.1
*					1. Update includes.
*	
*	@brief		Use system "jack_lsp" commands to scan and control JACK connections.		
*				
*	History:\n
*	
*		version 1.0	 25-Mar-2021	
*/

#include <stdlib.h>

#include "jackConnections.h"
#include "../utils/utils.h"

JackConnections *JackConnections::jack_connections_instance = NULL;

JackConnections::JackConnections()
{
	jack_connections_instance = this;

	//refresh_jack_clients_data();
}

JackConnections *JackConnections::get_instance()
{
	if (jack_connections_instance == NULL)
	{
		jack_connections_instance = new JackConnections();
	}

	return jack_connections_instance;
}

/**
*	@brief	Refresh all JACK clients and connections connections data
*	@param	none
*	@return 0
*/
int JackConnections::refresh_jack_clients_data()
{
	init_jack_clients_data();

	get_jack_clients_data();
	parse_jack_clients_data_text_lines();
	parse_jack_clients_data_text_lines_types();
	//	parse_jack_clients_data();
	parse_jack_client_data(&clients_properties_data);
	sort_jack_clients();
	// Connections data parsing must be called after sorting clients into in/out clients
	parse_jack_client_data(&clients_connections_data);

	return 0;
}

/**
*   @brief  Returns the number of detected input JACK clientes.
*   @param  none.
*   @return int the number of detected input JACK clients.
*/
int JackConnections::get_num_of_input_jack_clients()
{
	return input_clients_data.num_of_clients;
}

/**
*   @brief  Returns the number of detected output JACK clientes.
*   @param  none.
*   @return int the number of detected output JACK clients.
*/
int JackConnections::get_num_of_output_jack_clients()
{
	return output_clients_data.num_of_clients;
}

/**
*   @brief  Returns the number of JACK input client ports.
*   @param  cln client index.
*   @return int the number of an JACK input clients ports; -1 if param out of range.
*/
int JackConnections::get_num_of_jack_input_client_ports(int cln)
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
*   @brief  Returns the number of JACK output client ports.
*   @param  cln client index.
*   @return int the number of an JACK output clients ports; -1 if param out of range.
*/
int JackConnections::get_num_of_jack_output_client_ports(int cln)
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
*   @brief  Returns the name of JACK input client.
*   @param  cln client index.
*   @return int the number of an JACK input client name; "" if param out of range.
*/
std::string JackConnections::get_jack_input_client_name(int cln)
{
	if ((cln >= 0) && (cln < input_clients_data.num_of_clients))
	{
		return input_clients_data.clients_name[cln];
	}
	else
	{
		return "";
	}
}

/**
*   @brief  Returns the name of JACK output client.
*   @param  cln client index.
*   @return int the number of an JACK output client name; "" if param out of range.
*/
std::string JackConnections::get_jack_output_client_name(int cln)
{
	if ((cln >= 0) && (cln < output_clients_data.num_of_clients))
	{
		return output_clients_data.clients_name[cln];
	}
	else
	{
		return "";
	}
}

/**
*   @brief  Returns the name of JACK input client port.
*   @param  cln client index.
*   @param	prt port index
*   @return int the number of an JACK input client name; "" if param out of range.
*/
std::string JackConnections::get_jack_input_client_port_name(int cln, int prt)
{
	if ((cln >= 0) && (cln < input_clients_data.num_of_clients) &&
		(prt >= 0) && (prt < input_clients_data.clients_num_of_ports[cln]))
	{
		return input_clients_data.clients_ports_names[cln][prt];
	}
	else
	{
		return "";
	}
}

/**
*   @brief  Returns the name of JACK output client port.
*   @param  cln client index.
*   @param	prt port index
*   @return int the number of an JACK output client name; "" if param out of range.
*/
std::string JackConnections::get_jack_output_client_port_name(int cln, int prt)
{
	if ((cln >= 0) && (cln < output_clients_data.num_of_clients) &&
		(prt >= 0) && (prt < output_clients_data.clients_num_of_ports[cln]))
	{
		return output_clients_data.clients_ports_names[cln][prt];
	}
	else
	{
		return "";
	}
}

/**
*   @brief  Returns the number of active jack connections.
*   @param  none
*   @return int the number of active jack connectionse.
*/
int JackConnections::get_num_of_jack_clients_connections()
{
	return clients_connections_data.connections.size();
}

/**
*	@brief	Returns a list of input <"Client:Port> that connects to the given output client:port 
*	@param	client name
*	@param	client port name
*	@param	pointer to a list of strings
*	@return number of connections in list.
*/
int JackConnections::get_connected_jack_input_connections(std::string out_client_name,
									std::string out_client_port_name,
									std::list<std::string> *outputs_list)
{
	s_jack_connection_t connection;
	std::string input_connection_name_and_port;
	int num_of_connections = get_num_of_jack_clients_connections();

	if (num_of_connections <= 0)
	{
		return 0;
	}

	for (int con = 0; con < num_of_connections; con++)
	{
		connection = clients_connections_data.connections.at(con);

		if ((connection.out_client_name == out_client_name) &&
			(connection.out_client_port_name == out_client_port_name))
		{
			input_connection_name_and_port = connection.in_client_name +
											 ":" +
											 connection.in_client_port_name;
			outputs_list->push_back(input_connection_name_and_port);
		}
	}

	return outputs_list->size();
}

/**
*   @brief  Returns the connection input client name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection input client name 
*/
std::string JackConnections::get_jack_connection_in_client_name(int cln)
{
	if (cln >= clients_connections_data.connections.size())
	{
		return "";
	}
	else
	{
		return clients_connections_data.connections.at(cln).in_client_name;
	}
}

/**
*   @brief  Returns the connection input client port name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection input client port name 
*/
std::string JackConnections::get_jack_connection_in_client_port_name(int cln)
{
	if (cln >= clients_connections_data.connections.size())
	{
		return "";
	}
	else
	{
		return clients_connections_data.connections.at(cln).in_client_port_name;
	}
}

/**
*   @brief  Returns the connection output client name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection output client name. 
*/
std::string JackConnections::get_jack_connection_out_client_name(int cln)
{
	if (cln >= clients_connections_data.connections.size())
	{
		return "";
	}
	else
	{
		return clients_connections_data.connections.at(cln).out_client_name;
	}
}

/**
*   @brief  Returns the connection output client port name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection output client port name. 
*/
std::string JackConnections::get_jack_connection_out_client_port_name(int cln)
{
	if (cln >= clients_connections_data.connections.size())
	{
		return "";
	}
	else
	{
		return clients_connections_data.connections.at(cln).out_client_port_name;
	}
}

/**
*   @brief  Connects Jack input and output clients/ports.
*   @param  in_client_name			input client name
*   @param  in_client_port name		input client port name
*   @param  out_client_name			output client name
*   @param  out_client_port name	output client port name
*	@return 0 if done; -1 otherwise 
*/
int JackConnections::connect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name)
{
	char command[512];

	if ((in_client_name == "") || (in_client_port_name == "") ||
		(out_client_name == "") || (out_client_port_name == ""))
	{
		return -1;
	}

	// Verify it is a valid connection?
	sprintf(command,
			"jack_connect %s:%s %s:%s",
			in_client_name.c_str(),
			in_client_port_name.c_str(),
			out_client_name.c_str(),
			out_client_port_name.c_str());

	system(command);

	return 0;
}

/**
*   @brief  Disonnects Jack input and output clients/portse.
*   @param  in_client_name			input client name
*   @param  in_client_port name		input client port name
*   @param  out_client_name			output client name
*   @param  out_client_port name	output client port name
*	@return 0 if done; -1 otherwise 
*/
int JackConnections::disconnect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name)
{
	char command[512];

	if ((in_client_name == "") || (in_client_port_name == "") ||
		(out_client_name == "") || (out_client_port_name == ""))
	{
		return -1;
	}

	// Verify it is a valid connection?
	sprintf(command,
			"jack_disconnect %s:%s %s:%s",
			in_client_name.c_str(),
			in_client_port_name.c_str(),
			out_client_name.c_str(),
			out_client_port_name.c_str());

	system(command);

	return 0;
}

/**
*   @brief  Disonnects all Jack active connections.
*   @param  none
*	@return 0 if done; -1 otherwise 
*/
int JackConnections::disconnect_all_jack_connections()
{
	int c;

	for (c = 0; c < clients_connections_data.connections.size(); c++)
	{
		disconnect_jack_connection(
			clients_connections_data.connections.at(c).in_client_name,
			clients_connections_data.connections.at(c).in_client_port_name,
			clients_connections_data.connections.at(c).out_client_name,
			clients_connections_data.connections.at(c).out_client_port_name);
	}

	clients_connections_data.connections.clear();

	return 0;
}

/**
*	@brief	Initilizes clients data
*	@param	none
*	@return 0
*/
int JackConnections::init_jack_clients_data()
{
	clients_properties_data.data_type = _JACK_DATA_TYPE_PROPERTIES;
	clients_properties_data.num_of_clients = 0;
	clients_properties_data.clients_text_data = "";
	clients_properties_data.currently_processed_client_index = 0;
	clients_properties_data.currently_processed_client_name = "";
	clients_properties_data.currently_processed_port_index = 0;
	clients_properties_data.currently_processed_connection_index = 0;
	clients_properties_data.currently_processed_connection_client_name = "";
	clients_properties_data.currently_processed_connection_port_name = "";
	clients_properties_data.clients_data_text_lines_list.clear();
	clients_properties_data.connections.clear();

	clients_connections_data.data_type = _JACK_DATA_TYPE_CONNECTIONS;
	clients_connections_data.num_of_clients = 0;
	clients_connections_data.clients_text_data = "";
	clients_connections_data.currently_processed_client_index = 0;
	clients_connections_data.currently_processed_client_name = "";
	clients_connections_data.currently_processed_port_index = 0;
	clients_connections_data.currently_processed_connection_index = 0;
	clients_connections_data.currently_processed_connection_client_name = "";
	clients_connections_data.currently_processed_connection_port_name = "";
	clients_connections_data.clients_data_text_lines_list.clear();
	clients_connections_data.connections.clear();

	input_clients_data.data_type = _JACK_DATA_TYPE_INPUT_CLIENTS;
	input_clients_data.num_of_clients = 0;
	input_clients_data.clients_text_data = "";
	input_clients_data.currently_processed_client_index = 0;
	input_clients_data.currently_processed_client_name = "";
	input_clients_data.currently_processed_port_index = 0;
	input_clients_data.currently_processed_connection_index = 0;
	input_clients_data.currently_processed_connection_client_name = "";
	input_clients_data.currently_processed_connection_port_name = "";
	input_clients_data.clients_data_text_lines_list.clear();
	input_clients_data.connections.clear();

	output_clients_data.data_type = _JACK_DATA_TYPE_OUTPUT_CLIENTS;
	output_clients_data.num_of_clients = 0;
	output_clients_data.clients_text_data = "";
	output_clients_data.currently_processed_client_index = 0;
	output_clients_data.currently_processed_client_name = "";
	output_clients_data.currently_processed_port_index = 0;
	output_clients_data.currently_processed_connection_index = 0;
	output_clients_data.currently_processed_connection_client_name = "";
	output_clients_data.currently_processed_connection_port_name = "";
	output_clients_data.clients_data_text_lines_list.clear();
	output_clients_data.connections.clear();

	for (int cl = 0; cl < max_num_of_jack_clients; cl++)
	{
		clients_properties_data.clients_name[cl] = "";
		clients_properties_data.clients_num_of_ports[cl] = 0;

		clients_connections_data.clients_name[cl] = "";
		clients_connections_data.clients_num_of_ports[cl] = 0;

		input_clients_data.clients_name[cl] = "";
		input_clients_data.clients_num_of_ports[cl] = 0;

		output_clients_data.clients_name[cl] = "";
		output_clients_data.clients_num_of_ports[cl] = 0;

		for (int pr = 0; pr < max_num_of_jack_ports; pr++)
		{
			clients_properties_data.clients_ports_names[cl][pr] = "";
			clients_properties_data.clients_ports_properties[cl][pr] = 0;

			clients_connections_data.clients_ports_names[cl][pr] = "";
			clients_connections_data.clients_ports_properties[cl][pr] = 0;

			input_clients_data.clients_ports_names[cl][pr] = "";
			input_clients_data.clients_ports_properties[cl][pr] = 0;

			output_clients_data.clients_ports_names[cl][pr] = "";
			output_clients_data.clients_ports_properties[cl][pr] = 0;
		}
	}

	return 0;
}

/**
*	@brief	Activate jack_lsp commands (-p, -c), captures stdout response strings.
*	@param	none
*	@return 0
*/
int JackConnections::get_jack_clients_data()
{
	clients_properties_data.clients_text_data = Utils::execute_system_command_getstd_out("jack_lsp -p");
	clients_connections_data.clients_text_data = Utils::execute_system_command_getstd_out("jack_lsp -c");

	return 0;
}

/**
*	@brief	Parse the data text string into lines terminated by \n
*	@param	data_text pointer to a string that holds the data to be parsed.
*	@return a vector of text lines strings
*/
std::vector<s_jack_text_line_t> JackConnections::parse_data_text_lines(std::string *data_text)
{
	std::vector<s_jack_text_line_t> lines;
	int new_position = 0, last_position = 0;
	s_jack_text_line_t line;

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
int JackConnections::parse_jack_clients_data_text_lines()
{
	clients_properties_data.clients_data_text_lines_list = parse_data_text_lines(&clients_properties_data.clients_text_data);
	clients_connections_data.clients_data_text_lines_list = parse_data_text_lines(&clients_connections_data.clients_text_data);

	return 0;
}

/**
*	@brief	Parses a text line and get its type (client, porperties connection).
*	@param	text_line_data	pointer to a s_jack_text_line_t structure
*	@return line type; -1 if illegale
*/
int JackConnections::parse_text_line_type(s_jack_text_line_t *text_line_data)
{
	if ((text_line_data == NULL) || (text_line_data->text_line.size() == 0))
	{
		return -1;
	}

	text_line_data->type = -1;

	if ((text_line_data->text_line.at(0) == '\t') &&
		(text_line_data->text_line.find("properties:", 1) >= 1))
	{
		// "		properties: output, terminal,"
		text_line_data->type = _JACK_LINE_TYPE_PROPERTIES;
	}
	else if ((text_line_data->text_line.at(0) == ' ') &&
			 (text_line_data->text_line.find_first_of(":", 1) >= 2))
	{
		// "  Client:Port"
		text_line_data->type = _JACK_LINE_TYPE_CONNECTION;
	}
	else if ((text_line_data->text_line.find_first_of("abcdefghijklmnopABCDEFGHIJKLMNOP", 0) >= 0) &&
			 (text_line_data->text_line.find_first_of(":", 1) >= 1))
	{
		// "Client:Port"
		text_line_data->type = _JACK_LINE_TYPE_CLIENT_PORT;
	}

	return text_line_data->type;
}

/**
*	@brief	Parses all text lines and get there types (client, porperties connection).
*	@param	clients_data	a pointer to a s_jack_clients_data_t structure
*	@return 0 if done; else -1
*/
int JackConnections::parse_text_lines_types(s_jack_clients_data_t *clients_data)
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
*	@brief	Parses all captured clients data text lines and get there types (client, porperties connection).
*	@param	none
*	@return 0 if done; else -1
*/
int JackConnections::parse_jack_clients_data_text_lines_types()
{
	int res = 0;

	res = parse_text_lines_types(&clients_properties_data);
	res |= parse_text_lines_types(&clients_connections_data);

	return res;
}

/**
*	@brief	Parses and process a "client:port" text line. "Client:Port"
*	@param	text_line	pointer to a text line string
*	@param	clients_data	pointer to a s_jack_clients_data_t structure
*	@return 0 if a valid line; else -1
*/
int JackConnections::process_client_port_text_line(std::string *text_line, s_jack_clients_data_t *clients_data)
{
	int colons_pos;
	std::string client_name, port_name;

	if (text_line == NULL)
	{
		return -1;
	}

	if (text_line->length() < 12)
	{
		return -1;
	}

	colons_pos = text_line->find_first_of(":", 0);
	if (colons_pos == std::string::npos)
	{
		return -1;
	}

	client_name = text_line->substr(0, colons_pos);
	port_name = text_line->substr(colons_pos + 1, text_line->length() - colons_pos);

	if ((client_name.length() == 0) || (port_name.length() == 0))
	{
		return -1;
	}
	else
	{
		clients_data->currently_processed_client_name = client_name;
		clients_data->currently_processed_port_name = port_name;
	}

	return 0;
}

/**
*	@brief	Parses and process a "properties:" text line. "		properties: output, terminal,"
*	@param	text_line	pointer to a text line string
*	@param	clients_data	pointer to a s_jack_clients_data_t structure
*	@return properties bit-mask if a valid line; else -1
*/
int JackConnections::process_porperties_text_line(std::string *text_line, s_jack_clients_data_t *clients_data)
{
	int properties = 0;
	int pos;

	if (text_line == NULL)
	{
		return -1;
	}

	if (text_line->length() < 12)
	{
		return -1;
	}

	pos = text_line->find("input");
	if (pos != std::string::npos)
	{
		properties |= _JACK_CLIENT_PROPERTY_BIT_INPUT;
	}

	pos = text_line->find("output");
	if (pos != std::string::npos)
	{
		properties |= _JACK_CLIENT_PROPERTY_BIT_OUTPUT;
	}

	pos = text_line->find("terminal");
	if (pos != std::string::npos)
	{
		properties |= _JACK_CLIENT_PROPERTY_BIT_TERMINAL;
	}

	pos = text_line->find("physical");
	if (pos != std::string::npos)
	{
		properties |= _JACK_CLIENT_PROPERTY_BIT_PHYSICAL;
	}

	return properties;
}

/**
*	@brief	Parses and process a connections "   client:port"  text line.
*	@param	text_line	pointer to a text line string
*	@param	clients_data	pointer to a s_jack_clients_data_t structure
*	@return 0 if a valid line; else -1
*/
int JackConnections::process_connection_text_line(std::string *text_line, s_jack_clients_data_t *clients_data)
{
	int name_start_pos, colons_pos;
	std::string client_name, port_name;

	if (text_line == NULL)
	{
		return -1;
	}

	if (text_line->length() < 12)
	{
		return -1;
	}

	name_start_pos = text_line->find_first_not_of(" ");
	if (name_start_pos == std::string::npos)
	{
		return -1;
	}

	colons_pos = text_line->find_first_of(":", 0);
	if (colons_pos == std::string::npos)
	{
		return -1;
	}

	client_name = text_line->substr(name_start_pos, colons_pos - name_start_pos);
	port_name = text_line->substr(colons_pos + 1, text_line->length() - colons_pos);

	if ((client_name.length() == 0) || (port_name.length() == 0))
	{
		return -1;
	}
	else
	{
		clients_data->currently_processed_connection_client_name = client_name;
		clients_data->currently_processed_connection_port_name = port_name;
	}

	return 0;
}

/**
*   @brief  Process a text line based on its type.
*   @param  line_num  line number
*   @param	clients_data	pointer to a s_jack_clients_data_t structure
*   @return line type; -1 otherwise.
*/
int JackConnections::process_text_line(int line_num, s_jack_clients_data_t *clients_data)
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

	if (clients_data->data_type == _JACK_DATA_TYPE_PROPERTIES)
	{
		if (clients_data->clients_data_text_lines_list.at(line_num).type == _JACK_LINE_TYPE_CLIENT_PORT)
		{
			clients_data->currently_processed_client_index++; // starts at -1
			clients_data->currently_processed_port_index++;
			res = process_client_port_text_line(&clients_data->clients_data_text_lines_list.at(line_num).text_line, clients_data);
			if (res == 0)
			{
				if ((clients_data->currently_processed_client_index == 0) &&
					(clients_data->currently_processed_port_index == 0))
				{
					// 1st client and port
					clients_data->clients_name[clients_data->currently_processed_client_index] =
						clients_data->currently_processed_client_name;
					clients_data->clients_num_of_ports[clients_data->currently_processed_client_index] = 1;
					clients_data->clients_ports_names[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index] =
						clients_data->currently_processed_port_name;
				}
				else
				{
					if (clients_data->currently_processed_client_name == clients_data->clients_name[clients_data->currently_processed_client_index - 1])
					{
						// Additional port to previous client
						clients_data->currently_processed_client_index -= 1;
						clients_data->clients_ports_names[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index] =
							clients_data->currently_processed_port_name;
						clients_data->clients_num_of_ports[clients_data->currently_processed_client_index] += 1;
					}
					else
					{
						// New client

						clients_data->currently_processed_port_index = 0;
						clients_data->clients_name[clients_data->currently_processed_client_index] =
							clients_data->currently_processed_client_name;
						clients_data->clients_ports_names[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index] =
							clients_data->currently_processed_port_name;
						clients_data->clients_num_of_ports[clients_data->currently_processed_client_index] = 1;
					}
				}
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
		else if (clients_data->clients_data_text_lines_list.at(line_num).type == _JACK_LINE_TYPE_PROPERTIES)
		{
			res = process_porperties_text_line(&clients_data->clients_data_text_lines_list.at(line_num).text_line, clients_data);
			if (res >= 0)
			{
				clients_data->clients_ports_properties[clients_data->currently_processed_client_index][clients_data->currently_processed_port_index] = res;
			}
		}
		else
		{
			// A unknown line type
			return -1;
		}
	}
	else if (clients_data->data_type == _JACK_DATA_TYPE_CONNECTIONS)
	{
		if (clients_data->clients_data_text_lines_list.at(line_num).type == _JACK_LINE_TYPE_CLIENT_PORT)
		{
			clients_data->currently_processed_client_index++; // starts at -1
			clients_data->currently_processed_port_index++;
			res = process_client_port_text_line(&clients_data->clients_data_text_lines_list.at(line_num).text_line, clients_data);
			if (res != 0)
			{
				// Not a valid client line;
				if (clients_data->currently_processed_client_index > 0)
				{
					clients_data->currently_processed_client_index = -1;
				}
			}
		}
		else if (clients_data->clients_data_text_lines_list.at(line_num).type == _JACK_LINE_TYPE_CONNECTION)
		{
			res = process_connection_text_line(&clients_data->clients_data_text_lines_list.at(line_num).text_line, clients_data);
			if (res == 0)
			{
				// Valid connection
				res = add_jack_connection(
					clients_data->currently_processed_client_name,
					clients_data->currently_processed_port_name,
					clients_data->currently_processed_connection_client_name,
					clients_data->currently_processed_connection_port_name);
			}
		}
		else
		{
			// A unknown line type
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}

/**
*	@brief	Parses clients data.
*	@param	none
*	@return num of clients if ok; -1 if error
*/
int JackConnections::parse_jack_client_data(s_jack_clients_data_t *clients_data)
{
	int line_index = 0;
	int res;

	if (clients_data == NULL)
	{
		return -1;
	}

	clients_data->currently_processed_client_index = -1;
	clients_data->currently_processed_port_index = -1;
	clients_data->currently_processed_connection_index = -1;
	clients_data->currently_processed_client_name = "";
	clients_data->currently_processed_port_name = "";

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
			//			clients_data->clients_data_text_lines_list.at(line_index).client = clients_data->currently_processed_client_index;
			//			clients_data->clients_data_text_lines_list.at(line_index).port = clients_data->currently_processed_port_index;
			line_index++;
		}
	}

	clients_data->num_of_clients = clients_data->currently_processed_client_index + 1;

	return clients_data->num_of_clients;
}

/**
*	@brief	Parses the data for all clients.
*	@param	none
*	@return 0 ok; -1 if error
*/
int JackConnections::parse_jack_clients_data()
{
	int res;

	res = parse_jack_client_data(&clients_properties_data);
	res |= parse_jack_client_data(&clients_connections_data);

	return res;
}

/**
*	@brief	Copy source client data to destination client data.
*	@param	src_client_index	source client index
*	@param	src_port_index		source client port num
*	@param	src_clients_data	pointer to source s_jack_clients_data_t struct
*	@param	dest_client_index	destination client index
*	@param	dest_port_index		dest client port num
*	@param	dest_clients_data	pointer to destination s_jack_clients_data_t struct
*	@return 0 ok; -1 if error
*/
int JackConnections::copy_client_data(
	int src_client_index,
	int src_port_index,
	s_jack_clients_data_t *src_clients_data,
	int dest_client_index,
	int dest_port_index,
	s_jack_clients_data_t *dest_clients_data)
{
	if ((src_clients_data == NULL) || (dest_clients_data == NULL))
	{
		return -1;
	}

	if ((src_client_index >= src_clients_data->num_of_clients) ||
		(dest_client_index >= max_num_of_jack_clients) ||
		(src_port_index >= src_clients_data->clients_num_of_ports[src_client_index]) ||
		(dest_port_index >= max_num_of_jack_ports))
	{
		return -1;
	}

	dest_clients_data->clients_name[dest_client_index] = src_clients_data->clients_name[src_client_index];
	dest_clients_data->clients_num_of_ports[dest_client_index] = src_clients_data->clients_num_of_ports[dest_client_index];

	dest_clients_data->clients_ports_names[dest_client_index][dest_port_index] =
		src_clients_data->clients_ports_names[src_client_index][src_port_index];
	dest_clients_data->clients_ports_properties[dest_client_index][dest_port_index] =
		src_clients_data->clients_ports_properties[src_client_index][src_port_index];

	return 0;
}

/**
*	@brief	Sorts clients/ports into inputs -> input_clients_data and
*			outputs -> output_clients_data.
*	@param	none
*	@return 0 ok; -1 if error
*/
int JackConnections::sort_jack_clients()
{
	int in_index = 0, out_index = 0;
	bool in_client = false, out_client = false;

	for (int cl = 0; cl < clients_properties_data.num_of_clients; cl++)
	{
		for (int p = 0; p < clients_properties_data.clients_num_of_ports[cl]; p++)
		{
			if ((clients_properties_data.clients_ports_properties[cl][p] & _JACK_CLIENT_PROPERTY_BIT_INPUT) != 0)
			{
				in_client = true;
				copy_client_data(cl, p, &clients_properties_data, in_index, p, &input_clients_data);
			}
			if ((clients_properties_data.clients_ports_properties[cl][p] & _JACK_CLIENT_PROPERTY_BIT_OUTPUT) != 0)
			{
				out_client = true;
				copy_client_data(cl, p, &clients_properties_data, out_index, p, &output_clients_data);
			}
		}

		if (in_client)
		{
			in_client = false;
			in_index++;
		}

		if (out_client)
		{
			out_client = false;
			out_index++;
		}
	}

	input_clients_data.num_of_clients = in_index;
	output_clients_data.num_of_clients = out_index;

	return 0;
}

/**
*	@brief	Add an active connection.
*			Each connection appears twice, once: input->output and once output<-input.
*			Add each connection only once input->output (as apears in input_clients_data).
*			Connections are added to the clients_connections_data connections vector
*	@param	in_client_name			input client name
*	@param	in_client_port_name		input client port name
*	@param	out_client_name			output client name
*	@param	out_client_port_name	output client port name
*	@return 0 ok; -1 if error or not an input connection
*/
int JackConnections::add_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name)
{
	int cl, prt;
	bool found = false;
	s_jack_connection_t connection;

	if ((in_client_name == "") || (in_client_port_name == "") ||
		(out_client_name == "") || (out_client_port_name == ""))
	{
		return -1;
	}

	// Verify its an input connection
	for (cl = 0; !found && cl < input_clients_data.num_of_clients; cl++)
	{
		for (prt = 0; prt < input_clients_data.clients_num_of_ports[cl]; prt++)
		{
			if ((input_clients_data.clients_name[cl] == in_client_name) &&
				(input_clients_data.clients_ports_names[cl][prt] == in_client_port_name))
			{
				// A valid input client connection TODO: verify a valid output port?
				connection.in_client_name = in_client_name;
				connection.in_client_port_name = in_client_port_name;
				connection.out_client_name = out_client_name;
				connection.out_client_port_name = out_client_port_name;

				clients_connections_data.connections.push_back(connection);
				// break
				found = true;
			}

			if (found)
			{
				break;
			}
		}
	}

	return 0;
}
