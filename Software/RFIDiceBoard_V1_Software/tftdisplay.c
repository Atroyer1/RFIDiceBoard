#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "adc.h"
#include "main.h"
#include "tftdisplay.h"
#include "timer.h"
#include "pn532.h"
#include "rand_gen.h"

//private Function declarations
void sendCommand(uint8_t cmdByte, const uint8_t *dataBytes, uint8_t numDataBytes);
void drawPixel(uint8_t x, uint8_t y, uint16_t color);
void drawBackground(uint16_t color);
uint8_t drawString(uint8_t *string, uint8_t x, uint8_t y, uint16_t color, uint16_t background, uint8_t num_pix);
void drawLetter(uint32_t letter, uint8_t x, uint8_t y, uint16_t color, uint16_t background, uint8_t num_pix);


//Helper functions for string manipulation
bool numToString(uint32_t num, uint8_t *ret_string, uint8_t ret_string_len);
uint8_t string_length(uint8_t *string);
uint32_t decodeToPixelMap(uint8_t ch);
uint32_t getMagnitude(uint32_t num);

//private functions to print specific things
void tft_menu_print(void);
void idleUpdate(int32_t sliceCnt, int16_t clr);
void drawRectangle(uint8_t x_1, uint8_t x_2, uint8_t y_1, uint8_t y_2, uint16_t clr);
void drawButtons(void);


//Getters and setters
void cs_select(void);
void cs_deselect(void);
void dc_select(void);
void dc_deselect(void);
void rst_select(void);
void rst_deselect(void);


//initialization
void tft_init(void){
    uint8_t numCommands;
    uint8_t cmd;
    uint8_t numArgs;
    uint16_t ms;
    
    //Display initialization commands stored in an array for easy step-through
    //The array is organized in this pattern:
    /*Number of commands
      Command, length of data for command (Or a flag for length of delay),
      data,
      Command, ...,
      ...,
    */
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

    //Pointer to the commands
    const uint8_t *l_addr = init_commands;

    //SDK initialization command
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

    tft_menu_print();
    
    drawButtons();
    
    //drawRectangle(50, 55, 50, 58, 0xFFFF);
    /*
    drawString("BIG LETTERS", 5, 5, 0xFFFF, 0x0000, 2);
    drawString("HALEY", 5, 20, 0xFFFF, 0x0000, 3);
    drawString("ZBD-0", 5, 50, 0xFFFF, 0x0000, 4);
    drawString("WOO", 5, 70, 0xFFFF, 0x0000, 5);
    drawString("WOO", 5, 120, 0xFFFF, 0x0000, 6);
    */
    
    /*
    drawBigLetter(decodeToPixelMap('A'), 5, 5, 0xFFFF, 0x0000, 1);
    drawBigLetter(decodeToPixelMap('B'), 5 + 12, 5, 0xFFFF, 0x0000, 2);
    drawBigLetter(decodeToPixelMap('C'), 5 + (12*2), 5, 0xFFFF, 0x0000, 2);
    drawBigLetter(decodeToPixelMap('D'), 5 + (12*3), 5, 0xFFFF, 0x0000, 3);
    drawBigLetter(decodeToPixelMap('D'), 5, 20, 0xFFFF, 0x0000, 4);

    drawBigLetter(decodeToPixelMap('D'), 5, 60, 0xFFFF, 0x0000, 5);
    */
}

//Menu printing function to fix up the screen if the wire gets touched
void tft_menu_print(void){
    drawBackground(0x0000);
    /*
    
    drawString("Up Down to increase", 5, 11, 0xFFFF, 0x0000);
    drawString("and decrease num of", 5, 17, 0xFFFF, 0x0000);
    drawString("dice rolled", 5, 23, 0xFFFF, 0x0000);

    drawString("Left right to ", 5, 35, 0xFFFF, 0x0000);
    drawString("subtract and add 1 ", 5, 41, 0xFFFF, 0x0000);
    drawString("to the final rolled", 5, 47, 0xFFFF, 0x0000);
    drawString("number", 5, 53, 0xFFFF, 0x0000);

    drawString("Number of Dice 1", 5, 89, 0xFFFF, 0x0000);
    drawString("Add  ", 5, 95, 0xFFFF, 0x0000);
    */
}

