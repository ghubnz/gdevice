#include "SetupService.h"

// Public
SetupService::SetupService() {
	pinMode(HUB_AP_SETUP_BTN, INPUT);
}

uint8_t SetupService::loop() {
	int setupState = digitalRead(HUB_AP_SETUP_BTN);
	if (setupState == LOW) {
		return HUB_AP_STATE_INIT;
	}
	// TODO start WiFi AP
	// TODO start HTTP service
	// TODO write config into eeprom
	// TODO if (error) return HUB_AP_STATE_ERROR;
	return HUB_AP_STATE_INIT;
}
