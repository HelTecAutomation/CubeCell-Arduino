/*
  Adapted to CubeCell HTCC-AB01 by Mike Cochrane
  December 31, 2019

  Hardware Connections:

   CubeCell Pin  AHT10 Board        Function

   Vext          VCC                Power
   GND           GND                Ground
   SDA           SDA                I2C Data
   SCL           SCL                I2C Clock

 */
 
#include <Wire.h>
#include <Thinary_AHT10.h>

AHT10Class AHT10;

void setup() {
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    // Vext ON
    digitalWrite(Vext, LOW);
    delay(50);
  
    Wire.begin();
    if (AHT10.begin(eAHT10Address_Low)) {
        Serial.begin(115200);
        Serial.println("Init AHT10 Sucess.");
    } else {
        Serial.begin(115200);
        Serial.println("Init AHT10 Failure.");
        digitalWrite(Vext, HIGH);
        delay(1000);
    }
    
    Serial.println(String("Humidity(%RH):\t\t") + AHT10.GetHumidity() + "%");
    Serial.println(String("Temperature(℃):\t\t") + AHT10.GetTemperature() + "℃");
    Serial.println(String("Dewpoint(℃):\t\t") + AHT10.GetDewPoint() + "℃");
    Serial.println();
    Wire.end();

    // Vext OFF
    digitalWrite(Vext, HIGH);
    delay(1000);
}
