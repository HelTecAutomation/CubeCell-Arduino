/**
 * Adapted to CubeCell HTCC-AB01 by Mike Cochrane
 * December 28, 2019
 *
 * Hardware Connections:
 *
 *	CubeCell Pin  MAX44009 Board   Function
 *
 *	Vext          Vin              Power
 *	GND           GND              Ground
 *	SDA           SDA              I2C Data
 *	SCL           SCL              I2C Clock
 *
 * Development environment specifics:
 * Written in Arduino 1.8.10
 * MAX44009 library by Dan Tudose Version 1.0.1
 * Tested with CubeCell HTCC-AB01
 */

#include <Wire.h>
#include <MAX44009.h>

MAX44009 light;

void setup()
{
    // Vext ON
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
    delay(10);

    // Initialize Serial port
    Serial.begin(115200);
    Serial.println();
    Serial.println(F("-------------------------------"));
    Serial.println(F("MAX44009 - Ambient light sensor"));
    Serial.println(F("-------------------------------"));

    // Initilize sensor
    Wire.begin();
    delay(500);
    if (light.begin())
    {
        Serial.println(F("Could not find a valid MAX44009 sensor, check wiring!"));
    } else {
        Serial.println(F("MAX44009 initialization complete"));
    }
}

void loop()
{
    Serial.print("Light (lux):    ");
    Serial.println(light.get_lux());
    delay(1000);
}
