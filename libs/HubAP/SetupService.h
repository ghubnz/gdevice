#ifndef HUB_AP_SETUP_SERVICE
#define HUB_AP_SETUP_SERVICE

#include <SPI.h>
#include "HubAP.h"

#define HUB_AP_SETUP_BTN 3

// setup service class
class SetupService {
	public:
		SetupService();
		
		// call this in loop() and handle the return value
		uint8_t loop();
	private:

};

#endif // HUB_AP_SETUP_SERVICE
