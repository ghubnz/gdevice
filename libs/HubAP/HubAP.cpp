#include "HubAP.h"
// Public
HubAPClass::HubAPClass() {
	// serial speed
	Serial.begin(HUB_AP_SERIAL_SPEED);
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
	Serial.print("WiFi connection...");		
	_connectWiFi();
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
		return SetupService.loop();
	}
	if (_state == HUB_AP_STATE_RFID) {
		_state = RFID.loop();
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
// TODO if it reached the max waiting times, the AP will be in offline mode
void HubAPClass::_connectWiFi() {
	// read SSID and Password from the config	
	// Connect WiFi
	char ssid[HUB_AP_WIFI_SSID_SIZE] = {0};
	Config.getSSID(ssid);
	char pass[HUB_AP_WIFI_PASS_SIZE] = {0};
	Config.getPass(pass);
	WiFi.begin(ssid, pass);
	// Wait max to 30 seconds
	// TODO Set the max waiting times
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.printf("Waiting: SSID=%s", HUB_AP_WIFI_SSID);		
		flashLED(40);
	}
	String macStr = WiFi.macAddress();
	memcpy(_macAddr, macStr.c_str(), 6);
	Serial.printf("Connected: MAC=%s", macStr.c_str());
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
