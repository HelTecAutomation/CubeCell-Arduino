/*
  LoRaWan_MultiSensor
  programmed by WideAreaSensorNetwork
  v2.0.4 by WASN.eu
*/

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "Wire.h"

/*
   Define your Settings below
*/

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
#define One_Wire 0

#define ModularNode 0  // TCS9548A I2C 8 port Switch

const char myDevEui[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const char myAppEui[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const char myAppKey[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = 900000;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool IsTxConfirmed = false;
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


/*
  NO USER CHANGES NEEDED UNDER THIS LINE
*/

String wasnver = "2.0.4";

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
#include "OneWire.h"

extern uint8_t DevEui[];
extern uint8_t AppEui[];
extern uint8_t AppKey[];

bool BME_680_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 1
bool BME_280_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 2
bool CCS_811_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 3
bool HDC_1080_e[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // 4
bool BMP_180_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 5
bool BH_1750_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 6
bool BMP_280_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 7
bool SHT_2X_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};   // 8
bool ADS_1015_e[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // 9
bool MPU_9250_e[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // 10
bool LR_VL53L1X_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 11
bool HMC_5883L_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // 12
bool One_Wire_e = false;                       // 100-103
uint8_t sensortype = 0;

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

#ifndef LORAWAN_NETMODE
#define LORAWAN_NETMODE 0
#endif

#ifndef LORAWAN_ADR
#define LORAWAN_ADR 1
#endif

#ifndef LORAWAN_Net_Reserve
#define LORAWAN_Net_Reserve 1
#endif

/*LoraWan Class*/
DeviceClass_t CLASS = LORAWAN_CLASS;
/*OTAA or ABP*/
bool OVER_THE_AIR_ACTIVATION = LORAWAN_NETMODE;
/*ADR enable*/
bool LORAWAN_ADR_ON = LORAWAN_ADR;
/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool KeepNet = LORAWAN_Net_Reserve;
/*LoraWan REGION*/
LoRaMacRegion_t REGION = ACTIVE_REGION;

/* Application port */
uint8_t AppPort = 2;

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

OneWire ds(GPIO1); // on pin GPIO1 PIN6 (a 4.7K resistor is necessary)

/*!
   \brief   Prepares the payload of the frame
*/

static void PrepareTxFrame(uint8_t port)
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);
  pinMode(GPIO0, OUTPUT);
  digitalWrite(GPIO0, LOW);

  AppDataSize = 0;
  int pnr = 0;

#if (ModularNode == 1)
  for (pnr = 0; pnr < 8; pnr++)
  {
    Serial.print("Sensor Port ");
    Serial.println(pnr);
#endif

    /*
        BME680
    */

    if (BME_680_e[pnr])
    {
      sensortype = 1;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      bme680.begin(I2C_STANDARD_MODE);
      delay(1000);
      bme680.setOversampling(TemperatureSensor, Oversample16);
      bme680.setOversampling(HumiditySensor, Oversample16);
      bme680.setOversampling(PressureSensor, Oversample16);
      bme680.setIIRFilter(IIR4);
      bme680.setGas(320, 150); // 320C for 150 milliseconds

      static int32_t temperature, humidity, pressure, gas;
      bme680.getSensorData(temperature, humidity, pressure, gas);
      delay(500);
      bme680.getSensorData(temperature, humidity, pressure, gas);

      Temperature = temperature / 100.0;
      Humidity = humidity / 1000.0;
      Pressure = pressure / 100.0;
      co2 = gas / 100.0;
      tvoc = CalculateIAQ();

      Wire.end();
      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 1;

      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      AppData[AppDataSize++] = (uint8_t)((int)(Humidity * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)(Humidity * 10.0));

      AppData[AppDataSize++] = (uint8_t)((int)(Pressure * 10.0) >> 8);
      ;
      AppData[AppDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      AppData[AppDataSize++] = (uint8_t)((int)co2 >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)co2);

      AppData[AppDataSize++] = (uint8_t)((int)tvoc >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)tvoc);

      Serial.print("  BME680: T = ");
      Serial.print(Temperature);
      Serial.print("C, RH = ");
      Serial.print(Humidity);
      Serial.print(" %, Pressure = ");
      Serial.print(Pressure);
      Serial.print(" hPA, GAS = ");
      Serial.print(co2);
      Serial.print("mOhm, IAQ = ");
      Serial.println(tvoc);
    }

    /*
      BME280
    */
    if (BME_280_e[pnr])
    {
      sensortype = 2;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      if (!bme280.init())
      {
        Serial.println("  BME280 error!");
      }
      delay(1000);
      Temperature = bme280.getTemperature();
      Pressure = bme280.getPressure() / 100.0;
      Humidity = bme280.getHumidity();

      Wire.end();

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 2;

      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      AppData[AppDataSize++] = (uint8_t)((int)(Humidity * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)(Humidity * 10.0));

      AppData[AppDataSize++] = (uint8_t)((int)(Pressure * 10.0) >> 8);
      ;
      AppData[AppDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      Serial.print("  BME280: T = ");
      Serial.print(Temperature);
      Serial.print("C, RH = ");
      Serial.print(Humidity);
      Serial.print(" %, Pressure = ");
      Serial.print(Pressure);
      Serial.println(" hPA");
    }

    /*
        HDC1080
    */

    if (HDC_1080_e[pnr])
    {
      sensortype = 4;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      hdc1080.begin(0x40);
      delay(500);
      Temperature = (float)(hdc1080.readTemperature());
      Humidity = (float)(hdc1080.readHumidity());
      Wire.end();
      while (Temperature > 120.0 && count < maxtry)
      {
        hdc1080.begin(0x40);
        delay(500);
        Temperature = (float)(hdc1080.readTemperature());
        Humidity = (float)(hdc1080.readHumidity());
        Wire.end();
        count++;
        delay(500);
      }
      if (Temperature > 120.0)
      {
        Temperature = 0.0;
        Humidity = 0.0;
        Serial.println("  HDC ERROR");
      }
      hdc1080.end();

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 4;

      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      AppData[AppDataSize++] = ((int)(Humidity * 10.0)) >> 8;
      AppData[AppDataSize++] = (int)(Humidity * 10.0);

      Serial.print("  HDC1080: T = ");
      Serial.print(Temperature);
      Serial.print("C, RH = ");
      Serial.print(Humidity);
      Serial.println(" % ");
    }

    /*
      CCS811
    */

    if (CCS_811_e[pnr])
    {
      sensortype = 3;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      ccs.begin();
      delay(1000);

      FLASH_read_at(addr, baselineflash, sizeof(baselineflash));
      baselinetemp = (baselineflash[0] << 8) | baselineflash[1];
      if (baselinetemp > 0)
      {
        baseline = baselinetemp;
        Serial.print("  CCS811 Read BaseLine: ");
        Serial.println(baseline);
        ccs.setBaseline(baseline);
      }
      delay(5000);

      while (!ccs.available())
        ;
      ccs.readData();
      Temperature = ccs.calculateTemperature();
      co2 = ccs.geteCO2();
      tvoc = ccs.getTVOC();

      baseline = ccs.getBaseline();
      baselineflash[0] = (uint8_t)(baseline >> 8);
      baselineflash[1] = (uint8_t)baseline;
      FLASH_update(addr, baselineflash, sizeof(baselineflash));
      Serial.print("  CCS811 Write BaseLine: ");
      Serial.println(baseline);
      Wire.end();
      while (co2 > 65500.0 && count < maxtry)
      {
        ccs.begin();
        delay(1000);
        while (!ccs.available())
          ;
        ccs.readData();
        co2 = ccs.geteCO2();
        tvoc = ccs.getTVOC();
        Wire.end();
        count++;
      }
      if (co2 > 65500.0)
      {
        co2 = 0.0;
        tvoc = 0.0;
        Serial.println("  CCS ERROR");
      }

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 3;

      AppData[AppDataSize++] = (uint8_t)((int)co2 >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)co2);

      AppData[AppDataSize++] = (uint8_t)((int)tvoc >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)tvoc);

      Serial.print("  CCS811: CO2 = ");
      Serial.print(co2);
      Serial.print(" ppm, TVOC = ");
      Serial.print(tvoc);
      Serial.print(" ppb, Baseline: ");
      Serial.println(baseline);
    }

    /*
      BMP180
    */

    if (BMP_180_e[pnr])
    {
      sensortype = 5;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      bmp180.begin();
      delay(500);
      Temperature = (float)(bmp180.readTemperature());
      Pressure = (float)(bmp180.readPressure()) / 100.0;
      Wire.end();
      while (Temperature > 120.0 && count < maxtry)
      {
        bmp180.begin();
        delay(500);
        Temperature = (float)(bmp180.readTemperature());
        Pressure = (float)(bmp180.readPressure()) / 100.0;
        Wire.end();
        count++;
        delay(500);
      }
      if (Temperature > 120.0)
      {
        Temperature = 0.0;
        Humidity = 0.0;
        Serial.println("  BMP ERROR");
      }

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 5;

      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      AppData[AppDataSize++] = (uint8_t)((int)(Pressure * 10.0) >> 8);
      ;
      AppData[AppDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      Serial.print("  BMP180: T = ");
      Serial.print(Temperature);
      Serial.print("C, P = ");
      Serial.print(Pressure);
      Serial.println("hPa");
    }

    /*
      BH1750
    */

    if (BH_1750_e[pnr])
    {
      sensortype = 6;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2);
      delay(100);
      lux = lightMeter.readLightLevel();
      delay(100);
      lux = lightMeter.readLightLevel();
      lightMeter.end();
      Wire.end();

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 6;

      AppData[AppDataSize++] = (uint8_t)((int)(lux * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)(lux * 10.0));

      Serial.print("  BH1750: Light = ");
      Serial.print(lux);
      Serial.println("lx");
    }

    /*
        BMP_280
    */
    if (BMP_280_e[pnr])
    {
      sensortype = 7;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      bmp280.begin();
      delay(500);
      bmp280.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                         BMP280::SAMPLING_X2,     /* Temp. oversampling */
                         BMP280::SAMPLING_X16,    /* Pressure oversampling */
                         BMP280::FILTER_X16,      /* Filtering. */
                         BMP280::STANDBY_MS_500); /* Standby time. */
      Temperature = bmp280.readTemperature();
      Pressure = (float)bmp280.readPressure() / 100.0;
      Wire.end();
      while (Pressure > 1190.0 && count < maxtry)
      {
        bmp280.begin();
        delay(500);
        bmp280.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                           BMP280::SAMPLING_X2,     /* Temp. oversampling */
                           BMP280::SAMPLING_X16,    /* Pressure oversampling */
                           BMP280::FILTER_X16,      /* Filtering. */
                           BMP280::STANDBY_MS_500); /* Standby time. */
        Pressure = (float)bmp280.readPressure() / 100.0;
        Wire.end();
        count++;
        delay(500);
      }
      if (Pressure > 1190.0)
      {
        Pressure = 0;
        Serial.println("  BMP ERROR");
      }

      AppData[AppDataSize++] = pnr;

      AppData[AppDataSize++] = 7;

      AppData[AppDataSize++] = (uint8_t)(((int)((Temperature + 100.0) * 10.0)) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      AppData[AppDataSize++] = (uint8_t)(((int)(Pressure * 10.0)) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      Serial.print("  BMP280: T=");
      Serial.print(Temperature);
      Serial.print("C, Pressure=");
      Serial.print(Pressure);
      Serial.println(" hPA");
    }

    /*
        SHT_2X
    */
    if (SHT_2X_e[pnr])
    {
#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      Wire.begin();
      delay(500);
      Temperature = SHT2x.GetTemperature();
      Humidity = SHT2x.GetHumidity();
      Wire.end();

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 8;

      AppData[AppDataSize++] = (uint8_t)(((int)((Temperature + 100.0) * 10.0)) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      AppData[AppDataSize++] = ((int)(Humidity * 10.0)) >> 8;
      AppData[AppDataSize++] = (int)(Humidity * 10.0);

      Serial.print("  SHT2X: T=");
      Serial.print(Temperature);
      Serial.print("C, Humidity=");
      Serial.print(Humidity);
      Serial.println(" %");
    }

    /*
        ADS_1015
    */
    if (ADS_1015_e[pnr])
    {
#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      ads1015.begin();
      int16_t adc0, adc1, adc2, adc3;

      adc0 = ads1015.readADC_SingleEnded(0);
      adc1 = ads1015.readADC_SingleEnded(1);
      adc2 = ads1015.readADC_SingleEnded(2);
      adc3 = ads1015.readADC_SingleEnded(3);

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 9;

      AppData[AppDataSize++] = ((int)adc0) >> 8;
      AppData[AppDataSize++] = (int)adc0;
      AppData[AppDataSize++] = ((int)adc1) >> 8;
      AppData[AppDataSize++] = (int)adc1;
      AppData[AppDataSize++] = ((int)adc2) >> 8;
      AppData[AppDataSize++] = (int)adc2;
      AppData[AppDataSize++] = ((int)adc3) >> 8;
      AppData[AppDataSize++] = (int)adc3;

      Serial.print("  ADS1015: ADC0=");
      Serial.print(adc0);
      Serial.print(", ADC1=");
      Serial.print(adc1);
      Serial.print(", ADC2=");
      Serial.print(adc2);
      Serial.print(", ADC3=");
      Serial.print(adc3);
      Serial.println();

      Wire.end();
    }

    /*
      MPU9250
    */
    if (MPU_9250_e[pnr])
    {
      sensortype = 10;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      Wire.begin();
      delay(500);
      mpu9250.setWire(&Wire);
      delay(100);
      mpu9250.beginAccel();
      mpu9250.beginGyro();
      mpu9250.beginMag();
      delay(200);
      uint8_t sensorId;

      if (mpu9250.readId(&sensorId) == 0)
      {
        Serial.println("  MPU9250 sensorId: " + String(sensorId));
      }
      else
      {
        Serial.println("  Cannot read sensorId");
      }

      float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
      if (mpu9250.accelUpdate() == 0)
      {
        aX = mpu9250.accelX();
        aY = mpu9250.accelY();
        aZ = mpu9250.accelZ();
        aSqrt = mpu9250.accelSqrt();
        Serial.print("  accelX: " + String(aX));
        Serial.print(", accelY: " + String(aY));
        Serial.print(", accelZ: " + String(aZ));
        Serial.println(", accelSqrt: " + String(aSqrt));
      }
      else
      {
        Serial.println("  Cannod read accel values");
      }

      if (mpu9250.gyroUpdate() == 0)
      {
        gX = mpu9250.gyroX();
        gY = mpu9250.gyroY();
        gZ = mpu9250.gyroZ();
        Serial.print("  gyroX: " + String(gX));
        Serial.print(", gyroY: " + String(gY));
        Serial.println(", gyroZ: " + String(gZ));
      }
      else
      {
        Serial.println("  Cannot read gyro values");
      }

      if (mpu9250.magUpdate() == 0)
      {
        mX = mpu9250.magX();
        mY = mpu9250.magY();
        mZ = mpu9250.magZ();
        mDirection = mpu9250.magHorizDirection();
        Serial.print("  magX: " + String(mX));
        Serial.print(", maxY: " + String(mY));
        Serial.print(", magZ: " + String(mZ));
        Serial.println(", horizontal direction: " + String(mDirection));
      }
      else
      {
        Serial.println("  Cannot read mag values");
      }

      Wire.end();

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 10;

      unsigned char *puc;
      puc = (unsigned char *)(&aX);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&aY);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&aZ);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&gX);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&gY);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&gZ);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&mX);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&mY);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&mZ);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&aSqrt);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&mDirection);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
    }
    /*
        VL53L1X
    */

    if (LR_VL53L1X_e[pnr])
    {
      sensortype = 11;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      Serial.print("  VL53L1X: ");

      if (!LRSVL53L1X.begin() == 0) { //Begin returns 0 on a good init
        Serial.println("Sensor failure!");
      }
      LRSVL53L1X.setDistanceModeShort();
      //distanceSensor.setDistanceModeLong();
      LRSVL53L1X.startRanging(); //Write configuration block of 135 bytes to setup a measurement
      int distance = LRSVL53L1X.getDistance(); //Get the result of the measurement from the sensor
      LRSVL53L1X.stopRanging();
      Serial.print("range: ");
      Serial.print(distance);
      Serial.println("mm");

      Wire.end();

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 11;

      unsigned char *puc;
      puc = (unsigned char *)(&distance);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
    }
    /*
        HMC5883L
    */

    if (HMC_5883L_e[pnr])
    {
      sensortype = 12;

#if (ModularNode == 1)
      Wire.begin();
      tcaselect(pnr);
      delay(100);
#endif

      Serial.print("  HMC5883L: ");

      #define hmx5883address 0x0D // 0x1E 0x0D
      Wire.beginTransmission(hmx5883address); //open communication with HMC5883
      Wire.write(0x02); //select mode register
      Wire.write(0x00); //continuous measurement mode
      Wire.endTransmission();

      int magx,magy,magz; //triple axis data
      Wire.beginTransmission(hmx5883address);
      Wire.write(0x03); //select register 3, X MSB register
      Wire.endTransmission();
      //Read data from each axis, 2 registers per axis
      Wire.requestFrom(hmx5883address, 6);
      if(6<=Wire.available()){
        magx = Wire.read()<<8; //X msb
        magx |= Wire.read(); //X lsb
        magz = Wire.read()<<8; //Z msb
        magz |= Wire.read(); //Z lsb
        magy = Wire.read()<<8; //Y msb
        magy |= Wire.read(); //Y lsb
      }

      Serial.print("X: ");
      Serial.print(magx);
      Serial.print(", Y: ");
      Serial.print(magz);
      Serial.print(", Z: ");
      Serial.println(magz);

      Wire.end();

      AppData[AppDataSize++] = pnr;
      AppData[AppDataSize++] = 12;

      unsigned char *puc;
      puc = (unsigned char *)(&magx);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&magy);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
      puc = (unsigned char *)(&magz);
      AppData[AppDataSize++] = puc[0];
      AppData[AppDataSize++] = puc[1];
      AppData[AppDataSize++] = puc[2];
      AppData[AppDataSize++] = puc[3];
    }
