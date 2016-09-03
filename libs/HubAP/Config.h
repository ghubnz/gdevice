#ifndef HUB_AP_CONFIG
#define HUB_AP_CONFIG

#define HUB_AP_WIFI_SSID "GHub"
#define HUB_AP_WIFI_PASS "gribblehirst"
#define HUB_AP_SERIAL_SPEED 115200

#define HUB_AP_WIFI_SSID_OFFSET	0
#define HUB_AP_WIFI_SSID_SIZE	32
#define HUB_AP_WIFI_PASS_OFFSET HUB_AP_WIFI_SSID_OFFSET + HUB_AP_WIFI_SSID_SIZE //32
#define HUB_AP_WIFI_PASS_SIZE	64
#define HUB_AP_ADDR_OFFSET HUB_AP_WIFI_PASS_OFFSET + HUB_AP_WIFI_PASS_SIZE // 96
#define HUB_AP_ADDR_SIZE 128
#define HUB_AP_HUBKEY_OFFSET HUB_AP_ADDR_OFFSET + HUB_AP_ADDR_SIZE // 224
#define HUB_AP_HUBKEY_SIZE 16
#define HUB_AP_SECKEY_OFFSET HUB_AP_HUBKEY_OFFSET + HUB_AP_HUBKEY_SIZE // 240
#define HUB_AP_SECKEY_SIZE 16
#define HUB_AP_CARD_OFFSET HUB_AP_SECKEY_OFFSET + HUB_AP_SECKEY_SIZE // 256
#define HUB_AP_CARD_SIZE 16
#define HUB_AP_CARD_NUM 4
// #define HUB_AP_EEPROM_SIZE 512

#define HUB_AP_EEPROM_SIZE HUB_AP_CARD_OFFSET + \
	( HUB_AP_CARD_SIZE * HUB_AP_CARD_NUM ) // 320 = 256 + ( 16 * 4 )

// led
#define HUB_AP_LED LED_BUILTIN

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "HubAP.h"

// Config class
class ConfigClass {
	public:
		ConfigClass();
		
		void clean();
		void load();
		void dump();
		String debug();

		void setSSID(const char*);
		void setPass(const char*);
		void setHubAddr(const char*);
		void setHubKey(const char*);
		void setSecKey(const char*);
		void setCard(int, const char*);
	
		void getSSID(char *);
		void getPass(char *);
		void getHubAddr(char *);
		void getHubKey(char *);
		void getSecKey(char *);
		void getCard(int, char *);

		bool matchCard(int, char [HUB_AP_CARD_SIZE], int);
	private:
		char _wifiSSID[HUB_AP_WIFI_SSID_SIZE + 1] = {0};
		char _wifiPass[HUB_AP_WIFI_PASS_SIZE + 1] = {0};
		char _hubAddr[HUB_AP_ADDR_SIZE + 1] = {0};
		char _hubKey[HUB_AP_HUBKEY_SIZE + 1] = {0};
		char _secKey[HUB_AP_SECKEY_SIZE + 1] = {0};
		char _card[HUB_AP_CARD_NUM][HUB_AP_CARD_SIZE + 1] = {{0}};
};
static ConfigClass Config;

void printHex(char *buffer, int bufferSize);
#endif // HUB_AP_CONFIG
