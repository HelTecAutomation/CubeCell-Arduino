#include <LoRa_APP.h>


enum eDeviceState_Lora DeviceState_lora;
extern uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];
extern uint8_t AppDataSize;

//Tx Power
uint8_t Lora_TXPW = 10;

//LORA_SPREADING_FACTOR
uint32_t Lora_SF = 7;

//LORA FREQUENCY
uint32_t Lora_FREQ = 470000000; 

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
	Serial.println("TX Timeout");
}
static void OnLoraRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	Radio.Sleep( );
	if(PrintMode==false)
	{
		Serial.printf("Received String:%s\r\n",payload);
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
	Serial.printf("RSSI:%d, Size:%d\r\n",rssi,size);
}

static void OnLoraRxTimeout( void )
{
	Radio.Sleep();
	Serial.println("RX Timeout");
}


void LoRaClass::Init(uint32_t freq, uint8_t power,uint32_t datarate)
{
	LoraEvents.TxDone = OnLoraTxDone;
	LoraEvents.TxTimeout = OnLoraTxTimeout;
	LoraEvents.RxDone = OnLoraRxDone;
	LoraEvents.RxTimeout = OnLoraRxTimeout;
	Radio.Init( &LoraEvents );
	Radio.SetChannel( freq );
	Radio.SetTxConfig( MODEM_LORA, (int8_t)power, 0, 0,datarate, 1,8, false,true, 0, 0, false, 3000 );
	Radio.SetRxConfig( MODEM_LORA, 0, datarate,1, 0, 8,0, false,0, true, 0, 0, false, true );
}

void LoRaClass::Send()
{
	Radio.Send( AppData, AppDataSize );
}

void LoRaClass::Receive()
{
	Radio.Rx(LoraRxTimeout);
}

void LoRaClass::LOWPOWER()
{
	LowPower_Handler( );
	// Process Radio IRQ
	Radio.IrqProcess( );
}


LoRaClass LoRa;


