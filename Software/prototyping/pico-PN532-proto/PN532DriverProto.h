#define PN532_PREAMBLE 0x00   // Command sequence start, byte 1/3
#define PN532_STARTCODE1 0x00 // Command sequence start, byte 2/3
#define PN532_STARTCODE2 0xFF // Command sequence start, byte 3/3
#define PN532_POSTAMBLE 0x00  // EOD

#define PN532_HOSTTOPN532 0xD4 // Host-to-PN532
#define PN532_PN532TOHOST 0xD5 // PN532-to-host

#define PN532_COMMAND_GETFIRMWAREVERSION 0x02