//Handles all of the flags that other modules set when they want something
//  changed or put on the tft display
void TFTDisplayTask(void){
    uint8_t string[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\0'};
    uint8_t magnitude = 0;
    static bool negative_PlusMinusFlag = false;
    int32_t sum;
    uint8_t randnum;
    uint8_t modnum;
    uint8_t randnum_x;
    uint8_t randnum_y;
    static uint16_t blink_clr = 0xFFFF;
    static uint32_t count = 0;

    count = TimerGetSliceCount();

    //Cursor update
    

    //Time to check all the flags
    //Button check
    if(Button_Flag != 0){
        if(Button_Flag == 5){
            tft_menu_print();        
        }
        blink_clr = 0xF0F0;

        numToString(NumberOfDice, string, getMagnitude(NumberOfDice) + 2);
        drawString("  ", (16*6), 89, 0xFFFF, 0x0000, 1);
        drawString(string, (16*6), 89, 0xFFFF, 0x0000, 1);

        //PlusMinus can be negative so flipping to positive if necesary
        if(PlusMinus < 0){
            PlusMinus = -PlusMinus;
            negative_PlusMinusFlag = true;
        }else{
            negative_PlusMinusFlag = false;
        }
        numToString((uint16_t)PlusMinus, string, getMagnitude(PlusMinus) + 2);
        drawString("    ", (4*6), 95, 0xFFFF, 0x0000, 1);
        if(negative_PlusMinusFlag == true){ 
            PlusMinus = -PlusMinus;
            drawString("-", (5*6), 95, 0xFFFF, 0x0000, 1);
            drawString(string, (6*6), 95, 0xFFFF, 0x0000, 1);
        }else{
            drawString(string, (5*6), 95, 0xFFFF, 0x0000, 1);
        }

        Button_Flag = 0;

    //ADC check
    }else if(ADC_Flag != 0){
        switch(Batt_State){
        case(BATT_FULL):
            drawString("Battery full", 50, 5, 0xFFFF, 0x0000, 1);
        break;
        case(BATT_MED):
            drawString("Battery okay", 50, 5, 0xFFFF, 0x0000, 1);
        break;
        case(BATT_LOW):
            drawString("Battery low ", 50, 5, 0xFFFF, 0x0000, 1);
        break;
        case(BATT_DYING):
            drawString("Battery dead", 50, 5, 0xFFFF, 0x0000, 1);
        break;
        default:
        break;
        }
        ADC_Flag = 0;
    //RFID check
    }else if(RFID_Flag != 0){
        


        randnum_x = 5;
        randnum_y = 65;
        sum = 0;
        drawString("                     ", 5, 65, 0xFFFF, 0x0000, 1);
        drawString("                     ", 5, 71, 0xFFFF, 0x0000, 1);
        for(uint8_t i = 0; i < NumberOfDice; i++){
            switch(Current_Die){
            case D4UID:
                modnum = 4;
                break;
            case D6UID:
                modnum = 6;
                break;
            case D8UID:
                modnum = 8;
                break;
            case D10UID:
                modnum = 10;
                break;
            case D12UID:
                modnum = 12;
                break;
            case D20UID:
                modnum = 20;
                break;
            case D100UID:
                modnum = 100;
                break;
            default:
                //unidentified RFID tag
                modnum = 0;
                break;
            }
            randnum = (RandGen_GetRand32bit() % modnum) + 1;
            sum+= randnum;
            numToString(randnum, string, getMagnitude(randnum) + 2);

            drawString(string, randnum_x, randnum_y, 0xFFFF, 0x0000, 1);

            //Adjust screen position for next random number based on magnitude of number
            if(randnum < 10){
                randnum_x += 12;
            }else if(randnum < 100){
                randnum_x += 18;
            }else{
                //randnum should be 100
                randnum_x += 24;
            }

            //Check if the number is going to run off the screen
            if((randnum_x + 24) > (WIDTH)){
                randnum_x = 5;
                randnum_y += 6;
            }
        }

        //Displaing the sum (If we care about it)
        sum += PlusMinus;
        if((NumberOfDice > 1) || (PlusMinus != 0)){
            drawString("Sum is              ", 5, 77, 0xFFFF, 0x0000, 1);
            //Negative check
            if(sum >= 0){
                numToString(sum, string, getMagnitude(sum) + 2);
                drawString(string, (8*6), 77, 0xFFFF, 0x0000, 1);
            }else{ //Negative
                sum = -sum;
                numToString((uint16_t)sum, string, getMagnitude(sum) + 2);
                
                drawString("-", (8*6), 77, 0xFFFF, 0x0000, 1);
                drawString(string, (9*6), 77, 0xFFFF, 0x0000, 1);
            }
        }else{
            drawString("                    ", 5, 77, 0xFFFF, 0x0000, 1);
        }

        drawString("D   ", 5, 59, 0xFFFF, 0x0000, 1);
        numToString(modnum, string, getMagnitude(modnum) + 2);
        drawString(string, 11, 59, 0xFFFF, 0x0000, 1);
        RFID_Flag = 0;
    }else{}

    idleUpdate(count, blink_clr);

}

//Blinking "cursor" so we know the device is still alive
void idleUpdate(int32_t sliceCnt, int16_t clr){
    static uint16_t x_pos = 0;
    static uint16_t y_pos = 0;
    static uint8_t where = 0;
    static bool flip = true;
    
    //Moving the cursor around in a circle just for the hell of it
    drawPixel(x_pos, y_pos, flip*clr);
    if(where == 0){
        x_pos++;
        if(x_pos >= WIDTH){
            x_pos --;
            where = 1;
        }else{}
    }else if(where == 1){
        y_pos++;
        if(y_pos >= HEIGHT){
            y_pos--;
            where = 2;
        }else{}
    }else if(where == 2){
        x_pos--;
        if(x_pos <= 0){
            where = 3;
        }else{}
    }else if(where == 3){
        y_pos--;
        if(y_pos < 1){
            flip = !flip;
            where = 0;
        }else{}
    }else{}
}

//self-contained spi-write
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

void drawPixel(uint8_t x, uint8_t y, uint16_t color){
    uint8_t x_coords[] = {0, x, 0, x};
    uint8_t y_coords[] = {0, y, 0, y};

    sendCommand((uint8_t)CASET, x_coords, 4);
    sendCommand((uint8_t)RASET, y_coords, 4);

    sendCommand((uint8_t)RAMWR, (uint8_t *)&color, 2);
    sendCommand((uint8_t)NOP, 0, 0);
}

void drawRectangle(uint8_t x_1, uint8_t x_2, uint8_t y_1, uint8_t y_2, uint16_t clr){
    uint8_t x_coords[] = {0, x_1, 0, x_2};
    uint8_t y_coords[] = {0, y_1, 0, y_2};
    uint8_t ramwr = RAMWR;
    uint32_t numPixels = (((x_2 - x_1) + 1) * ((y_2 - y_1) + 1));

    sendCommand((uint8_t)RASET, y_coords, 4);
    sendCommand((uint8_t)CASET, x_coords, 4);
        
    cs_select();
    dc_select();
    spi_write_blocking(spi_default, (const uint8_t *)&ramwr, 1);
    dc_deselect();
    for(int i = 0; i < numPixels; i++){
        spi_write_blocking(spi_default, (uint8_t *)&clr, 2);
    }
    cs_deselect();
    sendCommand((uint8_t)NOP, 0, 0);
}

void drawBackground(uint16_t color){
    uint8_t x_coords[] = {0, 0, 0, WIDTH - 1};
    uint8_t y_coords[] = {0, 0, 0, HEIGHT - 1};
    uint32_t numPixels = (HEIGHT*WIDTH);
    uint8_t ramwr = RAMWR;

    sendCommand((uint8_t)RASET, y_coords, 4);
    sendCommand((uint8_t)CASET, x_coords, 4);
        
    cs_select();
    dc_select();
    spi_write_blocking(spi_default, (const uint8_t *)&ramwr, 1);
    dc_deselect();

    for(int i = 0; i < numPixels; i++){
        spi_write_blocking(spi_default, (uint8_t *)&color, 2);
    }
    cs_deselect();
    sendCommand((uint8_t)NOP, 0, 0);
}

//The goal here is to draw four things that are shaped like the buttons
//  on the PCB
void drawButtons(void){
    drawRectangle(BTN1_X1, BTN1_X2, BTN1_Y1, BTN1_Y2, 0xFFFF);
    drawRectangle(BTN2_X1, BTN2_X2, BTN2_Y1, BTN2_Y2, 0xFFFF);
    drawRectangle(BTN3_X1, BTN3_X2, BTN3_Y1, BTN3_Y2, 0xFFFF);
    drawRectangle(BTN4_X1, BTN4_X2, BTN4_Y1, BTN4_Y2, 0xFFFF);
    drawRectangle(BTN5_X1, BTN5_X2, BTN5_Y1, BTN5_Y2, 0xFFFF);
    
    drawRectangle(BTN1_X1 + 1, BTN1_X2 - 1, BTN1_Y1 - 1 , BTN1_Y1 - 1, 0xFFFF);
    drawRectangle(BTN1_X1 - 1, BTN1_X1 - 1, BTN1_Y1 + 1 , BTN1_Y2 - 1, 0xFFFF);
    drawRectangle(BTN1_X2 + 1, BTN1_X2 + 1, BTN1_Y1 + 1 , BTN1_Y2 - 1, 0xFFFF);
    drawRectangle(BTN1_X1 + 1, BTN1_X2 - 1, BTN1_Y2 + 1 , BTN1_Y2 + 1, 0xFFFF);

    drawRectangle(BTN2_X1 + 1, BTN2_X2 - 1, BTN2_Y1 - 1 , BTN2_Y1 - 1, 0xFFFF);
    drawRectangle(BTN2_X1 - 1, BTN2_X1 - 1, BTN2_Y1 + 1 , BTN2_Y2 - 1, 0xFFFF);
    drawRectangle(BTN2_X2 + 1, BTN2_X2 + 1, BTN2_Y1 + 1 , BTN2_Y2 - 1, 0xFFFF);
    drawRectangle(BTN2_X1 + 1, BTN2_X2 - 1, BTN2_Y2 + 1 , BTN2_Y2 + 1, 0xFFFF);

    drawRectangle(BTN3_X1 + 1, BTN3_X2 - 1, BTN3_Y1 - 1 , BTN3_Y1 - 1, 0xFFFF);
    drawRectangle(BTN3_X1 - 1, BTN3_X1 - 1, BTN3_Y1 + 1 , BTN3_Y2 - 1, 0xFFFF);
    drawRectangle(BTN3_X2 + 1, BTN3_X2 + 1, BTN3_Y1 + 1 , BTN3_Y2 - 1, 0xFFFF);
    drawRectangle(BTN3_X1 + 1, BTN3_X2 - 1, BTN3_Y2 + 1 , BTN3_Y2 + 1, 0xFFFF);

    drawRectangle(BTN4_X1 + 1, BTN4_X2 - 1, BTN4_Y1 - 1 , BTN4_Y1 - 1, 0xFFFF);
    drawRectangle(BTN4_X1 - 1, BTN4_X1 - 1, BTN4_Y1 + 1 , BTN4_Y2 - 1, 0xFFFF);
    drawRectangle(BTN4_X2 + 1, BTN4_X2 + 1, BTN4_Y1 + 1 , BTN4_Y2 - 1, 0xFFFF);
    drawRectangle(BTN4_X1 + 1, BTN4_X2 - 1, BTN4_Y2 + 1 , BTN4_Y2 + 1, 0xFFFF);

    drawRectangle(BTN5_X1 + 1, BTN5_X2 - 1, BTN5_Y1 - 1 , BTN5_Y1 - 1, 0xFFFF);
    drawRectangle(BTN5_X1 - 1, BTN5_X1 - 1, BTN5_Y1 + 1 , BTN5_Y2 - 1, 0xFFFF);
    drawRectangle(BTN5_X2 + 1, BTN5_X2 + 1, BTN5_Y1 + 1 , BTN5_Y2 - 1, 0xFFFF);
    drawRectangle(BTN5_X1 + 1, BTN5_X2 - 1, BTN5_Y2 + 1 , BTN5_Y2 + 1, 0xFFFF);
}

void drawLetter(uint32_t letter, uint8_t x, uint8_t y, uint16_t color, uint16_t background, uint8_t num_pix){
    uint8_t pixel_counter = 0;
    uint8_t x_pos = 0;
    uint8_t y_pos = 0;
    uint8_t x_coords[] = {0, 0, 0, 0};
    uint8_t y_coords[] = {0, 0, 0, 0};
    uint8_t ramwr = RAMWR;

    while(pixel_counter < 25){
        //Set up the coordinates
        x_coords[1] = (x + (x_pos * num_pix));
        x_coords[3] = (x + (x_pos * num_pix)) + (num_pix - 1);
        y_coords[1] = (y + (y_pos * num_pix));
        y_coords[3] = (y + (y_pos * num_pix)) + (num_pix - 1);

        //Set the coordinates
        sendCommand((uint8_t)CASET, x_coords, 4);
        sendCommand((uint8_t)RASET, y_coords, 4);

        //Having to do this manually because I want to color a block of pixels
        //  without having to send each and every coordinate
        cs_select();
        dc_select();
        spi_write_blocking(spi_default, (const uint8_t *)&ramwr, 1);
        dc_deselect();
        
        //Check if the current pixel should be part of the letter or the background
        if((((letter >> (24 - pixel_counter)) & 0x1)) == 0x1){
            for(uint8_t i = 0; i <= (num_pix * num_pix); i++){
                spi_write_blocking(spi_default, (uint8_t *)&color, 2);
                //sendCommand((uint8_t)RAMWR, (const uint8_t *)&color, 2);
            }
        }else{
            for(uint8_t i = 0; i <= (num_pix * num_pix); i++){
                spi_write_blocking(spi_default, (uint8_t *)&background, 2);
                //sendCommand((uint8_t)RAMWR, (const uint8_t *)&background, 2);
            }
        }
        
        
        cs_deselect();
        sendCommand((uint8_t)NOP, 0, 0);

        //Set up the next x and y pixel locations
        
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
//Writes a letter to the TFT display
void drawLetter(uint32_t letter, uint8_t x, uint8_t y, uint16_t color, uint16_t background){
    uint8_t pixel_counter = 0;
    uint8_t x_pos = 0;
    uint8_t y_pos = 0;
    uint8_t x_coords[] = {0, 0, 0, 0};
    uint8_t y_coords[] = {0, 0, 0, 0};

    while(pixel_counter < 25){
        //Set up the coordinates
        x_coords[1] = x + x_pos;
        x_coords[3] = x + x_pos;
        y_coords[1] = y + y_pos;
        y_coords[3] = y + y_pos;

        //Set the coordinates
        sendCommand((uint8_t)CASET, x_coords, 4);
        sendCommand((uint8_t)RASET, y_coords, 4);
        
        //Check if the current pixel should be part of the letter or the background
        if((((letter >> (24 - pixel_counter)) & 0x1)) == 0x1){
            sendCommand((uint8_t)RAMWR, (const uint8_t *)&color, 2);
        }else{
            sendCommand((uint8_t)RAMWR, (const uint8_t *)&background, 2);
        }
        sendCommand((uint8_t)NOP, 0, 0);

        //Set up the next x and y pixel locations
        
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
*/

//Takes in a char array, coordinates, letter color, and background color
//
//Draws a string on the display. 
//If the string fits it will return the length of the string.
//If the string doesn't fit it will return 0
uint8_t drawString(uint8_t *string, uint8_t x, uint8_t y, uint16_t color, uint16_t background, uint8_t num_pix){
    uint8_t ret;
    uint8_t l_x = x;
    uint8_t l_y = y;
    uint32_t pixelMap;
    //Check the number of characters
    uint8_t string_len = string_length(string);

    //Each character is 5 pixel wide and has a pixel of kerning between each character
    //If the number of characters * 6 is wider than the display, return
    if(WIDTH > (string_len * (5 * num_pix) + 1)){
        for(int i = 0; i < string_len; i++){
            //what is the character?
            pixelMap = decodeToPixelMap(string[i]);
            //print the character
            drawLetter(pixelMap, l_x, l_y, color, background, num_pix);
            //increment coordinates
            l_x += (5 * num_pix) + 1;
        }
        ret = 0;
    }else{
        ret = 1;
    }
    return ret;
}

//Checks the length of the string
uint8_t string_length(uint8_t *string){
    uint8_t i = 0;
    uint8_t ret;
    while((string[i] != '\0') && (i < 255)){
        i++;
    }
    if(i == 255){
        ret = 0;
    }else{
        ret = i;
    }
    return ret;
}

//I want the display to be able to display numbers larger than 9
//So I need to convert these numbers to a string
//Returns true if all went well. Returns false if something went wrong
bool numToString(uint32_t num, uint8_t *ret_string, uint8_t ret_string_len){
    uint8_t magnitude = 0;
    uint32_t num_l = num;
    uint32_t power;
    bool magCheck = false;
    bool ret = true;
    while(magCheck == false){
        if((num_l) >= 10){
            num_l = num_l/10;
            magnitude++;
        }else{
            magCheck = true;
        }
    }

    if((ret_string_len - 2) > magnitude){
        ret = false;
    }else{
        //Need to get the largest magnitude. I divide by 1 << magnitude
        for(uint8_t i = 0; i <= magnitude; i++){ 
            power = 1;
            for(uint8_t j = 0; j < magnitude - i; j++){
                power = power * 10;
            }
            ret_string[i] = ((num / (power) % 10) + '0');
        }
        ret_string[magnitude + 1] = '\0';
    }
    return ret;
}

//Decodes a character into one of my pixelMaps defined in tftdisplay.h
uint32_t decodeToPixelMap(uint8_t ch){
    uint32_t ret;
    uint8_t l_ch = ch;
    //Check what the character is
    if((l_ch >= 'A') && (l_ch <= 'Z')){
        //Uppercase letter
    }else if((l_ch >= 'a') && (l_ch <= 'z')){
        //Lowercase letter
        //
        //So we change it to an uppercase letter
        l_ch -= 0x20;
    }else if((l_ch >= '0') && (l_ch <= '9')){
        //Number, no change needed
    }else{
        //Not a character
        ret = l_not_a_letter;
    }
    switch(l_ch){
        case ' ':
            ret = l_space;
            break;
        case 'A':
            ret = l_A;
            break;
        case 'B':
            ret = l_B;
            break;
        case 'C':
            ret = l_C;
            break;
        case 'D':
            ret = l_D;
            break;
        case 'E':
            ret = l_E;
            break;
        case 'F':
            ret = l_F;
            break;
        case 'G':
            ret = l_G;
            break;
        case 'H':
            ret = l_H;
            break;
        case 'I':
            ret = l_I;
            break;
        case 'J':
            ret = l_J;
            break;
        case 'K':
            ret = l_K;
            break;
        case 'L':
            ret = l_L;
            break;
        case 'M':
            ret = l_M;
            break;
        case 'N':
            ret = l_N;
            break;
        case 'O':
            ret = l_O;
            break;
        case 'P':
            ret = l_P;
            break;
        case 'Q':
            ret = l_Q;
            break;
        case 'R':
            ret = l_R;
            break;
        case 'S':
            ret = l_S;
            break;
        case 'T':
            ret = l_T;
            break;
        case 'U':
            ret = l_U;
            break;
        case 'V':
            ret = l_V;
            break;
        case 'W':
            ret = l_W;
            break;
        case 'X':
            ret = l_X;
            break;
        case 'Y':
            ret = l_Y;
            break;
        case 'Z':
            ret = l_Z;
            break;


        case '0':
            ret = l_0;
            break;
        case '1':
            ret = l_1;
            break;
        case '2':
            ret = l_2;
            break;
        case '3':
            ret = l_3;
            break;
        case '4':
            ret = l_4;
            break;
        case '5':
            ret = l_5;
            break;
        case '6':
            ret = l_6;
            break;
        case '7':
            ret = l_7;
            break;
        case '8':
            ret = l_8;
            break;
        case '9':
            ret = l_9;
            break;
        case '-':
            ret = l_hyphen;
            break;
        default:
            ret = l_not_a_letter;
            break;
    }
    return ret;
}

uint32_t getMagnitude(uint32_t num){
    bool magCheck = false;
    uint32_t num_l = num;
    uint8_t magnitude = 0;

    while(magCheck == false){
        if((num_l) >= 10){
            num_l = num_l/10;
            magnitude++;
        }else{
            magCheck = true;
        }
    }
    return magnitude;
}

/*Setters*/
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
