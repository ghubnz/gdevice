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

void HubAPClass::setup() {
	// Config
	Serial.print("Loading Config...");	
	Config.load();
	Serial.println("...DONE");	

	// setup button
	Serial.print("Setting Service...");	
	if (_state = SetupService.setup() == HUB_AP_STATE_SETUP) {
		Serial.println("...DONE");	
		return;
	}
	Serial.println("...SKIP");		

	// connect WiFi
	Serial.print("Connecting WiFi...");		
	_connectWiFi();
	Serial.println("...DONE");		
}

uint8_t HubAPClass::loop(void *params ...) {
	if (_state == HUB_AP_STATE_SETUP) {
		SetupService.loop();
		return HUB_AP_STATE_SETUP;		
	}
	if (_states[_state] == NULL) {
		_state = _states[HUB_AP_STATE_ERROR](HUB_AP_STATE_ERROR, params);
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
	WiFi.begin(Config.getSSID(), Config.getPass());
	// Wait max to 30 seconds
	// TODO Set the max waiting times
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.printf("Waiting WiFi Connection: SSID=%s\n", HUB_AP_WIFI_SSID);		
		flashLED(40);
	}
	String macStr = WiFi.macAddress();
	memcpy(_macAddr, macStr.c_str(), 6);
	Serial.printf("WiFi Connected: MAC=%s\n", macStr.c_str());
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
