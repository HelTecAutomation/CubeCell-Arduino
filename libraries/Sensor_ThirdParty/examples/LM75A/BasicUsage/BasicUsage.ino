//---------------------------------------------------------------------------------------------
//
// Basic usage example for the M2M_LM75A Arduino library.
//
// Copyright 2016-2017, M2M Solutions AB
// Written by Jonny Bergdahl, 2016-11-18
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
//---------------------------------------------------------------------------------------------
// 2017-04-19 Added begin() and end() functions for Wire handling.
// 2017-04-19 Fixed a code merge problem
//
//
//---------------------------------------------------------------------------------------------
//
// Adapted to CubeCell HTCC-AB01 by Mike Cochrane
// December 29, 2019
//
// Hardware Connections:
//
//   CubeCell Pin  LM75A Board      Function
//
//   Vext          VIN              Power
//   GND           GND              Ground
//   SDA           SDA              I2C Data
//   SCL           SCL              I2C Clock
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <M2M_LM75A.h>

M2M_LM75A lm75a;

void setup()
{
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
    Serial.println(F("M2M_LM75A - Basic usage"));
    Serial.println(F("==========================================="));
    Serial.println();
}

void loop()
{
    // Vext ON
    digitalWrite(Vext, LOW);
    delay(50);

    lm75a.begin();

    // Temperature
    Serial.print(F("Temperature in Celsius: "));
    Serial.print(lm75a.getTemperature());
    Serial.println(F(" *C"));

    Serial.print(F("Temperature in Farenheit: "));
    Serial.print(lm75a.getTemperatureInFarenheit());
    Serial.println(F(" *F"));

    Serial.print(F("Hysteris temperature: "));
    Serial.print(lm75a.getHysterisisTemperature());
    Serial.println(F(" *C"));

    Serial.print(F("OS trip temperature: "));
    Serial.print(lm75a.getOSTripTemperature());
    Serial.println(F(" *C"));
    Serial.println();
    Serial.println();

    lm75a.end();

    // Vext OFF
    digitalWrite(Vext, HIGH);
    delay(1000);
}