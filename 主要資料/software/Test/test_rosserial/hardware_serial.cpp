#include "hardware_serial.h"
#include "platform.h"

#define UART_TX_FULL (1 << 31)
#define UART_RX_EMPTY (1 << 31)

#define UART_REG(offset) (*(volatile uint32_t *)(_uart + (offset))) // ->platform.h

#define UART_TXDATA 0
#define UART_RXDATA 1
#define UART_TXCTRL 2
#define UART_RXCTRL 3
#define UART_DIV    4

HardwareSerial::HardwareSerial(volatile uint32_t* uart_base)
  : _uart(uart_base) {}

//->demo_uart.c
void HardwareSerial::begin(unsigned long baudrate) { 
  // 根據 CPU 頻率設置 baudrate
  UART_REG(UART_DIV) = (get_cpu_freq() / baudrate) - 1;
  UART_REG(UART_TXCTRL) |= (1 << 0); // TXEN
  UART_REG(UART_RXCTRL) |= (1 << 0); // RXEN
}

//->demo_uart.c
void HardwareSerial::end() {
  UART_REG(UART_TXCTRL) &= ~(1 << 0);
  UART_REG(UART_RXCTRL) &= ~(1 << 0);
}

//->demo_uart.c
int HardwareSerial::available() {
  return !(UART_REG(UART_RXDATA) & UART_RX_EMPTY);
}

//->demo_uart.c
int HardwareSerial::read() {
  if (!available()) return -1;
  return UART_REG(UART_RXDATA) & 0xFF;
}

//->demo_uart.c
size_t HardwareSerial::write(uint8_t c) {
  while (UART_REG(UART_TXDATA) & UART_TX_FULL);
  UART_REG(UART_TXDATA) = c;
  return 1;
}

//->demo_uart.c
void HardwareSerial::flush() {
  // 可選: 若有 transmit FIFO，可等清空
}

// UART0 base address 請依照你平台實際設定
HardwareSerial Serial((volatile uint32_t*)UART0_CTRL_ADDR);

