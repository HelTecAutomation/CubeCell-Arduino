#include "LoRaWan_APP.h"
#include "Arduino.h"


/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/*LoraWan Class*/
DeviceClass_t  CLASS=CLASS_C;
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
static void PrepareTxFrame( uint8_t port )
{
    AppDataSize = 4;//AppDataSize max value is 64
    AppData[0] = 0x00;
    AppData[1] = 0x01;
    AppData[2] = 0x02;
    AppData[3] = 0x03;
}

MulticastParams_t mult1;
uint8_t mulNwkSKey[]={0x5c,0x1d,0xce,0x81,0xd8,0x19,0x40,0xb9,0xe0,0xfb,0x1e,0x07,0xdd,0x4d,0xd3,0x9c};
uint8_t mulAppSKey[]={0x6b,0x5b,0x47,0x6f,0x73,0xb6,0xc3,0x98,0xc8,0x11,0xa8,0xd0,0xd9,0x9f,0x25,0xc7};
uint32_t multicastAddress=0x00638f9e;

void setup() {
	BoardInitMcu();
	Serial.begin(115200);
#if(AT_SUPPORT)
	Enable_AT();
#endif
	DeviceState = DEVICE_STATE_INIT;
	LoRaWAN.Ifskipjoin();
	mult1.Address=multicastAddress;
	for(int i=0;i<16;i++)
	{
		mult1.NwkSKey[i]=mulNwkSKey[i];
		mult1.AppSKey[i]=mulAppSKey[i];
	}
	LoRaMacMulticastChannelLink(&mult1);
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
			Serial.printf("LoRaWan mutlcast start! \r\n");   
			LoRaWAN.Init(CLASS,ACTIVE_REGION);
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