#if (ModularNode == 1)
  }
#endif
  /*
    One_Wire
  */
  if (One_Wire_e)
  {
    Serial.println("Scan for OneWire Sensors ...");
    byte present = 0;
    byte type_s;
    byte data[12];
    byte owaddress[8];
    int owsensorcount = 0;

    ds.reset_search();
    delay(250);

    if (ds.search(owaddress))
    {
      Serial.print("  OneWire Sensor found with data: ");
      for (byte i = 0; i < 8; i++)
      {
        Serial.write(' ');
        Serial.print(owaddress[i], HEX);
      }

      if (OneWire::crc8(owaddress, 7) != owaddress[7])
      {
        Serial.print("  CRC is not valid!");
        return;
      }
      Serial.println();

      switch (owaddress[0])
      {
      case 0x10:
        Serial.print("    Chip = DS18S20 (0x10)"); // or old DS1820
        type_s = 1;
        break;
      case 0x28:
        Serial.print("    Chip = DS18B20 (0x28)");
        type_s = 0;
        break;
      case 0x22:
        Serial.print("    Chip = DS1822 (0x22)");
        type_s = 0;
        break;
      default:
        Serial.print("    Device is not known.");
        return;
      }

      ds.reset();
      ds.select(owaddress);
      ds.write(0x44, 1); //start conv with power

      delay(1000);

      present = ds.reset();
      ds.select(owaddress);
      ds.write(0xBE);

      for (byte i = 0; i < 9; i++)
      {
        data[i] = ds.read();
      }

      int16_t raw = (data[1] << 8) | data[0];
      if (type_s)
      {
        raw = raw << 3;
        if (data[7] == 0x10)
        {
          raw = (raw & 0xFFF0) + 12 - data[6];
        }
      }
      else
      {
        byte cfg = (data[4] & 0x60);
        if (cfg == 0x00)
          raw = raw & ~7;
        else if (cfg == 0x20)
          raw = raw & ~3;
        else if (cfg == 0x40)
          raw = raw & ~1;
      }
      Temperature = (float)raw / 16.0;

      AppData[AppDataSize++] = 100 + owsensorcount++;
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      AppData[AppDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      Serial.print(": T=");
      Serial.print(Temperature);
      Serial.println("C");
    }
    else
    {
      Serial.println("  No OneWire Sensors found.");
    }
    ds.reset_search();
  }

  digitalWrite(Vext, HIGH);

  uint16_t BatteryVoltage = GetBatteryVoltage();
  AppData[AppDataSize++] = (uint8_t)(BatteryVoltage >> 8);
  AppData[AppDataSize++] = (uint8_t)BatteryVoltage;
  Serial.print("BatteryVoltage: ");
  Serial.print(BatteryVoltage);
  Serial.println();
}

