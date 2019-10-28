/******************************************************************************
  Sensitivity Demo

  Marshall Taylor @ SparkFun Electronics

  April 4, 2017

  https://github.com/sparkfun/CCS811_Air_Quality_Breakout
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  Hardware Connections (Breakoutboard to Arduino):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to A4
  SCL to A5

  Generates random temperature and humidity data, and uses it to compensate the CCS811.
  This just demonstrates how the algorithm responds to various compensation points.
  Use NTCCompensated or BME280Compensated for real-world examples.

  Resources:
  Uses Wire.h for i2c operation

  Development environment specifics:
  Arduino IDE 1.8.1

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).

  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.

  Distributed as-is; no warranty is given.
******************************************************************************/
float temperatureVariable = 25.0; //in degrees C
float humidityVariable = 65.0; //in % relative

#include <Wire.h>
#include "SparkFunCCS811.h" //Click here to get the library: http://librarymanager/All#SparkFun_CCS811

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

CCS811 myCCS811(CCS811_ADDR);

void setup()
{
  Serial.begin(9600);
  Serial.println("CCS811 EnvironmentalReadings Example");

  Wire.begin();

  //This begins the CCS811 sensor and prints error status of .begin()
  CCS811Core::status returnCode = myCCS811.begin();
  Serial.print("begin exited with: ");
  printDriverError( returnCode );
  Serial.println();

}

void loop()
{
  Serial.println();
  //Randomize the Temperature and Humidity
  humidityVariable = (float)random(0, 10000) / 100; //0 to 100%
  temperatureVariable = (float)random(500, 7000) / 100; // 5C to 70C
  Serial.println("New humidity and temperature:");
  Serial.print("  Humidity: ");
  Serial.print(humidityVariable, 2);
  Serial.println("% relative");
  Serial.print("  Temperature: ");
  Serial.print(temperatureVariable, 2);
  Serial.println(" degrees C");
  myCCS811.setEnvironmentalData(humidityVariable, temperatureVariable);

  Serial.println("Environmental data applied!");
  myCCS811.readAlgorithmResults(); //Dump a reading and wait
  delay(1000);
  //Print data points
  for ( int i = 0; i < 10; i++)
  {
    if (myCCS811.dataAvailable())
    {
      //Calling readAlgorithmResults() function updates the global tVOC and CO2 variables
      myCCS811.readAlgorithmResults();

      Serial.print("CO2[");
      Serial.print(myCCS811.getCO2());
      Serial.print("] tVOC[");
      Serial.print(myCCS811.getTVOC());
      Serial.print("] millis[");
      Serial.print(millis());
      Serial.print("]");
      Serial.println();
    }
    else if (myCCS811.checkForStatusError())
    {
      //If the CCS811 found an internal error, print it.
      printSensorError();
    }
    delay(1000); //Wait for next reading
  }
}


//printDriverError decodes the CCS811Core::status type and prints the
//type of error to the serial terminal.
//
//Save the return value of any function of type CCS811Core::status, then pass
//to this function to see what the output was.
//printDriverError decodes the CCS811Core::status type and prints the
//type of error to the serial terminal.
//
//Save the return value of any function of type CCS811Core::status, then pass
//to this function to see what the output was.
void printDriverError( CCS811Core::status errorCode )
{
  switch ( errorCode )
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
}

//printSensorError gets, clears, then prints the errors
//saved within the error register.
void printSensorError()
{
  uint8_t error = myCCS811.getErrorRegister();

  if ( error == 0xFF ) //comm error
  {
    Serial.println("Failed to get ERROR_ID register.");
  }
  else
  {
    Serial.print("Error: ");
    if (error & 1 << 5) Serial.print("HeaterSupply");
    if (error & 1 << 4) Serial.print("HeaterFault");
    if (error & 1 << 3) Serial.print("MaxResistance");
    if (error & 1 << 2) Serial.print("MeasModeInvalid");
    if (error & 1 << 1) Serial.print("ReadRegInvalid");
    if (error & 1 << 0) Serial.print("MsgInvalid");
    Serial.println();
  }
}
