#include "Client.h"

// Public
ClientClass::ClientClass(ConfigClass *config) {
	Config = config;
}


/*
   String ClientClass::call(char *api, char *payload) {
   HTTPClient http;
   char addr[HUB_AP_ADDR_SIZE] = {0};
   Config->getHubAddr(addr);
   char port[HUB_AP_PORT_SIZE] = {0};
   Config->getHubPort(port);
   char path[HUB_AP_PATH_SIZE] = {0};
   Config->getHubPath(path);
   char key[HUB_AP_HUBKEY_SIZE] = {0};
   Config->getHubKey(key);
   http.begin(String(addr), String(port).toInt(), String(path) + String(api) , String(key));
   int httpCode = http.POST((uint8_t *)payload, sizeof(payload));
   String body;
   Serial.println(httpCode);
   if(httpCode > 0) {
   if(httpCode == HTTP_CODE_OK) {
   body = http.getString();
   }
   }
   http.end();
   return body;
   }
   */

String ClientClass::call(char *api, char *payload) {
	char addr[HUB_AP_ADDR_SIZE] = {0};
	Config->getHubAddr(addr);
	char port[HUB_AP_PORT_SIZE] = {0};
	Config->getHubPort(port);
	char path[HUB_AP_PATH_SIZE] = {0};
	Config->getHubPath(path);
	char key[HUB_AP_HUBKEY_SIZE] = {0};
	Config->getHubKey(key);
   
	// Use WiFiClientSecure class to create TLS connection
	WiFiClientSecure client;
	if (!client.connect(addr, String(port).toInt())) {
		return "";
	}

	client.print(String("POST ") + String(path) + String(api) + " HTTP/1.1\r\n" +
			"Host: " + addr + "\r\n" +
			"User-Agent: MeshedNET\r\n" +
			"Connection: close\r\n" +
			"Content-Type: application/x-www-form-urlencoded\r\n" +
			"Content-Length: " + String(strlen(payload)) + "\r\n" + 
			"Connection: close\r\n\r\n" + 
			String(payload) + 
			"\r\n\r\n");

	while (client.connected()) {
		String line = client.readStringUntil('\n');
		if (line == "\r") {
			break;
		}
	}
	return client.readStringUntil('\n');
}

int ClientClass::card(char *uid, char *apid) {
	char buf[15 + sizeof(uid)];
	sprintf(buf, "uid=%s&ap=%s", uid, apid);
	return call("card", buf).toInt();
}
