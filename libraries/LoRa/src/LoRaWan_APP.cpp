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

uint8_t DevEui[] = LORAWAN_DEVICE_EUI;
uint8_t AppEui[] = LORAWAN_APPLICATION_EUI;
uint8_t AppKey[] = LORAWAN_APPLICATION_KEY;

uint8_t NwkSKey[] = LORAWAN_NWKSKEY;
uint8_t AppSKey[] = LORAWAN_APPSKEY;
uint32_t DevAddr = LORAWAN_DEVICE_ADDRESS;


/*!
 * User application data size
 */
uint8_t AppDataSize = 4;

/*!
 * User application data
 */
uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];


/*!
 * Defines the application data transmission duty cycle
 */
uint32_t TxDutyCycleTime ;

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t TxNextPacketTimer;

/*!
 * PassthroughMode mode enable/disable. don't modify it here. 
 * when use PassthroughMode, set it true in app.ino , Reference the example PassthroughMode.ino 
 */
bool PassthroughMode = false;

/*!
 * when use PassthroughMode, Mode_LoraWan to set use lora or lorawan mode . don't modify it here. 
 * it is used to set mode lora/lorawan in PassthroughMode.
 */
bool Mode_LoraWan = true;


/*!
 * Indicates if a new packet can be sent
 */
static bool NextTx = true;


enum eDeviceState_LoraWan DeviceState;


