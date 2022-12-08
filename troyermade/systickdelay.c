#include "pico/stdlib.h"
#include "systickdelay.h"
#include "hardware/structs/systick.h"
#include "hardware/irq.h"

#define CLK_PER_MS 12000    //Clock cycles per 1ms

void sysTick_Handler(void);

static volatile uint32_t stmsCount; //1ms counter variable
static uint32_t stSliceCount;
static uint8_t stInitFlag;
static uint32_t stLastEvent;

void sysTick_init(void){
    //Check if the reload value is possible
    //set reload register
    //Set priority for systic interrupt
    //load the systick counter value ???
    //Enable systick IRQ and sysTick timer (clksource, tickint, enable)
    stInitFlag = 0;
    stmsCount = 0;
    stSliceCount = 0;
    stLastEvent = 0;
    systick_hw->rvr = CLK_PER_MS;
    systick_hw->cvr = 0x0;

    irq_add_shared_handler((-1), sysTick_Handler, 0);

    //Enables Systick Exception
    systick_hw->csr |= M0PLUS_SYST_CSR_TICKINT_BITS;
    //Enable. Don't do this first
    hw_set_bits(&systick_hw->csr, M0PLUS_SYST_CSR_ENABLE_BITS << M0PLUS_SYST_CSR_ENABLE_LSB);
}


void sysTickWaitEvent(const uint32_t period){
    if(stInitFlag == 1){
        while((stmsCount - stLastEvent) < period) {}
    }else{
        stInitFlag = 1;
    }
    stLastEvent = stmsCount;
    stSliceCount++;
}

void sysTick_Handler(void){
    stmsCount++;
}
