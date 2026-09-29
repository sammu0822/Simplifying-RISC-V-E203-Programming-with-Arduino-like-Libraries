#include "pins_T-core.h"


//腳位資訊結構，紀錄每個外部腳位的資訊（目前只有gpio，因此先宣告為GPIO_pinMap）
//未被定義的接腳(mode_undef == YES)無法被Read跟Write函數操控
//硬體沒有規劃的腳位support_GPIO先設成NO
GPIO_PinDescription GPIO_pinMap[] = {
//GPIO_number support_PIN support_PWM  PWM_ch   UART_ch   SPI_ch  I2C_ch    IOF0      IOF1        MODE   digit_able ana_able    REMARKS
  {	   0, 	      YES,	 YES,	0x00,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm0_0",  UNDEFINED,	YES,	YES,	""},
  {	   1,         YES,	 YES,	0x01,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm0_1",  UNDEFINED,	YES,	YES,	""},
  {	   2,         YES,	 YES,   0x02,  NO_UART,    0x00, NO_I2C,"spi0_cs0", "pwm0_2",  UNDEFINED,	YES,	YES,	""},
  {	   3,         YES,	 YES,   0x03,  NO_UART,    0x00, NO_I2C,"spi0_dq0", "pwm0_3",  UNDEFINED,	YES,	YES,	""},
  {	   4,         YES,	  NO, NO_PWM,  NO_UART,    0x00, NO_I2C,"spi0_dq1",       "",  UNDEFINED,	YES,	 NO,	""},
  {	   5,         YES,	  NO, NO_PWM,  NO_UART,    0x00, NO_I2C,"spi0_sck",       "",  UNDEFINED,	YES,	 NO,	""},
  {	   6,         YES,	  NO, NO_PWM,  NO_UART,    0x00, NO_I2C,"spi0_dq2",       "",  UNDEFINED,	YES,	 NO,	""},
  {	   7,         YES,	  NO, NO_PWM,  NO_UART,    0x00, NO_I2C,"spi0_dq3",       "",  UNDEFINED,	YES,	 NO,	""},
  {	   8,          NO,	  NO, NO_PWM,  NO_UART,    0x00, NO_I2C,"spi0_cs1",       "",  UNDEFINED,	YES,	 NO,	""},
  {	   9,          NO,	  NO, NO_PWM,  NO_UART,    0x00, NO_I2C,"spi0_cs2",       "",  UNDEFINED,	YES,	 NO,	""},
  {   10,         YES,	  YES,   0x20,  NO_UART,    0x00, NO_I2C,"spi0_cs3", "pwm2_0",  UNDEFINED,	YES,	YES,	""},
  {   11,         YES,	  YES,   0x21,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm2_1",  UNDEFINED,	YES,	YES,	""},
  {   12,         YES,	  YES,   0x22,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm2_2",  UNDEFINED,	YES,	YES,	""},
  {   13,         YES,	  YES,   0x23,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm2_3",  UNDEFINED,	YES,	YES,	""},
  {   14,         YES,	  NO, NO_PWM,  NO_UART,  NO_SPI,   0x00,"i2c0_sda",       "",  UNDEFINED,	YES,	 NO,	""},
  {   15,         YES,	  NO, NO_PWM,  NO_UART,  NO_SPI,   0x00,"i2c0_scl",       "",  UNDEFINED,	YES,	 NO,	""},
  {   16,          NO,	  NO, NO_PWM,     0x00,  NO_SPI, NO_I2C,"uart0_rx",       "",  UNDEFINED,	YES,	 NO,	"pc usb"},
  {   17,          NO,	  NO, NO_PWM,     0x00,  NO_SPI, NO_I2C,"uart0_tx",       "",  UNDEFINED,	YES,	 NO,	"pc usb"},
  {   18,         YES,	  NO, NO_PWM,  NO_UART,  NO_SPI, NO_I2C,	"",       "",  UNDEFINED,	YES,	 NO,	""},
  {   19,         YES,	  YES,   0x10,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm1_0",  UNDEFINED,	YES,	YES,	""},
  {   20,         YES,	  YES,   0x11,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm1_1",  UNDEFINED,	YES,	YES,	""},
  {   21,         YES,	  YES,   0x12,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm1_2",  UNDEFINED,	YES,	YES,	""},
  {   22,         YES,	  YES,   0x13,  NO_UART,  NO_SPI, NO_I2C,	"", "pwm1_3",  UNDEFINED,	YES,	YES,	""},
  {   23,         YES,	  NO, NO_PWM,  NO_UART,  NO_SPI, NO_I2C,	"",       "",  UNDEFINED,	YES,	 NO,	""},
  {   24,          NO,	  NO, NO_PWM, 	  0x01,  NO_SPI, NO_I2C,"uart1_rx",       "",  UNDEFINED,	 NO,	 NO,	""},
  {   25,          NO,	  NO, NO_PWM, 	  0x01,  NO_SPI, NO_I2C,"uart1_tx",       "",  UNDEFINED,	 NO,	 NO,	""},
  {   26,          NO,	  NO, NO_PWM,  NO_UART,    0x01, NO_I2C,"spi1_cs0",       "",  UNDEFINED,	 NO,	 NO,	""},
  {   27,          NO,	  NO, NO_PWM,  NO_UART,    0x01, NO_I2C,"spi1_dq0",       "",  UNDEFINED,	 NO,	 NO,	""},
  {   28,          NO,	  NO, NO_PWM,  NO_UART,    0x01, NO_I2C,"spi1_dq1",       "",  UNDEFINED,	 NO,	 NO,	""},
  {   29,          NO,	  NO, NO_PWM,  NO_UART,    0x01, NO_I2C,"spi1_sck",       "",  UNDEFINED,	 NO,	 NO,	""},
  {   30,          NO,	  NO, NO_PWM,  NO_UART,    0x01, NO_I2C,"spi1_dq2",       "",  UNDEFINED,	 NO,	 NO,	""},
  {   31,          NO,	  NO, NO_PWM,  NO_UART,    0x01, NO_I2C,"spi1_dq3",       "",  UNDEFINED,	 NO,	 NO,	""}
};

//計算結構大小
const size_t GPIO_PIN_COUNT = sizeof(GPIO_pinMap) / sizeof(GPIO_PinDescription);
