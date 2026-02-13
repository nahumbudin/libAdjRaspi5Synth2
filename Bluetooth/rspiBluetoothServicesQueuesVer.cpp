/**
* @file		rspiBluetoothServicesQueuesVer.h
*	@author		Nahum Budin
*	@date		13-May-2024
*	@version	1.3
*					1. Update includes.
*
*	@brief		Bluetooth thered using thread-safe message queues for transferring data.
*
*	History:\n
*
*		version	1.2		6-Feb-2021
*
*		version 1.1		10-Oct-2019 (Updated)
*			1. Code refactoring and notaion.
*/

#include <sys/select.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <errno.h>
#include <sys/shm.h>		//Used for shared memory
#include <fcntl.h>

#include "rspiBluetoothServicesQueuesVer.h"
#include "../misc/priorities.h"

char Raspi3BluetoothQ::addr[19];
char Raspi3BluetoothQ::name[248];
int Raspi3BluetoothQ::connection_socket;
bt_chan_data_t Raspi3BluetoothQ::bt_channels_data[_NUMBER_OF_BT_CONNECTIONS];
bool Raspi3BluetoothQ::bt_main_thread_is_running;
bool Raspi3BluetoothQ::connected_thread_is_running[_NUMBER_OF_BT_CONNECTIONS];
struct sockaddr_rc Raspi3BluetoothQ::local_address;
bdaddr_t Raspi3BluetoothQ::_BDADDR_ANY;
pthread_t Raspi3BluetoothQ::bt_main_thread_id;
pthread_t Raspi3BluetoothQ::connected_thread_id[_NUMBER_OF_BT_CONNECTIONS];
uint32_t Raspi3BluetoothQ::channel_allocated_mask;
uint8_t Raspi3BluetoothQ::num_of_used_channels;

Raspi3BluetoothQ* Raspi3BluetoothQ::raspi_bluetooth_instance;

SafeQueue<bt_chan_data_t*> Raspi3BluetoothQ::bt_rx_queue_alsa;
SafeQueue<bt_chan_data_t*> Raspi3BluetoothQ::bt_rx_queue_raw;
SafeQueue<bt_chan_data_t*> Raspi3BluetoothQ::bt_tx_queue;

bt_chan_data_t* tx_channel_data;

Raspi3BluetoothQ::Raspi3BluetoothQ()
{
	int j, err;
	unsigned int i;

	// Initializations
	for (j = 0; j < _NUMBER_OF_BT_CONNECTIONS; j++)
	{
		for (i = 0; i < sizeof(bt_channels_data[j].rem_addr.rc_bdaddr); i++)
		{
			bt_channels_data[j].rem_addr.rc_bdaddr.b[i] = 0;
		}

		channel_allocated_mask |= (1 << (sizeof(channel_allocated_mask) * 8 - j - 1));

		connected_thread_is_running[j] = false;
		bt_channels_data[j].channel_id = 0;
		bt_channels_data[j].client_id = 0;
	}

	bt_main_thread_is_running = false;

	for (i = 0; i < sizeof(addr); i++)
	{
		addr[i] = 0;
	}

	for (i = 0; i < sizeof(name); i++)
	{
		name[i] = 0;
	}

	for (i = 0; i < sizeof(local_address.rc_bdaddr); i++)
	{
		local_address.rc_bdaddr.b[i] = 0;
	}

	for (i = 0; i < sizeof(_BDADDR_ANY.b); i++)
	{
		_BDADDR_ANY.b[i] = 0;
	}

	num_of_used_channels = 0;
}

Raspi3BluetoothQ::~Raspi3BluetoothQ()
{
	// Stop all running connections threads
	for (int i = 0; i < _NUMBER_OF_BT_CONNECTIONS; i++)
	{
		connected_thread_is_running[i] = false;
	}
	// Stop the main BT thread
	bt_main_thread_is_running = false;
}

/**
*   @brief  retruns the single bluetooth handling instance
*
*   @param  none
*   @return tthe single bluetooth handling instance
*/
Raspi3BluetoothQ* Raspi3BluetoothQ::get_instance()
{
	if (raspi_bluetooth_instance == NULL)
	{
		raspi_bluetooth_instance = new(Raspi3BluetoothQ);
	}

	return raspi_bluetooth_instance;
}


