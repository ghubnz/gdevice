#ifndef HUB_AP_NODE
#define HUB_AP_NODE

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Config.h"

// Node class
class NodeClass {
	public:
		NodeClass(ConfigClass *config);
		
		uint8_t setup();
		uint8_t loop();

		int card(char *uid, char *apid);
	private:
		ConfigClass *_config;
		WiFiClient _client;
		PubSubClient _mqtt = PubSubClient(_client);
		uint8_t _finish = HUB_AP_STATE_WAIT;
		int _retry = 0;
		char _subTopic[HUB_AP_MQTT_TOPIC_SIZE] = {0};
		char _pubTopic[HUB_AP_MQTT_TOPIC_SIZE] = {0};	
		char _token[HUB_AP_CARD_SIZE] = {0};

		void _callback(char* topic, byte* payload, unsigned int length);
		uint8_t _reconnect();
};

void gen_random(char *s, const int len);
#endif // HUB_AP_NODE
