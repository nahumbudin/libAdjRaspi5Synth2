/**
 * @file		HttpBridge_ByteArrayParamsDispatcher.cpp
 *	@author		Nahum Budin
 *	@date		14-June-2026
 *	@version	1.0
 *
 *	@brief		HTTP Server REST Api call for byte array parameter dispatching.
 *				Used for raw midi data transfer.
 *
 */

#include "HttpBridge.h"

void HttpBridge::set_binary_parm_dispatcher(int module_id, int sub_module_id, int param_id, const std::vector<uint8_t> &val)
{
	// Will be streamed in as ALSA MIDI stream by the controlBoxClientAlsaOutput thread.
	
	int size = static_cast<int>(val.size());
	
	// Use object pool instead of new
	httpMidiRxData_t *rx_data = &rx_data_pool[pool_index];
	pool_index = (pool_index + 1) % POOL_SIZE;	

	rx_data->interface_num = param_id;
	rx_data->mssg_len = size;
	std::copy(val.begin(), val.end(), rx_data->message);
	alsa_midi_http_interface_rx_queue.enqueue(rx_data);
}