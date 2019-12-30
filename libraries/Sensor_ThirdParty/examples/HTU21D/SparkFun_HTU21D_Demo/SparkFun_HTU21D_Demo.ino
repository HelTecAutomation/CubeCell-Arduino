/* 
 HTU21D Humidity Sensor Example Code
 By: Nathan Seidle
 SparkFun Electronics
 Date: September 15th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 Uses the HTU21D library to display the current humidity and temperature
 
  Adapted to CubeCell HTCC-AB01 by Mike Cochrane
  December 31, 2019

  Hardware Connections:

   CubeCell Pin  HTU21D Board       Function

   Vext          VCC                Power
   GND           GND                Ground
   SDA           SDA                I2C Data
   SCL           SCL                I2C Clock

 */

#include <Wire.h>
#include "SparkFunHTU21D.h"

//Create an instance of the object
HTU21D myHumidity;

void setup()
{
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
    Serial.println("HTU21D Example!");
}

void loop()
{
    // Vext ON
    digitalWrite(Vext, LOW);
    delay(50);
  
    myHumidity.begin();
    float humd = myHumidity.readHumidity();
    float temp = myHumidity.readTemperature();
    Wire.end();

    // Vext OFF
    digitalWrite(Vext, HIGH);

    Serial.print("Temperature: ");
    if (temp == ERROR_I2C_TIMEOUT) {
      Serial.print("i2c timeout");
    } else if (temp == ERROR_BAD_CRC) {
      Serial.print("bad crc");
    } else {
      Serial.print(temp, 1);
      Serial.print("C");
    }
    Serial.print(" Humidity: ");
    Serial.print(humd, 1);
    Serial.println("%");  
    delay(1000);
}
