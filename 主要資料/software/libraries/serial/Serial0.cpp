/*
  SPDX-License-Identifier: LGPL-2.1-or-later

  Contains code adapted from HardwareSerial0.cpp, ArduinoCore-avr.
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

  Local adaptation: Serial instance and availability wrapper adapted to TcoreSerial.
  The original import revision and historical adaptation dates are not
  recorded in this project. The upstream notices above are retained;
  they do not describe the dates of the E203 port.
  Modified 2026-09-29: restored license and attribution notices only.
  Upstream reference:
  https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial0.cpp
*/

#include "T-core.h"
#include "serial.h"

#if defined(_SERIAL_H)
//硬體中斷處理函數
// ISR(USART2_UDRE_vect)
// {
//   Serial2._tx_udr_empty_irq();
// }



TcoreSerial Serial;


// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
// int available() {// UART RX 是否有資料
bool Serial0_available() {// UART RX 是否有資料
    return Serial.available() ? 1 : 0;
}

// bool Serial2_available() {
//   return Serial2.available();
// }

#endif 

