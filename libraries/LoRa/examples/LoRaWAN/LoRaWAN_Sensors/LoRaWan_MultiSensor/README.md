# LoRaWan_MultiSensor
# v2.1.2 by [WASN.eu](https://www.wasn.eu)


This sketch allows you to use a number of sensors.
All you have to do is to set the number to a 1 beside the sensor you would like to use.
You can choose more than one sensor.

Selecting AUTO_SCAN the Firmware will try to identify the connected I2C Sensor by its address.
AUTO_SCAN takes its time, it is not very battery friendly. 
Please select the sensor for saving battery power, for maximum flexibility choose AUTO_SCAN.

    #define AUTO_SCAN 1
    #define BME_680 0
    #define BME_280 0
    #define CCS_811 0
    #define BMP_280 0
    #define BMP_180 0
    #define HDC_1080 0
    #define BH_1750 0
    #define SHT_2X 0
    #define ADS_1015 0
    #define MPU_9250 0
    #define LR_VL53L1X 0
    #define HMC_5883L 0
    #define MLX_90614 0
    #define One_Wire 1
 
set if you have the I2C Switch installed and if you only want to use some ports of it
 
    #define TCS9548 0      // TCS9548A I2C 8 port Switch
    #define minpnr 0       // TCS9548A first I2C Port
    #define maxpnr 7       // TCS9548A last I2C Port

and set your keys:

    * OTAA para*/
    uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
    uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

    /* ABP para*/
    uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
    uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
    uint32_t devAddr =  ( uint32_t )0x007e6ae1;

and set the DutyCycle in ms:

    /*the application data transmission duty cycle.  value in [ms].*/
    uint32_t appTxDutyCycle = 90000;


By default are uplinks send unconfirmed. 

Set the build option to true for confirmed uplinks.



TTN Decoder:

  The ttn-decode.js file automaticaly decodes all known sensor types.

Chirpstack Decoder:

  The chirpstack-decode.js file automaticaly decodes all known sensor types.

