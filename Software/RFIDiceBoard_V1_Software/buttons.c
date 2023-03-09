#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/sync.h"
#include "buttons.h"
#include "main.h"

//Private functions
void Debounce_Btn(uint8_t btn_num);

//Private critical section variable
critical_section_t crit_section_buttons;

void button_init(void){
    for(int i = BTN1_PIN; i <= BTN5_PIN; i++){
        gpio_set_function(i, GPIO_FUNC_SIO);
        gpio_set_input_enabled(i, true);
        gpio_set_input_hysteresis_enabled(i, true);
        gpio_pull_up(i);
        sleep_ms(20);
        //gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_LEVEL_LOW, true, &gpio_irq_handler);
    }
    critical_section_init(&crit_section_buttons);
}

//Checks all states of the buttons, debounces current state, and sets the button 
//  flag to the current button
void Button_Task(void){
    volatile bool btn1; 
    volatile uint32_t btn_flags;
    static SW_STATE_T sw_state;
    
    critical_section_enter_blocking(&crit_section_buttons);
    btn_flags = (~(gpio_get_all())) & BTN_MASK;
    critical_section_exit(&crit_section_buttons);

    switch(sw_state){
    case SW_OFF:
        if(btn_flags != 0){
            sw_state = SW_EDGE;
        }else{}
        break;
    case SW_EDGE:
        if(btn_flags != 0){
            sw_state = SW_VERF;
            //converting btn_flags from one-hot to the number of the button
            //  Cancels the for loop on the first button to be detected
            //  with buttons having priority of 1,2,3, etc. 
            for(uint8_t i = 6; i <= 29; i++){
                if(((btn_flags >> i) & 1) == 0b1){
                    Button_Flag = i;
                    i = 30;
                }
            }
            if(Button_Flag == 0){
                Button_Flag = 1;
            }
        }else{
            sw_state = SW_OFF;
        }
        break;
    case SW_VERF:
        if(!btn_flags){
            sw_state = SW_OFF;
        }else{}
        break;
    default:
        sw_state = SW_OFF;
    }
}
