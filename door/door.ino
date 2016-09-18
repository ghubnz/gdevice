#define DEBUG_ESP_HTTP_CLIENT 1
#define DEBUG_ESP_PORT 1

#include <HubAP.h>



// user defined state range 0x50 <= state <= 0xFF
#define HUB_AP_STATE_RUN 0x50
// relay
#define RELAY_PIN     5 // D8

void relayOn() {
	digitalWrite(RELAY_PIN, LOW);	
}

void relayOff() {
	digitalWrite(RELAY_PIN, HIGH);
}

void initRelay() {
	pinMode(RELAY_PIN, OUTPUT);	
}

void setup() {
  initRelay();
  relayOff();
  HubAP.setup();
  HubAP.add(HUB_AP_STATE_ERROR, errHandler);
  HubAP.add(HUB_AP_STATE_ACCEPT, acceptHandler);
  HubAP.add(HUB_AP_STATE_DENY, denyHandler);
}

void loop() {
  HubAP.loop(NULL);
}

uint8_t errHandler(uint8_t state, void *params ...) {
  Serial.printf("Error: %s", state);
  HubAP.flashLED(200);
  return HUB_AP_STATE_RFID;
}


uint8_t denyHandler(uint8_t state, void *params ...) {
  Serial.println("DENY");
  HubAP.flashLED(50);
  HubAP.flashLED(100);
  HubAP.flashLED(50);  
  return HUB_AP_STATE_RFID;
}

uint8_t acceptHandler(uint8_t state, void *params ...) {  
  char num[HUB_AP_CARD_SIZE] = {0};  
  HubAP.RFID.getCard(num);
  Serial.print("Source: "); 
  Serial.print(HubAP.RFID.CardState);
  Serial.print(" ACCEPT: "); 
  printHex(num, strlen(num));
  HubAP.flashLED(100);
  relayOn();
  delay(5000);
  relayOff();
  return HUB_AP_STATE_RFID;
}
