/******************************************************************************
  Core

  Marshall Taylor @ SparkFun Electronics

  April 4, 2017

  https://github.com/sparkfun/CCS811_Air_Quality_Breakout
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  This example shows how the normally hidden core class operates the wire interface.

  The class 'CCS811Core' abstracts the wire library and contains special hardware
  functions, and is normally not needed.

  Use this sketch to test the core of the library, or inherit it with your own
  functions for performing CCS811 operations.

  Hardware Connections (Breakoutboard to Arduino):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to A4
  SCL to A5


  Resources:
  Uses Wire.h for i2c operation

  Development environment specifics:
  Arduino IDE 1.8.1

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).

  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.

  Distributed as-is; no warranty is given.
******************************************************************************/
#include <Wire.h>

#include "SparkFunCCS811.h" //Click here to get the library: http://librarymanager/All#SparkFun_CCS811

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

CCS811Core mySensor(CCS811_ADDR);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("CCS811 Core Example");

  Wire.begin();

  //This setup routine is similar to what is used in the subclass' .begin() function
  CCS811Core::status returnCode = mySensor.beginCore(Wire); //Pass in the Wire port you want to use
  Serial.print("beginCore exited with: ");
  switch ( returnCode )
  {
    case CCS811Core::SENSOR_SUCCESS:
      Serial.print("SUCCESS");
      break;
    case CCS811Core::SENSOR_ID_ERROR:
      Serial.print("ID_ERROR");
      break;
    case CCS811Core::SENSOR_I2C_ERROR:
      Serial.print("I2C_ERROR");
      break;
    case CCS811Core::SENSOR_INTERNAL_ERROR:
      Serial.print("INTERNAL_ERROR");
      break;
    case CCS811Core::SENSOR_GENERIC_ERROR:
      Serial.print("GENERIC_ERROR");
      break;
    default:
      Serial.print("Unspecified error.");
  }
  
  //Write to this register to start app
  Wire.beginTransmission(CCS811_ADDR);
  Wire.write(CSS811_APP_START);
  Wire.endTransmission();

}

void loop()
{
  uint8_t arraySize = 10;
  uint8_t tempData[arraySize];

  tempData[0] = 0x18;
  tempData[1] = 0x27;
  tempData[2] = 0x36;
  tempData[3] = 0x45;

  mySensor.multiWriteRegister(0x11, tempData, 2);

  tempData[0] = 0x00;
  tempData[1] = 0x00;
  tempData[2] = 0x00;
  tempData[3] = 0x00;

  mySensor.multiReadRegister(0x11, tempData, 3);

  for ( int i = 0; i < arraySize; i++)
  {
    if (i % 8 == 0)
    {
      Serial.println();
      Serial.print("0x");
      Serial.print(i, HEX);
      Serial.print(":");
    }

    Serial.print(tempData[i], HEX);
    Serial.print(" ");
  }


  Serial.println("\n");
  delay(1000); //Wait for next reading
}
