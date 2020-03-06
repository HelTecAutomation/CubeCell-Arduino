#include "Wire.h"

//for asr6502, there are two i2c(Wire and Wire1).
//for asr6501, only one i2c(Wire).

void setup() {
  //i2c begin, default frequency is 100000;
  Wire.begin();//or Wire1.begin();

  //update frequency to 500000;
  Wire.setFrequency(500000);//or Wire1.setFrequency(500000);

  Wire.end();

  //i2c begin and set frequency to 500000;
  Wire.begin(500000);

  /*for asr6502, Wire also can be set to Wire1 as follow:
   * Wire.begin(1000000,1)      
   * in this case, Wire == Wire1;
   */
}

void loop() {
  // put your main code here, to run repeatedly:

}