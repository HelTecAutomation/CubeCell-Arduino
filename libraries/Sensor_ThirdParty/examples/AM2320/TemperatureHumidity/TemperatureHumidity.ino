/**
  Adapted to CubeCell HTCC-AB01 by Mike Cochrane
  December 29, 2019

  Hardware Connections:

   CubeCell Pin  AM2320 Module    Function

   Vext          1                Power
   SDA           2                I2C Data
   GND           3                Ground
   SCL           4                I2C Clock

   SDA and SCL both require pull up resistors to Vext

  See: https://learn.adafruit.com/adafruit-am2320-temperature-humidity-i2c-sensor/pinouts
*/

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();

void setup()
{
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
    Serial.println("Adafruit AM2320 Basic Test");
}

void loop()
{
    // Vext ON
    digitalWrite(Vext, LOW);
    delay(250);

    am2320.begin();
    delay(400);

    Serial.print("Temp: ");
    Serial.println(am2320.readTemperature());
    Serial.print("Hum: ");
    Serial.println(am2320.readHumidity());
    Wire.end();

    // Vext OFF
    digitalWrite(Vext, HIGH);
    delay(1000);
}
