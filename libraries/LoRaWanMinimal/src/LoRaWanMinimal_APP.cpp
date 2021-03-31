#include <LoRaWanMinimal_APP.h>

#if(AT_SUPPORT)
#error "The LoRaWanMinimal library is configured for LORAWAN_AT_SUPPORT=OFF. Please disable."
#endif

#if defined( REGION_EU868 )
#include "loramac/region/RegionEU868.h"
#elif defined( REGION_EU433 )
#include "loramac/region/RegionEU433.h"
#elif defined( REGION_KR920 )
#include "loramac/region/RegionKR920.h"
#elif defined( REGION_AS923) || defined( REGION_AS923_AS1) || defined( REGION_AS923_AS2)
#include "loramac/region/RegionAS923.h"
#endif



#if(LoraWan_RGB==1)
#include "CubeCell_NeoPixel.h"
CubeCell_NeoPixel pixels(1, RGB, NEO_GRB + NEO_KHZ800);
#endif

/*loraWan default Dr when adr disabled*/
int8_t defaultDrForNoAdr;

/*loraWan current Dr when adr disabled*/
int8_t currentDrForNoAdr;


//Callback definitions
static void MlmeIndication( MlmeIndication_t *mlmeIndication );
static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm );
static void McpsIndication( McpsIndication_t *mcpsIndication );
static void McpsConfirm( McpsConfirm_t *mcpsConfirm );

//LoRaMAC references
LoRaMacPrimitives_t LoRaMacPrimitive;
LoRaMacCallback_t LoRaMacCallback;
extern uint32_t LoRaMacState;

//Not used but required to link with CubeCellLib.a
bool keepNet=false;
//A 'do nothing' function for timer callbacks
static void wakeUpDummy() {}

//Do the initialisation
void LoRaWanMinimal::begin(DeviceClass_t loraWanClass,LoRaMacRegion_t region)
{
	itsDeviceClass=loraWanClass;
	itsRegion=region;
	Serial.print("\r\nLoRaWAN ");
	switch(itsRegion)
	{
		case LORAMAC_REGION_AS923_AS1:
			Serial.print("AS923(AS1:922.0-923.4MHz)");
			break;
		case LORAMAC_REGION_AS923_AS2:
			Serial.print("AS923(AS2:923.2-924.6MHz)");
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
	Serial.printf(" Class %X start!\r\n\r\n",itsDeviceClass+10);

	if(itsRegion == LORAMAC_REGION_AS923_AS1 || itsRegion == LORAMAC_REGION_AS923_AS2)
		itsRegion = LORAMAC_REGION_AS923;
	MibRequestConfirm_t mibReq;

	LoRaMacPrimitive.MacMcpsConfirm = McpsConfirm;
	LoRaMacPrimitive.MacMcpsIndication = McpsIndication;
	LoRaMacPrimitive.MacMlmeConfirm = MlmeConfirm;
	LoRaMacPrimitive.MacMlmeIndication = MlmeIndication;
	LoRaMacCallback.GetBatteryLevel = NULL;
	LoRaMacCallback.GetTemperatureLevel = NULL;
	LoRaMacInitialization( &LoRaMacPrimitive, &LoRaMacCallback, itsRegion);

	mibReq.Type = MIB_ADR;
	mibReq.Param.AdrEnable = itsUseADR;
	LoRaMacMibSetRequestConfirm( &mibReq );

	mibReq.Type = MIB_PUBLIC_NETWORK;
	mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
	LoRaMacMibSetRequestConfirm( &mibReq );

	lwan_dev_params_update();

	mibReq.Type = MIB_DEVICE_CLASS;
	LoRaMacMibGetRequestConfirm( &mibReq );
	
	if(itsDeviceClass != mibReq.Param.Class)
	{
		mibReq.Param.Class = loraWanClass;
		LoRaMacMibSetRequestConfirm( &mibReq );
	}
}

//Generate the DevEUI from the hardware ID
bool LoRaWanMinimal::joinOTAA(uint8_t* appEui, uint8_t* appKey)
{
    return joinOTAA(appEui, appKey, generateDevEUIByChipID());
}

bool LoRaWanMinimal::joinOTAA(uint8_t* appEui, uint8_t* appKey, uint8_t* devEui)
{
	MlmeReq_t mlmeReq;
		
	mlmeReq.Type = MLME_JOIN;

	mlmeReq.Req.Join.DevEui = devEui;
	mlmeReq.Req.Join.AppEui = appEui;
	mlmeReq.Req.Join.AppKey = appKey;
	mlmeReq.Req.Join.NbTrials = 1;

	if ( LoRaMacMlmeRequest( &mlmeReq ) != LORAMAC_STATUS_OK ) {
		return false;
	}

	//Wait until radio has finished trying to join
    TimerEvent_t pollStateTimer;
    TimerInit( &pollStateTimer, wakeUpDummy );
    TimerSetValue( &pollStateTimer, 100 );
	while (LoRaMacState!=LORAMAC_IDLE) {
	  TimerStart( &pollStateTimer );
	  lowPowerHandler( );
	  TimerStop( &pollStateTimer );
      Radio.IrqProcess( );
	}

	//Result will have been set via a callback
    return itsJoined;
}

bool LoRaWanMinimal::joinABP(uint8_t* nwkSKey, uint8_t* appSKey, uint32_t devAddr)
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

    return true;
}

