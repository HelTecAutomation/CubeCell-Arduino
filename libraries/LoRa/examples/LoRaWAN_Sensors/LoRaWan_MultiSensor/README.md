# LoRaWan_MultiSensor
# v1.8.2 by WASN.eu


This sketch allows you to use a number of sensors.
All you have to do is to set the number to a 1 beside the sensor you would like to use 
(only 1 selection is possible, selecting more will result in errors).

Selecting AUTO_SCAN the Firmware will try to identify the connected I2C Sensor by its address.

#define AUTO_SCAN  1
#define MJMCU_8128 0
#define BME_680    0 // wrong values
#define BME_280    0
#define CCS_811    0
#define BMP_180    0 // not tested
#define HDC_1080   0
#define BH_1750    0
#define One_Wire   0 // sensors not found

and set your keys:

const char myDevEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

Known Failures:

1. BME_680 and One_wire: no sensors found