/*
  SPDX-License-Identifier: LGPL-2.1-or-later

  Contains code adapted from HardwareSerial.h, ArduinoCore-avr.
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
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman

  Local adaptation: Serial interface and buffer declarations adapted to TcoreSerial and E203.
  The original import revision and historical adaptation dates are not
  recorded in this project. The upstream notices above are retained;
  they do not describe the dates of the E203 port.
  Modified 2026-09-29: restored license and attribution notices only.
  Upstream reference:
  https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.h
*/

#ifndef _SERIAL_H
#define _SERIAL_H

#include <inttypes.h>   //不同寬度的整數類型提供輸入和輸出轉換宏，如果要讓C++ 與C 程式碼互通時可使用

#include <stdint.h>
#include <stdbool.h>
#include "platform.h"
#include "pins_T-core.h"
#include "T-core.h"

typedef uint8_t rx_buffer_index_t;
typedef uint8_t tx_buffer_index_t;

#if !defined(SERIAL_TX_BUFFER_SIZE)
#define SERIAL_TX_BUFFER_SIZE 64    // 預設傳輸緩衝區大小
#endif

#if !defined(SERIAL_RX_BUFFER_SIZE)
#define SERIAL_RX_BUFFER_SIZE 64    // 預設接收緩衝區大小
#endif


class TcoreSerial 
{
    protected:
        // volatile char tx_buffer[SERIAL_TX_BUFFER_SIZE];      // 傳輸緩衝區
        // volatile char rx_buffer[SERIAL_RX_BUFFER_SIZE];      // 接收緩衝區
        unsigned char tx_buffer[SERIAL_TX_BUFFER_SIZE];         // 傳輸緩衝區
        unsigned char rx_buffer[SERIAL_RX_BUFFER_SIZE];         // 接收緩衝區

        // int tx_head;     // 傳輸緩衝區頭指針
        // int tx_tail;     // 傳輸緩衝區尾指針
        // int rx_head;     // 接收緩衝區頭指針
        // int rx_tail;     // 接收緩衝區尾指針
        volatile tx_buffer_index_t tx_head;     // 傳輸緩衝區頭指針
        volatile tx_buffer_index_t tx_tail;     // 傳輸緩衝區尾指針
        volatile rx_buffer_index_t rx_head;     // 接收緩衝區頭指針
        volatile rx_buffer_index_t rx_tail;     // 接收緩衝區尾指針

        // Has any byte been written to the UART since begin()
        bool _written; // 是否有資料寫入 UART


    private:
        int scan(char *buf);            // 用於掃描 UART 接收的字元
        static void uart_tx(char c);    // 傳輸一個字元
        int uart_rx(char *c);           // 接收一個字元


    public:
        TcoreSerial() = default;                    // 預設建構函式
        void begin(uint32_t baudrate);              // 初始化序列埠，設定波特率
        void end(void);                             // 停止序列埠通訊
        virtual int available(void);                // 檢查是否有資料可讀 ->改成用來計算目前接收緩衝區（）ERIAL_RX_BUFFER_SIZE）有多少資料可以讀取
        virtual int peek(void);                     // 檢查下一個可讀的字元，但不移除它
        virtual int read(void);                     // 讀取一個字元，返回 -1 表示無資料  // char Serial_read(void);
        virtual int availableForWrite(void);        // 檢查是否有空間可以寫入資料
        virtual void flush(void);                   // 清空傳輸緩衝區
        virtual size_t write(uint8_t);              // 寫入一個字元

        inline size_t write(unsigned long n) { return write((uint8_t)n); }      // 寫入一個無符號長整數
        inline size_t write(int n) { return write((uint8_t)n); }                // 寫入一個整數
        inline size_t write(unsigned int n) { return write((uint8_t)n); }       // 寫入一個無符號整數
        inline size_t write(long n) { return write((uint8_t)n); }               // 寫入一個長整數
        // inline void write(char c);                                           // 寫入一個字元
        // inline void write(uint8_t* data, int length);                        // 寫入多個字元


        void print(const char *str);                // 輸出字串
        void println(const char *str);              // 輸出字串並換行
        uint32_t time(void);                        // 返回當前時間（毫秒）
        // void print_time(void);

        // Interrupt handlers - Not intended to be called externally
        // 中斷處理函式 - 不打算在外部調用
        inline void _rx_complete_irq(void);
        void _tx_udr_empty_irq(void);
};

#if defined(_T_CORE_H)
extern TcoreSerial Serial; // 定義全域的 TcoreSerial 實例
#define HAVE_SERIAL0
#else
    #error "TcoreSerial is not defined. Please include the correct header file."
// extern TcoreSerial Serial0; // 定義全域的 TcoreSerial 實例
#endif

extern void serialEventRun(void) __attribute__((weak));

#endif /*_SERIAL_H*/
/*-----------------------------------------------------
// extern TcoreSerial available() __attribute__((weak));

// #ifdef __cplusplus
// extern "C" {
// #endif

// void Serial_begin(uint32_t baudrate);
// void Serial_end(void);
// bool Serial_available(void);
// // char Serial_read(void);
// int Serial_read(void);
// void Serial_write(char c);
// void Serial_print(const char *str);
// void Serial_println(const char *str);
// void Serial_flush(void);
// void Serial_print_time(void);
// uint32_t Serial_millis(void);


// #ifdef __cplusplus
// }
// #endif


// // Define config for Serial.begin(baud, config);
// #define SERIAL_5N1 0x00
// #define SERIAL_6N1 0x02
// #define SERIAL_7N1 0x04
// #define SERIAL_8N1 0x06
// #define SERIAL_5N2 0x08
// #define SERIAL_6N2 0x0A
// #define SERIAL_7N2 0x0C
// #define SERIAL_8N2 0x0E
// #define SERIAL_5E1 0x20
// #define SERIAL_6E1 0x22
// #define SERIAL_7E1 0x24
// #define SERIAL_8E1 0x26
// #define SERIAL_5E2 0x28
// #define SERIAL_6E2 0x2A
// #define SERIAL_7E2 0x2C
// #define SERIAL_8E2 0x2E
// #define SERIAL_5O1 0x30
// #define SERIAL_6O1 0x32
// #define SERIAL_7O1 0x34
// #define SERIAL_8O1 0x36
// #define SERIAL_5O2 0x38
// #define SERIAL_6O2 0x3A
// #define SERIAL_7O2 0x3C
// #define SERIAL_8O2 0x3E
-----------------------------------------------------*/

