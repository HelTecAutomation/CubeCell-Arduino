#ifndef __IO_USART_H__
#define __IO_USART_H__

#include "Arduino.h"
#include <stdlib.h>

class softSerial
{
private:
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
    static void receiver(void);
    int read(void);
    void flush();
};

// io_usart::io_usart(/* args */)
// {
// }

// io_usart::~io_usart()
// {
// }

#endif