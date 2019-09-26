#include "LoRaWan_APP.h"
#include "LoRa_APP.h"
#include "Arduino.h"


#ifndef ACTIVE_REGION
#define ACTIVE_REGION LORAMAC_REGION_CN470
#endif

#ifndef CLASS_MODE
#define CLASS_MODE CLASS_A
#endif

DeviceClass_t  CLASS=CLASS_MODE;

/*
 * set LoraWan_RGB to 1,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

/*!
 * When set to true the application uses the Over-the-Air activation procedure
 * When set to false the application uses the Personalization activation procedure
 */
bool OVER_THE_AIR_ACTIVATION = true;

/* LoRaWAN Adaptive Data Rate */
bool LORAWAN_ADR_ON = true;

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


void setup() {
	BoardInitMcu();
	Serial.begin(115200);
	PassthroughMode = true;
	Enable_AT(); 
	getDevParam();
	printDevParam();
	DeviceState = DEVICE_STATE_SLEEP;
	DeviceState_lora = LORA_INIT;
}

void loop()
{
	if(Mode_LoraWan)//loraWan mode(defalt;)
	{
	  switch( DeviceState )
	  {
	    case DEVICE_STATE_INIT:
	    {
	      //Serial.printf("LoRaWan PassthroughMode start!\r\n");
	      //Serial.printf("please start with command\r\n");
	      getDevParam();
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
	      LoRaWAN.Send();
	      DeviceState = DEVICE_STATE_SLEEP;
	      break;
	    }
	    case DEVICE_STATE_CYCLE:
	    {
	      // only used while joining
	      TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( 0, APP_TX_DUTYCYCLE_RND );
	      DeviceState = DEVICE_STATE_SLEEP;
	      LoRaWAN.Cycle(TxDutyCycleTime);
	      break;
	    }
	    case DEVICE_STATE_SLEEP:
	    {
	      LoRaWAN.Sleep();
	      break;
	    }
	    default:
	    {
	      DeviceState = DEVICE_STATE_SLEEP;
	      break;
	    }
	  }
	}
	else//lora mode
	{
	  switch( DeviceState_lora )
	  {
	    case LORA_INIT:
	    {
	      getDevParam();
	      LoRa.Init(Lora_FREQ, Lora_TXPW, Lora_SF);
	      DeviceState_lora = MCU_SLEEP;
	      break;
	    }
	    case LORA_SEND:
	    {
	      LoRa.Send();
	      DeviceState_lora = MCU_SLEEP;
	      break;
	    }
	    case LORA_RECEIVE:
	    {
	      LoRa.Receive();
	      DeviceState_lora = MCU_SLEEP;
	      break;
	    }
	    case MCU_SLEEP:
	    {
	      LoRa.LOWPOWER();
	      break;
	    }
	    default:
	    {
	      DeviceState_lora = MCU_SLEEP;
	      break;
	    }
	  }
	}
}
