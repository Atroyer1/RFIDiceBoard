#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/sync.h"
#include "buttons.h"
#include "main.h"

//Private functions
void Debounce_Btn(uint8_t btn_num);
void button_gpio_set(uint8_t butt_num);

//Private critical section variable
critical_section_t crit_section_buttons;

void button_init(void){
    button_gpio_set(BTN1_PIN);
    button_gpio_set(BTN2_PIN);
    button_gpio_set(BTN3_PIN);
    button_gpio_set(BTN4_PIN);
    button_gpio_set(BTN5_PIN);

    critical_section_init(&crit_section_buttons);
}

void button_gpio_set(uint8_t butt_num){
    gpio_set_function(butt_num, GPIO_FUNC_SIO);
    gpio_set_input_enabled(butt_num, true);
    gpio_set_input_hysteresis_enabled(butt_num, true);
    gpio_pull_up(butt_num);
    sleep_ms(20);

}

//Checks all states of the buttons, debounces current state, and sets the button 
//  flag to the current button
void Button_Task(void){
    volatile uint32_t btn_flags;
    static SW_STATE_T sw_state = SW_OFF;
    
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
            
            for(uint8_t i = 22; i <= 27; i++){
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
