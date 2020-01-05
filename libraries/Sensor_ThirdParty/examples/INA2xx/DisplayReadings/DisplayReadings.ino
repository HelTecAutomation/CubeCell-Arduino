/***************************************************************************************************************//*!
*
* @file DisplayReadings.ino
*
* @brief Example program for the INA Library demonstrating reading an INA device and displaying results
*
* @section DisplayReadings_section Description
*
* Program to demonstrate the INA library for the Arduino. When started, the library searches the I2C bus for all
* INA2xx devices. Then the example program goes into an infinite loop and displays the power measurements 
* (bus voltage and current) for all devices.\n\n
*
* Detailed documentation can be found on the GitHub Wiki pages at https://github.com/SV-Zanshin/INA/wiki \n\n
* This example is for a INA set up to measure a 5-Volt load with a 0.1 Ohm resistor in place, this is the same
* setup that can be found in the Adafruit INA219 breakout board.  The complex calibration options are done at
* runtime using the 2 parameters specified in the "begin()" call and the library has gone to great lengths to
* avoid the use of floating point to conserve space and minimize runtime.  This demo program uses floating point
* only to convert and display the data conveniently. The INA devices have 15 bits of precision, and even though
* the current and watt information is returned using 32-bit integers the precision remains the same.\n\n
*
* The library supports multiple INA devices and multiple INA device types. The Atmel's EEPROM is used to store
* the 96 bytes of static information per device using https://www.arduino.cc/en/Reference/EEPROM function calls.
* Although up to 16 devices could theoretically be present on the I2C bus the actual limit is determined by the
* available EEPROM - ATmega328 UNO has 1024k so can support up to 10 devices but the ATmega168 only has 512 bytes
* which limits it to supporting at most 5 INAs. Support has been added for the ESP32 based Arduinos, these use
* the EEPROM calls differently and need specific code.
*
* @section DisplayReadings_license GNU General Public License v3.0
*
* This program is free software : you can redistribute it and/or modify it under the terms of the GNU General
* Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
* option) any later version.This program is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.You should have received a copy of the GNU General Public License
* along with this program(see https://github.com/SV-Zanshin/INA/blob/master/LICENSE).  If not, see
* <http://www.gnu.org/licenses/>.
*
* @section DisplayReadings_author Author
*
* Written by Arnd\@SV-Zanshin
*
* @section DisplayReadings_versions Changelog
*
* Version | Date       | Developer                      | Comments
* ------- | ---------- | ------------------------------ | --------
* 1.0.4   | 2019-02-16 | https://github.com/SV-Zanshin  | Reformatted and refactored for legibility and clarity
* 1.0.3   | 2019-02-10 | https://github.com/SV-Zanshin  | Issue #38. Made pretty-print columns line up
* 1.0.3   | 2019-02-09 | https://github.com/SV-Zanshin  | Issue #38. Added device number to display
* 1.0.2   | 2018-12-29 | https://github.com/SV-Zanshin  | Converted comments to doxygen format
* 1.0.1   | 2018-09-22 | https://github.com/SV-Zanshin  | Cleaned up comments, added INA wait loop, removed F("") calls
* 1.0.0   | 2018-06-22 | https://github.com/SV-Zanshin  | Initial release
* 1.0.0b  | 2018-06-17 | https://github.com/SV-Zanshin  | INA219 and INA226 completed, including testing
* 1.0.0a  | 2018-06-10 | https://github.com/SV-Zanshin  | Initial coding
*
*******************************************************************************************************************/

#if ARDUINO >= 100 // Arduino IDE versions before 100 need to use the older library
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <INA.h> // Zanshin INA Library
#include <stdlib_noniso.h> // for dtostrf

/*******************************************************************************************************************
** Declare program constants, global variables and instantiate INA class                                          **
*******************************************************************************************************************/
const uint32_t SERIAL_SPEED = 115200; ///< Use fast serial speed 
uint8_t        devicesFound =      0; ///< Number of INAs found
INA_Class      INA;                   ///< INA class instantiation

