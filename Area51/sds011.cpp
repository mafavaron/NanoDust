//
// Created by patrizia on 10/19/19.
//
#include "sds011.hpp"


sds011::sds011(void) {

    // Basic initialization
    this->id          = 0;
    this->iRetCode    = 1;
    this->sRetMsg     = "Uninitialized";
    this->sSerialPort = "/dev/ttyUSB0";

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

