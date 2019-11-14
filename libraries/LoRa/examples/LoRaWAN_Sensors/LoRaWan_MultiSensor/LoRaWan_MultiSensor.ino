/*
  LoRaWan_MultiSensor
  programmed by WideAreaSensorNetwork
  v1.9.0 by WASN.eu
*/

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>

/*
 * Define your Settings below
 */

#define AUTO_SCAN  1
#define MJMCU_8128 0
#define BME_680    0 // wrong values
#define BME_280    0
#define CCS_811    0
#define BMP_180    0 // not tested
#define HDC_1080   0
#define BH_1750    0

const char myDevEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = 900000;

/*
  NO USER CHANGES NEEDED UNDER THIS LINE
*/

extern uint8_t DevEui[];
extern uint8_t AppEui[];
extern uint8_t AppKey[];

bool MJMCU_8128_e = false;
bool BME_680_e = false;
bool BME_280_e = false;
bool CCS_811_e = false;
bool BMP_180_e = false;
bool HDC_1080_e = false;
bool BH_1750_e = false;

uint8_t sensortype = 0;

#if(MJMCU_8128 == 1)
//#include "BH1750.h"
#include <BMP280.h>
#include "HDC1080.h"
#include <CCS811.h>
#include <hal/soc/flash.h>
MJMCU_8128_e = true;
BME_680_e = false;
BME_280_e = false;
CCS_811_e = false;
BMP_180_e = false;
HDC_1080_e = false;
BH_1750_e = false;
sensortype = 0;
#endif

#if(BME_680 == 1)
//#include "BH1750.h"
#include "BME680.h"
MJMCU_8128_e = false;
BME_680_e = true;
BME_280_e = false;
CCS_811_e = false;
BMP_180_e = false;
HDC_1080_e = false;
BH_1750_e = false;
sensortype = 1;
#endif

#if(BME_280 == 1)
//#include "BH1750.h"
#include "BME280.h"
MJMCU_8128_e = false;
BME_680_e = false;
BME_280_e = true;
CCS_811_e = false;
BMP_180_e = false;
HDC_1080_e = false;
BH_1750_e = false;
sensortype = 2;
#endif

#if(One_Wire == 1)
#include <OneWire.h>
MJMCU_8128_e = false;
BME_680_e = false;
BME_280_e = false;
CCS_811_e = false;
BMP_180_e = false;
HDC_1080_e = false;
BH_1750_e = false;
#endif

#if(CCS_811 == 1)
#include <CCS811.h>
MJMCU_8128_e = false;
BME_680_e = false;
BME_280_e = false;
CCS_811_e = true;
BMP_180_e = false;
HDC_1080_e = false;
BH_1750_e = false;
sensortype = 3;
#endif

#if(HDC_1080 == 1)
#include "HDC1080.h"
MJMCU_8128_e = false;
BME_680_e = false;
BME_280_e = false;
CCS_811_e = false;
BMP_180_e = false;
HDC_1080_e = true;
BH_1750_e = false;
sensortype = 4;
#endif

#if(BMP_180 == 1)
#include <BMP180.h>
MJMCU_8128_e = false;
BME_680_e = false;
BME_280_e = false;
CCS_811_e = false;
BMP_180_e = true;
HDC_1080_e = false;
BH_1750_e = false;
sensortype = 5;
#endif

#if(BH_1750 == 1)
#include <BH1750.h>
MJMCU_8128_e = false;
BME_680_e = false;
BME_280_e = false;
CCS_811_e = false;
BMP_180_e = false;
HDC_1080_e = false;
BH_1750_e = true;
sensortype = 6;
#endif

#if(AUTO_SCAN == 1)
#include "BH1750.h"
#include <BMP280.h>
#include "HDC1080.h"
#include <CCS811.h>
#include <hal/soc/flash.h>
#include "BME680.h"
#include "BME280.h"
#include <BMP180.h>
#endif

