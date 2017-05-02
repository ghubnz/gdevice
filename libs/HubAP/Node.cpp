#include "Node.h"

// Public
NodeClass::NodeClass(ConfigClass *config) {
	_config = config;
}

uint8_t NodeClass::setup() {
	_config->getMQTTTopic(_pubTopic);
	gen_random(_subTopic, HUB_AP_MQTT_TOPIC_SIZE);
	return _reconnect();
}

uint8_t NodeClass::loop() {
   	if (_reconnect() == HUB_AP_STATE_ERROR) {
		return HUB_AP_STATE_DENY;
	}
	_mqtt.loop();
	if (_finish == HUB_AP_STATE_ACCEPT) {
		_finish = HUB_AP_STATE_WAIT;
		return HUB_AP_STATE_ACCEPT;
	}
	if (_retry > 600) {
		_retry = 0;
		_finish = HUB_AP_STATE_WAIT;
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
	// {"uid":"","topic":"","token":""}
	int l = 8 + 11 + 11 + 2 + HUB_AP_MQTT_TOPIC_SIZE + HUB_AP_CARD_SIZE + sizeof(uid);
	char buf[l];
    snprintf (buf, 8, "{\"uid\":\"");   
	snprintf (buf, sizeof(uid), uid);
    snprintf (buf, 11, "\",\"topic\":\"");
	snprintf (buf, HUB_AP_MQTT_TOPIC_SIZE, _subTopic);
	snprintf (buf, 11, "\",\"token\":\"");
	gen_random(_token, HUB_AP_CARD_SIZE);
	snprintf (buf, HUB_AP_CARD_SIZE, _token);
	snprintf (buf, 2, "\"}");
	Serial.print("Publish message: ");
    Serial.println(buf);
	_mqtt.publish(_pubTopic, buf); 
	return HUB_AP_STATE_WAIT;
}

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;
}

void NodeClass::_callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();
	_finish = HUB_AP_STATE_ACCEPT;
}


uint8_t NodeClass::_reconnect() {
	// 10 times retry
	for(int i = 0; i < 10; i ++ ) {
		if (_mqtt.connected()) {
			return HUB_AP_STATE_NONE;
		}
		char clientId[HUB_AP_MQTT_CLIENTID_SIZE] = {0};
		_config->getMQTTClientId(clientId);
		char user[HUB_AP_MQTT_USER_SIZE] = {0};
		_config->getMQTTUser(user);
		char pass[HUB_AP_MQTT_PASS_SIZE] = {0};
		_config->getMQTTPass(pass);	
		if (_mqtt.connect(clientId, user, pass)) {
			Serial.println("connected");
			_mqtt.subscribe(_subTopic);
			_finish = HUB_AP_STATE_WAIT;
			return HUB_AP_STATE_NONE;
		} else {
			Serial.print("failed, rc=");
			Serial.print(_mqtt.state());
			Serial.println(" try again in 3 seconds");
			// Wait 1 seconds before retrying
			delay(1000);
		}
	}
	_finish = HUB_AP_STATE_WAIT;
	return HUB_AP_STATE_ERROR;
}