/**
 * Scan for nearby BT devices
 *
 * @param *res a pointer to the reaults structure
 * @return parameter #_KEY_NOT_FOUND if the name has been found, #_KEY_NOT_FOUND otherwise
 */
int Raspi3BluetoothQ::scan(inq_resQ_t* res)
{
	int connection_socket, sum, a = 0, n = 0;

	if (res == NULL)
	{
		return _BT_RESULT_BAD_PARAM;
	}

	fprintf(stderr, "Scanning for nearby BT devices (this may take about 30 seconds)....\n");

	// Get the id of the first bluetooth device available (most likely 0)
	dev_id = hci_get_route(NULL);
	// Open a BT socket with the device
	connection_socket = hci_open_dev(dev_id);
	if (dev_id < 0 || connection_socket < 0)
	{
		fprintf(stderr, "BT: error opening socket");
		return _BT_RESULT_SOCKET_OPEN_ERR;
	}

	// Inquiery lasts at most 1.28 * len seconds
	len = 20;
	// Max Rsp devices will be returned
	max_rsp = 255;
	// Cash of previouaslly detected devices is flushed
	flags = IREQ_CACHE_FLUSH;
	inq_info = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
	// Perform a BT devices discovery and return the number and a list of detected devices info
	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &inq_info, flags);
	if (num_rsp < 0)
	{
		perror("BT: hci_inquiry");
	}

	// Initialize the results
	res->name = new char[num_rsp * 32];
	res->rem_addr = new char[num_rsp * 32];
	for (i = 0; i < num_rsp * 32; i++)
	{
		res->name[i] = 0;
		res->rem_addr[i] = 0;
	}
	// For each discoered device
	for (i = 0; i < num_rsp; i++)
	{
		// Convert the BT info into strings
		ba2str(&(inq_info + i)->bdaddr, addr);
		ba2str(&(inq_info + i)->bdaddr, &res->rem_addr[a]);
		a += 31;
		// Build a comma seperated list of addresses
		res->rem_addr[a] = ',';
		a++;
		memset(name, 0, sizeof(name));
		// Get device name
		if (hci_read_remote_name(connection_socket, &(inq_info + i)->bdaddr, sizeof(name), name, 0) < 0)
		{
			strcpy(name, "[unknown]");
		}
		// Build a comma separated list of devices names
		strcpy((res->name + n), name);
		n += 31;
		res->name[n] = ',';
		n++;

		printf("BT device %s  %s\n", addr, name);
	}

	// Comprees strings by removing nulls
	for (a = 0; a < num_rsp * 32; a++)
	{
		while (res->rem_addr[a] == 0)
		{
			sum = 0;
			for (n = a; n < num_rsp * 32; n++)
			{
				res->rem_addr[n] = res->rem_addr[n + 1];
				sum += res->rem_addr[n];
			}
			if (sum == 0)
			{
				// All the rest are 0
				break;
			}
		}
	}
	// Comprees strings by removing nulls
	for (a = 0; a < num_rsp * 32; a++)
	{
		while (res->name[a] == 0)
		{
			sum = 0;
			for (n = a; n < num_rsp * 32; n++)
			{
				res->name[n] = res->name[n + 1];
				sum += res->name[n];
			}
			if (sum == 0)
			{
				// All the rest are 0
				break;
			}
		}
	}

	free(inq_info);
	close(connection_socket);
	// Number of devices
	res->num_rsp = num_rsp;

	return 0;
}

/**
 * Starts the BT main thread
 */
void Raspi3BluetoothQ::start_bt_main_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the BT thread is not allready running
	if (bt_main_thread_is_running)
	{
		return;
	}

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_BT_MAIN;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	//	err = errno; // for debug
		//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0) {
		// Print the error
		fprintf(stderr, "Unsuccessful in setting BT Main thread realtime prio\n");
	}
	// Set the control flag
	bt_main_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&bt_main_thread_id, &tattr, bt_main_thread, (void*)100);
	char thname[] = { "bt_main_thread" };
	pthread_setname_np(bt_main_thread_id, &thname[0]);
}

/**
 * Starts the BT connected thread (running for each established connection)
 */
void Raspi3BluetoothQ::start_connected_thread(int tid)
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify the thread is not already running
	if (connected_thread_is_running[tid])
	{
		return;
	}

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_BT_CONNECTED;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	err = errno;

	if (ret != 0) {
		// Print the error
		fprintf(stderr, "Unsuccessful in setting BT Connected thread realtime prio\n");
	}
	// Mark the thread out of the multiple connections
	connected_thread_is_running[tid] = true;
	ret = pthread_create(&connected_thread_id[tid], &tattr, connected_thread, (void*)tid);
}

