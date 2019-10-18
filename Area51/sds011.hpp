#include "serprt.hpp"

#define OUT_CHAR 20
#define IN_CHAR  11

class sds011 {
	private:
		unsigned short int	id;
		char				sCommand[OUT_CHAR];
		char				sResponse[IN_CHAR];
	public:
		sds011(void);
		~sds011(void);
		bool setContinuousMode(void);
		bool setPollingMode(void);
		bool getReportingMode(void);
		bool getData(float* pm_2_5, float* pm_10);
		bool setInScan(void);
		bool setOutScan(void);
		bool getScanState(void);
};
