


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


#define ST77XX_BLACK 0x0000
#define ST77XX_WHITE 0xFFFF
#define ST77XX_RED 0xF800
#define ST77XX_GREEN 0x07E0
#define ST77XX_BLUE 0x001F
#define ST77XX_CYAN 0x07FF
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_YELLOW 0xFFE0
#define ST77XX_ORANGE 0xFC00


/*************/
//Time for me to figure out letters I guess

#define l_A 0x4000 | 0b010101111101
