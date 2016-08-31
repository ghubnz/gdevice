#ifndef HUB_AP_H
#define HUB_AP_H

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

// not working state
#define HUB_AP_STATE_NONE	0x00
// error state
#define HUB_AP_STATE_ERROR	0x01
// enter setup service
#define HUB_AP_STATE_SETUP	0x10
// RFID
#define HUB_AP_STATE_RFID	0x20
#define HUB_AP_STATE_ACCEPT	0x30
#define HUB_AP_STATE_DENY	0x31

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
	private:
		uint8_t _state = HUB_AP_STATE_NONE;
		uint8_t _macAddr[6];

		StateHandler _states[UINT8_MAX] = { NULL };
		void _connectWiFi();
};

static HubAPClass HubAP;
#endif // HUB_AP_H
