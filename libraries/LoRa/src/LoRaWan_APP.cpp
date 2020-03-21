#include <LoRaWan_APP.h>
#if(LoraWan_RGB==1)
#include "Adafruit_NeoPixel.h"
Adafruit_NeoPixel pixels(1, RGB, NEO_GRB + NEO_KHZ800);
#endif

#if REGION_EU868
#include "RegionEU868.h"
#endif

#if REGION_EU433
#include "RegionEU433.h"
#endif

/*!
 * Default datarate
 */
#define LORAWAN_DEFAULT_DATARATE                    DR_5

/*!
 * User application data size
 */
uint8_t appDataSize = 4;

/*!
 * User application data
 */
uint8_t appData[LORAWAN_APP_DATA_MAX_SIZE];


/*!
 * Defines the application data transmission duty cycle
 */
uint32_t txDutyCycleTime ;

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t TxNextPacketTimer;

/*!
 * PassthroughMode mode enable/disable. don't modify it here. 
 * when use PassthroughMode, set it true in app.ino , Reference the example PassthroughMode.ino 
 */
bool passthroughMode = false;

/*!
 * when use PassthroughMode, Mode_LoraWan to set use lora or lorawan mode . don't modify it here. 
 * it is used to set mode lora/lorawan in PassthroughMode.
 */
bool modeLoraWan = true;

/*!
 * Indicates if a new packet can be sent
 */
static bool nextTx = true;


enum eDeviceState_LoraWan deviceState;


/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
bool SendFrame( void )
{
	McpsReq_t mcpsReq;
	LoRaMacTxInfo_t txInfo;

	if( LoRaMacQueryTxPossible( appDataSize, &txInfo ) != LORAMAC_STATUS_OK )
	{
		// Send empty frame in order to flush MAC commands
		mcpsReq.Type = MCPS_UNCONFIRMED;
		mcpsReq.Req.Unconfirmed.fBuffer = NULL;
		mcpsReq.Req.Unconfirmed.fBufferSize = 0;
		mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
	}
	else
	{
		if( isTxConfirmed == false )
		{
			printf("unconfirmed uplink sending ...\r\n");
			mcpsReq.Type = MCPS_UNCONFIRMED;
			mcpsReq.Req.Unconfirmed.fPort = appPort;
			mcpsReq.Req.Unconfirmed.fBuffer = appData;
			mcpsReq.Req.Unconfirmed.fBufferSize = appDataSize;
			mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
		}
		else
		{
			printf("confirmed uplink sending ...\r\n");
			mcpsReq.Type = MCPS_CONFIRMED;
			mcpsReq.Req.Confirmed.fPort = appPort;
			mcpsReq.Req.Confirmed.fBuffer = appData;
			mcpsReq.Req.Confirmed.fBufferSize = appDataSize;
			mcpsReq.Req.Confirmed.NbTrials = confirmedNbTrials;
			mcpsReq.Req.Confirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
		}
	}
	if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
	{
		return false;
	}
	return true;
}

/*!
 * \brief Function executed on TxNextPacket Timeout event
 */
static void OnTxNextPacketTimerEvent( void )
{
	MibRequestConfirm_t mibReq;
	LoRaMacStatus_t status;

	TimerStop( &TxNextPacketTimer );

	mibReq.Type = MIB_NETWORK_JOINED;
	status = LoRaMacMibGetRequestConfirm( &mibReq );

	if( status == LORAMAC_STATUS_OK )
	{
		if( mibReq.Param.IsNetworkJoined == true )
		{
			deviceState = DEVICE_STATE_SEND;
			nextTx = true;
		}
		else
		{
			// Network not joined yet. Try to join again
			MlmeReq_t mlmeReq;
			mlmeReq.Type = MLME_JOIN;
			mlmeReq.Req.Join.DevEui = devEui;
			mlmeReq.Req.Join.AppEui = appEui;
			mlmeReq.Req.Join.AppKey = appKey;
			mlmeReq.Req.Join.NbTrials = 1;

			if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
			{
				deviceState = DEVICE_STATE_SLEEP;
			}
			else
			{
				deviceState = DEVICE_STATE_CYCLE;
			}
		}
	}
}

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] mcpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void McpsConfirm( McpsConfirm_t *mcpsConfirm )
{
	if( mcpsConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
	{
		switch( mcpsConfirm->McpsRequest )
		{
			case MCPS_UNCONFIRMED:
			{
				// Check Datarate
				// Check TxPower
				break;
			}
			case MCPS_CONFIRMED:
			{
				// Check Datarate
				// Check TxPower
				// Check AckReceived
				// Check NbTrials
				break;
			}
			case MCPS_PROPRIETARY:
			{
				break;
			}
			default:
				break;
		}
	}
	nextTx = true;
}

#if(LoraWan_RGB==1)
void turnOnRGB(uint32_t color,uint32_t time)
{
	uint8_t red,green,blue;
	red=(uint8_t)(color>>16);
	green=(uint8_t)(color>>8);
	blue=(uint8_t)color;
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW); //SET POWER
	delay(1);
	pixels.begin(); // INITIALIZE RGB strip object (REQUIRED)
	pixels.clear(); // Set all pixel colors to 'off'
	pixels.setPixelColor(0, pixels.Color(red, green, blue));
	pixels.show();   // Send the updated pixel colors to the hardware.
	if(time>0)
	{
		delay(time);
	}
}

