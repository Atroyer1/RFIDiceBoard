#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "buttons.h"
#include "main.h"

void Debounce_Btn(uint8_t btn_num);

void button_init(void){
    for(int i = BTN1_PIN; i <= BTN8_PIN; i++){
        gpio_set_function(i, GPIO_FUNC_SIO);
        gpio_set_input_enabled(i, true);
        gpio_set_input_hysteresis_enabled(i, true);
        gpio_pull_up(i);
        sleep_ms(20);
        //gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_LEVEL_LOW, true, &gpio_irq_handler);
    }
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

void Button_Task(void){
    if(gpio_get(BTN1_PIN) == 0){
        //Initial button press
        //Wait 20 ms then if the button is still pressed VERF
    }
}

/*
void button_gpio_irq_handler(uint gpio, uint32_t events){
    Button_Flag = gpio - BTN1_PIN + 1;
}
*/
