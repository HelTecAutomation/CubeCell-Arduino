#include "Wire.h"

//for asr6502, there are two i2c(Wire and Wire1). 
//for asr6501, only one i2c(Wire).
//for asr6601, there are three i2c(Wire,Wire1,Wire2).

void setup() {
  //i2c begin, default frequency is 100000;
  Wire.begin();//or Wire1.begin(); or Wire.begin(sdapin,sclpin,freq);

  //update frequency to 500000;
  Wire.setFrequency(500000);

  Wire.end();
}

void loop() {
  // put your main code here, to run repeatedly:

}