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

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
  Definitions
******************************************************************************/
#define PIN_IN_PORT(pin)    (pin % PIN_NUMBER_IN_PORT)
#define PORT_FROM_PIN(pin)  (pin / PIN_NUMBER_IN_PORT)
#define PORT_OFFSET(port)   (PORT_REG_SHFIT * port)
#define PORT_ADDRESS(pin)   (CYDEV_GPIO_BASE + PORT_OFFSET(PORT_FROM_PIN(pin)))

#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, pin)          CY_SYS_PINS_READ_PIN(PORT_ADDRESS(pin)+4, PIN_IN_PORT(pin))
#define DIRECT_WRITE_LOW(base, pin)     CY_SYS_PINS_CLEAR_PIN(PORT_ADDRESS(pin), PIN_IN_PORT(pin))
#define DIRECT_WRITE_HIGH(base, pin)    CY_SYS_PINS_SET_PIN(PORT_ADDRESS(pin), PIN_IN_PORT(pin))
#define DIRECT_MODE_INPUT(base, pin)    CY_SYS_PINS_SET_DRIVE_MODE(PORT_ADDRESS(pin)+8, PIN_IN_PORT(pin), CY_SYS_PINS_DM_DIG_HIZ)
#define DIRECT_MODE_OUTPUT(base, pin)   CY_SYS_PINS_SET_DRIVE_MODE(PORT_ADDRESS(pin)+8, PIN_IN_PORT(pin), CY_SYS_PINS_DM_STRONG)


#define _SS_MAX_RX_BUFF 1024 // RX buffer size
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class SoftwareSerial : public Stream {
  public:
    // per object data
    uint8_t _transmitPin;
    uint8_t _receivePin;
    uint32_t  _transmitBase;
    uint32_t _receiveBase;
    uint32_t _receiveBitMask;
    uint32_t _transmitBitMask;

    // Expressed as 4-cycle delays (must never be 0!)
    uint16_t _rx_delay_centering;
    uint16_t _rx_delay_intrabit;
    uint16_t _rx_delay_stopbit;
    uint16_t _tx_delay;

    uint16_t _buffer_overflow: 1;
    uint16_t _inverse_logic: 1;

    // static data
    static char _receive_buffer[_SS_MAX_RX_BUFF];
    static volatile uint32_t _receive_buffer_tail;
    static volatile uint32_t _receive_buffer_head;
    static SoftwareSerial *active_object;

    // private methods
    void recv() __attribute__((__always_inline__));
    uint32_t rx_pin_read();
    void tx_pin_write(uint8_t pin_state) __attribute__((__always_inline__));
    void setTX(uint8_t transmitPin);
    void setRX(uint8_t receivePin);
    void setRxIntMsk(bool enable) __attribute__((__always_inline__));


  public:
    // public methods
    SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
    ~SoftwareSerial();
    void begin(long speed);
    bool listen();
    void end();
    bool isListening() {
      return this == active_object;
    }
    bool stopListening();
    bool overflow() {
      bool ret = _buffer_overflow;
      if (ret) _buffer_overflow = false;
      return ret;
    }
    int peek();

    virtual size_t write(uint8_t byte);
    virtual uint32 read();
    virtual int available();
    virtual void flush();
    operator bool() {
      return true;
    }

    using Print::write;

    // public only for easy access by interrupt handlers
    static inline void handle_interrupt() __attribute__((__always_inline__));
};

#endif
