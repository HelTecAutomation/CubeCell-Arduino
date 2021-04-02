#ifndef LoRaWan_APP_H
#define LoRaWan_APP_H

#include <stdio.h>
#include "utilities.h"
#include "LoRaWan_102.h"
#include "HardwareSerial.h"
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

extern uint8_t devEui[];
extern uint8_t appEui[];
extern uint8_t appKey[];
extern uint8_t nwkSKey[];
extern uint8_t appSKey[];
extern uint32_t devAddr;
extern uint8_t appData[LORAWAN_APP_DATA_MAX_SIZE];
extern uint8_t appDataSize;
extern uint8_t appPort;
extern uint32_t txDutyCycleTime;
extern bool overTheAirActivation;
extern LoRaMacRegion_t loraWanRegion;
extern bool loraWanAdr;
extern bool isTxConfirmed;
extern uint32_t appTxDutyCycle;
extern DeviceClass_t loraWanClass;
extern bool passthroughMode;
extern uint8_t confirmedNbTrials;
extern bool modeLoraWan;
extern bool keepNet;
extern bool IsLoRaMacNetworkJoined;
extern uint16_t userChannelsMask[6];

/*!
 * Defines a random delay for application data transmission duty cycle. 1s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        1000

extern enum eDeviceState_LoraWan deviceState;

class LoRaWanClass{
public:
  void init(DeviceClass_t lorawanClass,LoRaMacRegion_t region);
  void join();
  void send();
  void cycle(uint32_t dutyCycle);
  void sleep();
  void setDataRateForNoADR(int8_t dataRate);
  void ifskipjoin();
  void generateDeveuiByChipID();

#if defined(CubeCell_BoardPlus)||defined(CubeCell_GPS)
  void displayJoining();
  void displayJoined();
  void displaySending();
  void displayAck();
  void displayMcuInit();
#endif
};


extern "C" bool SendFrame( void );
extern "C" void turnOnRGB(uint32_t color,uint32_t time);
extern "C" void turnOffRGB(void);
extern "C" bool checkUserAt(char * cmd, char * content);
extern "C" void downLinkAckHandle();
extern "C" void downLinkDataHandle(McpsIndication_t *mcpsIndication);
extern "C" void lwan_dev_params_update( void );
extern "C" void dev_time_updated( void );


extern LoRaWanClass LoRaWAN;

#endif
