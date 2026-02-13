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

#pragma once

#include <list>

#define _JACK_DATA_TYPE_PROPERTIES 0
#define _JACK_DATA_TYPE_CONNECTIONS 1
#define _JACK_DATA_TYPE_INPUT_CLIENTS 2
#define _JACK_DATA_TYPE_OUTPUT_CLIENTS 3

#define _JACK_LINE_TYPE_CLIENT_PORT 0
#define _JACK_LINE_TYPE_PROPERTIES 1
#define _JACK_LINE_TYPE_CONNECTION 2

#define _JACK_CLIENT_PROPERTY_BIT_INPUT 1 << 0
#define _JACK_CLIENT_PROPERTY_BIT_OUTPUT 1 << 1
#define _JACK_CLIENT_PROPERTY_BIT_TERMINAL 1 << 2
#define _JACK_CLIENT_PROPERTY_BIT_PHYSICAL 1 << 3

#include <string>
#include <vector>

#include "../LibAPI/audio.h"

const int max_jack_data_string_length = 2048;
static const int max_num_of_jack_clients = _MAX_NUM_OF_JACK_CLIENTS;
static const int max_num_of_jack_ports = _MAX_NUM_OF_JACK_CLIENT_PORTS;

typedef struct s_jack_text_line
{
	// Line type:
	// Lint type: clint:port, properties, connection
	int type;
	std::string text_line;
} s_jack_text_line_t;

typedef struct s_jack_connection
{
	std::string in_client_name;
	std::string in_client_port_name;
	std::string out_client_name;
	std::string out_client_port_name;
} s_jack_connection_t;

typedef struct s_jack_clients_data
{
	// Holds the data type (properties, connections)
	int data_type;
	// Number of clients
	int num_of_clients;

	// Currently processed client index
	int currently_processed_client_index;
	// Curently processed client name
	std::string currently_processed_client_name;
	// Currently processed port index
	int currently_processed_port_index;
	// Curently processed port name
	std::string currently_processed_port_name;
	// Currently processed connection index
	int currently_processed_connection_index;
	// Curently processed connection client name
	std::string currently_processed_connection_client_name;
	// Curently processed connection port name
	std::string currently_processed_connection_port_name;
	// Number of ports of each client
	int clients_num_of_ports[max_num_of_jack_clients];
	// Clients names
	std::string clients_name[max_num_of_jack_clients];
	// Ports names of each  client
	std::string clients_ports_names[max_num_of_jack_clients][max_num_of_jack_ports];
	// Ports properties bits-mask of each  client
	uint16_t clients_ports_properties[max_num_of_jack_clients][max_num_of_jack_ports];

	// Holds the parsed lines data
	std::vector<s_jack_text_line_t> clients_data_text_lines_list;
	// Holds the stdout redirected text
	std::string clients_text_data;
	// Holds active connections
	std::vector<s_jack_connection_t> connections;

} s_jack_clients_data_t;

class JackConnections
{
  public:
	static JackConnections *get_instance();

	int refresh_jack_clients_data();

	int get_num_of_input_jack_clients();
	int get_num_of_output_jack_clients();

	int get_num_of_jack_input_client_ports(int cln);
	int get_num_of_jack_output_client_ports(int cln);

	std::string get_jack_input_client_name(int cln);
	std::string get_jack_output_client_name(int cln);

	std::string get_jack_input_client_port_name(int cln, int prt);
	std::string get_jack_output_client_port_name(int cln, int prt);

	int get_num_of_jack_clients_connections();

	int get_connected_jack_input_connections(std::string out_client_name, std::string out_client_port_name,
											 std::list<std::string> *outputs_list);

	std::string get_jack_connection_in_client_name(int cln);
	std::string get_jack_connection_in_client_port_name(int cln);
	std::string get_jack_connection_out_client_name(int cln);
	std::string get_jack_connection_out_client_port_name(int cln);

	int connect_jack_connection(
		std::string in_client_name,
		std::string in_client_port_name,
		std::string out_client_name,
		std::string out_client_port_name);
	int disconnect_jack_connection(
		std::string in_client_name,
		std::string in_client_port_name,
		std::string out_client_name,
		std::string out_client_port_name);
	int disconnect_all_jack_connections();

  private:
	JackConnections();

	int init_jack_clients_data();
	int get_jack_clients_data();

	std::vector<s_jack_text_line_t> parse_data_text_lines(std::string *data_text);
	int parse_jack_clients_data_text_lines();
	int parse_text_line_type(s_jack_text_line_t *text_line_data);
	int parse_text_lines_types(s_jack_clients_data_t *clients_data);
	int parse_jack_clients_data_text_lines_types();

	int process_client_port_text_line(std::string *text_line, s_jack_clients_data_t *clients_data);
	int process_porperties_text_line(std::string *text_line, s_jack_clients_data_t *clients_data);
	int process_connection_text_line(std::string *text_line, s_jack_clients_data_t *clients_data);
	int process_text_line(int line_num, s_jack_clients_data_t *clients_data);
	int parse_jack_client_data(s_jack_clients_data_t *clients_data);
	int parse_jack_clients_data();

	int copy_client_data(
		int src_client_index, int src_port_index,
		s_jack_clients_data_t *src_clients_data,
		int dest_client_index, int dest_port_index,
		s_jack_clients_data_t *dest_clients_data);

	int sort_jack_clients();

	int add_jack_connection(
		std::string in_client_name,
		std::string in_client_port_name,
		std::string out_client_name,
		std::string out_client_port_name);

	static JackConnections *jack_connections_instance;

	s_jack_clients_data_t clients_properties_data;
	s_jack_clients_data_t clients_connections_data;

	s_jack_clients_data_t input_clients_data;
	s_jack_clients_data_t output_clients_data;

};
