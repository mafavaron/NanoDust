#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "serprt.hpp"

int writeport(int fd, unsigned char *chars, int numBytes) {
	
	//show array contents
	//printf("write array contents:\n");
	int counter = 0;
	printf("write array = ");
	while(counter < numBytes) {
		printf("%d,", chars[counter]);
		counter++;
	}
	printf("\n");
	
	int n = write(fd, chars, numBytes); //strlen(chars));
	if (n < 0) {
		fputs("write failed!\n", stderr);
		return 0;
	}
	//all good
	return 1;
}

int readport(int fd, unsigned char *result) {
	
	int iIn = read(fd, result, 254); //13); //254);
	
	//show result array
	int counter = 0;
	printf("result[] = ");
	while(counter < iIn) {
		printf("%d,",result[counter]);
		counter++;
	}
	printf("\n");
	
	
	//result[iIn-1] = 0x00;
	if (iIn < 0) {
		if (errno == EAGAIN) {
			printf("SERIAL EAGAIN ERROR\n");
			return 0;
		} else {
			printf("SERIAL read error %d %s\n", errno, strerror(errno));
			return 0;
		}
	}                    
	return 1;
}

int getbaud(int fd) {
	struct termios termAttr;
	int inputSpeed = -1;
	speed_t baudRate;
	tcgetattr(fd, &termAttr);
	/* Get the input speed.                              */
	baudRate = cfgetispeed(&termAttr);
	switch (baudRate) {
		case B0:      inputSpeed = 0; break;
		case B50:     inputSpeed = 50; break;
		case B110:    inputSpeed = 110; break;
		case B134:    inputSpeed = 134; break;
		case B150:    inputSpeed = 150; break;
		case B200:    inputSpeed = 200; break;
		case B300:    inputSpeed = 300; break;
		case B600:    inputSpeed = 600; break;
		case B1200:   inputSpeed = 1200; break;
		case B1800:   inputSpeed = 1800; break;
		case B2400:   inputSpeed = 2400; break;
		case B4800:   inputSpeed = 4800; break;
		case B9600:   inputSpeed = 9600; break;
		case B19200:  inputSpeed = 19200; break;
		case B38400:  inputSpeed = 38400; break;
		case B115200:  inputSpeed = 115200; break;
	}
	return inputSpeed;
}
