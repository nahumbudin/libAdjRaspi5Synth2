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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include <bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

#include "../utils/safeQueues.h"

/* Defines the max number of simultaneous RFCOMM connections */
#define _NUMBER_OF_BT_CONNECTIONS	4//5
/* Must be smaller than 8 */
#if (_NUMBER_OF_BT_CONNECTIONS > 8)
Error must be smaller than 8
#endif

/* Defines the number of bytes in received buffer */
#define _NUM_OF_RX_BYTES	256

/* Defines the number of bytes in transmit buffer */
#define _NUM_OF_TX_BYTES	_NUM_OF_RX_BYTES

#define _BT_CHAN_0							0
#define _BT_CHAN_1							1
#define _BT_CHAN_2							2
#define _BT_CHAN_3							3
#define _BT_CHAN_4							4

#define _BT_RESULT_OK						0
#define _BT_RESULT_BAD_PARAM				-1
#define _BT_RESULT_SOCKET_OPEN_ERR			-2
#define _BT_RESULT_NO_FREE_CHANNELS			-3
#define _BT_RESULT_ILLEGAL_CHANNEL			-4
#define _BT_RESULT_CHANNEL_NOT_CONNECTED	-5
#define _BT_RESULT_TX_MSSG_TOO_LONG			-6
#define _BT_RESULT_TX_NO_DATA				-7

#define _BT_RESULT_CHANNEL_IS_FREE			true
#define _BT_RESULT_CHANNEL_IS_USED			false

/* A structure used for returning the BT inquaiary results */
typedef struct inq_resQ
{
	inq_resQ() : num_rsp(0), name(NULL), rem_addr(NULL) { }
	/* Number of returned devices */
	int num_rsp;
	/* Comma seperated strings list of devices names */
	char* name;
	/* Comma seperated strings list of devices addresses */
	char* rem_addr;
} inq_resQ_t;

/* A structure used for sending data to connected clients */
typedef struct bt_data
{
	bt_data() : mssg_len(0) {}

	int mssg_len;
	char data[_NUM_OF_TX_BYTES];
} bt_data_t;

/* A structure to hold an allocated channel client ID
   channel ID is used for managiging channels allocations
   remAddr holds the client address
 */
typedef struct bt_chan_data
{
	bt_chan_data() : channel_id(-1), client_id(-1), message(new bt_data()) {}

	int channel_id;
	int client_id;
	sockaddr_rc rem_addr;
	bt_data_t* message;
} bt_chan_data_t;


class Raspi3BluetoothQ
{
public:

	~Raspi3BluetoothQ();

	static Raspi3BluetoothQ* get_instance();

	int scan(inq_resQ_t* res = NULL);

	static void start_bt_main_thread();
	static void stop_bt_main_thread();

	static bt_chan_data_t* get_channel_data(int channel);

	static int send_data_client(int channel, char* data, int len);

	/* A queue used to send the incoming BT data as ALSA midi events */
	static SafeQueue<bt_chan_data_t*> bt_rx_queue_alsa;
	/* A queue used to send the incoming BT data raw data */
	static SafeQueue<bt_chan_data_t*> bt_rx_queue_raw;
	/* A queue to hold Tx data */
	static SafeQueue<bt_chan_data_t*> bt_tx_queue;

private:

	Raspi3BluetoothQ();

	static void start_connected_thread(int tid);
	static void stop_connected_thread(int tid);

	static void* bt_main_thread(void* thread_id);
	static void* connected_thread(void* thread_id);

	static int allocate_channel();
	static void release_channel(int chan);
	static bool channel_is_free(int chan);

	/* USed for scanning BT devices */
	inquiry_info* inq_info = NULL;
	int max_rsp, num_rsp;
	int dev_id, len, flags;
	/* Holds the connection socket ID */
	static int connection_socket;
	int i;
	/* Used for scan */
	static char addr[19];
	static char name[248];
	/* When true indicates that the BT main thread is running */
	static bool bt_main_thread_is_running;
	/* When each true indicates that this BT connected channel thread is running */
	static bool connected_thread_is_running[_NUMBER_OF_BT_CONNECTIONS];
	/* Holds active channels client ID */
	static bt_chan_data_t bt_channels_data[_NUMBER_OF_BT_CONNECTIONS];
	/* Holds the local BT address */
	static struct sockaddr_rc local_address;
	/* Main BT thread handler */
	static pthread_t bt_main_thread_id;
	/* Channels connected trads handles */
	static pthread_t connected_thread_id[_NUMBER_OF_BT_CONNECTIONS];

	static bdaddr_t _BDADDR_ANY;
	/* Used for channels allocations managment.
	   A set bit indicated channel is free.
	   msb is allocated to channel 0; lsb to channel N */
	static uint32_t	channel_allocated_mask;
	/* Holds the actual allocated number of cahnnels */
	static uint8_t num_of_used_channels;

	static Raspi3BluetoothQ* raspi_bluetooth_instance;
};

