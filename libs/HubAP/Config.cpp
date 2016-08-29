#include "SetupService.h"

#define LOAD(v, offset, size)\
	for(int x = offset; x < offset + size; x++) {v[x] = char(EEPROM.read(x));}

#define DUMP(v, offset, size)\
	for(int x = offset; x < offset + size; x++) {EEPROM.write(x, v[x]);}

#define SET(name, v, size) \
	void ConfigClass::set##name(const char *v) {memcpy(_##v, v, size);}

#define GET(name, v) \
	char * ConfigClass::get##name() {return &_##v[0];}

// Public
ConfigClass::ConfigClass() {
	EEPROM.begin(HUB_AP_EEPROM_SIZE);
}

void ConfigClass::load() {
	LOAD(_wifiSSID, HUB_AP_WIFI_SSID_OFFSET, HUB_AP_WIFI_SSID_SIZE);
	LOAD(_wifiPass, HUB_AP_WIFI_PASS_OFFSET, HUB_AP_WIFI_PASS_SIZE);
	LOAD(_hubAddr, HUB_AP_ADDR_OFFSET, HUB_AP_ADDR_SIZE);
	LOAD(_hubKey, HUB_AP_HUBKEY_OFFSET, HUB_AP_HUBKEY_SIZE);
	LOAD(_secKey, HUB_AP_SECKEY_OFFSET, HUB_AP_SECKEY_SIZE);
	for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		LOAD(_card[i], HUB_AP_CARD_OFFSET + i * HUB_AP_CARD_SIZE, HUB_AP_CARD_SIZE);
	}
}

void ConfigClass::dump() {
	DUMP(_wifiSSID, HUB_AP_WIFI_SSID_OFFSET, HUB_AP_WIFI_SSID_SIZE);
	DUMP(_wifiPass, HUB_AP_WIFI_PASS_OFFSET, HUB_AP_WIFI_PASS_SIZE);
	DUMP(_hubAddr, HUB_AP_ADDR_OFFSET, HUB_AP_ADDR_SIZE);
	DUMP(_hubKey, HUB_AP_HUBKEY_OFFSET, HUB_AP_HUBKEY_SIZE);
	DUMP(_secKey, HUB_AP_SECKEY_OFFSET, HUB_AP_SECKEY_SIZE);
	for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		DUMP(_card[i], HUB_AP_CARD_OFFSET + i * HUB_AP_CARD_SIZE, HUB_AP_CARD_SIZE);
	}
}

void ConfigClass::setCard(int i, const char *num) {
	memcpy(_card[i], num, HUB_AP_CARD_SIZE);
}

char * ConfigClass::getCard(int i) {
	return &_card[i][0];
}

SET(SSID, wifiSSID, HUB_AP_WIFI_SSID_SIZE);
SET(Pass, wifiPass, HUB_AP_WIFI_PASS_SIZE);
SET(HubAddr, hubAddr, HUB_AP_ADDR_SIZE);
SET(HubKey, hubKey, HUB_AP_HUBKEY_SIZE);
SET(SecKey, secKey, HUB_AP_SECKEY_SIZE);

GET(SSID, wifiSSID);
GET(Pass, wifiPass);
GET(HubAddr, hubAddr);
GET(HubKey, hubKey);
GET(SecKey, secKey);

void ConfigClass::clean() {
	for (int i = 0; i < HUB_AP_EEPROM_SIZE; i++) {
		EEPROM.write(i, 0);
	}
}
