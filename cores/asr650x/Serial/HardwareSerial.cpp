#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "Arduino.h"
#include "HardwareSerial.h"
#include "project.h"


HardwareSerial Serial(UART_NUM_0);
HardwareSerial Serial1(UART_NUM_1);

HardwareSerial::HardwareSerial(int8_t uart_num) 
	:_uart_num(uart_num) 
	{}

void HardwareSerial::begin(unsigned long baud , int8_t uart_num, uint32_t config, bool invert, unsigned long timeout_ms)
{
	if(uart_num!=-1)
	{
		if(uart_num == 1)
		{
			_uart_num = uart_num;
		}
		else
		{
			_uart_num = 0;
		}
	}

	SerialBaud=baud;
	
	if( _uart_num == UART_NUM_0) 
	{
		if(digitalRead(UART_RX)==UART_RX_LEVEL)//uart start when uart chip powered
		{
			uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ/SerialBaud/UART_1_UART_OVS_FACTOR + 0.5 - 1;
			UART_1_SCBCLK_DIV_REG = div<<8;
			UART_1_Start();
		}
	}
	else
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ/SerialBaud/UART_2_UART_OVS_FACTOR + 0.5 - 1;
		UART_2_SCBCLK_DIV_REG = div<<8;
		UART_2_Start();
	}
}

void HardwareSerial::updateBaudRate(unsigned long baud)
{
	SerialBaud = baud;
	if( _uart_num == UART_NUM_0)
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ/SerialBaud/UART_1_UART_OVS_FACTOR + 0.5 - 1;
		UART_1_SCBCLK_DIV_REG = div<<8;
	}
	else
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ/SerialBaud/UART_2_UART_OVS_FACTOR + 0.5 - 1;
		UART_2_SCBCLK_DIV_REG = div<<8;
	}
}

void HardwareSerial::end()
{
	if( _uart_num == UART_NUM_0)
	{
		UART_1_Stop();
	}
	else
	{
		UART_2_Stop();
	}
}

size_t HardwareSerial::setRxBufferSize(size_t new_size) {
  //  return uartResizeRxBuffer(_uart, new_size);
  return 0;
}

void HardwareSerial::setDebugOutput(bool en)
{
  /*  if(_uart == 0) {
        return;
    }
    if(en) {
        uartSetDebug(_uart);
    } else {
        if(uartGetDebug() == _uart_nr) {
            uartSetDebug(0);
        }
    }*/
}

int HardwareSerial::available(void)
{
	uint8_t buffsize;
	//for(uint32_t i=0;i<(23040000/SerialBaud);i++)
	//{
	if( _uart_num == UART_NUM_0)
	{
		buffsize=UART_1_SpiUartGetRxBufferSize();
	}
	else
	{
		buffsize=UART_2_SpiUartGetRxBufferSize();
	}
	if(buffsize){
		return buffsize;
	}
	//}
	return 0;
}
void HardwareSerial::delayByte(void)
{
	delayMicroseconds(10000000/SerialBaud);
}


int HardwareSerial::availableForWrite(void)
{
   // return uartAvailableForWrite(_uart);
   return 0;
}

int HardwareSerial::peek(void)
{
  //  if (available()) {
  //      return uartPeek(_uart);
  //  }
  //  return -1;
  return 0;
}

int HardwareSerial::read(void)
{
	if(available()) {
		if( _uart_num == UART_NUM_0)
		{
			return UART_1_UartGetByte();
		}
		else
		{
			return UART_2_UartGetByte();
		}
	}
	return (uint32)(-1);
}

void HardwareSerial::flush()
{
	if( _uart_num == UART_NUM_0)
	{
		UART_1_SpiUartClearRxBuffer();
		UART_1_SpiUartClearTxBuffer();
	}
	else
	{
		UART_2_SpiUartClearRxBuffer();
		UART_2_SpiUartClearTxBuffer();	
	}
}

size_t HardwareSerial::write(uint8_t c)
{
	if( _uart_num == UART_NUM_0)
	{
		UART_1_UartPutChar(c);
	}
	else
	{
		UART_2_UartPutChar(c);
	}
	return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
	uint32 bufIndex;
	bufIndex = 0u;
	   
	while(bufIndex < size)
	{
		if( _uart_num == UART_NUM_0)
		{
			UART_1_UartPutChar( buffer[bufIndex] );
		}
		else
		{
			UART_2_UartPutChar( buffer[bufIndex] );
		}
		bufIndex++;
	}
	return size;
}
uint32_t  HardwareSerial::baudRate()
{
	//return uartGetBaudRate(_uart);
	return 0;
}
HardwareSerial::operator bool() const
{
    //return true;
    return 0;
}


