/**
 * Adapted to CubeCell HTCC-AB01 by Mike Cochrane
 * December 28, 2019
 *
 * Hardware Connections:
 *
 *	CubeCell Pin  VEML6070 Board   Function
 *
 *	Vext          Vin              Power
 *	GND           GND              Ground
 *	SDA           SDA              I2C Data
 *	SCL           SCL              I2C Clock
 *
 * Development environment specifics:
 * Written in Arduino 1.8.10
 * Tested with CubeCell HTCC-AB01
 */

#include <Wire.h>
#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup()
{
    // Vext ON
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
    delay(10);

    // Initialize Serial port
    Serial.begin(115200);
    Serial.println();

    Serial.println("VEML6070 Test");
    uv.begin(VEML6070_1_T); // pass in the integration time constant
}

void loop()
{
    Serial.print("UV light level: ");
    Serial.println(uv.readUV());

    delay(1000);
}
