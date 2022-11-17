#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tftdisplay.h"
#include "button.h"
#include "pn532.h"

void testDisplay(void);
void testPN532(void);
void testDisplayAndPN532Combo(void);

int main() {

    //Testing buttons
    /******************************************************/
    //initalizing button
    //button_init();

    /******************************************************/
    testDisplayAndPN532Combo();
    //testPN532();
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
    uint8_t old_uid0 = uid[0];
    uint8_t versionInfo[14];
    uint8_t uid_length[4];
    uint8_t StringArray[] = {0, 0, 0, 0, '\0'};
    tft_init();
    drawBackground(0x0000);
    pn532_init();
    pn532_send(PN532_COMMAND_GETFIRMWAREVERSION, 0, 1);
    pn532_read(versionInfo, 14);
    pn532_SAMConfig();
    sleep_ms(200);
    drawString("running", 5, 120, 0xFFFF, 0x0000);
    while(1){
        pn532_readPassiveTargetID(0x00, uid, uid_length);
        drawString("Made it to the if", 5, 30, 0xFFFF, 0x0000);
        if(uid[0] != old_uid0){
            old_uid0 = uid[0];
            for(int i = 0; i < 4; i++){
                StringArray[i] = (uid[i] % 9) + '0';
            }
            drawString(StringArray, 5, 5, 0xFFFF, 0x0000);
        }
    }
}

void testDisplay(void){
    uint8_t testNum = 0x0000;
    uint8_t testString[]= "Hi Haley";
    uint8_t set = 0;
    uint8_t array[] = {0, '\0'};
    uint8_t p = 0;

    //Should I stdio init here?
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
        
        for(uint8_t i = BTN1_PIN; i <= BTN8_PIN; i++){
            if(gpio_get(i) == 0){
                //drawString((uint8_t *)(i + '0'), 5, 66, 0xFFFF, 0x0000);
                if(set == 1){
                    set = 0;
                    drawString("BOOP", 5, 60, 0xFFFF, 0x0000);
                    array[0] = i + '0';
                    drawString(array, 5, 66, 0xFFFF, 0x0000);
                }
            }else{
                if(set == 0){
                    set = 1;
                    drawString("BEEP", 5, 60, 0xFFFF, 0x0000);
                }
            }
        }
        //sleep_ms(100);
    }

}
