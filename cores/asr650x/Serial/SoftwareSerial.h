#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include "Arduino.h"
#include "Stream.h"
#include "RingBuffer.h"

#include <stdlib.h>

class SoftwareSerial : public Stream
{
public:
    SoftwareSerial(uint8_t rx_GPIO, uint8_t tx_GPIO);
    ~SoftwareSerial() {
        stop();
    }

    void begin(uint16_t Baudrate);

    // Print API
    size_t write(uint8_t value);
    size_t write(const uint8_t *buffer, size_t size);

    // Stream API
    int available();
    int read();
    int peek();
    void flush();

    // SoftwareSerial API
    void listen();
    bool isListening();
    bool overflow();

    // Heltech SoftSerial API
    inline void sendByte(uint8_t value){
        write(value);
    }
    inline void sendStr(uint8_t *st, uint16_t len){
        write((const uint8_t*)st, len);
    }
    void softwarePrintf(char *p_fmt, ...);

private:
    void stop();

    static void receiveByte(void);
    static void receiver(void);

    static uint8_t s_currentRx;
    static RingBuffer<uint8_t, 64> s_rxDataBuffer;
    static uint16_t s_timeDelay;
    static bool s_overflow;

    uint8_t _rxpin;
    uint8_t _txpin;
};
#endif