/*
   set LoraWan_RGB to Active,the RGB active in loraWan
   RGB red means sending;
   RGB purple means joined done;
   RGB blue means RxWindow1;
   RGB yellow means RxWindow2;
   RGB green means received done;
*/
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

/*
   set to 1 the enable AT mode
   set to 0 the disable support AT mode
*/
#ifndef AT_SUPPORT
#define AT_SUPPORT 0
#endif

#ifndef ACTIVE_REGION
#define ACTIVE_REGION LORAMAC_REGION_EU868
#endif

#ifndef LORAWAN_CLASS
#define LORAWAN_CLASS CLASS_A
#endif

/*LoraWan Class*/
DeviceClass_t  CLASS = LORAWAN_CLASS;
/*OTAA or ABP*/
bool OVER_THE_AIR_ACTIVATION = LORAWAN_NETMODE;
/*ADR enable*/
bool LORAWAN_ADR_ON = LORAWAN_ADR;
/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool KeepNet = LORAWAN_Net_Reserve;
/*LoraWan REGION*/
LoRaMacRegion_t REGION = ACTIVE_REGION;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool IsTxConfirmed = true;
/*!
  Number of trials to transmit the frame, if the LoRaMAC layer did not
  receive an acknowledgment. The MAC performs a datarate adaptation,
  according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
  to the following table:

  Transmission nb | Data Rate
  ----------------|-----------
  1 (first)       | DR
  2               | DR
  3               | max(DR-1,0)
  4               | max(DR-1,0)
  5               | max(DR-2,0)
  6               | max(DR-2,0)
  7               | max(DR-3,0)
  8               | max(DR-3,0)

  Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
  the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t ConfirmedNbTrials = 8;

/* Application port */
uint8_t AppPort = 2;

float Temperature, Humidity, Pressure, lux, co2, tvoc;
uint16_t baseline, baselinetemp;
int count;
int maxtry = 50;

#if(MJMCU_8128 == 1)
HDC1080 hdc1080;
CCS811 ccs;
BMP280 bmp;
//BH1750 lightMeter;

#define ROW 0
#define ROW_OFFSET 0
#define addr CY_SFLASH_USERBASE+CY_FLASH_SIZEOF_ROW*ROW + ROW_OFFSET
uint8_t baselineflash[2];
#endif

#if(BME_680 == 1)
BME680 bme680;
//BH1750 lightMeter;
#endif

#if(BME_280 == 1)
BME280 bme280;
//BH1750 lightMeter;
#endif

#if(One_Wire == 1)
OneWire  ds(GPIO0);
#endif

#if(CCS_811 == 1)
CCS811 ccs;
#endif

#if(HDC_1080 == 1)
HDC1080 hdc0180;
#endif

#if(BMP_180 == 1)
BMP085 bmp180;
#endif

#if(BH_1750 == 1)
BH1750 lightMeter;
#endif

#if(AUTO_SCAN == 1)
HDC1080 hdc1080;
CCS811 ccs;
BMP280 bmp;
BH1750 lightMeter;
#define ROW 0
#define ROW_OFFSET 0
#define addr CY_SFLASH_USERBASE+CY_FLASH_SIZEOF_ROW*ROW + ROW_OFFSET
uint8_t baselineflash[2];
BME680 bme680;
BME280 bme280;
BMP085 bmp180;
#endif

/*!
   \brief   Prepares the payload of the frame
*/