bool LoRaWanMinimal::send(uint8_t datalen, uint8_t *data, uint8_t port, bool confirmed)
{
	MibRequestConfirm_t mibReq;
	mibReq.Type = MIB_DEVICE_CLASS;
	LoRaMacMibGetRequestConfirm( &mibReq );

	if (itsDeviceClass != mibReq.Param.Class) {
		mibReq.Param.Class = itsDeviceClass;
		LoRaMacMibSetRequestConfirm( &mibReq );
	}	

	lwan_dev_params_update();
	
	McpsReq_t mcpsReq;
	LoRaMacTxInfo_t txInfo;
	defaultDrForNoAdr=itsDRForNoAdr;
	
	if ( LoRaMacQueryTxPossible( datalen, &txInfo ) != LORAMAC_STATUS_OK ) {
		// Send empty frame in order to flush MAC commands
		printf("payload length error ...\r\n");
		mcpsReq.Type = MCPS_UNCONFIRMED;
		mcpsReq.Req.Unconfirmed.fBuffer = NULL;
		mcpsReq.Req.Unconfirmed.fBufferSize = 0;
		mcpsReq.Req.Unconfirmed.Datarate = currentDrForNoAdr;
	} else {
		if (confirmed)	{
			mcpsReq.Type = MCPS_CONFIRMED;
			mcpsReq.Req.Confirmed.fPort = port;
			mcpsReq.Req.Confirmed.fBuffer = data;
			mcpsReq.Req.Confirmed.fBufferSize = datalen;
			mcpsReq.Req.Confirmed.NbTrials = itsNumRetries;
			mcpsReq.Req.Confirmed.Datarate = currentDrForNoAdr;
		} else {
			mcpsReq.Type = MCPS_UNCONFIRMED;
			mcpsReq.Req.Unconfirmed.fPort = port;
			mcpsReq.Req.Unconfirmed.fBuffer = data;
			mcpsReq.Req.Unconfirmed.fBufferSize = datalen;
			mcpsReq.Req.Unconfirmed.Datarate = currentDrForNoAdr;
		}
	}
	uint32_t status=LoRaMacMcpsRequest( &mcpsReq );
	if( status != LORAMAC_STATUS_OK )
	{
		printf("LoRaWanMinimal_APP.send: Bad LoRaMacMcpsRequest status, couldn't send %d", status);
		return false;
	}

    TimerEvent_t pollStateTimer;
    TimerInit( &pollStateTimer, wakeUpDummy );
    TimerSetValue( &pollStateTimer, 100 );
	while (LoRaMacState!=LORAMAC_IDLE) {
	  TimerStart( &pollStateTimer );
	  lowPowerHandler( );
	  TimerStop( &pollStateTimer );
      Radio.IrqProcess( );
	}

	return true;		
}

