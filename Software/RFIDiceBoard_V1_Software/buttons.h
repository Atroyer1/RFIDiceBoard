#define BTN1_PIN 27
#define BTN2_PIN 22
#define BTN3_PIN 25
#define BTN4_PIN 23
#define BTN5_PIN 24


//#define BTN_MASK 0b00000011111111000000000000000000
//#define BTN_MASK 0x03FC0000
//#define BTN_MASK 0x3FE0
#define BTN_MASK 0x5C00000
typedef enum {SW_EDGE, SW_VERF, SW_OFF} SW_STATE_T;
void button_init(void);
void Button_Task(void);
