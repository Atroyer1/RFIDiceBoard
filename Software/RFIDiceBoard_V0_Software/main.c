#include "pico/stdlib.h"
#include "main.h"
#include "tftdisplay.h"
#include "buttons.h"
#include "pn532.h"
#include "rand_gen.h"
#include "adc.h"
#include "timer.h"

uint8_t ADC_Flag;
uint8_t RFID_Flag;
uint8_t Debounce_Flag;
uint32_t Button_Flag;
uint32_t Current_Die;

void main(void){

    //initialization
    Button_Flag = 1;
    ADC_Flag = 0;
    RFID_Flag = 0;
    Current_Die = 0;
    
    pn532_init();
    tft_init();
    adc_initialize();
    button_init();
    timer_init();
    rand_init();

    //Loop forever
    while(1){
        TimerDelay(10u);
        Adc_Task();
        Button_Task();
        Rand_Task();
        RFID_Task();

        TFTDisplayTask();
    }
}
