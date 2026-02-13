/**
 *	@file		alsaMidiSystemControl.h
 *	@author		Nahum Budin
 *	@date		21-Dec-2025
 *	@version	1.2
 *					1.	Increasing the number of alsa midi clients and ports to 32
 *
 *	@brief		Use system "aconnect" commands to scan and control ALSA midi connections.
 *
 *	History:\n
 *		version 1.1		21-Dec-2025
 *		version 1.0		6-Mar-2021
 */

#pragma once

#include <string>
#include <vector>
#include <list>

using namespace std;

#define _CLIENTS_TYPE_DATA_INPUT 0
#define _CLIENTS_TYPE_DATA_OUTPUT 1
#define _CLIENTS_TYPE_DATA_CONNECTIONS 2

#define _LINE_TYPE_CLIENT 0
#define _LINE_TYPE_PORT 1
#define _LINE_TYPE_CONNECTIONS 2



const int max_alsa_data_string_length = 2048;
static const int max_num_of_alsa_clients = 32;
static const int max_num_of_alsa_ports = 32;

typedef struct s_alsa_clients_connection
{
	int connected_client;
	int connected_port;
	int connecting_to_client_num;
	int connecting_to_port_num;
} s_alsa_clients_connection_t;

typedef struct s_alsa_text_line
{
	// Line type: client, port, connection
	int type;
	// Each text line refers to a specific client
	int client;
	// Connections text lines refers to specific port
	int port;
	string text_line;
} s_alsa_text_line_t;

typedef struct s_alsa_clients_data
{
	// in/out/connections
	int data_type;
	int num_of_clients;
	// Clients numbers
	int clients_num[max_num_of_alsa_clients];
	// Clients names
	string clients_name[max_num_of_alsa_clients];
	// Clients types
	string clients_type[max_num_of_alsa_clients];
	// Number of ports of each client
	int clients_num_of_ports[max_num_of_alsa_clients];
	// Ports numbers of each input client
	int clients_ports[max_num_of_alsa_clients][max_num_of_alsa_ports];
	// Ports names of each input client
	string clients_ports_names[max_num_of_alsa_clients][max_num_of_alsa_ports];
	// Currently processed client index
	int currently_processed_client_index;
	// Currently processed port index
	int currently_processed_port_index;
	// Currently processed connection index
	int currently_processed_connection_index;
	// Holds the clients each client is connected to (used only for input clients)
	vector<s_alsa_clients_connection_t> connected_to_list;

	// Holds the parsed lines data
	vector<s_alsa_text_line_t> clients_data_text_lines_list;
	// Holds the stdout redirected text
	string clients_text_data;
} s_alsa_clients_data_t;

class AlsaMidiSysControl
{
  public:
	static AlsaMidiSysControl *get_instance();

	int refresh_alsa_clients_data();

	int get_num_of_input_midi_clients();
	int get_num_of_output_midi_clients();

	int get_num_of_input_midi_client_ports(int cln);
	int get_num_of_output_midi_client_ports(int cln);

	void get_midi_input_client_name_string(int cln, string *name, bool replace = false);
	void get_midi_output_client_name_string(int cln, string *name);
	int get_alsa_midi_in_clients_names_list(list<string> *names_list);

	int get_midi_input_client_id(string name, bool replace = true);
	int get_midi_output_client_id(string name);

	void get_midi_input_client_port_name_string(int cln, int port, string *name);
	void get_midi_output_client_port_name_string(int cln, int port, string *name);

	int connect_midi_clients(int in_client, int in_port, int out_client, int out_port, bool use_index=true);
	int disconnect_midi_clients(int in_client, int in_port, int out_client, int out_port, bool use_index=true);
	int disconnect_all_midi_clients();

	int get_num_of_midi_clients_connections();
	int get_midi_client_connection_connected_client_num(int con_num);
	int get_midi_client_connection_connected_port_num(int con_num);
	int get_midi_client_connection_connecting_to_client_num(int con_num);
	int get_midi_client_connection_connecting_to_port_num(int con_num);

	int get_midi_client_connection_id(string name);
	int get_midi_client_connection_num(string name);

	int get_midi_client_connected_to_clients_numbers_list(int cln_num, list<int> *cln_lst);

	int get_midi_client_connected_to_clients_names_list(int cln_num, bool replace, list<string> *cln_lst);

	std::string strip_client_name_prefix(string name);

	static string bt_client_in_name;
	static string control_box_client_in_name;
	static string control_box_xt_midi_in_client_name;
	static string midi_player_client_in_name;
	static string midi_mapper_client_in_name;

  private:
	AlsaMidiSysControl();

	int init_clients_data();
	int get_alsa_clients_data();

	vector<s_alsa_text_line_t> parse_data_text_lines(string *data_text);
	int parse_alsa_clients_data_text_lines();
	int parse_text_line_type(s_alsa_text_line_t *text_line_data);
	int parse_text_lines_types(s_alsa_clients_data_t *clients_data);
	int parse_clients_data_text_lines_types();

	int process_client_text_line(string *text_line, s_alsa_clients_data_t *clients_data);
	int process_port_text_line(string *text_line, s_alsa_clients_data_t *clients_data);
	int process_connecting_to_text_line(string *text_line, s_alsa_clients_data_t *clients_data);
	int process_connected_from_text_line(string *text_line, s_alsa_clients_data_t *clients_data);
	bool first_non_space_char_is_digit(string *text_line);
	int process_text_line(int line_num, s_alsa_clients_data_t *clients_data);
	int parse_alsa_client_data(s_alsa_clients_data_t *clients_data);
	int parse_alsa_clients_data();

	s_alsa_clients_data_t input_clients_data;
	s_alsa_clients_data_t output_clients_data;
	s_alsa_clients_data_t clients_connections_data;

	static AlsaMidiSysControl *alsa_midi_sys_control_instance;
};


