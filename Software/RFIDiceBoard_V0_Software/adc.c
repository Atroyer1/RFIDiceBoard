#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "tftdisplay.h"
#include "adc.h"
#include "main.h"

BATTSTATE_T Batt_State;
repeating_timer_t battery_timer;

bool adc_timer_irq_handler(repeating_timer_t *rt);

void adc_initialize(void){
    adc_init();
    Batt_State = BATT_MED;
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    if(!add_repeating_timer_ms(1000u, adc_timer_irq_handler, 0u, &battery_timer)){
        while(1){
            //Trap if the timer can't be added
        }
    }
}

void Adc_Task(void){
    static BATTSTATE_T prev_Batt_State = 20; //Set to a number that Batt_State can't reach
    static uint16_t prev_adc_value = 0;
    static uint16_t adc_value;

    adc_value = adc_read();
    //Hysteresis
    if(((adc_value + 10) > (prev_adc_value)) || ((adc_value - 10) < (prev_adc_value))){
        prev_adc_value = adc_value;
        if(adc_value > BATT_FULL_VAL){
            //All is good
            Batt_State = BATT_FULL;
        }else if(adc_value > BATT_MED_VAL){
            //All is okay
            Batt_State = BATT_MED;
        }else if(adc_value > BATT_LOW_VAL){
            //NEEDS TO BE CHARGED
            Batt_State = BATT_LOW;
        }else{
            //GONNA DIE IN A MOMENT
            Batt_State = BATT_DYING;
        }

        if(Batt_State != prev_Batt_State){
            prev_Batt_State = Batt_State;
            updateTFTDisplay();
        }
    }else{
        //nothing. Previous value didn't change enough
    }
}

bool adc_timer_irq_handler(repeating_timer_t *rt){
    ADC_Flag = 1;
    return 1;

}
