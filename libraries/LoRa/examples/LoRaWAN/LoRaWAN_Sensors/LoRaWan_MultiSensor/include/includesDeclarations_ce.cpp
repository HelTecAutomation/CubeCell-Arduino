#include "BH1750.h"
#include "BMP280.h"
#include "HDC1080.h"
#include "CCS811.h"
#include "hal/soc/flash.h"
#include "BME680.h"
#include "Seeed_BME280.h"
#include "BMP180.h"
#include "SHT2x.h"
#include "Adafruit_ADS1015.h"
#include "MPU9250.h"
#include "VL53L1X.h"
#include "HMC5883L.h"
#include "OneWire.h"
#include "DallasTemperature.h"
//#include "Adafruit_MLX90614.h"
//#include "MLX90614.h"
#include "Adafruit_SHT31.h"

#if (GPS_SER == 1)
#include "softSerial.h"
#include "TinyGPS++.h"
TinyGPSPlus gps;
softSerial ss(GPIO5 /*TX pin*/, GPIO3 /*RX pin*/);
#endif

bool BME_680_e[8]    = {0, 0, 0, 0, 0, 0, 0, 0};     //    1
bool BME_280_e[8]    = {0, 0, 0, 0, 0, 0, 0, 0};     //    2
bool CCS_811_e[8]    = {0, 0, 0, 0, 0, 0, 0, 0};     //    3
bool HDC_1080_e[8]   = {0, 0, 0, 0, 0, 0, 0, 0};     //    4
bool BMP_180_e[8]    = {0, 0, 0, 0, 0, 0, 0, 0};     //    5
bool BH_1750_e[8]    = {0, 0, 0, 0, 0, 0, 0, 0};     //    6
bool BMP_280_e[8]    = {0, 0, 0, 0, 0, 0, 0, 0};     //    7
bool SHT_2X_e[8]     = {0, 0, 0, 0, 0, 0, 0, 0};     //    8
bool ADS_1015_e[8]   = {0, 0, 0, 0, 0, 0, 0, 0};     //    9
bool MPU_9250_e[8]   = {0, 0, 0, 0, 0, 0, 0, 0};     //   10
bool LR_VL53L1X_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};     //   11
bool HMC_5883L_e[8]  = {0, 0, 0, 0, 0, 0, 0, 0};     //   12
//bool MLX_90614_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};      //   13
bool SHT_31_e[8]     = {0, 0, 0, 0, 0, 0, 0, 0};     //  14
bool One_Wire_e = false;                             // 100-103
// serial GPS on GPIO3 TX, GPIO5 RX                        200

#define INT_PIN GPIO2
bool accelWoke = false;

#define TCAADDR 0x70

float Temperature, Humidity, Pressure, lux, co2, tvoc;
uint16_t baseline, baselinetemp;
int count;
int maxtry = 50;

HDC1080 hdc1080;
CCS811 ccs;
BMP280 bmp280;
BH1750 lightMeter;
#define ROW 0
#define ROW_OFFSET 0
#define addr CY_SFLASH_USERBASE + CY_FLASH_SIZEOF_ROW *ROW + ROW_OFFSET
uint8_t baselineflash[2];
BME680_Class bme680;
BME280 bme280;
BMP085 bmp180;
MPU9250 mpu9250;
Adafruit_ADS1015 ads1015;
SFEVL53L1X LRSVL53L1X;
HMC5883L hmc5883;
//Adafruit_MLX90614 mlx = Adafruit_MLX90614();
//MLX90614 mlx = MLX90614(MLX90614_BROADCASTADDR);
Adafruit_SHT31 sht31 = Adafruit_SHT31();

OneWire ds(GPIO1); // on pin GPIO1 PIN6 (a 4.7K resistor is necessary)
DallasTemperature sensors(&ds);