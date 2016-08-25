#include "SetupService.h"

// Public
SetupService::SetupService() {
	pinMode(HUB_AP_SETUP_BTN, INPUT);
}

uint8_t SetupService::loop() {
	if (digitalRead(HUB_AP_SETUP_BTN) == LOW) {
		Serial.print("Button off");
		return HUB_AP_STATE_INIT;
	}
	Serial.print("Button on");
	//start WiFi AP
	WiFi.softAP(HUB_AP_WIFI_SSID, HUB_AP_WIFI_PASS);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);	
	//start HTTP service
	std::function<void(void)> hRoot = [&]{this->_handleRoot();};
	_server.on("/", hRoot);
	std::function<void(void)> hSet = [&]{this->_handleSet();};
	_server.on("/set", hSet);
	std::function<void(void)> hRestart = [&]{this->_handleRestart();};
	_server.on("/restart", hRestart);
	std::function<void(void)> hNotFound = [&]{this->_handleNotFound();};
	_server.onNotFound(hNotFound);
	_server.begin();
	while(true) {
		_server.handleClient();
	}
	// TODO if (error) return HUB_AP_STATE_ERROR;
	return HUB_AP_STATE_INIT;
}

void SetupService::_handleRoot() {
	_server.send (200, "text/plain", "Hello world\n\n");	
}

void SetupService::_handleSet() {
	_server.sendHeader("Location", "/");
	_server.send (302, "text/plain", "Config updated...\n\n");	
}

void SetupService::_handleRestart() {
	_server.send (200, "text/plain", "Restarting...\n\n");
	ESP.restart();
}

void SetupService::_handleNotFound() {
	_server.send ( 404, "text/plain", "File Not Found\n\n");
}
