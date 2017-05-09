#ifndef HUB_AP_NODE
#define HUB_AP_NODE

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Config.h"

// Node class
class NodeClass {
	public:
		NodeClass(ConfigClass *config);
		
		uint8_t setup();
		uint8_t loop();

		int card(char *uid, char *apid);
		void debug();
	private:
		ConfigClass *_config;
		WiFiClientSecure _client;
		PubSubClient _mqtt = PubSubClient(_client);
		uint8_t _finish = HUB_AP_STATE_WAIT;
		int _retry = 0;
		char _pubTopic[HUB_AP_MQTT_TOPIC_SIZE + 1] = {0};	
		char _token[HUB_AP_CARD_SIZE + 1] = {0};
		char _user[HUB_AP_MQTT_USER_SIZE + 1] = {0};
		char _pass[HUB_AP_MQTT_PASS_SIZE + 1] = {0};
		char _clientId[HUB_AP_MQTT_CLIENTID_SIZE + 1] = {0};
		char _addr[HUB_AP_MQTT_ADDR_SIZE + 1] = {0};
		char _subTopic[HUB_AP_MQTT_TOPIC_SIZE + 1] = {0};
		int _port = 0;

		void _callback(char* topic, byte* payload, unsigned int length);
		uint8_t _reconnect();
};
#endif // HUB_AP_NODE
