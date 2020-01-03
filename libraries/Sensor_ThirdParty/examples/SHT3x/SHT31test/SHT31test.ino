/*************************************************** 
  This is an example for the SHT31-D Humidity & Temp Sensor

  Designed specifically to work with the SHT31-D sensor from Adafruit
  ----> https://www.adafruit.com/products/2857

  These sensors use I2C to communicate, 2 pins are required to  
  interface

  Adapted to CubeCell HTCC-AB01 by Mike Cochrane
  December 31, 2019

  Hardware Connections:

   CubeCell Pin  SHT3x Board        Function

   Vext          VIN                Power
   GND           GND                Ground
   SDA           SDA                I2C Data
   SCL           SCL                I2C Clock
  
 ****************************************************/
 
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
    Serial.println("SHT31 test");
}


void loop() {
    // Vext ON
    digitalWrite(Vext, LOW);
    delay(50);
  
    if (!sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
        Serial.println("Couldn't find SHT31");
        digitalWrite(Vext, HIGH);
        delay(1000);
    }
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    Wire.end();

    // Vext OFF
    digitalWrite(Vext, HIGH);
  
    if (! isnan(t)) {  // check if 'is not a number'
        Serial.print("Temp *C = "); Serial.println(t);
    } else { 
        Serial.println("Failed to read temperature");
    }
  
    if (! isnan(h)) {  // check if 'is not a number'
        Serial.print("Hum. % = "); Serial.println(h);
    } else { 
        Serial.println("Failed to read humidity");
    }
    Serial.println();
    
    delay(1000);
}
