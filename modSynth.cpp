/**
 *	@file		modSynth.h
 *	@author		Nahum Budin
 *	@date		24-Sep-2025
 *	@version	1.1
 *					1. Code refactoring rename patches to preset parameters.
 *					2. Adding support in both old and new MIDI program objects.
 *					3. Definning a global settings handling mutex
 *					4. Adding I2C interface support
 *
 *	@brief		This is the main modular synthesizer libraray object.
 *
 *	History:\n
 *
 *	version 1.0		11-May-2024: First version
 *
 *	Based on libAdjHeartModSynth_2 library
 *
 */

#include <thread>

#include "modSynth.h"
#include "commonDefs.h"
#include "modSynthPatch.h"

#include "./CPU/CPUSnapshot.h"

#include "./MIDI/midiAlsaQclient.h"

#include "./ALSA/controlBoxExtMidiInClientAlsaOutput.h"


#include "./ALSA/alsaMidiSequencerClient.h"
#include "./ALSA/alsaMidiSequencerEventsHandler.h"

#include "./Instrument/instrumentsManager.h"
#include "./Instrument/instrumentFluidSynth.h"
#include "./Instrument/instrumentHammondOrgan.h"
#include "./Instrument/instrumentAnalogSynth.h"

#include "./Instrument/instrumentMidiPlayer.h"
#include "./Instrument/instrumentMidiMapper.h"
#include "./Instrument/instrumentMidiMixer.h"
#include "./Instrument/instrumentAnalogReverbration.h"

#include "./Instrument/instrumentControlBoxHandler.h"

#include "./utils/xmlFiles.h"
#include "./Settings/settings.h"

// Mutex to controll audio memory blocks allocation
//pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

int ModSynth::cpu_utilization = 0;
volatile bool cheack_cpu_utilization_thread_is_running;


/*************************** ALSA MIDI *******************************************/

/* ALSA MIDI sequencer client - Receives input midi data stream from ALSA midi source.
 *								Connect to it using, for example, a JACK Audio Connection Kit.
 *								Received data is pushed into the alsa_seq_client_rx_queue */
//AlsaMidiSequencerInputClient* alsa_midi_sequencer_input_client = new AlsaMidiSequencerInputClient();

// Pulls out the events from alsa_seq_client_rx_queue and executes commands
//AlsaMidiSeqencerEventsHandler alsa_midi_seqencer_events_handler_1(_MIDI_STAGE_2);

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 0)
// MIDI ALSA input client - waits for data in the alsa_rx_queue 

//MidiAlsaQclientIn alsa_midi_in_1(&AlsaMidi::get_instance()->alsa_rx_queue[0], _MIDI_STAGE_0);

//MidiParser midi_parser_alsa_1(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_1(_MIDI_STAGE_2);

// ALSA MIDI connection ALSA-client -> parser
//MidiConnection midi_connection_alsa_1(&alsa_midi_in_1, 0, &midi_parser_alsa_1, 0);
// ALSA MIDI connection parser -> handler 
//MidiConnection midi_connection_alsa_handler_1(&midi_parser_alsa_1, 0, &midi_handler_alsa_1, 0);

#endif

// Callback that is initiated by the AudioManager audio - update thread.
void callback_audio_update_cycle_start_tasks_wrapper(int param)
{
	ModSynth::get_instance()->update_tasks(param);
}


// modSynth instance
ModSynth* ModSynth::mod_synth = NULL;

string ModSynth::mod_synth_current_working_directory = "";
string ModSynth::mod_synth_current_patches_directory = "";
string ModSynth::mod_synth_current_settings_directory = "";
string ModSynth::mod_synth_current_soundfonts_directory = "";
string ModSynth::mod_synth_current_midi_files_directory = "";
string ModSynth::mod_synth_general_settings_file_path = "";

std::mutex ModSynth::settings_handler_mutex;

