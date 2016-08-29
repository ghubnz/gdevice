#include "SetupService.h"

// Public
SetupServiceClass::SetupServiceClass() {
	pinMode(HUB_AP_SETUP_BTN, INPUT);
}

uint8_t SetupServiceClass::setup() {
	// read the eeprom
	char *ssid = Config.getSSID();
	if ((sizeof(ssid) > 0) && (ssid[0] != 0) && (digitalRead(HUB_AP_SETUP_BTN) == LOW)) {
		return HUB_AP_STATE_INIT;
	}
	// if the configration is empty or setup button was pressed down
	//start WiFi AP
	char *pass = Config.getPass();
	WiFi.softAP(ssid, pass);
	IPAddress apIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.print(apIP);
	//start HTTP service
	_server = ESP8266WebServer(80);
	std::function<void(void)> hRoot = [&]{this->_handleRoot();};
	_server.on("/", hRoot);
	std::function<void(void)> hSet = [&]{this->_handleSet();};
	_server.on("/post", HTTP_POST, hSet);
	std::function<void(void)> hRestart = [&]{this->_handleRestart();};
	_server.on("/restart", hRestart);
	std::function<void(void)> hReset = [&]{this->_handleReset();};
	_server.on("/restart", hReset);
	std::function<void(void)> hNotFound = [&]{this->_handleNotFound();};
	_server.onNotFound(hNotFound);
	_server.begin();
	return HUB_AP_STATE_SETUP;
}

void SetupServiceClass::loop() {
	_server.handleClient();
}

void SetupServiceClass::_handleRoot() {
	String cards;
	for(int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		String card = String(HUB_AP_HTML_CARD);
		card.replace("$I$", String(i));
		card.replace("$NUM$", String(Config.getCard(i)));
		cards += card;
	}
	String page = String(HUB_AP_HTML_ROOT);
	page.replace("$SSID$", String(Config.getSSID()));
	page.replace("$PASS$", String(Config.getPass()));
	page.replace("$ADDR$", String(Config.getHubAddr()));
	page.replace("$HUBKEY$", String(Config.getHubKey()));
	page.replace("$SECKEY$", String(Config.getSecKey()));
	page.replace("$CARD$", cards);	
	_server.send (200, "text/html", page);	
}

void SetupServiceClass::_handleSet() {
	_server.sendHeader("Location", "/");
	_server.send (302, "text/plain", "Config updated...\n\n");	

	Config.setSSID(_server.arg("ssid").c_str());
	Config.setPass(_server.arg("pass").c_str());
	Config.setHubAddr(_server.arg("addr").c_str());
	Config.setHubKey(_server.arg("hub_key").c_str());
	Config.setSecKey(_server.arg("sec_key").c_str());
	for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		Config.setCard(i, _server.arg(String("card") + String(i)).c_str());
	}
	Config.dump();
}

void SetupServiceClass::_handleReset() {
	_server.send (200, "text/plain", "Cleaning...\n\n");
	Config.clean();
}

void SetupServiceClass::_handleRestart() {
	_server.send (200, "text/plain", "Restarting...\n\n");
	ESP.restart();
}

void SetupServiceClass::_handleNotFound() {
	_server.send ( 404, "text/plain", "File Not Found\n\n");
}