void LoRaWanMinimal::setSubBand2()
{
	userChannelsMask[0]=0xFF00;
	/*// Enabling only 2nd block of 8 channels (8-15) + channel 65
	MibRequestConfirm_t mibReq;
	uint16_t channelMask[] = { 0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
	mibReq.Type = MIB_CHANNELS_MASK;
	mibReq.Param.ChannelsMask = channelMask;
	LoRaMacMibSetRequestConfirm( &mibReq );*/
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
				if (mcpsConfirm->AckReceived) {
					//printf("ACK was received");
				} else {
					//printf("ACK was NOT received");
				}
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
    //Leave overall management of Vext to the user
	//digitalWrite(Vext,HIGH);
}
#endif



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
	printf( "Downlink/ACK received: rssi = %d, snr = %d, datarate = %d\r\n", mcpsIndication->Rssi, (int)mcpsIndication->Snr,(int)mcpsIndication->RxDatarate);
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
		printf("Data is pending - should be sending again..");
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


void __attribute__((weak)) dev_time_updated()
{
	printf("device time updated\r\n");
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
                LoRaWAN.setJoined(true);
			} else {
                LoRaWAN.setJoined(false);
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
		case MLME_DEVICE_TIME:
		{
			if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
			{
				dev_time_updated();
			}
			break;
		}
		default:
			break;
	}
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
            printf("WARNING: MlmeIndication: MAC requested uplink ASAP");
			break;
		}
		default:
			break;
	}
}


void lwan_dev_params_update( void )
{
#if defined( REGION_EU868 )
	LoRaMacChannelAdd( 3, ( ChannelParams_t )EU868_LC4 );
	LoRaMacChannelAdd( 4, ( ChannelParams_t )EU868_LC5 );
	LoRaMacChannelAdd( 5, ( ChannelParams_t )EU868_LC6 );
	LoRaMacChannelAdd( 6, ( ChannelParams_t )EU868_LC7 );
	LoRaMacChannelAdd( 7, ( ChannelParams_t )EU868_LC8 );
#elif defined( REGION_EU433 )
	LoRaMacChannelAdd( 3, ( ChannelParams_t )EU433_LC4 );
	LoRaMacChannelAdd( 4, ( ChannelParams_t )EU433_LC5 );
	LoRaMacChannelAdd( 5, ( ChannelParams_t )EU433_LC6 );
	LoRaMacChannelAdd( 6, ( ChannelParams_t )EU433_LC7 );
	LoRaMacChannelAdd( 7, ( ChannelParams_t )EU433_LC8 );
#elif defined( REGION_KR920 )
	LoRaMacChannelAdd( 3, ( ChannelParams_t )KR920_LC4 );
	LoRaMacChannelAdd( 4, ( ChannelParams_t )KR920_LC5 );
	LoRaMacChannelAdd( 5, ( ChannelParams_t )KR920_LC6 );
	LoRaMacChannelAdd( 6, ( ChannelParams_t )KR920_LC7 );
	LoRaMacChannelAdd( 7, ( ChannelParams_t )KR920_LC8 );
#elif defined( REGION_AS923 ) || defined( REGION_AS923_AS1 ) || defined( REGION_AS923_AS2 )
	LoRaMacChannelAdd( 2, ( ChannelParams_t )AS923_LC3 );
	LoRaMacChannelAdd( 3, ( ChannelParams_t )AS923_LC4 );
	LoRaMacChannelAdd( 4, ( ChannelParams_t )AS923_LC5 );
	LoRaMacChannelAdd( 5, ( ChannelParams_t )AS923_LC6 );
	LoRaMacChannelAdd( 6, ( ChannelParams_t )AS923_LC7 );
	LoRaMacChannelAdd( 7, ( ChannelParams_t )AS923_LC8 );
#endif

	MibRequestConfirm_t mibReq;

	mibReq.Type = MIB_CHANNELS_DEFAULT_MASK;
	mibReq.Param.ChannelsMask = userChannelsMask;
	LoRaMacMibSetRequestConfirm(&mibReq);

	mibReq.Type = MIB_CHANNELS_MASK;
	mibReq.Param.ChannelsMask = userChannelsMask;
	LoRaMacMibSetRequestConfirm(&mibReq);
}

uint8_t* LoRaWanMinimal::generateDevEUIByChipID()
{
	uint8_t* devEui=new uint8_t[8];
    uint64_t chipID=getID();
	for(int i=7;i>=0;i--)
	{
		devEui[i] = (chipID>>(8*(7-i)))&0xFF;
	}
    return devEui;
}

LoRaWanMinimal LoRaWAN;

