#ifndef HUB_AP_SETUP_SERVICE
#define HUB_AP_SETUP_SERVICE

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "HubAP.h"
#include "html.h"

#define HUB_AP_SETUP_BTN 3

// setup service class
class SetupService {
	public:
		SetupService(Config *config);
		
		void loop();
	private:
		void _handleRoot();
		void _handleSet();
		void _handleRestart();
		void _handleNotFound();
		Config * _config;

		ESP8266WebServer _server = ESP8266WebServer(80);

};

#endif // HUB_AP_SETUP_SERVICE
