#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>

#include <BMP280.h>
#include "HDC1080.h"
//#include <BH1750.h>
#include <CCS811.h>
//#include "ClosedCube_BME680.h"

#ifndef ACTIVE_REGION
#define ACTIVE_REGION LORAMAC_REGION_EU868
#endif

#ifndef CLASS_MODE
#define CLASS_MODE CLASS_A
#endif


//HI
//AT+DevEui=002307E701EEDF8E
//AT+AppKey=21DD2980450EA8D5297AB29A90291262
//AT+AppEui=70B3D57ED0023C84
//AT+DutyCycle=900000 //15 Minuten
//AT+DutyCycle=60000  // 1 Minute
//AT+RESET=1


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
#define  AT_SUPPORT  1

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
uint32_t APP_TX_DUTYCYCLE = 600000;

float Temperature, Humidity, Pressure, lux, co2, tvoc;
int count;
int maxtry = 50;

HDC1080 hdc1080;
//BH1750 lightMeter;
CCS811 ccs;
BMP280 bmp;
//ClosedCube_BME680 bme680;

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

//ClosedCube_BME680_Status readAndPrintStatus() {
//  ClosedCube_BME680_Status status = bme680.readStatus();
//  Serial.print("status: (");
//  Serial.print(status.newDataFlag);
//  Serial.print(",");
//  Serial.print(status.measuringStatusFlag);
//  Serial.print(",");
//  Serial.print(status.gasMeasuringStatusFlag);
//  Serial.print(",");
//  Serial.print(status.gasMeasurementIndex);
//  Serial.println(") (newDataFlag,StatusFlag,GasFlag,GasIndex)");
//  return status;
//}


static void PrepareTxFrame( uint8_t port )
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

  count = 0;
  ccs.begin();
  bmp.begin();
  delay(100);
  bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                  BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  BMP280::FILTER_X16,      /* Filtering. */
                  BMP280::STANDBY_MS_500); /* Standby time. */
  hdc1080.begin(0x40);
   
  delay(5000);
  while (!ccs.available());
  ccs.readData();
  co2 = ccs.geteCO2();
  tvoc = ccs.getTVOC();
  while (co2 > 65500.0 && count < maxtry) {
    ccs.readData();
    co2 = ccs.geteCO2();
    tvoc = ccs.getTVOC();
    count++;
    delay(500);
  }
  if (co2 > 65500.0) {
    co2 = 0.0;
    tvoc = 0.0;
  }

  count = 0;
  float temp = bmp.readTemperature();
  Pressure = bmp.readPressure() / 100.0;
  while (Pressure > 1190.0 && count < maxtry) {
     Pressure = bmp.readPressure() / 100.0;
     count++;
     delay(500);
  }
  if (Pressure > 1190.0) {
    Pressure = 0;
  }

  count = 0;
  Temperature = (float)(hdc1080.readTemperature());
  Humidity = (float)(hdc1080.readHumidity());
  while (Temperature > 120.0 && count < maxtry) {
    Temperature = (float)(hdc1080.readTemperature());
    Humidity = (float)(hdc1080.readHumidity());
    count++;
    delay(500);
  }
  if (Temperature > 120.0) {
    Temperature = 0.0;
    Humidity = 0.0;
  }
  hdc1080.end();

  //  if (!lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2)) {
  //    Serial.print("Failed to start BH2750!");
  //  }
  //  float lux = lightMeter.readLightLevel();
  //  lightMeter.end();
  lux = 0.0;
  
  //  bme680.init(0x77); // I2C address: 0x76 or 0x77
  //  bme680.reset();
  //  bme680.setOversampling(BME680_OVERSAMPLING_X1, BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X16);
  //  bme680.setIIRFilter(BME680_FILTER_3);
  //  bme680.setGasOn(300, 100); // 300 degree Celsius and 100 milliseconds
  //  delay(5000);
  //  ClosedCube_BME680_Status status = readAndPrintStatus();
  //  if (status.newDataFlag) {
  //    Temperature = bme680.readTemperature();
  //    Pressure = bme680.readPressure();
  //    Humidity = bme680.readHumidity();
  //    tvoc = bme680.readGasResistance();
  //    co2 = 0.0;
  //  }

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
  Serial.print(" ppb, BatteryVoltage:");
  Serial.println(BatteryVoltage);
  //    Serial.print("PayLoad=");
  //    for (int xx=0; xx < AppDataSize; xx++) {
  //      Serial.print(AppData[xx]);
  //    }
  //    Serial.println("");
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
