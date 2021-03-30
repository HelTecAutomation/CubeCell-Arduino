
/**************************************************************************/
/*!
This is a demo for the Adafruit MCP9808 breakout
----> http://www.adafruit.com/products/1782
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
*/

/*!
Adapted to CubeCell HTCC-AB01 by Mike Cochrane
December 29, 2019

Hardware Connections:

  CubeCell Pin  MCP9808 Board    Function

  Vext          Vdd              Power
  GND           Gnd              Ground
  SDA           SDA              I2C Data
  SCL           SCL              I2C Clock
*/
/**************************************************************************/

#include <Wire.h>
#include "Adafruit_MCP9808.h"

void setup()
{
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
    Serial.println("MCP9808 demo");
}

void loop()
{
    // Vext ON
    digitalWrite(Vext, LOW);
    delay(50);

    // Create the MCP9808 temperature sensor object
    Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

    // Make sure the sensor is found, you can also pass in a different i2c
    // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
    // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
    // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
    //  A2 A1 A0 address
    //  0  0  0   0x18  this is the default address
    //  0  0  1   0x19
    //  0  1  0   0x1A
    //  0  1  1   0x1B
    //  1  0  0   0x1C
    //  1  0  1   0x1D
    //  1  1  0   0x1E
    //  1  1  1   0x1F
    if (!tempsensor.begin(0x18))
    {
        Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");

        // Vext OFF
        digitalWrite(Vext, HIGH);
        delay(1000);
        return;
    }

    // Mode Resolution SampleTime
    //  0    0.5°C       30 ms
    //  1    0.25°C      65 ms
    //  2    0.125°C     130 ms
    //  3    0.0625°C    250 ms
    tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:

    // Read the temperature.
    float c = tempsensor.readTempC();
    float f = tempsensor.readTempF();
    Wire.end();

    // Vext OFF
    digitalWrite(Vext, HIGH);
    delay(1000);

    // Print the temperatures
    Serial.println();
    Serial.print("Temp: ");
    Serial.print(c, 4);
    Serial.print("*C\t and ");
    Serial.print(f, 4);
    Serial.println("*F.");
}
