#include "HubAP.h"

// Public
HubAP::HubAP() {}

void HubAP::setup() {
	// serial speed
	Serial.begin(HUB_AP_SERIAL_SPEED);
	// buildin LED
	pinMode(HUB_AP_LED, OUTPUT);
	digitalWrite(HUB_AP_LED, HIGH);
	// setup button
	SetupService setupService;
	_state = setupService.loop();
	// connect WiFi
	_connectWiFi();
}

uint8_t HubAP::loop(void *params ...) {
	if (_states[_state] == NULL) {
		_state = _states[HUB_AP_STATE_ERROR](HUB_AP_STATE_ERROR, params);
	}
	_state = _states[_state](_state, params);
	return _state;
}

void HubAP::add(uint8_t state, StateHandler f) {
	_states[state] = f;
}

void HubAP::setState(uint8_t state) {
	_state = state;
}

// Connect WiFi
void HubAP::_connectWiFi() {
	// TODO read SSID and Password from the eeprom
	// Connect WiFi
	WiFi.begin(HUB_AP_WIFI_SSID, HUB_AP_WIFI_PASS);
	// Wait max to 30 seconds
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.printf("Waiting WiFi Connection: SSID=%s\n", HUB_AP_WIFI_SSID);		
		flashLED(40);
	}
	String macStr = WiFi.macAddress();
	memcpy(_macAddr, macStr.c_str(), 6);
	Serial.printf("WiFi Connected: MAC=%s\n", macStr.c_str());
} 

void HubAP::flashLED(int d) {
	int state = digitalRead(HUB_AP_LED);
	digitalWrite(HUB_AP_LED, !state);
	delay(d/2);
	digitalWrite(HUB_AP_LED, state);
	delay(d/2);	
}
/*
int HubAP::call(char api[], char p[]) {
HTTPClient http;
	http.begin("http://192.168.1.65:8080/echo"); //HTTP
	int httpCode = http.GET();
	if(httpCode > 0) {
		if(httpCode == HTTP_CODE_OK) {
			String payload = http.getString();
		}
	}
	http.end();
	return httpCode;	
}
*/
