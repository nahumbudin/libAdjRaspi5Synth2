/**
* @file			http.h
*	@author		Nahum Budin
*	@date		16-June-2026
*	@version	1.0
*	
*	@brief		Provides the HTTP related API
*	
*/

#pragma once

#include <functional>
#include <string>
#include <vector>

typedef void(*lib_api_callback_t)(int mod, int sub, int param, const char* type, const uint8_t* data, int data_len);
    
// Allows the host application to register a callback function to catch web updates
void mod_synthesizer_http_server_register_callback(lib_api_callback_t callback);
bool mod_synthesizer_http_server_start(const char* host, int port);
void mod_synthesizer_http_server_stop();
bool mod_synthesizer_http_server_is_running();