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
#include <HardwareSerial.h>


extern uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];
extern uint8_t AppDataSize;
extern uint8_t AppPort;
extern uint32_t TxDutyCycleTime;
extern bool OVER_THE_AIR_ACTIVATION;
extern LoRaMacRegion_t REGION;
extern bool LORAWAN_ADR_ON;
extern bool IsTxConfirmed;
extern uint32_t APP_TX_DUTYCYCLE;
extern DeviceClass_t  CLASS;
extern uint8_t AppPort;
extern bool PassthroughMode;
extern uint8_t ConfirmedNbTrials;
extern bool Mode_LoraWan;
extern bool KeepNet;

/*!
 * Defines a random delay for application data transmission duty cycle. 1s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        1000

extern enum eDeviceState_LoraWan DeviceState;

class LoRaWanClass{
public:
  void Init(DeviceClass_t CLASS,LoRaMacRegion_t REGION);
  void Join();
  void Send();
  void Cycle(uint32_t dutycycle);
  void Sleep();
  void Ifskipjoin();
};


extern "C" bool SendFrame( void );
extern "C" void RGB_ON(uint32_t color,uint32_t time);
extern "C" void RGB_OFF(void);
extern "C" uint16_t GetBatteryVoltage(void);



extern LoRaWanClass LoRaWAN;

#endif
