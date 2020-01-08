#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>
#include <BMP180.h>

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/*LoraWan Class*/
DeviceClass_t  CLASS=LORAWAN_CLASS;
/*OTAA or ABP*/
bool OVER_THE_AIR_ACTIVATION = LORAWAN_NETMODE;
/*ADR enable*/
bool LORAWAN_ADR_ON = LORAWAN_ADR;
/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool KeepNet = LORAWAN_Net_Reserve;
/*LoraWan REGION*/
LoRaMacRegion_t REGION = ACTIVE_REGION;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool IsTxConfirmed = LORAWAN_UPLINKMODE;

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
uint8_t ConfirmedNbTrials = 8;

/* Application port */
uint8_t AppPort = 2;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = 15000;

/* Prepares the payload of the frame */
BMP085 bmp;
static void PrepareTxFrame( uint8_t port )
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  
  bmp.begin();
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude();
  float sealevelpressure = bmp.readSealevelPressure();
  
  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  //    Serial.print("Real altitude = ");
  //    Serial.print(bmp.readAltitude(101500));
  //    Serial.println(" meters");
  Wire.end();
  
  digitalWrite(Vext, HIGH);
  uint16_t BatteryVoltage = GetBatteryVoltage();
  
  unsigned char *puc;
  puc = (unsigned char *)(&temperature);
  AppDataSize = 14;//AppDataSize max value is 64
  AppData[0] = puc[0];
  AppData[1] = puc[1];
  AppData[2] = puc[2];
  AppData[3] = puc[3];
  
  puc = (unsigned char *)(&pressure);
  AppData[4] = puc[0];
  AppData[5] = puc[1];
  AppData[6] = puc[2];
  AppData[7] = puc[3];
  
  puc = (unsigned char *)(&altitude);
  AppData[8] = puc[0];
  AppData[9] = puc[1];
  AppData[10] = puc[2];
  AppData[11] = puc[3];
  
  AppData[12] = (uint8_t)(BatteryVoltage>>8);
  AppData[13] = (uint8_t)BatteryVoltage;
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Pressure: ");
  Serial.print(pressure);
  Serial.print("Pa, ");
  Serial.print("BatteryVoltage:");
  Serial.println(BatteryVoltage);
}


void setup() {
    BoardInitMcu();
    Serial.begin(115200);
#if(AT_SUPPORT)
    Enable_AT();
#endif
    DeviceState = DEVICE_STATE_INIT;
    LoRaWAN.Ifskipjoin();
}

void loop()
{
	switch( DeviceState )
	{
		case DEVICE_STATE_INIT:
		{ 
#if(AT_SUPPORT)
			getDevParam();
#endif
			printDevParam();
			Serial.printf("LoRaWan Class%X start! \r\n",CLASS+10);  
			LoRaWAN.Init(CLASS,REGION);
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