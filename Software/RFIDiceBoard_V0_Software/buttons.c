#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "buttons.h"
#include "main.h"

void gpio_callback(uint gpio, uint32_t events);
bool button_timer_irq_handler(repeating_timer_t *rt);

bool rawButtonInput;
SWSTATE_T button_state = SW_OFF;
repeating_timer_t debounce_timer;

//The idea is:
//GOT EDGE DETECTED
    ///Alarm instead of timer?
    //TIMER FOR 20 MS
//Software check for BUTTON_LOW
//BUTTON_VERF = TRUE Button_Flag = button_num_pressed

void button_init(void){
    rawButtonInput = 0;
    for(int i = BTN1_PIN; i <= BTN8_PIN; i++){
        gpio_set_function(i, GPIO_FUNC_SIO);
        gpio_set_input_enabled(i, true);
        gpio_pull_up(i);
        gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_LEVEL_LOW, true, &gpio_callback);
    }

    /*if(!add_repeating_timer_ms(20u, button_timer_irq_handler, 0u, &debounce_timer)){
        while(1){
            //Trap if the timer can't be added
        }
    */
    //}
}

//Still have to debounce buttons
//I think I'll have this start a timer
void gpio_callback(uint gpio, uint32_t events){
    button_state = SW_EDGE;
    //Start a 20 ms timer
    //Actually, start the 20ms timer only if it's an edge from 1 to 0
    //TODO make or look a damn debounce state machine
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

bool button_timer_irq_handler(repeating_timer_t *rt){
    //add_alarm_in_ms(20u, gpio_button_debounce_irq, (void)0, false);
    return 1;
}
