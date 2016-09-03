#include <HubAP.h>
#include <Servo.h>

// user defined state range 0x50 <= state <= 0xFF
#define HUB_AP_STATE_RUN 0x50
// servo
#define SERVO_PIN     5 // D8
#define SERVO_SPEED   1
#define STAT_LOCK   10
#define STAT_UNLOCK 170

Servo servo;

void initDoor() {
  servo.attach(SERVO_PIN);
  servo.write(STAT_LOCK);
  delay(500);
}

/**
   Pull the trigger and reset
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

void setup() {
  HubAP.setup();
  HubAP.add(HUB_AP_STATE_ERROR, errHandler);
  HubAP.add(HUB_AP_STATE_ACCEPT, acceptHandler);
  HubAP.add(HUB_AP_STATE_DENY, denyHandler);
  initDoor();
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
  HubAP.flashLED(200);
  return HUB_AP_STATE_RFID;
}

uint8_t acceptHandler(uint8_t state, void *params ...) {
  char num[HUB_AP_CARD_SIZE] = {0};
  RFID.getCard(num);
  Serial.printf("ACCEPT: %s\n", num);
  HubAP.flashLED(200);
  triggerDoor(SERVO_SPEED);
  return HUB_AP_STATE_RFID;
}
