void RFID_Task(void);
void pn532_init(void);

#define PN532_IRQ_PIN 3

#define D4UID 4//306399841
#define D6UID 0xf2522b61
#define D8UID 1386753121
#define D10UID 1387080801
#define D12UID 2
#define D20UID 3
#define D100UID 306399841

#define PN532_PREAMBLE (uint8_t)0x00   // Command sequence start, byte 1/3
#define PN532_STARTCODE1 (uint8_t)0x00 // Command sequence start, byte 2/3
#define PN532_STARTCODE2 (uint8_t)0xFF // Command sequence start, byte 3/3
#define PN532_POSTAMBLE (uint8_t)0x00  // EOD

#define PN532_HOSTTOPN532 (uint8_t)0xD4 // Host-to-PN532
#define PN532_PN532TOHOST (uint8_t)0xD5 // PN532-to-host

#define PN532_COMMAND_DIAGNOSE (0x00)              ///< Diagnose
#define PN532_COMMAND_GETFIRMWAREVERSION (0x02)    ///< Get firmware version
#define PN532_COMMAND_GETGENERALSTATUS (0x04)      ///< Get general status
#define PN532_COMMAND_READREGISTER (0x06)          ///< Read register
#define PN532_COMMAND_WRITEREGISTER (0x08)         ///< Write register
#define PN532_COMMAND_READGPIO (0x0C)              ///< Read GPIO
#define PN532_COMMAND_WRITEGPIO (0x0E)             ///< Write GPIO
#define PN532_COMMAND_SETSERIALBAUDRATE (0x10)     ///< Set serial baud rate
#define PN532_COMMAND_SETPARAMETERS (0x12)         ///< Set parameters
#define PN532_COMMAND_SAMCONFIGURATION (0x14)      ///< SAM configuration
#define PN532_COMMAND_POWERDOWN (0x16)             ///< Power down
#define PN532_COMMAND_RFCONFIGURATION (0x32)       ///< RF config
#define PN532_COMMAND_RFREGULATIONTEST (0x58)      ///< RF regulation test
#define PN532_COMMAND_INJUMPFORDEP (0x56)          ///< Jump for DEP
#define PN532_COMMAND_INJUMPFORPSL (0x46)          ///< Jump for PSL
#define PN532_COMMAND_INLISTPASSIVETARGET (0x4A)   ///< List passive target
#define PN532_COMMAND_INATR (0x50)                 ///< ATR
#define PN532_COMMAND_INPSL (0x4E)                 ///< PSL
#define PN532_COMMAND_INDATAEXCHANGE (0x40)        ///< Data exchange
#define PN532_COMMAND_INCOMMUNICATETHRU (0x42)     ///< Communicate through
#define PN532_COMMAND_INDESELECT (0x44)            ///< Deselect
#define PN532_COMMAND_INRELEASE (0x52)             ///< Release
#define PN532_COMMAND_INSELECT (0x54)              ///< Select
#define PN532_COMMAND_INAUTOPOLL (0x60)            ///< Auto poll
#define PN532_COMMAND_TGINITASTARGET (0x8C)        ///< Init as target
#define PN532_COMMAND_TGSETGENERALBYTES (0x92)     ///< Set general bytes
#define PN532_COMMAND_TGGETDATA (0x86)             ///< Get data
#define PN532_COMMAND_TGSETDATA (0x8E)             ///< Set data
#define PN532_COMMAND_TGSETMETADATA (0x94)         ///< Set metadata
#define PN532_COMMAND_TGGETINITIATORCOMMAND (0x88) ///< Get initiator command
#define PN532_COMMAND_TGRESPONSETOINITIATOR (0x90) ///< Response to initiator
#define PN532_COMMAND_TGGETTARGETSTATUS (0x8A)     ///< Get target status
