#ifndef LoRaWanMinimal_APP_H
#define LoRaWanMinimal_APP_H

#include <stdio.h>
#include "utilities.h"
#include "LoRaWan_102.h"
#include "HardwareSerial.h"
#include "Arduino.h"
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


extern uint16_t userChannelsMask[6];

class LoRaWanMinimal {
public:
  void begin(DeviceClass_t lorawanClass, LoRaMacRegion_t region);
  void setSubBand2();

  bool joinOTAA(uint8_t* appEui, uint8_t* appKey);
  bool joinOTAA(uint8_t* appEui, uint8_t* appKey, uint8_t* devEui);
  bool joinABP(uint8_t* nwkSKey, uint8_t* appSKey, uint32_t devAddr);
  bool isJoined() {return itsJoined;};
  void setJoined(bool joined) {itsJoined=joined;}

  bool send(uint8_t datalen, uint8_t *data, uint8_t port, bool confirmed);

  void setAdaptiveDR(bool useadr) {itsUseADR=useadr;}  
  void setFixedDR(int8_t dr) {itsUseADR=false; itsDRForNoAdr=dr;}

private:
  uint8_t* generateDevEUIByChipID();
  bool itsUseADR=true;
#ifdef REGION_US915
  int8_t itsDRForNoAdr = 3;
#else
  int8_t itsDRForNoAdr = 5;
#endif
  uint8_t itsNumRetries=4;
  bool itsJoined=false;
  DeviceClass_t itsDeviceClass;
  LoRaMacRegion_t itsRegion;
};

extern "C" void turnOnRGB(uint32_t color,uint32_t time);
extern "C" void turnOffRGB(void);
extern "C" void downLinkDataHandle(McpsIndication_t *mcpsIndication);
extern "C" void lwan_dev_params_update( void );
extern "C" void dev_time_updated( void );

extern LoRaWanMinimal LoRaWAN;

#endif
