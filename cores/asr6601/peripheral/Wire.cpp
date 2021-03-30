/*
  TwoWire.cpp - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified December 2014 by Ivan Grokhotkov (ivan@esp8266.com) - esp8266 support
  Modified April 2015 by Hrsto Gochkov (ficeto@ficeto.com) - alternative esp8266 support
  Modified Nov 2017 by Chuck Todd (ctodd@cableone.net) - ESP32 ISR Support
 */

extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}

#include "Wire.h"
#include "Arduino.h"
#include "asr6601_i2c.h"

TwoWire Wire(0);
TwoWire Wire1(1);
TwoWire Wire2(2);




TwoWire::TwoWire(uint8_t bus_num)
    :_num(bus_num)
    ,_sda(-1)
    ,_scl(-1)
    ,_i2c(NULL)
    ,rxIndex(0)
    ,rxLength(0)
    ,rxQueued(0)
    ,txIndex(0)
    ,txLength(0)
    ,txAddress(0)
    ,txQueued(0)
    ,transmitting(0)
    ,last_error(I2C_ERROR_OK)
    ,_timeOutMillis(500)
{}

TwoWire::~TwoWire()
{
    flush();
    if(_i2c) {
        //i2cRelease(i2c);
        _i2c=NULL;
    }
}

bool TwoWire::begin(int sdaPin, int sclPin, uint32_t frequency)
{
	
	if(_num == 0 && sdaPin < 0 && sclPin < 0) {
		sdaPin = SDA;
		sclPin = SCL;
	}
	else if(_num == 1 && sdaPin < 0 && sclPin < 0) {
		sdaPin = SDA1;
		sclPin = SCL1;
	}
	else if(_num == 2 && sdaPin < 0 && sclPin < 0) {
		sdaPin = SDA2;
		sclPin = SCL2;
	}
	
	_sda = sdaPin;
	_scl = sclPin;
	
	_i2c = i2cInit(_num, sdaPin, sclPin, frequency);
	if(!_i2c) {
		return false;
	}
	flush();
	return true;
}
void TwoWire::end()
{
	i2cEnd(_i2c, _sda, _scl);
}
void TwoWire::setTimeOut(uint16_t timeOutMillis)
{
    _timeOutMillis = timeOutMillis;
}

uint16_t TwoWire::getTimeOut()
{
    return _timeOutMillis;
}

void TwoWire::setClock(uint32_t frequency)
{
    i2cSetFrequency(_i2c, frequency);
}

void TwoWire::setFrequency(uint32_t frequency)
{
    i2cSetFrequency(_i2c, frequency);
}

uint32_t TwoWire::getFrequency()
{
    return i2cGetFrequency(_i2c);
}

uint32_t TwoWire::getClock()
{
    return i2cGetFrequency(_i2c);
}

/* stickBreaker Nov 2017 ISR, and bigblock 64k-1
 */
i2c_err_t TwoWire::writeTransmission(uint16_t address, uint8_t *buff, uint16_t size, bool sendStop)
{
    last_error = i2cWrite(_i2c, address, buff, size, sendStop, _timeOutMillis);
    return last_error;
}

i2c_err_t TwoWire::readTransmission(uint16_t address, uint8_t *buff, uint16_t size, bool sendStop, uint32_t *readCount)
{
    last_error = i2cRead(_i2c, address, buff, size, sendStop, _timeOutMillis, readCount);
    return last_error;
}

void TwoWire::beginTransmission(uint16_t address)
{
    transmitting = 1;
    txAddress = address;
    txIndex = txQueued; // allow multiple beginTransmission(),write(),endTransmission(false) until endTransmission(true)
    txLength = txQueued;
    last_error = I2C_ERROR_OK;
}

/*stickbreaker isr
 */
