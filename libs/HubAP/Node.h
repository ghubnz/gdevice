#ifndef HUB_AP_NODE
#define HUB_AP_NODE

// #include <ESP8266HTTPClient.h>
#include "Config.h"

// Node class
class NodeClass {
	public:
		NodeClass(ConfigClass *config);
		uint8_t wait();
		int card(char *uid, char *apid);
	private:
		ConfigClass *Config;
};

#endif // HUB_AP_NODE
