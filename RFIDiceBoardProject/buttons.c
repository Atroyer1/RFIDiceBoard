#include "pico/stdlib.h"
#include "buttons.h"


//I need an init function for each button
//The init functions need to pull the buttons high
//The buttons then need to trigger probably an IRQ to tell the RP2040 that a button has been pressed


/*
static uint8_t event_str[128];

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events){
    gpio_event_string(event_str, events);
    //drawString("You hit a button MF", 5, 60, 0xFFFF, 0x0000);
}
*/


void button_init(void){
    uint32_t pin_mask;
    //Eventually I should use gpio_init_mask to initialize multiple GPIO pins

    for(int i = BTN1_PIN; i <= BTN8_PIN; i++){
        gpio_set_function(i, GPIO_FUNC_SIO);
        gpio_set_input_enabled(i, true);
        gpio_pull_up(i);
    }
}
