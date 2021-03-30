### Third party sensors example

This folder is for the sensors made by third parties, examples code may had fixed, **developers can submit Push Request to this folder**.

- Separate source code and examples, source code (.h / .c / .cpp etc.) in src folder, example (.ino files) in examples folder with sensor type;
- Before PR, please make sure your examples can compile at least.

&nbsp;

Currently with following sensor breakout boards:

|       Sensor       |   Breakout Manufacturer    |                                                                       Library Source                                                                        |     Status     |
|:------------------:|:--------------------------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------:|:--------------:|
|    Bosch BMP280    |          Adafruit          |                                          [Adafruit](https://github.com/adafruit/Adafruit_BMP280_Library) - bundled                                          |   Works well   |
|    Bosch BME280    |        Seeed Studio        |                               [Seeed Studio](https://github.com/Seeed-Studio/Grove_BME280) - install through library manager                                |   Works well   |
|    Bosch BME680    |         SV-Zanshin         |                                                [SV-Zanshin](https://github.com/SV-Zanshin/BME680) - bundled                                                 |   Works well   |
|     TI ADS1x15     |          Adafruit          |                                 [Adafruit](https://github.com/adafruit/Adafruit_ADS1X15) - install through library manager                                  |   Works well   |
| Broadcom APDS-9960 |          Sparkfun          |                         [Sparkfun](https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor) - install through library manager                          |   Works well   |
| Broadcom APDS-9960 |         GY-9960LLC         |                      [Sparkfun - patched](https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor/pull/25) - download from Github                      |   Works well   |
|  Sensirion SHT2x   |             -              |                                               [SodaqMoja](https://github.com/SodaqMoja/Sodaq_SHT2x) - bundled                                               |   Works well   |
|     ST VL53L1X     |          Sparkfun          |                                     [Sparkfun](https://github.com/sparkfun/SparkFun_VL53L1X_Arduino_Library) - bundled                                      | not tested yet |
|   Maxim MAX44009   |     GY-49, CJMCU-44009     |                                    [Dan Tudose](https://github.com/dantudose/MAX44009) - install through library manager                                    |   Works well   |
|   SiLabs SI1145    |          GY-1145           | [Adafruit - patched by Oleg Moiseenko (merlokk), Mike Cochrane (MikeNZ)](https://github.com/mikenz/A9_Adafruit_SI1145/tree/CubeCell) - download from Github |   Works well   |
|      VEML6070      |        GY-VEML6070         |                                 [Adafruit](https://github.com/adafruit/Adafruit_VEML6070) - install through library manager                                 |   Works well   |
|       LM75A        |          CJMCU-75          |                              [M2M Solutions AB](https://github.com/m2m-solutions/M2M_LM75A) - install through library manager                               |   Works well   |
| Microchip MCP9808  |    Adafruit, CJMCU-9808    |                             [Adafruit](https://github.com/adafruit/Adafruit_MCP9808_Library) - install through library manager                              |   Works well   |
|       AM2320       |             -              |                                  [Adafruit](https://github.com/adafruit/Adafruit_AM2320) - install through library manager                                  |   Works well   |
|   SiLabs Si7021    | Adafruit, Sparkfun, GY-21  |                                  [Adafruit](https://github.com/adafruit/Adafruit_Si7021) - install through library manager                                  |   Works well   |
|     TE HTU21D      | Adafruit, Sparkfun, GY-21  |                     [Sparkfun](https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library) - install through library manager                      |   Works well   |
|    Asair AHT10     |     Various AliExpress     |                                             [Thinary](https://github.com/Thinary/AHT10) - download from Github                                              |   Works well   |
|  Sensirion SHT3x   |          Adafruit          |                                  [Adafruit](https://github.com/adafruit/Adafruit_SHT31) - install through library manager                                   |   Works well   |
|     ST VL53L0X     |  Adafruit, Pololu, GY-530  |                                 [Adafruit](https://github.com/adafruit/Adafruit_VL53L0X) - install through library manager                                  |   Works well   |
|     ST VL6180X     | Adafruit, Pololu, GY-6180  |                                 [Adafruit](https://github.com/adafruit/Adafruit_VL6180X) - install through library manager                                  |   Works well   |
|     ams CCS811     |     Adafruit, Sparkfun     |                                  [Adafruit](https://github.com/adafruit/Adafruit_CCS811) - install through library manager                                  |   Works well   |
|     TI INA219      | Adafruit, Sparkfun, GY-219 |                                  [Adafruit](https://github.com/adafruit/Adafruit_INA219) - install through library manager                                  |   Works well   |
|     TI INA219      | Adafruit, Sparkfun, GY-219 |                                      [SV-Zanshin](https://github.com/SV-Zanshin/INA) - install through library manager                                      |   Works well   |
|     TI INA226      |         CJMCU-226          |                                      [SV-Zanshin](https://github.com/SV-Zanshin/INA) - install through library manager                                      |   Works well   |
|   Maxim DS18B20    |             -              |          [Miles Burton](https://github.com/milesburton/Arduino-Temperature-Control-Library) - install through library manager "DallasTemperature"           |   Works well   |
|     SC16IS740      |        MJMCU-740           |                                           [rikkas7](https://github.com/rickkas7/SC16IS740RK)  - bundled                                                     | not tested yet |