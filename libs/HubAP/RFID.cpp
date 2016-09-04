#include "RFID.h"

// Public
RFIDClass::RFIDClass() {
	_rfid = MFRC522(HUB_AP_RFID_SS, HUB_AP_RFID_RST);
}

uint8_t RFIDClass::setup() {	// Look for new cards
	// SPI
	Config.load();
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
		Serial.println(F("UID is out of range"));
		return HUB_AP_STATE_ERROR;
	}

	Serial.print(F("PICC type: "));
	MFRC522::PICC_Type piccType = _rfid.PICC_GetType(_rfid.uid.sak);
	Serial.println(_rfid.PICC_GetTypeName(piccType));

	uint8_t state = HUB_AP_STATE_DENY;
	// pre-checking
	//
	// compare to the last read card
	// if not last read 
	// or last read time greater than 1 min
	uint32_t now = millis();
	uint32_t s = - _readCardTime + now ;
	if (now < _readCardTime) { // timer rolling
    	s = ULONG_MAX + s;
  	}
	while (memcmp(_rfid.uid.uidByte, _readCard, _rfid.uid.size) != 0 || s < 60000) {
  		// trigger checking process
		// checking
		//
		// compare to accepted card
		// 1. compare to root cards
		for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
			if (Config.matchCard(i, (char *)_rfid.uid.uidByte, _rfid.uid.size)) {
				state = HUB_AP_STATE_ACCEPT;
      			goto EXIT;
	    	}
		}
		
		// 2. compare to Hub data
		//
		//	if (HubAP.tag(_rfid.uid.uidByte)) {
		//		state = HUB_AP_STATE_ACCEPT;
		//		goto EXIT;
		//	}
		//
		// 3. compare to local cache
		//
		//	if (Cache.search(_rfid.uid.uidByte)) {
		//		state = HUB_AP_STATE_ACCEPT;
		//		goto EXIT;
		//	}
		//
		// DO NOT FORGET break
		break;
	}
EXIT:
	_readCardTime = now;
	printHex((char *)_rfid.uid.uidByte, _rfid.uid.size);
	memcpy(_readCard, _rfid.uid.uidByte, _rfid.uid.size);
	printHex(_readCard, _rfid.uid.size);	
	// Halt PICC
	_rfid.PICC_HaltA();
	// Stop encryption on PCD
	_rfid.PCD_StopCrypto1();
	return state;
}

void RFIDClass::getCard(char output[HUB_AP_CARD_SIZE]) {
	memcpy(output, _readCard, HUB_AP_CARD_SIZE);
}

