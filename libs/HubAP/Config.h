#ifndef HUB_AP_CONFIG
#define HUB_AP_CONFIG

#define HUB_AP_WIFI_SSID "GHub-SetupService"
// #define HUB_AP_WIFI_SSID "GHub"
#define HUB_AP_WIFI_PASS "Gis4Great"
#define HUB_AP_SERIAL_SPEED 9600

// WiFi
#define HUB_AP_WIFI_SSID_OFFSET	0
#define HUB_AP_WIFI_SSID_SIZE	32
#define HUB_AP_WIFI_PASS_OFFSET HUB_AP_WIFI_SSID_OFFSET + HUB_AP_WIFI_SSID_SIZE
#define HUB_AP_WIFI_PASS_SIZE	32

// MQTT
#define HUB_AP_MQTT_ADDR_OFFSET HUB_AP_WIFI_PASS_OFFSET + HUB_AP_WIFI_PASS_SIZE
#define HUB_AP_MQTT_ADDR_SIZE 64
#define HUB_AP_MQTT_PORT_OFFSET HUB_AP_MQTT_ADDR_OFFSET + HUB_AP_MQTT_ADDR_SIZE
#define HUB_AP_MQTT_PORT_SIZE 5
#define HUB_AP_MQTT_USER_OFFSET HUB_AP_MQTT_PORT_OFFSET + HUB_AP_MQTT_PORT_SIZE
#define HUB_AP_MQTT_USER_SIZE 32
#define HUB_AP_MQTT_PASS_OFFSET HUB_AP_MQTT_USER_OFFSET + HUB_AP_MQTT_USER_SIZE
#define HUB_AP_MQTT_PASS_SIZE 32
#define HUB_AP_MQTT_TOPIC_OFFSET HUB_AP_MQTT_PASS_OFFSET + HUB_AP_MQTT_PASS_SIZE
#define HUB_AP_MQTT_TOPIC_SIZE 32
#define HUB_AP_MQTT_CLIENTID_OFFSET HUB_AP_MQTT_TOPIC_OFFSET + HUB_AP_MQTT_TOPIC_SIZE
#define HUB_AP_MQTT_CLIENTID_SIZE 32

// Card
#define HUB_AP_CARD_OFFSET HUB_AP_MQTT_PASS_OFFSET + HUB_AP_MQTT_PASS_SIZE
#define HUB_AP_CARD_SIZE 16
#define HUB_AP_CARD_NUM 4

// not working state
#define HUB_AP_STATE_NONE	0x00
// error state
#define HUB_AP_STATE_ERROR	0x01
// enter setup service
#define HUB_AP_STATE_SETUP	0x10
// RFID
#define HUB_AP_STATE_RFID	0x20
#define HUB_AP_STATE_ACCEPT	0x30
#define HUB_AP_STATE_DENY	0x31
#define HUB_AP_STATE_WAIT	0x32
#define HUB_AP_STATE_TIMEOUT 0x33

#define HUB_AP_EEPROM_SIZE HUB_AP_CARD_OFFSET + \
	( HUB_AP_CARD_SIZE * HUB_AP_CARD_NUM )

// led
#define HUB_AP_LED LED_BUILTIN

#include <SPI.h>
#include <EEPROM.h>

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
	
		void setMQTTAddr(const char*);
		void setMQTTPort(const char*);
		void setMQTTUser(const char*);
		void setMQTTPass(const char*);
		void setMQTTTopic(const char*);
		void setMQTTClientId(const char*);

		void setCard(int, const char*);
	
		void getSSID(char *);
		void getPass(char *);
		
		void getMQTTAddr(char *);
		void getMQTTPort(char *);
		void getMQTTUser(char *);
		void getMQTTPass(char *);
		void getMQTTTopic(char*);
		void getMQTTClientId(char*);

		void getCard(int, char *);

		bool matchCard(int, char [HUB_AP_CARD_SIZE], int);
	private:
		char _wifiSSID[HUB_AP_WIFI_SSID_SIZE + 1] = {0};
		char _wifiPass[HUB_AP_WIFI_PASS_SIZE + 1] = {0};

		char _mqttAddr[HUB_AP_MQTT_ADDR_SIZE + 1] = {0};
		char _mqttPort[HUB_AP_MQTT_PASS_SIZE + 1] = {0};	
		char _mqttUser[HUB_AP_MQTT_USER_SIZE + 1] = {0};
		char _mqttPass[HUB_AP_MQTT_PASS_SIZE + 1] = {0};
		char _mqttTopic[HUB_AP_MQTT_TOPIC_SIZE + 1] = {0};
		char _mqttClientId[HUB_AP_MQTT_CLIENTID_SIZE + 1] = {0};

		char _card[HUB_AP_CARD_NUM][HUB_AP_CARD_SIZE + 1] = {{0}};
};
void printHex(char *buffer, int bufferSize);
String sprintHex(char *buffer, int bufferSize);
void gen_random(char *s, const int len);
#endif // HUB_AP_CONFIG
