/**
 * @file		i2cInterface.cpp
 *	@author		Nahum Budin
 *	@date		2-Dec-2025
 *	@version	1.
 *					1. First version
 *
 *	@brief		Handle the I2C interface streams
 *
 *	History:\n
 */

#include <iomanip>
#include <string.h>
#include "i2cInterface.h"
#include "../misc/priorities.h"
#include "../LibAPI/defines.h"

SafeQueue<i2cRxData_t *> I2Cinterface::alsa_control_box_i2c_interface_rx_queue;

SafeQueue<i2cWriteRegisterCommandData_t *> I2Cinterface::write_reg_8_commands_queue;

I2Cinterface::I2Cinterface(int i2cBusNum, uint8_t slave_address, uint8_t interface_num)
	: i2c_bus_number(i2cBusNum),
	  i2c_file_descriptor(-1),
	  interface_num(interface_num),
	  slave_address(slave_address),
	  i2c_polling_thread_is_running(false),
	  data_1_reg_address(0),
	  data_1_len_reg_address(0),
	  data_2_reg_address(0),
	  data_2_len_reg_address(0)
{
	open_interface(i2cBusNum);
	set_slave_address(slave_address);
}

I2Cinterface::~I2Cinterface()
{
	close_interface();
}

/**
 * @brief Opens the I2C bus and sets the slave address.
 * @return 0 on success, -1 on failure.
 */
int I2Cinterface::open_interface(int i2cBusNum)
{
	std::string device_filename = "/dev/i2c-" + std::to_string(i2cBusNum);

	// Close existing file descriptor if open
	if (i2c_file_descriptor != -1)
	{
		close(i2c_file_descriptor);
		i2c_file_descriptor = -1;
	}

	// Open the I2C bus file
	i2c_file_descriptor = open(device_filename.c_str(), O_RDWR);
	if (i2c_file_descriptor < 0)
	{
		std::cerr << "Failed to open I2C bus " << i2cBusNum << std::endl;
		return -1;
	}
	return 0;
}

int I2Cinterface::set_slave_address(uint8_t address)
{
	if (ioctl(i2c_file_descriptor, I2C_SLAVE, address) < 0)
	{
		std::cerr << "Failed to set I2C slave address: " << strerror(errno) << std::endl;
		close(i2c_file_descriptor);
		i2c_file_descriptor = -1;
		return -1;
	}
	return 0;
}

int I2Cinterface::set_blocks_registers_addresses(
										uint8_t data_1_reg_addr, 
										uint8_t data_1_len_reg_addr,
										uint8_t data_2_reg_addr, 
										uint8_t data_2_len_reg_addr)
{
	data_1_reg_address = data_1_reg_addr;
	data_1_len_reg_address = data_1_len_reg_addr;
	data_2_reg_address = data_2_reg_addr;
	data_2_len_reg_address = data_2_len_reg_addr;
	return 0;
}

int I2Cinterface::close_interface()
{
	if (i2c_file_descriptor >= 0)
	{
		close(i2c_file_descriptor);
		i2c_file_descriptor = -1;
	}
	return 0;
}

int I2Cinterface::write_reg8(int file_desc, uint8_t reg_num, uint8_t reg_val, uint8_t *wr_buff)
{
	wr_buff[0] = reg_num;
	wr_buff[1] = reg_val;

	if (write(file_desc, wr_buff, 2) != 2)
	{
		std::cerr << "Write Reg: Failed to write to I2C device: " << strerror(errno) << std::endl;
		// Try to recover by reopening the interface
		if (open_interface(interface_num) == 0)
		{
			if (set_blocks_registers_addresses(data_1_reg_address, data_1_len_reg_address,
											   data_2_reg_address, data_2_len_reg_address) == 0)
			{
				// Successfully recovered
				
				fprintf(stderr, "? Write Reg 8 Interface reopened\n");
				
				return 0;
			}
		}
		// Cannot recover
		close(file_desc);
		return -1;
	}

	return 0;
}