/***************************************************************************************************************//*!
* @brief    Arduino method called once at startup to initialize the system
* @details  This is an Arduino IDE method which is called first upon boot or restart. It is only called one time
*           and then control goes to the main "loop()" method, from which control never returns. The serial port
*           is initialized and the INA.begin() method called to find all INA devices on the I2C bus and then the
*           devices are initialized to given conversion and averaging rates.
* @return   void
*******************************************************************************************************************/
void setup() 
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW); //set vext to high
  delay(500);
  
  Serial.begin(SERIAL_SPEED);
  #ifdef  __AVR_ATmega32U4__ // If a 32U4 processor, then wait 2 seconds for the serial interface to initialize
    delay(2000);
  #endif
  Serial.print("\n\nDisplay INA Readings V1.0.4\n");
  Serial.print(" - Searching & Initializing INA devices\n");
  /*****************************************************************************************************************
  ** The INA.begin call initializes the device(s) found with an expected ±1 Amps maximum current and for a 0.1Ohm **
  ** resistor, and since no specific device is given as the 3rd parameter all devices are initially set to these  **
  ** values.                                                                                                      **
  *****************************************************************************************************************/
  devicesFound = INA.begin(1,100000); // Set to an expected 1 Amp maximum and a 100000 microOhm resistor
  while (INA.begin(1, 100000) == 0)
  {
    Serial.println("No INA device found, retrying in 10 seconds...");
     delay(10000); // Wait 10 seconds before retrying 
  } // while no devices detected
  Serial.print(" - Detected ");
  Serial.print(devicesFound);
  Serial.println(" INA devices on the I2C bus");
  INA.setBusConversion(8500);            // Maximum conversion time 8.244ms
  INA.setShuntConversion(8500);          // Maximum conversion time 8.244ms
  INA.setAveraging(128);                 // Average each reading n-times
  INA.setMode(INA_MODE_CONTINUOUS_BOTH); // Bus/shunt measured continuously
  INA.AlertOnBusOverVoltage(true,5000);  // Trigger alert if over 5V on bus
} // method setup()

/***************************************************************************************************************//*!
* @brief    Arduino method for the main program loop
* @details  This is the main program for the Arduino IDE, it is an infinite loop and keeps on repeating. In order
*           to format the output use is made of the "sprintf()" function, but in the Arduino implementation it has
*           no support for floating point output, so the "dtostrf()" function is used to convert the floating point
*           numbers into formatted strings.
* @return   void
*******************************************************************************************************************/
void loop() 
{
  static uint16_t loopCounter = 0; // Count the number of iterations
  static char     sprintfBuffer[100]; // Buffer to format output
  static char     busChar[8], shuntChar[10], busMAChar[10], busMWChar[10]; // Floating point string output buffers

  Serial.print("Nr Adr Type   Bus      Shunt       Bus         Bus\n");
  Serial.print("== === ====== ======== =========== =========== ===========\n");
  for (uint8_t i=0;i<devicesFound;i++) // Loop through all devices
  {
    dtostrf(INA.getBusMilliVolts(i)   / 1000.0, 7, 4, busChar  ); // Convert floating point to character
    dtostrf(INA.getShuntMicroVolts(i) / 1000.0, 9, 4, shuntChar); // Convert floating point to character
    dtostrf(INA.getBusMicroAmps(i)    / 1000.0, 9, 4, busMAChar); // Convert floating point to character
    dtostrf(INA.getBusMicroWatts(i)   / 1000.0, 9, 4, busMWChar); // Convert floating point to character
    sprintf(sprintfBuffer, "%2d %3d %s %sV %smV %smA %smW\n", i+1, INA.getDeviceAddress(i),
            INA.getDeviceName(i), busChar, shuntChar, busMAChar, busMWChar);
    Serial.print(sprintfBuffer);
  } // for-next each INA device loop
  Serial.println();
  delay(5000); // Wait 5 seconds before next reading
  Serial.print("Loop iteration ");
  Serial.print(++loopCounter);
  Serial.print("\n\n");
} // method loop()
