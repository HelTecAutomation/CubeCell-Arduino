#include "LoRaWan_APP.h"
#include <Region.h>
#include "Arduino.h"

const char myDevEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char myAppKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// The interrupt pin is attached to D4/GPIO1
#define INT_PIN GPIO1

bool accelWoke = false;

DeviceClass_t  CLASS = LORAWAN_CLASS;

/*OTAA or ABP*/
bool OVER_THE_AIR_ACTIVATION = LORAWAN_NETMODE;

/* LoRaWAN Adaptive Data Rate */
bool LORAWAN_ADR_ON = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool KeepNet = LORAWAN_Net_Reserve;

/*LoraWan REGION*/
LoRaMacRegion_t REGION = ACTIVE_REGION;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool IsTxConfirmed = LORAWAN_UPLINKMODE;

/*!
  Number of trials to transmit the frame, if the LoRaMAC layer did not
  receive an acknowledgment. The MAC performs a datarate adaptation,
  according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
  to the following table:

  Transmission nb | Data Rate
  ----------------|-----------
  1 (first)       | DR
  2               | DR
  3               | max(DR-1,0)
  4               | max(DR-1,0)
  5               | max(DR-2,0)
  6               | max(DR-2,0)
  7               | max(DR-3,0)
  8               | max(DR-3,0)

  Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
  the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t ConfirmedNbTrials = 8;

/* Application port */
#define DEVPORT 2
#define APPPORT 1
uint8_t AppPort = 1;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = (24 * 60 * 60 * 1000); // 24h

/* Prepares the payload of the frame */
static bool prepareTxFrame( uint8_t port )
{
  int head;
  AppPort = port;
  switch (port) {
    case 1: // woke up from interrupt
      Serial.println("Sending data packet");
      AppDataSize = 1;//AppDataSize max value is 64
      AppData[0] = 0xFF; // set to something useful  
      break;
    case 2: // daily wake up
      Serial.println("Sending dev status packet");
      AppDataSize = 1;//AppDataSize max value is 64
      AppData[0] = 0xA0; // set to something else useful
      break;
  }
  return true;
}

extern uint8_t DevEui[];
extern uint8_t AppEui[];
extern uint8_t AppKey[];
extern bool IsLoRaMacNetworkJoined;

void accelWakeup()
{
  accelWoke = true;
}

void setup() {
  Serial.begin(115200);

  delay(200); // wait for stable
  accelWoke = false;

  memcpy(DevEui, myDevEui, sizeof(myDevEui));
  memcpy(AppEui, myAppEui, sizeof(myAppEui));
  memcpy(AppKey, myAppKey, sizeof(myAppKey));
  BoardInitMcu();
  
  DeviceState = DEVICE_STATE_INIT;
  LoRaWAN.Ifskipjoin();

  pinMode(INT_PIN, INPUT);
  attachInterrupt(INT_PIN, accelWakeup, RISING);
  Serial.println("Interrupts attached");
}

void loop()
{
  if (accelWoke) {
    uint32_t now = TimerGetCurrentTime();
    Serial.print(now); Serial.println("accel woke");
  }

  switch ( DeviceState )
  {
    case DEVICE_STATE_INIT:
      {
        Serial.printf("LoRaWan Class%X test start! \r\n", CLASS + 10);
#if(AT_SUPPORT)
        Enable_AT();
        getDevParam();
#endif
        printDevParam();
        LoRaWAN.Init(CLASS, REGION);
        DeviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
        LoRaWAN.Join();
        break;
      }
    case DEVICE_STATE_SEND: // a send is scheduled to occur, usu. daily status
      {
        prepareTxFrame( DEVPORT );
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
        if (accelWoke) {
          if (IsLoRaMacNetworkJoined) {
            if(prepareTxFrame(APPPORT)) {
              LoRaWAN.Send();
            }
          }
          accelWoke = false;
        }
        LoRaWAN.Sleep();
        Serial.println("LoRaWAN.Sleep() finished");
        break;
      }
    default:
      {
        DeviceState = DEVICE_STATE_INIT;
        break;
      }
  }
}
