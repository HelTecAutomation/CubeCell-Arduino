#include "LoRaWan_APP.h"
#include "Arduino.h"

/* OTAA para*/
uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;

uint8_t confirmedNbTrials = 4;

const int soilMoisturePin = 1;  

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
  int sensorValue = analogRead(soilMoisturePin);  
  Serial.print("ADC Value: ");
  Serial.println(sensorValue);
  
   appDataSize = 0; 
   unsigned char*puc;
   appData[appDataSize++] = 0x00; // parent ID
   appData[appDataSize++] = 0x00; // parent ID
   appData[appDataSize++] = 0x05; // sensor length
   appData[appDataSize++] = 0x00; 

   puc = (unsigned char *)(&sensorValue); 
   appData[appDataSize++] = puc[3];
   appData[appDataSize++] = puc[2];
   appData[appDataSize++] = puc[1];
   appData[appDataSize++] = puc[0];

   Serial.print("Sending data: ");
   for (int i = 0; i < appDataSize; i++) {
     Serial.print(appData[i], HEX);
     Serial.print(" ");
        }
    
  if (sensorValue < 300) {
    Serial.println("🌱 The soil is too dry, water it quickly！");
  } 
  else if (sensorValue >= 300 && sensorValue <= 700) {
    Serial.println("🌼 The soil moisture is just right, no watering is needed");
  } 
  else {
    Serial.println("🚫 The soil is too wet, be careful of rotting the roots");
  }


}

bool waterPumpState = false;
uint8_t command = 0;  // To store received command
void downLinkDataHandle(McpsIndication_t *mcpsIndication) {
  if (mcpsIndication->BufferSize > 0) {
    // Print the received downlink data
    Serial.print("+Received Buffer: ");
    for (uint8_t i = 0; i < mcpsIndication->BufferSize; i++) {
      Serial.printf("0x%02X ", mcpsIndication->Buffer[i]);
    }
    Serial.println();

    // Extract the first byte as a command
    uint8_t command = mcpsIndication->Buffer[0];
    Serial.print("Received command: 0x");
    Serial.println(command, HEX);

    // Process according to commands
    switch (command) {
      case 0x01:
        Serial.println("Command 0x01: Turn on the water pump...");
        waterPumpState = true;
        break;

      case 0x02:
        Serial.println("Command 0x02: Turn off the water pump");
        waterPumpState = false;
        break;

      default:
        Serial.println("Unknown command received.");  
        break;
    }
  } else {
    Serial.println("No data received.");
  }
}

void setup() {
	Serial.begin(115200);
  pinMode(P0_1, OUTPUT);
  digitalWrite(P0_1, LOW);
}

void loop()
{
	switch( deviceState )
	{
		case DEVICE_STATE_INIT:
		{
#if(LORAWAN_DEVEUI_AUTO)
			LoRaWAN.generateDeveuiByChipID();
#endif
#if(AT_SUPPORT)
			getDevParam();
#endif
			printDevParam();
			LoRaWAN.init(loraWanClass,loraWanRegion);
			deviceState = DEVICE_STATE_JOIN;
			break;
		}
		case DEVICE_STATE_JOIN:
		{
			LoRaWAN.join();
			break;
		}
		case DEVICE_STATE_SEND:
		{
			prepareTxFrame( appPort );
      pinMode(P0_1, OUTPUT);
      if(waterPumpState) {
        digitalWrite(P0_1, HIGH);  // Turn on the water pump
        Serial.println("Water pump is ON.");
      } else {
        digitalWrite(P0_1, LOW);   // Turn off the water pump
        Serial.println("Water pump is OFF.");
      }
			LoRaWAN.send();
			deviceState = DEVICE_STATE_CYCLE;
			break;
		}
		case DEVICE_STATE_CYCLE:
		{
			// Schedule next packet transmission
			txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			deviceState = DEVICE_STATE_SLEEP;
			break;
		}
		case DEVICE_STATE_SLEEP:
		{ 
			LoRaWAN.sleep();
			break;
		}
		default:
		{
			deviceState = DEVICE_STATE_INIT;
			break;
		}
	}
}
