#ifndef HUB_AP_SETUP_SERVICE
#define HUB_AP_SETUP_SERVICE

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "HubAP.h"
#include "html.h"

#define HUB_AP_SETUP_BTN 15

String redirectPage(String, String, String); 

// setup service class
class SetupServiceClass {
	public:
		SetupServiceClass();
		
		uint8_t setup();
		uint8_t loop();
	private:
		void _handleRoot();
		void _handlePost();
		void _handleRestart();
		void _handleNotFound();
		void _handleReset();
		void _handleDebug();

		ESP8266WebServer _server;

};

static SetupServiceClass SetupService;

#endif // HUB_AP_SETUP_SERVICE
