

#ifndef _HELTEC_H_
#define _HELTEC_H_

#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"


class Heltec_CubeCell {

 public:
    Heltec_CubeCell();
	 ~Heltec_CubeCell();

    void begin(bool DisplayEnable=true, bool SerialEnable=true);
    void end();

    SSD1306Wire *display;

    void VextON(void);
    void VextOFF(void);
};

extern Heltec_CubeCell Heltec;

#endif
