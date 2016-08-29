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
	_server.on("/reset", hReset);
	std::function<void(void)> hDebug = [&]{this->_handleDebug();};
	_server.on("/debug", hDebug);
	std::function<void(void)> hNotFound = [&]{this->_handleNotFound();};
	_server.onNotFound(hNotFound);
	_server.begin();
	return HUB_AP_STATE_SETUP;
}

void SetupServiceClass::loop() {
	_server.handleClient();
}

void SetupServiceClass::_handleRoot() {
	Serial.println("Root handle");
	Config.load();
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
	Serial.println("Set handle");
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
	Serial.println("Reset handle");
	_server.send (200, "text/html",
			redirectPage("3", "/", "Resetting..."));
	Config.clean();
}

void SetupServiceClass::_handleRestart() {
	Serial.println("Restart handle");
	_server.send (200, "text/html",
			redirectPage("30", "javascript:window.close();", "Restarting..."));
	ESP.restart();
}

void SetupServiceClass::_handleDebug() {
	Serial.println("Debug handle");
	_server.send (200, "text/plain", String(Config.debug()));
}

void SetupServiceClass::_handleNotFound() {
	Serial.println("Not Found handle");
	_server.send ( 404, "text/html",
			redirectPage("5", "/", "File Not Found"));
}

String redirectPage(String delay, String url, String text) {
	String page = String(HUB_AP_HTML_REDIRECT);
	page.replace("$DELAY$", delay);
	page.replace("$URL$", url);
	page.replace("$TEXT$", text);
	return page;
}
