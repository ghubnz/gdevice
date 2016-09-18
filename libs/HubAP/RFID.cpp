#include "RFID.h"

// Public
RFIDClass::RFIDClass(ConfigClass *config, ClientClass *client) {
	_rfid = MFRC522(HUB_AP_RFID_SS, HUB_AP_RFID_RST);
	_config = config;
	_client = client;
}

uint8_t RFIDClass::setup() {	// Look for new cards
	// SPI
	SPI.begin();
	_rfid.PCD_Init();
	WiFi.macAddress(_key.keyByte);
	return HUB_AP_STATE_RFID;
}

uint8_t RFIDClass::loop() {
	if ( ! _rfid.PICC_IsNewCardPresent())
		return HUB_AP_STATE_RFID;

	// Verify if the NUID has been readed
	if ( ! _rfid.PICC_ReadCardSerial())
		return HUB_AP_STATE_RFID;

	if (_rfid.uid.size > HUB_AP_CARD_SIZE) {
//		Serial.println(F("UID is out of range"));
		return HUB_AP_STATE_ERROR;
	}

	/*
	Serial.print(F("PICC type: "));
	MFRC522::PICC_Type piccType = _rfid.PICC_GetType(_rfid.uid.sak);
	Serial.println(_rfid.PICC_GetTypeName(piccType));
*/
	uint8_t state = HUB_AP_STATE_DENY;
	// pre-checking
	//
	// compare to the last read card
	// if not last read 
	// or last read time greater than 5 seconds
	uint32_t now = millis();
	uint32_t s = - _readCardTime + now ;
	if (now < _readCardTime) { // timer rolling
    	s = ULONG_MAX + s;
  	}
	cardState = HUB_AP_RFID_DENY;
	while ((memcmp(_rfid.uid.uidByte, _readCard, _rfid.uid.size) != 0) || (s > 5000)) {
  		// trigger checking process
		// checking
		//
		// compare to accepted card
		// 
		// 0. Hardcode Master Key
		char masterKey[2][HUB_AP_CARD_SIZE] = {
		    {0x75, 0x44, 0xd1, 0x65},
		    {0x2b, 0xc9, 0xbd, 0x43}
		};	
  		for (int i = 0; i < sizeof(masterKey); i ++) {
			if (memcmp(_rfid.uid.uidByte, masterKey[i], _rfid.uid.size) == 0) {
				state = HUB_AP_STATE_ACCEPT;
				cardState = HUB_AP_RFID_MASTER;
				goto EXIT;
			}  
		}		
		//
		// 1. compare to root cards in EEPROM
		for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
			if (_config->matchCard(i, (char *)_rfid.uid.uidByte, _rfid.uid.size)) {
				state = HUB_AP_STATE_ACCEPT;
				cardState = HUB_AP_RFID_ADMIN;	
      			goto EXIT;
	    	}
		}
		// 2. compare to local cache
		//
		//	if (Cache.search(_rfid.uid.uidByte)) {
		//		state = HUB_AP_STATE_ACCEPT;
		//		cardState = HUB_AP_RFID_CACHE;
		//		goto EXIT;
		//	}
		//
		// 3. compare to Hub data
		char buf[1 + _rfid.uid.size];
		memcpy(buf, _rfid.uid.uidByte, _rfid.uid.size);
		buf[_rfid.uid.size] = '\0';
		int s = _client->card(buf, (char *)_key.keyByte);
		if (s != -1) {
			state = HUB_AP_STATE_ACCEPT;
			cardState = s;
			goto EXIT;
		}
		// DO NOT FORGET break
		break;
	}
EXIT:
	_readCardTime = now;
	memcpy(_readCard, _rfid.uid.uidByte, _rfid.uid.size);
	// Halt PICC
	_rfid.PICC_HaltA();
	// Stop encryption on PCD
	_rfid.PCD_StopCrypto1();
	return state;
}

void RFIDClass::getCard(char output[HUB_AP_CARD_SIZE]) {
	memcpy(output, _readCard, strlen(_readCard));
}

void RFIDClass::printCard() {
	Serial.print("Print: ");
	printHex(_readCard, HUB_AP_CARD_SIZE);
}
