#define PN532_PREAMBLE (uint8_t)0x00   // Command sequence start, byte 1/3
#define PN532_STARTCODE1 (uint8_t)0x00 // Command sequence start, byte 2/3
#define PN532_STARTCODE2 (uint8_t)0xFF // Command sequence start, byte 3/3
#define PN532_POSTAMBLE (uint8_t)0x00  // EOD

#define PN532_HOSTTOPN532 (uint8_t)0xD4 // Host-to-PN532
#define PN532_PN532TOHOST (uint8_t)0xD5 // PN532-to-host

#define PN532_COMMAND_GETFIRMWAREVERSION (uint8_t)0x02