void turnOffRGB(void)
{
	turnOnRGB(0,0);
	digitalWrite(Vext,HIGH);
}
#endif

/*  get the BatteryVoltage in mV. */
uint16_t getBatteryVoltage(void)
{
	pinMode(VBAT_ADC_CTL,OUTPUT);
	digitalWrite(VBAT_ADC_CTL,LOW);
	uint16_t volt=analogRead(ADC)*2;
	digitalWrite(VBAT_ADC_CTL,HIGH);
	return volt;
}


void __attribute__((weak)) downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
	printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
	printf("+REV DATA:");
	for(uint8_t i=0;i<mcpsIndication->BufferSize;i++)
	{
		printf("%02X",mcpsIndication->Buffer[i]);
	}
	printf("\r\n");
}

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */
static void McpsIndication( McpsIndication_t *mcpsIndication )
{
	if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
	{
		return;
	}


	printf( "receive data: rssi = %d, snr = %d, datarate = %d\r\n", mcpsIndication->Rssi, (int)mcpsIndication->Snr,(int)mcpsIndication->RxDatarate);

#if (LoraWan_RGB==1)
	turnOnRGB(COLOR_RECEIVED, 200);
	turnOffRGB();
#endif

	switch( mcpsIndication->McpsIndication )
	{
		case MCPS_UNCONFIRMED:
		{
			break;
		}
		case MCPS_CONFIRMED:
		{
			break;
		}
		case MCPS_PROPRIETARY:
		{
			break;
		}
		case MCPS_MULTICAST:
		{
			break;
		}
		default:
			break;
	}

	// Check Multicast
	// Check Port
	// Check Datarate
	// Check FramePending
	if( mcpsIndication->FramePending == true )
	{
		// The server signals that it has pending data to be sent.
		// We schedule an uplink as soon as possible to flush the server.
		OnTxNextPacketTimerEvent( );
	}
	// Check Buffer
	// Check BufferSize
	// Check Rssi
	// Check Snr
	// Check RxSlot
	if( mcpsIndication->RxData == true )
	{
		downLinkDataHandle(mcpsIndication);
	}
}

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] mlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
	switch( mlmeConfirm->MlmeRequest )
	{
		case MLME_JOIN:
		{
			if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
			{

#if (LoraWan_RGB==1)
				turnOnRGB(COLOR_JOINED,500);
				turnOffRGB();
#endif
				printf("joined\r\n");
				
				//in PassthroughMode,do nothing while joined
				if(passthroughMode == false)
				{
					// Status is OK, node has joined the network
					deviceState = DEVICE_STATE_SEND;
				}
			}
			else
			{
				uint32_t rejoin_delay = 30000;
				printf("join failed, join again at 30s later\r\n");
				TimerSetValue( &TxNextPacketTimer, rejoin_delay );
				TimerStart( &TxNextPacketTimer );
			}
			break;
		}
		case MLME_LINK_CHECK:
		{
			if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
			{
				// Check DemodMargin
				// Check NbGateways
			}
			break;
		}
		default:
			break;
	}
	nextTx = true;
}

/*!
 * \brief   MLME-Indication event function
 *
 * \param   [IN] mlmeIndication - Pointer to the indication structure.
 */
