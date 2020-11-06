/***************************************************************************
  This is a library for the CCS811 air

  This sketch reads the sensor

  Designed specifically to work with the Adafruit CCS811 breakout
  ----> http://www.adafruit.com/products/3566

  These sensors use I2C to communicate. The device's I2C address is 0x5A

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <CCS811.h>

CCS811 ccs;

void setup() {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);
  pinMode(GPIO0,OUTPUT);
  digitalWrite(GPIO0,LOW);
  Serial.begin(115200);
  Serial.println("CCS811 test");
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
}

void loop() {
  digitalWrite(GPIO0,LOW);
  if(ccs.available()){
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.println(" ppm");
      Serial.print("TVOC:");
      Serial.print(ccs.getTVOC());
      Serial.println(" ppb");
      Serial.print("millis:");
      Serial.println(millis());
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  digitalWrite(GPIO0,HIGH);
  delay(5000);
}
