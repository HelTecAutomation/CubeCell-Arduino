#ifndef LoRa_APP_H
#define LoRa_APP_H

#include <stdio.h>
#include "utilities.h"
#include "LoRaWan_102.h"
#include "Arduino.h"
#include "AT_Command.h"
#if defined(__asr650x__)
#include "board.h"
#include "gpio.h"
#include "hw.h"
#include "low_power.h"
#include "spi-board.h"
#include "rtc-board.h"
#include "asr_timer.h"
#include "board-config.h"
#include "hw_conf.h"
#include <uart_port.h>
#endif

extern enum eDeviceState_Lora deviceState_lora;
extern uint8_t Lora_TXPW;
extern uint32_t Lora_SF; 
extern uint32_t Lora_FREQ; 
extern uint32_t LoraRxTimeout;
extern bool PrintMode;
extern uint32_t Lora_BW;
extern uint32_t Lora_coderate; 
extern uint16_t lora_preamblelth;
extern bool lora_iqInvert;
extern bool lora_crc;

class LoRaClass{
public:
  void init();
  void send();
  void receive();
  void lowpower();
};

extern LoRaClass LoRa;

#endif

