#include <HubAP.h>

void setup() {
  HubAP.setup();
  HubAP.add(HUB_AP_STATE_INIT, initHandler);
  HubAP.add(HUB_AP_STATE_ERROR, errHandler);
}

void loop() {
  HubAP.loop(NULL);  
}

uint8_t initHandler(uint8_t state, void *params ...) {
  Serial.println("INIT");
  HubAP.flashLED(200);
  return HUB_AP_STATE_ERROR;
}

uint8_t errHandler(uint8_t state, void *params ...) {
  Serial.println("ERROR");
  HubAP.flashLED(200);
  return HUB_AP_STATE_INIT;
}
