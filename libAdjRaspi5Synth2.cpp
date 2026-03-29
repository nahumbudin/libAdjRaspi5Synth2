/**
* @file		libAdjRaspi5Synth_1_1.cpp
*	@author		Nahum Budin
*	@date		22-Sep-2025
*	@version	1.1
*					1. Code refactoring rename modules to instruments and patches to preset parameters.
*					2. Replacing all analog synthgesizers instruments Jack output ports
*						name and port names with AdjSynth names.
*					3. Add AdjSynth panic pushbutton
*					4. Added MIDI mixer Pan modulation LFO level and selection control definitions
 *					5. Added MIDI mixer Send control definition
*
*	History:
*			Ver1.0  8-May-2024 Initial
*	
*\n
*
*
*
*	@brief		A raspberry Pi multi-core polyphonic music synthesizer library api.
*
*	\mainpage AdjHeart Polyphonic Synthesizer
*
*	This library was writen to run over a Raspberry Pi 5B.\n
*	The librarry implements a modular synthesizer \n
*
*
*	It provides a full API supporting all features incluuding callback functions for updates and events initiation.
*
*/

//#include "libAdjRaspi5Synth_1.h"



#include "utils\log.h"
#include <stdio.h>
#include <string>

#include "modSynth.h"
#include "./Settings/settings.h"

#include "Bluetooth\rspiBluetoothServicesQueuesVer.h"

#include "MIDI/midiStream.h"

#include "./Instrument/instrumentsManager.h"
#include "./Instrument/instrumentControlBoxHandler.h"
#include "./Instrument/instrumentFluidSynth.h"
#include "./Instrument/instrumentMidiPlayer.h"
#include "./Instrument/instrumentMidiMapper.h"
#include "./Instrument/instrumentAnalogSynth.h"

#include "./utils/xmlFiles.h"

#include "libAdjRaspi5Synth2.h"




/******************************************************************
 *********************** ModSynth Management API **************************
 ******************************************************************/

func_ptr_void_int_t callback_ptr_message_id = NULL;

/** The main modular synthesizer object instance */
ModSynth* mod_synthesizer;

Settings *settings_manager;


/**
*   @brief  Initializes the synthesizer engine.
*			Must be called first before any other call.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init()
{
	// Debuger
	FILELog::ReportingLevel() = FILELog::FromString("DEBUG4");
	FILE* pFile = fopen("AdjRaspi5Synth_1.log", "a");
	Output2FILE::Stream() = pFile;
	FILE_LOG(logINFO) << "...";

	mod_synthesizer = ModSynth::get_instance(); // new ModSynth();

	return 0;
}

int mod_synth_start_audio()
{
	return mod_synthesizer->start_audio();
}

int mod_synth_stop_audio()
{
	return mod_synthesizer->stop_audio();
}

int mod_synth_init_bt_services()
{
	/* Inilize */
	Raspi3BluetoothQ* bluetooth = Raspi3BluetoothQ::get_instance();
	bluetooth->start_bt_main_thread();
	mod_synthesizer->get_bt_alsa_out()->start_bt_alsa_out_thread();
	return 0;
}

int mod_synth_deinit_bt_services()
{
	Raspi3BluetoothQ::stop_bt_main_thread();
	mod_synthesizer->get_bt_alsa_out()->stop_bt_alsa_out_thread();
	return 0;
}

/******************************************************************
 *********************** Instrumants API **************************
 ******************************************************************/

en_instruments_types_t mod_synth_get_instrument_type(en_instruments_ids_t inst_id)
{
	return mod_synthesizer->instruments_manager->get_instrument_type(inst_id);
}

/******************************************************************
 *********************** Midi Services API **************************
 ******************************************************************/

int mod_synth_init_ext_midi_services(int port_num)
{
	
	mod_synthesizer->init_midi_ext_interface(_MIDI_EXT_INTERFACE_SERIAL_PORT_NUM);

	return 0;
}

int mod_synth_deinit_ext_midi_services()
{
	
	return 0;
}

int mod_synth_init_midi_services()
{
	// Allocate midi blocks data memory pool
	allocate_midi_stream_messages_memory_pool(_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE);
	allocate_raw_data_mssgs_memory_pool(_MAX_RAWDATA_MSSGS_POOL_SIZE);
	MidiStream::start_thread();


	return 0;
}

void mod_synth_on_exit()
{
	//	jackExit(); ???
	mod_synth_deinit_bt_services();
	mod_synth_deinit_midi_services();
	
	
	ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->deinitialize_fluid_synthesizer();

	ModSynth::get_instance()->adj_synth->audio_manager->stop_audio_service();
}


int mod_synth_deinit_midi_services()
{
	MidiStream::stop_thread();

	return 0;
}

int mod_synth_get_synthesizer_num_of_polyphonic_voices() 
{ 
	return AdjSynth::get_instance()->get_num_of_voices(); 
}

int mod_synth_get_synthesizer_num_of_programs() 
{ 
	return AdjSynth::get_instance()->get_num_of_programs(); 
}


int mod_synth_get_cpu_utilization() 
{ 
	return ModSynth::cpu_utilization; 
}