ModSynth::ModSynth()
{
	mod_synth = this;
	int i, res, last_input_client_num;

	// An object that uses system "aconnect" commands to scan and control ALSA midi connections.
	alsa_midi_system_control = AlsaMidiSysControl::get_instance();
	
	// An object that uses system "jack_lsp" commands to scan and control JACK connections.
	jack_connections = JackConnections::get_instance();

	// The Bluetooth ALSA output Client.
	// Handles the BT RFCOMM(SPP) midi input streams and
	// uses ALSA output(source) to enable connecting them to any alsa input.
	bt_alsa_out = AlsaBtClientOutput::get_instance();

	// Scan for current ALSA MIDI In client - the last is the BT client above (last created)
	alsa_midi_system_control->refresh_alsa_clients_data();
	last_input_client_num = alsa_midi_system_control->get_num_of_input_midi_clients() - 1;
	alsa_midi_system_control->get_midi_input_client_name_string(last_input_client_num,
																&alsa_midi_system_control->bt_client_in_name);

	// Serial Port based Control Box Support (See I2C based Control Box support below)
	
	// The Control Box MIDI ALSA output Client 
	// A Serial Port input based alsa output client (serial->alsa)
	control_box_alsa_out = ControlBoxClientAlsaOutput::get_instance();
	// Scan for current ALSA MIDI In Clients - the last is the Control Box In client (last created)
	alsa_midi_system_control->refresh_alsa_clients_data();
	last_input_client_num = alsa_midi_system_control->get_num_of_input_midi_clients() - 1;
	alsa_midi_system_control->get_midi_input_client_name_string(last_input_client_num,
																&alsa_midi_system_control->control_box_client_in_name);

	// The Control Box external MIDI in port ALSA output Client
	// This is a Serial Port input based alsa output client (serial->alsa)
	control_box_ext_midi_in_alsa_out = ControlBoxExtMidiInClientAlsaOutput::get_instance();
	
	// Scan for current ALSA MIDI In Clients - the last should be the Control Box Ext MIDI In client
	alsa_midi_system_control->refresh_alsa_clients_data();
	last_input_client_num = alsa_midi_system_control->get_num_of_input_midi_clients() - 1;
	alsa_midi_system_control->get_midi_input_client_name_string(last_input_client_num,
																&alsa_midi_system_control->control_box_xt_midi_in_client_name);

	/* The external MIDI interface
	 * Handeles the external MIDI interfaces (including the device knobs, sliders, etc.)
	 * Note - this serial port interfaces to the device controller board that hosts the
	 * 32500 MIDI UART.  */
	midi_ext_interface = MidiExtInterface::get_midi_ext_interface_instance();
	
	
	/* A singleton of the AdjSynth. */ // TODO: can it be above
	adj_synth = AdjSynth::get_instance();
	
	set_audio_driver_type(_DEFAULT_AUDIO_DRIVER);	// (Jack)
	set_audio_block_size(_DEFAULT_BLOCK_SIZE);		// (512)
	set_sample_rate(_DEFAULT_SAMPLE_RATE);			// (44100)

	// The Instruments Manager that manages active instruments
	instruments_manager = InstrumentsManager::get_instance();
	
	// TODO: create instances only when oppened ?.
	
	// Midi Mixer Instrument
	//midi_mixer_instrument = new InstrumentMidiMixer();

	// A FLuidSynth instument
	fluid_synth = new InstrumentFluidSynth();
	// Add it to list of instruments
	instruments_manager->add_instrument(_INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY, 
										fluid_synth);

	// Init the Adj synthesizers
	init();
	
	
	// An Hammond Organ Instrument (TODO: define the Analog Synth based innstruments concept)
	hammond_organ = new InstrumentHammondOrgan(adj_synth,
											adj_synth->synth_program[_HAMMOND_ORGAN_PROGRAM_20]->active_preset_params);
	instruments_manager->add_instrument(_INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY, 
										hammond_organ);
	hammond_organ->use_external_settings(adj_synth->synth_program[_HAMMOND_ORGAN_PROGRAM_20]->active_preset_params);
	//adj_synth->set_program_preset_params_ptr(_HAMMOND_ORGAN_PROGRAM_20, hammond_organ->active_settings_params);
	
	// An Analog Synth Instrument (implements all the non-sampled based synthesis methods, 
	// including additive, subtractive, Karplus-Strong, PADsynth, etc.)
	analog_synth = new InstrumentAnalogSynth(this->adj_synth);
	instruments_manager->add_instrument(_INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY,
										analog_synth);
	//adj_synth->set_program_preset_params_ptr(_PROGRAM_16, adj_synth->synth_program[_SKETCH_PROGRAM_1]->active_preset_params);
	//adj_synth->set_program_preset_params_ptr(_PROGRAM_17, adj_synth->synth_program[_SKETCH_PROGRAM_2]->active_preset_params);
	//adj_synth->set_program_preset_params_ptr(_PROGRAM_18, adj_synth->synth_program[_SKETCH_PROGRAM_3]->active_preset_params);
	
	// Midi Mixer Instrument
	midi_mixer_instrument = new InstrumentMidiMixer();
	instruments_manager->add_instrument(_INSTRUMENT_NAME_MIDI_MIXER_STR_KEY, 
										midi_mixer_instrument);
	
	// Analog Reverberation Instrument
	analog_reverberation_instrument = new InstrumentAnalogReverbration();
	instruments_manager->add_instrument(_INSTRUMENT_NAME_REVERB_STR_KEY, 
		analog_reverberation_instrument);

	// A MIDI Mapper Instrument. Maps MIDI cahnnel events to assigned synthesizers (FluidSynth, Analog Synth, Hammond Organ, etc.).
	midi_mapper = new InstrumentMidiMapper(alsa_midi_system_control,
										   &alsa_midi_system_control->midi_mapper_client_in_name);
	instruments_manager->add_instrument(_INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY,
										midi_mapper);

	// A MIDI Player Instrument.
	midi_player = new InstrumentMidiPlayer(alsa_midi_system_control, 
										   &alsa_midi_system_control->midi_player_client_in_name);
	instruments_manager->add_instrument(_INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY,
										midi_player);
	
	// Make sure no notes are left hanging when the MIDI Player is stopped or a new song is loaded.
	midi_player->register_clear_all_playing_notes_callback(mod_synth_panic_action); // TODO: Done elsewhere ?.
	// Traps MIDI change channel volume events to update the MIDI Mixer channels volume.
	midi_player->register_midi_change_channel_volume_callback(NULL); // TODO:
	// Traps MIDI change channel program events.
	midi_player->register_midi_change_program_callback(NULL); // TODO:
	
	
	midi_player->register_midi_player_song_remaining_time_update_callback(
		InstrumentMidiPlayer::midi_player_total_playing_time_update_callback_ptr);
	midi_player->register_midi_player_song_remaining_time_update_callback(
		InstrumentMidiPlayer::midi_player_playing_time_update_callback_ptr);
	midi_player->register_midi_player_playing_time_update_callback(
		InstrumentMidiPlayer::midi_player_file_remaining_time_update_callback_ptr);
	midi_player->register_midi_player_potision_update_callback(NULL);
	
	// midi_player->register_send_midi_events_vector_callback(NULL);  in player
	
	// I2C interface - control device	
	
	// Create an I2C interface to the control device/box (the one with the buttons, knobs, sliders, etc.)
	i2c_interface_main_control = new I2Cinterface(
		_I2C_BUS_NUMBER, _I2C_SLAVE_MAIN_CONTROL_DEVICE_ADDRESS, _I2C_INTERFACE_NUMBER_MAIN_CONTROL);
	
	i2c_interface_main_control->set_blocks_registers_addresses(
		_I2C_SLAVE_MAIN_CONTROL_DEVICE_DATA_REG_ADDRESS,		// Used to activate commands
		_I2C_SLAVE_MAIN_CONTROL_DEVICE_DATA_LEN_REG_ADDRESS,	// Used for the command parameters length
		_I2C_SLAVE_EXT_MIDI_INT_RX_DATA_REG_ADDRESS,			// Used for receving data from the control box external MIDI interface connectors
		_I2C_SLAVE_EXT_MIDI_INT_RX_DATA_LEN_REG_ADDRESS); // Used for the length of the received data from the MIDI interface connectors.
	// Start the I2C polling thread to read the control box state and events.
	i2c_interface_main_control->start_i2c_polling_thread();

	// I2C interface - secondary control board (OK button, 4 Arrows and 2 LEDs) Bitmaped oriented, not command oriented like the main control board. 
	// This is because of the need to update the LEDs state together with the buttons state, and to update the 4 arrows state together 
	// (they are used together for navigation in the menu system).
	i2c_interface_control_board = new I2Cinterface(
		_I2C_BUS_NUMBER, _I2C_SLAVE_CONTROL_BOARD_ADDRESS, _I2C_INTERFACE_NUMBER_CONTROL_BOARD);
	
	i2c_interface_control_board->set_blocks_registers_addresses(
		_I2C_SLAVE_CONTROL_BOARD_DATA_REG_ADDRESS,				
		_I2C_SLAVE_CONTROL_BOARD_DATA_LEN_REG_ADDRESS,
		0, 0);
	// Start the I2C polling thread to read the control board state and events.
	i2c_interface_control_board->start_i2c_polling_thread();

	// The Control Box events handler Instrument.
	control_box_events_handler = new InstrumentControlBoxEventsHandler();
	instruments_manager->add_instrument(_INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY, 
										control_box_events_handler);

	// Refresh the list of all ALSA clients
	alsa_midi_system_control->refresh_alsa_clients_data();
	// Refresh the list of all JackAudio clients
	jack_connections->refresh_jack_clients_data();

	// Holds all the input clients names
	std::list<std::string> in_clients;
	// The number of input clients.
	int num_of_alsa_clients =
		fluid_synth->alsa_connections->get_alsa_midi_in_clients_names_list(&in_clients);
	
	//fluid_synth->instrument_connections_control->alsa_midi_system_contrl->connect_midi_clients(5, 0, 2, 0);

	// The number of JackAudio clients.
	int num_of_jack_clients = 
		fluid_synth->jack_connections->get_num_of_output_jack_clients();

	// Get the index of the control box input client
	i = alsa_midi_system_control->get_midi_input_client_id(_ALSA_NAME_CLIENT_CONTROL_BOX_EXT_MIDI_STR, true);
	
	// Connect the control box handler to the midi input client
	// Remove the xxx: prefix
	std::string removed_name;
	int loc = alsa_midi_system_control->control_box_client_in_name.find_first_of(":");
	if (loc != std::string::npos)
	{
		removed_name = alsa_midi_system_control->control_box_client_in_name.substr(loc + 1);
	}

	// The id of the control box input handler.
	int in_dev = alsa_midi_system_control->get_midi_input_client_id(removed_name);
	
	// The id of control box output handler. 
	int out_dev = alsa_midi_system_control->get_midi_output_client_id(
		_INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY);
	
	// Connect the the control box input (to the system) to the control box output (from the control box)
	alsa_midi_system_control->connect_midi_clients(in_dev, 0, out_dev, 0);

	// The Patchs Handler.
	patches_handler = PatchsHandler::get_patchs_handler_instance();
	
	// Clear all MIDI channels synthesizer assignment to default (none assigned).
	for (i = _PROGRAM_0; i < _PROGRAM_15; i++)
	{
		// Init all programs to synthersizer not assigned state
		midi_channel_synth[i] = _MIDI_CHAN_ASSIGNED_SYNTH_NONE;
	}
	
	// Init skethc-programs state to assigned to AdjSynth (this will never change)
	midi_channel_synth[_SKETCH_PROGRAM_1] = _MIDI_CHAN_ASSIGNED_SYNTH_ADJ; // Sketches are only assigned to the AdjSynth, and this will never change.
	midi_channel_synth[_SKETCH_PROGRAM_2] = _MIDI_CHAN_ASSIGNED_SYNTH_ADJ;
	midi_channel_synth[_SKETCH_PROGRAM_3] = _MIDI_CHAN_ASSIGNED_SYNTH_ADJ;
	
	// TODO: Get the default settings directories from a file - currentlly hard coded below
	
	// ModSynth current working directory - used for presets and settings files if not specified otherwise.
	char buffer[PATH_MAX];
	if (getcwd(buffer, sizeof(buffer)) != nullptr)
	{
		mod_synth_current_working_directory = std::string(buffer);
		mod_synth_current_patches_directory = mod_synth_current_working_directory + "/Patches";
		mod_synth_current_settings_directory = mod_synth_current_working_directory + "/Settings";
		mod_synth_current_soundfonts_directory = mod_synth_current_working_directory + "/SoundFonts";
		mod_synth_current_midi_files_directory = mod_synth_current_working_directory + "/MidiFiles";
		
		mod_synth_general_settings_file_path = mod_synth_current_settings_directory + "/ModSynth/Default_Settings";
	}
	else
	{
		// Handle error
		std::cerr << "Error getting current working directory" << std::endl;
		mod_synth_current_working_directory = "";
	}
	
	/* ModSynth default settings directory */
	//mod_synth_general_settings_file_path_name = "/home/pi/AdjRaspi5Synth/Settings/ModSynth/Default_Settings";
	
	/* create the general setting manager(mange audio and midi settings) */
	general_settings_manager = new Settings(&active_general_synth_settings_params);
	/* Create the FLuidSynth settings manager */
	fluid_synth_settings_manager = new Settings(&active_fluid_synth_settings_params);

	// Initialize before use
	active_general_synth_settings_params.name = "";
	active_general_synth_settings_params.settings_type = "";
	active_general_synth_settings_params.version = 0;
	
	/* Create a singleton of the AdjSynth. */ // TODO: can it be above
	//adj_synth = AdjSynth::get_instance();
	
	//set_audio_driver_type(_DEFAULT_AUDIO_DRIVER);
	//set_audio_block_size(_DEFAULT_BLOCK_SIZE);
	//set_sample_rate(_DEFAULT_SAMPLE_RATE);
	
	// Init the Adj synthesizers
	// init();  Moved up
	
	// Register the callback that indicates that a new updat cycle has started.
	adj_synth->audio_manager->register_callback_audio_update_cycle_start_tasks
		(&callback_audio_update_cycle_start_tasks_wrapper);
	
	// Open the general settings file
	res = open_mod_synth_general_settings_file(mod_synth_general_settings_file_path + 
												"/ModSynth_general_settings_1");
	if (res != 0)
	{
		// File read failed - set to default settings parameters
		set_default_general_settings_parameters(&active_general_synth_settings_params);
	}

	// Init temp presets   ******************** Presets OR Patches????????????????
	preset_temp.name = "Preset_Temp";
	preset_temp.settings_type = _MOD_SYNTH_PRESET_PARAMS;
	preset_temp.version = general_settings_manager->get_settings_version();
	set_default_preset_parameters(&preset_temp);

	// Initilizes the presets data.
	ModSynthPatches::init();
	
	
	// Start the CPU utilization measuring thread.
	start_cheack_cpu_utilization_thread();
}


