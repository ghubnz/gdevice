#include <HubAP.h>

ConfigClass Config;

void setup() {
    Config.clean();
}

void loop() {
  Serial.println(Config.debug());
  delay(10000);
}
