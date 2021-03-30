/**
  Adapted to CubeCell HTCC-AB01 by Mike Cochrane
  December 29, 2019

  Hardware Connections:

   CubeCell Pin  Si7021 Board     Function

   Vext          VIN                Power
                 3.3v               3.3v out
   GND           GND                Ground
   SDA           SDA                I2C Data
   SCL           SCL                I2C Clock

*/

#include "Adafruit_Si7021.h"

Adafruit_Si7021 sensor = Adafruit_Si7021();

void setup()
{
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
    Serial.println("Si7021 test!");

    // Vext ON
    digitalWrite(Vext, LOW);
    delay(50);

    if (!sensor.begin())
    {
        Serial.println("Did not find Si7021 sensor!");
        while (true)
            ;
    }

    Serial.print("Found model ");
    switch (sensor.getModel())
    {
    case SI_Engineering_Samples:
        Serial.print("SI engineering samples");
        break;
    case SI_7013:
        Serial.print("Si7013");
        break;
    case SI_7020:
        Serial.print("Si7020");
        break;
    case SI_7021:
        Serial.print("Si7021");
        break;
    case SI_UNKNOWN:
    default:
        Serial.print("Unknown");
    }
    Serial.print(" Rev(");
    Serial.print(sensor.getRevision());
    Serial.print(")");
    Serial.print(" Serial #");
    Serial.print(sensor.sernum_a, HEX);
    Serial.println(sensor.sernum_b, HEX);
    Serial.println();
    delay(1000);
}

void loop()
{
    // Vext ON
    digitalWrite(Vext, LOW);
    delay(50);

    if (!sensor.begin())
    {
        Serial.println("Did not find Si7021 sensor!");
        digitalWrite(Vext, HIGH);
        delay(1000);
        return;
    }

    Serial.print("Humidity:    ");
    Serial.print(sensor.readHumidity(), 2);
    Serial.print("\tTemperature: ");
    Serial.println(sensor.readTemperature(), 2);
    Wire.end();

    // Vext OFF
    digitalWrite(Vext, HIGH);
    delay(1000);
}
