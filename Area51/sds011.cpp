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
    }

    // Configure port
    speed_t baudRate;
    struct termios termAttr;
    tcgetattr(this->iPort, &termAttr);
    termAttr.c_cflag |= CREAD | CLOCAL | CS8;   // Enable receiver and local mode, and force 8 data bits
    termAttr.c_cflag |= IGNPAR;                 // Ignore framing and parity errors
    termAttr.c_lflag &= ~(ICANON);              // Disable canonical mode

    fcntl(this->iPort, F_SETFL, 0);

    // Assign command fixed parts
    this->cvCommand[ 0] = 0xAA;
    this->cvCommand[18] = 0xAB;

};


sds011::~sds011(void) {

};


short int sds011::setStreamMode(void);							// 0:OK, -1:Error


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


unsigned char sds011::computeChecksum(void);

