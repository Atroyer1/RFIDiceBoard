#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "PN532DriverProto.h"


#define I2CADDR 0x24
#define RST_PIN 2
#define IRQ_PIN 3

void dummyRead(void);
void resetTheThing(void);
uint8_t isReady(uint8_t pin);

uint8_t packetbuffer[64];

int main() {
//cmdlen:
//Includes the byte frame identifier (tohost or hostto) as well as
//the cmd and data bytes


    uint8_t cmdlen1 = 2; //cmdlen1 for getfirmwareversion
                         
    uint8_t cmdlen2 = 2; //cmdlen2 for return bytes of getfirmwareversion
    uint8_t checksum1 = (uint8_t)(PN532_PREAMBLE + PN532_STARTCODE1 + PN532_STARTCODE2 + PN532_HOSTTOPN532 + PN532_COMMAND_GETFIRMWAREVERSION); 
    uint8_t checksum2 = (uint8_t)(PN532_PREAMBLE + PN532_STARTCODE1 + PN532_STARTCODE2 + PN532_PN532TOHOST + (PN532_COMMAND_GETFIRMWAREVERSION + 1));
    uint8_t cmd1[] = {PN532_PREAMBLE, PN532_STARTCODE1, PN532_STARTCODE2,
                     cmdlen1, ((~cmdlen1) + 1), PN532_HOSTTOPN532, 
                     PN532_COMMAND_GETFIRMWAREVERSION,
                     ~checksum1, PN532_POSTAMBLE
    };
    uint8_t cmd2[] = {PN532_PREAMBLE, PN532_STARTCODE1, PN532_STARTCODE2,
                     cmdlen2, ((~cmdlen2) + 1), PN532_PN532TOHOST, 
                     PN532_COMMAND_GETFIRMWAREVERSION + 1
    };
    uint8_t recieve[32];
    int ret;

    stdio_init_all();

    gpio_init(RST_PIN);
    gpio_set_dir(RST_PIN, GPIO_OUT); 
    gpio_init(IRQ_PIN);
    gpio_set_dir(IRQ_PIN, GPIO_IN); 
    //i2c init
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    gpio_put(RST_PIN, 1);
    gpio_put(RST_PIN, 0);
    sleep_ms(400);
    gpio_put(RST_PIN, 1);
    sleep_ms(10);

    while (1) {
        i2c_write_blocking(i2c_default, I2CADDR, cmd1, 9, false);
        sleep_ms(2);
        
        while(isReady(IRQ_PIN)){
        }
        
        i2c_read_blocking(i2c_default, I2CADDR, recieve, 8, false);

        sleep_ms(2);

        while(isReady(IRQ_PIN)){
        }
        
        i2c_read_blocking(i2c_default, I2CADDR, recieve, 14, false);
        //i2c_read_blocking(i2c_default, I2CADDR, (void), 8, false); 
        sleep_ms(15);
        //i2c_read_blocking(i2c_default, I2CADDR, (void), 14, false); 
        sleep_ms(200);
        
        //dummyRead();
        //i2c_write_blocking(i2c_default, I2CADDR, cmd2, 7, false);
        //i2c_read_blocking(i2c_default, I2CADDR, recieve, 12, false); 
        //while(1){};
    }
}


void dummyRead(void){
    uint8_t garbage;
    i2c_read_blocking(i2c_default, I2CADDR, &garbage, 1, false); 
}

        //ret = i2c_write_blocking(i2c_default, I2CADDR, &cmd, 1, false);
        //ret = i2c_read_blocking(i2c_default, I2CADDR, recieve, 12, false);


void resetTheThing(void){
    


}

uint8_t isReady(uint8_t pin){
    gpio_get(IRQ_PIN);
}
