/**
* @file		instrumentConnectionsControl.h
*	@author		Nahum Budin
*	@date		4-Jul-2024
*	@version	1.0	Initial release
*					
*	@brief		Provides control of the instrument ALSA MIDI and Jacj Audio connections.
*	
*	History:\n
*	
*/

#include "instrumentConnectionsControl.h"

InstrumentConnectionsControl::InstrumentConnectionsControl()
{
	alsa_midi_system_contrl = AlsaMidiSysControl::get_instance();
}

InstrumentConnectionsControl::~InstrumentConnectionsControl()
{
	
}

/**
*   @brief  Returns the name of the client that is on the given place on the list.
*			If the client name is equal to the detected BT input client name, the name is exchanged.
*			The same for the Control Box input client and the Control Box Ext MIDI interface.
*   @param  int client num on the list
*   @return string client name
*	
*/
std::string InstrumentConnectionsControl::get_alsa_midi_in_client_name(int client_num)
{
	std::string name;


	alsa_midi_system_contrl->get_midi_input_client_name_string(client_num, &name);

	if ((alsa_midi_system_contrl->bt_client_in_name != "") &&
		(name == alsa_midi_system_contrl->bt_client_in_name))
	{
		name = "Bluetooth MIDI (" + name + ")";
	}
	else if ((alsa_midi_system_contrl->control_box_client_in_name != "") &&
			(name == alsa_midi_system_contrl->control_box_client_in_name))
	{
		name = "Control Box (" + name + ")";
	}
	else if ((alsa_midi_system_contrl->control_box_xt_midi_in_client_name != "") &&
			(name == alsa_midi_system_contrl->control_box_xt_midi_in_client_name))
	{
		name = "Control Box Ext MIDI (" + name + ")";
	}

	return name;
}

/**
*   @brief  Returns the number of MIDI in clients and an ordered list of the clients name.
*   @param  std::list<std::string>* a pointer to the list of names
*   @return int number of clients
*	
*/

/*
int InstrumentConnectionsControl::get_alsa_midi_in_clients_names_list(std::list<std::string> *names_list)
{
	int num_of_clients = alsa_midi_system_contrl->get_num_of_input_midi_clients();

	if ((names_list == NULL) || (num_of_clients <= 0)) 
	{
		return -1;
	}

	for (int i = 0; i < num_of_clients; i++)
	{
		names_list->push_back(get_alsa_midi_in_client_name(i));
	}

	return num_of_clients;
}

*/
