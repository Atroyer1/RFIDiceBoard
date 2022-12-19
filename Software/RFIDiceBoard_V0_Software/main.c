#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "main.h"
#include "tftdisplay.h"
#include "buttons.h"
#include "pn532.h"
#include "rand_gen.h"
#include "adc.h"

uint8_t Button_Flag;
uint8_t ADC_Flag;

void main(void){

    //initialization
    Button_Flag = 0;
    ADC_Flag = 0;

    pn532_init();
    tft_init();
    adc_initialize();
    button_init();

    updateTFTDisplay();
    //Loop forever
    while(1){
        //sleep_ms(1);
        if(Button_Flag != 0){
            //Debounce!
            updateTFTDisplay();
            Button_Flag = 0;
        }else if(ADC_Flag != 0){ 
            adc_Task();
        }else{
            //Do nothing
            asm("nop");
        }
    }
}
