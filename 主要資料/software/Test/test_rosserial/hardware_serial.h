#ifndef HARDWARE_SERIAL_H
#define HARDWARE_SERIAL_H

#include <stdint.h>
#include <stddef.h>

class HardwareSerial {
public:
  HardwareSerial(volatile uint32_t* uart_base);

  void begin(unsigned long baudrate);
  void end();

  int available();
  int read();
  size_t write(uint8_t c);
  void flush();

private:
  volatile uint32_t* _uart;
};

extern HardwareSerial Serial;

#endif /*HARDWARE_SERIAL_H*/