ModSynth::~ModSynth()
{
	on_exit();
}

/**
*   @brief  retruns the single modular synth instance
*
*   @param  none
*   @return the single modular synth instance
*/
ModSynth *ModSynth::get_instance()
{
	if (mod_synth == NULL)
	{
		mod_synth = new ModSynth();
	}

	return mod_synth;
}

std::string ModSynth::get_mod_synth_current_working_directory()
{
	return mod_synth_current_working_directory;
}

std::string ModSynth::get_mod_synth_current_patches_directory()
{
	return mod_synth_current_patches_directory;
}

std::string ModSynth::get_mod_synth_current_settings_directory()
{
	return mod_synth_current_settings_directory;
}

std::string ModSynth::get_mod_synth_current_soundfonts_directory()
{
	return mod_synth_current_soundfonts_directory;
}

std::string ModSynth::get_mod_synth_current_midi_files_directory()
{
	return mod_synth_current_midi_files_directory;
}

std::string ModSynth::get_mod_synth_general_settings_file_path()
{
	return mod_synth_general_settings_file_path;
}
	

int ModSynth::init()
{
	settings_res_t res;

	// Init programs - must be called before init voices and settings.
	// Set the synth programs and their preset settings to default values, and assign them to the voices.
	adj_synth->init_synth_programs();
	
	/* Assign the ModSynth general settings manager, active general settings settings parameters.
	 * Mixer, Keyboard, Eqalizer and reverb */
	adj_synth->set_settings_params(general_settings_manager, &active_general_synth_settings_params);
	// Create and initialize the ModSynth default General Settings parameters values.
	set_default_general_settings_parameters(&active_general_synth_settings_params);
	
	// Assign the AdjSynth settings manager, active settings parameters..
	adj_synth->set_settings_params(adj_synth->adj_synth_settings_manager,
		adj_synth->get_active_settings_params());
	
	for (int s = _SKETCH_PROGRAM_1; s <= _SKETCH_PROGRAM_3; s++)
	{
		adj_synth->set_active_sketch(s);
		adj_synth->set_settings_params(adj_synth->adj_synth_settings_manager,
			adj_synth->get_active_settings_params());
	}

	adj_synth->set_active_sketch(_SKETCH_PROGRAM_1);
	
	// Create and initialize the AdjSynth default Preset parameters values.
	
	
	// ****************** TODO:  29-Sep-2025   set_adj_synth_default_settings(adj_synth->get_active_preset_params());
	
	
	// Init programs - must be called before init voices.
	// Set the synth programs and their preset settings to default values, and assign them to the voices.
	//adj_synth->init_synth_programs(); // Moved up.
	
	// Init voices
	adj_synth->init_synth_voices();
	
	// Init polyphony manager
	adj_synth->init_poly();
	
	// Init JACK audio
	adj_synth->init_jack();

	res = general_settings_manager->set_int_param(
		&active_general_synth_settings_params, // TODO: general settings params?
		"synth.master_volume",
		_DEFAULT_MASTER_VOLUME,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL, // no callbacks,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE,
		-1);

	set_fluid_synth_volume(_DEFAULT_FLUID_SYNTH_VOLUME);
	set_master_volume(_DEFAULT_MASTER_VOLUME);

	return 0;
}

