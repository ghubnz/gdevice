#ifndef HUB_AP_H
#define HUB_AP_H

#include <SPI.h>
#include <ESP8266WiFi.h>

#include "Config.h"
#include "SetupService.h"
#include "RFID.h"

// handler function 
typedef uint8_t (*StateHandler)(uint8_t, void * ...);

// main class
class HubAPClass {
	public:
		HubAPClass();
		
		// call this in loop() and handle the return value
		uint8_t loop(void *...);
		uint8_t setup(uint8_t = HUB_AP_STATE_NONE);
		void flashLED(int);
		void add(uint8_t, StateHandler);
		// int call(char * api, char **pn, char **pv);
		void setState(uint8_t);

		void reset();

		ConfigClass Config;
		RFIDClass RFID = RFIDClass(&Config);
		SetupServiceClass SetupService = SetupServiceClass(&Config);
	private:
		uint8_t _state = HUB_AP_STATE_NONE;
		uint8_t _macAddr[6];

		StateHandler _states[UINT8_MAX] = { NULL };
		bool _connectWiFi();
		bool _waitWiFi(int);
};

static HubAPClass HubAP;
#endif // HUB_AP_H
