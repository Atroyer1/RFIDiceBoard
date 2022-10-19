#include "tftdisplay.h"
#include "pico/stdlib.h"

int main() {

    while (1) {
        tft_init();
        sleep_ms(30);
    }
}
