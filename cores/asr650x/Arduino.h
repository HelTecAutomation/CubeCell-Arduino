/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ASR_Arduino.h>
#include <HardwareSerial.h>
#include <CyLib.h> 
#include "radio.h"
#include "board.h"
#include "timer.h"
#include "rtc-board.h"
#include "hw.h"
#include "low_power.h"
#include "asr_timer.h"

#define ARDUINO_ARCH_SAMD 1

//extern HardwareSerial Serial;

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define LSBFIRST 0
#define MSBFIRST 1

#define  lowByte(w)   ((w) & 0xff) 
#define  highByte(w)   ((w) >> 8) 
#define  bitRead(value, bit)   (((value) >> (bit)) & 0x01) 
#define  bitWrite(value, bit, bitvalue)   (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define  bitSet(value, bit)   ((value) |= (1UL << (bit))) 
#define  bitClear(value, bit)   ((value) &= ~(1UL << (bit))) 
#define  bit(b)   (1 << (b)) 

#define min(a, b) ((a)<(b)?(a):(b))
#define max(a, b) ((a)>(b)?(a):(b))
#define abs(x)   ((x)>0?(x):-(x))
#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#define noInterrupts() CyGlobalIntDisable
#define interrupts()   CyGlobalIntEnable

#define clockCyclesPerMicrosecond() ( 48L ) // clock cycle 48 MHz
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

// some libraries and sketches depend on this AVR stuff,
// assuming Arduino.h or WProgram.h automatically includes it...
//


// Include Atmel headers

void yield( void ) ;

/* system functions */
int main( void );
void init( void );

/* sketch */
void setup( void ) ;
void loop( void ) ;


#endif // Arduino_h