static void MlmeIndication( MlmeIndication_t *mlmeIndication )
{
	switch( mlmeIndication->MlmeIndication )
	{
		case MLME_SCHEDULE_UPLINK:
		{// The MAC signals that we shall provide an uplink as soon as possible
			OnTxNextPacketTimerEvent( );
			break;
		}
		default:
			break;
	}
}


static void lwan_dev_params_update( void )
{
#ifdef REGION_EU868
	LoRaMacChannelAdd( 3, ( ChannelParams_t )EU868_LC4 );
	LoRaMacChannelAdd( 4, ( ChannelParams_t )EU868_LC5 );
	LoRaMacChannelAdd( 5, ( ChannelParams_t )EU868_LC6 );
	LoRaMacChannelAdd( 6, ( ChannelParams_t )EU868_LC7 );
	LoRaMacChannelAdd( 7, ( ChannelParams_t )EU868_LC8 );
#endif

#ifdef REGION_EU433
		LoRaMacChannelAdd( 3, ( ChannelParams_t )EU433_LC4 );
		LoRaMacChannelAdd( 4, ( ChannelParams_t )EU433_LC5 );
		LoRaMacChannelAdd( 5, ( ChannelParams_t )EU433_LC6 );
		LoRaMacChannelAdd( 6, ( ChannelParams_t )EU433_LC7 );
		LoRaMacChannelAdd( 7, ( ChannelParams_t )EU433_LC8 );
#endif

	MibRequestConfirm_t mibReq;
	uint16_t channelsMaskTemp[6];
	channelsMaskTemp[0] = 0x00FF;
	channelsMaskTemp[1] = 0x0000;
	channelsMaskTemp[2] = 0x0000;
	channelsMaskTemp[3] = 0x0000;
	channelsMaskTemp[4] = 0x0000;
	channelsMaskTemp[5] = 0x0000;

	mibReq.Type = MIB_CHANNELS_DEFAULT_MASK;
	mibReq.Param.ChannelsMask = channelsMaskTemp;
	LoRaMacMibSetRequestConfirm(&mibReq);

	mibReq.Type = MIB_CHANNELS_MASK;
	mibReq.Param.ChannelsMask = channelsMaskTemp;
	LoRaMacMibSetRequestConfirm(&mibReq);


}

uint8_t BoardGetBatteryLevel()
{
	int8 batlevel = ((getBatteryVoltage()-3.7)/(4.2-3.7))*100;
	return batlevel;
}

LoRaMacPrimitives_t LoRaMacPrimitive;
LoRaMacCallback_t LoRaMacCallback;

void LoRaWanClass::init(DeviceClass_t lorawanClass,LoRaMacRegion_t region)
{
	Serial.print("\r\nLoRaWAN ");
	switch(region)
	{
		case LORAMAC_REGION_AS923:
			Serial.print("AS923");
			break;
		case LORAMAC_REGION_AU915:
			Serial.print("AU915");
			break;
		case LORAMAC_REGION_CN470:
			Serial.print("CN470");
			break;
		case LORAMAC_REGION_CN779:
			Serial.print("CN779");
			break;
		case LORAMAC_REGION_EU433:
			Serial.print("EU433");
			break;
		case LORAMAC_REGION_EU868:
			Serial.print("EU868");
			break;
		case LORAMAC_REGION_KR920:
			Serial.print("KR920");
			break;
		case LORAMAC_REGION_IN865:
			Serial.print("IN865");
			break;
		case LORAMAC_REGION_US915:
			Serial.print("US915");
			break;
		case LORAMAC_REGION_US915_HYBRID:
			Serial.print("US915_HYBRID ");
			break;
	}
	Serial.printf(" Class %X start!\r\n\r\n",loraWanClass+10);

	MibRequestConfirm_t mibReq;

	LoRaMacPrimitive.MacMcpsConfirm = McpsConfirm;
	LoRaMacPrimitive.MacMcpsIndication = McpsIndication;
	LoRaMacPrimitive.MacMlmeConfirm = MlmeConfirm;
	LoRaMacPrimitive.MacMlmeIndication = MlmeIndication;
	LoRaMacCallback.GetBatteryLevel = BoardGetBatteryLevel;
	LoRaMacCallback.GetTemperatureLevel = NULL;
	LoRaMacInitialization( &LoRaMacPrimitive, &LoRaMacCallback,region);
	TimerStop( &TxNextPacketTimer );
	TimerInit( &TxNextPacketTimer, OnTxNextPacketTimerEvent );

	mibReq.Type = MIB_ADR;
	mibReq.Param.AdrEnable = loraWanAdr;
	LoRaMacMibSetRequestConfirm( &mibReq );

	mibReq.Type = MIB_PUBLIC_NETWORK;
	mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
	LoRaMacMibSetRequestConfirm( &mibReq );

	lwan_dev_params_update();

	deviceState = DEVICE_STATE_JOIN;
}


