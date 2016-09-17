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
#define HUB_AP_RFID_MASTERKEY	0x00
#define HUB_AP_RFID_ADMINKEY	0x10
#define HUB_AP_RFID_HUBKEY		0x20
#define HUB_AP_RFID_CACHEKEY	0x30
#define HUB_AP_RFID_DENYKEY		0xFF

// RFID class
class RFIDClass {
	public:
		RFIDClass(ConfigClass*, ClientClass*);
		
		uint8_t setup();
		uint8_t loop();

		void getCard(char[HUB_AP_CARD_SIZE]);

		void printCard();

	private:
		MFRC522 _rfid;
		MFRC522::MIFARE_Key _key;
		char _readCard[HUB_AP_CARD_SIZE] = {0};
		uint32_t _readCardTime = 0;
		ConfigClass *_config;
		ClientClass *_client;
};

#endif // HUB_AP_RFID
