/**
*	@file		midiExtInterface.h
*	@author		Nahum Budin
*	@date		15-May-2024
*	@version	1.0
*
*	@brief		Handeles the external MIDI interfaces (including the device knobs, sliders, etc.
*				Note - this serial port interfaces to the device controller board that hosts the
*					   32500 MIDI UART.
*	
*/

#pragma once

#include "../Serial/serialPort.h"

class MidiExtInterface
{
	
	
public:
	MidiExtInterface(int serial_port_num);
	~MidiExtInterface();
	
	static MidiExtInterface* get_midi_ext_interface_instance();
	
	/**
	*   @brief  Initializes the Serial Port services.
	*   @param  none
	*   @return 0 if done
	*/
	int init_serial_port_services(int port_num = 1,
		int baud = 115200, 
		const char *mode = "8N1",
		int flow_ctl = 0); // 0=FLOW_CONTROL_DISABLED
	
private:
	
	static int serial_port_number;
	
	static SerialPort *serial_port;
	
	
	
	static MidiExtInterface* midi_ext_int_instance;
	
};

