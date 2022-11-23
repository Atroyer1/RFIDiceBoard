#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "rand_gen.h"


void rand_gen_init(void){
    rtc_init();
    //RTC requires 64us to update datetime correctly
    sleep_us(64);
}

void rand_gen_get_curr_datetime(datetime_t *t){
    rtc_get_datetime(t);
}

void rand_gen_set_curr_datetime(datetime_t *t){
    rtc_set_datetime(t);
    sleep_us(64);
}
