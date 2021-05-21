#ifndef __IO_USART_H__
#define __IO_USART_H__

#include "Arduino.h"
#include <stdlib.h>

class softSerial:public Stream
{
protected:
uint8_t pbuffer;

public:
    softSerial(uint8_t tx_GPIO, uint8_t rx_GPIO);
    ~softSerial() {}

    void begin(uint16_t Baudrate);
    
    void sendByte(uint8_t value);
    void sendStr(uint8_t *st, uint16_t len);
    void softwarePrintf(char *p_fmt, ...);

    int available(void);
    static void receiverBegin(void);
    int read(void);
    void flush();
    int peek(void);
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);

    inline size_t write(const char * s)
    {
        return write((uint8_t*) s, strlen(s));
    }
    inline size_t write(unsigned long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t) n);
    }
};

// io_usart::io_usart(/* args */)
// {
// }

// io_usart::~io_usart()
// {
// }

#endif