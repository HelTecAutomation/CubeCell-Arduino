/*
  This file is part of the ArduinoECCX08 library.
  Copyright (c) 2019 Arduino SA. All rights reserved.
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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "PWM1.h"

SoftwareSerial *SoftwareSerial::active_object = 0;
char SoftwareSerial::_receive_buffer[_SS_MAX_RX_BUFF];
volatile uint32_t SoftwareSerial::_receive_buffer_tail = 0;
volatile uint32_t SoftwareSerial::_receive_buffer_head = 0;

static uint8 sstate = 0; //
static uint8 bit = 0; // 0-10
static uint8 rbyte = 0;
static int32 waitDelay = 0;

bool SoftwareSerial::listen()
{
  if (!_rx_delay_stopbit)
    return false;

  if (active_object != this) {
    if (active_object) {
      active_object->stopListening();
    }

    _buffer_overflow = false;
    _receive_buffer_head = _receive_buffer_tail = 0;
    active_object = this;

    if (_inverse_logic)
      //Start bit high
      attachInterrupt(_receivePin, handle_interrupt, RISING);
    else
      //Start bit low
      attachInterrupt(_receivePin, handle_interrupt, FALLING);

    return true;
  }
  return false;
}

bool SoftwareSerial::stopListening() {
  if (active_object == this) {
    detachInterrupt(_receivePin);
    active_object = NULL;
    return true;
  }
  return false;
}


inline void SoftwareSerial::recv() {
  uint8_t d = 0;
  uint8_t ints;

  if(sstate > 0) {
	return; // we're busy in the other handler
  }
  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (_inverse_logic ? rx_pin_read() : !rx_pin_read()) {
    ints = CyEnterCriticalSection();
	sstate = 1; // first bit recv
        bit = 0;
        rbyte = 0;
        waitDelay = _rx_delay_centering;
        PWM1_BLOCK_CONTROL_REG |= PWM1_MASK; // start the timing interrupt
           PWM1_COUNTER_REG = (20000 & PWM1_16BIT_MASK);
            PWM1_PERIOD_REG = (0 & PWM1_16BIT_MASK);
            PWM1_COMP_CAP_REG = (1001-1 & PWM1_16BIT_MASK);

// adding these made it work again...
        PWM1_PERIOD_REG = (PWM1_TC_PERIOD_VALUE & PWM1_16BIT_MASK);
        PWM1_COUNTER_REG = (PWM1_TC_PERIOD_VALUE & PWM1_16BIT_MASK);

    PWM1_BLOCK_CONTROL_REG |= PWM1_MASK;
(* (reg32 *) CYREG_HSIOM_PORT_SEL6)=((* (reg32 *) CYREG_HSIOM_PORT_SEL6)&(~0x00000800u))|0x00000800u;

    PWM1_CONTROL_REG |= PWM1_MODE_TIMER_COMPARE;
    PWM1_CONTROL_REG &= (uint32)~PWM1_UPDOWN_MASK;
    PWM1_CONTROL_REG |= PWM1_COUNT_DOWN;
    PWM1_COMMAND_REG = (PWM1_MASK << PWM1_CMD_START);

//        Serial.println("Going to other handler");
     CyExitCriticalSection(ints);
   }
  
}


uint32_t SoftwareSerial::rx_pin_read() {
  return DIRECT_READ(_receiveBase, _receiveBitMask);
}

/* static */
inline void SoftwareSerial::handle_interrupt() {
  if (active_object) {
    active_object->recv();
  }
}


// Constructor
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic /* = false */) :
  _rx_delay_centering(0),
  _rx_delay_intrabit(0),
  _rx_delay_stopbit(0),
  _tx_delay(0),
  _buffer_overflow(false),
  _inverse_logic(inverse_logic)
{
  _receivePin = receivePin;
  _transmitPin = transmitPin;
}

// Destructor
SoftwareSerial::~SoftwareSerial() {
  end();
}

void SoftwareSerial::setTX(uint8_t tx) {
  // First write, then set output. If we do this the other way around,
  // the pin would be output low for a short while before switching to
  // output hihg. Now, it is input with pullup for a short while, which
  // is fine. With inverse logic, either order is fine.
  pinMode(tx, OUTPUT);
  _transmitBitMask = PIN_TO_BITMASK(tx);
  _transmitBase = PIN_TO_BASEREG(tx);
  _transmitPin = tx;
}

extern cyisraddress CyRamVectors[CYINT_IRQ_BASE + CY_NUM_INTERRUPTS];

void PWM1_ISR_SetVector(cyisraddress address)
{
    CyRamVectors[CYINT_IRQ_BASE + PWM1_ISR__INTC_NUMBER] = address;
}

