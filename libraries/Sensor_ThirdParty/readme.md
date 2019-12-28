### Third party sensors example

This folder is for the sensors made by third parties, examples code may had fixed, **developers can submit Push Request to this folder**.

- Separate source code and examples, source code (.h / .c / .cpp etc.) in src folder, example (.ino files) in examples folder with sensor type;
- Before PR, please make sure your examples can compile at least.

&nbsp;

Currently with following sensor breakout boards:

|       Sensor       | Breakout Manufacturer |                                                                 Source                                                                  |     Status     |
|:------------------:|:---------------------:|:---------------------------------------------------------------------------------------------------------------------------------------:|:--------------:|
|    Bosch BMP280    |       Adafruit        |                                     [Adafruit](https://github.com/adafruit/Adafruit_BMP280_Library)                                     |   Works well   |
|    Bosch BME280    |     Seeed Studio      |                                      [Seeed Studio](https://github.com/Seeed-Studio/Grove_BME280)                                       |   Works well   |
|    Bosch BME680    |      SV-Zanshin       |                                           [SV-Zanshin](https://github.com/SV-Zanshin/BME680)                                            |   Works well   |
|     TI ADS1x15     |       Adafruit        |                                        [Adafruit](https://github.com/adafruit/Adafruit_ADS1X15)                                         |   Works well   |
| Broadcom APDS-9960 |       Sparkfun        |                                [Sparkfun](https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor)                                 |   Works well   |
| Broadcom APDS-9960 |      GY-9960LLC       |                       [Sparkfun - patched](https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor/pull/25)                        |   Works well   |
|  Sensirion SHT2x   |       SodaqMoja       |                                          [SodaqMoja](https://github.com/SodaqMoja/Sodaq_SHT2x)                                          | not tested yet |
|     ST VL53L1X     |       Sparkfun        |                                [Sparkfun](https://github.com/sparkfun/SparkFun_VL53L1X_Arduino_Library)                                 | not tested yet |
|   Maxim MAX44009   |  GY-49, CJMCU-44009   |                                           [Dan Tudose](https://github.com/dantudose/MAX44009)                                           |   Works well   |
|   SiLabs SI1145    |        GY-1145        | [Adafruit - patched by Oleg Moiseenko (merlokk) and Mike Cochrane (MikeNZ)](https://github.com/mikenz/A9_Adafruit_SI1145/tree/CubeCell) |   Works well   |
