#ifndef __SDS011__
#define __SDS011__

#include "serprt.hpp"
#include <string>

#define OUT_CHAR 19
#define IN_CHAR  10

class sds011 {
	private:
		unsigned short int	id;
		char				cvCommand[OUT_CHAR];
		char				cvResponse[IN_CHAR];
		unsigned short int	iRetCode;
		string				sRetMsg;
	public:
		sds011(void);
		~sds011(void);
		short int setContinuousMode(void);					// 0:OK, -1:Error
		short int setPollingMode(void);						// 0:OK, -1:Error
		short int getReportingMode(void);					// 1:Polling, 0:Stream, -1:Error
		short int getData(float* pm_2_5, float* pm_10);		// 0:OK, -1:Error
		short int setInScan(void);							// 0:OK, -1:Error
		short int setOutScan(void);							// 0:OK, -1:Error
		short int getScanState(void);						// 1:In scan, 0:Out scan, -1:Error
};

#endif /* __SDS011__ */
