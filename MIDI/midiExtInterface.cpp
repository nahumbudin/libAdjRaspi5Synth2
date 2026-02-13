/**
*	@file		midiExtInterface.cpp
*	@author		Nahum Budin
*	@date		15-May-2024
*	@version	1.0
*
*	@brief		Handeles the external MIDI interfaces (including the device knobs, sliders, etc.
*				Note - this serial port interfaces to the device controller board that hosts the
*					   32500 MIDI UART.
*	
*/

#include "midiExtInterface.h"

MidiExtInterface* MidiExtInterface::midi_ext_int_instance;
int MidiExtInterface::serial_port_number;
SerialPort *MidiExtInterface::serial_port;

MidiExtInterface::MidiExtInterface(int serial_port_num)
{
	midi_ext_int_instance = this;
	
	serial_port_number = serial_port_num;
	serial_port = SerialPort::get_serial_port_instance(serial_port_num);
	

	
	
}

MidiExtInterface::~MidiExtInterface()
{
	
	
}

MidiExtInterface* MidiExtInterface::get_midi_ext_interface_instance()
{
	if (midi_ext_int_instance == NULL)
	{
		midi_ext_int_instance = new MidiExtInterface(serial_port_number);
	}
	
	return midi_ext_int_instance;
}

/**
*   @brief  Initializes the Serial Port services.
*   @param  none
*   @return 0 if done
*/
int MidiExtInterface::init_serial_port_services(int port_num, int baud, const char *mode, int flow_ctl)
{
	int res;
	
	res = serial_port->open_port(port_num, baud, mode, flow_ctl);
	if (res != 0)
	{
		return -1;
	}

	serial_port->start_serial_port_RX_thread();
	serial_port->start_serial_port_TX_thread();

	return 0;	
}



