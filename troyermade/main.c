#include "pico/stdlib.h"
#include "tftdisplay.h"

int main() {
    uint8_t testNum = 0x0000;

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
    drawLetter(l_Z, 23, 5, 0xFFFF, 0x0000);
    /******************************************************/

    /******************************************************/
    /******************************************************/
    while (1) {
        testNum = ~testNum;
        for(uint8_t i = 30; i <= 35; i++){
            for(uint8_t j = 5; j <= 10; j++){
                drawPixel(i, j, (uint16_t)testNum);
            }
        } 
        sleep_ms(100);
    }
}
