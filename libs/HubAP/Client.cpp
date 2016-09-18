#include "Client.h"

// Public
ClientClass::ClientClass(ConfigClass *config) {
	Config = config;
}

String ClientClass::call(char *api, char *payload) {
	HTTPClient http;
	char addr[HUB_AP_ADDR_SIZE] = {0};
	Config->getHubAddr(addr);
	char port[HUB_AP_PORT_SIZE] = {0};
	Config->getHubAddr(port);
	char path[HUB_AP_PATH_SIZE] = {0};
	Config->getHubAddr(path);
	char key[HUB_AP_HUBKEY_SIZE] = {0};
	Config->getHubKey(key);
	http.begin(String(addr), String(port).toInt(), String(path) + String(api) , String(key));
    int httpCode = http.POST((uint8_t *)payload, sizeof(payload));
	String body;
    if(httpCode > 0) {
        if(httpCode == HTTP_CODE_OK) {
			body = http.getString();
		}
	}
    http.end();
	return body;
}

bool ClientClass::card(char *uid, char *apid) {
	char buf[15 + sizeof(uid)];
	sprintf(buf, "uid=%s&ap=%s", uid, apid);	
	Serial.println(call("card", buf));
}
