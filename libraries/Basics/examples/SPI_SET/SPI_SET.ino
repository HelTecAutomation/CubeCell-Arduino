#include "SPI.h"

//for asr6502, there are two i2c(Wire and Wire1). 
//for asr6501, only one i2c(Wire).
//for asr6601, there are three i2c(Wire,Wire1,Wire2).

//for asr6502, there are two spi(SPI and SPI1).
//for asr6501, only one spi(SPI).
//for asr6601, there are two spi(SPI, SPI1, SPI2).

void setup() {
  //spi begin with GPIO1 as nss, default frequency is 6000000;
  //SPI.begin() use default pin SCK,MISO,MOSI, nss is not setted, use software nss.
  SPI.begin(SCK,MISO,MOSI,GPIO1);// sck, miso, mosi, nss

  //update frequency to 10000000;
  SPI.setFrequency(1000000);

  SPI.end();
}

void loop() {
  // put your main code here, to run repeatedly:

}
