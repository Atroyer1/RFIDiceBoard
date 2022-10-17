#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "display-troyera.h"

const uint8_t DC_PIN = 21;
const uint8_t LED_PIN = 25;
const uint8_t RS_PIN = 20;
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
    
void drawLetterTest(uint32_t letter, int16_t x, int16_t y, uint16_t color, uint16_t background);

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
    uint16_t colorNum = 0;
    uint16_t cornerColor = 0x00FF;

    uint16_t testVar;
    uint32_t testVar2;

    uint8_t testArray[] = {0, 0, 0, HEIGHT, 0, 0, 0, WIDTH};
    //uint8_t testArray[] = {54, 0, 55, 0, 70, 0, 71, 0};

    uint8_t colorArray[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
                           };

 

    /****INITIALIZATION****************/
    /*************************************************************************************/
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
    sleep_ms(1000);
    rst_deselect();
    
    sleep_ms(1000); //Sleeping for a sec just in case the display needs it.
                    //unsure if this is necessary
    displayInit(cmd1);
    displayInit(cmd2);
    displayInit(cmd3);

    /****END_INITIALIZATION****************/
    /*************************************************************************************/
  
    
    //Supposedly the display is ready to display

    for(int i = 1; i <= WIDTH; i++){
        for(int j = 1; j <= HEIGHT; j++){
            drawPixelTest(i, j, 1, 1, (uint16_t)0x0000);
        }
    }
    /*************************************************************
    drawPixelTest(50, 50, 1, 1, (uint16_t)0xFF00);

    //Row is the longways
    sendCommand((uint8_t)RASET, testArray, 4);
    //Column is the shortways
    sendCommand((uint8_t)CASET, testArray + 4, 4);

    //Have to send RAMWR with all the color data to draw a big 'ol rectangle

    sendCommand((uint8_t)RAMWR, colorArray, 32);
    sendCommand((uint8_t)NOP, 0, 0);

    while(1){//PAUSE
    }
    *************************************************************/

    drawLetterTest(l_A, 5, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_B, 11, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_C, 17, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_D, 23, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_E, 29, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_F, 35, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_G, 41, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_H, 47, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_I, 55, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_J, 61, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_K, 67, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_L, 73, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_M, 79, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_N, 85, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_O, 91, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_P, 97, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_Q, 105, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_R, 111, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_S, 117, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_T, 123, 5, 0xFFFF, 0x0000);
    drawLetterTest(l_U, 5, 11, 0xFFFF, 0x0000);
    drawLetterTest(l_V, 11, 11, 0xFFFF, 0x0000);
    drawLetterTest(l_W, 17, 11, 0xFFFF, 0x0000);
    drawLetterTest(l_X, 23, 11, 0xFFFF, 0x0000);
    drawLetterTest(l_Y, 29, 11, 0xFFFF, 0x0000);
    drawLetterTest(l_Z, 35, 11, 0xFFFF, 0x0000);

    drawLetterTest(l_0, 5, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_1, 11, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_2, 17, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_3, 23, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_4, 29, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_5, 35, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_6, 41, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_7, 47, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_8, 55, 17, 0xFFFF, 0x0000);
    drawLetterTest(l_9, 61, 17, 0xFFFF, 0x0000);


    /*****************************************************************
    testVar = 50;

    testVar2 = ( ( ((uint32_t)testVar) << 24 ) | ( ((testVar)) << 8) );

    //uint32_t ya = ((uint32_t)testVar << 24) | ((testVar + 2) << 8);


    //Set the Colomn and row
    sendCommand((uint8_t)CASET, (uint8_t *)&testVar2, 4);
    sendCommand((uint8_t)RASET, (uint8_t *)&testVar2, 4);

    testVar = 0xFFFF;

    sendCommand((uint8_t)RAMWR, (uint8_t *)&testVar, 2);
    sendCommand((uint8_t)NOP, 0, 0);

    while(1){//PAUSE
    }
     ****************************************************************/

    colorNum = 0xFFFF;
    while(1){
        //colorNum = colorNum + 1;
        colorNum = ~colorNum;
        cornerColor = (0xFF00 & (~cornerColor));

        //This perfectly spans the x-direction (COLUMN direction)
        drawPixelTest(1, 1, 2, 2, (uint16_t)cornerColor);
        for(int i = 3; i <= WIDTH - 2; i++){
            drawPixelTest(i, 1, 1, 1, (uint16_t)colorNum);
            sleep_ms(1);
        }
        drawPixelTest(WIDTH - 2, 1, 2, 2, (uint16_t)cornerColor);

        for(int i = 1; i <= HEIGHT; i++){
            //drawPixelTest(1, i, 1, 1, (uint16_t)colorNum);
            sleep_ms(1);
        }

        for(int i = HEIGHT + 1; i > 0; i--){
            //drawPixelTest(i, HEIGHT, 1, 1, (uint16_t)~colorNum);
            sleep_ms(1);
        }

        for(int i = WIDTH + 1; i > 0; i--){
            //drawPixelTest(WIDTH, i, 1, 1, (uint16_t)~colorNum);
            sleep_ms(1);
        }


        //Color the entire screen, pixel by pixel

        /***********************************************************************
        for(int i = 0; i < WIDTH; i++){
            for(int j = 0; j < HEIGHT; j++){
                drawPixelTest(i, j, 1, 1, (uint16_t)colorNum);
            }
        }
        sleep_ms(100);
        for(int i = 0; i < WIDTH; i++){
            for(int j = 0; j < HEIGHT; j++){
                drawPixelTest(i, j, 1, 1, (uint16_t)(~colorNum));
            }
        }

        ***********************************************************************/
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
    //const uint8_t coords[] = {(uint32_t)x, (uint32_t)y};
    uint16_t x1 = x - 1;
    uint16_t y1 = y - 1;
    
    uint32_t xa = ((uint32_t)x1 << 24) | ((x1 + w - 1) << 8);

    uint32_t ya = ((uint32_t)y1 << 24) | ((y1 + h - 1) << 8);


    //Set the Colomn and row
    sendCommand((uint8_t)CASET, (uint8_t *)&xa, 4);
    sendCommand((uint8_t)RASET, (uint8_t *)&ya, 4);

    sendCommand((uint8_t)RAMWR, (uint8_t *)&color, 2);
    sendCommand((uint8_t)NOP, 0, 0);
    sleep_us(1);

}

