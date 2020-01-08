#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>
#include <MPU9250.h>


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
MPU9250 mySensor;

static void PrepareTxFrame( uint8_t port )
{
  float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
	pinMode(Vext, OUTPUT);
	digitalWrite(Vext, LOW);
  Wire.begin();
  mySensor.setWire(&Wire);
 
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();


  
  // You can set your own offset for mag values
  // mySensor.magXOffset = -50;
  // mySensor.magYOffset = -55;
  // mySensor.magZOffset = -10;
  

  mySensor.accelUpdate();
  aX = mySensor.accelX();
  aY = mySensor.accelY();
  aZ = mySensor.accelZ();
  aSqrt = mySensor.accelSqrt();
  Serial.print("aX:");
  Serial.println(aX);
  Serial.print("aY:");
  Serial.println(aY);
  Serial.print("aZ:");
  Serial.println(aZ);
  Serial.print("aSqrt:");
  Serial.println(aSqrt);
  

  mySensor.gyroUpdate();
  gX = mySensor.gyroX();
  gY = mySensor.gyroY();
  gZ = mySensor.gyroZ();
  Serial.print("gX:");
  Serial.println(gX);
  Serial.print("gY:");
  Serial.println(gY);
  Serial.print("gZ:");
  Serial.println(gZ);
  

  mySensor.magUpdate();
  mX = mySensor.magX();
  mY = mySensor.magY();
  mZ = mySensor.magZ();
  mDirection = mySensor.magHorizDirection();
  Serial.print("mX:");
  Serial.println(mX);
  Serial.print("mY:");
  Serial.println(mY);
  Serial.print("mZ:");
  Serial.println(mZ);
  Serial.print("mDirection:");
  Serial.println(mDirection);
  
	Wire.end();
	digitalWrite(Vext, HIGH);
	uint16_t BatteryVoltage = GetBatteryVoltage();
	unsigned char *puc;

	puc = (unsigned char *)(&aX);
	AppDataSize = 46;//AppDataSize max value is 64
  AppData[0] = puc[0];
  AppData[1] = puc[1];
  AppData[2] = puc[2];
  AppData[3] = puc[3];
  puc = (unsigned char *)(&aY);
  AppData[4] = puc[0];
  AppData[5] = puc[1];
  AppData[6] = puc[2];
  AppData[7] = puc[3];
  puc = (unsigned char *)(&aZ);
  AppData[8] = puc[0];
  AppData[9] = puc[1];
  AppData[10] = puc[2];
  AppData[11] = puc[3];
  puc = (unsigned char *)(&gX);
  AppData[12] = puc[0];
  AppData[13] = puc[1];
  AppData[14] = puc[2];
  AppData[15] = puc[3];
  puc = (unsigned char *)(&gY);
  AppData[16] = puc[0];
  AppData[17] = puc[1];
  AppData[18] = puc[2];
  AppData[19] = puc[3];
  puc = (unsigned char *)(&gZ);
  AppData[20] = puc[0];
  AppData[21] = puc[1];
  AppData[22] = puc[2];
  AppData[23] = puc[3];
  puc = (unsigned char *)(&mX);
  AppData[24] = puc[0];
  AppData[25] = puc[1];
  AppData[26] = puc[2];
  AppData[27] = puc[3];
  puc = (unsigned char *)(&mY);
  AppData[28] = puc[0];
  AppData[29] = puc[1];
  AppData[30] = puc[2];
  AppData[31] = puc[3];
  puc = (unsigned char *)(&mZ);
  AppData[32] = puc[0];
  AppData[33] = puc[1];
  AppData[34] = puc[2];
  AppData[35] = puc[3];
  puc = (unsigned char *)(&aSqrt);
  AppData[36] = puc[0];
  AppData[37] = puc[1];
  AppData[38] = puc[2];
  AppData[39] = puc[3];
  puc = (unsigned char *)(&mDirection);
  AppData[40] = puc[0];
  AppData[41] = puc[1];
  AppData[42] = puc[2];
  AppData[43] = puc[3];
  
	AppData[44] = (uint8_t)(BatteryVoltage>>8);
	AppData[45] = (uint8_t)BatteryVoltage;
	

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