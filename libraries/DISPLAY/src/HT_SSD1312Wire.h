#ifndef SSD1312Wire_h
#define SSD1312Wire_h

#include <HT_Display.h>
#include <Wire.h>

class SSD1312Wire : public ScreenDisplay {
  private:
      uint8_t             _address;
      int                 _sda;
      int                 _scl;
      uint32_t             _freq;
      bool                _doI2cAutoInit = false;

  public:
    SSD1312Wire(uint8_t _address, uint32_t _freq, int sda,int scl, DISPLAY_GEOMETRY g = GEOMETRY_128_64,int8_t _rst=-1) {
      setGeometry(g);
      setRst(_rst);
      this->_address = _address;
      this->_freq = _freq;
      this->_sda = sda;
      this->_scl = scl;
      this->displayType = OLED;
    }

    bool connect() {
      Wire.begin(_sda,_scl,_freq);
      return true;
    }

    void display(void) {
        initI2cIfNeccesary();
        if(rotate_angle==ANGLE_0_DEGREE||rotate_angle==ANGLE_180_DEGREE)
        {
        #ifdef DISPLAY_DOUBLE_BUFFER
            uint8_t minBoundY = UINT8_MAX;
            uint8_t maxBoundY = 0;

            uint8_t minBoundX = UINT8_MAX;
            uint8_t maxBoundX = 0;
            uint8_t x, y;

            // Calculate the Y bounding box of changes
            // and copy buffer[pos] to buffer_back[pos];
            for (y = 0; y < (this->height() / 8); y++)
            {
                for (x = 0; x < this->width(); x++)
                {
                    uint16_t pos = x + y * this->width();
                    if (buffer[pos] != buffer_back[pos])
                    {
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
            sendCommand( minBoundX);
            sendCommand( maxBoundX);

            sendCommand(PAGEADDR);
            sendCommand(minBoundY);
            sendCommand(maxBoundY);

            byte k = 0;
            for (y = minBoundY; y <= maxBoundY; y++)
            {
                for (x = minBoundX; x <= maxBoundX; x++)
                {
                    if (k == 0)
                    {
                        Wire.beginTransmission(_address);
                        Wire.write(0x40);
                    }

                    Wire.write(buffer[x + y * this->width()]);
                    k++;
                    if (k == 16)
                    {
                        Wire.endTransmission();
                        k = 0;
                    }
                }
                //yield();
            }

            if (k != 0) {
                Wire.endTransmission();
            }
        #else

            sendCommand(COLUMNADDR);
            sendCommand(0);
            sendCommand((this->width() - 1));

            sendCommand(PAGEADDR);
            sendCommand(0x0);

            if (geometry == GEOMETRY_128_64)
            {
                sendCommand(0x7);
            }
            else if (geometry == GEOMETRY_128_32)
            {
                sendCommand(0x3);
            }

            for (uint16_t i=0; i < displayBufferSize; i++)
            {
                Wire.beginTransmission(this->_address);
                Wire.write(0x40);
                for (uint8_t x = 0; x < 16; x++)
                {
                    Wire.write(buffer[i]);
                    i++;
                }
                i--;
                Wire.endTransmission();
            }
        #endif
        }
        else
        {
            uint8_t buffer_rotate[displayBufferSize];
            memset(buffer_rotate,0,displayBufferSize);
            uint8_t temp;
            for(uint16_t i=0;i<this->width();i++)
            {
                for(uint16_t j=0;j<this->height();j++)
                {
                    temp = buffer[(j>>3)*this->width()+i]>>(j&7)&0x01;
                    buffer_rotate[(i>>3)*this->height()+j]|=(temp<<(i&7));
                }
            }
        #ifdef DISPLAY_DOUBLE_BUFFER
            uint8_t minBoundY = UINT8_MAX;
            uint8_t maxBoundY = 0;

            uint8_t minBoundX = UINT8_MAX;
            uint8_t maxBoundX = 0;
            uint8_t x, y;

            // Calculate the Y bounding box of changes
            // and copy buffer[pos] to buffer_back[pos];
            for (y = 0; y < (this->width() / 8); y++)
            {
                for (x = 0; x < this->height(); x++)
                {
                    uint16_t pos = x + y * this->height();
                    if (buffer_rotate[pos] != buffer_back[pos])
                    {
                        minBoundY = _min(minBoundY, y);
                        maxBoundY = _max(maxBoundY, y);
                        minBoundX = _min(minBoundX, x);
                        maxBoundX = _max(maxBoundX, x);
                    }
                    buffer_back[pos] = buffer_rotate[pos];
                }
                //yield();
            }
            if (minBoundY == UINT8_MAX) return;

            sendCommand(COLUMNADDR);
            sendCommand(minBoundX);
            sendCommand(maxBoundX);

            sendCommand(PAGEADDR);
            sendCommand(minBoundY);
            sendCommand(maxBoundY);

            byte k = 0;
            for (y = minBoundY; y <= maxBoundY; y++)
            {
                for (x = minBoundX; x <= maxBoundX; x++)
                {
                    if (k == 0)
                    {
                        Wire.beginTransmission(_address);
                        Wire.write(0x40);
                    }

                    Wire.write(buffer_rotate[x + y * this->height()]);
                    k++;
                    if (k == 16)
                    {
                        Wire.endTransmission();
                        k = 0;
                    }
                }
                //yield();
            }

            if (k != 0) {
                Wire.endTransmission();
            }
        #else
            sendCommand(COLUMNADDR);
            sendCommand(0);
            sendCommand((this->height() - 1));

            sendCommand(PAGEADDR);
            sendCommand(0x0);

            if (geometry == GEOMETRY_128_64)
            {
                sendCommand(0x7);
            }
            else if (geometry == GEOMETRY_128_32)
            {
                sendCommand(0x3);
            }

            for (uint16_t i=0; i < displayBufferSize; i++)
            {
                Wire.beginTransmission(this->_address);
                Wire.write(0x40);
                for (uint8_t x = 0; x < 16; x++)
                {
                    Wire.write(buffer_rotate[i]);
                    i++;
                }
                i--;
                Wire.endTransmission();
            }
        #endif
        }
    }

    void setI2cAutoInit(bool doI2cAutoInit) {
      _doI2cAutoInit = doI2cAutoInit;
    }

    void stop(){
        end();
        Wire.end();
    }
  private:
    int getBufferOffset(void) {
        return 0;
    }
    inline void sendCommand(uint8_t command) __attribute__((always_inline)){
      initI2cIfNeccesary();
      Wire.beginTransmission(_address);
      Wire.write(0x80);
      Wire.write(command);
      Wire.endTransmission();
    }

    void initI2cIfNeccesary() {
      if (_doI2cAutoInit) {
        Wire.begin(_sda,_scl,_freq);
      }
    }

void sendInitCommands(void)
    {
        #if 1
        if (geometry == GEOMETRY_RAWMODE)
            return;
        sendCommand(DISPLAYOFF);
        sendCommand(SETDISPLAYCLOCKDIV);
        sendCommand(0xF0); // Increase speed of the display max ~96Hz
        sendCommand(SETMULTIPLEX);
        sendCommand(63); // sendCommand(this->height() - 1);
        sendCommand(SETDISPLAYOFFSET);
        sendCommand(0x00);
        sendCommand(SETSTARTLINE);
        sendCommand(CHARGEPUMP);
        sendCommand(0x10);
        sendCommand(MEMORYMODE);
        sendCommand(0x00);
        sendCommand(SEGREMAP|0x09);
        sendCommand(0xA0);
        sendCommand(COMSCANDEC);
        sendCommand(SETCOMPINS);

        if (geometry == GEOMETRY_128_64) {
        sendCommand(0x12);
        } else if (geometry == GEOMETRY_128_32) {
        sendCommand(0x02);
        }

        sendCommand(0x81);

        if (geometry == GEOMETRY_128_64) {
        sendCommand(0xFF);
        } else if (geometry == GEOMETRY_128_32) {
        sendCommand(0xFF);
        }

        sendCommand(SETPRECHARGE);
        sendCommand(0xF1);
        sendCommand(SETVCOMDETECT); //0xDB, (additionally needed to lower the contrast)
        sendCommand(0x30);          //0x40 default, to lower the contrast, put 0
        sendCommand(DISPLAYALLON_RESUME);
        sendCommand(NORMALDISPLAY);
        sendCommand(0x2e);            // stop scroll
        sendCommand(DISPLAYON);
        #else
        sendCommand(0xAE);/*display off*/
        sendCommand(0xA8);/*multiplex ratio*/
        sendCommand(0x3F);/*duty = 1/128*/
        sendCommand(0xad);/*External or Internal IREF Selection */
        sendCommand(0x40);
        sendCommand(0xD3);/*set display offset*/
        sendCommand(0x00);
        sendCommand(0xA0);/*set segment remap*/
        sendCommand(0xC8); /*Com scan direction*/
        sendCommand(0xA6);/*normal / reverse*/
        sendCommand(0x40);/*set display start line*/
        sendCommand(0xA4);
        sendCommand(0x81);/*contract control*/
        sendCommand(0x57);/*30*/
        sendCommand(0xD9);/*set pre-charge*/
        sendCommand(0x42);
        sendCommand(0xD5);/*set osc division*/
        sendCommand(0x90);
        sendCommand(0xDA);/*setCOM pins**/
        sendCommand(0x10);
        sendCommand(0x20);/*Set Memory Addressing Mode */
        sendCommand(0x09);/*Page Addressing Mode--02*/
        sendCommand(0xdb);/*setvcomh*/
        sendCommand(0x30);
        sendCommand(0x8d);
        sendCommand(0x10);
        /*set charge pump enable*/
        /*52-8.0V */
        sendCommand(0x00);/*set lower column address*/
        sendCommand(0x10);/*set higher column address*/
        sendCommand(0xAF);/*display ON*/
        #endif
    }

    void sendScreenRotateCommand(){
        switch(this->rotate_angle)
        {
            case ANGLE_0_DEGREE:
                sendCommand(SEGREMAP|0x00); // Set Segment Re-map   //a1 mirror
                sendCommand(0xc8); // Set Common scan direction
                break;
            case ANGLE_90_DEGREE:
                sendCommand(SEGREMAP|0x01); // Set Segment Re-map   //a1 mirror
                sendCommand(0xc8); // Set Common scan direction
                break;
            case ANGLE_180_DEGREE:
                sendCommand(SEGREMAP|0x01); // Set Segment Re-map   //a1 mirror
                sendCommand(0xc0); // Set Common scan direction
                break;
            case ANGLE_270_DEGREE:
                sendCommand(SEGREMAP|0x00); // Set Segment Re-map   //a1 mirror
                sendCommand(0xc0); // Set Common scan direction
                break;
            default:
                break;
        }
    }

};

#endif
