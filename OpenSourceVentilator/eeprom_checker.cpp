#include "eeprom_checker.h"

#define CRC_STORE_INX     0     /*<< Index in EEPROM is where CRC will be located */
#define CRC_LEN           4     /*<< CRC length */

const unsigned long crc_table[16] = {
0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

static unsigned long eeprom_crc(EEPROMClass& eeprom) {
  unsigned long crc = ~0L;

  for (int index = 0 ; index < eeprom.length()  ; ++index) {
    if ( CRC_STORE_INX <= index && index < CRC_STORE_INX + CRC_LEN) continue; 
    crc = crc_table[(crc ^ eeprom[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (eeprom[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

unsigned long eeprom_write_crc(EEPROMClass& eeprom) { 
  unsigned long crc = eeprom_crc(eeprom); 
  eeprom.put(CRC_STORE_INX, crc);
  return crc;
}

bool eeprom_test_crc(EEPROMClass& eeprom) {
  unsigned long crc; 
  eeprom.get(CRC_STORE_INX, crc); 
  return crc == eeprom_crc(eeprom); 
}
