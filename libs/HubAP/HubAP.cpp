#include "HubAP.h"
// Public
HubAPClass::HubAPClass() {
	Config.load();
	SetupService.init(&Config);
	Node.init(&Config);
	RFID.init(&Config, &Node);
	// serial speed
	Serial.begin(HUB_AP_SERIAL_SPEED);
	while (!Serial) {;}
	Serial.printf("Serial speed: %d\n", HUB_AP_SERIAL_SPEED);

	// buildin LED
	Serial.print("Buildin LED...");
	pinMode(HUB_AP_LED, OUTPUT);
	digitalWrite(HUB_AP_LED, HIGH);
	Serial.println("...DONE");
}

uint8_t HubAPClass::setup(uint8_t initState /* = HUB_AP_STATE_NONE*/) {
	// setup button
	Serial.print("Setup Service...");
	if (SetupService.setup() == HUB_AP_STATE_SETUP) {
		_state = HUB_AP_STATE_SETUP;
		Serial.println("...DONE");
		return HUB_AP_STATE_SETUP;
	}
	Serial.println("...SKIP");

	// connect WiFi
	Serial.println("WiFi...");
	if (_connectWiFi()) {
		Serial.println("...DONE");
	} else {
		Serial.println("...SKIP");
	}

	Serial.print("Setup MQTT...");
	if (Node.setup() == HUB_AP_STATE_ERROR) {
		Serial.println("...NOT CONNECT");
	}
	Serial.println("...DONE");

	// RFID
	Serial.print("RFID...");
	uint8_t state = (initState == HUB_AP_STATE_NONE) ? RFID.setup() : initState;
	Serial.println("...DONE");
	_state = state;
	return state;
}

uint8_t HubAPClass::loop(void *params ...) {
	if (_state == HUB_AP_STATE_SETUP) {
		_state = SetupService.loop();
		return _state;
	}
	if (10 > (millis() % 60000)) {
		_waitWiFi(4);
	}
	Node.preloop();
	if (_state == HUB_AP_STATE_RFID) {
		_state = RFID.loop();
		return _state;
	}
	if (_state == HUB_AP_STATE_WAIT) {
		_state = Node.loop();
		return _state;
	}
	if (_states[_state] == NULL) {
		_state = _states[HUB_AP_STATE_ERROR](_state, params);
	}
	_state = _states[_state](_state, params);
	return _state;
}

void HubAPClass::add(uint8_t state, StateHandler f) {
	Serial.print("State Handler...");
	_states[state] = f;
	Serial.println("DONE");
}

void HubAPClass::setState(uint8_t state) {
	_state = state;
}

// Connect WiFi
bool HubAPClass::_connectWiFi() {
	// read SSID and Password from the config
	// Connect WiFi
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	char ssid[HUB_AP_WIFI_SSID_SIZE] = {0};
	Config.getSSID(ssid);
	Serial.printf("SSID: %s\n", ssid);
	char pass[HUB_AP_WIFI_PASS_SIZE] = {0};
	Config.getPass(pass);
	Serial.printf("Pass: %s\n", pass);
	int a = WiFi.begin(ssid, pass);
	Serial.printf("Rslt: %d\n", a);
	return _waitWiFi(100);
}

bool HubAPClass::_waitWiFi(int tick) {
	if (WiFi.isConnected()) {
		return true;
	}
	String macStr = WiFi.macAddress();
	memcpy(_macAddr, macStr.c_str(), 6);
	// Wait max to tick/50 seconds
	for (int i = 0; i < tick; i ++) {
		if (WiFi.isConnected()) {
			Serial.printf("Connected: MAC=%s; SSID=%s; IP=", macStr.c_str(), WiFi.SSID().c_str());
			Serial.println(WiFi.localIP());
			return true;
		}
		flashLED(50);
	}
	switch (int status = WiFi.status()) {
		case WL_NO_SSID_AVAIL:
			Serial.printf("No SSID: MAC=%s; SSID=%s\n", macStr.c_str(), WiFi.SSID().c_str());
			break;
		case WL_CONNECTION_LOST:
			Serial.printf("Connection Lost: MAC=%s; SSID=%s\n", macStr.c_str(), WiFi.SSID().c_str());
			break;
		case WL_CONNECT_FAILED:
			Serial.printf("Connect Failed: MAC=%s; SSID=%s\n", macStr.c_str(), WiFi.SSID().c_str());
			break;
		default:
			Serial.printf("Connect Issue: MAC=%s; SSID=%s; Status=%d\n", macStr.c_str(), WiFi.SSID().c_str(), status);
			break;
	}
	return false;
}

void HubAPClass::flashLED(int d) {
	int state = digitalRead(HUB_AP_LED);
	digitalWrite(HUB_AP_LED, !state);
	delay(d/2);
	digitalWrite(HUB_AP_LED, state);
	delay(d/2);
}

void HubAPClass::reset() {
	Config.clean();
}
