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

void HardwareSerial::begin(unsigned long baud , uint32_t config, int8_t uart_num, bool invert, unsigned long timeout_ms)
{
	uint32_t stop_bits,parity_ctrl,data_bits,parity;

	stop_bits = config & 0x0F;
	parity = (config>>4) & 0x0F;
	data_bits = (config>>8) & 0x0F;

	parity_ctrl = ((UART_1_UART_PARITY_NONE != parity) ?	 \
					(UART_1_GET_UART_RX_CTRL_PARITY(parity) | \
					UART_1_UART_RX_CTRL_PARITY_ENABLED) : (0u));

	uint32_t rx_ctrl0 = (UART_1_GET_UART_RX_CTRL_MODE(stop_bits)			| \
					 UART_1_GET_UART_RX_CTRL_POLARITY(UART_1_UART_IRDA_POLARITY)				| \
					 UART_1_GET_UART_RX_CTRL_MP_MODE(UART_1_UART_MP_MODE_ENABLE)				| \
					 UART_1_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(UART_1_UART_DROP_ON_PARITY_ERR) | \
					 UART_1_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(UART_1_UART_DROP_ON_FRAME_ERR)	| \
					 UART_1_GET_UART_RX_CTRL_BREAK_WIDTH(UART_1_UART_RX_BREAK_WIDTH)			| \
					 parity_ctrl);

	uint32_t rx_ctrl1 =(UART_1_GET_RX_CTRL_DATA_WIDTH(data_bits)		 | \
						UART_1_GET_RX_CTRL_MEDIAN	  (UART_1_UART_MEDIAN_FILTER_ENABLE) | \
						UART_1_GET_UART_RX_CTRL_ENABLED(UART_1_UART_DIRECTION));

	uint32_t tx_ctrl0 = (UART_1_GET_UART_TX_CTRL_MODE(stop_bits)	   | \
							 UART_1_GET_UART_TX_CTRL_RETRY_NACK(UART_1_UART_RETRY_ON_NACK) | \
							 parity_ctrl);

	uint32_t tx_ctrl1 = (UART_1_GET_TX_CTRL_DATA_WIDTH(data_bits) | \
							 UART_1_GET_UART_TX_CTRL_ENABLED(UART_1_UART_DIRECTION));
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
			UART_1_Start();
			UART_1_SCBCLK_DIV_REG = div<<8;
			UART_1_UART_RX_CTRL_REG = rx_ctrl0;
			UART_1_RX_CTRL_REG =rx_ctrl1;
			UART_1_UART_TX_CTRL_REG =tx_ctrl0;
			UART_1_TX_CTRL_REG = tx_ctrl1;
		}
	}
	else
	{
		uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ/SerialBaud/UART_2_UART_OVS_FACTOR + 0.5 - 1;
		UART_2_Start();
		UART_2_SCBCLK_DIV_REG = div<<8;
		UART_2_UART_RX_CTRL_REG = rx_ctrl0;
		UART_2_RX_CTRL_REG =rx_ctrl1;
		UART_2_UART_TX_CTRL_REG =tx_ctrl0;
		UART_2_TX_CTRL_REG = tx_ctrl1;
	}
/*
	printf("config %d\r\n",config);
	printf("stop_bits %d\r\n",stop_bits);
	printf("parity %d\r\n",parity);
	printf("data_bits %d\r\n",data_bits);
*/
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
	delayMicroseconds(11000000/SerialBaud);
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
		while (UART_1_SpiUartGetTxBufferSize());
		UART_1_SpiUartClearTxBuffer();
	}
	else
	{
		UART_2_SpiUartClearRxBuffer();
		while (UART_2_SpiUartGetTxBufferSize());
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
	return SerialBaud;
}
HardwareSerial::operator bool() const
{
    return true;
    //return 0;
}


