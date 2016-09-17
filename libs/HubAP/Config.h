#ifndef HUB_AP_CONFIG
#define HUB_AP_CONFIG

#define HUB_AP_WIFI_SSID "MeshedNET"
// #define HUB_AP_WIFI_SSID "GHub"
#define HUB_AP_WIFI_PASS "rfidrfidrfid"
#define HUB_AP_SERIAL_SPEED 115200

#define HUB_AP_WIFI_SSID_OFFSET	0
#define HUB_AP_WIFI_SSID_SIZE	32
#define HUB_AP_WIFI_PASS_OFFSET HUB_AP_WIFI_SSID_OFFSET + HUB_AP_WIFI_SSID_SIZE //32
#define HUB_AP_WIFI_PASS_SIZE	64

#define HUB_AP_ADDR_OFFSET HUB_AP_WIFI_PASS_OFFSET + HUB_AP_WIFI_PASS_SIZE // 96
#define HUB_AP_ADDR_SIZE 128
#define HUB_AP_PORT_OFFSET HUB_AP_ADDR_OFFSET + HUB_AP_ADDR_SIZE // 96
#define HUB_AP_PORT_SIZE 5
#define HUB_AP_PATH_OFFSET HUB_AP_PORT_OFFSET + HUB_AP_PORT_SIZE // 96
#define HUB_AP_PATH_SIZE 128

#define HUB_AP_HUBKEY_OFFSET HUB_AP_PATH_OFFSET + HUB_AP_PATH_SIZE
#define HUB_AP_HUBKEY_SIZE 59
#define HUB_AP_SECKEY_OFFSET HUB_AP_HUBKEY_OFFSET + HUB_AP_HUBKEY_SIZE
#define HUB_AP_SECKEY_SIZE 16
#define HUB_AP_CARD_OFFSET HUB_AP_SECKEY_OFFSET + HUB_AP_SECKEY_SIZE
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

#define HUB_AP_EEPROM_SIZE HUB_AP_CARD_OFFSET + \
	( HUB_AP_CARD_SIZE * HUB_AP_CARD_NUM ) // 320 = 256 + ( 16 * 4 )

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
	
		void setHubAddr(const char*);
		void setHubPort(const char*);
		void setHubPath(const char*);
	
		void setHubKey(const char*);
		void setSecKey(const char*);
		void setCard(int, const char*);
	
		void getSSID(char *);
		void getPass(char *);
		
		void getHubAddr(char *);
		void getHubPort(char *);
		void getHubPath(char *);

		void getHubKey(char *);
		void getSecKey(char *);
		void getCard(int, char *);

		bool matchCard(int, char [HUB_AP_CARD_SIZE], int);
	private:
		char _wifiSSID[HUB_AP_WIFI_SSID_SIZE + 1] = {0};
		char _wifiPass[HUB_AP_WIFI_PASS_SIZE + 1] = {0};

		char _hubAddr[HUB_AP_ADDR_SIZE + 1] = {0};
		char _hubPort[HUB_AP_PORT_SIZE + 1] = {0};
		char _hubPath[HUB_AP_PATH_SIZE + 1] = {0};
		
		char _hubKey[HUB_AP_HUBKEY_SIZE + 1] = {0};
		char _secKey[HUB_AP_SECKEY_SIZE + 1] = {0};
		char _card[HUB_AP_CARD_NUM][HUB_AP_CARD_SIZE + 1] = {{0}};
};
void printHex(char *buffer, int bufferSize);
#endif // HUB_AP_CONFIG
