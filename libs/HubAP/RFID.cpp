#include "RFID.h"

// Public
RFIDClass::RFIDClass() {
	WiFi.macAddress(_key.keyByte);
}

uint8_t RFIDClass::setup() {
	return HUB_AP_STATE_RFID;
}

uint8_t RFIDClass::loop() {
	return HUB_AP_STATE_RFID;
}
