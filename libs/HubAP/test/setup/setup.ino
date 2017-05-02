#include <HubAP.h>

ConfigClass config;
SetupServiceClass SetupService(&config);

void setup() {
    SetupService.setup();
}

void loop() {
  SetupService.loop();
}
