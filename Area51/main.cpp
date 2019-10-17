/*
 * main.cpp - Part of the SDS011 PM-2.5 and PM-10 dedicated logger
 *
 *  Created on: 9-Jan-2009
 *      Author: root
 */


#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "serprt.hpp"

int fd;

int initport(int fd) {
	struct termios options;
	// Get the current options for the port...
	tcgetattr(fd, &options);
	// Set the baud rates to 115200...
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	// Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);
	
	//disable hardware flow control
	options.c_cflag &= ~CRTSCTS;

	//disable software flow control
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	//raw input
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		
	//raw output
	options.c_oflag &= ~OPOST;
	
	//No parity - 8N1
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Set the new options for the port...
	tcsetattr(fd, TCSANOW, &options);
	
   
	return 1;
}

int main(int argc, char **argv) {

	fd = open("/dev/ttyS0", O_RDWR); // | O_NOCTTY); // | O_NDELAY);

	if (fd == -1) {
		perror("open_port: Unable to open /dev/ttyS0 - ");
		return 1;
	} else {
		fcntl(fd, F_SETFL, 0);
		//fcntl(fd, F_SETFL, FNDELAY);
	}
	
	//printf("baud=%d\n", getbaud(fd));
	initport(fd);
	printf("baud=%d\n", getbaud(fd));

	unsigned char sCmd[13]; 
	sCmd[0] = 0x00
        sCmd[1] = 0x01
        //etc

	if (!writeport(fd, sCmd, 13)) {
		printf("write failed\n");
		close(fd);
		return 1;
	}

	//printf("write cmd:%d\n", sCmd[1]);
	
	usleep(50000);
	unsigned char sResult[13];

	fcntl(fd, F_SETFL, 0);
    //fcntl(fd, F_SETFL, FNDELAY); // don't block serial read

	if (!readport(fd,sResult)) {
		printf("read failed\n");
		close(fd);
		return 1;
	}
	
	close(fd);
	return 0;
}
