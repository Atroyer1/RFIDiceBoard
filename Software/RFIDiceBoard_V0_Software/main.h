extern uint8_t Button_Flag;
extern uint8_t RFID_Flag;
extern uint8_t ADC_Flag;
extern uint8_t PN532_Flag;
extern uint8_t Debounce_Flag;

//This timer is externed so that I can turn it off whilst in the Menu mode that is in
//  buttons.c
extern repeating_timer_t timer_main;
