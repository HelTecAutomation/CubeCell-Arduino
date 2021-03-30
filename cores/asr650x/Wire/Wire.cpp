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
#include "project.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include <ASR_Arduino.h>



TwoWire::TwoWire(int8_t bus_num)
    :_i2c_num(bus_num)
    ,_freq(100000)
    ,_sda(-1)
    ,_scl(-1)
    ,rxIndex(0)
    ,rxLength(0)
    ,rxQueued(0)
    ,txIndex(0)
    ,txLength(0)
    ,txAddress(0)
    ,txQueued(0)
    ,last_error(I2C_ERROR_OK)
    ,transmitting(0)
    ,_timeOutMillis(50)
{}

TwoWire::~TwoWire()
{
}

bool TwoWire::begin(int sda, int scl, uint32_t frequency)
{

	if(_i2c_num == 0 && sda < 0 && scl < 0) {
		_sda = SDA;
		_scl = SCL;
	}
	else if(sda == SDA && scl == SCL)
	{
		_i2c_num = 0;
	}
#ifdef __ASR6502__
	else if(_i2c_num == 1 && sda < 0 && scl < 0) {
		_sda = SDA1;
		_scl = SCL1;
	}
	else if(sda == SDA1 && scl == SCL1)
	{
		_i2c_num = 1;
	}
#endif
	else
	{
		return false;
	}
	_freq = frequency;

	if(_freq > 1000000)
	{
		_freq = 1000000;
	}

	if(_i2c_num == I2C_NUM_0)
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ / 8 / _freq / 3;
		I2C_SCBCLK_DIV_REG = div << 8 ;
		I2C_Start();
	}
	else
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ / 8 / _freq / 3;
		I2C_1_SCBCLK_DIV_REG = div << 8 ;
		I2C_1_Start();
	}
    return true;
}

void TwoWire::setFrequency(uint32_t freq)
{
	_freq = freq;
	
	if(_freq > 1000000)
	{
		_freq = 1000000;
	}

	if(_i2c_num == 0)
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ / SPI_1_SPI_OVS_FACTOR / _freq - 1;
		SPI_1_SCBCLK_DIV_REG = div << 8 ;
	}
	else
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ / SPI_2_SPI_OVS_FACTOR / _freq - 1;
		SPI_2_SCBCLK_DIV_REG = div << 8 ;
	}
}


void TwoWire::end()
{
	if(_i2c_num == I2C_NUM_0)
	{
		I2C_Stop();
	}
	else
	{
		I2C_1_Stop();
	}
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
    setFrequency(frequency);
}

size_t TwoWire::getClock()
{
    return _freq;
}

/* stickBreaker Nov 2017 ISR, and bigblock 64k-1
 */
i2c_err_t TwoWire::writeTransmission(uint16_t address, uint8_t *buff, uint16_t size, bool sendStop)
{
	uint8_t Status=0;
	uint16_t i;
	flush();
	if(_i2c_num == I2C_NUM_0)
	{
		I2C_I2CMasterClearStatus();                                            //清除I2C状态数据
		Status =I2C_I2CMasterSendRestart(address, I2C_I2C_WRITE_XFER_MODE,I2CTIMEOUT);      //发送读数据命令
	
		if(Status == I2C_I2C_MSTR_NOT_READY)
			Status =I2C_I2CMasterSendStart(address, I2C_I2C_WRITE_XFER_MODE,I2CTIMEOUT);

		if(Status == I2C_I2C_MSTR_NO_ERROR)
		{
			for(i=0;i<size;i++)
			{
				I2C_I2CMasterWriteByte(buff[i],I2CTIMEOUT);
			}
			if(sendStop)
				I2C_I2CMasterSendStop(I2CTIMEOUT); 
		}
	}
	else
	{
		I2C_1_I2CMasterClearStatus();                                            //清除I2C状态数据
		Status =I2C_1_I2CMasterSendRestart(address, I2C_1_I2C_WRITE_XFER_MODE,I2CTIMEOUT);      //发送读数据命令

		if(Status == I2C_I2C_MSTR_NOT_READY)
			Status =I2C_1_I2CMasterSendStart(address, I2C_1_I2C_WRITE_XFER_MODE,I2CTIMEOUT);

		if(Status == I2C_1_I2C_MSTR_NO_ERROR)
		{
			for(i=0;i<size;i++)
			{
				I2C_1_I2CMasterWriteByte(buff[i],I2CTIMEOUT);
			}
			if(sendStop)
				I2C_1_I2CMasterSendStop(I2CTIMEOUT);
	    }
	}
    if(Status==I2C_I2C_MSTR_NO_ERROR)
    {
    	last_error=I2C_ERROR_OK;
    }
    else
    {
    	last_error=I2C_ERROR_TIMEOUT;
    }
    return last_error;	  
}

