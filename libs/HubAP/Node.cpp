#include "Node.h"

// Public
NodeClass::NodeClass(ConfigClass *config) {
	Config = config;
}

uint8_t NodeClass::wait() {
	return HUB_AP_STATE_ACCEPT;
}

int NodeClass::card(char *uid, char *apid) {
	char buf[15 + sizeof(uid)];
	sprintf(buf, "uid=%s&ap=%s", uid, apid);
	return HUB_AP_STATE_WAIT;
}