std::list<std::string> mod_synth_get_midi_input_client_name_strings()
{
	std::list<std::string> names_list;
	std::string name;

	int num_of_names = mod_synthesizer->alsa_midi_system_control->get_num_of_input_midi_clients();

	for (int i = 0; i < num_of_names; i++)
	{
		mod_synthesizer->alsa_midi_system_control->get_midi_input_client_name_string(i, &name, true); // replace virtual clients names
		names_list.push_back(name);
	}

	return names_list;
}

int mod_synth_connect_midi_clients(std::string instrument_name, int in_client, int in_port, bool connect, bool use_index)
{
	std::string client_name;
	int client_num;
	bool client_found = false;
	int num_of_output_clients = mod_synthesizer->alsa_midi_system_control->get_num_of_output_midi_clients();

	if (num_of_output_clients > 0)
	{
		if (use_index)
		{
			for (client_num = 0; client_num < num_of_output_clients; client_num++)
			{
				mod_synthesizer->alsa_midi_system_control->get_midi_output_client_name_string(client_num, &client_name);
				if (client_name.find(instrument_name) != std::string::npos)
				{
					client_found = true;
					break;
				}
			}
		}
		else
		{
			client_num = mod_synthesizer->alsa_midi_system_control->get_midi_client_connection_num(instrument_name);
			client_found = true;
		}

		if (client_found)
		{			
			if (connect)
			{
				return mod_synthesizer->alsa_midi_system_control->connect_midi_clients(in_client, in_port, client_num, 0, use_index);
			}
			else
			{
				return mod_synthesizer->alsa_midi_system_control->disconnect_midi_clients(in_client, in_port, client_num, 0, use_index);
			}
		}
	}

	return -1;
}

int mod_synth_refresh_alsa_clients_data()
{
	int res = mod_synthesizer->alsa_midi_system_control->refresh_alsa_clients_data();

	/* Tests */
	//int num = mod_synthesizer->alsa_midi_system_control->get_midi_client_connection_num(_INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY);
	//std::list<int> con_to;
	//std::list<std::string> names_list;

	// mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_numbers_list(num, &con_to);
	//mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_names_list(num, true, &names_list);
	/* Teset End*/

	return res;
}

int mod_synth_get_midi_client_connection_num(std::string instrument_name)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_client_connection_num(instrument_name);
}

int mod_synth_get_midi_input_client_id(string name, bool replace)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_input_client_id(name, replace);
}

int mod_synth_get_midi_client_connected_to_clients_names_list(int cln_num, bool replace,
															  std::list<std::string> *cln_lst)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_names_list(cln_num, replace,
																									  cln_lst);
}

int mod_synth_get_midi_client_connected_to_clients_numbers_list(int cln_num, std::list<int> *cln_lst)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_numbers_list(cln_num, cln_lst);
}

int mod_synth_set_instrument_active_midi_channels(std::string instrument_name, uint16_t active_midi_ch_mask)
{
	Instrument *instrument;

	instrument = mod_synthesizer->instruments_manager->get_instrument(instrument_name);
	if (instrument != NULL)
	{
		instrument->set_active_midi_channels(active_midi_ch_mask);

		return 0;
	}

	return -1;
}

uint16_t mod_synth_get_instrument_active_midi_channels(std::string instrument_name)
{
	Instrument *instrument;

	instrument = mod_synthesizer->instruments_manager->get_instrument(instrument_name);
	if (instrument != NULL)
	{
		return instrument->get_active_midi_channels();
	}

	return 0;
}

/******************************************************************
 *********************** JackAudio API **************************
 ******************************************************************/


int mod_synth_get_num_of_output_jack_clients()
{
	return mod_synthesizer->jack_connections->get_num_of_output_jack_clients();
}

std::string mod_synth_get_jack_output_client_name(int cln)
{
	return mod_synthesizer->jack_connections->get_jack_output_client_name(cln);
}

int mod_synth_get_num_of_jack_output_client_ports(int cln)
{
	return mod_synthesizer->jack_connections->get_num_of_jack_output_client_ports(cln);
}

std::string mod_synth_get_jack_output_client_port_name(int cln, int prt)
{
	return mod_synthesizer->jack_connections->get_jack_output_client_port_name(cln, prt);
}

int mod_synth_get_num_of_input_jack_clients()
{
	return mod_synthesizer->jack_connections->get_num_of_input_jack_clients();
}

std::string mod_synth_get_jack_input_client_name(int cln)
{
	return mod_synthesizer->jack_connections->get_jack_input_client_name(cln);
}

int mod_synth_get_num_of_jack_input_client_ports(int cln)
{
	return mod_synthesizer->jack_connections->get_num_of_jack_input_client_ports(cln);
}

std::string mod_synth_get_jack_input_client_port_name(int cln, int prt)
{
	return mod_synthesizer->jack_connections->get_jack_input_client_port_name(cln, prt);
}

