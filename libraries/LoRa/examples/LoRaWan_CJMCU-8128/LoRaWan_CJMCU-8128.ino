#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>

#define MJMCU_8128 1
#define BME_680    0
#define BME_280    0

#if(MJMCU_8128 == 1)
//#include "BH1750.h"
#include <BMP280.h>
#include "HDC1080.h"
#include <CCS811.h>
#include <hal/soc/flash.h>
#endif

#if(BME_680 ==1)
//#include "BH1750.h"
#include "BME680.h"
#endif

#if(BME_280 ==1)
//#include "BH1750.h"
#include "BME280.h"
#endif

//HI
//AT+DevEui=002307E701EEDF8E
//AT+AppKey=21DD2980450EA8D5297AB29A90291262
//AT+AppEui=70B3D57ED0023C84
//AT+DutyCycle=900000 //15 Minuten
//AT+DutyCycle=60000  // 1 Minute
//AT+RESET=1

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

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = 900000;

float Temperature, Humidity, Pressure, lux, co2, tvoc;
uint16_t baseline;
int count;
int maxtry = 50;

#if(MJMCU_8128 == 1)
HDC1080 hdc1080;
CCS811 ccs;
BMP280 bmp;
//BH1750 lightMeter;
#endif

#if(BME_680 ==1)
BME680 bme680;
//BH1750 lightMeter;
#endif

#if(BME_280 ==1)
BME280 bme280;
//BH1750 lightMeter;
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
 *  MJMCU-8128
 */
#if(MJMCU_8128 == 1)
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
#endif

/*
 * BME680
 */

#if(BME_680 == 1)
  //  if (!lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2)) {
  //    Serial.print("Failed to start BH2750!");
  //  }
  //  float lux = lightMeter.readLightLevel();
  //  lightMeter.end();
  lux = 0.0;
  bme680.init(0x77); // I2C address: 0x76 or 0x77
  bme680.reset();

  Serial.print("Chip ID=0x");
  Serial.println(bme680.getChipID(), HEX);

  // oversampling: humidity = x1, temperature = x2, pressure = x16
  bme680.setOversampling(BME680_OVERSAMPLING_X1, BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X16);
  bme680.setIIRFilter(BME680_FILTER_3);
  bme680.setGasOn(300, 100); // 300 degree Celsius and 100 milliseconds

  bme680.setForcedMode();
  BME680_Status status = readAndPrintStatus();
  if (status.newDataFlag) {
    Serial.print("result: ");
    Temperature = bme680.readTemperature();
    Pressure = bme680.readPressure();
    humidity = bme680.readHumidity();
    TVOC = bme680.readGasResistance();

    Wire.end();
    digitalWrite(Vext, HIGH);
    uint16_t BatteryVoltage = GetBatteryVoltage();
    unsigned char *puc;

    puc = (unsigned char *)(&Temperature);
    AppDataSize = 22;//AppDataSize max value is 64
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
    Serial.print(" hPA, GAS=");
    Serial.print(tvoc);
    Serial.print(", BatteryVoltage:");
    Serial.println(BatteryVoltage);

    bme680.setForcedMode();
  } else {
    delay(200); // sensor data not yet ready
  }
#endif

/*
 * BME280
 */
#if(BME_280 == 1)
  if (!bme280.init()) {
    Serial.println("Device error!");
  }
  Temperature = bme280.getTemperature();
  Pressure = bme280.getPressure();
  Humidty = bme280.getHumidity()

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
  AppDataSize = 18;//AppDataSize max value is 64
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
  Serial.print(" hPA, BatteryVoltage:");
  Serial.println(BatteryVoltage);
#endif
}

#if(BME_680 == 1)
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
  BoardInitMcu( );
  Serial.begin(115200);
#if(AT_SUPPORT == 1)
  Enable_AT();
#endif
  DeviceState = DEVICE_STATE_INIT;
  LoRaWAN.Ifskipjoin();
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
