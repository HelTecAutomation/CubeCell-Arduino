#include <LoRa_APP.h>


enum eDeviceState_Lora deviceState_lora;
extern uint8_t appData[LORAWAN_APP_DATA_MAX_SIZE];
extern uint8_t appDataSize;

//Tx Power
uint8_t Lora_TXPW = 10;

//LORA_SPREADING_FACTOR
uint32_t Lora_SF = 7;

//LORA FREQUENCY
uint32_t Lora_FREQ = 470000000; 

//bandwidth:[0:125KHz;1:250KHz;2:500KHz]
uint32_t Lora_BW = 0;

//coderate:[1:4/5; 2:4/6; 3:4/7; 4:4/8]
uint32_t Lora_coderate = 1; 

uint16_t lora_preamblelth = 8;

bool lora_iqInvert = false;

bool lora_crc = true;

/*  receive date print mode
 *  true: hex
 *  false: string;
 */
bool PrintMode = true;

//Rx timeout
uint32_t LoraRxTimeout = 3000;

static RadioEvents_t LoraEvents;
static void OnLoraTxDone( void );
static void OnLoraTxTimeout( void );
static void OnLoraRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
static void OnLoraRxTimeout( void );

static void OnLoraTxDone( void )
{
	Radio.Sleep( );
	Serial.println("TX done");
}

static void OnLoraTxTimeout( void )
{
	Radio.Sleep();
	Serial.println("TX timeout");
}


static void OnLoraRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	Radio.Sleep( );
	if(PrintMode==false)
	{
		Serial.print("Received String:");
		for(int i=0;i<size;i++)
		{
			Serial.printf("%c",*payload++);
		}
		Serial.println();
	}
	else
	{
		Serial.print("Received Hex:");
		for(int i=0;i<size;i++)
		{
			Serial.printf("%02X",*payload++);
		}
		Serial.println();
	}
	Serial.printf("RSSI:%d, SNR:%d, Size:%d\r\n",rssi,snr,size);
}

static void OnLoraRxTimeout( void )
{
	Radio.Sleep();
	Serial.println("RX Timeout");
}

static void OnLoraRxError( void )
{
	Radio.Sleep();
	Serial.println("Rx error");
}

void LoRaClass::init()
{
	LoraEvents.TxDone = OnLoraTxDone;
	LoraEvents.TxTimeout = OnLoraTxTimeout;
	LoraEvents.RxDone = OnLoraRxDone;
	LoraEvents.RxTimeout = OnLoraRxTimeout;
	LoraEvents.RxError = OnLoraRxError;
	Radio.Init( &LoraEvents );
	Radio.SetChannel( Lora_FREQ );
	Radio.SetTxConfig( MODEM_LORA, (int8_t)Lora_TXPW, 0, Lora_BW,Lora_SF, Lora_coderate,lora_preamblelth, false,lora_crc, 0, 0, lora_iqInvert, 5000 );
	Radio.SetRxConfig( MODEM_LORA, Lora_BW, Lora_SF,Lora_coderate, 0, lora_preamblelth,0, false,0, lora_crc, 0, 0, lora_iqInvert, true );
	Radio.Sleep();
}

void LoRaClass::send()
{
	Radio.Send( appData, appDataSize );
}

void LoRaClass::receive()
{
	Radio.Rx(LoraRxTimeout);
}

void LoRaClass::lowpower()
{
	TimerLowPowerHandler( );
	Radio.IrqProcess( );
}


LoRaClass LoRa;


