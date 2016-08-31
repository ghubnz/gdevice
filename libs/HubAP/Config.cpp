#include "SetupService.h"

#define LOAD(v, offset, size)\
	for(int x = 0; x < size; x++) { \
		v[x] = char(EEPROM.read(x + offset));}\
	v[offset + size] = '\0';

#define DUMP(v, offset, size)\
	for(int x = 0; x < size; x++) { EEPROM.write(x + offset, v[x]);}

#define SET(name, v, size) \
	void ConfigClass::set##name(const char *v) {strncpy(_##v, v, size);}

#define GET(name, v) \
	const char * ConfigClass::get##name() {return &_##v[0];}

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
	clean();
	DUMP(_wifiSSID, HUB_AP_WIFI_SSID_OFFSET, HUB_AP_WIFI_SSID_SIZE);
	DUMP(_wifiPass, HUB_AP_WIFI_PASS_OFFSET, HUB_AP_WIFI_PASS_SIZE);
	DUMP(_hubAddr, HUB_AP_ADDR_OFFSET, HUB_AP_ADDR_SIZE);
	DUMP(_hubKey, HUB_AP_HUBKEY_OFFSET, HUB_AP_HUBKEY_SIZE);
	DUMP(_secKey, HUB_AP_SECKEY_OFFSET, HUB_AP_SECKEY_SIZE);
	for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		DUMP(_card[i], HUB_AP_CARD_OFFSET + i * HUB_AP_CARD_SIZE, HUB_AP_CARD_SIZE);
	}
	EEPROM.commit();
}

// TODO parse card format
void ConfigClass::setCard(int i, const char *numStr) {
    for (int x = 0; x < HUB_AP_CARD_SIZE; x++) {
        _card[i][x] = strtoul(numStr, NULL, HEX);  // Convert byte
        numStr = strchr(numStr, ':');               // Find next separator
        if (numStr == NULL || *numStr == '\0') {
            break;                            // No more separators, exit
        }
        numStr++;                                // Point to next character after separator
    }
}

void ConfigClass::getCard(int i, char * card) {
	if (_card[i][0] == 0x00) {
		card[0] = 0x00;
		return;
	}
	sprintf(card, "%X", _card[i][0]);
	for (int x = 1; x < HUB_AP_CARD_SIZE; x ++) {	
		card += strlen(card);
		if (_card[i][x] == 0x00) {
			*card = '\0';
			break;
		}
		sprintf(card, ":%X", _card[i][x]);
	}
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
	EEPROM.commit();
}

String ConfigClass::debug() {
	String eeprom;
	for (int i = 0; i < HUB_AP_EEPROM_SIZE; i++) {
		uint8_t c = EEPROM.read(i);
		if (c >= 0x20 && c <= 0x7E) {
			eeprom += String((char)c);
		} else {
			eeprom += "[" + String(c, HEX) + "]";
		}
		if (i % 32 == 31) {
			eeprom += "\n";
		}
	}
	return eeprom;
}

void parseBytes(const char* str, char sep, char* bytes, int maxBytes, int base) {

}