/**
 * Stop the BT main thread
 */
void Raspi3BluetoothQ::stop_bt_main_thread()
{
	bt_main_thread_is_running = false;
}

/**
 * Stop the BT connected thread (running for each established connection)
 */
void Raspi3BluetoothQ::stop_connected_thread(int tid)
{
	connected_thread_is_running[tid] = false;
}

/**
 * Allocate a free channel out of the available channels
 */
int Raspi3BluetoothQ::allocate_channel()
{
	uint8_t n;

	if (num_of_used_channels >= _NUMBER_OF_BT_CONNECTIONS)
	{
		return _BT_RESULT_NO_FREE_CHANNELS;
	}
	// count the number of leading zeros in the channels mask - the 1st available channel
	n = __builtin_clz(channel_allocated_mask);
	// Mark the channel as used by clearing the bit mask
	channel_allocated_mask &= ~(0x80000000 >> n);
	num_of_used_channels++;

	return n;
}

/**
 * Releases a channel
 *
 * @param chan the released channel number
 */
void Raspi3BluetoothQ::release_channel(int chan)
{
	// No used channels
	if ((chan >= _NUMBER_OF_BT_CONNECTIONS) || (chan < 0) || num_of_used_channels <= 0)
	{
		return;
	}
	// Mark as free by setting the bit mask
	channel_allocated_mask |= (0x80000000 >> chan);
	num_of_used_channels--;
}

/**
 * Check if a channel is free
 *
 * @param chan the released channel number
 * @return #_BT_RESULT_CHANNEL_IS_FREE if channel is free #_BT_RESULT_CHANNEL_IS_USED otherwise
 */
bool Raspi3BluetoothQ::channel_is_free(int chan)
{
	if ((chan >= _NUMBER_OF_BT_CONNECTIONS) || (chan < 0))
	{
		return _BT_RESULT_CHANNEL_IS_USED;
	}
	// Channel is free if mask bit is set
	return (channel_allocated_mask & (0x80000000 >> chan)) != 0;
}

bt_chan_data_t* Raspi3BluetoothQ::get_channel_data(int channel)
{
	if ((channel >= 0) && (channel < _NUMBER_OF_BT_CONNECTIONS))
	{
		return &bt_channels_data[channel];
	}
	else
	{
		return NULL;
	}
}

/**
 * Sends data to a connected client.
 *
 * @param chan the connection channel number
 * @param data a pointer to the data chars array
 * @param len the number of data bytes to send
 * @return #_BT_RESULT_OK if data was sent, #_BT_RESULT_ILLEGAL_CHANNEL,
 *		   #_BT_RESULT_CHANNEL_NOT_CONNECTED, #_BT_RESULT_TX_MSSG_TOO_LONG
 *		   #_BT_RESULT_TX_NO_DATA
 */
int Raspi3BluetoothQ::send_data_client(int channel, char* data, int len)
{
	int i, result = _BT_RESULT_OK;

	if ((channel < 0) || (channel >= _NUMBER_OF_BT_CONNECTIONS))
	{
		// Illegal parameter
		result = _BT_RESULT_ILLEGAL_CHANNEL;
	}
	else if (channel_is_free(channel))
	{
		result = _BT_RESULT_CHANNEL_NOT_CONNECTED;
	}
	else if (len > _NUM_OF_TX_BYTES)
	{
		result = _BT_RESULT_TX_MSSG_TOO_LONG;
	}
	else if (data == NULL)
	{
		result = -_BT_RESULT_TX_NO_DATA;
	}
	else
	{
		// Put data into the Tx queue
		// Allocate channel data structure. Don't forget to relase when poping out of Q.
		tx_channel_data = new bt_chan_data();
		tx_channel_data->channel_id = bt_channels_data[channel].channel_id;
		tx_channel_data->client_id = bt_channels_data[channel].client_id;
		tx_channel_data->rem_addr = bt_channels_data[channel].rem_addr;
		// Copy data
		tx_channel_data->message->mssg_len = len;
		for (i = 0; i < len; i++)
		{
			tx_channel_data->message->data[i] = *(data + i);
		}
		// Push into queue
		bt_tx_queue.enqueue(tx_channel_data);
	}

	return result;
}

