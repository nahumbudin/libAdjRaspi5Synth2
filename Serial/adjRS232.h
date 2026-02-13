/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005 - 2020 Teunis van Beelen
*
* Email: teuniz@protonmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/

/* Last revision: August 6, 2020 */

/* For more info and how to use this library, visit: http://www.teuniz.net/RS-232/ */

/* adjRS232 - cpp version Nahum Budin 26 - 11 - 2020 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#if defined(__linux__) || defined(__FreeBSD__)

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#else

#include <windows.h>

#endif

class ADJRS232
{
  public:
	static ADJRS232 *getAdjRS232instance();

	int open_comport(int, int, const char *, int);
	int poll_comport(int, unsigned char *, int);
	int send_byte(int, unsigned char);
	int send_buf(int, unsigned char *, int);
	void close_comport(int);
	void cputs(int, const char *);
	int is_DCD_enabled(int);
	int is_RING_enabled(int);
	int is_CTS_enabled(int);
	int is_DSR_enabled(int);
	void enable_DTR(int);
	void disable_DTR(int);
	void enable_RTS(int);
	void disable_RTS(int);
	void flush_RX(int);
	void flush_TX(int);
	void flush_RXTX(int);
	int get_port_nr(const char *);
	
  private:
	ADJRS232();

	static ADJRS232 *adj_RS232_instance;

	int error;
};

#ifdef __cplusplus
} /* extern "C" */
#endif


