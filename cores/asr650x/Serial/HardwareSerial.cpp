#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>


#include "HardwareSerial.h"
#include "project.h"


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
HardwareSerial Serial(0);
#endif

HardwareSerial::HardwareSerial(int uart_nr) : _uart_nr(uart_nr) {}

void HardwareSerial::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms)
{
  uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ/baud/UART_1_UART_OVS_FACTOR + 0.5 - 1;
  UART_1_SCBCLK_DIV_REG = div<<8;
  UART_1_SCBCLK_CMD_REG = 0x8000FF41u;
  if(digitalRead(UART_RX)==UART_RX_LEVEL)//uart start when uart chip powered
  {
	  UART_1_Start();
  }
  SerialBaud=baud;
}

void HardwareSerial::updateBaudRate(unsigned long baud)
{
	uint32_t div = (float)CYDEV_BCLK__HFCLK__HZ/baud/UART_1_UART_OVS_FACTOR + 0.5 - 1;
	UART_1_SCBCLK_DIV_REG = div<<8;
	UART_1_SCBCLK_CMD_REG = 0x8000FF41u;
	SerialBaud=baud;
}

void HardwareSerial::end()
{
  UART_1_Stop();
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
	for(uint32_t i=0;i<(23040000/SerialBaud);i++)
	{
		buffsize=UART_1_SpiUartGetRxBufferSize();
		if(buffsize){
			return buffsize;
		}
	}
	return 0;
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

uint32 HardwareSerial::read(void)
{
    if(available()) {
        return UART_1_UartGetByte();
    }
    return (uint32)(-1);
}

void HardwareSerial::flush()
{
    UART_1_SpiUartClearRxBuffer();
    UART_1_SpiUartClearTxBuffer();
}

size_t HardwareSerial::write(uint8_t c)
{
    UART_1_UartPutChar(c);
    return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
   uint32 bufIndex;
   bufIndex = 0u;
   
   while(bufIndex < size)
   {
      UART_1_UartPutChar( buffer[bufIndex] );
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