int I2Cinterface::read_reg8(uint8_t *reg_val, int file_desc, uint8_t reg_num, uint8_t *rd_buff, uint8_t *wr_buff)
{
	wr_buff[0] = reg_num;
	if (write(file_desc, wr_buff, 1) != 1)
	{
		std::cerr << "Failed to write to I2C device: " << strerror(errno) << std::endl;
		// Try to recover by reopening the interface
		if (open_interface(interface_num) == 0)
		{
			if (set_blocks_registers_addresses(data_1_reg_address, data_1_len_reg_address,
											   data_2_reg_address, data_2_len_reg_address) == 0)
			{
				// Successfully recovered
				fprintf(stderr, "? Read Reg 8Interface reopened\n");
				
				return 0;
			} 
		}

		// Cannot recover
		close(file_desc);
		return -1;
	}

	if (read(file_desc, reg_val, 1) != 1)
	{
		std::cerr << "Failed to read from I2C device: " << strerror(errno) << std::endl;
		// Try to recover by reopening the interface
		if (open_interface(interface_num) == 0)
		{
			if (set_blocks_registers_addresses(data_1_reg_address, data_1_len_reg_address,
											   data_2_reg_address, data_2_len_reg_address) == 0)
			{
				// Successfully recovered
				
				fprintf(stderr, "? Read Reg 8 Interface reopened\n");
				
				return 0;
			}
		}

		// Cannot recover
		close(file_desc);
		return -1;
	}

	return 0;
}

int I2Cinterface::read_block(int file_desc, uint8_t reg_num, uint8_t *rd_buff, uint8_t *wr_buff, int size)
{
	if (size > 0)
	{
		wr_buff[0] = reg_num;
		if (write(file_desc, wr_buff, 1) != 1)
		{
			std::cerr << "Failed to write to I2C device: " << strerror(errno) << std::endl;
			// Try to recover by reopening the interface
			if (open_interface(interface_num) == 0)
			{
				if (set_blocks_registers_addresses(data_1_reg_address, data_1_len_reg_address,
												   data_2_reg_address, data_2_len_reg_address) == 0)
				{
					// Successfully recovered
					fprintf(stderr, "? Read Block Interface reopened\n");
					
					return 0;
				}
			}

			// Cannot recover
			close(file_desc);
			return -1;
		}

		// Reading the message
		if (read(file_desc, &rd_buff[0], size) != size)
		{
			std::cerr << "Failed to read from I2C device: " << strerror(errno) << std::endl;
			// Try to recover by reopening the interface
			if (open_interface(interface_num) == 0)
			{
				if (set_blocks_registers_addresses(data_1_reg_address, data_1_len_reg_address,
												   data_2_reg_address, data_2_len_reg_address) == 0)
				{
					// Successfully recovered
					fprintf(stderr, "? Read Block Interface reopened\n");
					
					return 0;
				}
			}

			// Cannot recover
			close(file_desc);
			return 1;
		}
	}

	return 0;
}

int I2Cinterface::start_i2c_polling_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the I2C polling thread is not allready running
	if (i2c_polling_thread_is_running)
	{
		std::cerr << "I2C polling thread is allready running" << std::endl;
		return -1;
	}

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_I2C_INTERFACE;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	//	err = errno; // for debug
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		std::cerr << "Unsuccessful in setting I2C polling thread realtime prio" << std::endl;
	}
	// Create and start the thread
	ret = pthread_create(&i2c_polling_thread_id, &tattr, &I2Cinterface::i2c_polling_thread, this);
	char thname[] = {"i2c_polling_thread"};
	pthread_setname_np(i2c_polling_thread_id, &thname[0]);

	// Set the control flag
	i2c_polling_thread_is_running = true;
	
	return 0;
}

int I2Cinterface::stop_i2c_polling_thread()
{
	if (!i2c_polling_thread_is_running)
	{
		std::cerr << "I2C polling thread is not running" << std::endl;
		return -1;
	}
	// Stop the thread
	i2c_polling_thread_is_running = false;
	pthread_join(i2c_polling_thread_id, NULL);
	return 0;
}

