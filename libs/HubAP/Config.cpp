#include "Config.h"

#define LOAD(v, offset, size)\
	for(int x = 0; x < size; x++) { \
		v[x] = char(EEPROM.read(x + offset));}\
	v[offset + size] = '\0';

#define DUMP(v, offset, size)\
	for(int x = 0; x < size; x++) { EEPROM.write(x + offset, v[x]);}

#define SET(name, v, size) \
	void ConfigClass::set##name(const char *v) {strncpy(_##v, v, size);}

#define GET(name, v) \
	void ConfigClass::get##name(char *v) {strcpy(v, _##v);}

// Public
ConfigClass::ConfigClass() {
	EEPROM.begin(HUB_AP_EEPROM_SIZE);
}

void ConfigClass::load() {
	LOAD(_wifiSSID, HUB_AP_WIFI_SSID_OFFSET, HUB_AP_WIFI_SSID_SIZE);
	LOAD(_wifiPass, HUB_AP_WIFI_PASS_OFFSET, HUB_AP_WIFI_PASS_SIZE);
	LOAD(_mqttAddr, HUB_AP_MQTT_ADDR_OFFSET, HUB_AP_MQTT_ADDR_SIZE);
	LOAD(_mqttPort, HUB_AP_MQTT_PORT_OFFSET, HUB_AP_MQTT_PORT_SIZE);
	LOAD(_mqttUser, HUB_AP_MQTT_USER_OFFSET, HUB_AP_MQTT_USER_SIZE);
	LOAD(_mqttPass, HUB_AP_MQTT_PASS_OFFSET, HUB_AP_MQTT_USER_SIZE);
	for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		LOAD(_card[i], HUB_AP_CARD_OFFSET + i * HUB_AP_CARD_SIZE, HUB_AP_CARD_SIZE);
	}
}

void ConfigClass::dump() {
	clean();
	DUMP(_wifiSSID, HUB_AP_WIFI_SSID_OFFSET, HUB_AP_WIFI_SSID_SIZE);
	DUMP(_wifiPass, HUB_AP_WIFI_PASS_OFFSET, HUB_AP_WIFI_PASS_SIZE);
	DUMP(_mqttAddr, HUB_AP_MQTT_ADDR_OFFSET, HUB_AP_MQTT_ADDR_SIZE);
	DUMP(_mqttPort, HUB_AP_MQTT_PORT_OFFSET, HUB_AP_MQTT_PORT_SIZE);
	DUMP(_mqttUser, HUB_AP_MQTT_USER_OFFSET, HUB_AP_MQTT_USER_SIZE);	
	DUMP(_mqttPass, HUB_AP_MQTT_PASS_OFFSET, HUB_AP_MQTT_PASS_SIZE);
	for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
		DUMP(_card[i], HUB_AP_CARD_OFFSET + i * HUB_AP_CARD_SIZE, HUB_AP_CARD_SIZE);
	}
	EEPROM.commit();
}

// parse card format
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

bool ConfigClass::matchCard(int i, char card[HUB_AP_CARD_SIZE], int size) {
	return memcmp(card, _card[i], size) == 0;
}

SET(SSID, wifiSSID, HUB_AP_WIFI_SSID_SIZE);
SET(Pass, wifiPass, HUB_AP_WIFI_PASS_SIZE);
SET(MQTTAddr, mqttAddr, HUB_AP_MQTT_ADDR_SIZE);
SET(MQTTPort, mqttPort, HUB_AP_MQTT_PORT_SIZE);
SET(MQTTUser, mqttUser, HUB_AP_MQTT_USER_SIZE);
SET(MQTTPass, mqttPass, HUB_AP_MQTT_PASS_SIZE);

GET(SSID, wifiSSID);
GET(Pass, wifiPass);
GET(MQTTAddr, mqttAddr);
GET(MQTTPort, mqttPort);
GET(MQTTUser, mqttUser);
GET(MQTTPass, mqttPass);

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

void printHex(char *buffer, int bufferSize) {
	for (byte i = 0; i < bufferSize; i++) {
		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], HEX);
	}
  Serial.println();
}
