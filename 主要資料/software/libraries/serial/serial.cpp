/*
  SPDX-License-Identifier: LGPL-2.1-or-later

  Contains code adapted from HardwareSerial.cpp, ArduinoCore-avr.
  Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; see Section B of ../../../LICENSE.
  If not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.

  Upstream modification notices:
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman

  Local adaptation: Register access and UART setup adapted for E203; serial events and buffering retained.
  The original import revision and historical adaptation dates are not
  recorded in this project. The upstream notices above are retained;
  they do not describe the dates of the E203 port.
  Modified 2026-09-29: restored license and attribution notices only.
  Upstream reference:
  https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.cpp
*/

#include "T-core.h"

#include "serial.h"
#include "time_utils.h"
#include "wiring_digital.h"  // 這裡定義了 digitalRead, digitalWrite, pinMode
#include <stdint.h>


#define GPIO_PIN_INOUT0 0  // 第 0 腳接 LED
#define GPIO_PIN_INOUT1 1  // 第 1 腳接 LED
#define GPIO_PIN_INOUT2 2  // 第 2 腳接 LED

// The next line disables the entire serial.cpp
#if defined(HAVE_SERIAL0)


// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// HardwareSerial instance if the user doesn't also refer to it.
#if defined(HAVE_SERIAL0)
  void serialEvent() __attribute__((weak));
  bool Serial0_available() __attribute__((weak));
#else
	#error "HAVE_SERIAL0 is not defined. Please include the correct header file."
#endif


void serialEventRun(void)
{//?
	#if defined(HAVE_SERIAL0)
		if (Serial0_available && serialEvent && Serial0_available()) serialEvent();
	#endif
}


// #ifdef (HAVE_SERIAL0)
// 	bool Serial0_available() __attribute__((weak));
// #endif



// ------------  Private Methods -----------------
int TcoreSerial::scan(char *buf) // 用於掃描 UART 接收的字元
{//x
	char c = 0; // 初始化接收字元
	int exit = 0; // 初始化退出標誌
	int len = 0; // 初始化接收長度

	while (exit != 1)
	{
		if (available()) // 如果有資料可讀 uart_rx(&c)==1
		{
			c = read(); // 讀取一個字元
			if (c == '\r') // 當輸入 'enter' 時，退出循環
			{
				exit = 1;
			}
			else
			{
				buf[len] = c; // 儲存輸入的資料
				len++;
				write(c); // 回顯輸入的字元
			}
		}
	}
	return len; // 返回資料長度
}

static void TcoreSerial::uart_tx(char c)//�Ω�o�e��r 
{//x
	while ((UART0_REG(UART_REG_TXFIFO) >> 31) & 1 == 1); //check fifo status,send new data if fifo isn't full
	
	UART0_REG(UART_REG_TXFIFO) = c ;
} 

int TcoreSerial::uart_rx(char *c)//�Ω󱵦���r 
{//x
	uint32_t val = UART0_REG(UART_REG_RXFIFO); // get rxfifo data
	
	if ((val >> 31)==0) // check fifo status,return data if not empty
	{
		*c = val & 0xff; // only 8bit of data is valid
		return 1;	
	}
	return 0; // fifo is empty , return 0
}


// ------------  Public Methods -----------------
void HardwareSerial::_tx_udr_empty_irq(void) //當 UART 傳輸暫存器空時（由中斷觸發），從 TX buffer 取資料送出，並維護相關旗標
{//x
	// If interrupts are enabled, there must be more data in the output
  	// buffer. Send the next byte
	// 如果啟用中斷，代表還有資料要送，從暫存區取下一個 byte
	unsigned char c = _tx_buffer[tx_tail];
  	tx_tail = (tx_tail + 1) % SERIAL_TX_BUFFER_SIZE;

	uart_tx(c);// 寫入 UART 資料暫存器

	// 清除 TXC 旗標（代表傳送完成），以確保 flush() 正確運作

	// 如果暫存區空了就停用中斷

}

void TcoreSerial::begin(uint32_t baudrate)
{//x
	GPIO_REG(GPIO_IOF_EN)	|= IOF0_UART0_MASK;
	GPIO_REG(GPIO_IOF_SEL)	&= ~IOF0_UART0_MASK;
	
	//115200 Baud Rate -> 138
	//get_cpu_freq() / baud_rate - 1, and get_cpu_freq () = 16MHz
	UART0_REG(UART_REG_DIV) = (16000000 / baudrate) - 1;
	// UART0_REG(UART_REG_DIV) = 138;
	

	UART0_REG(UART_REG_TXCTRL) |= UART_TXEN;// enable tx
	UART0_REG(UART_REG_RXCTRL) |= UART_RXEN;// enable rx 

	_written = false;
}

