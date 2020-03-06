#ifndef SSD1306Spi_h
#define SSD1306Spi_h

#include <cubecell_OLEDDisplay.h>
#include <SPI.h>

#if F_CPU == 160000000L
  #define BRZO_I2C_SPEED 1000
#else
  #define BRZO_I2C_SPEED 800
#endif

class SSD1306Spi : public OLEDDisplay {
  private:
      uint8_t             _rst;
      uint8_t             _dc;
      uint8_t             _cs;
      uint32_t            _freq;
      uint8_t             _spi_num;

  public:
    SSD1306Spi(uint8_t _rst, uint8_t _dc, uint8_t _cs,  uint8_t _spi_num ,uint32_t _freq = 6000000, OLEDDISPLAY_GEOMETRY g = GEOMETRY_128_64) {
        setGeometry(g);

      this->_rst = _rst;
      this->_dc  = _dc;
      this->_cs  = _cs;
      this->_freq  = _freq;
      this->_spi_num  = _spi_num;
      }

    bool connect(){
      pinMode(_dc, OUTPUT);
      pinMode(_cs, OUTPUT);
      pinMode(_rst, OUTPUT);

      SPI.begin (this->_cs,this->_freq,this->_spi_num);

      // Pulse Reset low for 10ms
      digitalWrite(_rst, HIGH);
      delay(1);
      digitalWrite(_rst, LOW);
      delay(10);
      digitalWrite(_rst, HIGH);
      return true;
    }

    void display(void) {
    #ifdef OLEDDISPLAY_DOUBLE_BUFFER
       uint8_t minBoundY = UINT8_MAX;
       uint8_t maxBoundY = 0;

       uint8_t minBoundX = UINT8_MAX;
       uint8_t maxBoundX = 0;

       uint8_t x, y;

       // Calculate the Y bounding box of changes
       // and copy buffer[pos] to buffer_back[pos];
       for (y = 0; y < (displayHeight / 8); y++) {
         for (x = 0; x < displayWidth; x++) {
          uint16_t pos = x + y * displayWidth;
          if (buffer[pos] != buffer_back[pos]) {
            minBoundY = _min(minBoundY, y);
            maxBoundY = _max(maxBoundY, y);
            minBoundX = _min(minBoundX, x);
            maxBoundX = _max(maxBoundX, x);
          }
          buffer_back[pos] = buffer[pos];
        }
        //yield();
       }

       // If the minBoundY wasn't updated
       // we can savely assume that buffer_back[pos] == buffer[pos]
       // holdes true for all values of pos
       if (minBoundY == UINT8_MAX) return;

       sendCommand(COLUMNADDR);
       sendCommand(minBoundX);
       sendCommand(maxBoundX);

       sendCommand(PAGEADDR);
       sendCommand(minBoundY);
       sendCommand(maxBoundY);

       digitalWrite(_dc, HIGH);   // data mode
       SPI.beginTransaction();
       for (y = minBoundY; y <= maxBoundY; y++) {
         for (x = minBoundX; x <= maxBoundX; x++) {
           SPI.transfer(buffer[x + y * displayWidth]);
         }
         //yield();
       }
       SPI.endTransaction();
     #else
       // No double buffering
       sendCommand(COLUMNADDR);
       sendCommand(0x0);
       sendCommand(0x7F);

       sendCommand(PAGEADDR);
       sendCommand(0x0);

       if (geometry == GEOMETRY_128_64) {
         sendCommand(0x7);
       } else if (geometry == GEOMETRY_128_32) {
         sendCommand(0x3);
       }

        digitalWrite(_dc, HIGH);   // data mode
        SPI.beginTransaction();
        for (uint16_t i=0; i<displayBufferSize; i++) {
          SPI.transfer(buffer[i]);
          //yield();
        }
        SPI.endTransaction();
     #endif
    }

  private:
	int getBufferOffset(void) {
		return 0;
	}
    inline void sendCommand(uint8_t com) __attribute__((always_inline)){
      digitalWrite(_dc, LOW);
      SPI.beginTransaction();
      SPI.transfer(com);
      SPI.endTransaction();
    }
};

#endif
