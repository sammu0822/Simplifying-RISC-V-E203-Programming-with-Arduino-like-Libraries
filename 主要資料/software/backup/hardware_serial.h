#ifndef _HARDWARE_SERIAL_H
#define _HARDWARE_SERIAL_H

#include <stdint.h>

// 定義函式指標類型
typedef void (*SetBaudFunc)(uint32_t baudrate);
typedef void (*SetPortFunc)(void *port);
typedef int  (*WriteFunc)(const uint8_t *data, int len);
typedef int  (*ReadFunc)(uint8_t *buffer, int maxlen);

// 定義 HardwareSerial 結構
typedef struct {
    SetBaudFunc setBaud;
    SetPortFunc setPort;
    WriteFunc write;
    ReadFunc read;

    void *port_handle;  // 可以保存指向 UART 裝置的指標
    uint32_t baudrate;  // 保留 baudrate 設定值
} HardwareSerial;

#endif /*_HARDWARE_SERIAL_H */