
#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <cstdint>
#include <functional>

// Third-party headers bundled with your library source
#include "../httplib.h"
#include "../json.hpp"

#include "../utils/safeQueues.h"

typedef struct httpMidiRxData
{
	httpMidiRxData()
		: interface_num(16)
		, mssg_len(0) {}

	int interface_num;
	uint8_t message[256];
	int mssg_len;
} httpMidiRxData_t;

class HttpBridge
{
public:
	HttpBridge();
	~HttpBridge();

	// Starts the server loop inside a standalone std::thread
	bool start(const std::string& host = "0.0.0.0", int port = 8080);
    
	// Safely stops the server and joins the background thread
	void stop();
    
	// Returns true if the background server thread is actively listening
	bool is_running() const;

	// Direct C++ callback signature to replace Qt signals
	using ParameterCallback = std::function<void(int mod, int sub, int param, const std::string& type, const std::vector<uint8_t>& data)>;
    
	// Allows the host application to register a callback function to catch web updates
	void register_parameter_update_callback(ParameterCallback callback);
	
	// --- STANDALONE GETTER OVERLOAD SIGNATURES ---
	void get_parm(int module_id, int sub_module_id, int param_id, int& val);
	void get_parm(int module_id, int sub_module_id, int param_id, float& val);
	void get_parm(int module_id, int sub_module_id, int param_id, bool& val);
	void get_parm(int module_id, int sub_module_id, int param_id, std::string& val);
	void get_parm(int module_id, int sub_module_id, int param_id, std::vector<uint8_t>& val);
	
	/* A queue used to send into the system the incoming MIDI data as ALSA midi events */
	static SafeQueue<httpMidiRxData_t *> alsa_midi_http_interface_rx_queue;

private:
	httplib::Server m_server;
	std::thread server_thread;
	std::atomic<bool> thread_running;
	ParameterCallback update_callback;

	void setupRoutes();
    
	// Base64 helper definitions built into the standalone class
	bool isValidBase64(const std::string& s) const;
	std::vector<uint8_t> decodeBase64(const std::string& input) const;
	std::string encodeBase64(const std::vector<uint8_t>& input) const;
	
	// Object pool to avoid heap allocation in hot path
	static const int POOL_SIZE = 16;
	httpMidiRxData_t rx_data_pool[POOL_SIZE];
	int pool_index = 0;
	
	void set_int_parm_dispatcher(int module_id, int sub_module_id, int param_id, int val);
	void set_float_parm_dispatcher(int module_id, int sub_module_id, int param_id, float val);
	void set_bool_parm_dispatcher(int module_id, int sub_module_id, int param_id, bool val);
	void set_string_parm_dispatcher(int module_id, int sub_module_id, int param_id, const std::string &val);
	void set_binary_parm_dispatcher(int module_id, int sub_module_id, int param_id, const std::vector<uint8_t> &val);
};


