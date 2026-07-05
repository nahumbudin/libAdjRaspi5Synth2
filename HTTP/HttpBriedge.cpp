

#include "HttpBridge.h"
#include <iostream>
#include <algorithm>
#include <cctype>

SafeQueue<httpMidiRxData_t *> HttpBridge::alsa_midi_http_interface_rx_queue;

HttpBridge::HttpBridge() 
	: thread_running(false)
	, update_callback(nullptr)
{
	setupRoutes();
}

HttpBridge::~HttpBridge()
{
	stop();
}

bool HttpBridge::start(const std::string& host, int port)
{
	if (thread_running.load()) return false;
	thread_running.store(true);

	// Launch a pure standard background worker thread
	server_thread = std::thread([this, host, port]() {
		std::cout << "[Bridge Thread] Attempting to listen on " << host << ":" << port << std::endl;
        
		if (!m_server.listen(host.c_str(), port)) {
			std::cerr << "[Bridge Thread] CRITICAL: Failed to bind socket!" << std::endl;
			thread_running.store(false);
		}
	});

	return true;
}

void HttpBridge::stop()
{
	if (thread_running.load()) {
		m_server.stop(); // Signals the httplib loop to break
        
		if (server_thread.joinable()) {
			server_thread.join(); // Wait for the background thread to safely exit
		}
		thread_running.store(false);
		std::cout << "[Bridge Thread] Server stopped cleanly." << std::endl;
	}
}

bool HttpBridge::is_running() const
{
	return thread_running.load();
}

void HttpBridge::register_parameter_update_callback(ParameterCallback callback)
{
	update_callback = callback;
}

void HttpBridge::get_parm(int module_id, int sub_module_id, int param_id, int& val) {
	// Call your static library internal code here
	val = 42; 
}

void HttpBridge::get_parm(int module_id, int sub_module_id, int param_id, float& val) {
	val = 3.1415f; 
}

void HttpBridge::get_parm(int module_id, int sub_module_id, int param_id, bool& val) {
	val = true; 
}

void HttpBridge::get_parm(int module_id, int sub_module_id, int param_id, std::string& val) {
	val = "Hello From Pure C++ Lib Backend"; 
}

void HttpBridge::get_parm(int module_id, int sub_module_id, int param_id, std::vector<uint8_t>& val) {
	// Verified 5-byte pattern with correct trailing data offsets
	val = { 0xDE, 0xAD, 0xBE, 0xEF, 0x18 }; 
}


