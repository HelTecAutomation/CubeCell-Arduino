#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>
#include "softSerial.h"
#include "TinyGPS++.h"

String wasnver = "1.0.0";
String wasnflash = "GPS-Capsule";

TinyGPSPlus gps;
softSerial ss(GPIO5 /*TX pin*/, GPIO3 /*RX pin*/);

/*
   set LoraWan_RGB to Active,the RGB active in loraWan
   RGB red means sending;
   RGB purple means joined done;
   RGB blue means RxWindow1;
   RGB yellow means RxWindow2;
   RGB green means received done;
*/

/* OTAA para*/
uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint32_t devAddr =  ( uint32_t )0x00000000;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

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
uint8_t confirmedNbTrials = 4;

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void prepareTxFrame( uint8_t port )
{
  /*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
    appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
    if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
    if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
    for example, if use REGION_CN470,
    the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
  */

  pinMode(GPIO0, OUTPUT);
  pinMode(Vext, OUTPUT);
  digitalWrite(GPIO0, HIGH);
  digitalWrite(Vext, LOW);

  float lat, lon, alt, course, speed, hdop, sats;

  Serial.println("Waiting for GPS FIX ...");
 
  while (!gps.location.isValid()) 
  {
    //smartDelay(1000);
    //uint32_t start = millis();
    do 
    {
      if (ss.available()) 
      {
        gps.encode(ss.read());
      }
    } while (gps.charsProcessed() < 100); //((millis() - start) < 200);

    //if ((millis() + start) > 5000)// && gps.charsProcessed() < 10) 
    //{
    //  Serial.println("No GPS data received: check wiring");
    //}
  }

  lat = gps.location.lat();
  lon = gps.location.lng();
  alt = gps.altitude.meters();
  course = gps.course.deg();
  speed = gps.speed.kmph();
  sats = gps.satellites.value();
  hdop = gps.hdop.hdop();

  digitalWrite(Vext, HIGH);
  digitalWrite(GPIO0, LOW);

  pinMode(GPIO0, ANALOG);
  uint16_t batteryVoltage = getBatteryVoltage();

  unsigned char *puc;

  appDataSize = 0;
  puc = (unsigned char *)(&lat);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];
  puc = (unsigned char *)(&lon);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];
  puc = (unsigned char *)(&alt);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];
  puc = (unsigned char *)(&course);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];
  puc = (unsigned char *)(&speed);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];
  puc = (unsigned char *)(&hdop);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];
  appData[appDataSize++] = (uint8_t)(batteryVoltage >> 8);
  appData[appDataSize++] = (uint8_t)batteryVoltage;

  Serial.print("SATS: ");
  Serial.print(gps.satellites.value());
  Serial.print(", HDOP: ");
  Serial.print(gps.hdop.hdop());
  Serial.print(", LAT: ");
  Serial.print(gps.location.lat());
  Serial.print(", LON: ");
  Serial.print(gps.location.lng());
  Serial.print(", AGE: ");
  Serial.print(gps.location.age());
  Serial.print(", ALT: ");
  Serial.print(gps.altitude.meters());
  Serial.print(", COURSE: ");
  Serial.print(gps.course.deg());
  Serial.print(", SPEED: ");
  Serial.println(gps.speed.kmph());
  Serial.print(" BatteryVoltage:");
  Serial.println(batteryVoltage);
}


void setup() {
  Serial.begin(115200);
  ss.begin(9600);

#if(AT_SUPPORT)
  enableAt();
#endif

  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
}

void loop()
{
  switch ( deviceState )
  {
    case DEVICE_STATE_INIT:
      {
#if(LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
#endif
#if(AT_SUPPORT)
        getDevParam();
#endif
        printDevParam();
        LoRaWAN.init(loraWanClass, loraWanRegion);
        deviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
        LoRaWAN.join();
        break;
      }
    case DEVICE_STATE_SEND:
      {
        prepareTxFrame( appPort );
        LoRaWAN.send();
        deviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
      }
    case DEVICE_STATE_SLEEP:
      {
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

bool checkUserAt(char *cmd, char *content)
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
  if (strcmp(cmd, "FLASH") == 0) 
  {
    if (content[0] == '?')
    {
      Serial.print("+FLASH=");
      Serial.println(wasnflash);
    }
    return true;
  }
  if (strcmp(cmd, "BATTERY") == 0)
  {
    if (content[0] == '?')
    {
      uint16_t BatteryVoltage = getBatteryVoltage();
      Serial.print("+BATTERY=");
      Serial.print(BatteryVoltage);
      Serial.println();
    }
    return true;
  }
  if (strcmp(cmd, "LED") == 0)
  {
    if (content[0] == '?')
    {
      Serial.print("+LED=");
      Serial.print(LoraWan_RGB);
      Serial.println();
    }
    return true;
  }
  return false;
}