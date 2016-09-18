#ifndef HUB_AP_RFID
#define HUB_AP_RFID

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <limits.h>

#include "Config.h"
#include "Client.h"

// pins
#define HUB_AP_RFID_SS 4 // D4
#define HUB_AP_RFID_RST 16 // D2

//
#define HUB_AP_RFID_MASTER	0x00
#define HUB_AP_RFID_ADMIN	0x10
#define HUB_AP_RFID_HUB		0x20
#define HUB_AP_RFID_CACHE	0x30
#define HUB_AP_RFID_DENY		0xFF

// RFID class
class RFIDClass {
	public:
		RFIDClass(ConfigClass*, ClientClass*);
		
		uint8_t setup();
		uint8_t loop();

		void getCard(char[HUB_AP_CARD_SIZE]);

		void printCard();

		uint32_t CardState = HUB_AP_RFID_DENY;
	private:
		MFRC522 _rfid;
		MFRC522::MIFARE_Key _key;
		char _readCard[HUB_AP_CARD_SIZE] = {0};
		uint32_t _readCardTime = 0;
		ConfigClass *_config;
		ClientClass *_client;
};

#endif // HUB_AP_RFID
