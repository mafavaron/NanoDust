#include "serprt.hpp"

#define OUT_CHAR 20
#define IN_CHAR  11

// Reporting modes
#define REPMODE_CONTINUOUS 0x00
#define REPMODE_POLLING    0x01

class sds011 {
	private:
		unsigned short int	id;
		char				sCommand[OUT_CHAR];
		char				sResponse[IN_CHAR];
	public:
		sds011(void);
		~sds011(void);
		void setReportingMode(const unsigned char cReportingMode);
		unsigned char getReportingMode(void);
};
