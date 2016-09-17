#ifndef HUB_AP_CLIENT
#define HUB_AP_CLIENT

#include <ESP8266HTTPClient.h>
#include "Config.h"

// Client class
class ClientClass {
	public:
		ClientClass(ConfigClass *config);

		String call(char *api, char *payload);
		bool door(char *uid, char *apid);
	private:
		ConfigClass *Config;
};

#endif // HUB_AP_CLIENT
