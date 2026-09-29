#ifndef _WIRING_ANALOG_H
#define _WIRING_ANALOG_H

#include <stdint.h>
#include <stdbool.h>
#include "platform.h"
#include "pins_T-core.h"
#include "T-core.h"

#define PWM_0_GPIO_BASE  0
#define PWM_2_GPIO_BASE 10
#define PWM_1_GPIO_BASE 19

#define PWM_REG_CMP0_DEFAULT 255 //ros雙輪車使用255 (搭配test_wiring_analogWrite.c測試用)

//C++語法相容
#ifdef __cplusplus
extern "C" {
#endif

//static void setupPWM(uint8_t pin, uint16_t value);
static void analogSetting(uint8_t PWMchennel, uint8_t count, uint8_t timer);

void enableIOF1(uint8_t pin);
void analogWrite(uint8_t pin, uint8_t val) ;
//void analogRead(uint8_t pin, uint8_t value);

//int PWM_REG_CMP0;

//C++語法相容
#ifdef __cplusplus
}
#endif

#endif /*_WIRING_ANALOG_H*/ 
