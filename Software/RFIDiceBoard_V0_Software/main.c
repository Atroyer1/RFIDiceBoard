#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "main.h"
#include "tftdisplay.h"
#include "buttons.h"
#include "pn532.h"
#include "rand_gen.h"
#include "adc.h"

//int64_t debounce_handler(alarm_id_t id, void *user_data);

bool repeating_timer_callback(struct repeating_timer *t);
void wait_for_timer_event(void);

uint8_t Button_Flag;
uint8_t ADC_Flag;
uint8_t RFID_Flag;
uint8_t Debounce_Flag;
uint8_t timer_flag;

repeating_timer_t timer;

//uint8_t GPIO_irq_Flag;

//uint8_t GPIO_irq_Fired;

//alarm_id_t debounce_alarm;


void main(void){

    //initialization
    Button_Flag = 0;
    ADC_Flag = 0;
    RFID_Flag = 0;
    //GPIO_irq_Flag = 0;
    timer_flag = 1;
    
    add_repeating_timer_ms(10, repeating_timer_callback, NULL, &timer);

    pn532_init();
    tft_init();
    adc_initialize();
    button_init();

    //alarm_pool_init_default();

    updateTFTDisplay();
    //Loop forever
    while(1){
        //TODO
        wait_for_timer_event();
        /*
        if(gpio_get(BTN1_PIN) == 0){
            Debounce_Flag = 1;
            //Eventually the buttons are gonna have to do something else
            //TODO debounce
            //updateTFTDisplay();

            //Button_Flag = 0;
        }else{
            Debounce_Flag = 0;
        }

        if(Debounce_Flag == 1){
            Debounce_Btn(BTN1_PIN);
        }
        */
        Button_Task();

        if(ADC_Flag != 0){ 
            Adc_Task();
            ADC_Flag = 0;
        }else if(RFID_Flag != 0){ 
            RFID_Task();
            updateTFTDisplay();
            RFID_Flag = 0;
        }else{}
    }
}

void wait_for_timer_event(void){
    //Do I sleep in here? What do I do?
    while(timer_flag){
    }
    timer_flag = 0;
}

bool repeating_timer_callback(struct repeating_timer *t){
    timer_flag = 1;
    return true;
}

/*
int64_t debounce_handler(alarm_id_t id, void *user_data){
    updateTFTDisplay();
    if(gpio_get(GPIO_irq_Fired) == 0){

        //GPIO_irq_Flag = GPIO_irq_Fired;
    }
    return 0;
}
*/

/*
void gpio_irq_handler(uint gpio, uint32_t events){
    GPIO_irq_Flag = gpio;
    gpio_set_irq_enabled(gpio, GPIO_IRQ_LEVEL_LOW, false);
    add_alarm_in_ms(1000, debounce_handler, NULL, true);

   //if(events |= GPIO_IRQ_EDGE_FALL){
   //    GPIO_irq_Fired = gpio;

   //    add_alarm_in_ms(20, debounce_handler, NULL, true);

   //    gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL, false);
   //}else if(events |= GPIO_IRQ_LEVEL_HIGH){
   //    gpio_set_irq_enabled(gpio, GPIO_IRQ_LEVEL_HIGH, false);
   //    gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL, true);
   //}
   //if(events == GPIO_IRQ_LEVEL_LOW){
   //    GPIO_irq_Flag = gpio;
   //    gpio_set_irq_enabled(gpio, GPIO_IRQ_LEVEL_LOW, false);
   //    gpio_set_irq_enabled(gpio, GPIO_IRQ_LEVEL_HIGH, true);
   //}else if(events == GPIO_IRQ_LEVEL_HIGH){
   //    gpio_set_irq_enabled(gpio, GPIO_IRQ_LEVEL_HIGH, false);
   //    gpio_set_irq_enabled(gpio, GPIO_IRQ_LEVEL_LOW, true);
   //}else{
   //}
   //if((gpio >= BTN1_PIN) && (gpio <= BTN8_PIN)){
   //    Button_Flag = gpio - BTN1_PIN + 1;
   //}else if(gpio == PN532_IRQ_PIN){
   //    RFID_Flag = 1;
   //    gpio_set_irq_enabled(PN532_IRQ_PIN, GPIO_IRQ_EDGE_FALL, false);
   //}
}
*/

