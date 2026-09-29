// hardware_serial.c
#include "hardware_serial.h"
#include "your_uart_driver.h"  // ← 這是你已經完成的 UART 實作

// 全域變數：實例
HardwareSerial serial_hw;

// 實作函式
void setBaudImpl(uint32_t baudrate) {
    serial_hw.baudrate = baudrate;
    uart_set_baud(serial_hw.port_handle, baudrate);  // 你自己實作的函式
}

void setPortImpl(void *port) {
    serial_hw.port_handle = port;
}

int writeImpl(const uint8_t *data, int len) {
    return uart_write(serial_hw.port_handle, data, len);  // 你已實作的 UART 傳送
}

int readImpl(uint8_t *buffer, int maxlen) {
    return uart_read(serial_hw.port_handle, buffer, maxlen);  // 你已實作的 UART 接收
}

// 初始化 HardwareSerial 結構
void initHardwareSerial(void) {
    serial_hw.setBaud = setBaudImpl;
    serial_hw.setPort = setPortImpl;
    serial_hw.write   = writeImpl;
    serial_hw.read    = readImpl;
}
