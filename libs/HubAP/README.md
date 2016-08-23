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

TODO: EEPROM structure, cache and transaction data
TODO: Built-in Online/Offline status
TODO: Common API design