static void PrepareTxFrame( uint8_t port )
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

  /*
      MJMCU-8128
  */
  if (MJMCU_8128_e) {
    count = 0;
    hdc1080.begin(0x40);
    delay(500);
    Temperature = (float)(hdc1080.readTemperature());
    Humidity = (float)(hdc1080.readHumidity());
    Wire.end();
    while (Temperature > 120.0 && count < maxtry) {
      hdc1080.begin(0x40);
      delay(500);
      Temperature = (float)(hdc1080.readTemperature());
      Humidity = (float)(hdc1080.readHumidity());
      Wire.end();
      count++;
      delay(500);
    }
    if (Temperature > 120.0) {
      Temperature = 0.0;
      Humidity = 0.0;
      Serial.println("HDC ERROR");
    }
    hdc1080.end();

    count = 0;
    ccs.begin();
    delay(1000);

    FLASH_read_at(addr, baselineflash, sizeof(baselineflash));
    baselinetemp = (baselineflash[0] << 8) | baselineflash[1];
    if (baselinetemp > 0) {
      baseline = baselinetemp;
      Serial.print("Read BaseLine: ");
      Serial.println(baseline);
      ccs.setBaseline(baseline);
    }
    delay(5000);

    while (!ccs.available());
    ccs.readData();
    co2 = ccs.geteCO2();
    tvoc = ccs.getTVOC();

    baseline = ccs.getBaseline();
    baselineflash[0] = (uint8_t)(baseline >> 8);
    baselineflash[1] = (uint8_t)baseline;
    FLASH_update(addr, baselineflash, sizeof(baselineflash));
    Serial.print("Write BaseLine: ");
    Serial.println(baseline);
    Wire.end();
    while (co2 > 65500.0 && count < maxtry) {
      ccs.begin();
      delay(1000);
      while (!ccs.available());
      ccs.readData();
      co2 = ccs.geteCO2();
      //co2 = ccs.getCO2();
      tvoc = ccs.getTVOC();
      Wire.end();
      count++;
    }
    if (co2 > 65500.0) {
      co2 = 0.0;
      tvoc = 0.0;
      Serial.println("CCS ERROR");
    }

    count = 0;
    bmp.begin();
    delay(500);
    bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                    BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    BMP280::FILTER_X16,      /* Filtering. */
                    BMP280::STANDBY_MS_500); /* Standby time. */
    float temp = bmp.readTemperature();
    Pressure = (float)bmp.readPressure() / 100.0;
    Wire.end();
    while (Pressure > 1190.0 && count < maxtry) {
      bmp.begin();
      delay(500);
      bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                      BMP280::SAMPLING_X2,     /* Temp. oversampling */
                      BMP280::SAMPLING_X16,    /* Pressure oversampling */
                      BMP280::FILTER_X16,      /* Filtering. */
                      BMP280::STANDBY_MS_500); /* Standby time. */
      Pressure = (float)bmp.readPressure() / 100.0;
      Wire.end();
      count++;
      delay(500);
    }
    if (Pressure > 1190.0) {
      Pressure = 0;
      Serial.println("BMP ERROR");
    }

    //  if (!lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2)) {
    //    Serial.print("Failed to start BH2750!");
    //  }
    //  float lux = lightMeter.readLightLevel();
    //  lightMeter.end();
    lux = 0.0;

    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();

    AppDataSize = 15;//AppDataSize max value is 64
    AppData[0] = (uint8_t)sensortype;
     
    AppData[1] = (uint8_t)((int)((Temperature+100.0)*10.0) >> 8);
    AppData[2] = (uint8_t)((int)((Temperature+100.0)*10.0));
    
    AppData[3] = (uint8_t)((int)(Humidity*10.0) >> 8);
    AppData[4] = (uint8_t)((int)(Humidity*10.0));
    
    AppData[5] = (uint8_t)((int)(Pressure*10.0) >> 8);;
    AppData[6] = (uint8_t)((int)(Pressure*10.0));
    
    AppData[7] = (uint8_t)((int)(lux*10.0) >> 8);
    AppData[8] = (uint8_t)((int)(lux*10.0));

    AppData[9] = (uint8_t)((int)co2 >> 8);
    AppData[10] = (uint8_t)((int)co2);
    
    AppData[11] = (uint8_t)((int)tvoc >> 8);
    AppData[12] = (uint8_t)((int)tvoc);

    AppData[13] = (uint8_t)(BatteryVoltage >> 8);
    AppData[14] = (uint8_t)BatteryVoltage;

    Serial.print("T=");
    Serial.print(Temperature);
    Serial.print("C, RH=");
    Serial.print(Humidity);
    Serial.print("%, Lux=");
    Serial.print(lux);
    Serial.print(" lx, Pressure=");
    Serial.print(Pressure);
    Serial.print(" hPA, CO2=");
    Serial.print(co2);
    Serial.print(" ppm, TVOC=");
    Serial.print(tvoc);
    Serial.print(" ppb, Baseline: ");
    Serial.print(baseline);
    Serial.print(", BatteryVoltage:");
    Serial.println(BatteryVoltage);
  }

  /*
     BME680
  */

  if (BME_680_e) {
    //  if (!lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2)) {
    //    Serial.print("Failed to start BH2750!");
    //  }
    //  float lux = lightMeter.readLightLevel();
    //  lightMeter.end();
    lux = 0.0;
    bme680.init(0x77); // I2C address: 0x76 or 0x77
    delay(1000);
    bme680.reset();

    Serial.print("Chip ID=0x");
    Serial.println(bme680.getChipID(), HEX);

    // oversampling: humidity = x1, temperature = x2, pressure = x16
    bme680.setOversampling(BME680_OVERSAMPLING_X1, BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X16);
    bme680.setIIRFilter(BME680_FILTER_3);
    bme680.setGasOn(300, 100); // 300 degree Celsius and 100 milliseconds

    bme680.setForcedMode();
    BME680_Status status = readAndPrintStatus();

    count = 0;
    while (status.newDataFlag == 0 && count < maxtry) {
      delay(1000);
      BME680_Status status = readAndPrintStatus();
      count++;
    }

    Serial.print("result: ");
    Temperature = bme680.readTemperature();
    Pressure = bme680.readPressure();
    Humidity = bme680.readHumidity();
    tvoc = bme680.readGasResistance();

    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();

    AppDataSize = 11;//AppDataSize max value is 64
    AppData[0] = (uint8_t)sensortype;
    
    AppData[1] = (uint8_t)((int)((Temperature+100.0)*10.0) >> 8);
    AppData[2] = (uint8_t)((int)((Temperature+100.0)*10.0));
    
    AppData[3] = (uint8_t)((int)(Humidity*10.0) >> 8);
    AppData[4] = (uint8_t)((int)(Humidity*10.0));
    
    AppData[5] = (uint8_t)((int)(Pressure*10.0) >> 8);;
    AppData[6] = (uint8_t)((int)(Pressure*10.0));
    
    AppData[7] = (uint8_t)((int)tvoc >> 8);
    AppData[8] = (uint8_t)((int)tvoc);

    AppData[9] = (uint8_t)(BatteryVoltage >> 8);
    AppData[10] = (uint8_t)BatteryVoltage;
    
    Serial.print("T=");
    Serial.print(Temperature);
    Serial.print("C, RH=");
    Serial.print(Humidity);
    Serial.print("%, Lux=");
    Serial.print(lux);
    Serial.print(" lx, Pressure=");
    Serial.print(Pressure);
    Serial.print(" hPA, GAS=");
    Serial.print(tvoc);
    Serial.print(", BatteryVoltage:");
    Serial.println(BatteryVoltage);

    bme680.setForcedMode();
  }

  /*
     BME280
  */
  if (BME_280_e) {
    if (!bme280.init()) {
      Serial.println("Device error!");
    }
    delay(1000);
    Temperature = bme280.getTemperature();
    Pressure = bme280.getPressure() / 100.0;
    Humidity = bme280.getHumidity();

    //  if (!lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2)) {
    //    Serial.print("Failed to start BH2750!");
    //  }
    //  float lux = lightMeter.readLightLevel();
    //  lightMeter.end();
    lux = 0.0;
    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();
   
    AppDataSize = 9;//AppDataSize max value is 64
    AppData[0] = (uint8_t)sensortype;
    
    AppData[1] = (uint8_t)((int)((Temperature+100.0)*10.0) >> 8);
    AppData[2] = (uint8_t)((int)((Temperature+100.0)*10.0));
    
    AppData[3] = (uint8_t)((int)(Humidity*10.0) >> 8);
    AppData[4] = (uint8_t)((int)(Humidity*10.0));
    
    AppData[5] = (uint8_t)((int)(Pressure*10.0) >> 8);;
    AppData[6] = (uint8_t)((int)(Pressure*10.0));

    AppData[7] = (uint8_t)(BatteryVoltage >> 8);
    AppData[8] = (uint8_t)BatteryVoltage;

    Serial.print("T=");
    Serial.print(Temperature);
    Serial.print("C, RH=");
    Serial.print(Humidity);
    Serial.print("%, Lux=");
    Serial.print(lux);
    Serial.print(" lx, Pressure=");
    Serial.print(Pressure);
    Serial.print(" hPA, BatteryVoltage:");
    Serial.println(BatteryVoltage);
  }

  /*
    CCS811
  */

  if (CCS_811_e) {
    count = 0;
    ccs.begin();
    delay(1000);

    FLASH_read_at(addr, baselineflash, sizeof(baselineflash));
    baselinetemp = (baselineflash[0] << 8) | baselineflash[1];
    if (baselinetemp > 0) {
      baseline = baselinetemp;
      Serial.print("Read BaseLine: ");
      Serial.println(baseline);
      ccs.setBaseline(baseline);
    }
    delay(5000);

    while (!ccs.available());
    ccs.readData();
    Temperature = ccs.calculateTemperature();
    co2 = ccs.geteCO2();
    tvoc = ccs.getTVOC();

    baseline = ccs.getBaseline();
    baselineflash[0] = (uint8_t)(baseline >> 8);
    baselineflash[1] = (uint8_t)baseline;
    FLASH_update(addr, baselineflash, sizeof(baselineflash));
    Serial.print("Write BaseLine: ");
    Serial.println(baseline);
    Wire.end();
    while (co2 > 65500.0 && count < maxtry) {
      ccs.begin();
      delay(1000);
      while (!ccs.available());
      ccs.readData();
      co2 = ccs.geteCO2();
      //co2 = ccs.getCO2();
      tvoc = ccs.getTVOC();
      Wire.end();
      count++;
    }
    if (co2 > 65500.0) {
      co2 = 0.0;
      tvoc = 0.0;
      Serial.println("CCS ERROR");
    }

    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();

    AppDataSize = 7;//AppDataSize max value is 64
    AppData[0] = (uint8_t)sensortype;

    AppData[1] = (uint8_t)((int)co2 >> 8);
    AppData[2] = (uint8_t)((int)co2);
    
    AppData[3] = (uint8_t)((int)tvoc >> 8);
    AppData[4] = (uint8_t)((int)tvoc);

    AppData[5] = (uint8_t)(BatteryVoltage >> 8);
    AppData[6] = (uint8_t)BatteryVoltage;

    Serial.print("T=");
    Serial.print(Temperature);
    Serial.print("C, CO2=");
    Serial.print(co2);
    Serial.print(" ppm, TVOC=");
    Serial.print(tvoc);
    Serial.print(" ppb, Baseline: ");
    Serial.print(baseline);
    Serial.print(", BatteryVoltage:");
    Serial.println(BatteryVoltage);
  }

  /*
    HDC1080
  */

  if (HDC_1080_e) {
    count = 0;
    hdc1080.begin(0x40);
    delay(500);
    Temperature = (float)(hdc1080.readTemperature());
    Humidity = (float)(hdc1080.readHumidity());
    Wire.end();
    while (Temperature > 120.0 && count < maxtry) {
      hdc1080.begin(0x40);
      delay(500);
      Temperature = (float)(hdc1080.readTemperature());
      Humidity = (float)(hdc1080.readHumidity());
      Wire.end();
      count++;
      delay(500);
    }
    if (Temperature > 120.0) {
      Temperature = 0.0;
      Humidity = 0.0;
      Serial.println("HDC ERROR");
    }
    hdc1080.end();

    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();

    AppDataSize = 7;//AppDataSize max value is 64
    AppData[0] = (uint8_t)sensortype;
    
    AppData[1] = (uint8_t)((int)((Temperature+100.0)*10.0) >> 8);
    AppData[2] = (uint8_t)((int)((Temperature+100.0)*10.0));
    
    AppData[3] = (uint8_t)((int)(Humidity*10.0) >> 8);
    AppData[4] = (uint8_t)((int)(Humidity*10.0));

    AppData[5] = (uint8_t)(BatteryVoltage >> 8);
    AppData[6] = (uint8_t)BatteryVoltage;

    Serial.print("T=");
    Serial.print(Temperature);
    Serial.print("C, RH=");
    Serial.print(Humidity);
    Serial.print("%, BatteryVoltage:");
    Serial.println(BatteryVoltage);
  }

  if (BMP_180_e) {
    count = 0;
    bmp180.begin();
    delay(500);
    Temperature = (float)(bmp180.readTemperature());
    Pressure = (float)(bmp180.readPressure()) / 100.0;
    Wire.end();
    while (Temperature > 120.0 && count < maxtry) {
      bmp180.begin();
      delay(500);
      Temperature = (float)(bmp180.readTemperature());
      Pressure = (float)(bmp180.readPressure()) / 100.0;
      Wire.end();
      count++;
      delay(500);
    }
    if (Temperature > 120.0) {
      Temperature = 0.0;
      Humidity = 0.0;
      Serial.println("BMP ERROR");
    }

    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();
    
    AppDataSize = 7;//AppDataSize max value is 64
    AppData[0] = (uint8_t)sensortype;
    
    AppData[1] = (uint8_t)((int)((Temperature+100.0)*10.0) >> 8);
    AppData[2] = (uint8_t)((int)((Temperature+100.0)*10.0));
    
    AppData[3] = (uint8_t)((int)(Pressure*10.0) >> 8);;
    AppData[4] = (uint8_t)((int)(Pressure*10.0));

    AppData[5] = (uint8_t)(BatteryVoltage >> 8);
    AppData[6] = (uint8_t)BatteryVoltage;

    Serial.print("T=");
    Serial.print(Temperature);
    Serial.print("C, P=");
    Serial.print(Pressure);
    Serial.print("hPa, BatteryVoltage:");
    Serial.println(BatteryVoltage);
  }

  /*
    BH1750
  */

  if (BH_1750_e) {
    count = 0;
    lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2);
    delay(100);
    lux = lightMeter.readLightLevel();
    delay(100);
    lux = lightMeter.readLightLevel();
    lightMeter.end();
    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();

    AppDataSize = 5;//AppDataSize max value is 64
    AppData[0] = (uint8_t)sensortype;
    
    AppData[1] = (uint8_t)((int)(lux*10.0) >> 8);
    AppData[2] = (uint8_t)((int)(lux*10.0));

    AppData[3] = (uint8_t)(BatteryVoltage >> 8);
    AppData[4] = (uint8_t)BatteryVoltage;

    Serial.print("Light=");
    Serial.print(lux);
    Serial.print("lx, BatteryVoltage:");
    Serial.println(BatteryVoltage);
  }
}

