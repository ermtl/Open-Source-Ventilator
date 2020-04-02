#include <EEPROM.h>

unsigned long eeprom_write_crc(EEPROMClass& eeprom);

bool eeprom_test_crc(EEPROMClass& eeprom);
