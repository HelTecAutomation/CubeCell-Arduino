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
#include "project.h"
#include "HardwareSerial.h"

#define spi_TIMEOUT	500


SPIClass::SPIClass(int8_t ss, int8_t spiNum)
	:_freq(6000000),
	_spi_num(spiNum),
	_inTransaction(false),
	_bitOrder(SPI_MSBFIRST),
	_dataMode(SPI_MODE0)
	{}


void SPIClass::begin(int8_t ss, uint32_t freq, int8_t spiNum)
{
	if(ss!=-1)
	{
		_ss = ss;
	}
	
	if(spiNum != -1)
	{
		if(spiNum == 1)
		{
			_spi_num = spiNum;
		}
		else
		{
			_spi_num = 0;
		}
	}
	
	_freq = freq;
	
	if(_freq > 6000000)
	{
		_freq = 6000000;
	}

	pinMode(_ss,OUTPUT);
	
	if(_spi_num == 0)
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ / SPI_1_SPI_OVS_FACTOR / _freq - 1;
		SPI_1_SCBCLK_DIV_REG = div << 8 ;
		SPI_1_Start();
	}
	else
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ / SPI_2_SPI_OVS_FACTOR / _freq - 1;
		SPI_2_SCBCLK_DIV_REG = div << 8 ;
		SPI_2_Start();
	}
}

void SPIClass::end()
{
	if(_spi_num == 0)
	{
		SPI_1_Stop();
	}
	else
	{
		SPI_2_Stop();
	}
}

void SPIClass::setFrequency(uint32_t freq)
{
	_freq = freq;
	
	if(_freq > 6000000)
	{
		_freq = 6000000;
	}
	
	if(_spi_num == 0)
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


void SPIClass:: beginTransaction(SPISettings settings)
{
	if(_spi_num == 0)
	{
		if(_freq!=settings._clock)
		{
			setFrequency(settings._clock);
		}
		if(_bitOrder!=settings._bitOrder)
		{
			_bitOrder = settings._bitOrder;
			uint32_t temp = SPI_1_RX_CTRL_REG;
			SPI_1_RX_CTRL_REG = temp & ~(0x01<<8) | (_bitOrder<<8);
			temp = SPI_1_TX_CTRL_REG;
			SPI_1_TX_CTRL_REG = temp & ~(0x01<<8) | (_bitOrder<<8);
		}
		if(_dataMode!=settings._dataMode)
		{
			_dataMode = settings._dataMode;
			uint32_t temp = SPI_1_SPI_CTRL_REG;
			SPI_1_SPI_CTRL_REG = temp & ~(0x03<<2) | (_dataMode << 2);
		}
	}
	if(_spi_num == 1)
	{
		if(_freq!=settings._clock)
		{
			setFrequency(settings._clock);
		}
		if(_bitOrder!=settings._bitOrder)
		{
			_bitOrder = settings._bitOrder;
			uint32_t temp = SPI_2_RX_CTRL_REG;
			SPI_2_RX_CTRL_REG = temp & ~(0x01<<8) | (_bitOrder<<8);
			temp = SPI_2_TX_CTRL_REG;
			SPI_2_TX_CTRL_REG = temp & ~(0x01<<8) | (_bitOrder<<8);
		}
		if(_dataMode!=settings._dataMode)
		{
			_dataMode = settings._dataMode;
			uint32_t temp = SPI_2_SPI_CTRL_REG;
			SPI_2_SPI_CTRL_REG = temp & ~(0x03<<2) | (_dataMode << 2);
		}
	}
	_inTransaction = true;
	digitalWrite(_ss,LOW);
}

void SPIClass::endTransaction()
{
    if(_inTransaction){
        _inTransaction = false;
    }
    digitalWrite(_ss,HIGH);
}



uint8_t SPIClass::transfer(uint8_t data)
{
	uint32 rxdata;
	uint32 timeout = 0;

	if(_spi_num == 0)
	{
		while(SPI_1_SpiUartGetTxBufferSize() != 0);
		SPI_1_SpiUartWriteTxData(data);
		
		while(SPI_1_SpiUartGetRxBufferSize() == 0)
		{
			timeout++;
			if(timeout > spi_TIMEOUT )break;
		}
		if(timeout > spi_TIMEOUT ){}
		else
		{
			rxdata = SPI_1_SpiUartReadRxData();
			return (uint8_t)rxdata;
		}
	}
	else
	{
		while(SPI_2_SpiUartGetTxBufferSize() != 0);
		SPI_2_SpiUartWriteTxData(data);
		
		while(SPI_2_SpiUartGetRxBufferSize() == 0)
		{
			timeout++;
			if(timeout > spi_TIMEOUT )break;
		}
		if(timeout > spi_TIMEOUT ){}
		else
		{
			rxdata = SPI_2_SpiUartReadRxData();
			return (uint8_t)rxdata;
		}
	}
}

void SPIClass::transfer(uint8_t * data, uint32_t size) 
{
	for(uint32_t i;i<size;i++)
	{
		if(_spi_num == 0)
		{
			while(SPI_1_SpiUartGetTxBufferSize() != 0);
			SPI_1_SpiUartWriteTxData(*data++);
		}
		else
		{
			while(SPI_2_SpiUartGetTxBufferSize() != 0);
			SPI_2_SpiUartWriteTxData(*data++);
		}
	}
}


/* @param data uint8_t * data buffer. can be NULL for Read Only operation
 * @param out  uint8_t * output buffer. can be NULL for Write Only operation
 * @param size uint32_t
 */
void SPIClass::transferBytes(uint8_t * data, uint8_t * out, uint32_t size)
{
	uint32 timeout = 0;
	for(uint32_t i;i<size;i++)
	{
		if(_spi_num == 0)
		{
			while(SPI_1_SpiUartGetTxBufferSize() != 0);
			SPI_1_SpiUartWriteTxData(*data++);
			
			while(SPI_1_SpiUartGetRxBufferSize() == 0)
			{
				timeout++;
				if(timeout > spi_TIMEOUT )break;
			}
			
			if(timeout > spi_TIMEOUT ){}
			else
			{
				* out++ = (uint8_t)SPI_1_SpiUartReadRxData();
			}
		}
		else
		{
			while(SPI_2_SpiUartGetTxBufferSize() != 0);
			SPI_2_SpiUartWriteTxData(*data++);

			while(SPI_2_SpiUartGetRxBufferSize() == 0)
			{
				timeout++;
				if(timeout > spi_TIMEOUT )break;
			}
			
			if(timeout > spi_TIMEOUT ){}
			else
			{
				* out++ = (uint8_t)SPI_2_SpiUartReadRxData();
			}
		}
	}
}

SPIClass SPI(-1,SPI_NUM_0);
SPIClass SPI1(-1,SPI_NUM_1);

