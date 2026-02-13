/**
* @file		i2cInterface.h
*	@author		Nahum Budin
*	@date		2-Dec-2025
*	@version	1.
*					1. First version
*
*	@brief		Handle the I2C interface streams
*
*	History:\n
*/

#include <stdint.h>
#include <iostream>
#include <fcntl.h> // For open()
#include <unistd.h> // For close()
#include <sys/ioctl.h> // For ioctl()
#include <linux/i2c-dev.h> // For I2C_SLAVE

#include "../utils/safeQueues.h"

typedef struct i2cRxData
{
	i2cRxData()
		: interface_num(16), 
		  mssg_len(0) {}

	int interface_num;
	uint8_t message[256];
	int mssg_len;
} i2cRxData_t;

typedef struct i2cWriteRegisterCommandData
{
	i2cWriteRegisterCommandData()
		: interface_num(0), 
		  reg_address(0), 
		  reg_value(0) {}
	int interface_num;
	uint8_t reg_address;
	uint8_t reg_value;
} i2cWriteRegisterCommandData_t;

class I2Cinterface
{
public:
  I2Cinterface(int i2cBusNum, uint8_t slave_address, uint8_t interface_num);
	~I2Cinterface();
	int close_interface();
	int set_blocks_registers_addresses(uint8_t data_1_reg_addr = 0,
									   uint8_t data_1_len_reg_addr = 0,
									   uint8_t data_2_reg_addr = 0,
									   uint8_t data_2_len_reg_addr = 0);

	int write_reg8(int file_desc, uint8_t reg_num, uint8_t reg_val, uint8_t *wr_buff);
	int read_reg8(uint8_t *reg_val, int file_desc, uint8_t reg_num, uint8_t *rd_buff, uint8_t *wr_buff);
	int read_block(int file_desc, uint8_t reg_num, uint8_t *rd_buff, uint8_t *wr_buff, int size);
	int start_i2c_polling_thread();
	int stop_i2c_polling_thread();

	/* A queue used to send into the system the incoming I2C data as ALSA midi events */
	static SafeQueue<i2cRxData_t *> alsa_control_box_i2c_interface_rx_queue;
	
	/* A queue used to send i2c Write Register commands to the I2C polling thread */
	static SafeQueue<i2cWriteRegisterCommandData_t *> write_reg_8_commands_queue;

  private:
	int i2c_bus_number;
	int i2c_file_descriptor;
	uint8_t slave_address;
	uint8_t interface_num;

	int open_interface(int i2cBusNum);
	int set_slave_address(uint8_t address);

	bool i2c_polling_thread_is_running;
	static void *i2c_polling_thread(void *arg);
	pthread_t i2c_polling_thread_id;

	uint8_t data_1_reg_address;
	uint8_t data_1_len_reg_address;
	uint8_t data_2_reg_address;
	uint8_t data_2_len_reg_address;
};
