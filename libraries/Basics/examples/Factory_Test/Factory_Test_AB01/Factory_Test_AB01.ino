#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "HT_SSD1306Wire.h"
#include "Wire.h"
/*
 * set LoraWan_RGB to 1,the RGB active
 * RGB red means sending;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

SSD1306Wire display(0x3c,500000, SDA, SCL);

#define RF_FREQUENCY                                868000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 30 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
void displayInof();
void sleep(void);
void testRGB(void);
void gpioOn();

typedef enum
{
    LOWPOWER,
    RX,
    TX
}States_t;

int16_t txNumber;
States_t state;
bool sleepMode = false;
int16_t Rssi,rxSize;



void setup() {
    Serial.begin(115200);
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW); 
    delay(100);
    display.init();
    //display.flipScreenVertically();
    
    testRGB();
    txNumber=0;
    Rssi=0;

    pinMode(P3_3,INPUT);
    attachInterrupt(P3_3,sleep,FALLING);
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxDone = OnRxDone;

    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

    state=TX;
}



void loop()
{
	switch(state)
	{
		case TX:
			delay(1000);
			txNumber++;
		    sprintf(txpacket,"%s","hello");
		    sprintf(txpacket+strlen(txpacket),"%d",txNumber);
		    sprintf(txpacket+strlen(txpacket),"%s"," rssi : ");
		    sprintf(txpacket+strlen(txpacket),"%d",Rssi);
		    turnOnRGB(0x100000,0);

		    Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));

		    Radio.Send( (uint8_t *)txpacket, strlen(txpacket) );
		    state=LOWPOWER;
		    break;
		case RX:
			Serial.println("into RX mode");
		    Radio.Rx( 0 );
		    state=LOWPOWER;
		    break;
		case LOWPOWER:
			if(sleepMode)
			{
				Radio.Sleep( );
				Wire.end();
				detachInterrupt(RADIO_DIO_1);
				turnOffRGB();
			    pinMode(GPIO0,ANALOG);
			    pinMode(GPIO1,ANALOG);
			    pinMode(GPIO2,ANALOG);
			    pinMode(GPIO3,ANALOG);
			    pinMode(GPIO4,ANALOG);
			    pinMode(GPIO5,ANALOG);
			    pinMode(Vext,ANALOG);
			    pinMode(ADC,ANALOG);
			}
			lowPowerHandler();
		    break;
        default:
            break;
	}
    Radio.IrqProcess( );
}

void OnTxDone( void )
{
	Serial.print("TX done......");
    displayInof();
	turnOnRGB(0,0);
	state=RX;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.print("TX Timeout......");
    //state=TX;
}
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	gpioOn();
    Rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    turnOnRGB(0x001000,100);
    turnOnRGB(0,0);
    Radio.Sleep( );

    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,Rssi,rxSize);
    Serial.println("wait to send next packet");
    displayInof();

    state=TX;
}

void displayInof()
{
    display.clear();
    display.drawString(0, 50, "Packet " + String(txNumber,DEC) + " sent done");
    display.drawString(0, 0,  "Received Size" + String(rxSize,DEC) + " packages:");
    display.drawString(0, 15, rxpacket);
    display.drawString(0, 30, "With rssi " + String(Rssi,DEC));
    display.display();
}


void sleep(void)
{
	delay(10);
	if(digitalRead(P3_3)==0)
	{
		sleepMode = true;
	}
}

void testRGB(void)
{
	display.drawString(0, 20, "RGB Testing");
	display.display();
	for(uint32_t i=0;i<=30;i++)
	{
		turnOnRGB(i<<16,10);
	}
	for(uint32_t i=0;i<=30;i++)
	{
		turnOnRGB(i<<8,10);
	}
	for(uint32_t i=0;i<=30;i++)
	{
		turnOnRGB(i,10);
	}
	turnOnRGB(0,0);
}

void gpioOn(void)
{
    pinMode(GPIO0,OUTPUT);
    pinMode(GPIO1,OUTPUT);
    pinMode(GPIO2,OUTPUT);
    pinMode(GPIO3,OUTPUT);
    pinMode(GPIO4,OUTPUT);
    pinMode(GPIO5,OUTPUT);
    digitalWrite(GPIO0,HIGH);
    digitalWrite(GPIO1,HIGH);
    digitalWrite(GPIO2,HIGH);
    digitalWrite(GPIO3,HIGH);
    digitalWrite(GPIO4,HIGH);    
    digitalWrite(GPIO5,HIGH);
}
