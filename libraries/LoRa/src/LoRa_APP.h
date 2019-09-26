#ifndef LoRa_APP_H
#define LoRa_APP_H

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
#include "Arduino.h"

extern enum eDeviceState_Lora DeviceState_lora;
extern uint8_t Lora_TXPW;
extern uint32_t Lora_SF; 
extern uint32_t Lora_FREQ; 
extern uint32_t LoraRxTimeout;
extern bool PrintMode;

class LoRaClass{
public:
  void Init(uint32_t freq, uint8_t power,uint32_t datarate);
  void Send();
  void Receive();
  void LOWPOWER();
};

extern LoRaClass LoRa;

#endif

