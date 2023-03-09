#define BTN1_PIN 39
#define BTN2_PIN 34
#define BTN3_PIN 37
#define BTN4_PIN 35
#define BTN5_PIN 36

//#define BTN_MASK 0b00000011111111000000000000000000
//#define BTN_MASK 0x03FC0000
#define BTN_MASK 0x3FE0
typedef enum {SW_EDGE, SW_VERF, SW_OFF} SW_STATE_T;
void button_init(void);
void Button_Task(void);
