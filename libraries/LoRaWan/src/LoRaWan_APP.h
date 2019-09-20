#ifndef LoRaWan_APP_H
#define LoRaWan_APP_H

#include <stdio.h>
#include "utilities.h"
#include "board.h"
#include "gpio.h"
#include "LoRaMac.h"
#include "Commissioning.h"
#include "hw.h"
#include "low_power.h"
#include "spi-board.h"
#include "rtc-board.h"
#include "asr_timer.h"
#include "sx126x.h"
#include "board-config.h"
#include "hw_conf.h"
#include <uart_port.h>
#include "AT_Command.h"


extern uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];
extern uint8_t AppDataSize;
extern uint8_t AppPort;
extern uint32_t TxDutyCycleTime;
extern bool OVER_THE_AIR_ACTIVATION;
extern bool LORAWAN_ADR_ON;
extern bool IsTxConfirmed;
extern uint32_t APP_TX_DUTYCYCLE;
extern DeviceClass_t  CLASS;
extern uint8_t AppPort;
extern bool PassthroughMode;
extern uint8_t ConfirmedNbTrials;
/*!
 * Defines a random delay for application data transmission duty cycle. 1s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        1000

/*!
 * Device states
 */
enum eDeviceState
{
    DEVICE_STATE_INIT,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_SEND,
    DEVICE_STATE_CYCLE,
    DEVICE_STATE_SLEEP
};

extern enum eDeviceState DeviceState;

class LoRaWanClass{
public:
  void Init(DeviceClass_t CLASS,LoRaMacRegion_t REGION);
  void Join();
  void Send();
  void Cycle(uint32_t dutycycle);
  void Sleep();
};
extern "C" bool SendFrame( void );
extern "C" void RGB_ON(uint32_t color,uint32_t time);
extern "C" void RGB_OFF(void);
extern LoRaWanClass LoRaWAN;

#endif