/**
*   @brief  Closing and cleanup when application goes down.
*			Must be called whenever terminating the application.
*   @param  none
*   @return void
*/
void ModSynth::on_exit()
{
	if (fluid_synth != NULL)
	{
		fluid_synth->get_fluid_synth_interface()->deinitialize_fluid_synthesizer();
	}
	
	this->stop_cheack_cpu_utilization_thread();
	//TODO: stop whatever else should be terminated
}

/**
*   @brief  Clear all playing notes and polyphonic allocations
*   @param  none
*   @return void
*/
void ModSynth::synth_panic_action()
{
	fluid_synth->get_fluid_synth_interface()->fluid_synth_panic_action();
	AdjSynth::get_instance()->synth_panic_action();
}

/**
*   @brief  performs periodic update tasks - called by the audio processing update cycle process
*   @param  voc	voice number (na - global)
*   @return pointer to adjheart synthesizr
*/
void ModSynth::update_tasks(int voc)
{
	// Update portamento frequency
	if (adj_synth->kbd1->portamento_is_enabled())
	{
		adj_synth->kbd1->update_actual_frequency();
		AdjSynth::get_instance()->synth_voice[_SYNTH_VOICE_1]->dsp_voice->set_voice_frequency(
					adj_synth->kbd1->get_note_frequency());
	}
}

