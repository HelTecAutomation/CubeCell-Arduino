/*
BME680 Test

Written by Arnd\@SV-Zanshin

@section I2CDemoversions Changelog

Version | Date       | Developer           | Comments
------- | ---------- | ------------------- | --------
1.0.1   | 2019-01-30 | https://github.com/SV-Zanshin | Removed old comments
1.0.1   | 2019-01-26 | https://github.com/SV-Zanshin | Issue #3 - convert documentation to Doxygen
1.0.0b  | 2018-06-30 | https://github.com/SV-Zanshin | Cloned from original BME280 program
*/
#include "BME680.h" // Include the BME680 Sensor library
/*******************************************************************************************************************
** Declare all program constants                                                                                  **
*******************************************************************************************************************/
const uint32_t SERIAL_SPEED = 115200; ///< Set the baud rate for Serial I/O

/*******************************************************************************************************************
** Declare global variables and instantiate classes                                                               **
*******************************************************************************************************************/
BME680_Class BME680; ///< Create an instance of the BME680

/*!
* @brief     This converts a pressure measurement into a height in meters
* @details   The corrected sea-level pressure can be passed into the function if it is know, otherwise the standard 
*            atmospheric pressure of 1013.25hPa is used (see https://en.wikipedia.org/wiki/Atmospheric_pressure
* @param[in] seaLevel Sea-Level pressure in millibars
* @return    floating point altitude in meters.
*/
float altitude(const float seaLevel=1013.25) 
{
  static float Altitude;
  int32_t temp, hum, press, gas;
  BME680.getSensorData(temp,hum,press,gas); // Get the most recent values from the device
  Altitude = 44330.0*(1.0-pow(((float)press/100.0)/seaLevel,0.1903)); // Convert into altitude in meters
  return(Altitude);
} // of method altitude()


/*!
    @brief    Arduino method called once at startup to initialize the system
    @details  This is an Arduino IDE method which is called first upon boot or restart. It is only called one time
              and then control goes to the main "loop()" method, from which control never returns
    @return   void
*/
void setup()
{
  //Vext ON
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);
  delay(500);

  Serial.begin(SERIAL_SPEED); // Start serial port at Baud rate
  #ifdef  __AVR_ATmega32U4__  // If this is a 32U4 processor, then wait 3 seconds to initialize USB
    delay(3000);
  #endif
  Serial.println(F("Starting I2CDemo example program for BME680"));
  Serial.print(F("- Initializing BME680 sensor\n"));
  while (!BME680.begin(I2C_STANDARD_MODE)) // Start BME680 using I2C protocol
  {
    Serial.println(F("-  Unable to find BME680. Waiting 3 seconds."));
    delay(3000);
  } // of loop until device is located
  Serial.println(F("- Setting 16x oversampling for all sensors"));
  BME680.setOversampling(TemperatureSensor,Oversample16); // Use enumerated type values
  BME680.setOversampling(HumiditySensor,   Oversample16);
  BME680.setOversampling(PressureSensor,   Oversample16);
  Serial.println(F("- Setting IIR filter to a value of 4 samples"));
  BME680.setIIRFilter(IIR4);
  Serial.println(F("- Setting gas measurement to 320\xC2\xB0\C for 150ms"));
  BME680.setGas(320,150); // 320�c for 150 milliseconds
  Serial.println();
} // of method setup()

/*!
    @brief    Arduino method for the main program loop
    @details  This is the main program for the Arduino IDE, it is an infinite loop and keeps on repeating. 
    @return   void
*/
void loop() 
{
  static uint8_t loopCounter = 0;
  static int32_t temperature, humidity, pressure, gas;     // Variable to store readings
  BME680.getSensorData(temperature,humidity,pressure,gas); // Get most recent readings
  Serial.print(temperature/100.0,2);                       // Temperature in deci-degrees
#ifdef ESP32
  Serial.print(F(" ")); // Esp32 compiler doesn't liked escaped string
#else
  Serial.print(F("\xC2\xB0\C "));                          // Representation of the � symbol
#endif
  Serial.print(humidity/1000.0,2);                         // Humidity in milli-percent
  Serial.print(F("%Hum "));
  Serial.print(pressure/100.0,2);                          // Pressure in Pascals
  Serial.print(F("hPa "));
  Serial.print(altitude(),2);
  Serial.print(F("m "));
  Serial.print(gas/100.0,2);
  Serial.println(F("mOhm"));
  delay(1000);
} // of method loop()
