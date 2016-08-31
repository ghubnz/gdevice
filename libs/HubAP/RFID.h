#ifndef HUB_AP_RFID
#define HUB_AP_RFID

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <limits.h>

#include "HubAP.h"

// pins
#define HUB_AP_RFID_SS 4 // D4
#define HUB_AP_RFID_RST 16 // D2

// RFID class
class RFIDClass {
	public:
		RFIDClass();
		
		uint8_t setup();
		uint8_t loop();

		void getCard(char[HUB_AP_CARD_SIZE]);
	private:
		MFRC522 _rfid;
		MFRC522::MIFARE_Key _key;
		char _readCard[HUB_AP_CARD_SIZE] = {0};
		uint32_t _readCardTime = 0;
};

static RFIDClass RFID;

#endif // HUB_AP_RFID