void TcoreSerial::end(void)
{//x

	flush(); // 等待傳送完畢

	// -- 關閉收發及相關中斷 --
	
  	// Disable the UART by clearing the TXEN and RXEN bits
  	// 停用 UART，清除 TXEN 和 RXEN 位元
	UART0_REG(UART_REG_TXCTRL) &= ~UART_TXEN;// disable tx
  	UART0_REG(UART_REG_RXCTRL) &= ~UART_RXEN;// disable rx

	// disable rx and tx interrupts ︱ 停用 RX 和 TX 中斷
	// UART0_REG(UART_REG_IE) &= ~(UART_IE_RXIE | UART_IE_TXIE); 

	// clear any received data ｜ 清空接收 buffer
	rx_head = rx_tail;
}

int TcoreSerial::available(void) //回傳 RX buffer 裡尚未讀出的資料量
{//o
	return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + rx_head - rx_tail)) % SERIAL_RX_BUFFER_SIZE; // 計算目前接收緩衝區（SERIAL_RX_BUFFER_SIZE）有多少資料可以讀取
	// return !(UART0_REG(UART_REG_RXFIFO) >> 31); //fifo is empty , return 0  //bit31 (empty域) = 0 表示有資料 (數據有效)
}

int HardwareSerial::peek(void) //不取出，只看rx buffer 下一個 byte
{//o
	if (rx_head == rx_tail) { // 如果接收緩衝區為空

		return -1; // 沒有資料可讀，返回 -1

	}else{

		return _rx_buffer[rx_tail]; // 返回接收緩衝區的下一個字元，但不移除它

	}
}

int TcoreSerial::read(void) //取出一個byte並移動 RX buffer tail 指標 
{//o
	// pinMode(GPIO_PIN_INOUT0, OUTPUT);
	// pinMode(GPIO_PIN_INOUT1, OUTPUT);
	// pinMode(GPIO_PIN_INOUT2, OUTPUT);
	// digitalWrite(GPIO_PIN_INOUT0, LOW);
	// if (!available()) return 0; // check fifo status //等待有資料 
	// digitalWrite(GPIO_PIN_INOUT0, HIGH);
	// return (int)(UART0_REG(UART_REG_RXFIFO) & 0xff); // only 8bit of data is valid

	if(rx_head == rx_tail) // 如果接收緩衝區為空
	{
		return -1; // 沒有資料可讀，返回 -1 ( 改成 int，可返回 -1 ， 沒資料就回 -1，這是 rosserial 要求 )

	}else{

		unsigned char c = _rx_buffer[rx_tail]; // 讀取接收緩衝區的資料
		rx_tail = (rx_buffer_index_t)(rx_tail + 1) % SERIAL_RX_BUFFER_SIZE; // 更新接收緩衝區尾指針
		return (int)c; // 返回讀取的字元

	}
}

int TcoreSerial::availableForWrite(void) //查詢串列傳送暫存區（TX buffer）還剩多少空間可以寫入 （ 需要原子操作 ）
{//x
	tx_buffer_index_t head; // 下一個要寫入的位置
  	tx_buffer_index_t tail; // 下一個要送出的資料位置
	
	// ATOMIC_BLOCK 
	// TX_BUFFER_ATOMIC {
	// 	head = _tx_buffer_head;
	// 	tail = _tx_buffer_tail;
	// }

	// return !(UART0_REG(UART_REG_TXFIFO) >> 31); //fifo is full , return 0 //bit31 (full域) = 0 表示有空間可以寫入
	return ((unsigned int)(SERIAL_TX_BUFFER_SIZE + tx_head - tx_tail)) % SERIAL_TX_BUFFER_SIZE; // 計算目前傳輸緩衝區（SERIAL_TX_BUFFER_SIZE）有多少空間可以寫入
}

void TcoreSerial::flush(void) // 等待全部資料都送出（TXC 旗標設置）
{//x
	if (!_written) return; // 如果沒有資料寫入過，直接返回
    while (((UART0_REG(UART_REG_TXFIFO) >> 31) & 1) == 1);  // 等待直到 FIFO 有空位可寫 （清空暫存器）

	_tx_udr_empty_irq(); // 呼叫中斷處理函式，確保所有資料都已送出
}