void PWM1_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    PWM1_CONTROL_REG &= (uint32)~PWM1_ONESHOT_MASK;
    PWM1_CONTROL_REG |= ((uint32)((oneShotEnable & PWM1_1BIT_MASK) <<
                                                               PWM1_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


void PWM1_ISR_Enable(void)
{
    /* Enable the general interrupt. */
    *PWM1_ISR_INTC_SET_EN = PWM1_ISR__INTC_MASK;
}

void PWM1_ISR_Disable(void)
{
    /* Disable the general interrupt. */
    *PWM1_ISR_INTC_CLR_EN = PWM1_ISR__INTC_MASK;
}

void PWM1_ISR_SetPriority(uint8 priority)
{
	uint8 interruptState;
    uint32 priorityOffset = ((PWM1_ISR__INTC_NUMBER % 4u) * 8u) + 6u;
    
	interruptState = CyEnterCriticalSection();
    *PWM1_ISR_INTC_PRIOR = (*PWM1_ISR_INTC_PRIOR & (uint32)(~PWM1_ISR__INTC_PRIOR_MASK)) |
                                    ((uint32)priority << priorityOffset);
	CyExitCriticalSection(interruptState);
}


void PWM1_ISR_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    PWM1_ISR_Disable();

    /* Set the ISR to point to the PWM1_ISR Interrupt. */
    PWM1_ISR_SetVector(address);

    /* Set the priority. */
    PWM1_ISR_SetPriority((uint8)PWM1_ISR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    PWM1_ISR_Enable();
}

static uint32 cnt = 0;
static uint32 last = 0;

CY_ISR(PWM1_ISR_Interrupt)
{
    (* (reg32 *) CYREG_HSIOM_PORT_SEL6)=((* (reg32 *) CYREG_HSIOM_PORT_SEL6)&(~0x00000800u));

           PWM1_COUNTER_REG = (20000 & PWM1_16BIT_MASK);
            PWM1_PERIOD_REG = (0 & PWM1_16BIT_MASK);
            PWM1_COMP_CAP_REG = (1001-1 & PWM1_16BIT_MASK);
    PWM1_BLOCK_CONTROL_REG |= PWM1_MASK;
//    PWM1_CONTROL_REG |= PWM1_MODE_TIMER_COMPARE;
//    PWM1_CONTROL_REG &= (uint32)~PWM1_UPDOWN_MASK;
//    PWM1_CONTROL_REG |= PWM1_COUNT_DOWN;
//    PWM1_COMMAND_REG = (PWM1_MASK << PWM1_CMD_START);
//    PWM1_BLOCK_CONTROL_REG &= (uint32)~PWM1_MASK;

    uint8 enableInterrupts;
    if(cnt++ % 100000 == 0) {
       Serial.printf("state %d, bit %d, waitDelay %d\n", sstate, bit, waitDelay);	
    }
    if(sstate == 0) {
       return;
    }

    enableInterrupts = CyEnterCriticalSection();
    if(waitDelay > 0) {
	waitDelay -= 2; // 2uS
    } else {
//     Serial.printf("state2 %d, bit %d, waitDelay %d\n", sstate, bit, waitDelay);
     rbyte |= (DIRECT_READ(SoftwareSerial::active_object->_receiveBase, SoftwareSerial::active_object->_receiveBitMask) ? 1 : 0) << bit;	
     //rbyte |= (digialRead(SoftwareSerial::active_object->_receivePin) ? 1 : 0) << bit;
     bit++;
     waitDelay = SoftwareSerial::active_object->_rx_delay_intrabit;
     if(bit == 10) { // 11th bit, so finished
        // if buffer full, set the overflow flag and return
        uint32_t next = (SoftwareSerial::active_object->_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
        if (next != SoftwareSerial::active_object->_receive_buffer_head) {
           // save new data in buffer: tail points to where byte goes
           SoftwareSerial::active_object->_receive_buffer[SoftwareSerial::active_object->_receive_buffer_tail] = rbyte; // save new byte
           SoftwareSerial::active_object->_receive_buffer_tail = next;
       //    Serial.print(rbyte);
        } else {
           Serial.println("OVERFLOW");
           SoftwareSerial::active_object->_buffer_overflow = true;
        }
       sstate = 0; // ready for next byte
       PWM1_BLOCK_CONTROL_REG &= (uint32)~PWM1_MASK; // interrupt off.
     }
/*           PWM1_COUNTER_REG = (20000 & PWM1_16BIT_MASK);
            PWM1_PERIOD_REG = (0 & PWM1_16BIT_MASK);
            PWM1_COMP_CAP_REG = (1001-1 & PWM1_16BIT_MASK);
            Serial.printf("Start Counter %d period %d\n", (PWM1_COUNTER_REG & PWM1_16BIT_MASK), (PWM1_PERIOD_REG & PWM1_16BIT_MASK)$
            Serial.printf("Start compare register %d\n", (PWM1_COMP_CAP_REG & PWM1_16BIT_MASK));
    PWM1_BLOCK_CONTROL_REG |= PWM1_MASK;
    PWM1_CONTROL_REG |= PWM1_MODE_TIMER_COMPARE;
    PWM1_CONTROL_REG &= (uint32)~PWM1_UPDOWN_MASK;
    PWM1_CONTROL_REG |= PWM1_COUNT_DOWN;
    PWM1_COMMAND_REG = (PWM1_MASK << PWM1_CMD_START);
    PWM1_BLOCK_CONTROL_REG &= (uint32)~PWM1_MASK;
} */
}
//    Serial.println("Interrupt");
    CyExitCriticalSection(enableInterrupts);
}

void SoftwareSerial::setRX(uint8_t rx) {
  pinMode(rx, INPUT_PULLUP);
  _receiveBitMask = PIN_TO_BITMASK(rx);
  _receiveBase = PIN_TO_BASEREG(rx);
  _receivePin = rx;
}

void SoftwareSerial::begin(long speed) {
        PWM1_CONTROL_REG = PWM1_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        PWM1_TRIG_CONTROL1_REG  = PWM1_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        //PWM1_SetInterruptMode(PWM1_TC_INTERRUPT_MASK);
        // ****** this stopped the interrupt firing at all when set to PWM1_INTR_MASK_CC_MATCH (2)
	PWM1_INTERRUPT_MASK_REG = PWM1_TC_INTERRUPT_MASK; // orig, 1
//	PWM1_INTERRUPT_SET_REG = PWM1_INTR_MASK_CC_MATCH;
        
        /* Set other values from customizer */
        PWM1_SetOneShot(0);
	PWM1_PERIOD_REG = (PWM1_TC_PERIOD_VALUE & PWM1_16BIT_MASK);
        PWM1_COUNTER_REG = (PWM1_TC_PERIOD_VALUE & PWM1_16BIT_MASK);
        //PWM1_WritePeriod(PWM1_TC_PERIOD_VALUE );

	PWM1_ISR_StartEx(PWM1_ISR_Interrupt);
        (* (reg32 *) CYREG_HSIOM_PORT_SEL6)=((* (reg32 *) CYREG_HSIOM_PORT_SEL6)&(~0x00000800u))|0x00000800u;

    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    PWM1_COMP_CAP_REG = (1001-1 & PWM1_16BIT_MASK);
    PWM1_BLOCK_CONTROL_REG |= PWM1_MASK;

    PWM1_CONTROL_REG |= PWM1_MODE_TIMER_COMPARE;
    PWM1_CONTROL_REG &= (uint32)~PWM1_UPDOWN_MASK;
    PWM1_CONTROL_REG |= PWM1_COUNT_DOWN;
    PWM1_COMMAND_REG = (PWM1_MASK << PWM1_CMD_START);
    CyExitCriticalSection(enableInterrupts);

  setTX(_transmitPin);
  setRX(_receivePin);
  // Precalculate the various delays
  //Calculate the distance between bit in micro seconds
  uint32_t bit_delay = (uint32_t)(1000000) / (speed);

  _tx_delay = bit_delay;
Serial.printf("Bit delay for %d is %duS\n", speed, bit_delay);

  //Wait 1/2 bit - 2 micro seconds (time for interrupt to be served)
  _rx_delay_centering = (bit_delay / 2);
  //Wait 1 bit - 2 micro seconds (time in each loop iteration)
  _rx_delay_intrabit = bit_delay;
  //Wait 1 bit (the stop one)
  _rx_delay_stopbit = bit_delay;

  listen();
//  write(0x55); //send a dummy byte to sync the start byte
}

void SoftwareSerial::end() {
  stopListening();
}

uint32 SoftwareSerial::read() {
  if (!isListening()) {
    return -1;
  }


  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail) {
    return -1;
  }

  // Read from "head"
  noInterrupts();
  uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  interrupts();
  return d;
}


int SoftwareSerial::available() {
  if (!isListening())
    return 0;

  return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
}


size_t SoftwareSerial::write(uint8_t b) {
  if (_tx_delay == 0) {
    setWriteError();
    return 0;
  }

  // By declaring these as local variables, the compiler will put them
  // in registers _before_ disabling interrupts and entering the
  // critical timing sections below, which makes it a lot easier to
  // verify the cycle timings
  bool inv = _inverse_logic;
  uint16_t delay = _tx_delay;

  if (inv)
    b = ~b;
  // turn off interrupts for a clean txmit
  noInterrupts();
  // Write the start bit
  if (inv)
    DIRECT_WRITE_HIGH(_transmitBase, _transmitBitMask);
  else
    DIRECT_WRITE_LOW(_transmitBase, _transmitBitMask);  

  delayMicroseconds(delay);

  // Write each of the 8 bits
  for (uint8_t i = 0; i < 8; i++) {
    if (bitRead(b, i)) {
	DIRECT_WRITE_HIGH(_transmitBase, _transmitBitMask); 
    }
    else {
        DIRECT_WRITE_LOW(_transmitBase, _transmitBitMask);
    }
    delayMicroseconds(delay);
  }

  // restore pin to natural state
  if (inv)
    DIRECT_WRITE_LOW(_transmitBase, _transmitBitMask);
  else
    DIRECT_WRITE_HIGH(_transmitBase, _transmitBitMask);

  interrupts();
  delayMicroseconds(delay);

  return 1;
}

void SoftwareSerial::flush() {
  if (!isListening())
    return;

  noInterrupts();
  _receive_buffer_head = _receive_buffer_tail = 0;
  interrupts();

}

int SoftwareSerial::peek() {
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}