void setup()
{
  memcpy(DevEui, myDevEui, sizeof(myDevEui));
  memcpy(AppEui, myAppEui, sizeof(myAppEui));
  memcpy(AppKey, myAppKey, sizeof(myAppKey));

#if (AUTO_SCAN == 1)
  for (int xx = 0; xx < 8; xx++)
  {
    BME_680_e[xx] = 0;
    BME_280_e[xx] = 0;
    CCS_811_e[xx] = 0;
    BMP_180_e[xx] = 0;
    HDC_1080_e[xx] = 0;
    BH_1750_e[xx] = 0;
    SHT_2X_e[xx] = 0;
    ADS_1015_e[xx] = 0;
    MPU_9250_e[xx] = 0;
    LR_VL53L1X_e[xx] = 0;
    HMC_5883L_e[xx] = 0;
  }
  sensortype = 88;
#endif

#if (BME_680 == 1)
  BME_680_e[0] = 1;
#endif

#if (BME_280 == 1)
  BME_280_e[0] = 1;
#endif

#if (CCS_811 == 1)
  CCS_811_e[0] = 1;
#endif

#if (HDC_1080 == 1)
  HDC_1080_e[0] = 1;
#endif

#if (BMP_180 == 1)
  BMP_180_e[0] = 1;
#endif

#if (BH_1750 == 1)
  BH_1750_e[0] = 1;
#endif

#if (BMP_280 == 1)
  BMP_280_e[0] = 1;
#endif

#if (SHT_2x == 1)
  SHT_2X_e[0] = 1;
#endif

#if (ADS_1015 == 1)
  ADS_1015_e[0] = 1;
#endif

#if (MPU_9250 == 1)
  MPU_9250_e[0] = 1;
#endif

#if (VL53L1X == 1)
  LR_VL53L1X_e[0] = 1;
#endif

#if (HMC_5883L == 1)
  HMC_5883L_e[0] = 1;
#endif

#if (One_Wire == 1)
  One_Wire_e = true;
  sensortype = 100;
#endif

  BoardInitMcu();
  Serial.begin(115200);
  Serial.println("Copyright @ 2019 WASN.eu");
  Serial.print("FW-version: ");
  Serial.println(wasnver);
  Serial.println("");
#if (AT_SUPPORT == 1)
  Enable_AT();
#endif
  DeviceState = DEVICE_STATE_INIT;
  LoRaWAN.Ifskipjoin();

pinMode(Vext, OUTPUT);
digitalWrite(Vext, LOW); //set vext to high
delay(500);
pinMode(GPIO0, OUTPUT);
digitalWrite(GPIO0, LOW);
Wire.begin();

#if (AUTO_SCAN == 1)
  byte error, address;
  int nDevices;

  Serial.println("Sensor Port Scanning...");

  nDevices = 0;
  int pnr = 0;
  int unknowndev = 0;

  #if (ModularNode == 1)
  for (pnr = 0; pnr < 8; pnr++)
  {
    tcaselect(pnr);
    Serial.print(" Port ");
    Serial.println(pnr);
    nDevices = 0;
    unknowndev = 0;
  #endif

    for (address = 1; address < 127; address++)
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {
        /* if (address != 112) {
          Serial.print("    Sensor found at address 0x");
          if (address < 16) {
            Serial.print("0");
          }
          Serial.print(address, HEX);
          Serial.println("  !");
        } */

        switch (address)
        {
          case 13: //0x0D -- HMC5883L
          {
              Serial.println("      found HMC5883L");
              HMC_5883L_e[pnr] = true;
              break;
          }
          case 35: //0x23 -- BH1750
          {
            Serial.println("      found BH1750");
            BH_1750_e[pnr] = true;
            break;
          }
          case 64: //0x40 -- HDC1080/SHT2X
          {
            hdc1080.begin(0x40);
            delay(200);
            if (hdc1080.readTemperature() > 120)
            {
              Serial.println("      found SHT2X");
              SHT_2X_e[pnr] = true;
            }
            else
            {
              Serial.println("      Found HDC1080");
              HDC_1080_e[pnr] = true;
            }
            //hdc1080.end();
            break;
          }
          case 72: //0x48 -- ADS1015
          {
            Serial.println("      Found ADS1015");
            ADS_1015_e[pnr] = true;
            break;
          }
          case 82: //0x52 -- VL53L1X
          {
            Serial.println("      Found VL53L1X");
            LR_VL53L1X_e[pnr] = true;
            break;
          }
          case 90: //0x5A --CCS811
          {
            Serial.println("      Found CCS811");
            CCS_811_e[pnr] = true;
            break;
          }
          case 104: //0x68 -- MPU9250 9-axis sensor
          {
            Serial.println("      Found MPU9250");
            MPU_9250_e[pnr] = true;
            break;
          }
          case 118: //0x76 -- BMP280/BME280
          {
            if (!bme280.init())
            {
              Serial.println("      Found BMP280");
              BMP_280_e[pnr] = true;
            }
            else
            {
              Serial.println("      Found BME280");
              BME_280_e[pnr] = true;
            }
            break;
          }
          case 119: //0x77 -- BME680/BMP180
          {
            if (!bmp180.begin())
            {
              Serial.println("      Found BME680");
              BME_680_e[pnr] = true;
            }
            else
            {
              Serial.println("      Found BMP180");
              BMP_180_e[pnr] = true;
            }
            break;
          }
          default:
          {
            if (address != 112 && address > 2) {
              Serial.print("      Unknown Sensor at address 0x");
              if (address < 16)
                Serial.print("0");
              Serial.println(address, HEX);
            }
            unknowndev++;
            if (unknowndev == address && address > 1) {
              address = 128;
              Serial.println("     Sensor Bus error");
            }
            break;
          }
        }
        nDevices++;
      }
      else if (error == 4)
      {
        Serial.print("    Unknown error at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
      }
    }
#if (ModularNode == 1)
    nDevices--;
#endif
    if (nDevices == 0)
    {
      Serial.println("  No Sensors found");
    }
#if (ModularNode == 1)
  }
#endif

  Wire.end();
#endif
}

