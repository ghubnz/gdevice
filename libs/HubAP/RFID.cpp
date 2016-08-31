#include "RFID.h"

// Public
RFIDClass::RFIDClass() {
	_rfid = MFRC522();
}

uint8_t RFIDClass::setup() {	// Look for new cards
	_rfid.PCD_Init();
	WiFi.macAddress(_key.keyByte);
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
	while (memcmp(_rfid.uid.uidByte, _readCard, _rfid.uid.size) == 0 || s > 60000) {
  		// trigger checking process
		// checking
		//
		// compare to accepted card
		// 1. compare to root cards
		for (int i = 0; i < HUB_AP_CARD_NUM; i ++) {
			char card[HUB_AP_CARD_SIZE] = {0};
			Config.getCard(i, card);
			if (memcmp(_rfid.uid.uidByte, card, _rfid.uid.size) == 0) {
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
	// Halt PICC
	_rfid.PICC_HaltA();
	// Stop encryption on PCD
	_rfid.PCD_StopCrypto1();
	return state;
}

void RFIDClass::getCard(char output[HUB_AP_CARD_SIZE]) {
	memcpy(output, _readCard, HUB_AP_CARD_SIZE);
}

