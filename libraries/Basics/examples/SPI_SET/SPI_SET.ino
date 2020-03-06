#include "SPI.h"

//for asr6502, there are two spi(SPI and SPI1).
//for asr6501, only one spi(SPI).

void setup() {
  //spi begin with GPIO1 as nss, default frequency is 6000000;
  SPI.begin(GPIO1);//or SPI1.begin(GPIO1);

  //update frequency to 10000000;
  SPI.setFrequency(1000000);//or SPI1.setFrequency(1000000);

  SPI.end();

  //spi begin with GPIO1 as nss, and set frequency to 1000000;
  SPI.begin(GPIO1,1000000);

  /*for asr6502, SPI also can be set to SPI1 as follow:
   * SPI.begin(GPIO1,1000000,1)      
   * in this case, SPI == SPI1;
   */
}

void loop() {
  // put your main code here, to run repeatedly:

}