void loop()
{
  switch (DeviceState)
  {
  case DEVICE_STATE_INIT:
  {
#if (AT_SUPPORT == 1)
    getDevParam();
#endif
    printDevParam();
    Serial.printf("LoRaWan Class % X  start! \r\n", CLASS + 10);
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
    PrepareTxFrame(AppPort);
    LoRaWAN.Send();
    DeviceState = DEVICE_STATE_CYCLE;
    break;
  }
  case DEVICE_STATE_CYCLE:
  {
    // Schedule next packet transmission
    TxDutyCycleTime = APP_TX_DUTYCYCLE + randr(0, APP_TX_DUTYCYCLE_RND);
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

float CalculateIAQ()
{
  float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
  float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

  float hum_score, gas_score;
  float gas_reference = co2;
  float hum_reference = 40;
  int getgasreference_count = 0;

  //Calculate humidity contribution to IAQ index
  if (Humidity >= 38 && Humidity <= 42)
    hum_score = 0.25 * 100; // Humidity +/-5% around optimum
  else
  { //sub-optimal
    if (Humidity < 38)
      hum_score = 0.25 / hum_reference * Humidity * 100;
    else
    {
      hum_score = ((-0.25 / (100 - hum_reference) * Humidity) + 0.416666) * 100;
    }
  }

  //Calculate gas contribution to IAQ index
  float gas_lower_limit = 5000;  // Bad air quality limit
  float gas_upper_limit = 50000; // Good air quality limit
  if (gas_reference > gas_upper_limit)
    gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit)
    gas_reference = gas_lower_limit;
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100;

  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = hum_score + gas_score;

  return air_quality_score;
}

#define TCAADDR 0x70

void tcaselect(uint8_t i)
{
  if (i > 7)
    return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

bool AT_user_check(char *cmd, char *content)
{
  if (strcmp(cmd, "VER") == 0)
  {
    if (content[0] == '?')
    {
      Serial.print("+VER=");
      Serial.println(wasnver);
    }
    return true;
  }
  return false;
}

//downlink data handle function
void DownLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    if (mcpsIndication->Buffer[i] == 220) { // DC for APP_TX_DUTYCYCLE; 0D BB A0  for 900000 (15min); 04 93 E0 for 300000 (5min)
      APP_TX_DUTYCYCLE = mcpsIndication->Buffer[i++]<<32|mcpsIndication->Buffer[i++]<<16|mcpsIndication->Buffer[i++]<<8|mcpsIndication->Buffer[i++];
      Serial.print("  new DutyCycle received: ");
      Serial.print(APP_TX_DUTYCYCLE);
      Serial.println("ms");
      SaveDr();
    }
  }
}