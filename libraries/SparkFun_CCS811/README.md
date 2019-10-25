SparkFun CCS811 Arduino Library
========================================

![SparkFun CCS811 Breakout](https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library/blob/master/extras/readme_picture.jpg)

[*SparkFun CCS811 (SEN-14193)*](https://www.sparkfun.com/products/14193)

This is an arduino IDE library to control the CCS811.

This has been tested with Arduino Uno, ESP32, and Teensy 3.2 using Arduino 1.8.1.

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/extras** - Contains class diagrams for the driver.  Ignored by IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Examples
--------------

* BaselineOperator - Save and restore baselines to the EEPROM
* BasicReadings - Get data from the CCS811 as fast as possible
* BME280Compensated - Compensate the CCS811 with data from your BME280 sensor
* Core - Shows how the underlying hardware object works
* NTCCompensated - Compensate the CCS811 with data from a supplied NTC Thermistor
* setEnvironmentalReadings - Compensate the CCS with random data
* TwentyMinuteTest - Report data with timestamp.
* WakeAndInterrupt - Shows how to use the nWake and nInt pins

Documentation
--------------

* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.
* **[Product Repository](https://github.com/sparkfun/CCS811_Air_Quality_Breakout)** - Main repository (including hardware files) for the CCS811 Breakout.
* **[Hookup Guide](https://learn.sparkfun.com/tutorials/ccs811-air-quality-breakout-hookup-guide)** - Basic hookup guide for the CCS811 Breakout.

Products that use this Library 
---------------------------------

* [SEN-14193](https://www.sparkfun.com/)- CCS811 Breakout board

Version History
---------------

* [V 1.0.0](https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library/tree/V_1.0.0) -- Initial commit of Arduino compatible library.

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact techsupport@sparkfun.com.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.

