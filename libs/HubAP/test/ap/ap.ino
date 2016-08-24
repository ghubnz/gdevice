#include <HubAP.h>

HubAP ap;

void setup() {    
  ap.add(HUB_AP_STATE_INIT, initHandler);
  ap.add(HUB_AP_STATE_ERROR, errHandler);
  ap.setup();
}
 
void loop() {
  ap.loop(NULL);
}

uint8_t initHandler(uint8_t state, void *params ...) {
  Serial.println("INIT");
  ap.flashLED(200);
  return HUB_AP_STATE_ERROR;
}

uint8_t errHandler(uint8_t state, void *params ...) {
  Serial.println("ERROR");
  ap.flashLED(200);
  return HUB_AP_STATE_INIT;
}

