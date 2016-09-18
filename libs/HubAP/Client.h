#ifndef HUB_AP_CLIENT
#define HUB_AP_CLIENT

// #include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Config.h"

// Client class
class ClientClass {
	public:
		ClientClass(ConfigClass *config);

		String call(char *api, char *payload);
		int card(char *uid, char *apid);
	private:
		ConfigClass *Config;
};

#endif // HUB_AP_CLIENT
