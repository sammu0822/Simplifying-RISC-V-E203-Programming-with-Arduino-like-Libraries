#include "wiring_digital.h"  // 這裡定義了 digitalRead, digitalWrite, pinMode
#include "wiring_analog.h"
#include <stdint.h>
#include "time_utils.h"  // 包含 millis() 函式


// #include "serial.h"  //如果你只是要使用底層的 UART 函式
#include "T-core_serial.hpp"  // 含有 class SerialHardware
#include "serial.h"  // 包含 TcoreSerial 的定義

void delay(int ms) //（僅限測試用途，請視時脈微調）
{
    volatile int count = ms * 100000;
    while(count--);
}

int main(void)
{
    Serial.begin(115200);

    char c ;
    while(1)
    {
        scan(rx_buffer); // 掃描 UART 接收的字元;
        if(Serial.available())
        {
            c = Serial.read(); // 如果有資料可讀
            Serial.write(c); // 回傳讀取的字元
        }
        else
            Serial.write("Waiting for data...\n");  // 等待序列埠有資料可讀
        delay(1000);  // 等待 1 秒
    }
}

// while(1) {
//         scan(rx_buffer) // 掃描 UART 接收的字元;
//         if(Serial.available())
//         {
//             if(Serial.read() > 0) // 如果有資料可讀
//             {
//                 Serial.write(c); // 回傳讀取的字元
//             }
//         }
//         else
//             Serial.write("Waiting for data...\n");  // 等待序列埠有資料可讀
//         // delay(1000);  // 等待 1 秒
//     }