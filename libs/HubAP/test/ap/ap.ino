#include <Config.h>
#include <SetupService.h>

ConfigClass Config;
SetupServiceClass SetupService = SetupServiceClass(&Config);

void setup() {
  SetupService.setup();
}

void loop() {
 // SetupService.loop();
}
