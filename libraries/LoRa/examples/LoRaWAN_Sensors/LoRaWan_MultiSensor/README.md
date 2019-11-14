# LoRaWan_MultiSensor
# v1.9.4 by [WASN.eu](https://wasn.eu)


This sketch allows you to use a number of sensors.
All you have to do is to set the number to a 1 beside the sensor you would like to use 
(only 1 selection is possible, selecting more will result in errors).

Selecting AUTO_SCAN the Firmware will try to identify the connected I2C Sensor by its address.
AUTO_SCAN takes its time, it is not very battery friendly. 
Please select the sensor for saving battery power, for maximum flexibility choose AUTO_SCAN.

    #define AUTO_SCAN  1
    #define MJMCU_8128 0
    #define BME_680    0 
    #define BME_280    0
    #define CCS_811    0
    #define BMP_180    0 
    #define HDC_1080   0
    #define BH_1750    0
    #define One_Wire   0 // not working

and set your keys:

    const char myDevEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const char myAppEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const char myAppKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

and set the DutyCycle in ms:

    uint32_t APP_TX_DUTYCYCLE = 900000;


TTN Decoder:

  The ttn-decode.js file automaticaly decodes all known sensor types.

Known Failures:

- One_Wire: no sensors found