i2c_err_t TwoWire::readTransmission(uint16_t address, uint8_t *buff, uint16_t size, bool sendStop, uint32_t *readCount)
{
	uint8_t Status=0;
	uint16_t i;
	flush();
	if(_i2c_num == I2C_NUM_0)
	{
		I2C_I2CMasterClearStatus();                                            //清除I2C状态数据
		Status =I2C_I2CMasterSendRestart(address, I2C_I2C_READ_XFER_MODE,I2CTIMEOUT);      //发送读数据命令

		if(Status == I2C_I2C_MSTR_NOT_READY)
			Status =I2C_I2CMasterSendStart(address, I2C_I2C_READ_XFER_MODE,I2CTIMEOUT);

		if(Status == I2C_I2C_MSTR_NO_ERROR)
		{
			for(i=0;i<size;i++)
			{
				I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA,&buff[i],I2CTIMEOUT);
				(* readCount)++;
			}
			if(sendStop)
				I2C_I2CMasterSendStop(I2CTIMEOUT); 
		}
	}
	else
	{
		I2C_1_I2CMasterClearStatus();                                            //清除I2C状态数据
		Status =I2C_1_I2CMasterSendRestart(address, I2C_1_I2C_READ_XFER_MODE,I2CTIMEOUT);      //发送读数据命令

		if(Status == I2C_I2C_MSTR_NOT_READY)
			Status =I2C_1_I2CMasterSendStart(address, I2C_1_I2C_READ_XFER_MODE,I2CTIMEOUT);

		if(Status == I2C_1_I2C_MSTR_NO_ERROR)
		{
			for(i=0;i<size;i++)
			{
				I2C_1_I2CMasterReadByte(I2C_1_I2C_ACK_DATA,&buff[i],I2CTIMEOUT);
				(* readCount)++;
			}
			if(sendStop)
				I2C_1_I2CMasterSendStop(I2CTIMEOUT); 
		}
	}
	
	if(Status==I2C_I2C_MSTR_NO_ERROR)
	{
		last_error=I2C_ERROR_OK;
	}
	else
	{
		last_error=I2C_ERROR_TIMEOUT;
	}
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
        if( last_error == I2C_ERROR_OK){
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
    return last_error;// Don't return Continue for compatibility.
    
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
	if(_i2c_num == I2C_NUM_0)
    {
	    I2C_I2CMasterClearReadBuf();
		I2C_I2CMasterClearWriteBuf();
		I2C_RX_FIFO_CTRL_REG=I2C_RX_FIFO_CTRL_REG|I2C_RX_FIFO_CTRL_CLEAR;
		I2C_RX_FIFO_CTRL_REG=I2C_RX_FIFO_CTRL_REG&(~I2C_RX_FIFO_CTRL_CLEAR);
		I2C_TX_FIFO_CTRL_REG=I2C_TX_FIFO_CTRL_REG|I2C_TX_FIFO_CTRL_CLEAR;
		I2C_TX_FIFO_CTRL_REG=I2C_TX_FIFO_CTRL_REG&(~I2C_TX_FIFO_CTRL_CLEAR);
	}
    else
    {
	    I2C_1_I2CMasterClearReadBuf();
		I2C_1_I2CMasterClearWriteBuf();
		I2C_1_RX_FIFO_CTRL_REG=I2C_1_RX_FIFO_CTRL_REG|I2C_1_RX_FIFO_CTRL_CLEAR;
		I2C_1_RX_FIFO_CTRL_REG=I2C_1_RX_FIFO_CTRL_REG&(~I2C_1_RX_FIFO_CTRL_CLEAR);
		I2C_1_TX_FIFO_CTRL_REG=I2C_1_TX_FIFO_CTRL_REG|I2C_1_TX_FIFO_CTRL_CLEAR;
		I2C_1_TX_FIFO_CTRL_REG=I2C_1_TX_FIFO_CTRL_REG&(~I2C_1_TX_FIFO_CTRL_CLEAR);
	}
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
//    return (uint8_t)last_error;
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
/*    uint8_t t = 0;
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
    }*/
}

/*stickbreaker Dump i2c Interrupt buffer, i2c isr Debugging
 */
 
uint32_t TwoWire::setDebugFlags( uint32_t setBits, uint32_t resetBits){
//  return i2cDebug(i2c,setBits,resetBits);
}

bool TwoWire::busy(void){
//  return ((i2cGetStatus(i2c) & 16 )==16);
}

TwoWire Wire = TwoWire(I2C_NUM_0);
TwoWire Wire1 = TwoWire(I2C_NUM_1);
