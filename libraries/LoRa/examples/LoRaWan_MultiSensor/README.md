# LoRaWan_MultiSensor
# v1.8.1 by LoRa8.eu


This sketch allows you to use a number of sensors.
All you have to do is to set the number to a 1 beside the sensor you would like to use 
(only 1 selection is possible, selecting more will result in errors):

#define MJMCU_8128 1
#define BME_680    0 // wrong values
#define BME_280    0
#define CCS_811    0
#define BMP_180    0 // not tested
#define HDC_1080   0
#define BH_1750    0 // not yet
#define One_Wire   0 // sensors not found

and set your keys:

const char myDevEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

Known Failures:

1. BME_680 and One_wire: no sensors found
2. BH1750: not implemented yet