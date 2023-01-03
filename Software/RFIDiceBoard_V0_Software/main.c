#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "main.h"
#include "tftdisplay.h"
#include "buttons.h"
#include "pn532.h"
#include "rand_gen.h"
#include "adc.h"
#include "timer.h"

//int64_t debounce_handler(alarm_id_t id, void *user_data);

//bool repeating_timer_callback(struct repeating_timer *t);
//void wait_for_timer_event(void);

uint8_t Button_Flag;
uint8_t ADC_Flag;
uint8_t RFID_Flag;
uint8_t Debounce_Flag;
//uint8_t timer_main_flag;

//repeating_timer_t timer_main;

//uint8_t GPIO_irq_Flag;

//uint8_t GPIO_irq_Fired;

//alarm_id_t debounce_alarm;


void main(void){

    //initialization
    Button_Flag = 0;
    ADC_Flag = 0;
    RFID_Flag = 0;
    //GPIO_irq_Flag = 0;
    //timer_main_flag = 1;
    
    //add_repeating_timer_ms(10, repeating_timer_callback, NULL, &timer_main);

    pn532_init();
    tft_init();
    adc_initialize();
    button_init();
    timer_init();

    //alarm_pool_init_default();

    //Loop forever
    while(1){
        TimerDelay(10u);
        Adc_Task();
        Button_Task();
        RFID_Task();

    
        TFTDisplayTask();

    }
}
