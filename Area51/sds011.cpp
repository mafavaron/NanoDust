//
// Created by patrizia on 10/19/19.
//
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "sds011.hpp"

#define RS232_MINCHAR 0
#define RS232_TIMEOUT 50


sds011::sds011(const std::string sSerialPort) {

    // Assume success (will falsify on failure)
    this->iRetCode    = 0;

    // Basic initialization
    this->id          = 0;
    this->sRetMsg     = "Uninitialized";
    this->sSerialPort = sSerialPort;

    // Check port exists and try opening it
    this->iPort = open(this->sSerialPort.c_str(), O_RDWR|O_NOCTTY);
    if (this->iPort == -1) {
        this->iRetCode = 1;
        this->sRetMsg  = "Unable to open serial port";
        return;
    }
    errno = 0;
    if(!isatty(this->iPort)) {
        this->iRetCode = 2;
        this->sRetMsg  = "The serial port exist, but is not a 'terminal'";
        return;
    }

    // Assign default id (broadcast)
    this -> id = 0xFFFF;

    // Configure port
    speed_t baudRate = B9600;
    struct termios termAttr;
    tcgetattr(this->iPort, &termAttr);
    termAttr.c_cflag |= CREAD | CLOCAL | CS8;       // Enable receiver and local mode, and force 8 data bits
    termAttr.c_cflag |= IGNPAR;                     // Ignore framing and parity errors
    termAttr.c_lflag &= ~(ICANON);                  // Disable canonical mode
    termAttr.c_cflag &= ~(CRTSCTS);                 // Disable hardware flow control
    termAttr.c_iflag &= ~(IXON | IXOFF | IXANY);    // Disable software flow control
    termAttr.c_lflag &= ~(ECHO);                    // Disable terminal echo
    termAttr.c_lflag &= ~(ECHOE);                   // Disable terminal echo
    termAttr.c_lflag &= ~(ISIG);                    // Disable (kill-and-more) signal detection
    termAttr.c_oflag &= ~(OPOST);                   // Raw output
    termAttr.c_cflag &= ~(PARENB | CSTOPB | CSIZE); // Set "8N1" configuration
    options.c_cflag |= CS8;                         // Set "8N1" configuration
    termAttr.c_cc[VMIN]  =  0;                      // Disable inter-character delay detection: use timeout instead
    termAttr.c_cc[VTIME] = 50;                      // Timeout = 50 tenths of a second = 5s
    cfsetispeed(&termAttr, baudRate);
    cfsetospeed(&termAttr, baudRate);
    int retCode = tcsetattr(this->iPort, TCSANOW, &termAttr);
    if (retCode != 0) {
        this->sRetMsg  = "Attempt to write port configuration failed";
        this->iRetCode = 3;
        return;
    }

    fcntl(this->iPort, F_SETFL, 0);

};


sds011::~sds011(void) {

};


short int sds011::setStreamMode(void) {

    // Fill command
    this->cvCommand[ 0] = 0xAA;
    this->cvCommand[ 1] = 0xB4;
    this->cvCommand[ 2] = 0x02;
    this->cvCommand[ 3] = 0x01;
    this->cvCommand[ 4] = 0x00;
    this->cvCommand[ 5] = 0x00;
    this->cvCommand[ 6] = 0x00;
    this->cvCommand[ 7] = 0x00;
    this->cvCommand[ 8] = 0x00;
    this->cvCommand[ 9] = 0x00;
    this->cvCommand[10] = 0x00;
    this->cvCommand[11] = 0x00;
    this->cvCommand[12] = 0x00;
    this->cvCommand[13] = 0x00;
    this->cvCommand[14] = 0x00;
    this->cvCommand[15] = (this->id & 0xFF00) >> 8;
    this->cvCommand[16] = (this->id & 0x00FF);
    this->cvCommand[17] = this->computeChecksum();
    this->cvCommand[18] = 0xAB;

};


short int sds011::setPollingMode(void);							// 0:OK, -1:Error


short int sds011::getReportingMode(void);						// 1:Polling, 0:Stream, -1:Error


short int sds011::getData(float* pm_2_5, float* pm_10);			// 0:OK, -1:Error


short int sds011::setDeviceId(unsigned short int* id);			// 0:OK, -1:Error


short int sds011::getDeviceId(unsigned short int* id);			// 0:OK, -1:Error


short int sds011::setInScan(void);								// 0:OK, -1:Error


short int sds011::setOutScan(void);								// 0:OK, -1:Error


short int sds011::getScanState(void);							// 1:In scan, 0:Out scan, -1:Error


short int sds011::setStreamingPeriod(unsigned short int* dt);	// 0:OK, -1:Error


short int sds011::getStreamingPeriod(unsigned short int* dt);	// 0:OK, -1:Error


short int sds011::getFirmwareDate(unsigned int* date);			// 0:OK, -1:Error


unsigned short int sds011::getCompletionCode(void) {
    return this->iRetCode;
};


string sds011::getCompletionMessage(void) {
    return this->sRetMsg;
};


unsigned char sds011::computeChecksum(void) {

    unsigned short int sum = 0x0000;
    for(int i=2; i<=16; i++) sum += this->cvCommand[i];
    sum &= 0x0FF;
    return(sum);
    
};

