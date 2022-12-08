//public functions 

void tft_init(void);
void drawPixel(uint8_t x, uint8_t y, uint16_t color);
void drawLetter(uint32_t letter, uint8_t x, uint8_t y, uint16_t color, uint16_t background);
void drawBackground(uint16_t color);
uint8_t drawString(uint8_t *string, uint8_t x, uint8_t y, uint16_t color, uint16_t background);
bool numToString(uint32_t num, uint8_t *ret_string, uint8_t ret_string_len);

//Predefined pins

#define DC_PIN 21
#define RS_PIN 20


//List of ST7735 Commands

#define ST_CMD_DELAY 0x80
#define HEIGHT 160
#define WIDTH 128

#define NOP 0x00
#define SWRESET 0x01
#define RDDID 0x04
#define RDDST 0x09

#define SLPIN 0x10
#define SLPOUT 0x11
#define PTLON 0x12
#define NORON 0x13

#define INVOFF 0x20
#define INVON 0x21
#define DISPOFF 0x28
#define DISPON 0x29
#define CASET 0x2A
#define RASET 0x2B
#define RAMWR 0x2C
#define RAMRD 0x2E

#define PTLAR 0x30
#define TEOFF 0x34
#define TEON 0x35
#define MADCTL 0x36
#define COLMOD 0x3A 

#define MADTCL_MY 0x80
#define MADCTL_MX 0x40
#define MADCTL_MV 0x20
#define MATCTL_ML 0x10 
#define MADCTL_RGB 0x00 

#define RDID1 0xDA 
#define RDID2 0xDB 
#define RDID3 0xDC 
#define RDID4 0xDD 

#define FRMCTR1 0xB1 
#define FRMCTR2 0xB2
#define FRMCTR3 0xB3 
#define INVCTR 0xB4 
#define DISSET5 0xB6

#define PWCTR1 0xC0 
#define PWCTR2 0xC1 
#define PWCTR3 0xC2 
#define PWCTR4 0xC3 
#define PWCTR5 0xC4 
#define VMCTR1 0xC5

#define PWCTR6 0xFC

#define VMOFCTR 0xC7
#define WRID2 0xD1 

#define GMCTRP1 0xE0
#define GMCTRN1 0xE1

//Pre-defined 565 colors
#define ST77XX_BLACK 0x0000
#define ST77XX_WHITE 0xFFFF
#define ST77XX_RED 0xF800
#define ST77XX_GREEN 0x07E0
#define ST77XX_BLUE 0x001F
#define ST77XX_CYAN 0x07FF
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_YELLOW 0xFFE0
#define ST77XX_ORANGE 0xFC00


//Pixel letters defined in a 25 bit format

#define l_A 0b0010001010011100101001010 
#define l_B 0b0110001010011000101001100
#define l_C 0b0010001010010000101000100
#define l_D 0b0110001010010100101001100
#define l_E 0b0111001000011100100001110
#define l_F 0b0111001000011000100001000
#define l_G 0b0111010000101101001001110
#define l_H 0b0101001010011100101001010
#define l_I 0b0111000100001000010001110
#define l_J 0b0001000010000101001001100
#define l_K 0b1001010100110001010010010
#define l_L 0b0100001000010000100001110
#define l_M 0b1000111111101011000110001
#define l_N 0b1001010010110101011010010
#define l_O 0b0010001010010100101000100
#define l_P 0b1110010010111001000010000
#define l_Q 0b0110010010100101011000111
#define l_R 0b1110010010111001010010010
#define l_S 0b0111010000011000001011100
#define l_T 0b0111000100001000010000100
#define l_U 0b1001010010100101001001110
#define l_V 0b0101001010010100101000100
#define l_W 0b1000110001100011010101010
#define l_X 0b0101001010001000101001010
#define l_Y 0b0101001010011100010000100
#define l_Z 0b1111000010001000100011110

#define l_0 0b0110010110111101101001100
#define l_1 0b0010001100001000010001110
#define l_2 0b1110000010111101000011110
#define l_3 0b1110000010011000001011100
#define l_4 0b1001010010011100001000010
#define l_5 0b1111010000111100001011100
#define l_6 0b0110010000111001001001100
#define l_7 0b1111000010001000010000100
#define l_8 0b0110010010011001001001100
#define l_9 0b0110010010011100001001100

#define l_space 0b0000000000000000000000000
#define l_not_a_letter 0b1111111111111111111111111