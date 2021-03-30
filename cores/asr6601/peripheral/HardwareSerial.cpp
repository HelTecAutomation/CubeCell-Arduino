#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "lora_config.h"
#include "HardwareSerial.h"

HardwareSerial Serial(0);
HardwareSerial Serial1(1);
HardwareSerial Serial2(2);
HardwareSerial Serial3(3);

HardwareSerial::HardwareSerial(int uart) : _uart(uart) {}

bool HardwareSerial::begin(uint32_t baud, uint32_t config, int rxPin, int txPin, bool invert, unsigned long timeout_ms)
{
	if(0 > _uart || _uart > 3) {
		return false;
	}
	_baud = baud;
	_config = config;

	if(_uart == 0 && (rxPin < 0 || txPin < 0)) {
		rxPin = UART0_RX;
		txPin = UART0_TX;
	}
	else if(_uart == 1 && (rxPin < 0 || txPin < 0)) {
		rxPin = UART1_RX;
		txPin = UART1_TX;
	}
	else if(_uart == 2 && (rxPin < 0 || txPin < 0)) {
		rxPin = UART2_RX;
		txPin = UART2_TX;
	}
	else if(_uart == 3 && (rxPin < 0 || txPin < 0)) {
		rxPin = UART3_RX;
		txPin = UART3_TX;
	}
	_rxPin = rxPin;
	_txPin = txPin;
	
	_uart = uartStart(baud,config,_rxPin,_txPin);
	if(_uart<0)
		return false;
	else
		return true;
}

void HardwareSerial::updateBaudRate(unsigned long baud)
{
	uartStart(baud,_config,_rxPin,_txPin);
}

void HardwareSerial::end()
{
	uartEnd(_uart,_rxPin,_txPin);
}

int HardwareSerial::available(void)
{
	return uartAvailable(_uart);
}
int HardwareSerial::availableForWrite(void)
{
	return uartAvailableForWrite(_uart);
}

bool HardwareSerial::busy(void)
{
	return uartBusy(_uart);
}

int HardwareSerial::peek(void)
{
	if (available()) {
		return uartPeek(_uart);
	}
	return -1;
}

int HardwareSerial::read(void)
{
	if(available()) {
		return uartRead(_uart);
	}
	return -1;
}

void HardwareSerial::flush(void)
{
	uartFlush(_uart);
}

void HardwareSerial::flush(bool txOnly)
{
	uartFlushTxOnly(_uart,txOnly);
}

size_t HardwareSerial::write(uint8_t c)
{
	return uartWrite(_uart,&c,1);
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
#ifdef PRINT_BY_DMA
	return uartWrite(_uart,buffer,size);
#else
	uint32_t bufIndex;
	bufIndex = 0u;
	while(bufIndex < size)
	{
		uart_t* uartx = serialArray[_uart];
		uart_send_data(uartx, buffer[bufIndex]);
		bufIndex++;
	}
	return size;
#endif
}

uint32_t  HardwareSerial::baudRate()
{
	//return _baud;
	return uartGetBaudRate(_uart);
}
HardwareSerial::operator bool() const
{
	return true;
}

int HardwareSerial::read(uint8_t* buff, uint32_t timeout)
{
    uint32_t timestart = millis();
    while(available()==0)
    {
      if( (millis()-timestart) > timeout )
      {
        return 0;
      }
    }
    int serialBuffer_index=0;
    while(available())
    {
      buff[serialBuffer_index++]=read();

      int i = 0;
      while(i<1000)
      {
          if(available())
              break;
          delayMicroseconds(1);
          i++;
      }
    }
    return serialBuffer_index;
      //Serial.write(serialBuffer,serialBuffer_index);
}