#if(BME_680 == 1 || AUTO_SCAN == 1)
BME680_Status readAndPrintStatus() {
  BME680_Status status = bme680.readStatus();
  Serial.print("status: (");
  Serial.print(status.newDataFlag);
  Serial.print(",");
  Serial.print(status.measuringStatusFlag);
  Serial.print(",");
  Serial.print(status.gasMeasuringStatusFlag);
  Serial.print(",");
  Serial.print(status.gasMeasurementIndex);
  Serial.println(") (newDataFlag,StatusFlag,GasFlag,GasIndex)");
  return status;
}
#endif

void setup() {
  memcpy(DevEui, myDevEui, sizeof(myDevEui)); //Add these 3 lines to setup func
  memcpy(AppEui, myAppEui, sizeof(myAppEui));
  memcpy(AppKey, myAppKey, sizeof(myAppKey));
  BoardInitMcu( );
  Serial.begin(115200);
#if(AT_SUPPORT == 1)
  Enable_AT();
#endif
  DeviceState = DEVICE_STATE_INIT;
  LoRaWAN.Ifskipjoin();

#if(AUTO_SCAN == 1)
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW); //set vext to high
  delay(500);
  Wire.begin();
  byte error, address, sum;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  sum = 0;
  for (address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
      sum += address;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println("No I2C devices found");
  } else {
    Serial.print("Address SUM: ");
    Serial.println(sum);
  }
  Wire.end();

  switch (sum)
  {
    case 35: //0x23 -- BH1750
      {
        Serial.println("found BH1750");
        MJMCU_8128_e = false;
        BME_680_e = false;
        BME_280_e = false;
        CCS_811_e = false;
        BMP_180_e = false;
        HDC_1080_e = false;
        BH_1750_e = true;
        sensortype = 6;
        break;
      }
    case 64: //0x40 -- HDC1080 temperature and humidity sensor
      {
        Serial.println("Found HDC1080");
        MJMCU_8128_e = false;
        BME_680_e = false;
        BME_280_e = false;
        CCS_811_e = false;
        BMP_180_e = false;
        HDC_1080_e = true;
        BH_1750_e = false;
        sensortype = 4;
        break;
      }
    case 90: //0x5A --CCS811
      {
        Serial.println("Found CCS811");
        MJMCU_8128_e = false;
        BME_680_e = false;
        BME_280_e = false;
        CCS_811_e = true;
        BMP_180_e = false;
        HDC_1080_e = false;
        BH_1750_e = false;
        sensortype = 3;
        break;
      }
    case 118: //0x76 -- BME280
      {
        Serial.println("Found BME280");
        MJMCU_8128_e = false;
        BME_680_e = false;
        BME_280_e = true;
        CCS_811_e = false;
        BMP_180_e = false;
        HDC_1080_e = false;
        BH_1750_e = false;
        sensortype = 2;
        break;
      }
    case 119: //0x77 -- BMP180 Barometer
      {
        Serial.println("Found BMP180");
        MJMCU_8128_e = false;
        BME_680_e = false;
        BME_280_e = false;
        CCS_811_e = false;
        BMP_180_e = true;
        HDC_1080_e = false;
        BH_1750_e = false;
        sensortype = 5;
        break;
      }
    case 16: //MJMCU-8128
      {
        Serial.println("Found MJMCU-8128");
        MJMCU_8128_e = true;
        BME_680_e = false;
        BME_280_e = false;
        CCS_811_e = false;
        BMP_180_e = false;
        HDC_1080_e = false;
        BH_1750_e = false;
        sensortype = 0;
        break;
      }
    case 65: //BME680
      {
        Serial.println("Found BME680");
        MJMCU_8128_e = false;
        BME_680_e = true;
        BME_280_e = false;
        CCS_811_e = false;
        BMP_180_e = false;
        HDC_1080_e = false;
        BH_1750_e = false;
        sensortype = 1;
        break;
      }
  }
#endif
}

void loop()
{
  switch ( DeviceState )
  {
    case DEVICE_STATE_INIT:
      {
#if(AT_SUPPORT == 1)
        getDevParam();
#endif
        printDevParam();
        Serial.printf("LoRaWan Class%X  start! \r\n", CLASS + 10);
        LoRaWAN.Init(CLASS, REGION);
        DeviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
        LoRaWAN.Join();
        break;
      }
    case DEVICE_STATE_SEND:
      {
        PrepareTxFrame( AppPort );
        LoRaWAN.Send();
        DeviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission
        TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( 0, APP_TX_DUTYCYCLE_RND );
        LoRaWAN.Cycle(TxDutyCycleTime);
        DeviceState = DEVICE_STATE_SLEEP;
        break;
      }
    case DEVICE_STATE_SLEEP:
      {
        LoRaWAN.Sleep();
        break;
      }
    default:
      {
        DeviceState = DEVICE_STATE_INIT;
        break;
      }
  }
}