uint8_t TwoWire::endTransmission(bool sendStop)  // Assumes Wire.beginTransaction(), Wire.write()
{
    if(transmitting == 1) {
            // txlength is howmany bytes in txbuffer have been use
        last_error = writeTransmission(txAddress, &txBuffer[txQueued], txLength - txQueued, sendStop);
        if(last_error == I2C_ERROR_CONTINUE){
            txQueued = txLength;
        } else if( last_error == I2C_ERROR_OK){
          rxIndex = 0;
          rxLength = rxQueued;
          rxQueued = 0;
          txQueued = 0; // the SendStop=true will restart all Queueing
        }
    } else {
        last_error = I2C_ERROR_NO_BEGIN;
        flush();
    }
    txIndex = 0;
    txLength = 0;
    transmitting = 0;
    return (last_error == I2C_ERROR_CONTINUE)?I2C_ERROR_OK:last_error; // Don't return Continue for compatibility.
}

/* @stickBreaker 11/2017 fix for ReSTART timeout, ISR
 */
uint8_t TwoWire::requestFrom(uint16_t address, uint8_t size, bool sendStop)
{
    //use internal Wire rxBuffer, multiple requestFrom()'s may be pending, try to share rxBuffer
    uint32_t cnt = rxQueued; // currently queued reads, next available position in rxBuffer
    if(cnt < (I2C_BUFFER_LENGTH-1) && (size + cnt) <= I2C_BUFFER_LENGTH) { // any room left in rxBuffer
        rxQueued += size;
    } else { // no room to receive more!
        cnt = 0;
        last_error = I2C_ERROR_MEMORY;
        flush();
        return cnt;
    }

    last_error = readTransmission(address, &rxBuffer[cnt], size, sendStop, &cnt);
    rxIndex = 0;
  
    rxLength = cnt;
  
    if( last_error != I2C_ERROR_CONTINUE){ // not a  buffered ReSTART operation
      // so this operation actually moved data, queuing is done.
        rxQueued = 0;
        txQueued = 0; // the SendStop=true will restart all Queueing or error condition
    }
  
    if(last_error != I2C_ERROR_OK){ // ReSTART on read does not return any data
        cnt = 0;
    }
  
    return cnt;
}

size_t TwoWire::write(uint8_t data)
{
    if(transmitting) {
        if(txLength >= I2C_BUFFER_LENGTH) {
            last_error = I2C_ERROR_MEMORY;
            return 0;
        }
        txBuffer[txIndex] = data;
        ++txIndex;
        txLength = txIndex;
        return 1;
    }
    last_error = I2C_ERROR_NO_BEGIN; // no begin, not transmitting
    return 0;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    for(size_t i = 0; i < quantity; ++i) {
        if(!write(data[i])) {
            return i;
        }
    }
    return quantity;

}

int TwoWire::available(void)
{
    int result = rxLength - rxIndex;
    return result;
}

int TwoWire::read(void)
{
    int value = -1;
    if(rxIndex < rxLength) {
        value = rxBuffer[rxIndex];
        ++rxIndex;
    }
    return value;
}

int TwoWire::peek(void)
{
    int value = -1;
    if(rxIndex < rxLength) {
        value = rxBuffer[rxIndex];
    }
    return value;
}

void TwoWire::flush(void)
{
    rxIndex = 0;
    rxLength = 0;
    txIndex = 0;
    txLength = 0;
    rxQueued = 0;
    txQueued = 0;
    i2cFlush(_i2c); // cleanup
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t TwoWire::requestFrom(uint16_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom(address, static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(uint16_t address, uint8_t quantity)
{
    return requestFrom(address, static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return static_cast<uint8_t>(requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop)));
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission(static_cast<uint16_t>(address));
}

void TwoWire::beginTransmission(uint8_t address)
{
    beginTransmission(static_cast<uint16_t>(address));
}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

/* stickbreaker Nov2017 better error reporting
 */
uint8_t TwoWire::lastError()
{
    return (uint8_t)last_error;
}

const char ERRORTEXT[] =
    "OK\0"
    "DEVICE\0"
    "ACK\0"
    "TIMEOUT\0"
    "BUS\0"
    "BUSY\0"
    "MEMORY\0"
    "CONTINUE\0"
    "NO_BEGIN\0"
    "\0";


char * TwoWire::getErrorText(uint8_t err)
{
    uint8_t t = 0;
    bool found = false;
    char * message = (char*)&ERRORTEXT;

    while(!found && message[0]) {
        found = t == err;
        if(!found) {
            message = message + strlen(message) + 1;
            t++;
        }
    }
    if(!found) {
        return NULL;
    } else {
        return message;
    }
}


