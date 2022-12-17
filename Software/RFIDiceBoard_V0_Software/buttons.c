#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "buttons.h"
#include "main.h"

void gpio_callback(uint gpio, uint32_t events);
bool rawButtonInput;
SWSTATE_T button_state = SW_OFF;


//The buttons then need to trigger probably an IRQ to tell the RP2040 that a button has been pressed

void button_init(void){
    rawButtonInput = 0;
    for(int i = BTN1_PIN; i <= BTN8_PIN; i++){
        gpio_set_function(i, GPIO_FUNC_SIO);
        gpio_set_input_enabled(i, true);
        gpio_pull_up(i);
        gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_LEVEL_LOW , true, &gpio_callback);
    }
}

//Still have to debounce buttons
//I think I'll have this start a timer
void gpio_callback(uint gpio, uint32_t events){
    button_state = SW_EDGE;
}

//This will check if enough time has passed to allow a button press
uint8_t gpio_switch_debounce(){
    switch(button_state){
        case(SW_OFF):
        break;
        case(SW_EDGE):
        break;
        case(SW_VERF):
        break;
    }
}
