#include "Node.h"

// Public
NodeClass::NodeClass() {
}

void NodeClass::init(ConfigClass *config) {
	_config = config;
}

uint8_t NodeClass::setup() {
	_config->getMQTTTopicRFID(_pubTopicRFID);
	_config->getMQTTTopicHeartbeat(_pubTopicHeartbeat);
	_config->getMQTTClientId(_clientId);
	_config->getMQTTUser(_user);
	_config->getMQTTPass(_pass);
	_config->getMQTTAddr(_addr);

	char port[HUB_AP_MQTT_PORT_SIZE] = {0};
	_config->getMQTTPort(port);
	_port = atoi(port);

	char tick[HUB_AP_MQTT_HBTICK_SIZE] = {0};
	_config->getMQTTHeartbeatTick(tick);
	_heartbeatTick = atoi(tick);

	gen_random(_token, HUB_AP_CARD_SIZE);
	gen_random(_subTopic, HUB_AP_MQTT_TOPIC_SIZE);

	_mqtt.setServer(_addr, _port);
	_mqtt.setCallback([this](char* topic, byte* payload, unsigned int length){
		_callback(topic, payload, length);	
	});
	// reset for buzzer
	pinMode(HUB_AP_BUZZER, OUTPUT);
	pinMode(HUB_AP_LED, OUTPUT);
	return _reconnect();
}

void NodeClass::preloop() {
	if (_reconnect() == HUB_AP_STATE_ERROR) {
		return;
	}
	if (false == _mqtt.loop()) {
		Serial.println("MQTT Disconnected");
	}
	unsigned long now = millis();
	// customise frequency of heartbeat
	if ((now < _lastHeartbeat) || (now - _lastHeartbeat) > 1000 * _heartbeatTick) {
		// customise heartbeat topic
		_mqtt.publish(_pubTopicHeartbeat, _clientId);
		_lastHeartbeat = now;
	}
}

uint8_t NodeClass::loop() {
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
		// buzzer timeout
		_buzTimeout();
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
	StaticJsonBuffer<256> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
  	root["uid"] = sprintHex(uid, strlen(uid));
	root["token"] = _token;
	root["topic"] = _subTopic;
	root["device"] = _clientId;
	String s;
	root.printTo(s);
	_mqtt.publish(_pubTopicRFID, s.c_str());
	Serial.print("Publish [");
	Serial.print(_pubTopicRFID);
	Serial.print("] => [");
	Serial.print(s);
	Serial.println("] ");
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
	if ((strcmp(topic, _subTopic) == 0) && (strcmp(token, _token) == 0)) {
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
			// buzzer connected
			_buzConnected();
			return HUB_AP_STATE_NONE;
		} else {
			Serial.print("failed, rc=");
			Serial.print(_mqtt.state());
			Serial.println(" try again in 1 seconds");
			// Wait 1 seconds before retrying
			// buzzer retrying
			_buzRetry();
			delay(1000);
		}
	}
	_finish = HUB_AP_STATE_WAIT;
	return HUB_AP_STATE_ERROR;
}

void NodeClass::debug() {
	Serial.println(_port);
	Serial.println(_pubTopicRFID);
	Serial.println(_subTopic);
	Serial.println(_token);
	Serial.println(_addr);
	Serial.println(_user);
	Serial.println(_pass);
	Serial.println(_clientId);
}

void NodeClass::buzAccept() {
	tone(HUB_AP_BUZZER, NOTE_C, TONE_SPEED);
	digitalWrite(HUB_AP_LED, HIGH);
	delay(TONE_SPEED);
	tone(HUB_AP_BUZZER, NOTE_G, TONE_SPEED);
	digitalWrite(HUB_AP_LED, LOW);		
	delay(TONE_SPEED);
	tone(HUB_AP_BUZZER, NOTE_E, TONE_SPEED * 2);
	digitalWrite(HUB_AP_LED, HIGH);		
	delay(TONE_SPEED * 2);
	noTone(HUB_AP_BUZZER);
	digitalWrite(HUB_AP_LED, LOW);	
}

void NodeClass::buzDeny() {
	tone(HUB_AP_BUZZER, NOTE_E, TONE_SPEED);
	digitalWrite(HUB_AP_LED, HIGH);	
	delay(TONE_SPEED);
	tone(HUB_AP_BUZZER, NOTE_G, TONE_SPEED);
	delay(TONE_SPEED);
	tone(HUB_AP_BUZZER, NOTE_C, TONE_SPEED);
	delay(TONE_SPEED);
	noTone(HUB_AP_BUZZER);
	digitalWrite(HUB_AP_LED, LOW);	
}

void NodeClass::_buzTimeout() {
	tone(HUB_AP_BUZZER, NOTE_E, TONE_SPEED);
	digitalWrite(HUB_AP_LED, HIGH);	
	delay(TONE_SPEED);
	tone(HUB_AP_BUZZER, NOTE_G, TONE_SPEED);
	delay(TONE_SPEED);
	tone(HUB_AP_BUZZER, NOTE_E, TONE_SPEED);
	digitalWrite(HUB_AP_LED, LOW);	
	delay(TONE_SPEED);
	tone(HUB_AP_BUZZER, NOTE_G, TONE_SPEED);
	digitalWrite(HUB_AP_LED, HIGH);	
	delay(TONE_SPEED);	
	noTone(HUB_AP_BUZZER);
	digitalWrite(HUB_AP_LED, LOW);	
}

void NodeClass::_buzConnected() {
	tone(HUB_AP_BUZZER, NOTE_C, TONE_SPEED);
	digitalWrite(HUB_AP_LED, HIGH);	
	delay(TONE_SPEED);
	noTone(HUB_AP_BUZZER);
	digitalWrite(HUB_AP_LED, LOW);	
}

void NodeClass::_buzRetry() {
	tone(HUB_AP_BUZZER, NOTE_G, TONE_SPEED);
	digitalWrite(HUB_AP_LED, HIGH);	
	delay(TONE_SPEED);
	noTone(HUB_AP_BUZZER);
	digitalWrite(HUB_AP_LED, LOW);	
}