void ModSynth::start_cheack_cpu_utilization_thread()
{
	cheack_cpu_utilization_thread_is_running = true;
	pthread_create(&cheack_cpu_utilization_thread_id, NULL, cheack_cpu_utilization_thread, NULL);
	pthread_setname_np(cheack_cpu_utilization_thread_id, "cpuutilthread");
}

void ModSynth::stop_cheack_cpu_utilization_thread()
{
	cheack_cpu_utilization_thread_is_running = false;
	//	pthread_create(&cheack_cpu_utilization_thread_id, NULL, cheackCpuUtilizationThread, NULL);
	//	pthread_setname_np(cheack_cpu_utilization_thread_id, "cpuutilthread");
}


int ModSynth::init_midi_ext_interface(int ser_port_num)
{
	midi_ext_interface->init_serial_port_services(ser_port_num, 115200, "8N1", 0);
	
	control_box_ext_midi_in_alsa_out->startAlsaOutThread();

	control_box_alsa_out->startAlsaOutThread();
	
	return 0;
}

int ModSynth::deinit_midi_ext_interface()
{
	// TODO:
	
	return 0;
}



/**
*   @brief  retruns a pointer to the global bluetooth alsa midi out client
*   @param  none
*   @return the global bluetooth alsa midi out client
*/
AlsaBtClientOutput* ModSynth::get_bt_alsa_out()
{
	return bt_alsa_out;
}

