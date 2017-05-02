#include "Node.h"

// Public
NodeClass::NodeClass(ConfigClass *config) {
	_config = config;
}

uint8_t NodeClass::setup() {
	return _reconnect();
}

uint8_t NodeClass::loop() {
   	if (_reconnect() == HUB_AP_STATE_ERROR) {
		return HUB_AP_STATE_DENY;
	}
	_mqtt.loop();
	if (_finish == true) {
		_finish = false;
		return HUB_AP_STATE_ACCEPT;
	}
	if (_retry > 600) {
		_retry = 0;
		return HUB_AP_STATE_TIMEOUT;
	}
	delay(50);
	_retry ++;
	return HUB_AP_STATE_WAIT;
}

int NodeClass::card(char *uid, char *apid) {
   	if (_reconnect() == HUB_AP_STATE_ERROR) {
		return HUB_AP_STATE_DENY;
	}
	int l = sizeof(uid);
	char buf[l];
    snprintf (buf, l, "hello world");   
	Serial.print("Publish message: ");
    Serial.println(buf);
	// TODO read the topic from Config
	// Add the in-topic to msg
    _mqtt.publish("outTopic", buf); 
	return HUB_AP_STATE_WAIT;
}


void NodeClass::_callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();
	_finish = true;
}


uint8_t NodeClass::_reconnect() {
	// 10 times retry
	for(int i = 0; i < 10; i ++ ) {
		if (_mqtt.connected()) {
			return HUB_AP_STATE_NONE;
		}
		// TODO read the client id, user, and password from Config
		if (_mqtt.connect("test", "", "")) {
			Serial.println("connected");
			// TODO generate a random topic name for this device
			_mqtt.subscribe("inTopic");
			return HUB_AP_STATE_NONE;
		} else {
			Serial.print("failed, rc=");
			Serial.print(_mqtt.state());
			Serial.println(" try again in 3 seconds");
			// Wait 1 seconds before retrying
			delay(1000);
		}
	}
	_finish = false;
	return HUB_AP_STATE_ERROR;
}
