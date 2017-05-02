#include "Node.h"

// Public
NodeClass::NodeClass(ConfigClass *config) {
	_config = config;
}

uint8_t NodeClass::setup() {
	_config->getMQTTTopic(_pubTopic);
	_config->getMQTTClientId(_clientId);
	_config->getMQTTUser(_user);
	_config->getMQTTPass(_pass);	
	_config->getMQTTAddr(_addr);	
	char port[HUB_AP_MQTT_PORT_SIZE] = {0};
	_config->getMQTTPort(port);
	_port = atoi(port);
	
	gen_random(_subTopic, HUB_AP_MQTT_TOPIC_SIZE);
 	_mqtt.setServer(_addr, _port);
	_mqtt.setCallback([this](char* topic, byte* payload, unsigned int length){
		_callback(topic, payload, length);		
	});
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
	} else if (_finish == HUB_AP_STATE_DENY) {
		_finish = HUB_AP_STATE_WAIT;
		return HUB_AP_STATE_DENY;	
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

void NodeClass::_callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();
	
	char token[length + 1];
	memcpy(token, payload, length);
	token[length] = '\0';
	if ((strcmp(topic, _pubTopic) == 0) && (strcmp(token, _token) == 0)) {
		_finish = HUB_AP_STATE_ACCEPT;
		return;
	}
	_finish = HUB_AP_STATE_DENY;
}


uint8_t NodeClass::_reconnect() {
	// 10 times retry
	for(int i = 0; i < 10; i ++ ) {
		if (_mqtt.connected()) {
			return HUB_AP_STATE_NONE;
		}
		Serial.printf("%s:%d ", _addr, _port);
	
		int r;
		if (strlen(_user) == 0) {
			r = _mqtt.connect(_clientId);
		} else {
			r = _mqtt.connect(_clientId, _user, _pass);
		}
		if (r) {
			Serial.println("connected");
			_mqtt.subscribe(_subTopic);
			_finish = HUB_AP_STATE_WAIT;
			return HUB_AP_STATE_NONE;
		} else {
			Serial.print("failed, rc=");
			Serial.print(_mqtt.state());
			Serial.println(" try again in 1 seconds");
			// Wait 1 seconds before retrying
			delay(1000);
		}
	}
	_finish = HUB_AP_STATE_WAIT;
	return HUB_AP_STATE_ERROR;
}
