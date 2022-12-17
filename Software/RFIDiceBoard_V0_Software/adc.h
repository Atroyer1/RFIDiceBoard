//Arbitrary Battery values measured by the ADC`
#define BATT_FULL_VAL 2720
#define BATT_MED_VAL 2600
#define BATT_LOW_VAL 2490


void adc_initialize(void);
void adc_Task(void);
typedef enum {BATT_FULL, BATT_MED, BATT_LOW, BATT_DYING} BATTSTATE_T;

extern BATTSTATE_T battstate;