void drawLetterTest(uint32_t letter, int16_t x, int16_t y, uint16_t color, uint16_t background){
    uint32_t l_letter = letter;
    uint8_t coords[] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t pixel_counter = 0;
    uint8_t x_pos = 0;
    uint8_t y_pos = 0;
    
    //So in this version all letters are 5x5 pixels
    while(pixel_counter <= 24){
        //Set the coordinates and send the information for the first pixel
        coords[1] = y + y_pos;
        coords[3] = y + y_pos;
        coords[5] = (x + x_pos);
        coords[7] = (x + x_pos);

        //Column is the shortways
        sendCommand((uint8_t)CASET, (coords + 4), 4);
        //Row is the longways
        sendCommand((uint8_t)RASET, (coords), 4);

        //Adjust for the color
        if((((l_letter >> (24 - pixel_counter)) & 0x1)) == 0x1){
            sendCommand((uint8_t)RAMWR, (const uint8_t *)&color, 2);
            sendCommand((uint8_t)NOP, 0, 0);
        }else{
            sendCommand((uint8_t)RAMWR, (const uint8_t *)&background, 2);
            sendCommand((uint8_t)NOP, 0, 0);
        }

        if((pixel_counter % 5) == 4){
            x_pos = 0; 
        }else{
            x_pos++;
        }

        if((x_pos == 0) && (pixel_counter != 0)){
            y_pos++;
        }else{}

        pixel_counter ++;
    }
}
/*
void drawLetterTest(int32_t letter, int16_t x, int16_t y, uint16_t color, uint16_t background){
                      
    uint8_t coords[] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t curr_color[] = {0, 0};
    uint16_t test_color = 0x07E0;
    uint8_t pixel_counter = 0;
    uint32_t pixel_flag; 
    uint8_t x_pos = 0;
    uint8_t y_pos = 0;

    if(letter || 0x4000){ //If the letter has the 12 pixel flag set
        pixel_counter = 0;
        letter &= ~0x4000;
        //I need to figure out if the pixel at the current coordinate is color or background
        //I then need to swap to the next pixel
        //Then! I need to do that twice more
        //THen!!! I need to switch to the next row.
        
        //Hrm. Should I organize the bits differently? idk.
        //So now l_letter at the lsb is the current pixel

        while(pixel_counter <= 14){
            coords[1] = y + y_pos;
            coords[3] = y + y_pos;
            coords[5] = (x + x_pos);
            coords[7] = (x + x_pos);

            //Column is the shortways
            sendCommand((uint8_t)CASET, (coords + 4), 4);
            //Row is the longways
            sendCommand((uint8_t)RASET, (coords), 4);

            //Is the pixel On or Off
            if(((((letter) >> (14 - pixel_counter)) & 0x1)) == 0x1){
               test_color = 0xFFFF;
            }else{
               test_color = 0x0000;
            } 
            sendCommand((uint8_t)RAMWR, (const uint8_t *)&test_color, 2);
            sendCommand((uint8_t)NOP, 0, 0);

            //Time to decide the location of the next pixel
            if((pixel_counter % 3) == 2){
                x_pos = 0; 
            }else{
                x_pos++;
            }

            if((x_pos == 0) && (pixel_counter != 0)){
                y_pos++;
            }else{}

            pixel_counter ++;
        }
    }else if(letter || 0x8000){ //If the letter has the 16 pixel flag set

    }else if(letter || 0xA000){ //If the letter has the 20 pixel flag set

    }

}
*/