int mod_synth_connect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name,
	bool connect)
{
	int res;
	s_jack_connection_t jack_connection;

	// All analog based instruments uses the AdjHeartSynth output
	std::string client_out_name = out_client_name;
	std:string client_port_out_name = out_client_port_name;
	
	if ((out_client_name == _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY) ||
		(out_client_name == _INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY) ||
		(out_client_name == _INSTRUMENT_NAME_KARPLUS_STRONG_STRING_SYNTH_STR_KEY) ||
		(out_client_name == _INSTRUMENT_NAME_MORPHED_SINUS_SYNTH_STR_KEY) ||
		(out_client_name == _INSTRUMENT_NAME_PADSYNTH_SYNTH_STR_KEY))
	{
		client_out_name = "AdjHeartSynth_out";
		
		if (out_client_port_name == "left")
		{
			client_port_out_name = "AdjHeartSynthL_out";
		}
		else if (out_client_port_name == "right")
		{
			client_port_out_name = "AdjHeartSynthR_out";
		}
	}
	
	
	if (connect)
	{
		res = mod_synthesizer->jack_connections->connect_jack_connection(
			in_client_name,																		  
			in_client_port_name, 
			client_out_name, 
			client_port_out_name);

		if (res == 0)
		{
			jack_connection.in_client_name = in_client_name;
			jack_connection.in_client_port_name = in_client_port_name;
			jack_connection.out_client_name = client_out_name;
			jack_connection.out_client_port_name = client_port_out_name;
			
			if (out_client_port_name == "left")
			{
				mod_synthesizer->get_analog_synth()->set_analog_synth_left_jack_output_connection(
					jack_connection);
			}
			else if (out_client_port_name == "right")
			{
				mod_synthesizer->get_analog_synth()->set_analog_synth_right_jack_output_connection(
					jack_connection);
			}
		}
	}
	else
	{
		res = mod_synthesizer->jack_connections->disconnect_jack_connection(
			in_client_name,
			in_client_port_name,
			client_out_name,
			client_port_out_name);

		if (res == 0)
		{
			jack_connection.in_client_name = "";
			jack_connection.in_client_port_name = "";
			jack_connection.out_client_name = "";
			jack_connection.out_client_port_name = "";

			if (out_client_port_name == "left")
			{
				mod_synthesizer->get_analog_synth()->set_analog_synth_left_jack_output_connection(
					jack_connection);
			}
			else if (out_client_port_name == "right")
			{
				mod_synthesizer->get_analog_synth()->set_analog_synth_right_jack_output_connection(
					jack_connection);
			}
		}
	}
	
	return res;
}

int mod_synth_refresh_jack_clients_data()
{
	return mod_synthesizer->jack_connections->refresh_jack_clients_data();
}

int mod_synth_get_jack_input_connections(std::string out_client_name, 
									std::string out_client_port_name,
									std::list<std::string> *outputs_list)
{
	return mod_synthesizer->jack_connections->get_connected_jack_input_connections(out_client_name,
											out_client_port_name,
											outputs_list);
}



/******************************************************************
 *********************** Modules Management API *******************
 ******************************************************************/

int mod_synth_add_module(string ins_name, Instrument *instrument)
{
	mod_synthesizer->instruments_manager->add_instrument(ins_name, instrument);

	return 0;
}

int mod_synth_remove_module(string ins_name)
{
	int client_number;
	list<int> connected_to_midi_clients_numbers_list;
	
	mod_synthesizer->instruments_manager->remove_instrument(ins_name);
	/* Disconnect midi in connections */
	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();
	/* Get module Midi Input connections */
	client_number = mod_synth_get_midi_client_connection_num(ins_name);
	connected_to_midi_clients_numbers_list.clear();
	mod_synth_get_midi_client_connected_to_clients_numbers_list(client_number,
		&connected_to_midi_clients_numbers_list);

	for (int client_num : connected_to_midi_clients_numbers_list)
	{
		mod_synth_connect_midi_clients(ins_name, client_num, 0, false, false); // disconnect, use client num
	}
	

	return 0;
}

int mod_synth_add_active_instrument(en_instruments_ids_t mod)
{
	mod_synthesizer->instruments_manager->add_active_instrument(mod);
	
	return 0;
}

int mod_synth_remove_active_instrument(en_instruments_ids_t mod)
{
	mod_synthesizer->instruments_manager->remove_active_instrument(mod);
	
	return 0;
}

int mod_synth_allocate_midi_channel_synth(int ch, en_instruments_ids_t synth)
{
	return mod_synthesizer->instruments_manager->allocate_midi_channel_synth(ch, synth);
}

en_instruments_ids_t mod_synth_get_allocated_midi_channel_synth(int ch)
{
	return mod_synthesizer->instruments_manager->get_allocated_midi_channel_synth(ch);
}

void mod_synth_register_callback_wrapper_close_instrument_pannel_id(func_ptr_void_en_instruments_ids_t_t ptr)
{
	mod_synthesizer->instruments_manager->register_callback_close_instrument_pannel_id(ptr);
}

void mod_synth_register_callback_wrapper_close_instrument_pannel_name(func_ptr_void_string_t ptr)
{
	mod_synthesizer->instruments_manager->register_callback_close_instrument_pannel_name(ptr);
}

void mod_synth_register_callback_wrapper_open_instrument_pannel_name(func_ptr_void_string_t ptr)
{
	mod_synthesizer->instruments_manager->register_callback_open_instrument_pannel_name(ptr);
}


/******************************************************************
 *********************** AdjSynth API **************************
 ******************************************************************/


func_ptr_void_int_t callback_ptr_set_osc1_unison_labales_callback = NULL;


void mod_synth_set_active_sketch(int ask)
{
	AdjSynth::get_instance()->set_active_sketch(ask);
}

int mod_synth_get_active_sketch()
{
	return AdjSynth::get_instance()->get_active_sketch();
}

