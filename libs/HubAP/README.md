Basic Concept
=============

AP = Access Point, i.e. the controller of devices
HUB = Centralize service
APID = Each AP uses their MAC address as the Access Point identify.
PNID = Each individual uses their NUID from any RFID card as the personal identity.
DT = Date and time

AP communicates with HUB for:
 1. Check the register status of AP
 2. Send tuple (APID, PNID) and receive permit
 3. Send logs

AP can work under offline mode:
 1. Use EEPROM to store any data
 2. Read the offline cache from EEPROM
 3. When the AP is in the offline mode, the root NUID can be used anytime. (TODO: Should it be hard-code in the sketch?)

The access permission is matched by the triple group (APID, PNID, DT).

TODO: EEPROM/FS structure, cache and transaction data
TODO: Built-in Online/Offline status
TODO: Common API design

State
=====

AP has 3 states:

 1. Setup
 2. Online
 3. Offline

Setup
-----

Setup mode only can be accessed when the AP is reset.
It will open a WiFi AP with SSID: GHub, Passphrase: gribblehirst, and open an HTTP server to receive config options.

TODO: Onboard config server API

Online
---------

When the WiFi is connected, AP enters online mode.
In this mode, the AP will read and write to the HUB directly.
At the meanwhile, it also writes to the local cache as well.
When entering the online mode, AP will send all cached logs to HUB and clean them from local storage.

Offline
---------

When the WiFi is disconnected, AP enters offline mode.
In this mode, the AP will read and write to the cache directly.
At the meanwhile, it also tries to reconnect the WiFi.
