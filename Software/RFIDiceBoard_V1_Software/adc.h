//Arbitrary Battery values measured by the ADC
#define BATT_FULL_VAL 2720
#define BATT_MED_VAL 2600
#define BATT_LOW_VAL 2490

//For my project(RFIDiceBoard) 2820 and above is 4.2 volts
//
//2488-2490 is at 3.70 Volts (Should technically have 20% of power left)
//2417 is below 3.59 where the device shuts off automatically


void adc_initialize(void);
void Adc_Task(void);

typedef enum {BATT_FULL, BATT_MED, BATT_LOW, BATT_DYING} BATTSTATE_T;
volatile extern BATTSTATE_T Batt_State;
