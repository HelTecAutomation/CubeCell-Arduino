/*
  LoRaWan_ModularNode
  programmed by WideAreaSensorNetwork
  copyright by WASN.eu
*/

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "Wire.h"


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
#if(LORAWAN_DEVEUI_AUTO)
      LoRaWAN.generateDeveuiByChipID();
#endif
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