size_t TcoreSerial::write(uint8_t c) //傳送一個 byte，必要時等 buffer，有空直接送出，否則排入 buffer （ 需要原子操作 ）
{//x
	_written = true;// 設置已寫入標誌 //代表已開始寫資料（這個旗標在 flush() 會用到）

	if((tx_head == tx_tail) && (((UART0_REG(UART_REG_TXFIFO) >> 31) & 1) == 0)) // 如果 TX buffer 空且 UART 傳輸暫存器為空
	{
		UART0_REG(UART_REG_TXFIFO) = c ; // 直接寫入 FIFO
		return 1;
	}
	// else // 否則，將資料寫入 TX buffer
	// {
	// 	tx_buffer_index_t i = (tx_head + 1) % SERIAL_TX_BUFFER_SIZE; // 計算下一個要寫入的位置

	// 	// 等待直到有空間可以寫入（避免覆蓋未送出的資料）
	// 	while (i == tx_tail); // 等待直到有空間可以寫入（避免覆蓋未送出的資料）

	// 	tx_buffer[tx_head] = c; // 將資料寫入 TX buffer
	// 	tx_head = i; // 更新 TX buffer 頭指針

	// 	// 啟用傳輸暫存器空中斷，以便開始傳送資料
	// 	_tx_udr_empty_irq(); // 呼叫中斷處理函式，開始傳送資料
	// }
}


// ------------ Customized Methods -----------------
uint32_t TcoreSerial::time(void)
{//x
	return millis();
}

void TcoreSerial::print(const char *str)
{//x	
	while (*str != '\0') // the string is end of '\0'
		write(*str++);
}

void TcoreSerial::println(const char *str)
{//x
	print(str);
	write('\r');
  	write('\n');
}


#endif // whole file

/*--------------------------------------------------
基地址外設，應該在platform.h有定義
#define UART_BASE_ADDR    0x10013000
#define UART_TXDATA       (*(volatile uint32_t *)(UART_BASE_ADDR + 0x00))
#define UART_RXDATA       (*(volatile uint32_t *)(UART_BASE_ADDR + 0x04))
#define UART_TXCTRL       (*(volatile uint32_t *)(UART_BASE_ADDR + 0x08))
#define UART_RXCTRL       (*(volatile uint32_t *)(UART_BASE_ADDR + 0x0C))
#define UART_IE           (*(volatile uint32_t *)(UART_BASE_ADDR + 0x10))
#define UART_IP           (*(volatile uint32_t *)(UART_BASE_ADDR + 0x14))
#define UART_DIV          (*(volatile uint32_t *)(UART_BASE_ADDR + 0x18))
---------------------------------------------------*/

// void TcoreSerial::print_time(void)
// {
//     uint32_t t = millis();
//     // 將數字轉字串後透過 Serial_print() 輸出
// }


// char Serial_read(void)
// int Serial_read(void) // 改成 int，可返回 -1
// {//o		
// 	if (!Serial_available()) return -1; // check fifo status //等待有資料 // 沒資料就回 -1，這是 rosserial 要求
// 	return (char)(UART0_REG(UART_REG_RXFIFO) & 0xff); // only 8bit of data is valid
// }


// void TcoreSerial::write(char c)// 一次寫入一個字元
// {//x
// 	digitalWrite(GPIO_PIN_INOUT1, HIGH);
// 	while (((UART0_REG(UART_REG_TXFIFO) >> 31) & 1 ) == 1); //check fifo status,send new data if fifo isn't full
// 	UART0_REG(UART_REG_TXFIFO) = (uint8_t)c ; // only 8bit of data is valid
// }

// void TcoreSerial::write(uint8_t* data, int length)// 寫入多個字元
// {//x
// 	digitalWrite(GPIO_PIN_INOUT2, HIGH);
// 	if (data == nullptr || length <= 0) return; //防止錯誤地傳入錯誤長度或 data 無效，導致 Serial_write() 寫入未定義記憶體。

//     for (int i = 0; i < length; ++i) {
//     //   Serial_write(data[i]); //(char)data[i] -> data[i]
// 		while (((UART0_REG(UART_REG_TXFIFO) >> 31) & 1 ) == 1); //check fifo status,send new data if fifo isn't full
// 		UART0_REG(UART_REG_TXFIFO) = (uint8_t)data[i] ; // only 8bit of data is valid
//     }
// }
