#ifndef HUB_AP_CONFIG
#define HUB_AP_CONFIG

#define HUB_AP_WIFI_SSID "GHub"
#define HUB_AP_WIFI_PASS "gribblehirst"
#define HUB_AP_SERIAL_SPEED 115200

#define HUB_AP_WIFI_SSID_OFFSET	0
#define HUB_AP_WIFI_SSID_SIZE	32
#define HUB_AP_WIFI_PASS_OFFSET HUB_AP_WIFI_SSID_OFFSET + HUB_AP_WIFI_SSID_SIZE
#define HUB_AP_WIFI_PASS_SIZE	64
#define HUB_AP_ADDR_OFFSET HUB_AP_WIFI_PASS_OFFSET + HUB_AP_WIFI_PASS_SIZE
#define HUB_AP_ADDR_SIZE 128
#define HUB_AP_HUBKEY_OFFSET HUB_AP_ADDR_OFFSET + HUB_AP_ADDR_SIZE
#define HUB_AP_HUBKEY_SIZE 16
#define HUB_AP_SECKEY_OFFSET HUB_AP_HUBKEY_OFFSET + HUB_AP_HUBKEY_SIZE
#define HUB_AP_SECKEY_SIZE 16

#define HUB_AP_EERPOM_SIZE HUB_AP_WIFI_SSID_SIZE + HUB_AP_WIFI_PASS_SIZE +\
	HUB_AP_ADDR_SIZE + HUB_AP_HUBKEY_SIZE + HUB_AP_SECKEY_SIZE

// led
#define HUB_AP_LED LED_BUILTIN

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "HubAP.h"

// Config class
class Config {
	public:
		Config();
		
		void clean();
		void load();
		void dump();

		void setSSID(char[32]);
		void setPass(char[64]);
		void setHubAddr(char[128]);
		void setHubKey(char[16]);
		void setSecKey(char[16]);
	
		char * getSSID();
		char * getPass();
		char * getHubAddr();
		char * getHubKey();
		char * getSecKey();
	private:
		char _wifiSSID[32] = {0};
		char _wifiPass[64] = {0};
		char _hubAddr[128] = {0};
		char _hubKey[16] = {0};
		char _secKey[16] = {0};
		// TODO admin card
};

#endif // HUB_AP_CONFIG
