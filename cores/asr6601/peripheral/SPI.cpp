/* 
 SPI.cpp - SPI library for esp8266

 Copyright (c) 2015 Hristo Gochkov. All rights reserved.
 This file is part of the esp8266 core for Arduino environment.
 
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

#include "SPI.h"


SPIClass::SPIClass(uint8_t spi_bus)
    :_spi_num(spi_bus)
    ,_spi(NULL)
    ,_use_hw_ss(false)
    ,_sck(-1)
    ,_miso(-1)
    ,_mosi(-1)
    ,_ss(-1)
    ,_freq(1000000)
    ,_inTransaction(false)
{}

bool SPIClass::begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
{
	if(_spi) {
		return false;
	}

	if(_spi_num == 0 &&sck < 0 && miso < 0 && mosi < 0) {
		sck = SCK;
		mosi = MOSI;
		miso = MISO;
	}
	else if(_spi_num == 1 &&sck < 0 && miso < 0 && mosi < 0) {
		sck = SCK1;
		mosi = MOSI1;
		miso = MISO1;
	}
	else if(_spi_num == 2 &&sck < 0 && miso < 0 && mosi < 0) {
		sck = SCK2;
		mosi = MOSI2;
		miso = MISO2;
	}

	_sck = sck;
	_mosi = mosi;
	_miso = miso;
	_ss = ss;
	_spi = spiStartBus(_spi_num, _freq, SPI_MODE0, SPI_MSBFIRST,_sck,_mosi,_miso,_ss);
	if(!_spi) {
		return false;
	}
	setFrequency(_freq);
	return true;
}

void SPIClass::end()
{
	if(!_spi)
	{	
		return;
	}
	spiStopBus(_spi,_sck,_mosi,_miso,_ss);
	_spi = NULL;
}

void SPIClass::setHwCs(bool use)
{
	if(use && !_use_hw_ss) {
		if(spiSSEnable(_spi, _ss))
			_use_hw_ss = use;
	} else if(_use_hw_ss) {
		spiSSDisable(_spi, _ss);
		_use_hw_ss = use;
	}
}

void SPIClass::setFrequency(uint32_t freq)
{
	if(_freq != freq ) {
		_freq = freq;
		spiSetFrequency(_spi,_freq);
	}
}

uint32_t SPIClass::getFrequency()
{
	return spiGetFrequency(_spi);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
	if(_dataMode != dataMode)
	{
		_dataMode = dataMode;
		spiSetDataMode(_spi, dataMode);
	}
}

uint8_t SPIClass::getDataMode()
{
	return spiGetDataMode(_spi);
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
	if(_bitOrder != bitOrder)
	{
		_bitOrder != bitOrder;
		spiSetBitOrder(_spi, bitOrder);
	}
}

uint8_t SPIClass::getBitOrder()
{
	return spiGetBitOrder(_spi);
}

void SPIClass::beginTransaction(SPISettings settings)
{
	if(_freq!=settings._clock || _bitOrder!=settings._bitOrder || _dataMode!=settings._dataMode)
	{
		_freq = settings._clock;
		_bitOrder = settings._bitOrder;
		_dataMode = settings._dataMode;
		spiTransaction(_spi, _freq, _dataMode, _bitOrder);
	}
	_inTransaction = true;
}

void SPIClass::endTransaction()
{
	if(_inTransaction){
		_inTransaction = false;
	}
}

void SPIClass::write(uint8_t data)
{
	spiWriteByte(_spi, data);
}

uint8_t SPIClass::transfer(uint8_t data)
{
	return spiTransferByte(_spi, data);
}

void SPIClass::write16(uint16_t data)
{
	spiWriteWord(_spi, data);
}

uint16_t SPIClass::transfer16(uint16_t data)
{
	return spiTransferWord(_spi, data);
}

void SPIClass::write32(uint32_t data)
{
	spiWriteLong(_spi, data);
}

uint32_t SPIClass::transfer32(uint32_t data)
{
	return spiTransferLong(_spi, data);
}

void SPIClass::transfer(uint8_t * data, uint32_t size) 
{
	transferBytes(data, data, size); 
}

/**
 * @param data uint8_t * data buffer. can be NULL for Read Only operation
 * @param out  uint8_t * output buffer. can be NULL for Write Only operation
 * @param size uint32_t
 */
void SPIClass::transferBytes( uint8_t * data, uint8_t * out, uint32_t size)
{
	spiTransferBytes(_spi, data, out, size);
}

SPIClass SPI(0);
SPIClass SPI1(1);
SPIClass SPI2(2);