int mod_synth_copy_sketch(int srcsk, int destsk)
{
	return AdjSynth::get_instance()->copy_sketch(srcsk, destsk);
}

void mod_synth_set_master_volume(int vol)
{
	AdjSynth::get_instance()->set_master_volume(vol);
	mod_synth_set_fluid_synth_volume(vol);
}

int mod_synth_get_master_volume()
{
	return AdjSynth::get_instance()->get_master_volume();
}

int mod_synth_amp_event_int(int ampid, int eventid, int val)
{
	return AdjSynth::get_instance()->amp_event(
		ampid, eventid, val, 
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_audio_event_int(int audid, int eventid, int val)
{
	return AdjSynth::get_instance()->audio_event_int(
		audid, eventid, val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_audio_event_bool(int audid, int eventid, bool val)
{
	return AdjSynth::get_instance()->audio_event_bool(
		audid, eventid, val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_distortion_event_int(int distid, int eventid, int val)
{
	return AdjSynth::get_instance()->distortion_event_int(
		distid, eventid, val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_distortion_event_bool(int distid, int eventid, bool val)
{
	return AdjSynth::get_instance()->distortion_event_bool(
		distid, eventid, val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_band_equalizer_event(int beqid, int eventid, int val)
{
	return AdjSynth::get_instance()->band_equalizer_event(
		beqid, eventid, val,
		AdjSynth::get_instance()->get_active_common_params());
}

int mod_synth_filter_event_int(int filtid, int eventid, int val)
{
	return AdjSynth::get_instance()->filter_event(
		filtid, eventid, val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_kbd_event_int(int kbid, int eventid, int val)
{
	return AdjSynth::get_instance()->kbd_event_int(
		kbid, eventid, val,
		AdjSynth::get_instance()->get_active_preset_params());
}

int mod_synth_karplus_event_int(int karlplusid, int eventid, int val)
{
	return AdjSynth::get_instance()->karplus_event_int(
		karlplusid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_karplus_event_bool(int karlplusid, int eventid, bool val)
{
	return AdjSynth::get_instance()->karplus_event_bool(
		karlplusid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_modulator_event_int(int modid, int eventid, int val)
{
	return AdjSynth::get_instance()->modulator_event_int(
		modid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_mso_event_int(int msoid, int eventid, int val)
{
	return AdjSynth::get_instance()->mso_event_int(
		msoid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_mso_event_bool(int msoid, int eventid, bool val)
{
	return AdjSynth::get_instance()->mso_event_bool(
		msoid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_noise_event_int(int noiseid, int eventid, int val)
{
	return AdjSynth::get_instance()->noise_event_int(
		noiseid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_noise_event_bool(int noiseid, int eventid, bool val)
{
	return AdjSynth::get_instance()->noise_event_bool(
		noiseid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_pad_event_int(int padid, int eventid, int val)
{
	return AdjSynth::get_instance()->pad_event_int(
		padid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_pad_event_bool(int padid, int eventid, bool val)
{
	return AdjSynth::get_instance()->pad_event_bool(
		padid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_reverb_event_int(int revid, int eventid, int val)
{
	return AdjSynth::get_instance()->reverb_event_int(
		revid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_common_params());
}

int mod_synth_reverb_event_bool(int revid, int eventid, bool val)
{
	return AdjSynth::get_instance()->reverb_event_bool(
		revid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_common_params());
}

int mod_synth_vco_event_int(int vcoid, int eventid, int val)
{
	return AdjSynth::get_instance()->vco_event_int(
		vcoid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_vco_event_bool(int vcoid, int eventid, bool val)
{
	return AdjSynth::get_instance()->vco_event_bool(
		vcoid,
		eventid,
		val,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

void mod_synth_enable_osc1() 
{
	AdjSynth::get_instance()->vco_event_bool(
		_OSC_1_EVENT,
		_OSC_ENABLE,
		_ENABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
	
	//mod_synth->synth_event_mapping(_OSC_1_EVENT, _OSC_ENABLE, 127);
}

void mod_synth_disable_osc1() 
{
	AdjSynth::get_instance()->vco_event_bool(
		_OSC_1_EVENT,
		_OSC_ENABLE,
		_DISABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch()); 
	
	//mod_synth->synth_event_mapping(_OSC_1_EVENT, _OSC_ENABLE, 0);
}

void mod_synth_enable_osc2() 
{
	AdjSynth::get_instance()->vco_event_bool(
		_OSC_2_EVENT,
		_OSC_ENABLE,
		_ENABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch()); 
	
	//mod_synth->synth_event_mapping(_OSC_2_EVENT, _OSC_ENABLE, 127);
}

void mod_synth_disable_osc2() 
{
	AdjSynth::get_instance()->vco_event_bool(
		_OSC_2_EVENT,
		_OSC_ENABLE,
		_DISABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());
	
	//mod_synth->synth_event_mapping(_OSC_1_EVENT, _OSC_ENABLE, 0);
}

void mod_synth_enable_noise() 
{
	AdjSynth::get_instance()->noise_event_bool(
		_NOISE_1_EVENT,
		_NOISE_ENABLE,
		_ENABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_NOISE_1_EVENT, _NOISE_ENABLE, 127);
}

void mod_synth_disable_noise() 
{
	AdjSynth::get_instance()->noise_event_bool(
		_NOISE_1_EVENT,
		_NOISE_ENABLE,
		_DISABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_NOISE_1_EVENT, _NOISE_ENABLE, 0);
}

void mod_synth_enable_karplus() 
{
	AdjSynth::get_instance()->karplus_event_bool(
		_KARPLUS_1_EVENT,
		_KARPLUS_STRONG_ENABLE,
		_ENABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_KARPLUS_1_EVENT, _KARPLUS_STRONG_ENABLE, 127);
}

void mod_synth_disable_karplus() 
{
	AdjSynth::get_instance()->karplus_event_bool(
		_KARPLUS_1_EVENT,
		_KARPLUS_STRONG_ENABLE,
		_DISABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_KARPLUS_1_EVENT, _KARPLUS_STRONG_ENABLE, 0);

}

void mod_synth_enable_mso() 
{
	AdjSynth::get_instance()->mso_event_bool(
		_MSO_1_EVENT,
		_MSO_ENABLE,
		_ENABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_MSO_1_EVENT, _MSO_ENABLE, 127);
}

void mod_synth_disable_mso() 
{
	AdjSynth::get_instance()->mso_event_bool(
		_MSO_1_EVENT,
		_MSO_ENABLE,
		_DISABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_MSO_1_EVENT, _MSO_ENABLE, 0);
}

void mod_synth_enable_pad_synth() 
{
	AdjSynth::get_instance()->pad_event_bool(
		_PAD_1_EVENT,
		_PAD_ENABLE,
		_ENABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_PAD_1_EVENT, _PAD_ENABLE, 127);
}

void mod_synth_disable_pad_synth() 
{
	AdjSynth::get_instance()->pad_event_bool(
		_PAD_1_EVENT,
		_PAD_ENABLE,
		_DISABLE,
		AdjSynth::get_instance()->get_active_preset_params(),
		AdjSynth::get_instance()->get_active_sketch());

	//mod_synth->synth_event_mapping(_PAD_1_EVENT, _PAD_ENABLE, 0);
}

void mod_synth_panic_action()
{
	mod_synthesizer->synth_panic_action();

	mod_synthesizer->adj_synth->init_poly();
}

void mod_synth_adj_synt_panic_action()
{
	AdjSynth::get_instance()->synth_panic_action();
}

void mod_synth_register_set_osc_1_unison_mode_callback(func_ptr_void_int_t ptr)
{
	callback_ptr_set_osc1_unison_labales_callback = ptr;
}

void mod_synth_callback_set_osc_1_unison_mode(int mode)
{
	if (callback_ptr_set_osc1_unison_labales_callback)
	{
		(*callback_ptr_set_osc1_unison_labales_callback)(mode);
	}
}

float *mod_synth_get_pad_base_harmony_profile() 
{ 
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_profile_data(); 
}

int mod_synth_get_pad_base_harmony_profile_size() 
{
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_profile_size();
}

float *mod_synth_get_pad_spectrum()  
{ 
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_spectrum_data();
}

int mod_synth_get_pad_spectrum_size() 
{ 
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_spectrum_size();
}



void mod_synth_activate_callback_update_ui(func_ptr_void_void_t ptr)
{
	//if (callback_ptr_update_ui != NULL)
	//{
	//	callback_ptr_update_ui();
	//}
}

void mod_synth_register_callback_update_ui(func_ptr_void_void_t ptr)
{
	//callback_ptr_update_ui = ptr;
}





/******************************************************************
 *********************** FLUID SYNTH API **************************
 ******************************************************************/

void mod_synth_set_fluid_synth_volume(int vol)
{
	//mod_synthesizer->get_fluid_synth()->set_volume(vol);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_VOLUME,
		vol,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_reverb_room_size(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_reverb_room_size(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_REVERB_ROOM_SIZE,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_reverb_damp(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_reverb_damp(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_REVERB_DAMP,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_reverb_width(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_reverb_width(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_REVERB_WIDTH,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_reverb_level(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_reverb_level(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_REVERB_LEVEL,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_chorus_number(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_chorus_number(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_CHORUS_NUMBER,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_chorus_level(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_chorus_level(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_CHORUS_LEVEL,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_chorus_speed(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_chorus_speed(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_CHORUS_SPEED,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_chorus_depth(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_chorus_depth(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_CHORUS_DEPTH,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_chorus_waveform(int val)
{
	//mod_synthesizer->get_fluid_synth()->set_chorus_waveform(val);
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_CHORUS_WAVEFORM,
		val,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_enable_reverb()
{
	//mod_synthesizer->get_fluid_synth()->enable_reverb();
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_REVERB_ENABLE,
		true,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_disable_reverb()
{
	//mod_synthesizer->get_fluid_synth()->disable_reverb();
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_REVERB_ENABLE,
		false,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_enable_chorus()
{
	//mod_synthesizer->get_fluid_synth()->enable_chorus();
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_CHORUS_ENABLE,
		true,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

void mod_synth_set_fluid_synth_disable_chorus()
{
	//mod_synthesizer->get_fluid_synth()->disable_chorus();
	mod_synthesizer->get_fluid_synth()->events_handler(
		_FLUID_SYNTH_1_EVENT,
		_FLUID_SYNTH_CHORUS_ENABLE,
		false,
		mod_synthesizer->get_fluid_synth()->instrument_settings->get_active_settings_parameters());
}

int mod_synth_get_active_fluid_synth_volume()
{
	fluid_res_t res = 0;

	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.volume", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	// 0-1.0 -> 0-200
	return (int)(param.value * 200.0);
}

int mod_synth_get_active_fluid_synth_reverb_room_size()
{
	fluid_res_t res = 0;

	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.reverb.room-size", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100.0);
}

int mod_synth_get_active_fluid_synth_reverb_damp()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.reverb.damp", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_reverb_width()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.reverb.width", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_reverb_level()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.reverb.level", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_chorus_number()
{
	fluid_res_t res = 0;
	_settings_int_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_int_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.chorus.nr", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-99
	return param.value;
}

int mod_synth_get_active_fluid_synth_chorus_level()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.chorus.level", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-10.0 -> 0-100
	return (int)(param.value * 10);
}

int mod_synth_get_active_fluid_synth_chorus_speed()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.chorus.speed", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0.29-5.0 -> 29-500
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_chorus_depth()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.chorus.depth", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-21.0 -> 0-210
	return (int)(param.value * 10);
}

int mod_synth_get_active_fluid_synth_chorus_waveform()
{
	fluid_res_t res = 0;
	_settings_int_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_int_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.chorus.waveform", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0, 1
	return param.value;
}

bool mod_synth_get_active_fluid_synth_reverb_activation_state()
{
	fluid_res_t res = 0;
	_settings_bool_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_bool_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.reverb.active", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return false;
	}

	return param.value;
}

bool mod_synth_get_active_fluid_synth_chorus_activation_state()
{
	fluid_res_t res = 0;
	_settings_bool_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_bool_param(
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params, 
		"fluid.synth.chorus.active", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	return param.value;
}

void mod_synth_set_fluid_synth_soundfont(string path)
{
	mod_synthesizer->get_fluid_synth()->set_soundfont(path);
}

void mod_synth_load_and_set_fluid_synth_soundfont(string path)
{
	mod_synthesizer->get_fluid_synth()->load_and_set_fluid_synth_soundfont(path);
}

int mod_synth_set_fluid_synth_select_program(int chan,
											 unsigned int sfid,
											 unsigned int bank,
											 unsigned int program)
{
	mod_synthesizer->get_fluid_synth()->select_program(chan, sfid, bank, program);

	return 0;
}

int mod_synth_set_fluid_synth_program_select(int chan,
											 unsigned int sfid,
											 unsigned int bank,
											 unsigned int program)
{
	return mod_synthesizer->get_fluid_synth()->set_fluid_synth_program_select(chan, sfid, bank, program);
}

int mod_synth_collect_fluid_synth_preset_parms(_settings_params_t *params)
{
	return mod_synthesizer->get_fluid_synth()->collect_fluid_synth_preset_parms(params);
}

int mod_synth_save_fluid_synth_preset_file(string path)
{
	return mod_synthesizer->get_fluid_synth()->save_fluid_synth_preset_file(path);
}

int mod_synth_open_fluid_synth_preset_file(string path, int preset_num){

	int res;

	mod_synthesizer->get_fluid_synth()->open_fluid_synth_preset_file(
		path,
		mod_synthesizer->get_fluid_synth()->active_preset_settings_params,
		&mod_synthesizer->get_fluid_synth()->presets_summary_str[preset_num]);

	mod_synthesizer->get_fluid_synth()->generate_fluid_synth_prest_summary(
		&mod_synthesizer->get_fluid_synth()->channels_presets[preset_num][0],
		&mod_synthesizer->get_fluid_synth()->presets_summary_str[preset_num]);
}

std::string mod_synth_get_fluid_synth_soundfont_id_name(int sfid)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_soundfont_id_name(sfid);
}

int mod_synth_get_fluid_synth_loaded_soundfonts_presets(std::vector<_soundfont_presets_data_t> *presets)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_loaded_sound_fonts_presets(presets);
}

int mod_synth_get_fluid_synth_channel_bank(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_channel_bank(chan);
}

int mod_synth_get_fluid_synth_channel_program(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_program(chan);
}

std::string mod_synth_get_fluid_synth_channel_preset_name(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_preset_name(chan);
}

int mod_synth_get_fluid_synth_channel_sfont_id(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_sfont_id(chan);
}

std::string mod_synth_get_fluid_synth_channel_preset_soundfont_name(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_preset_soundfont_name(chan);
}

/* Settings Files Handling */

int mod_synth_save_fluid_synth_settings_file(string path)
{
	return mod_synthesizer->get_fluid_synth()->save_fluid_synth_settings_file(path);
}

int mod_synth_open_fluid_synth_settings_file(string path)
{
	return mod_synthesizer->get_fluid_synth()->open_fluid_synth_settings_file(path);
}

int mod_synth_save_adj_synth_patch_file(string path)
{
	return mod_synthesizer->save_adj_synth_patch_file(path);
}

int mod_synth_open_adj_synth_patch_file(std::string path, int channel)
{
	return mod_synthesizer->open_adj_synth_patch_file(path, channel);
}


std::string mod_synth_get_program_patch_name(int prog)
{
	
}

int mod_synth_save_adj_synth_settings_file(string path)
{
	
}

int mod_synth_open_adj_synth_settings_file(std::string path)
{
	
}

int mod_synth_save_mod_synth_general_settings_file(string path)
{
	
}

int mod_synth_open_mod_synth_general_settings_file(std::string path)
{
	
}

int mod_synth_save_mod_synth_preset_file(string path)
{
	
}

int mod_synth_open_mod_synth_preset_file(std::string path, int presetNum)
{
	
}

void mod_synth_set_mod_synth_active_preset_num(int presetNum)
{
	
}

int mod_synth_get_mod_synth_active_preset_num()
{
	
}

string *mod_synth_get_preset_summary_string(int presetNum)
{
	
}

/* Utils */
float mod_synth_log_scale_100_float(float min, float max, float base, int val)
{
	return Utils::calc_log_scale_100_float(min, max, base, val);
}

int mod_synth_log_scale_100_int(int min, int max, int base, int val)
{
	return Utils::calc_log_scale_100_float(min, max, base, val);
}



/* UI Callback Functions Registration */

void mod_synth_fluid_synth_register_ui_update_callback(func_ptr_void_void_t ptr)
{
	mod_synthesizer->get_fluid_synth()->register_ui_update_callback(ptr);
}

void mod_synth_fluid_synth_activate_ui_update_callback()
{
	mod_synthesizer->get_fluid_synth()->activate_ui_update_callback();
}

void mod_synth_fluid_synth_register_callback_update_preset_text(func_ptr_void_string_t ptr)
{
	mod_synthesizer->get_fluid_synth()->register_callback_update_preset_text(ptr);
}

void mod_synth_fluid_synth_activate_ui_update_preset_text_callback(std::string text)
{
	mod_synthesizer->get_fluid_synth()->activate_ui_update_preset_text_callback(text);
}

void mod_synth_fluid_synth_set_active_preset(int preset_num)
{
	if ((preset_num >= 0) && (preset_num < _NUM_OF_FLUID_PRESETS))
	{
		mod_synthesizer->get_fluid_synth()->set_active_preset(preset_num);

		mod_synth_fluid_synth_activate_ui_update_callback();
	}
}

int mod_synth_fluid_synth_get_active_preset()
{
	return mod_synthesizer->get_fluid_synth()->active_preset;
}

/******************************************************************
 *********************** MIDI Player API **************************
 ******************************************************************/

int mod_synth_open_midi_file(std::string path)
{
	return mod_synthesizer->get_midi_player()->init_new_file(path);
}

void mod_synth_midi_player_play()
{
	mod_synthesizer->get_midi_player()->play();
}

void mod_synth_midi_player_pause()
{
	mod_synthesizer->get_midi_player()->pause();
}

void mod_synth_midi_player_stop()
{
	mod_synthesizer->get_midi_player()->stop();
}

void mod_synth_midi_player_backward()
{
	mod_synthesizer->midi_player->backward();
}

void mod_synth_midi_player_forward()
{
	mod_synthesizer->midi_player->forward();
}

void mod_synth_register_midi_player_potision_update_callback(func_ptr_void_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_potision_update_callback(ptr);
}

void mod_synth_register_midi_player_total_song_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_total_playing_time_update_callback(ptr);
}

void mod_synth_register_midi_player_song_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_playing_time_update_callback(ptr);
}

void mod_synth_register_midi_player_song_remaining_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_song_remaining_time_update_callback(ptr);
}


/******************************************************************
 *********************** Control Box API **************************
 ******************************************************************/


/**
*   @brief  Register a callback function that initiates a full GUI update based on control box event.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
void mod_synth_register_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr)
{
	mod_synthesizer->control_box_events_handler->register_callback_control_box_event_update_ui(ptr);
}

void mod_synth_unregister_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr)
{
	mod_synthesizer->control_box_events_handler->unregister_callback_control_box_event_update_ui(ptr);
}



/******************************************************************
 *********************** MIDI Mixer API **************************
 ******************************************************************/

void mod_synth_midi_mixer_set_channel_volume(int chan, int vol)
{
	if ((chan < 0) || (chan > 16) || (vol < 0) || (vol > 100))
	{
		return;
	}
	
	// Update the MIDI mixer settup parrameters
	AdjSynth::get_instance()->midi_mixer_event(_MIDI_MIXER_1_EVENT,
		_MIXER_CHAN_1_LEVEL + chan,
		vol,
												ModSynth::get_instance()->adj_synth->get_active_settings_params());

	mod_synthesizer->midi_mapper->set_midi_channel_volume(chan, vol);
}

void mod_synth_midi_mixer_set_channel_pan(int chan, int pan)
{
	if ((chan < 0) || (chan > 16) || (pan < 0) || (pan > 100))
	{
		return;
	}
	
	// Update the MIDI mixer settup parrameters
	AdjSynth::get_instance()->midi_mixer_event(_MIDI_MIXER_1_EVENT,
		_MIXER_CHAN_1_PAN + chan,
		pan,
		ModSynth::get_instance()->adj_synth->get_active_settings_params());

	mod_synthesizer->midi_mapper->set_midi_channel_pan(chan, pan);
}

void mod_synth_midi_mixer_set_channel_pan_mod_level(int chan, int lvl)
{
	en_instruments_ids_t chan_allocated_instrument = mod_synth_get_allocated_midi_channel_synth(chan);
	
	// Update the MIDI mixer settup parrameters
	AdjSynth::get_instance()->midi_mixer_event(_MIDI_MIXER_1_EVENT,
		_MIXER_CHAN_1_PAN_MOD_LFO_LEVEL + chan,
		lvl,
		ModSynth::get_instance()->adj_synth->get_active_settings_params());
	
	switch (chan_allocated_instrument)
	{
		case en_instruments_ids_t::fluid_synth:
			// Not supported?
			break;
		case en_instruments_ids_t::adj_analog_synth:
		case en_instruments_ids_t::adj_hammond_organ:
			mod_synthesizer->midi_mapper->set_midi_channel_pan_mod_lfo_level(chan, lvl);
			break;
		
		default:
			break;
	}
}

void mod_synth_midi_mixer_set_channel_pan_mod_lfo(int chan, int lfo)
{
	en_instruments_ids_t chan_allocated_instrument = mod_synth_get_allocated_midi_channel_synth(chan);
	
	// Update the MIDI mixer settup parrameters
	AdjSynth::get_instance()->midi_mixer_event(_MIDI_MIXER_1_EVENT,
		_MIXER_CHAN_1_PAN_MOD_LFO + chan,
		lfo,
		ModSynth::get_instance()->adj_synth->get_active_settings_params());
	
	switch (chan_allocated_instrument)
	{
	case en_instruments_ids_t::fluid_synth:
		// Not supported?
		break;
	case en_instruments_ids_t::adj_analog_synth:
	case en_instruments_ids_t::adj_hammond_organ:
		mod_synthesizer->midi_mapper->set_midi_channel_pan_mod_lfo(chan, lfo);
		
		break;
		
	default:
		break;
	}
}

void mod_synth_midi_mixer_set_channel_send_level(int chan, int snd)
{
	en_instruments_ids_t chan_allocated_instrument = mod_synth_get_allocated_midi_channel_synth(chan);
	
	// Update the MIDI mixer settup parrameters
	AdjSynth::get_instance()->midi_mixer_event(_MIDI_MIXER_1_EVENT,
		_MIXER_CHAN_1_SEND + chan,
		snd,
		ModSynth::get_instance()->adj_synth->get_active_settings_params());
	
	switch (chan_allocated_instrument)
	{
	case en_instruments_ids_t::fluid_synth:
		// Not supported?
		break;
	case en_instruments_ids_t::adj_analog_synth:
	case en_instruments_ids_t::adj_hammond_organ:
		mod_synthesizer->midi_mapper->set_midi_channel_send(chan, snd);
		break;
		
	default:
		break;
	}
}

void mod_synth_register_midi_mixer_channel_volume_update_callback(func_ptr_void_int_int_t ptr)
{
	
}

void mod_synth_midi_mixer_set_channel_static_volume(int chan, bool state)
{
	if (state)
	{
		mod_synthesizer->midi_mapper->enable_midi_channel_block_volume_commands(chan);
	}
	else
	{
		mod_synthesizer->midi_mapper->disable_midi_channel_block_volume_commands(chan);
	}
}

void mod_synth_register_midi_mixer_channel_pan_update_callback(func_ptr_void_int_int_t ptr)
{
	
}

void mod_synth_register_midi_mixer_channel_static_volume_update_callback(func_ptr_void_int_bool_t ptr)
{
	
}

/******************************************************************
 *********************** MIDI Mapper API **************************
 ******************************************************************/

void mod_synth_register_midi_channel_volume_control_command_trapped_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_mapper->register_midi_channel_volume_control_command_trap_callback(ptr);
}

void mod_synth_register_midi_channel_change_program_command_trapped_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_mapper->register_midi_channel_change_program_command_trap_callback(ptr);
}

void mod_synth_register_midi_channel_active_indication_update_callback(func_ptr_void_int_bool_t ptr)
{
	mod_synthesizer->midi_mapper->register_midi_channel_active_indication_update_callback(ptr);
}

/******************************************************************
 *********************** Patches Management API **************************
 ******************************************************************/

void mod_synth_register_callback_get_active_instruments_names_list(func_ptr_vector_std_string_void_t ptr)
{
	return mod_synthesizer->patches_handler->register_callback_get_active_instruments_names_list(ptr);
}

int mod_synth_save_patch_file(std::string file_path)
{
	return mod_synthesizer->patches_handler->save_patch_file(file_path);
}

int mod_synth_load_patch_file(std::string file_path)
{
	int res = 0;

	XML_files *xml_files = new XML_files();

	mod_synthesizer->patches_handler->disconnect_current_oppened_instruments_midi_in_connections();
	mod_synthesizer->patches_handler->close_current_oppened_instruments();
	res = mod_synthesizer->patches_handler->load_patch_file(file_path);

	// TODO: selcte file name base on patch name or instrument names
	file_path = xml_files->get_xml_file_path(file_path) + "Adj-Analog-Synth-settings.xml";

	/* Moved to patches.cpp*/
	//res |= mod_synthesizer->get_analog_synth()->instrument_settings->read_settings_file(
	//	mod_synthesizer->get_analog_synth()->active_preset_settings_params,
	//	file_path,
	//	_ADJ_SYNTH_PRESET_PARAMS, _SKETCH_PROGRAM_1); // TODO: fix sketch number

	return res;
}


void callback_message_id(int mssgid)
{
	if (callback_ptr_message_id)
	{	
		(*callback_ptr_message_id)(mssgid);
	}
}



