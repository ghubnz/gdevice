#include "SetupService.h"

// Public
Config::Config() {
	EEPROM.begin(HUB_AP_EERPOM_SIZE);
}

void Config::load() {
	for (int i = HUB_AP_WIFI_SSID_OFFSET; 
			i < HUB_AP_WIFI_SSID_OFFSET + HUB_AP_WIFI_SSID_SIZE; ++i) {
 		_wifiSSID[i] = char(EEPROM.read(i));
 	}

	for (int i = HUB_AP_WIFI_PASS_OFFSET;
			i < HUB_AP_WIFI_PASS_SIZE; ++i) {
    	_wifiPass[i] = char(EEPROM.read(i));
  	}

	for (int i = HUB_AP_ADDR_OFFSET;
			i < HUB_AP_ADDR_OFFSET + HUB_AP_ADDR_SIZE; ++i) {
    	_hubAddr[i] = char(EEPROM.read(i));
  	}

	for (int i = HUB_AP_HUBKEY_OFFSET;
			i < HUB_AP_HUBKEY_OFFSET + HUB_AP_ADDR_SIZE; ++i) {
    	_hubKey[i] = char(EEPROM.read(i));
  	}

	for (int i = HUB_AP_SECKEY_OFFSET;
			i < HUB_AP_SECKEY_OFFSET + HUB_AP_ADDR_SIZE; ++i) {
    	_secKey[i] = char(EEPROM.read(i));
  	}
}

void Config::dump() {
	for (int i = HUB_AP_WIFI_SSID_OFFSET; 
			i < HUB_AP_WIFI_SSID_OFFSET + HUB_AP_WIFI_SSID_SIZE; ++i) {
		EEPROM.write(i, _wifiSSID[i]);
 	}

	for (int i = HUB_AP_WIFI_PASS_OFFSET;
			i < HUB_AP_WIFI_PASS_SIZE; ++i) {
		EEPROM.write(i, _wifiPass[i]);
  	}

	for (int i = HUB_AP_ADDR_OFFSET;
			i < HUB_AP_ADDR_OFFSET + HUB_AP_ADDR_SIZE; ++i) {
		EEPROM.write(i, _hubAddr[i]);  	
  	}

	for (int i = HUB_AP_HUBKEY_OFFSET;
			i < HUB_AP_HUBKEY_OFFSET + HUB_AP_ADDR_SIZE; ++i) {
		EEPROM.write(i, _hubKey[i]);  	 
  	}

	for (int i = HUB_AP_SECKEY_OFFSET;
			i < HUB_AP_SECKEY_OFFSET + HUB_AP_ADDR_SIZE; ++i) {
		EEPROM.write(i, _secKey[i]);  	   	
  	}
}

void Config::setSSID(char ssid[32]) {
	memcpy(_wifiSSID, ssid, 32);
}

void Config::setPass(char pass[64]) {
	memcpy(_wifiPass, pass, 64);
}

void Config::setHubAddr(char addr[128]) {
	memcpy(_hubAddr, addr, 128);
}

void Config::setHubKey(char key[16]) {
	memcpy(_hubKey, key, 16);
}

void Config::setSecKey(char key[16]) {
	memcpy(_secKey, key, 16);
}

char * Config::getSSID() {
	return &_wifiSSID[0];
}

char * Config::getPass() {
	return &_wifiPass[0];
}

char * Config::getHubAddr() {
	return &_hubAddr[0];
}

char * Config::getHubKey() {
	return &_hubKey[0];
}

char * Config::getSecKey() {
	return &_secKey[0];
}

void Config::clean() {
	for (int i = 0; i < HUB_AP_EERPOM_SIZE; i++) {
		EEPROM.write(i, 0);
	}
}
