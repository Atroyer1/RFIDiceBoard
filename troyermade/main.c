#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tftdisplay.h"
#include "buttons.h"
#include "pn532.h"
#include "rand_gen.h"
#include "adc.h"

void testDisplay(void);
void testPN532(void);
void testDisplayAndPN532Combo(void);
void testButtons(void);
void testDisplay2(void);
void testRandGen(void);
void testADC(void);

int main() {

    //testButtons();
    //testDisplayAndPN532Combo();
    //testPN532();
    //testDisplay();
    //testDisplay2();
    //testRandGen();
    testADC();
    
}

void testADC(void){
    const float conversion_factor = 3.0f / (1 << 12);
    uint32_t num_l;
    uint16_t adc_num;
    uint8_t adc_str[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\0'};
    uint8_t x = 5;
    uint8_t magnitude = 0;
    bool magCheck = false;

    tft_init();
    drawBackground(0x0000);
    drawString("ADC raw value", 5, 10, 0xFFFF, 0x0000);
    
    //Magnitude checker
    while(1){
        adc_num = adc_get_input();
        magnitude = 0;
        magCheck = false;
        num_l = adc_num;
        while(magCheck == false){
            if((num_l) >= 10){
                num_l = num_l/10;
                magnitude++;
            }else{
                magCheck = true;
            }
        }
        if(!numToString(adc_num, adc_str, magnitude + 2)){
            drawString("Problem", 25, 5, 0xFFFF, 0x00);
            drawString("              ", 5, 16, 0xFFFF, 0x0000);
            drawString(adc_str, 5, 100, 0xFFFF, 0x0000);
            drawString("Magnitude is", 25, 30, 0xFFFF, 0x0000);
            numToString(magnitude, adc_str, 2);
            drawString(adc_str, 25, 36, 0xFFFF, 0x0000);
            while(1){};
        }
        x = 60 - (((magnitude) * 6));

        //Display the number
        drawString("                   . ", 5, 16, 0xFFFF, 0x0000);
        drawString(adc_str, x, 16, 0xFFFF, 0x0000);
        sleep_ms(200);
    }


}

void testRandGen(void){
    uint32_t number;
    uint32_t num_l;
    uint8_t magnitude = 0;
    uint8_t x = 5;
    bool magCheck = false;
    uint8_t rand_string[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\0'};

    tft_init();
    drawBackground(0x0000);
    sleep_ms(500);

    while(1){
        magnitude = 0;
        magCheck = false;
        number = rand_gen_get_rand();
        num_l = number;
        while(magCheck == false){
            if((num_l) >= 10){
                num_l = num_l/10;
                magnitude++;
            }else{
                magCheck = true;
            }
        }
        drawString("Hi Allison", 25, 100, 0xFFFF, 0x0000);

        //Convert the number to a string
        if(!numToString(number, rand_string, magnitude + 2)){
            drawString("Problem", 25, 5, 0xFFFF, 0x00);
            drawString("              ", 5, 16, 0xFFFF, 0x0000);
            drawString(rand_string, 5, 100, 0xFFFF, 0x0000);
            drawString("Magnitude is", 25, 30, 0xFFFF, 0x0000);
            numToString(magnitude, rand_string, 2);
            drawString(rand_string, 25, 36, 0xFFFF, 0x0000);
            while(1){};
        }
        
        
        x = 60 - (((magnitude) * 6));

        //Display the number
        drawString("                   . ", 5, 16, 0xFFFF, 0x0000);
        drawString(rand_string, x, 16, 0xFFFF, 0x0000);
        sleep_ms(200);
    }

}
/////////////////////////////////////////////
/*
void testRandGen(void){
    bool problem = false;
    uint8_t counter = 0;
    uint32_t number;
    uint8_t counter_string[] = {0, '\0'};
    uint8_t num_string[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\0'};
    uint8_t binary_string[] = {0, '\0'};
    uint8_t power_string[] = {0, 0, '\0'};
    uint8_t x = 5;
    uint32_t power = 1;
    tft_init();
    drawBackground(0x0000);
    while(1){
        counter++;
        counter = counter % 10;
        numToString(counter, counter_string, 2);
        drawString(counter_string, 5, 25, 0xFFFF, 0x0000);
        //number = test_rand() & 100u;

        number = test_rand();

        x = 5;
        
        for(uint8_t i = 1; i < 11; i++){
            power = 1;
            
            //This is a small power function
            for(uint8_t j = 1; j < i; j++){
                power = power * 10;
            }
            ////

            //Power drawing

            //Detecting the magnitude of the random number
            if(number < power){
                drawString("Power equals", 5, 74, 0xFFFF, 0x0000);
                numToString(power, power_string, 3);
                drawString("               ", 5, 80, 0xFFFF, 0x0000);
                drawString(power_string, 5, 80, 0xFFFF, 0x0000);

                if(!numToString(i, binary_string, 2)){
                    drawString("AAAAAA", 20, 68, 0xFFFF, 0x0000);
                }
                drawString(binary_string, 5, 62, 0xFFFF, 0x0000);
                num_string[i] = '\0';                
                x = 59 - ((i - 1) * 6);
                problem = numToString(number, num_string, i);
                i = 10;
            }
        }

        if(!problem){
            drawString("Problem", 5, 50, 0xFFFF, 0x0000);
            drawString("       ", 5, 56, 0xFFFF, 0x0000);

            drawString(num_string, x, 56, 0xFFFF, 0x0000);
        }else{
            drawString("       ", 5, 56, 0xFFFF, 0x0000);
        }

        drawString("Hopefully random", 5, 5, 0xFFFF, 0x0000);
        drawString("           ", 5, 12, 0xFFFF, 0x0000);

        drawString(num_string, x, 12, 0xFFFF, 0x0000);

        sleep_ms(500);
    }
}
/////////////////////////////////////////////////////////////////
*/



void testButtons(void){
    button_init();
    

    //Need the display to prove buttons work
    tft_init();
    drawBackground(0x0000);
    
    while(1){
        for(int i = BTN1_PIN; i <= BTN8_PIN; i++){
            if(gpio_get(i) == 0){
                drawString("BTN Pressed", 5, 5, 0xFFFF, 0x0000);
                while(gpio_get(i) == 0){
                }
            }else{
                drawString("           ", 5, 5, 0xFFFF, 0x0000);
            }
        }
    }
    
    /*
    while(1){
        if(gpio_get(BTN1_PIN) == 0){
            drawString("BTN1 Pressed      ", 5, 5, 0xFFFF, 0x0000);
        }else if(gpio_get(BTN3_PIN) == 0){
            drawString("BTN3 Pressed      ", 5, 5, 0xFFFF, 0x0000);
        }else{
            drawString("No Buttons Pressed", 5, 5, 0xFFFF, 0x0000);
        }
    }
    */
}

void testPN532(void){
    //Testing PN532
    uint8_t versionInfo[14];
    uint8_t uid[4];
    uint8_t uid_length[4];
    pn532_init();
    
    pn532_send(PN532_COMMAND_GETFIRMWAREVERSION, 0, 1);
    pn532_read(versionInfo, 14);
    pn532_SAMConfig();
    sleep_ms(200);
    while(1){
        pn532_readPassiveTargetID(0x00, uid, uid_length);
    
        sleep_ms(1000);
    }
}

//So I want the pn532 to read a uid from an RFID card and then display it on the tft
void testDisplayAndPN532Combo(void){
    uint8_t uid[4];
    uint8_t known_uid[4] = {0x52, 0xAD, 0x2C, 0x61};
    uint8_t card_uid[4] = {0x6E, 0xAF, 0xFD, 0x6F};
    uint8_t versionInfo[14];
    uint8_t uid_length[4];
    uint8_t StringArray[] = {0, 0, 0, 0, '\0'};
    tft_init();
    drawBackground(0x0000);

    pn532_init();
    pn532_send(PN532_COMMAND_GETFIRMWAREVERSION, 0, 1);
    pn532_read(versionInfo, 14);
    pn532_SAMConfig();

    //sleep_ms(200);
    drawString("running", 5, 120, 0xFFFF, 0x0000);
    while(1){
        pn532_readPassiveTargetID(0x00, uid, uid_length);
        for(uint8_t i = 0; i < 4; i++){
            if(uid[i] != known_uid[i]){
                drawString("not disk uid", 5, 11, 0xFFFF, 0x0000);
            }
            if(uid[i] != card_uid[i]){
                drawString("not card uid", 5, 17, 0xFFFF, 0x0000);
            }
        }
        drawString("Got out", 5, 5, 0xFFFF, 0x0000);
        sleep_ms(500);
        drawString("       ", 5, 5, 0xFFFF, 0x0000);
        drawString("            ", 5, 11, 0xFFFF, 0x0000);
        drawString("            ", 5, 17, 0xFFFF, 0x0000);
    }
}

void testDisplay(void){
    uint8_t testNum = 0x0000;
    uint8_t testString[]= "Hi Haley";
    uint8_t set = 0;
    uint8_t array[] = {0, '\0'};
    uint8_t p = 0;

    tft_init();

    //Testing drawBackground
    drawBackground(0x0000);

    //Testing drawPixel
    /******************************************************
    for(uint8_t i = 30; i <= 35; i++){
        for(uint8_t j = 5; j <= 10; j++){
            drawPixel(i, j, 0x0000);
        }
    } 
    *****************************************************/

    //Testing drawLetter
    /******************************************************/
    drawLetter(l_A, 5, 5, 0xFFFF, 0x0000);
    drawLetter(l_B, 11, 5, 0xFFFF, 0x0000);
    drawLetter(l_X, 17, 5, 0xFFFF, 0x0000);
    drawLetter(l_Y, 23, 5, 0xFFFF, 0x0000);
    /******************************************************/

    //Testing drawString
    /******************************************************/
    drawString(testString, 5, 30, 0xFFFF, 0x0000);
    drawString("I am a robot", 5, 36, 0xFFFF, 0x0000);
    while (1) {
        testNum = ~testNum;
        for(uint8_t i = 30; i <= 35; i++){
            for(uint8_t j = 5; j <= 10; j++){
                drawPixel(i, j, (uint16_t)testNum);
            }
        } 
    }

}

void testDisplay2(void){
    uint32_t num = 1929292;
    uint8_t num_string[2];
    uint8_t length = 2;
    bool worked;
    tft_init();
    drawBackground(0x0000);
    
    worked = numToString(num, num_string, length);
    
    if(worked){
        drawString(num_string, 5, 5, 0xFFFF, 0x0000);
    }else{
        drawString("Didn't Work", 5, 5, 0xFFFF, 0x0000);
    }
}
