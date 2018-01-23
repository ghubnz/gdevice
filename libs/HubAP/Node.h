#ifndef HUB_AP_NODE
#define HUB_AP_NODE

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Config.h"

#define HUB_AP_BUZZER 5 // D3
#define NOTE_C 262
#define NOTE_D 294
#define NOTE_E 330
#define NOTE_F 349
#define NOTE_G 392
#define NOTE_A 440
#define NOTE_B 494
#define TONE_SPEED 200

// Node class
class NodeClass {
	public:
		NodeClass(ConfigClass *config);
		
		uint8_t setup();
		uint8_t loop();
		void preloop();

		int card(char *uid, char *apid);
		void debug();
	private:
		ConfigClass *_config;
		WiFiClientSecure _client;
		PubSubClient _mqtt = PubSubClient(_client);
		uint8_t _finish = HUB_AP_STATE_WAIT;
		int _retry = 0;
		char _pubTopicRFID[HUB_AP_MQTT_TOPIC_SIZE + 1] = {0};	
		char _pubTopicHeartbeat[HUB_AP_MQTT_TOPIC_SIZE + 1] = {0};
		int _heartbeatTick = 60;
		char _token[HUB_AP_CARD_SIZE + 1] = {0};
		char _user[HUB_AP_MQTT_USER_SIZE + 1] = {0};
		char _pass[HUB_AP_MQTT_PASS_SIZE + 1] = {0};
		char _clientId[HUB_AP_MQTT_CLIENTID_SIZE + 1] = {0};
		char _addr[HUB_AP_MQTT_ADDR_SIZE + 1] = {0};
		char _subTopic[HUB_AP_MQTT_TOPIC_SIZE + 1] = {0};
		int _port = 0;
		unsigned long _lastHeartbeat = 0;

		void _callback(char* topic, byte* payload, unsigned int length);
		uint8_t _reconnect();

		void _buzAccept();
		void _buzDeny();
		void _buzTimeout();
		void _buzConnected();
		void _buzRetry();
};
#endif // HUB_AP_NODE
