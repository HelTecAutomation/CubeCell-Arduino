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


#define spi_TIMEOUT	500


SPIClass::SPIClass(uint8_t ss):_ss(ss),_inTransaction(false){}


void SPIClass::begin(uint8_t ss)
{
    _ss = ss;
    pinMode(_ss,OUTPUT);
    SPI_1_Start();
}

void SPIClass::end()
{
	SPI_1_Stop();
}


void SPIClass::beginTransaction(void)
{
    _inTransaction = true;
    digitalWrite(_ss,LOW);
}

void SPIClass::endTransaction()
{
    if(_inTransaction){
        _inTransaction = false;
//        spiEndTransaction(_spi);
    }
    digitalWrite(_ss,HIGH);
}



uint32_t SPIClass::transfer(uint32_t data)
{
		uint32 rxdata;
		uint32 timeout = 0;
		
		while(SPI_1_SpiUartGetTxBufferSize() != 0);
		SPI_1_SpiUartWriteTxData(data);
		
		while(SPI_1_SpiUartGetRxBufferSize() == 0)
		{
			timeout++;
			if(timeout > spi_TIMEOUT )break;
		}
		if(timeout > spi_TIMEOUT ){}
//			DBG_PRINTF("Receive Data timeout!.\r\n");
		else
		{
			rxdata = SPI_1_SpiUartReadRxData();
			return rxdata;
		}
}

void SPIClass::transfer(uint8_t * data, uint32_t size) 
{ 
	//transferBytes(data, data, size); 
}


/* @param data uint8_t * data buffer. can be NULL for Read Only operation
 * @param out  uint8_t * output buffer. can be NULL for Write Only operation
 * @param size uint32_t
 */
void SPIClass::transferBytes(uint8_t * data, uint8_t * out, uint32_t size)
{

}


SPIClass SPI(-1);
