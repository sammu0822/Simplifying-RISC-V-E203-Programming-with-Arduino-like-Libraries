/* 
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote prducts derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Contains code adapted from rosserial ArduinoHardware.h.
 * Upstream reference: https://github.com/ros-drivers/rosserial/blob/noetic-devel/rosserial_arduino/src/ros_lib/ArduinoHardware.h
 * Local adaptation: Platform selection and serial abstraction adapted to E203/TcoreSerial.
 * The original import revision and adaptation dates are not recorded.
 * Modified 2026-09-29: restored license and attribution notices only.
 * Full license also provided in ../../../LICENSE, Section C.
 */

//T-core abstractive serial layer

#ifndef _TCORE_SERIAL_HPP
#define _TCORE_SERIAL_HPP

extern "C" {
  #include "time.h"
  #include "T-core.h"
}
// #include "../../libraries/ros_lib/ros.h"

// inline void normalizeSecNSec(uint32_t, uint32_t)__attribute__((error("You must use ros::normalizeSecNSec(...)")));

// #if ARDUINO>=100
//   #include <Arduino.h>  // Arduino 1.0
// #else
//   #include <WProgram.h>  // Arduino 0022
// #endif

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__MKL26Z64__) || defined(__IMXRT1062__)
  #if defined(USE_TEENSY_HW_SERIAL)
    #define SERIAL_CLASS HardwareSerial // Teensy HW Serial
  #else
    #include <usb_serial.h>  // Teensy 3.0 and 3.1
    #define SERIAL_CLASS usb_serial_class
  #endif
#elif defined(_SAM3XA_)
  #include <UARTClass.h>  // Arduino Due
  #define SERIAL_CLASS UARTClass
#elif defined(USE_USBCON)
  // Arduino Leonardo USB Serial Port
  #define SERIAL_CLASS Serial_
#elif (defined(__STM32F1__) and !(defined(USE_STM32_HW_SERIAL))) or defined(SPARK) 
  // Stm32duino Maple mini USB Serial Port
  #define SERIAL_CLASS USBSerial
#elif defined(_T_CORE_H)
  // T-core Serial Port
  #include "serial.h"
  #define SERIAL_CLASS TcoreSerial
#else 
  #include <HardwareSerial.h>  // Arduino AVR
  #define SERIAL_CLASS HardwareSerial
#endif

class SerialHardware {
public:
  SerialHardware() : iostream(nullptr), baud_(9600) {} // 預設建構函式

  SerialHardware(SERIAL_CLASS* io , long baud= 9600){
    iostream = io;
    baud_ = baud;
  }


  void init() {// 初始化 UART //會在 nh.init() 時由ros::NodeHandle_<Hardware>自動呼叫
    // Serial_begin(115200);
    iostream->begin(baud_);
  }

  int read() {// 從 UART 讀 1 byte
    // return (int)Serial_read() ? (int)Serial_read() : -1; //如果沒有資料，回傳 -1
    return iostream->read();
  }

  void write(uint8_t* data, int length) {// 寫入 UART
    iostream->write(data, length);
  }

  void flush(){// 選擇性實作（可留空）
    flush();
  }

  // unsigned long time() { // 取得當前時間
  //   return millis(); // 這裡使用 Serial_millis() 來獲取毫秒級時間
  // }
  // uint32_t time() {
  //   return Serial_millis(); 
  // }

  void setBaud(long baud) { // 設定 UART 鮑率
    // Serial_begin(baud); //nh.getHardware()->setBaud(115200);來由外部靈活指定
    this->baud_= baud;
  }
  void setPort(SERIAL_CLASS* io) { 
    this->iostream = io;
  }// 設定 UART 實體指標

  int getBaud(){
    return baud_;
  }// 獲取當前 UART 鮑率

  unsigned long time(){
    return iostream->time();
  }// 獲取當前時間


  protected:
    SERIAL_CLASS* iostream;
    long baud_;
};


#endif /*_TCORE_SERIAL_HPP*/
