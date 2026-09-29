extern "C" {
#include "serial.h"
}

void delay(int ms) {
    volatile int count = ms * 100000;
    while(count--);
}

int main(void) {
    Serial_begin(115200);
    const char* msg = "Hello Minicom!\r\n";
    while (1) {
        for (int i = 0; msg[i] != '\0'; i++) {
            Serial_write(msg[i]);
        }
        delay(10);
    }
}
