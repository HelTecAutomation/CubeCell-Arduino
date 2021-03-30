
#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include "Stream.h"
#include "uart.h"

class HardwareSerial: public Print
{
public:
    HardwareSerial(int uart_nr);

    bool begin(uint32_t baud=115200, uint32_t config=SERIAL_8N1, int rxPin=-1, int txPin=-1, bool invert=false, unsigned long timeout_ms = 20000UL);
    void end();
    void updateBaudRate(unsigned long baud);
    int available(void);
    int availableForWrite(void);
    int peek(void);
    int read(void);
    int read(uint8_t* buff, uint32_t timeout);
    void flush(void);
    void flush( bool txOnly);
    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);
    bool busy(void);

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
    uint32_t baudRate();
    operator bool() const;
    size_t setRxBufferSize(size_t);
    void setDebugOutput(bool);
protected:
    int _uart;
    uint32_t _baud;
    int _rxPin=-1;
    int _txPin=-1;
    uint32_t _config;
};

extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;

#endif // HardwareSerial_h
