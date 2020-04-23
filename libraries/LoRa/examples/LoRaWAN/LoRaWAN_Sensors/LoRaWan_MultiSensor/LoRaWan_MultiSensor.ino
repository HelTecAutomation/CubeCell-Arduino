/*
  LoRaWan_ModularNode
  programmed by WideAreaSensorNetwork
  copyright by WASN.eu
*/

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "Wire.h"

<<<<<<< HEAD
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
#define MLX_90614 0
#define One_Wire 1
  
#define TCS9548 0      // TCS9548A I2C 8 port Switch
#define minpnr 0       // TCS9548A first I2C Port
#define maxpnr 7       // TCS9548A last I2C Port

/* OTAA para*/
uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 90000;

/*
  NO USER CHANGES NEEDED UNDER THIS LINE
*/
#define ModularNode 0

String wasnver = "2.1.2";
String wasnflash = "Board"; //Board, Capsule, IndoorNode, ModularNode, TCA9548A

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
#include "MLX90614.h"

bool BME_680_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};     //   1
bool BME_280_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};     //   2
bool CCS_811_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};     //   3
bool HDC_1080_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};    //   4
bool BMP_180_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};     //   5
bool BH_1750_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};     //   6
bool BMP_280_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};     //   7
bool SHT_2X_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};      //   8
bool ADS_1015_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};    //   9
bool MPU_9250_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};    //  10
bool LR_VL53L1X_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //  11
bool HMC_5883L_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};   //  12
bool MLX_90614_e[8] = {0, 0, 0, 0, 0, 0, 0, 0};   //  13
bool One_Wire_e = false;                          // 100-103

/*
   set LoraWan_RGB to Active,the RGB active in loraWan
   RGB red means sending;
   RGB purple means joined done;
   RGB blue means RxWindow1;
   RGB yellow means RxWindow2;
   RGB green means received done;
*/

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;

/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;
=======
>>>>>>> 915279da3451e2521192c1239d29cd6d867cc0ed

#include "include/config.cpp"
#include "include/LoRaWAN.cpp"
#include "include/includesDeclarations_ce.cpp"
#include "include/functions_ce.cpp"
#include "include/checkUserAt_ce.cpp"
#include "include/DownLinkDataHandle.cpp"
#include "include/prepareTxFrame.cpp"


void setup()
{
#if (GPS_SER == 1)  
  ss.begin(9600);
#endif

accelWoke = false;
#if (TriggerInt == 1)
  PINMODE_INPUT_PULLDOWN(INT_PIN);
  attachInterrupt(INT_PIN, accelWakeup, RISING);
#endif

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
    //MLX_90614_e[xx] = 0;
    SHT_31_e[xx] = 0;
  }
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

/*
#if (MLX_90614 == 1)
  MLX_90614_e[0] = 1;
#endif
*/

#if (SHT_31 == 1)
  SHT_31_e[0] = 1;
#endif

#if (One_Wire == 1)
  One_Wire_e = true;
#endif

  boardInitMcu();
  Serial.begin(115200);
  Serial.println("Copyright @ 2020 WASN.eu");
  Serial.print("FW-version: ");
  Serial.println(wasnver);
  Serial.println("");
#if (AT_SUPPORT == 1)
  enableAt();
#endif

  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();

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

  #if (ModularNode == 1 || TCA9548 == 1)
  for (pnr = minpnr; pnr < (maxpnr+1); pnr++)
  {
    #if (ModularNode == 1)
      if (pnr == 4) {
        pnr = 7;
      }
    #endif
    tcaselect(pnr);
    if (ModularNode == 1 && (pnr == 0 || pnr == 7)) {
      Serial.print(" Modular Port ");
    } else {
      Serial.print(" Sensor Port ");
    }
    if (ModularNode == 1 && pnr == 7) {
      Serial.println("1");
    } else {
      Serial.println(pnr);
    }
    nDevices = 0;
    unknowndev = 0;
  #endif

    for (address = 1; address < 127; address++)
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (address == 35) {
        pinMode(GPIO0, OUTPUT);
        digitalWrite(GPIO0, HIGH);
      } else {
        pinMode(GPIO0, OUTPUT);
        digitalWrite(GPIO0, LOW);
      }
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
              Serial.println("      Found HMC5883L");
              HMC_5883L_e[pnr] = true;
              break;
          }
          case 35: //0x23 -- BH1750
          {
            Serial.println("      Found BH1750");
            BH_1750_e[pnr] = true;
            break;
          }
          case 64: //0x40 -- HDC1080/SHT2X
          {
            hdc1080.begin(0x40);
            delay(200);
            if (hdc1080.readTemperature() > 120)
            {
              Serial.println("      Found SHT2X");
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
          case 68: //0x44 -- SHT31
          {
              Serial.println("      Found SHT31");
              SHT_31_e[pnr] = true;
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
            Serial.println("      Found VL53L1X at 0x52");
            LR_VL53L1X_e[pnr] = true;
            break;
          }
          case 41: //0x29 -- VL53L1X
          {
            Serial.println("      Found VL53L1X at 0x29");
            LR_VL53L1X_e[pnr] = true;
            break;
          }
          case 90: //0x5A --CCS811 or MLX90614
          {
            if (ccs.begin()) {
              Serial.println("      Found CCS811");
              CCS_811_e[pnr] = true;
            } 
            /*
            if (mlx.begin()) {
              Serial.println("      Found MLX90614");
              MLX_90614_e[pnr] = true;
            }
            */
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
#if (ModularNode == 1 || TCA9548 == 1)
    nDevices--;
#endif
    if (nDevices == 0)
    {
      Serial.println("    No Sensors found");
    }
#if (ModularNode == 1 || TCA9548 == 1)
  }
#endif

  Wire.end();
#endif
}


/*
  LOOP
*/

void loop()
{
  switch (deviceState)
  {
    case DEVICE_STATE_INIT:
    {
#if (AT_SUPPORT == 1)
      getDevParam();
#endif
      printDevParam();
      LoRaWAN.init(loraWanClass,loraWanRegion);
      deviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
#if (ACTIVE_REGION == 1)
      setSubBand2();
#endif
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      prepareTxFrame(appPort);
      if (checkcopyprotect() == true) 
      {
        LoRaWAN.send();
      }
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
#if (TriggerInt == 1)
      if (accelWoke) {
        if (IsLoRaMacNetworkJoined) {
          prepareTxFrame(appPort);
          if (checkcopyprotect() == true) 
          {
            LoRaWAN.send();
          }
        //} else {
        //  LoRaWAN.join();
        //  prepareTxFrame(appPort);
        //  LoRaWAN.send();
        }
        accelWoke = false;
      }
#endif
      LoRaWAN.sleep();
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}