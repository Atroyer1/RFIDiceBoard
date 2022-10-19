#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "tftdisplay.h"


//private functions


void sendCommand(uint8_t cmdByte, const uint8_t *dataBytes, uint8_t numDataBytes);
void cs_select(void);
void cs_deselect(void);
void dc_select(void);
void dc_deselect(void);
void rst_select(void);
void rst_deselect(void);

/************************************************************************************/
//Display initialization commands stored in an array for easy step-through
static const uint8_t 
    init_commands[] = {
    21,                         //21 commands in this initialization
    SWRESET, ST_CMD_DELAY,
    150,
    SLPOUT, ST_CMD_DELAY,
    255,
    FRMCTR1, 3,
    0x01, 0x2C, 0x2D,
    FRMCTR2, 3,
    0x01, 0x2C, 0x2D,
    FRMCTR3, 6,
    0x01, 0x2C, 0x2D,
    0x01, 0x2C, 0x2D,
    INVCTR, 1,
    0x07,
    PWCTR1, 3,
    0xA2,
    0x02,
    0x84,
    PWCTR2, 1,
    0xC5,
    PWCTR3, 2,
    0x0A,
    0x00,
    PWCTR4, 2,
    0x8A,
    0x2A,
    PWCTR5, 2,
    0x8A, 0xEE,
    VMCTR1, 1,
    0x0E,    
    INVOFF, 0,
    MADCTL, 1,
    0xC8,
    COLMOD, 1,
    0x05,
    CASET, 4,
    0x00, 0x00,
    0x00, 0x7F,
    RASET, 4,
    0x00, 0x00,
    0x00, 0x7F,
    GMCTRP1, 16,
    0x02, 0x1c, 0x07, 0x12,
    0x37, 0x32, 0x29, 0x2d,
    0x29, 0x25, 0x2B, 0x39,
    0x00, 0x01, 0x03, 0x10,
    GMCTRN1, 16,
    0x03, 0x1d, 0x07, 0x06,
    0x2E, 0x2C, 0x29, 0x2D,
    0x2E, 0x2E, 0x37, 0x3F,
    0x00, 0x00, 0x02, 0x10,
    NORON, ST_CMD_DELAY,
    10,
    DISPON, ST_CMD_DELAY,
    100
};

/************************************************************************************/

//initialization
void tft_init(void){
    const uint8_t *l_addr = init_commands;
    uint8_t numCommands;
    uint8_t cmd;
    uint8_t numArgs;
    uint16_t ms;

    stdio_init_all();
    sleep_ms(10);
    spi_init(spi_default, 32000000);

    //SCK, TX, CSN, DC, and RS pin initalization
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI); 
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    gpio_init(DC_PIN);
    gpio_set_dir(DC_PIN, GPIO_OUT);
    gpio_put(DC_PIN, 1);

    gpio_init(RS_PIN);
    gpio_set_dir(RS_PIN, GPIO_OUT);
    gpio_put(RS_PIN, 1);


    rst_select();
    sleep_ms(1000);
    rst_deselect();
    sleep_ms(1000);
    //Initialization for display commands loop

    numCommands = l_addr[0];
    l_addr++;
    while(numCommands)  {
        cmd = *l_addr;
        l_addr++;
        numArgs = *l_addr;
        l_addr++;
        ms = numArgs & ST_CMD_DELAY;
        numArgs &= ~ST_CMD_DELAY;
        sendCommand(cmd, l_addr, numArgs);
        l_addr += numArgs;
        
        if(ms) {
            ms = l_addr[0];
            l_addr++;
            if(ms == 255){
                ms = 500;
            }else{}
            sleep_ms(ms);
        }
        numCommands--;
    }
    
    /************************************************************************************/
    //Testing if I need to send something to the display to have it actually start up!
    /************************************************************************************/

    /*
    sendCommand((uint8_t)RASET, 0x00010001, 4);
    sendCommand((uint8_t)CASET, 0x00010001, 4);

    sendCommand((uint8_t)RAMWR, 0x0000, 32);
    sendCommand((uint8_t)NOP, 0, 0);
    */

    /************************************************************************************/
    /************************************************************************************/

}

void sendCommand(uint8_t cmdByte, const uint8_t *dataBytes, uint8_t numDataBytes){
    cs_select();

    dc_select();
    spi_write_blocking(spi_default, (const uint8_t *)&cmdByte, 1);
    dc_deselect();
    
    if(numDataBytes){
        spi_write_blocking(spi_default, dataBytes, numDataBytes);
    }
    cs_deselect();
}

void cs_select(void) {
    asm volatile("nop \n nop \n nop \n"); //Just copying what the sdk does
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0); //Active low
    asm volatile("nop \n nop \n nop \n");
}

void cs_deselect(void) {
    asm volatile("nop \n nop \n nop \n"); //Just copying what the sdk does
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); //Active low
    asm volatile("nop \n nop \n nop \n");
}

void dc_select(void) {
    asm volatile("nop \n nop \n nop \n"); //Just copying what the sdk does
    gpio_put(DC_PIN, 0);
    asm volatile("nop \n nop \n nop \n");
}

void dc_deselect(void) {
    asm volatile("nop \n nop \n nop \n"); //Just copying what the sdk does
    gpio_put(DC_PIN, 1); 
    asm volatile("nop \n nop \n nop \n");
}

void rst_select(void) {
    asm volatile("nop \n nop \n nop \n"); //Just copying what the sdk does
    gpio_put(RS_PIN, 0);
    asm volatile("nop \n nop \n nop \n");
}

void rst_deselect(void) {
    asm volatile("nop \n nop \n nop \n"); //Just copying what the sdk does
    gpio_put(RS_PIN, 1); 
    asm volatile("nop \n nop \n nop \n");
}
