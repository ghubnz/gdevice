#include "SetupService.h"

// Public
SetupService::SetupService(Config *config) {
	_config = config;
	pinMode(HUB_AP_SETUP_BTN, INPUT);
}

void SetupService::loop() {
	// read the eeprom
	char *ssid = _config->getSSID();
	if ((sizeof(ssid) > 0) && (ssid[0] != 0) && (digitalRead(HUB_AP_SETUP_BTN) == LOW)) {
		Serial.print("Initialised & Button off");
		return;
	}
	// if the configration is empty or setup button was pressed down
	//start WiFi AP
	char *pass = _config->getPass();
	WiFi.softAP(ssid, pass);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);	
	//start HTTP service
	std::function<void(void)> hRoot = [&]{this->_handleRoot();};
	_server.on("/", hRoot);
	std::function<void(void)> hSet = [&]{this->_handleSet();};
	_server.on("/post", hSet);
	std::function<void(void)> hRestart = [&]{this->_handleRestart();};
	_server.on("/restart", hRestart);
	std::function<void(void)> hNotFound = [&]{this->_handleNotFound();};
	_server.onNotFound(hNotFound);
	_server.begin();
	while(true) {
		_server.handleClient();
	}
}

void SetupService::_handleRoot() {
	String page = String(HUB_AP_HTML_ROOT);
	page.replace("$SSID$", String(_config->getSSID()));
	page.replace("$PASS$", String(_config->getPass()));
	page.replace("$ADDR$", String(_config->getHubAddr()));
	page.replace("$HUBKEY$", String(_config->getHubKey()));
	page.replace("$SECKEY$", String(_config->getSecKey()));	
	_server.send (200, "text/html", page);	
}

void SetupService::_handleSet() {
	_config->dump();
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
