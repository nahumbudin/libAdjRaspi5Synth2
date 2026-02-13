/**
*	@file		controlBoxClientAlsaOutput.h
*	@author		Nahum Budin
*	@date		27-6-2024
*	@version	1.1 
*					1. Customizing to Control Box controls
*
*	@brief		Serial Port input based alsa output client (serial->alsa)
*	
*	History:
*				Ver 1.0 27-11-2020   serialPortClientAlsaOutput.h
*				
*/

#pragma once

#include <pthread.h>
#include <alsa/asoundlib.h>

#include "../utils/safeQueues.h"
#include "../Serial/serialPort.h"

class ControlBoxClientAlsaOutput
{
  public:
	~ControlBoxClientAlsaOutput();

	static ControlBoxClientAlsaOutput *get_instance();

	void startAlsaOutThread();
	void stopAlsaOutThread();

  private:
	ControlBoxClientAlsaOutput();

	static void *alsaOutThread(void *threadid);

	/* When true the Serial Port alsa out thread is running when false the thread will terminate */
	static bool alsa_out_thread_is_running;
	/* Serial port alsa out thread handler */
	static pthread_t alsa_out_thread_id;

	int mode;
	static snd_rawmidi_t *midiout;

	static ControlBoxClientAlsaOutput *control_box_client_alsa_output;
};