void LoRaWanClass::join()
{	
	if( overTheAirActivation )
	{
		Serial.print("joining...");
		MlmeReq_t mlmeReq;
		
		mlmeReq.Type = MLME_JOIN;

		mlmeReq.Req.Join.DevEui = devEui;
		mlmeReq.Req.Join.AppEui = appEui;
		mlmeReq.Req.Join.AppKey = appKey;
		mlmeReq.Req.Join.NbTrials = 1;

		if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
		{
			deviceState = DEVICE_STATE_SLEEP;
		}
		else
		{
			deviceState = DEVICE_STATE_CYCLE;
		}
	}
	else
	{
		MibRequestConfirm_t mibReq;

		mibReq.Type = MIB_NET_ID;
		mibReq.Param.NetID = LORAWAN_NETWORK_ID;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_DEV_ADDR;
		mibReq.Param.DevAddr = devAddr;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_NWK_SKEY;
		mibReq.Param.NwkSKey = nwkSKey;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_APP_SKEY;
		mibReq.Param.AppSKey = appSKey;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_NETWORK_JOINED;
		mibReq.Param.IsNetworkJoined = true;
		LoRaMacMibSetRequestConfirm( &mibReq );
		
		deviceState = DEVICE_STATE_SEND;
	}
}

void LoRaWanClass::send()
{
	if( nextTx == true )
	{
	lwan_dev_params_update();
		MibRequestConfirm_t mibReq;
		mibReq.Type = MIB_DEVICE_CLASS;
		LoRaMacMibGetRequestConfirm( &mibReq );

		if(loraWanClass != mibReq.Param.Class)
		{
			mibReq.Param.Class = loraWanClass;
			LoRaMacMibSetRequestConfirm( &mibReq );
		}
		
		nextTx = SendFrame( );
	}
}

void LoRaWanClass::cycle(uint32_t dutyCycle)
{
	TimerSetValue( &TxNextPacketTimer, dutyCycle );
	TimerStart( &TxNextPacketTimer );
}

void LoRaWanClass::sleep()
{
	lowPowerHandler( );
	// Process Radio IRQ
	Radio.IrqProcess( );
}

void LoRaWanClass::ifskipjoin()
{
//if saved net info is OK in lorawan mode, skip join.
	if(checkNetInfo()&&modeLoraWan){
		Serial.println();
		if(passthroughMode==false)
		{
			Serial.println("Wait 3s for user key to rejoin network");
			uint16_t i=0;
			pinMode(GPIO7,INPUT);
			while(i<=3000)
			{
				if(digitalRead(GPIO7)==LOW)//if user key down, rejoin network;
				{
					netInfoDisable();
					pinMode(GPIO7,OUTPUT);
					digitalWrite(GPIO7,HIGH);
					return;
				}
				delay(1);
				i++;
			}
			pinMode(GPIO7,OUTPUT);
			digitalWrite(GPIO7,HIGH);
		}
#if(AT_SUPPORT)
		getDevParam();
#endif

		init(loraWanClass,loraWanRegion);
		getNetInfo();
		if(passthroughMode==false){
			Serial.println("User key not detected,Use reserved Net");
		}
		else{
			Serial.println("Use reserved Net");
		}
		if(passthroughMode==false)
		{
			int32_t temp=randr(0,appTxDutyCycle);
			Serial.println();
			Serial.printf("Next packet send %d ms later(random time from 0 to APP_TX_DUTYCYCLE)\r\n",temp);
			Serial.println();
			cycle(temp);//send packet in a random time to avoid network congestion.
		}
		deviceState = DEVICE_STATE_SLEEP;
	}
}


LoRaWanClass LoRaWAN;