/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
bool SendFrame( void )
{
	McpsReq_t mcpsReq;
	LoRaMacTxInfo_t txInfo;

	if( LoRaMacQueryTxPossible( AppDataSize, &txInfo ) != LORAMAC_STATUS_OK )
	{
		// Send empty frame in order to flush MAC commands
		mcpsReq.Type = MCPS_UNCONFIRMED;
		mcpsReq.Req.Unconfirmed.fBuffer = NULL;
		mcpsReq.Req.Unconfirmed.fBufferSize = 0;
		mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
	}
	else
	{
		if( IsTxConfirmed == false )
		{
			printf("unconfirmed uplink sending ...\r\n");
			mcpsReq.Type = MCPS_UNCONFIRMED;
			mcpsReq.Req.Unconfirmed.fPort = AppPort;
			mcpsReq.Req.Unconfirmed.fBuffer = AppData;
			mcpsReq.Req.Unconfirmed.fBufferSize = AppDataSize;
			mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
		}
		else
		{
			printf("confirmed uplink sending ...\r\n");
			mcpsReq.Type = MCPS_CONFIRMED;
			mcpsReq.Req.Confirmed.fPort = AppPort;
			mcpsReq.Req.Confirmed.fBuffer = AppData;
			mcpsReq.Req.Confirmed.fBufferSize = AppDataSize;
			mcpsReq.Req.Confirmed.NbTrials = ConfirmedNbTrials;
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
			DeviceState = DEVICE_STATE_SEND;
			NextTx = true;
		}
		else
		{
			// Network not joined yet. Try to join again
			MlmeReq_t mlmeReq;
			mlmeReq.Type = MLME_JOIN;
			mlmeReq.Req.Join.DevEui = DevEui;
			mlmeReq.Req.Join.AppEui = AppEui;
			mlmeReq.Req.Join.AppKey = AppKey;

			if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
			{
				DeviceState = DEVICE_STATE_SLEEP;
			}
			else
			{
				DeviceState = DEVICE_STATE_CYCLE;
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
	NextTx = true;
}

#if(LoraWan_RGB==1)
void RGB_ON(uint32_t color,uint32_t time)
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

void RGB_OFF(void)
{
	RGB_ON(0,0);
	digitalWrite(Vext,HIGH);
}
#endif

/*  get the BatteryVoltage in mV. */
uint16_t GetBatteryVoltage(void)
{
	pinMode(ADC_CTL,OUTPUT);
	digitalWrite(ADC_CTL,LOW);
	uint16_t volt=analogRead(ADC)*2;
	digitalWrite(ADC_CTL,HIGH);
	return volt;
}


void __attribute__((weak)) DownLinkDataHandle(McpsIndication_t *mcpsIndication)
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
	RGB_ON(COLOR_RECEIVED, 200);
	RGB_OFF();
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
		DownLinkDataHandle(mcpsIndication);
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
				RGB_ON(COLOR_JOINED,500);
				RGB_OFF();
#endif
				printf("joined\r\n");
				
				//in PassthroughMode,do nothing while joined
				if(PassthroughMode == false)
				{
					// Status is OK, node has joined the network
					DeviceState = DEVICE_STATE_SEND;
				}
			}
			else
			{
				uint32_t rejoin_delay = 30000;
				printf("join failed\r\n");
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
	NextTx = true;
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
	int8 batlevel = ((GetBatteryVoltage()-3.7)/(4.2-3.7))*100;
	return batlevel;
}

LoRaMacPrimitives_t LoRaMacPrimitive;
LoRaMacCallback_t LoRaMacCallback;

void LoRaWanClass::Init(DeviceClass_t CLASS,LoRaMacRegion_t REGION)
{

	MibRequestConfirm_t mibReq;

	LoRaMacPrimitive.MacMcpsConfirm = McpsConfirm;
	LoRaMacPrimitive.MacMcpsIndication = McpsIndication;
	LoRaMacPrimitive.MacMlmeConfirm = MlmeConfirm;
	LoRaMacPrimitive.MacMlmeIndication = MlmeIndication;
	LoRaMacCallback.GetBatteryLevel = BoardGetBatteryLevel;
	LoRaMacCallback.GetTemperatureLevel = NULL;
	LoRaMacInitialization( &LoRaMacPrimitive, &LoRaMacCallback,REGION);
	TimerStop( &TxNextPacketTimer );
	TimerInit( &TxNextPacketTimer, OnTxNextPacketTimerEvent );

	mibReq.Type = MIB_ADR;
	mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
	LoRaMacMibSetRequestConfirm( &mibReq );

	mibReq.Type = MIB_PUBLIC_NETWORK;
	mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
	LoRaMacMibSetRequestConfirm( &mibReq );


	lwan_dev_params_update();

	DeviceState = DEVICE_STATE_JOIN;
}

extern "C" void GetNetInfo(void);


void LoRaWanClass::Join()
{	
	if( OVER_THE_AIR_ACTIVATION )
	{
		Serial.println("joining...");
		MlmeReq_t mlmeReq;
		
		mlmeReq.Type = MLME_JOIN;

		mlmeReq.Req.Join.DevEui = DevEui;
		mlmeReq.Req.Join.AppEui = AppEui;
		mlmeReq.Req.Join.AppKey = AppKey;
		if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
		{
			DeviceState = DEVICE_STATE_SLEEP;
		}
		else
		{
			DeviceState = DEVICE_STATE_CYCLE;
		}
	}
	else
	{
		MibRequestConfirm_t mibReq;

		mibReq.Type = MIB_NET_ID;
		mibReq.Param.NetID = LORAWAN_NETWORK_ID;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_DEV_ADDR;
		mibReq.Param.DevAddr = DevAddr;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_NWK_SKEY;
		mibReq.Param.NwkSKey = NwkSKey;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_APP_SKEY;
		mibReq.Param.AppSKey = AppSKey;
		LoRaMacMibSetRequestConfirm( &mibReq );

		mibReq.Type = MIB_NETWORK_JOINED;
		mibReq.Param.IsNetworkJoined = true;
		LoRaMacMibSetRequestConfirm( &mibReq );
		
		DeviceState = DEVICE_STATE_SEND;
	}
}

void LoRaWanClass::Send()
{
	if( NextTx == true )
	{
		MibRequestConfirm_t mibReq;
		mibReq.Type = MIB_DEVICE_CLASS;
		LoRaMacMibGetRequestConfirm( &mibReq );
		
		if(CLASS == CLASS_C)
		{
			if( mibReq.Param.Class!= CLASS_C )
			{
				mibReq.Param.Class = CLASS_C;
				LoRaMacMibSetRequestConfirm( &mibReq );
			}
		}

		if(CLASS == CLASS_A)
		{
			if( mibReq.Param.Class!= CLASS_A )
			{
				mibReq.Param.Class = CLASS_A;
				LoRaMacMibSetRequestConfirm( &mibReq );
			}
		}
		
		NextTx = SendFrame( );
	}
}

void LoRaWanClass::Cycle(uint32_t dutycycle)
{
	TimerSetValue( &TxNextPacketTimer, dutycycle );
	TimerStart( &TxNextPacketTimer );
}

void LoRaWanClass::Sleep()
{
	LowPower_Handler( );
	// Process Radio IRQ
	Radio.IrqProcess( );
}

void LoRaWanClass::Ifskipjoin()
{
//if saved net info is OK in lorawan mode, skip join.
	if(CheckNetInfo()&&Mode_LoraWan){
		Serial.println();
		if(PassthroughMode==false)
		{
			Serial.println("Wait 3s for user key to rejoin network");
			uint16_t i=0;
			pinMode(GPIO7,INPUT);
			while(i<=3000)
			{
				if(digitalRead(GPIO7)==LOW)//if user key down, rejoin network;
				{
					NetInfoDisable();
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

		Init(CLASS,REGION);
		GetNetInfo();
		if(PassthroughMode==false){
			Serial.println("User key not detected,Use reserved Net");
		}
		else{
			Serial.println("Use reserved Net");
		}
		if(PassthroughMode==false)
		{
			int32_t temp=randr(0,APP_TX_DUTYCYCLE);
			Serial.println();
			Serial.printf("Next packet send %d ms later(random time from 0 to APP_TX_DUTYCYCLE)\r\n",temp);
			Serial.println();
			Cycle(temp);//send packet in a random time to avoid network congestion.
		}
		DeviceState = DEVICE_STATE_SLEEP;
	}
}


LoRaWanClass LoRaWAN;

