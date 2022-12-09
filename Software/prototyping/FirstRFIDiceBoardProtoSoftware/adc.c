#include "hardware/adc.h"

//For my project(RFIDiceBoard) 2820 and above is 4.2 volts
//
//2488-2490 is at 3.70 Volts (Should technically have 20% of power left)
//2417 is below 3.59

void adc_initialize(void){
    adc_init();
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
}


uint16_t adc_get_input(void){
    uint16_t result = adc_read();
    return result;
}
