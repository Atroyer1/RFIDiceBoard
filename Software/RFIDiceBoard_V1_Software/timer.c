#include "pico/stdlib.h"

//Private irq for a ms timer
bool repeating_timer_callback(struct repeating_timer *t);

repeating_timer_t timer;

static volatile uint32_t tmsCount; //1ms counter variable
static uint32_t tSliceCount;
static uint32_t tInitFlag;
static uint32_t tLastEvent;

//Delays for however much time is left of the input period +-1ms
void TimerDelay(const uint32_t period){
    if(tInitFlag == 1){
        while((tmsCount - tLastEvent) < period){
        }
    }else{
        tInitFlag = 1;
    }
    tLastEvent = tmsCount;
    tSliceCount++;
}

void timer_init(void){
    tInitFlag = 0;
    tmsCount = 0;
    tSliceCount = 0;
    tLastEvent = 0;
    add_repeating_timer_ms(1, repeating_timer_callback, NULL, &timer);
}

//Private access to tSliceCount
uint32_t TimerGetSliceCount(void){
    return tSliceCount;
}

bool repeating_timer_callback(struct repeating_timer *t){
    tmsCount++;
}
