/* Heltec Automation Ping Pong communication test example
 *
 * Function:
 * 1. CubeCell read battery voltage and transmit via LoRa.
 * 
 * Description:
 * 1. Only hardware layer communicate, no LoRaWAN protocol support;
 * 2. This examplese can communicate with other ESP32 LoRa device,
 *    WiFi LoRa 32, Wireless Stick, Wireless Stick Lite, etc. 
 * 3. This example is for CubeCell hardware basic test.
 *
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * https://heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/ASR650x-Arduino
 * */

#include "LoRaWan_APP.h"
#include "Arduino.h"

/*
 * set LoraWan_RGB to 1,the RGB active in loraWan
 * RGB red means sending;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY                                868000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       8         // [SF7..SF12]
#define LORA_CODINGRATE                             4         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 30 // Define the payload size here

char txPacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );



typedef enum
{
    LOWPOWER,
    ReadVoltage,
    TX
}States_t;

States_t state;
bool sleepMode = false;
int16_t rssi,rxSize;
uint16_t voltage;

void setup() {
    Serial.begin(115200);

    voltage = 0;
    rssi=0;

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;

    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    state=ReadVoltage;
}



void loop()
{
  switch(state)
  {
    case TX:
    {
      sprintf(txPacket,"%s","ADC_battery: ");
      sprintf(txPacket+strlen(txPacket),"%d",voltage);
      turnOnRGB(COLOR_SEND,0);
      Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txPacket, strlen(txPacket));
      Radio.Send( (uint8_t *)txPacket, strlen(txPacket) );
      state=LOWPOWER;
      break;
    }
    case LOWPOWER:
    {
      lowPowerHandler();
      delay(100);
      turnOffRGB();
      delay(2000);  //LowPower time
      state = ReadVoltage; 
      break;
    }
    case ReadVoltage:
    {
      pinMode(VBAT_ADC_CTL,OUTPUT);
      digitalWrite(VBAT_ADC_CTL,LOW);
      voltage=analogRead(ADC)*2;

      /*
       * Board, BoardPlus, Capsule, GPS and HalfAA variants
       * have external 10K VDD pullup resistor
       * connected to GPIO7 (USER_KEY / VBAT_ADC_CTL) pin
       */
      pinMode(VBAT_ADC_CTL, INPUT);

      state = TX;
      break;
    }
     default:
          break;
  }
  Radio.IrqProcess();
}

void OnTxDone( void )
{
  Serial.print("TX done!");
  turnOnRGB(0,0);
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.print("TX Timeout......");
    state=ReadVoltage;
}
