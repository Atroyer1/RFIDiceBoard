#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "main.h"
#include "pn532.h"
#include "timer.h"

#define I2CADDR 0x24
#define RST_PIN 2

uint8_t pn532_send(uint8_t command, uint8_t *data, size_t len);
void pn532_read(uint8_t *buffer, size_t len);
void pn532_SAMConfig(void);
void pn532_readPassiveTargetID_send(uint8_t cardbaudrate);
void pn532_readPassiveTargetID_recieve(uint8_t *uid);
uint8_t pn532_read_ACK(void);
uint8_t pn532_isReady(void);

//void pn532_gpio_irq_handler(uint gpio, uint32_t events);

bool waiting_for_read;

void pn532_init(void){
    //RP2040 getting ready
    gpio_init(RST_PIN);
    gpio_set_dir(RST_PIN, GPIO_OUT); 
    /*
    gpio_init(PN532_IRQ_PIN);
    gpio_set_dir(PN532_IRQ_PIN, GPIO_IN); 
    */
    gpio_set_function(PN532_IRQ_PIN, GPIO_FUNC_SIO);
    gpio_set_input_enabled(PN532_IRQ_PIN, true);
    gpio_set_input_hysteresis_enabled(PN532_IRQ_PIN, false);

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

    //Setting the PN532 to be a card reader
    pn532_SAMConfig();

    pn532_readPassiveTargetID_send(0x00);

    //irq pin can now work after setting up for reading RFID tags
    //gpio_set_irq_enabled_with_callback(PN532_IRQ_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}


void RFID_Task(void){
    uint8_t uid[4];
    uint32_t number_uid;
    bool volatile pn532_irq_pin;
    static bool got_one;
    static uint32_t waitCount;
    uint32_t sliceCount;
    //uint32_t const lookupDie[7] = {D4UID, D6UID, D8UID, D10UID, D12UID, D20UID, D100UID};
    
    sliceCount = TimerGetSliceCount();
    pn532_irq_pin = !gpio_get(PN532_IRQ_PIN);
    if(got_one == false){
        if(pn532_irq_pin){
            waitCount = TimerGetSliceCount() + 100;
            got_one = true;
            //The device wants to talk to us about something
            pn532_readPassiveTargetID_recieve(uid);
            //Need to verify which uid we got but we should probably do it next cycle
            //RFID_Flag = 1;
            Current_Die = (uid[0] << 24) | (uid[1] << 16) | (uid[2] << 8) | (uid[3]);
            RFID_Flag = 1;
        }
    }else{
        if(waitCount <= sliceCount){
            pn532_readPassiveTargetID_send(0);
            got_one = false;
        }
    }
}

//Okay
//The idea here is that I am using a big 'ol buffer that is a set size so that I can send more than just commands
/***************///The other idea here is that data always comes with a command.
uint8_t pn532_send(uint8_t command, uint8_t *data, size_t len){
    uint8_t ret;
    uint8_t buf[64];
    uint8_t checksum = PN532_STARTCODE2 + PN532_HOSTTOPN532 + command;
    int index = 7;
    
    //Every i2c message to the pn532 needs at least 8 bytes for information framesk
    if(len > (64 - 8) || len < 0){
        ret = 0;
    }else{
        //Time to fill the buffer
        buf[0] = PN532_PREAMBLE; 
        buf[1] = PN532_STARTCODE1;
        buf[2] = PN532_STARTCODE2;
        //The length sent is the number of bytes in the data field as well
        //  as the identifier of hostto or tohost
        buf[3] = len + 1;
        buf[4] = ((~(len + 1)) + 1);
        buf[5] = PN532_HOSTTOPN532;
        buf[6] = command;
        
        //Time for the command and data to be sent
        if(len > 1){
            for(index = 7; index < ((len - 1) + 7); index++){
                buf[index] = data[index - 7];
                checksum += buf[index];
            }
            //index++;
        }else{}
        //Checksum
        buf[index] = ~checksum;
        index++;
        buf[index] = PN532_POSTAMBLE;
        //Postamble
        
        i2c_write_blocking(i2c_default, I2CADDR, buf, (index + 1), false);
        ret = len;
    }
    return ret;
}

//Read handles both waits
void pn532_read(uint8_t *buffer, size_t len){
    uint8_t ret = 0;

    //Do the read and return through buffer
    while(!pn532_isReady()){
    }
    i2c_read_blocking(i2c_default, I2CADDR, buffer, len, false);

    /*
    Old version. Saving just in case for a minute
    uint8_t ret = 0;
    if(pn532_read_ACK() == 0){
        //Do the read and return through buffer
        while(!pn532_isReady()){
        }
        i2c_read_blocking(i2c_default, I2CADDR, buffer, len, false);
    }else{
        //I don't know what to do but I needed to get rid of the ACK
    }
    */
}

void pn532_SAMConfig(void){
                   //Command, normal mode, timeout 50ms*20 = 1 second, use IRQ pin
    uint8_t buf1[] = {0x01, 0x14, 0x01};
    uint8_t buf2[8];
    pn532_send(PN532_COMMAND_SAMCONFIGURATION, buf1, 4);
    pn532_read_ACK();

    pn532_read(buf2, 8);
}

void pn532_readPassiveTargetID_send(uint8_t cardbaudrate){
    uint8_t buf[] = {1, cardbaudrate};

    pn532_send(PN532_COMMAND_INLISTPASSIVETARGET, buf, 3);
    pn532_read_ACK();
}


void pn532_readPassiveTargetID_recieve(uint8_t *uid){
    uint8_t returnbuf[20];

    pn532_read(returnbuf, 20);
    for(int i = 14; i <= 17; i++){
        uid[i-14] = returnbuf[i];
    }
    
}

//Returns 0 if no issues
//Returns 1 if ACK failed
uint8_t pn532_read_ACK(void){
    uint8_t buffer[7];
    uint8_t ack[] = {0x01, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
    uint8_t ret = 0;
    while(!pn532_isReady()){
    //Wait
    }
    i2c_read_blocking(i2c_default, I2CADDR, buffer, 7, false);
    //Compare buffer and ack
    for(int i = 0; i < 7; i++){
        if(buffer[i] != ack[i]){
            ret = 1;
        }else{
        }
    }
    return ret;
}

//IRQ pin gets pulled low when the PN532 is ready with something
uint8_t pn532_isReady(void){
    return !gpio_get(PN532_IRQ_PIN);
}
