/*
	»¡©ú-³Wœd©ÒŠ³¥i¥HšÏ¥ÎªºGPIOž}Šìžê°T¡AšÏ¥Î€@­Óµ²ºc¯x°}ÀxŠs 
*/

///*硬體固定連接（不可更改）*/
//#define GPIO_0_LED0	   0	// GPIO 0 腳接 LED
//#define GPIO_1_LED1      1	// GPIO 1 腳接 LED
//#define GPIO_2_LED2      2	// GPIO 2 腳接 LED
//#define GPIO_3_LED3      3	// GPIO 3 腳接 LED
//
//#define GPIO_4_SWITCH0   4	// GPIO 4 接腳開關
//#define GPIO_5_SWITCH1   5	// GPIO 5 接腳開關
//#define GPIO_6_SWITCH2   6	// GPIO 6 接腳開關
//#define GPIO_7_SWITCH3   7	// GPIO 7 接腳開關
//
//#define GPIO_8_BUTTON0   8	// GPIO 8 接腳按鈕（reset）
//#define GPIO_9_BUTTON1   9	// GPIO 9 接腳按鈕
//
///*可用GPIO*/
//#define GPIO_10_PIN0     10	// GPIO  10 腳接外部 PIN0 (GPIO 10)
//#define GPIO_11_PIN1     11	// GPIO  11 腳接外部 PIN1 (GPIO 11)
//#define GPIO_12_PIN2     12	// GPIO  12 腳接外部 PIN2 (GPIO 12)
//#define GPIO_13_PIN3     13	// GPIO  13 腳接外部 PIN3 (GPIO 13)
//#define GPIO_14_PIN4     14	// GPIO  14 腳接外部 PIN4 (GPIO 14)
//#define GPIO_15_PIN5     15	// GPIO  15 腳接外部 PIN5 (GPIO 15)
//
//#define GPIO_18_PIN6     18	// GPIO  18 腳接外部 PIN6 (GPIO 18)
//#define GPIO_19_PIN7     19	// GPIO  19 腳接外部 PIN7 (GPIO 19)
//#define GPIO_20_PIN8     20	// GPIO  20 腳接外部 PIN8 (GPIO 20)
//#define GPIO_21_PIN9     21	// GPIO  21 腳接外部 PIN9 (GPIO 21)
//#define GPIO_22_PIN10    22	// GPIO  22 腳接外部 PIN10 (GPIO 22)
//#define GPIO_23_PIN11    23	// GPIO  23 腳接外部 PIN11 (GPIO 23)
//
///*特殊用途GPIO（不可更改）*/
//#define GPIO_16_UART0_TX 16	//UART0_TX
//#define GPIO_17_UART0_RX 17	//UART0_RX
//#define GPIO_24_UART1_TX 24	//UART1_RX (IOF0)
//#define GPIO_25_UART1_TX 25	//UART1_TX (IOF0)
//#define GPIO_26_SPI1_CS0 26	//SPI1_cs0
//#define GPIO_27_SPI1_DQ0 27	//SPI1_dq0 / mosi
//#define GPIO_28_SPI1_DQ1 28	//SPI1_dq1 / miso
//#define GPIO_29_SPI1_SCK 29	//SPI1_sck
//#define GPIO_30_SPI1_DQ2 30	//SPI1_dq2
//#define GPIO_31_SPI1_DQ3 31	//SPI1_dq3

//------------------------------------------------

// GPIO 0-31 IOF map
/////////////////////
// GPIO,  IOF0  ,  IOF1
//  0  ,        ,pwn0_0 
//  1  ,        ,pwn0_1 
//  2  ,spi0_cs0,pwn0_2 
//  3  ,spi0_dq0,pwn0_3 
//  4  ,spi0_dq1,
//  5  ,spi0_sck,
//  6  ,spi0_dq2,
//  7  ,spi0_dq3,
//  8  ,spi0_cs1,
//  9  ,spi0_cs2,
// 10  ,spi0_cs3,pwn2_0
// 11  ,        ,pwn2_1
// 12  ,        ,pwn2_2
// 13  ,        ,pwn2_3
// 14  ,i2c0_sda,
// 15  ,i2c0_scl,
// 16  ,uart0_rx,
// 17  ,uart0_tx,
// 18  ,        ,
// 19  ,        ,pwn1_0
// 20  ,        ,pwn1_1
// 21  ,        ,pwn1_2
// 22  ,        ,pwn1_3
// 23  ,        ,
// 24  ,uart1_rx,
// 25  ,uart1_tx,
// 26  ,spi1_cs0,
// 27  ,spi1_dq0/mosi,
// 28  ,spi1_dq1/miso,
// 29  ,spi1_sck,
// 30  ,spi1_dq2,
// 31  ,spi1_dq3,
/////  to Use GPIO, iof_en = 0 
////   to Use IOF0, iof_en = 1, iof_sel = 0
////   to Use IOF1, iof_en = 1, iof_sel = 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> // 為了 size_t

#ifndef _PINS_T_CORE_H
#define _PINS_T_CORE_H


//#ifndef GPIO_BIT(pin)
#define GPIO_BIT(pin) (1 << ((pin) + 0)) //GPIO[0] = ..0010 0000 0000
//#endif


#define NO_PWM  0xFF 
#define NO_UART 0xFF
#define NO_SPI  0xFF 
#define NO_I2C  0xFF

#define NO 0x0
#define YES 0x1

#define UNDEFINED 0x0
#define DIGITAL 0x1
#define ANALOG 0x2

typedef struct {
    uint8_t gpio_num;       // E203 GPIO接腳編號
    bool support_gpio;      // 紀錄是否可作T-core外部排針pin腳使用
    bool support_pwm;       // 紀錄是否可作T-core外部排針pin腳的PWM輸出使用
    uint8_t pwm_channel;    // 紀錄有無PWM功能和PWM頻道
    uint8_t uart_channel;   // 紀錄有無UART功能和UART頻道
    uint8_t spi_channel;    // 紀錄有無SPI功能和SPI頻道
    uint8_t i2c_channel;    // 紀錄有無I2C功能和I2C頻道
    const char* iof0_func;  // 紀錄IOF0功能
    const char* iof1_func;  // 紀錄IOF1功能
    uint8_t MODE;	    // 紀錄目前這個GPIO的功能（未定義、digital、analog、uart、spi、i2c等等）
    bool digital_available; // 紀錄這個GPIO是否可作為digital輸入輸出用
    bool analog_available;  // 紀錄這個GPIO是否可作為analog輸入輸出用
    const char* REMARKS;    // 備註
} GPIO_PinDescription;


//C++語法相容
#ifdef __cplusplus
extern "C" {
#endif

extern GPIO_PinDescription GPIO_pinMap[];
extern const size_t GPIO_PIN_COUNT;

#ifdef __cplusplus
}
#endif

#endif /*_PINS_T_CORE_H*/
