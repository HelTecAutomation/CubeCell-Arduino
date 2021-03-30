/* Heltec Automation I2C scanner example
 *
 * Function:
 * 1. ASR650x I2C device address scan, via I2C address to recognisse sensor;
 * 2. Multi sensor basic initial and test;
 * 
 * Now support with following sensors:
 * 0x23 -- BH1750 light sensor
 * 0x77 -- BMP180 Barometer sensor
 * 0x40 -- HDC1080 temperature and humidity sensor
 * 0x68 -- MPU9250 9-axis sensor
 * 0x5A -- CCS811 Air quality sensor
 *
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/ASR650x-Arduino
 * */

#include "Arduino.h"
#include "Wire.h"
#include <BH1750.h>
#include <BMP180.h>
#include "HDC1080.h"
#include <MPU9250.h>
#include <CCS811.h>

BH1750 lightMeter;
BMP085 bmp;
HDC1080 hdc1080;
MPU9250 mySensor;
CCS811 ccs;

byte address;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

void I2C_Scan()
{
  byte error;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;

  for(address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
      Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
      break;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
      Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if(nDevices == 0)
  {
    Serial.println("I2C devices check again\n");
    pinMode(GPIO0,OUTPUT);
    digitalWrite(GPIO0,LOW);//set GPIO0 to enable CCS811
    I2C_Scan();
  }
}

void printSerialNumber() {
  Serial.print("Device Serial Number=");
  HDC1080_SerialNumber sernum = hdc1080.readSerialNumber();
  char format[12];
  sprintf(format, "%02X-%04X-%04X", sernum.serialFirst, sernum.serialMid, sernum.serialLast);
  Serial.println(format);
}

void setup()
{
  Serial.begin(115200);
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);//set vext to high

  Wire.begin();

  I2C_Scan();

  switch(address)
  {
    case 35: //0x23 -- BH1750 light sensor
    {
      lightMeter.begin();
      Serial.println(F("BH1750 Test begin"));

      break;
    }

    case 119: //0x77 -- BMP180 Barometer
    {
      if (!bmp.begin())
      {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      while(1);
      }
      else
        Serial.println("BMP180 Barometr sensor initial done");

      break;
    }

    case 64: //0x40 -- HDC1080 temperature and humidity sensor
    {
      hdc1080.begin(0x40);
      Serial.print("Manufacturer ID=0x");
      Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
      Serial.print("Device ID=0x");
      Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device

      printSerialNumber();

      break;
    }

    case 104: //0x68 -- MPU9250 9-axis sensor
    {
      mySensor.setWire(&Wire);

      mySensor.beginAccel();
      mySensor.beginGyro();
      mySensor.beginMag();

      break;
    }
      case 90: //0x5A --CCS811
      {
         Serial.begin(115200);
         Serial.println("CCS811 test");
         if(!ccs.begin())
         {
          Serial.println("Failed to start sensor! Please check your wiring.");
          while(1);
        }
        break;
    }
  }
}

void loop()
{
  switch(address)
  {
    case 35: //0x23 -- BH1750 light sensor
    {
      float lux = lightMeter.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");

      break;
    }

    case 119: //0x77 -- BMP180 Barometer sensor
    {
      Serial.print("Temperature = ");
      Serial.print(bmp.readTemperature());
      Serial.println(" *C");

      Serial.print("Pressure = ");
      Serial.print(bmp.readPressure());
      Serial.println(" Pa");

      Serial.print("Altitude = ");
      Serial.print(bmp.readAltitude());
      Serial.println(" meters");

      Serial.print("Pressure at sealevel (calculated) = ");
      Serial.print(bmp.readSealevelPressure());
      Serial.println(" Pa");

      Serial.print("Real altitude = ");
      Serial.print(bmp.readAltitude(101500));
      Serial.println(" meters");

      Serial.println();

      break;
    }

    case 64: //0x40 -- HDC1080 temperature and humidity sensor
    {
      Serial.print("T=");
      Serial.print(hdc1080.readTemperature());
      Serial.print("C, RH=");
      Serial.print(hdc1080.readHumidity());
      Serial.println("%");

      break;
    }

    case 104: //0x68 -- MPU9250 9-axis sensor
    {
      uint8_t sensorId;

      if (mySensor.readId(&sensorId) == 0) {
      Serial.println("sensorId: " + String(sensorId));
      } else {
      Serial.println("Cannot read sensorId");
      }

      if (mySensor.accelUpdate() == 0)
      {
      aX = mySensor.accelX();
      aY = mySensor.accelY();
      aZ = mySensor.accelZ();
      aSqrt = mySensor.accelSqrt();
      Serial.println("accelX: " + String(aX));
      Serial.println("accelY: " + String(aY));
      Serial.println("accelZ: " + String(aZ));
      Serial.println("accelSqrt: " + String(aSqrt));
      } else {
      Serial.println("Cannod read accel values");
      }

      if (mySensor.gyroUpdate() == 0) {
      gX = mySensor.gyroX();
      gY = mySensor.gyroY();
      gZ = mySensor.gyroZ();
      Serial.println("gyroX: " + String(gX));
      Serial.println("gyroY: " + String(gY));
      Serial.println("gyroZ: " + String(gZ));
      } else {
      Serial.println("Cannot read gyro values");
      }

      if (mySensor.magUpdate() == 0) {
      mX = mySensor.magX();
      mY = mySensor.magY();
      mZ = mySensor.magZ();
      mDirection = mySensor.magHorizDirection();
      Serial.println("magX: " + String(mX));
      Serial.println("maxY: " + String(mY));
      Serial.println("magZ: " + String(mZ));
      Serial.println("horizontal direction: " + String(mDirection));
      } else {
      Serial.println("Cannot read mag values");
      }

      Serial.println("at " + String(millis()) + "ms");
      Serial.println(""); // Add an empty line

      break;
    }
    case 90: //0x5a -- CCS811 Air quality sensor
      {
      digitalWrite(GPIO0,LOW);//set GPIO0 to enable CCS811

        if(ccs.available()){
          if(!ccs.readData()){
            Serial.print("CO2: ");
            Serial.print(ccs.geteCO2());
            Serial.println(" ppm");
            Serial.print("TVOC:");
            Serial.print(ccs.getTVOC());
            Serial.println(" ppb");
            Serial.print("millis:");

            Serial.println(millis());
         }
          else{
            Serial.println("ERROR!");
            while(1);
         }
       }
        break;
      }
    }

  digitalWrite(GPIO0,HIGH);//turn off GPIO0 2S
  delay(2000);
}