/**
* BT service main thread.
*
* 1. Allocates a socket bind to it and accepts it.
* 2. Makes the socket nonblocking.
* 3. Loop:		wait for new connections.
*				allocate a free channel, if available
*				initiate a connected thread to receive data from connected data.
*/
void* Raspi3BluetoothQ::bt_main_thread(void* threadid)
{
	int rc, err, free_channel, tmp_client_id, on = 1;
	bool first = true, error_cond = false;
	struct sockaddr_rc tmp_rem_addr;
	socklen_t opt = sizeof(struct sockaddr_rc);

	/* Debeug data */
	int debug_count = 0;
	char debug_data[] = { 1, 2, 3 };
	char dbg_str[128] = { 0 };

	fprintf(stderr, "BT main thread started\n");
	// allocate a socket
	connection_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if (connection_socket < 0)
	{
		// Allocation failed
		err = errno;
		fprintf(stderr, "BT main thread Socket error\n");
		error_cond = true;
	}
	else
	{
		fprintf(stderr, "BT main thread: Socket OK\n");
		// bind thesocket to port 1 of the first available local bluetooth adapter
		local_address.rc_family = AF_BLUETOOTH;
		local_address.rc_bdaddr = _BDADDR_ANY;
		local_address.rc_channel = (uint8_t)1;
		bind(connection_socket, (struct sockaddr*)&local_address, sizeof(local_address));
		// put socket into listening mode
		rc = -1;
		rc = listen(connection_socket, 1);
		// Socket accepted?
		if (rc < 0)
		{
			// Not accepted
			err = errno;
			fprintf(stderr, "BT main thread error: Listen\n");
			error_cond = true;
		}
		else
		{
			// Accepted
			fprintf(stderr, "BT main thread: Start looking for connections\n");
			while (bt_main_thread_is_running && !error_cond)
			{
				if (first)
				{
					// Only once allow socket descriptor to be reuseable
					rc = setsockopt(connection_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
					if (rc < 0)
					{
						// Faile to set socket options
						err = errno;
						perror("Main BT thread: setsockopt() failed");
						error_cond = true;
						break;
						// :TODO
					}
					// Only once set socket to be nonblocking
					rc = ioctl(connection_socket, FIONBIO, (char*)&on);
					if (rc < 0)
					{
						// Failed to set socket
						err = errno;
						fprintf(stderr, "BT main thread: ioctl() failed");
						error_cond = true;
						break;
						// TODO:							
					}
				}
				// accept one connection
				tmp_client_id = accept(connection_socket, (struct sockaddr*)&tmp_rem_addr, &opt);
				err = errno;
				if (tmp_client_id < 0)
				{
					if ((err != EAGAIN) && (err != EWOULDBLOCK))
					{
						// Error - not a in connection state
						err = errno;
						fprintf(stderr, "BT main thread: Accept error\n");
						error_cond = true;
						break;
					}
				}
				else
				{
					// Establish a new connection to a new allocated free channel
					free_channel = allocate_channel();
					if (free_channel >= 0)
					{
						// A free channel is available - update channel info
						bt_channels_data[free_channel].channel_id = free_channel;
						bt_channels_data[free_channel].client_id = tmp_client_id;
						bt_channels_data[free_channel].rem_addr = tmp_rem_addr;
						// Convert address to string
						ba2str(&bt_channels_data[free_channel].rem_addr.rc_bdaddr, dbg_str);
						fprintf(stderr, "BT main thread: Client %i accepted connection from %s\n", free_channel, dbg_str);
						fprintf(stderr, "BT main thread: Client %i Connected\n", free_channel);
						fprintf(stderr, "BT main thread: %i channels used out of %i.\n", num_of_used_channels, _NUMBER_OF_BT_CONNECTIONS);
						// Start a new connection thread allocated to this channel
						start_connected_thread(free_channel);
					}
					else
					{
						// No available free channel - close
						fprintf(stderr, "BT main thread: No free channels. Channels %i used out of %i.\n", num_of_used_channels, _NUMBER_OF_BT_CONNECTIONS);
						close(tmp_client_id);
					}
				} // new connection
				// Keepalive message - send to avoid non active channel disconnection [Is it really executed?]
				debug_count++;
				if (debug_count >= 1000)
				{
					debug_count = 0;
					rc = send_data_client(0, debug_data, 3);
				}

				usleep(10000);

			} // while thread is running loop
		} // listen	ok				
	} // valid socket
	if (error_cond)
	{

		; // TODO
	}

	fprintf(stderr, "BT main thread exit\n");
	// Close any opened connection

	close(connection_socket);

	return NULL;
}

/**
* BT connection thread.
*
* Handlles channel/connection data transfer while connected
*/
void* Raspi3BluetoothQ::connected_thread(void* threadid)
{
	bool loop = true, timeout;
	int bytes_read, i, rc, on = 1, err = 1;
	bt_chan_data_t* tx_channel;
	bt_chan_data_t* rx_channel_alsa, * rx_channel_raw;
	char data[_NUM_OF_RX_BYTES];

	// Get thread id = channel
	int tid = (intptr_t)threadid;
	// Get thread identifier
	pthread_t th_id = pthread_self();
	// Set thread name string to include channel number
	char thname[16];
	sprintf(thname, "bt_conected_thread%i", bt_channels_data[tid].client_id);
	pthread_setname_np(th_id, thname);

	// Put the socket in non-blocking mode:
	rc = ioctl(bt_channels_data[tid].client_id, FIONBIO, (char*)&on);
	if (rc < 0)
	{
		// Failed to set socket
		err = errno;
		fprintf(stderr, "BT connected thread: ioctl() failed");
		loop = false;
		// :TODO
	}

	while ((connected_thread_is_running[tid]) && loop)
	{
		// Wait for a new Tx message or a timeout of 5msec.
		tx_channel = bt_tx_queue.dequeue(5, &timeout);
		if (!timeout)
		{
			// Exit not on timeout - New tx data is available - send it
			rc = send(bt_channels_data[tid].client_id,
				tx_channel->message->data,
				tx_channel->message->mssg_len, 0);
			err = errno;
			if (rc < 0)
			{
				// Send error
				fprintf(stderr, "BT connected thread: send() failed");
				// TODO
			}
			delete tx_channel;
		}

		// read data from the client
		bytes_read = read(bt_channels_data[tid].client_id, data, sizeof(data));
		err = errno;
		if (bytes_read > 0)
		{
			// New data received - Create a new channel object (Don't forget to free after processing!!!)
			rx_channel_alsa = new bt_chan_data_t;
			rx_channel_alsa->channel_id = bt_channels_data[tid].channel_id; // = tid!!!???
			rx_channel_alsa->client_id = bt_channels_data[tid].client_id;
			rx_channel_alsa->rem_addr = bt_channels_data[tid].rem_addr;
			rx_channel_alsa->message->mssg_len = bytes_read;
			// Copy received data
			for (i = 0; i < bytes_read; i++)
			{
				rx_channel_alsa->message->data[i] = data[i];
			}
			// Push into ALSA queue
			bt_rx_queue_alsa.enqueue(rx_channel_alsa);
			// New data received - Create a new channel object (Don't forget to free after processing!!!)
			rx_channel_raw = new bt_chan_data_t;
			rx_channel_raw->channel_id = bt_channels_data[tid].channel_id;
			rx_channel_raw->client_id = bt_channels_data[tid].client_id;
			rx_channel_raw->rem_addr = bt_channels_data[tid].rem_addr;
			rx_channel_raw->message->mssg_len = bytes_read;
			// Copy received data
			for (i = 0; i < bytes_read; i++)
			{
				rx_channel_raw->message->data[i] = data[i];
			}
			// Push into ALSA queue
			bt_rx_queue_alsa.enqueue(rx_channel_raw);

			printf("BT connected thread: ch%i:  ", tid);
			for (i = 0; i < bytes_read; i++)
			{
				printf("%x ", rx_channel_alsa->message->data[i]);
			}
			printf("\n");
		}
		else
		{
			// No data received
			if ((err != EAGAIN) && (err != EWOULDBLOCK))
			{
				// Disconnected?
				printf("BT connected thread: Channel %i disconnected.\n", bt_channels_data[tid].channel_id);
				// Terminate the thread
				loop = false;
				connected_thread_is_running[tid] = false;
				close(bt_channels_data[tid].client_id);
				release_channel(bt_channels_data[tid].channel_id);
				fprintf(stderr, "BT connected thread: %i channels used out of %i.\n", num_of_used_channels, _NUMBER_OF_BT_CONNECTIONS);
			}
		}
	}

	return NULL;
}