void *I2Cinterface::i2c_polling_thread(void *arg)	
{
	uint8_t mssg_size = 0;
	uint8_t read_buffer[512];
	uint8_t write_buffer[512];
	int res;
	// Will count down the number of iterations to toggle the LED
	int green_led_is_on = 0, yellow_led_is_on = 0;
	// Used for the non-blocking read from the write commands queue.
	bool timeout = false, blocking = true, non_blocking = false;

	I2Cinterface *i2c_interface = static_cast<I2Cinterface *>(arg);

	fprintf(stderr, "I2C polling thread started\n");
	
	// Object pool to avoid heap allocation in hot path
	static const int POOL_SIZE = 16;
	i2cRxData_t rx_data_pool[POOL_SIZE];
	int pool_index = 0;
	
	// Error tracking for retry logic
	int consecutive_errors = 0;
	const int MAX_CONSECUTIVE_ERRORS = 3;
	
	// Rate limiting for LED updates
	int led_update_counter = 0;
	const int LED_UPDATE_INTERVAL = 10; // Update LED every 10th message
	
	while (i2c_interface->i2c_polling_thread_is_running)
	{
		bool had_error = false;
	
		// ==================== DATA BLOCK 1 ====================
		if ((i2c_interface->data_1_len_reg_address != 0) &&
			(i2c_interface->data_1_reg_address != 0))
		{
			// Get the size of block 1 data message
			res = i2c_interface->read_reg8(&mssg_size, 
				i2c_interface->i2c_file_descriptor, 
				i2c_interface->data_1_len_reg_address, 
				read_buffer, 
				write_buffer);
			if (res < 0)
			{
				had_error = true;
			}
			else if (mssg_size > 0)
			{
				// Read the block data message
				res = i2c_interface->read_block(i2c_interface->i2c_file_descriptor, 
					i2c_interface->data_1_reg_address,
					read_buffer, 
					write_buffer, 
					mssg_size);

				if (res >= 0)
				{
					// Use object pool instead of new
					i2cRxData_t *rx_data = &rx_data_pool[pool_index];
					pool_index = (pool_index + 1) % POOL_SIZE;
				
					rx_data->interface_num = i2c_interface->interface_num;
					rx_data->mssg_len = mssg_size;
					std::copy(read_buffer, read_buffer + mssg_size, rx_data->message);
					alsa_control_box_i2c_interface_rx_queue.enqueue(rx_data);

					// Rate-limited LED updates
					led_update_counter++;
					if (led_update_counter % LED_UPDATE_INTERVAL == 0)
					{
						if (i2c_interface->interface_num == _I2C_INTERFACE_NUMBER_CONTROL_BOARD)
						{
							yellow_led_is_on = 5;
							res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor, 
								_REG_YELLOW_LED_ON, 
								0xff, 
								write_buffer);
						}
						else
						{
							i2cWriteRegisterCommandData_t *command = new i2cWriteRegisterCommandData_t;
							command->interface_num = i2c_interface->interface_num;
							command->reg_address = _REG_YELLOW_LED_ON;
							command->reg_value = 0x5;
							I2Cinterface::write_reg_8_commands_queue.enqueue(command);
						}
					}
				
					// Reset error counter on success
					consecutive_errors = 0;
				}
				else
				{
					had_error = true;
				}
			}
		}

		// ==================== DATA BLOCK 2 ====================
		if ((i2c_interface->data_2_len_reg_address != 0) &&
			(i2c_interface->data_2_reg_address != 0))
		{
			// Get the size of block 2 data message
			res = i2c_interface->read_reg8(&mssg_size, 
				i2c_interface->i2c_file_descriptor, 
				i2c_interface->data_2_len_reg_address,
				read_buffer, 
				write_buffer);
			if (res < 0)
			{
				had_error = true;
			}
			else if (mssg_size > 0)
			{
				// Read the block data message
				res = i2c_interface->read_block(i2c_interface->i2c_file_descriptor, 
					i2c_interface->data_2_reg_address,
					read_buffer, 
					write_buffer, 
					mssg_size);

				if (res >= 0)
				{
					// Use object pool instead of new
					i2cRxData_t *rx_data = &rx_data_pool[pool_index];
					pool_index = (pool_index + 1) % POOL_SIZE;
				
					rx_data->interface_num = i2c_interface->interface_num;
					rx_data->mssg_len = mssg_size;
					std::copy(read_buffer, read_buffer + mssg_size, rx_data->message);
					alsa_control_box_i2c_interface_rx_queue.enqueue(rx_data);

					//fprintf(stderr, "I2C polling thread received new block len %i\n", mssg_size);

					// Rate-limited LED updates
					led_update_counter++;
					if (led_update_counter % LED_UPDATE_INTERVAL == 0)
					{
						if (i2c_interface->interface_num == _I2C_INTERFACE_NUMBER_CONTROL_BOARD)
						{
							yellow_led_is_on = 5;
							res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor, 
								_REG_YELLOW_LED_ON, 
								0xff, 
								write_buffer);
						}
						else
						{
							i2cWriteRegisterCommandData_t *command = new i2cWriteRegisterCommandData_t;
							command->interface_num = i2c_interface->interface_num;
							command->reg_address = _REG_YELLOW_LED_ON;
							command->reg_value = 0x5;
							I2Cinterface::write_reg_8_commands_queue.enqueue(command);
						}
					}
				
					// Reset error counter on success
					consecutive_errors = 0;
				}
				else
				{
					had_error = true;
				}
			}
		}

		// Error handling with retry logic
		if (had_error)
		{
			consecutive_errors++;
		
			if (consecutive_errors >= MAX_CONSECUTIVE_ERRORS)
			{
				// Only reopen interface after multiple consecutive failures
				std::cerr << "I2C: Too many consecutive errors (" << consecutive_errors 
						  << "), attempting to reopen interface..." << std::endl;
			
				if (i2c_interface->open_interface(i2c_interface->i2c_bus_number) == 0)
				{
					if (i2c_interface->set_slave_address(i2c_interface->slave_address) == 0)
					{
						std::cerr << "I2C: Interface successfully reopened" << std::endl;
						consecutive_errors = 0;
					}
				}
				else
				{
					std::cerr << "I2C: Failed to reopen interface" << std::endl;
					// Don't reset counter - will try again next time
				}
			}
		}

		// ==================== WRITE COMMANDS QUEUE (CONTROL BOARD ONLY) ====================
		if (i2c_interface->interface_num == _I2C_INTERFACE_NUMBER_CONTROL_BOARD)
		{
			// Process write commands from queue - non-blocking
			i2cWriteRegisterCommandData_t *command = 
				I2Cinterface::write_reg_8_commands_queue.dequeue(0, &timeout, &non_blocking);
		
			// Process all queued commands
			while (command != NULL)
			{
				// Only process commands for other interfaces (not this one)
				if (command->interface_num != i2c_interface->interface_num)
				{
					if (command->reg_address == _REG_YELLOW_LED_ON)
					{
						yellow_led_is_on = command->reg_value;
						res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor,
							command->reg_address,
							command->reg_value,
							write_buffer);
					}
					else if (command->reg_address == _REG_GREEN_LED_ON)
					{
						green_led_is_on = command->reg_value;
						res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor,
							command->reg_address,
							command->reg_value,
							write_buffer);
					}
					else if (command->reg_address == _REG_YELLOW_LED_OFF)
					{
						res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor,
							command->reg_address,
							command->reg_value,
							write_buffer);
						yellow_led_is_on = 0;
					}
					else if (command->reg_address == _REG_GREEN_LED_OFF)
					{
						res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor,
							command->reg_address,
							command->reg_value,
							write_buffer);
						green_led_is_on = 0;
					}
				}
			
				delete command;
				command = I2Cinterface::write_reg_8_commands_queue.dequeue(0, &timeout, &non_blocking);
			}

			// LED countdown timers
			if (yellow_led_is_on > 0)
			{
				yellow_led_is_on--;
				if (yellow_led_is_on <= 0)
				{
					yellow_led_is_on = 0;
					res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor, 
						_REG_YELLOW_LED_OFF, 
						0xff, 
						write_buffer);
				}
			}
		
			if (green_led_is_on > 0)
			{
				green_led_is_on--;
				if (green_led_is_on <= 0)
				{
					green_led_is_on = 0;
					res = i2c_interface->write_reg8(i2c_interface->i2c_file_descriptor, 
						_REG_GREEN_LED_OFF, 
						0xff, 
						write_buffer);
				}
			}
		}

		// 
		usleep(1000); 
	}
	
	fprintf(stderr, "I2C polling thread stopped\n");
	return NULL;
}
