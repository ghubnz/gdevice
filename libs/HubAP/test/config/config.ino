#include <HubAP.h>

ConfigClass Config;

void setup() {
  Config.clean();
  /*Config.setSSID("12345678123456781234567812345678");
  Config.setPass("12345678123456781234567812345678");
  Config.setMQTTAddr("1234567812345678123456781234567812345678123456781234567812345678");
  Config.setMQTTPort("12345");
  Config.setMQTTUser("12345678123456781234567812345678");
  Config.setMQTTPass("12345678123456781234567812345678");
  Config.setMQTTTopicRFID("12345678123456781234567812345678");
  Config.setMQTTTopicHeartbeat("12345678123456781234567812345678");
  
  Config.setMQTTClientId("12345678123456781234567812345678");
  Config.setMQTTHeartbeatTick("99999");
  Config.setCard(0, "31:32:33:34:35:36:37:38:31:32:33:34:35:36:37:38");
  Config.setCard(1, "31:32:33:34:35:36:37:38:31:32:33:34:35:36:37:38");
  Config.setCard(2, "31:32:33:34:35:36:37:38:31:32:33:34:35:36:37:38");
  Config.setCard(3, "31:32:33:34:35:36:37:38:31:32:33:34:35:36:37:38");
  Config.dump();*/
}

void loop() {
  Serial.println(Config.debug());
  delay(10000);
}
