#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "buttons.h"
#include "main.h"

void button_gpio_irq_handler(uint gpio, uint32_t events);

void button_init(void){
    for(int i = BTN1_PIN; i <= BTN8_PIN; i++){
        gpio_set_function(i, GPIO_FUNC_SIO);
        gpio_set_input_enabled(i, true);
        gpio_set_input_hysteresis_enabled(i, false);
        gpio_pull_up(i);
        gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_LEVEL_LOW, true, &button_gpio_irq_handler);
    }
}

//Still have to debounce buttons
//I think I'll have this start a timer
void button_gpio_irq_handler(uint gpio, uint32_t events){
    Button_Flag = gpio - BTN1_PIN + 1;
}
