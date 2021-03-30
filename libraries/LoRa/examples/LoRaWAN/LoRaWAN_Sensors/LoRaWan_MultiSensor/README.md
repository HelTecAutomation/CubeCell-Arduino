# LoRaWan_MultiSensor
# v3.0.7 by [WASN.eu](https://www.wasn.eu)


This sketch allows you to use a number of sensors.
All you have to do is to set the number to a 1 beside the sensor you would like to use.
You can choose more than one sensor.

You need to install these libraries:

|                   | Where to get                                 |
|-------------------|----------------------------------------------|
| Seeed_BME280      | library manager                              |
| Adafruit_ADS1015  | library manager                              |
| Seeed_HMC5883L    | library manager                              |
| DallasTemperature | library manager                              |
| Adafruit_SHT31    | library manager                              |
| TinyGPS++         | http://arduiniana.org/libraries/tinygpsplus/ |


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
    #define SHT_31 0

If you need the OneWire Interface on GPIO1 set the following to 1

    #define One_Wire 0     // OneWire on GPIO1

If you have a serial GPS connected to GPIO3 TX and GPIO5 RX st the following to 1
 
    #define GPS_SER 0      // serial GPS on GPIO3 TX, GPIO5 RX

For sending all data triggered by an interrupt on GPIO2 set the following to 1

    #define TriggerInt 0    // Interrupt on GPIO2, high for trigger

set if you have the I2C Switch installed and if you only want to use some ports of it
 
    #define TCS9548 0      // TCS9548A I2C 8 port Switch
    #define minpnr 0       // TCS9548A first I2C Port
    #define maxpnr 7       // TCS9548A last I2C Port

and set your keys:

    * OTAA para*/
    uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00 };
    uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    /* ABP para*/
    uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t appSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint32_t devAddr =  ( uint32_t )0x00000000;

and set the DutyCycle in ms:

    /*the application data transmission duty cycle.  value in [ms].*/
    uint32_t appTxDutyCycle = 90000;

Set the string wasnflash according to what you are using. IndoorNode and ModularNode are only useable with the original WASN hardware.

    String wasnflash = "Board"; //Board, Capsule, TCA9548A, IndoorNode, ModularNode

Please let all things below this text unchanged:

    /*
    NO USER CHANGES NEEDED UNDER THIS LINE
    */

By default are uplinks send unconfirmed. 

Set the build option to true for confirmed uplinks.



TTN Decoder:

  The ttn-decode.js file automaticaly decodes all known sensor types.

Chirpstack Decoder:

  The chirpstack-decode.js file automaticaly decodes all known sensor types.

