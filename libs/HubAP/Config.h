#ifndef HUB_AP_CONFIG
#define HUB_AP_CONFIG

#define HUB_AP_WIFI_SSID "GHub"
#define HUB_AP_WIFI_PASS "gribblehirst"
#define HUB_AP_SERIAL_SPEED 115200
#define HUB_AP_EERPOM_SIZE 256
#define HUB_AP_LED LED_BUILTIN

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "HubAP.h"

// Config class
class Config {
	public:
		Config();
		
		uint8_t load();
		uint8_t dump();
	private:
};

#endif // HUB_AP_CONFIG
