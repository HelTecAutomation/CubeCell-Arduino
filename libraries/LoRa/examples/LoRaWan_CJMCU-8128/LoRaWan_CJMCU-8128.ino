#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>

#include <BMP280.h>
#include "HDC1080.h"
//#include <BH1750.h>
#include <CCS811.h>
#include <hal/soc/flash.h>

#ifndef ACTIVE_REGION
#define ACTIVE_REGION LORAMAC_REGION_EU868
#endif

#ifndef CLASS_MODE
#define CLASS_MODE CLASS_A
#endif

DeviceClass_t  CLASS = CLASS_MODE;

/*
   set LoraWan_RGB to 1,the RGB active in loraWan
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
#define  AT_SUPPORT  0

/*!
   When set to true the application uses the Over-the-Air activation procedure
   When set to false the application uses the Personalization activation procedure
*/
bool OVER_THE_AIR_ACTIVATION = true;

/* LoRaWAN Adaptive Data Rate */
bool LORAWAN_ADR_ON = true;

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

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = 900000;

float Temperature, Humidity, Pressure, lux, co2, tvoc;
uint16_t baseline;
int count;
int maxtry = 50;

HDC1080 hdc1080;
//BH1750 lightMeter;
//CCS811 ccs(CCS811_ADDR);
CCS811 ccs;
BMP280 bmp;
//ClosedCube_BME680 bme680;
//BME280 bme280;

/*  get the BatteryVoltage in mV. */
static uint16_t GetBatteryVoltage(void)
{
  pinMode(ADC_CTL, OUTPUT);
  digitalWrite(ADC_CTL, LOW);
  uint16_t volt = analogRead(ADC) * 2;
  digitalWrite(ADC_CTL, HIGH);
  return volt;
}

/*!
   \brief   Prepares the payload of the frame
*/

static void PrepareTxFrame( uint8_t port )
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

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

//  uint8_t basetemp;
//  if (FLASH_read_at(0x0, baseline, 2)) {
//    baseline = basetemp;
//    Serial.print("Read BaseLine: ");
//    Serial.println(baseline);
  baseline = 13440;
  ccs.setBaseline(baseline);
//  }
  delay(5000);

  while (!ccs.available());
  ccs.readData();
  co2 = ccs.geteCO2();
  tvoc = ccs.getTVOC();

  baseline = ccs.getBaseline();
//  FLASH_Update(0x0, baseline, 2);

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
  unsigned char *puc;

  puc = (unsigned char *)(&Temperature);
  AppDataSize = 26;//AppDataSize max value is 64
  AppData[0] = puc[0];
  AppData[1] = puc[1];
  AppData[2] = puc[2];
  AppData[3] = puc[3];

  puc = (unsigned char *)(&Humidity);
  AppData[4] = puc[0];
  AppData[5] = puc[1];
  AppData[6] = puc[2];
  AppData[7] = puc[3];

  puc = (unsigned char *)(&lux);
  AppData[8] = puc[0];
  AppData[9] = puc[1];
  AppData[10] = puc[2];
  AppData[11] = puc[3];

  puc = (unsigned char *)(&Pressure);
  AppData[12] = puc[0];
  AppData[13] = puc[1];
  AppData[14] = puc[2];
  AppData[15] = puc[3];

  puc = (unsigned char *)(&co2);
  AppData[16] = puc[0];
  AppData[17] = puc[1];
  AppData[18] = puc[2];
  AppData[19] = puc[3];

  puc = (unsigned char *)(&tvoc);
  AppData[20] = puc[0];
  AppData[21] = puc[1];
  AppData[22] = puc[2];
  AppData[23] = puc[3];

  AppData[24] = (uint8_t)(BatteryVoltage >> 8);
  AppData[25] = (uint8_t)BatteryVoltage;

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


void setup() {
  BoardInitMcu( );
  Serial.begin(115200);
  DeviceState = DEVICE_STATE_INIT;
}

void loop()
{
  switch ( DeviceState )
  {
    case DEVICE_STATE_INIT:
      {
        Serial.printf("LoRaWan Class%X test start! \r\n", CLASS + 10);
#if(AT_SUPPORT)
        Enable_AT();
        getDevParam();
#endif
        printDevParam();
        LoRaWAN.Init(CLASS, ACTIVE_REGION);
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
