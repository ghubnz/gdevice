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
		char *_inTopic = NULL;
		bool _finish = false;
		int _retry = 0;

		void _callback(char* topic, byte* payload, unsigned int length);
		uint8_t _reconnect();
};

#endif // HUB_AP_NODE
