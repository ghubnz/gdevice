#ifndef HUB_AP_SETUP_SERVICE
#define HUB_AP_SETUP_SERVICE

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Config.h"
#include "html.h"

// #define HUB_AP_SETUP_BTN 15 // D10
#define HUB_AP_SETUP_BTN 5 // D3



String redirectPage(String, String, String); 

// setup service class
class SetupServiceClass {
	public:
		SetupServiceClass(ConfigClass *);
		
		uint8_t setup();
		uint8_t loop();
	private:
		void _handleRoot();
		void _handlePost();
		void _handleRestart();
		void _handleNotFound();
		void _handleReset();
		void _handleDebug();

		ESP8266WebServer _server = ESP8266WebServer();
		ConfigClass *_config;

};

#endif // HUB_AP_SETUP_SERVICE
