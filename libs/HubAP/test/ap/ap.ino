#include <HubAP.h>

// user defined state range 0x50 <= state <= 0xFF
#define HUB_AP_STATE_RUN 0x50

void setup() {
  HubAP.setup();
  HubAP.add(HUB_AP_STATE_ERROR, errHandler);
  HubAP.add(HUB_AP_STATE_ACCEPT, acceptHandler);
  HubAP.add(HUB_AP_STATE_DENY, denyHandler);
}

void loop() {
  HubAP.loop(NULL);
}

uint8_t errHandler(uint8_t state, void *params ...) {
  Serial.println(state);
  HubAP.flashLED(200);
  return HUB_AP_STATE_RFID;
}


uint8_t denyHandler(uint8_t state, void *params ...) {
  Serial.println("DENY");
  HubAP.flashLED(200);
  return HUB_AP_STATE_RFID;
}

uint8_t acceptHandler(uint8_t state, void *params ...) {
  char num[HUB_AP_CARD_SIZE] = {0};
  RFID.getCard(num);
  Serial.printf("ACCEPT: %s\n", num);
  HubAP.flashLED(200);
  return HUB_AP_STATE_RFID;
}
