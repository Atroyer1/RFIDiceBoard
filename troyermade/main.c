#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tftdisplay.h"
#include "button.h"

int main() {
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

    /******************************************************/


    //Testing a button triggering an IRQ
    /******************************************************/
    //initalizing button
    button_init();

    /******************************************************/

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
