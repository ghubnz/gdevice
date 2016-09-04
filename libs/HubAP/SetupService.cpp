#include "SetupService.h"

// Public
SetupServiceClass::SetupServiceClass() {
	pinMode(HUB_AP_SETUP_BTN, INPUT);
}

uint8_t SetupServiceClass::setup() {
	// read the eeprom
	Config.load();
	char ssid[HUB_AP_WIFI_SSID_SIZE] = {0};
	Config.getSSID(ssid);
	if ((strlen(ssid) != 0) && (digitalRead(HUB_AP_SETUP_BTN) == LOW)) {
		return HUB_AP_STATE_NONE;
	}
	// if the configration is empty or setup button was pressed down
	//start WiFi AP
	char pass[HUB_AP_WIFI_PASS_SIZE] = {0};
	Config.getPass(pass);
	WiFi.softAP(ssid, pass);
	IPAddress apIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.print(apIP);
	//start HTTP service
	_server = ESP8266WebServer(80);
	std::function<void(void)> hRoot = [&]{this->_handleRoot();};
	_server.on("/", hRoot);
	std::function<void(void)> hPost = [&]{this->_handlePost();};
	_server.on("/post", HTTP_POST, hPost);
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

uint8_t SetupServiceClass::loop() {
	_server.handleClient();
	return HUB_AP_STATE_SETUP;
}

void SetupServiceClass::_handleRoot() {
	Serial.println("Root handle");
	String cards;
	for(int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		cards += String(HUB_AP_HTML_CARD);
		cards.replace("$I$", String(i));
		char card[HUB_AP_CARD_SIZE] = {0};
		Config.getCard(i, card);
		cards.replace("$NUM$", String(card));
	}	
	String page = String(HUB_AP_HTML_ROOT);
	char field[HUB_AP_EEPROM_SIZE] = {0};
	Config.getSSID(field);
	Serial.println(field);
	page.replace("$SSID$", String(field));
	Config.getPass(field);
	page.replace("$PASS$", String(field));
	Config.getHubAddr(field);
	page.replace("$ADDR$", String(field));
	Config.getHubKey(field);
	page.replace("$HUBKEY$", String(field));
	Config.getSecKey(field);
	page.replace("$SECKEY$", String(field));
	page.replace("$CARD$", cards);	
	_server.send (200, "text/html", page);
}

void SetupServiceClass::_handlePost() {
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
