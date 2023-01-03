#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/sync.h"
#include "buttons.h"
#include "main.h"

//Private functions
void Debounce_Btn(uint8_t btn_num);

critical_section_t crit_section_buttons;

//Private variables

void button_init(void){
    for(int i = BTN1_PIN; i <= BTN8_PIN; i++){
        gpio_set_function(i, GPIO_FUNC_SIO);
        gpio_set_input_enabled(i, true);
        gpio_set_input_hysteresis_enabled(i, true);
        gpio_pull_up(i);
        sleep_ms(20);
        //gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_LEVEL_LOW, true, &gpio_irq_handler);
    }
    critical_section_init(&crit_section_buttons);
}

void Debounce_Btn(uint8_t btn_num){
    static uint8_t count = 0;
    if(Debounce_Flag == 1){
        count++;
    }else{
        count = 0;
    }
    if(count >= 20){
        count = 0;
        Button_Flag = 1;
        //BUTTON Press verified
    }
}

//Currently only works for one button
void Button_Task(void){
    volatile bool btn1; 
    static SW_STATE_T sw_state;
    
    critical_section_enter_blocking(&crit_section_buttons);
    btn1 = !gpio_get(BTN1_PIN);
    critical_section_exit(&crit_section_buttons);
    switch(sw_state){
    case SW_OFF:
        if(btn1){
            sw_state = SW_EDGE;
        }else{}
        break;
    case SW_EDGE:
        if(btn1){
            sw_state = SW_VERF;
            Button_Flag = 1;
        }else{
            sw_state = SW_OFF;
        }
        break;
    case SW_VERF:
        if(!btn1){
            sw_state = SW_OFF;
        }else{}
        break;
    default:
        sw_state = SW_OFF;
    }
}

/*
void button_gpio_irq_handler(uint gpio, uint32_t events){
    Button_Flag = gpio - BTN1_PIN + 1;
}
*/
