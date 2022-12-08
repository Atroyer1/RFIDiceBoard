/**************************************************************************
* RFIDice Board
* Specialized software for RFIDice Board hardware.
* Utilizes Adafruit PN532 breakout board, Adafruit 1.8" Color TFT LCD, and developed
*   on the RP2040
*
* The RFIDiceBoard emulates rolling dice with RFID tags representing die with different
*   number sides. The user places the RFID tag with a unique id associated with the number of *   sides they desire close to the RFIDiceBoard to trigger a dice roll to be displayed on
*   the LCD screen.
*
* Andrew Troyer 12-5-2022
**************************************************************************/

#include "pico/stdlib.h"
#include "tftdisplay.h"
#include "buttons.h"
#include "pn532.h"
#include "rand_gen.h"
#include "adc.h"

int main(){
//
    tft_init();


    while(1){
    //Systick thing
    tftTask();


    }


}
