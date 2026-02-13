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

#pragma once

#include <string>
#include <list>

#include "../ALSA/alsaMidiSystemControl.h"

using namespace std;

class InstrumentConnectionsControl
{
  public:
	InstrumentConnectionsControl();
	~InstrumentConnectionsControl();

	int connect_to_alsa_midi_in_client();

	string get_alsa_midi_in_client_name(int client_num);

	// int get_alsa_midi_in_clients_names_list(list<string> *names_list);

	int get_alsa_input_client_id();

	AlsaMidiSysControl *alsa_midi_system_contrl;
	
  private:
	
	
};
