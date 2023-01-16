#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "tftdisplay.h"
#include "pico/sync.h"
#include "adc.h"
#include "main.h"
#include "timer.h"


//Public state of the battery
volatile BATTSTATE_T Batt_State;

//private critical section variable
critical_section_t crit_section_adc;

void adc_initialize(void){
    adc_init();
    Batt_State = BATT_MED;
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);

    critical_section_init(&crit_section_adc);
}

//Checks the current state of the battery once a second and updates Batt_State accordingly
void Adc_Task(void){
    static BATTSTATE_T prev_Batt_State = 20;
    static uint16_t prev_adc_value = 0;
    static volatile uint16_t adc_value = 100;
    static uint32_t sliceCount = 0;
    
    sliceCount = TimerGetSliceCount();

    if((sliceCount % 100) == 0){
        critical_section_enter_blocking(&crit_section_adc); 
        adc_value = adc_read();
        critical_section_exit(&crit_section_adc);
        //Hysteresis
        if(((adc_value - 10) > (prev_adc_value)) || ((adc_value + 10) < (prev_adc_value))){
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
                ADC_Flag = 1;
                prev_Batt_State = Batt_State;
            }
        }else{
            //nothing. Previous value didn't change enough
        }
    }
}
