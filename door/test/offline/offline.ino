#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <ESP8266WiFi.h>

// pins
#define RC522_SS_PIN  4 // D4
#define RC522_RST_PIN 16 // D2
#define SERVO_PIN     0 // D8
#define SERVO_SPEED   1
// servo
#define STAT_LOCK   10
#define STAT_UNLOCK 170
// const
#define MAX_NUID    32

Servo servo;
MFRC522 rfid(RC522_SS_PIN, RC522_RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte lastCard[MAX_NUID] = {0};
byte acceptCard[MAX_NUID] = {0x75, 0x44, 0xd1, 0x65};

void setup() { 
	Serial.begin(115200);
	SPI.begin(); // Init SPI bus
	rfid.PCD_Init(); // Init MFRC522 

	WiFi.macAddress(key.keyByte);
	
	Serial.println(F("This code scan the MIFARE Classsic NUID."));
	Serial.print(F("Using the following key:"));
	printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
	
	initDoor();
}

void loop() {

	// Look for new cards
	if ( ! rfid.PICC_IsNewCardPresent())
		return;

	// Verify if the NUID has been readed
	if ( ! rfid.PICC_ReadCardSerial())
		return;

	Serial.print(F("PICC type: "));
	MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
	Serial.println(rfid.PICC_GetTypeName(piccType));

  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.print(F("In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);

  if (rfid.uid.size > MAX_NUID) {
    Serial.println(F("NUID is out of range"));
    return;
  }
  
  // compare to the last read card
  if (memcmp(rfid.uid.uidByte, lastCard, rfid.uid.size) == 0) {
    Serial.println(F("Card read previously."));    
  } else {
    Serial.println(F("A new card has been detected."));
    for (byte i = 0; i < rfid.uid.size; i++) {
      lastCard[i] = rfid.uid.uidByte[i];
    } 
  }

  // compare to accepted card
  if (memcmp(rfid.uid.uidByte, acceptCard, rfid.uid.size) == 0) {    
    Serial.println(F("Trigger door"));
    triggerDoor(SERVO_SPEED);
  }  
 
	// Halt PICC
	rfid.PICC_HaltA();

	// Stop encryption on PCD
	rfid.PCD_StopCrypto1();
}

void initDoor() {
  servo.attach(SERVO_PIN);
  servo.write(STAT_LOCK);
  delay(500);  
}

/**
 * Pull the trigger and reset
 */
void triggerDoor(int d) {
	for (int i = STAT_LOCK; i <= STAT_UNLOCK; i ++) {
		servo.write(i);
		delay(d);
	}
	delay(500);
	for (int i = STAT_UNLOCK; i >= STAT_LOCK; i --) {
		servo.write(i);
		delay(d);
	}
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
	for (byte i = 0; i < bufferSize; i++) {
		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], HEX);
	}
  Serial.println();
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
	for (byte i = 0; i < bufferSize; i++) {
		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], DEC);
	}
  Serial.println();
}
