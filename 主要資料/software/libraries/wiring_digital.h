#ifndef _WIRING_DIGITAL_H
#define _WIRING_DIGITAL_H

#include <stdint.h>
#include <stdbool.h>
#include "platform.h"
#include "pins_T-core.h"
#include "T-core.h"

//C++語法相容
#ifdef __cplusplus
extern "C" {
#endif

static inline bool isPWMSupported(uint8_t pin);
static inline bool getPWMChannel(uint8_t pin) ;
static void disableIOF(uint8_t pin) ;

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /*_WIRING_DIGITAL_H*/

//#define GPIO_BASE_ADDR (0x10012000) // gpio base address
//#define GPIO_INPUT_VAL_ADDR (GPIO_BASE_ADDR+0x00) // gpio input valueregister addr
//#define GPIO_INPUT_EN_ADDR (GPIO_BASE_ADDR+0x04) // gpio input enableregister addr
//#define GPIO_OUTPUT_EN_ADDR (GPIO_BASE_ADDR+0x08) // gpio outputenable register addr
//#define GPIO_OUTPUT_VAL_ADDR (GPIO_BASE_ADDR+0x0C) // gpio output valueregister addr

//#define GPIO_REG(offset)       (*(volatile uint32_t *)(GPIO_BASE_ADDR + (offset)))
//#define GPIO_INPUT_EN          0x04
//#define GPIO_OUTPUT_EN         0x08

//#define INPUT         0x0
//#define OUTPUT        0x1
//#define INPUT_PULLUP  0x2
//#define ANALOG_INPUT  0x3
//#define ANALOG_OUTPUT 0x4
