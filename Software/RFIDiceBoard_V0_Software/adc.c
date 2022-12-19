#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "tftdisplay.h"
#include "adc.h"
#include "main.h"

//For my project(RFIDiceBoard) 2820 and above is 4.2 volts
//
//2488-2490 is at 3.70 Volts (Should technically have 20% of power left)
//2417 is below 3.59

BATTSTATE_T Batt_State;
uint16_t adc_value;
repeating_timer_t timer;

bool adc_irq_handler(repeating_timer_t *rt);

void adc_initialize(void){
    adc_init();
    Batt_State = BATT_MED;
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    if(!add_repeating_timer_ms(1000u, adc_irq_handler, 0u, &timer)){
        while(1){
            //Trap if the timer can't be added
        }
    }
}

void adc_Task(void){
    static BATTSTATE_T prev_Batt_State;

    adc_value = adc_read();

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
}

bool adc_irq_handler(repeating_timer_t *rt){
    ADC_Flag = 1;
    return 1;
}