/**
*   @brief  retruns a pointer to the FluidSynth instrument object
*   @param  none
*   @return a pointer to the FluidSynth instrument object
*/
InstrumentFluidSynth *ModSynth::get_fluid_synth()
{
	return fluid_synth;
}

/**
*   @brief  retruns a pointer to the AnalogSynth instrument object
*   @param  none
*   @return a pointer to the AnalogSynth instrument object
*/
InstrumentAnalogSynth *ModSynth::get_analog_synth()
{
	return analog_synth;
}

/**
*   @brief  retruns a pointer to the FluidSynth instrument object
*   @param  none
*   @return a pointer to the InstrumentHammondOrgan instrument object
*/
InstrumentHammondOrgan *ModSynth::get_hammond_organ()
{
	return hammond_organ;
}

/**
*   @brief  retruns a pointer to the MIDI-Mixer instrument object
*   @param  none
*   @return a pointer to the MIDI-Mixer instrument object
*/
InstrumentMidiMixer *ModSynth::get_midi_mixer()
{
	return midi_mixer_instrument;
}

/**
*   @brief  retruns a pointer to the AnalogReverbration instrument object
*   @param  none
*   @return a pointer to the AnalogReverbration instrument object
*/
InstrumentAnalogReverbration *ModSynth::get_analog_reverberation()
{
	return analog_reverberation_instrument;
}

/**
*   @brief  retruns a pointer to the MIDI-Player instrument object
*   @param  none
*   @return a pointer to the MIDI-Player instrument object
*/
InstrumentMidiPlayer *ModSynth::get_midi_player()
{
	return midi_player;
}

/**
*   @brief  Set the master volume level.
*   @param  vol volume level (0-100)
*   @return none
*/
void ModSynth::set_master_volume(int vol)
{
	double gain;
	settings_res_t res;

	// for correct limit testing, synth.master_volume param must be already intialized
	res = general_settings_manager->set_int_param(
		&active_general_synth_settings_params,
		"synth.master_volume",
		vol,
		100,
		0,
		_ADJ_SYNTH_PRESET_PARAMS,
		NULL,
		_SET_VALUE);

	if (res == _SETTINGS_OK)
	{
		master_volume = vol;
		// Update all other volume levels
		// Fluid synth - max gain 0.25
		gain = (float)(fluid_synth_volume * master_volume) / 40000.f; // 100 * 100 / 40000 = 0.25
		fluid_synth->get_fluid_synth_interface()->set_fluid_synth_gain(gain);

		AdjSynth::get_instance()->set_master_volume(vol);
	}
}

/**
*   @brief  Get the master volume level.
*   @param  none
*   @return volume level (0-100)
*/
int ModSynth::get_master_volume() 
{ 
	return 
		master_volume; 
}

/**
*   @brief  Set the fluid synth volume level.
*   @param  vol volume level (0-100)
*   @return none
*/
void ModSynth::set_fluid_synth_volume(int vol)
{
	// 0-0.25
	double gain = (float)(vol*master_volume) / 40000.f; // 100 * 100 / 40000 = 0.25

	settings_res_t res = fluid_synth->get_fluid_synth_interface()->set_fluid_synth_gain(gain);
	// set gain also verifies range
	if (res == _SETTINGS_OK)
	{
		fluid_synth_volume = vol;			
	}
} 

