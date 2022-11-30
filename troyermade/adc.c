#include "hardware/adc.h"

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
