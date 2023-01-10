#include "pico/stdlib.h"
#include "rand_gen.h"
#include "hardware/structs/rosc.h"
#include "hardware/regs/rosc.h"
#include "buttons.h"
#include "main.h"

uint32_t random_32_bit_gen(void);

//Public
int8_t PlusMinus;
uint8_t NumberOfDice;

//Private
uint32_t s1 = 100;
uint32_t s2 = 100;
uint32_t s3 = 100;


void rand_init(void){
    PlusMinus = 0;
    NumberOfDice = 1;
}

uint32_t RandGen_GetRand32bit(void){
    s1 = s1 | (random_32_bit_gen());
    s2 = s2 | (random_32_bit_gen());
    s3 = s3 | (random_32_bit_gen());
    s1 = ((171 * s1) % 30269);
    s2 = ((172 * s2) % 30207);
    s3 = ((173 * s3) % 30323);
    
    return (s1 + s2 + s3) % 5212632;
}

uint32_t random_32_bit_gen(){
    static volatile uint32_t *randbit_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    uint32_t random = 0;
    uint32_t random_bit = 0;
    for(int i = 0; i < 32; i++){
        asm("nop");
        asm("nop");
        random_bit = 0x1 & (*randbit_reg);
        random = ((random << 1) | (random_bit));
    }
    return random;
}

void Rand_Task(void){
    if(Button_Flag != 0){
        switch(Button_Flag){
        case(BTN1_PIN):
            if(NumberOfDice < 12){
                NumberOfDice++;
            }else{}
            //Up
            break;
        case(BTN2_PIN):
            //Left
            if(PlusMinus >= -30){
                PlusMinus--;
            }else{}
            break;
        case(BTN3_PIN):
            //Down
            if(NumberOfDice >= 1){
                NumberOfDice--;
            }else{}
            break;
        case(BTN4_PIN):
            //Right
            if(PlusMinus <= 30){
                PlusMinus++;
            }else{}
            break;
        case(BTN5_PIN):
                //Lowest Right button
            break;
        case(BTN6_PIN):
                //Second Lowest Right button
            break;
        case(BTN7_PIN):
                //Third
            break;
        case(BTN8_PIN):
                //Fourth
            break;
        default:
            break;
        }
    }
}