/**
*   @brief  Get the fluid synth volume level.
*   @param  none
*   @return volume level (0-100)
*/
int ModSynth::get_fluid_synth_volume() 
{ 
	return fluid_synth_volume; 
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*	@param	restart_audio if true only restart audio
*					
*	@return set sample-rate
*/
int ModSynth::set_sample_rate(int samp_rate, bool restart_audio)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	//	adj_synth->set_sample_rate(sample_rate);
		// TODO: fluid_synth
	
	if (restart_audio)
	{
		stop_audio();
		usleep(100000);
		start_audio();
	}
	
	return sample_rate;
}

/**
*   @brief  Starts the audio service.
*			Must be called after setting the audio driver type, 
*			sample-rate and audio block-size (else, default values will be used)..
*   @param  none
*   @return 0 if done
*/
int ModSynth::start_audio()
{
	int res = 0;
	// TODO:: res = start FluidSynth audio handling
	res |= adj_synth->start_audio(audio_driver, sample_rate, audio_block_size);
	
	return res;
}

/**
*   @brief  Stops the audio service.
*   @param  none
*   @return 0 if done
*/
int ModSynth::stop_audio()
{
	int res = 0;
	// TODO:: res = stop FluidSynth audio handling
	res |= adj_synth->stop_audio();
	
	return res;
}

int ModSynth::get_midi_channel_synth(int chan)
{
	if ((chan >= 0) && (chan <= 15))
	{
		 // Holds a bit mask for each MIDI channel 0-15, sketc 1-3 (16-18) indicating assigned synthesizer ?
		 //	Channel 0 is the main edditing channel ?
		return midi_channel_synth[chan];
	}
	else
	{
		return 0;
	}
}

/**
*   @brief  Set the midi channel assigned synthesizer.
*   @param  chan	midi channel number 0-15
*	@parm	synth	synthesizer id
*   @return void
*/
void ModSynth::set_midi_channel_synth(int chan, int synth)
{
	if ((chan >= 0) && (chan <= 15))
	{
		midi_channel_synth[chan] = synth;
	}
}

/**
*	@brief	Sets the sample-rate value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int ModSynth::set_sample_rate(int samp_rate)
{
	return set_sample_rate(samp_rate, false);
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int ModSynth::get_sample_rate() 
{ 
	return sample_rate; 
}

/**
*   @brief  sets the audio block size 
*			Settings will be effective only after the next call to start_audio().
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @param	restart_audio if true restart audio
*   @return size if OK; -1 param out of range
*/
int ModSynth::set_audio_block_size(int size, bool restart_audio)
{	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;		
		
		//		adj_synth->set_audio_block_size(audio_block_size);
				// TODO: fluid_synth
		
		if (restart_audio)
		{
			stop_audio();
			usleep(100000);
			start_audio();
		}
		
		return audio_block_size;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  sets the audio block size value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return size if OK; -1 param out of range
*/
int ModSynth::set_audio_block_size(int size)
{
	return set_audio_block_size(size, false);
}

/**
*	@brief	Sets the audio driver type
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@param	restart_audio if true set value and restart audio
*	@return set audio-driver
*/
int ModSynth::set_audio_driver_type(int driver, bool restart_audio)
{
	if (is_valid_audio_driver(driver))
	{
		audio_driver = driver;
	}
	else
	{
		audio_driver = _DEFAULT_AUDIO_DRIVER;
	}
	
	if (restart_audio)
	{
		stop_audio();
		usleep(100000);
		start_audio();
	}
	
	return audio_driver;
}

/**
*	@brief	Sets the audio driver type  value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@param	set_only if true only set value, false - set value and restart audio
*	@return set audio-driver
*/
int ModSynth::set_audio_driver_type(int driver)
{
	return set_audio_driver_type(driver, false);
}

/**
*	@brief	Returns the audio-driver type
*	@param	none
*	@return audio driver type
*/	
int ModSynth::get_audio_driver_type() 
{ 
	return audio_driver; 
}


	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int ModSynth::get_audio_block_size() 
{ 
	return audio_block_size; 
}

/**
*   @brief  Save AdjSynth Patch parameters as XML file
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*	@param save_mask bit map to set saved params type #_SAVE_FLUID_SETTINGS, #_SAVE_ADJ_SYNTH_PATCH, _SAVE_ADJ_SYNTH_SETTINGS
*   @return 0 if done
*/
int ModSynth::save_adj_synth_patch_file(string path, Settings *settings, _settings_params_t *params)
{
	int res;
	XML_files *xml_files = new XML_files();
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);

	res = settings->write_settings_file(
		params,
		settings->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		_ADJ_SYNTH_PRESET_PARAMS);
	
	//	printf("Save settings to  %s\n", path.c_str());

	return res;
}