void HttpBridge::setupRoutes()
{
	// --- POST: /set_param ---
	m_server.Post("/set_param",
		[this](const httplib::Request& req, httplib::Response& res) {
			res.set_header("Access-Control-Allow-Origin", "*");
			try {
				auto inputJson = nlohmann::json::parse(req.body);
				int mod = inputJson.at("module_id").get<int>();
				int sub = inputJson.at("sub_module_id").get<int>();
				int param = inputJson.at("param_id").get<int>();
				auto valField = inputJson.at("val");

				std::string typeDetected = "unknown";
				std::vector<uint8_t> rawBytes;

				if (valField.is_number_integer()) {
					typeDetected = "int";
					int val = valField.get<int>();
					set_int_parm_dispatcher(mod, sub, param, val);
                
					// Pack int to byte vector for the callback payload
					rawBytes.assign(reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(int));
				} 
				else if (valField.is_number_float()) {
					typeDetected = "float";
					float val = valField.get<float>();
					set_float_parm_dispatcher(mod, sub, param, val);
                
					rawBytes.assign(reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(float));
				} 
				else if (valField.is_boolean()) {
					typeDetected = "bool";
					bool val = valField.get<bool>();
					set_bool_parm_dispatcher(mod, sub, param, val);
                
					rawBytes.push_back(val ? 1 : 0);
				} 
				else if (valField.is_string()) {
					std::string rawStr = valField.get<std::string>();
					if (isValidBase64(rawStr)) {
						typeDetected = "byte[]";
						rawBytes = decodeBase64(rawStr);
						set_binary_parm_dispatcher(mod, sub, param, rawBytes);
					}
					else {
						typeDetected = "string";
						rawBytes.assign(rawStr.begin(), rawStr.end());
						set_string_parm_dispatcher(mod, sub, param, rawStr);
					}
				}

				// Trigger the C++ callback if the parent application registered one
				if (update_callback && typeDetected != "unknown") {
					update_callback(mod, sub, param, typeDetected, rawBytes);
				}

				res.status = 200;
				res.set_content(nlohmann::json{ { "status", "success" } }.dump(), "application/json");
			}
			catch (const std::exception& e) {
				res.status = 400;
				res.set_content(nlohmann::json{ { "status", "error" }, { "message", e.what() } }.dump(), "application/json");
			}
		});

	// --- Handling Browser Handshake Handlers (CORS Options) ---
	m_server.Options(R"(.*)",
		[](const httplib::Request&, httplib::Response& res) {
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
			res.set_header("Access-Control-Allow-Headers", "Content-Type");
			res.status = 200;
		});
	
	// --- POST: /get_param ---
	m_server.Post("/get_param",
		[this](const httplib::Request& req, httplib::Response& res) {
			res.set_header("Access-Control-Allow-Origin", "*");
			try {
				auto inputJson = nlohmann::json::parse(req.body);
				int mod = inputJson.at("module_id").get<int>();
				int sub = inputJson.at("sub_module_id").get<int>();
				int param = inputJson.at("param_id").get<int>();
				std::string requestedType = inputJson.value("type", "int");
            
				nlohmann::json outputJson;
				outputJson["status"] = "success";

				// Dynamic type evaluation and reference population
				if (requestedType == "int") {
					int val; 
					get_parm(mod, sub, param, val);
					outputJson["val"] = val;
				} 
				else if (requestedType == "float") {
					float val; 
					get_parm(mod, sub, param, val);
					outputJson["val"] = val;
				} 
				else if (requestedType == "bool") {
					bool val; 
					get_parm(mod, sub, param, val);
					outputJson["val"] = val;
				} 
				else if (requestedType == "string") {
					std::string val; 
					get_parm(mod, sub, param, val);
					outputJson["val"] = val;
				} 
				else if (requestedType == "byte[]" || requestedType == "binary") {
					std::vector<uint8_t> binaryVal;
					get_parm(mod, sub, param, binaryVal);
                
					// Use the internal bug-free base64 method to serialize back safely
					outputJson["val"] = encodeBase64(binaryVal);
				} 
				else {
					throw std::runtime_error("Unknown data profile type requested");
				}

				res.status = 200;
				res.set_content(outputJson.dump(), "application/json");
			}
			catch (const std::exception& e) {
				res.status = 400;
				res.set_content(nlohmann::json{ { "status", "error" }, { "message", e.what() } }.dump(), "application/json");
			}
		});

}

// =========================================================================
//   BUG-FREE NATIVE BASE64 HELPERS (Unchanged from our verified code)
// =========================================================================
bool HttpBridge::isValidBase64(const std::string& s) const {
	if (s.empty() || s.length() % 4 != 0) return false;
	return std::all_of(s.begin(),
		s.end(),
		[](unsigned char c) {
			return (std::isalnum(c) || c == '+' || c == '/' || c == '=');
		});
}

std::vector<uint8_t> HttpBridge::decodeBase64(const std::string& input) const {
	static const std::string b64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::vector<uint8_t> out;
	int val = 0, valb = -8;
	for (unsigned char c : input) {
		if (c == '=') break;
		size_t idx = b64_chars.find(c);
		if (idx == std::string::npos) continue;
		val = (val << 6) + idx;
		valb += 6;
		if (valb >= 0) {
			out.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}

std::string HttpBridge::encodeBase64(const std::vector<uint8_t> &input) const
{
	static const char b64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string out;
	size_t i = 0;
	size_t length = input.size();

	// 1. Process bytes in groups of 3 (24 bits total)
	while (i < length)
	{
		uint32_t octet_a = i < length ? input[i++] : 0;
		uint32_t octet_b = i < length ? input[i++] : 0;
		uint32_t octet_c = i < length ? input[i++] : 0;

		// Combine the three 8-bit bytes into a single 24-bit integer
		uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

		// Split the 24-bit integer into four 6-bit indexes and map to Base64 characters
		out.push_back(b64_chars[(triple >> 18) & 0x3F]);
		out.push_back(b64_chars[(triple >> 12) & 0x3F]);
		out.push_back(b64_chars[(triple >> 6) & 0x3F]);
		out.push_back(b64_chars[triple & 0x3F]);
	}

	// 2. Add standard padding character markers if the array length isn't a multiple of 3
	size_t remainder = length % 3;
	if (remainder == 1)
	{
		// One trailing byte remaining -> requires two padding characters
		out[out.size() - 1] = '=';
		out[out.size() - 2] = '=';
	}
	else if (remainder == 2)
	{
		// Two trailing bytes remaining -> requires one padding character
		out[out.size() - 1] = '=';
	}

	return out;
}
