/*
	測試程式碼:
*/

//#include "../../libraries/wiring_digital.h"
#include "wiring_digital.h"  // 這裡定義了 digitalRead, digitalWrite, pinMode
#include "wiring_analog.h"
#include <stdint.h>

#define GPIO_PIN_INOUT0  0  // 第 0 腳接 LED
#define GPIO_PIN_INOUT1  1  // 第 1 腳接 LED
#define GPIO_PIN_INOUT2  2  // 第 2 腳接 LED
#define GPIO_PIN_INOUT3  3  // 第 3 腳接 LED

#define GPIO_PIN_INOUT4   4  // 第 4 接腳開關
#define GPIO_PIN_INOUT5   5  // 第 5 接腳開關
#define GPIO_PIN_INOUT6   6  // 第 6 接腳開關
#define GPIO_PIN_INOUT7   7  // 第 7 接腳開關

#define GPIO_PIN_INPUT8_  8  // 第 8 接腳按鈕
#define GPIO_PIN_INPUT9   9  // 第 9 接腳按鈕

#define GPIO_10_PIN0     10	// 第  0 腳接 GPIO[10]
#define GPIO_11_PIN1     11	// 第  1 腳接 GPIO[11]
#define GPIO_12_PIN2     12	// 第  2 腳接 GPIO[12]
#define GPIO_13_PIN3     13	// 第  3 腳接 GPIO[13]
#define GPIO_14_PIN4     14	// 第  4 腳接 GPIO[14]
#define GPIO_15_PIN5     15	// 第  5 腳接 GPIO[15]

#define GPIO_18_PIN6     18	// 第  6 腳接 GPIO[18]
#define GPIO_19_PIN7     19	// 第  7 腳接 GPIO[19]
#define GPIO_20_PIN8     20	// 第  8 腳接 GPIO[20]
#define GPIO_21_PIN9     21	// 第  9 腳接 GPIO[21]
#define GPIO_22_PIN10    22	// 第 10 腳接 GPIO[22]
#define GPIO_23_PIN11    23	// 第 11 腳接 GPIO[23]

int main(void)
{
	pinMode(GPIO_PIN_INOUT0, OUTPUT);
	pinMode(GPIO_PIN_INOUT4, INPUT);
	
	
	while (1) 
	{
	    if (digitalRead(GPIO_PIN_INOUT4) == HIGH)
		digitalWrite(GPIO_PIN_INOUT0, HIGH);
	    else
		digitalWrite(GPIO_PIN_INOUT0, LOW);
	}
}



