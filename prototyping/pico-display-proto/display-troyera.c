#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "display-troyera.h"

const uint8_t DC_PIN = 20;
const uint8_t LED_PIN = 25;
const uint8_t RS_PIN = 21;
static uint8_t blCount = 0;
const uint8_t colstart = 2;
const uint8_t rowstart = 1;


void displayInit(const uint8_t *addr);
void sendCommand(uint8_t cmdByte, const uint8_t *dataBytes, uint8_t numDataBytes);
void cs_select(void);
void cs_deselect(void);
void dc_select(void);
void dc_deselect(void);
void rst_select(void);
void rst_deselect(void);
void ledBlink(uint16_t onTime, uint16_t offTime);
void drawPixelTest(int16_t x, int16_t y, int16_t h, int16_t w, uint16_t color);
    

//Ok. This is gonna look like a mess but I'm just trying to get it to work by looking at how adafruit did it.


static const uint8_t 
    cmd1[] = {
    15,                         //15 commands in this initialization
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
    0x05 },
    
    cmd2[] = {
    2,
    CASET, 4,
    0x00, 0x00,
    0x00, 0x7F,
    RASET, 4,
    0x00, 0x00,
    0x00, 0x7F
 },
    cmd3[] = {
    4,
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





int main() {
    //bi_decl(bi_program_description("This is a test binary."));
    //bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));
    stdio_init_all();
 
    //Led initialization
    gpio_init(LED_PIN);
    
    gpio_set_dir(LED_PIN, GPIO_OUT);

    spi_init(spi_default, 32000000); //3200000
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
    rst_deselect();
    
    sleep_ms(1000); //Sleeping for a sec just in case the display needs it.
                    //unsure if this is necessary
    displayInit(cmd1);
    displayInit(cmd2);
    displayInit(cmd3);

    //drawPixelTest(50, 50, 10, 10, 0xF800);
  
    /*  
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            drawPixelTest(i, j, (uint16_t)0xF800);
        }
    }*/
    
    /*
    for(int i = 0; i < 126; i++){
        drawPixelTest(i, 20, (uint16_t)0xF800);
    }*/
    //drawPixelTest(0, 0, (uint16_t)0xF800);
    
    //Supposedly the display is ready to display shit
    while(1){
        for(int i = 0; i < WIDTH; i++){
            for(int j = 0; j < HEIGHT; j++){
                drawPixelTest(i, j, 1, 1, (uint16_t)0x0000);
            }
        }
        sleep_ms(50);
        for(int i = 0; i < WIDTH; i++){
            for(int j = 0; j < HEIGHT; j++){
                drawPixelTest(i, j, 1, 1, (uint16_t)0xFFFF);
            }
        }
        /*
        displayInit(cmd1);
        displayInit(cmd2);
        displayInit(cmd3);
        */
        /*
        drawPixelTest((int16_t)0x003e, (int16_t)0x0050, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0051, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0052, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0053, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0054, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0055, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0056, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0057, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0058, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0059, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x005A, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x005b, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x005c, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x005d, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x005e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x005f, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0060, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0061, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0062, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0063, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0064, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0065, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0066, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0067, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0068, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x0069, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x006A, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x006b, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x006c, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x006d, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x006e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x003e, (int16_t)0x006f, 1, 1, (uint16_t)0x0000);

        drawPixelTest((int16_t)0x0040, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0041, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0042, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0043, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0044, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0045, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0046, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0047, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0048, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x0049, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x004A, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x004b, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x004c, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x004d, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x004e, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        drawPixelTest((int16_t)0x004f, (int16_t)0x004e, 1, 1, (uint16_t)0x0000);
        */
        
        //sleep_ms(1000);
        //sendCommand(NOP, 0, 0);
        //sendCommand(INVON, 0, 0);
        //sleep_ms(1000);
        //ledBlink(50, 50);
        //sendCommand(INVOFF, 0, 0);
        //sleep_ms(1000);
        //drawPixelTest(20, 20, 0xF800);
    }
}

void ledBlink(uint16_t onTime, uint16_t offTime){
        gpio_put(LED_PIN, 1);
        sleep_ms(onTime);
        gpio_put(LED_PIN, 0);
        sleep_ms(offTime);
}

void ledCountDisp(uint16_t blinks){
    uint8_t manyBlinks = 30;
    if(blinks == 0){
        while(manyBlinks--){
            ledBlink(25,25);
        }
    }else{
            ledBlink(2000, 50);
        while(blinks--){
            ledBlink(250, 250);
        }
    }
}

void displayInit(const uint8_t *addr){
    const uint8_t *l_addr = addr;
    uint8_t numCommands;
    uint8_t cmd;
    uint8_t numArgs;
    uint16_t ms;

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
    //sleep_ms(10);
}
void cs_deselect(void) {
    asm volatile("nop \n nop \n nop \n"); //Just copying what the sdk does
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); //Active low
    asm volatile("nop \n nop \n nop \n");
    //sleep_ms(10);
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

void drawPixelTest(int16_t x, int16_t y, int16_t h, int16_t w, uint16_t color){
    //TODO make these arrays instead of uint32_t's just like you input them in sendCommand
    uint32_t xa;
    uint32_t ya;
    
    //x += colstart;
    //y += rowstart;
    //xa = (((uint32_t)0x99 )<< 8) | ((x + w - 1)); 
    //ya = (((uint32_t)0x99 )<< 8) | ((y + h - 1)); 

    xa = ((uint32_t)x << 24) | ((x + w - 1) << 8);
    //xa |= ((x + w - 1)); 

    ya = ((uint32_t)y << 24) | ((y + h - 1) << 8);
    //ya |= ((y + h - 1)); 

    sendCommand((uint8_t)CASET, (uint8_t *)&xa, 4);

    sendCommand((uint8_t)RASET, (uint8_t *)&ya, 4);

    sendCommand((uint8_t)RAMWR, (uint8_t *)&color, 2);
    sendCommand((uint8_t)NOP, 0, 0);
    sleep_us(1);

    //cs_select();
    //spi_write_blocking(spi_default, (uint8_t *)&color, 2);
    //cs_deselect();

    //sendCommand((uint8_t)INVON, 0, 0);
    //sendCommand((uint8_t)INVOFF, 0, 0);
}
