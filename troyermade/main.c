#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tftdisplay.h"
#include "buttons.h"
#include "pn532.h"
#include "pico/util/datetime.h"
#include "rand_gen.h"

void testDisplay(void);
void testPN532(void);
void testDisplayAndPN532Combo(void);
void testButtons(void);
void testDisplay2(void);
void testRandGen(void);

int main() {


    //testButtons();
    //testDisplayAndPN532Combo();
    //testPN532();
    //testDisplay2();
    testRandGen();
    
}

void testRandGen(void){
    uint8_t worked;
    uint8_t counter = 0;
    uint8_t counter_str[] = {0,'\0'};
    uint8_t sec_string[] = {0, 0, '\0'};
    datetime_t t = {
            .year  = 2002,
            .month = 05,
            .day   = 1,
            .dotw  = 1, // 0 is Sunday, so 5 is Friday
            .hour  = 1,
            .min   = 1,
            .sec   = 2
    };
    rand_gen_init();
    tft_init();
    drawBackground(0x0000);
    rand_gen_set_curr_datetime(&t);

    while(1){
        worked = numToString(counter, counter_str, 2);
        if(!worked){
            drawString("Bad things happened", 5, 25, 0xFFFF, 0x0000);
        }
        drawString(counter_str, 5, 15, 0xFFFF, 0x0000);
        counter++;
        if(counter > 9){
            counter = 0;
        }
        rand_gen_get_curr_datetime(&t);
        if(t.sec <= 9){
            worked = numToString(t.sec, sec_string, 2);
            sec_string[1] = 0;
        }else{
            worked = numToString(t.sec, sec_string, 3);
        }
        if(!worked){
            drawString("2 Bad things       ", 5, 25, 0xFFFF, 0x0000);
        }
        drawString(sec_string, 5, 5, 0xFFFF, 0x0000);
        sleep_ms(100);
    }
    
    
}

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