/**
*   @brief  Open an AdjSynth Patch parameters XML file and set it parameters
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*   @return 0 if done
*/
int ModSynth::open_adj_synth_patch_file(string path, Settings *settings, _settings_params_t *params, int channel)
{
	settings_res_t res;
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);

	res = settings->read_settings_file(params, path, _ADJ_SYNTH_PRESET_PARAMS, channel);

	if (res == _SETTINGS_OK)
	{
		//		adj_synth->synth_program[channel]->set_program_patch_params(params);
		
		adj_synth->synth_program[channel]->synth_pad_creator->generate_wavetable(
			adj_synth->synth_program[channel]->program_wavetable);

		adj_synth->synth_program[channel]->mso_wtab->calc_segments_lengths(
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_lengths, 
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_positions);
		
		adj_synth->synth_program[channel]->mso_wtab->calc_wtab(
			adj_synth->synth_program[channel]->mso_wtab->morphed_waveform_tab, 
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_lengths, 
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_positions);
		
		//	printf("Open settings  %s\n", path.c_str());
		return 0;
	}
	else
	{
		return -1;
	}	

	return res;
}

/**
*   @brief  Save the active AdjSynth patch parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::save_adj_synth_patch_file(string path)
{
	return save_adj_synth_patch_file(path,
		adj_synth->adj_synth_settings_manager,
		#ifdef _USE_NEW_MIDI_PROGRAM
		adj_synth->get_active_preset_params());  // synth_program[mod_synth_get_active_sketch()]->active_preset_params);
#else
									 &adj_synth->synth_program[mod_synth_get_active_sketch()]->active_preset_params);							 
#endif

}

/**
*   @brief  Open settings parameters XML file and set it as the AdjSynth active patch parameters
*   @param  path settings XML file full path
*	@param	channel	midi channel (0-15, 16-18 for active patch).
*   @return 0 if done
*/
int ModSynth::open_adj_synth_patch_file(string path, int channel)
{
	return open_adj_synth_patch_file(path,
									 adj_synth->adj_synth_settings_manager,
#ifdef _USE_NEW_MIDI_PROGRAM
									 adj_synth->synth_program[channel]->active_preset_params,						 
#else
									 &adj_synth->synth_program[channel]->active_preset_params,		
#endif
									channel);
}

/**
*   @brief  Save the active AdjSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*	@param save_mask bit map to set saved params type #_SAVE_FLUID_SETTINGS, #_SAVE_ADJ_SYNTH_PATCH, _SAVE_ADJ_SYNTH_SETTINGS
*   @return 0 if done
*/
int ModSynth::save_adj_synth_settings_file(string path, Settings *settings, _settings_params_t *params)
{
	XML_files *xml_files = new XML_files();
	int res;
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);

	res = settings->write_settings_file(
		params, 
		settings->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		_ADJ_SYNTH_SETTINGS_PARAMS);
	return res;
}

/**
*   @brief  Open an AdjSynth settings parameters XML file and set it as the active settings
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*   @return 0 if done
*/
int ModSynth::open_adj_synth_settings_file(string path, Settings *settings, _settings_params_t *params)
{
	int res;
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = settings->read_settings_file(params, path, _ADJ_SYNTH_SETTINGS_PARAMS);
	
	//	printf("Open settings  %s\n", path.c_str());

	return res;
}

/**
*   @brief  Save the active ModSynth general settings parameters as XML file
*   @param  path general settings XML file full path
*   @return 0 if done
*/
int ModSynth::save_mod_synth_general_settings_file(string path)
{
	return save_adj_synth_settings_file(path,
		general_settings_manager,
		&active_general_synth_settings_params);
}

/**
*   @brief  Open general settings parameters XML file and set it as the AdjSynth active settings parameters
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::open_mod_synth_general_settings_file(string path)
{
	return open_adj_synth_settings_file(path,
		general_settings_manager, 
		&active_general_synth_settings_params);
}


void ModSynth::note_on(uint8_t channel, uint8_t note, uint8_t velocity)
{

}

void ModSynth::note_off(uint8_t channel, uint8_t note)
{

}

void ModSynth::change_program(uint8_t channel, uint8_t program)
{

}

void ModSynth::channel_pressure(uint8_t channel, uint8_t val)
{

}

void ModSynth::controller_event(uint8_t channel, uint8_t num, uint8_t val)
{

}

void ModSynth::pitch_bend(uint8_t channel, int pitch)
{

}


void *cheack_cpu_utilization_thread(void *arg)
{
	sleep(1);
	while (cheack_cpu_utilization_thread_is_running)
	{
		CPUSnapshot previousSnap;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		CPUSnapshot curSnap;

		const float ACTIVE_TIME = curSnap.GetActiveTimeTotal() - previousSnap.GetActiveTimeTotal();
		const float IDLE_TIME = curSnap.GetIdleTimeTotal() - previousSnap.GetIdleTimeTotal();
		const float TOTAL_TIME = ACTIVE_TIME + IDLE_TIME;
		ModSynth::cpu_utilization = (int)(100.f * ACTIVE_TIME / TOTAL_TIME);
	}

	return NULL;
}


