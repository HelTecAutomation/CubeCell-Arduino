#ifndef __AT_COMMAND_H__
#define __AT_COMMAND_H__

#ifdef __asr650x__
#include "hw.h"
#include "low_power.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ASR_Arduino.h"

/*!
 * Device states
 */
enum eDeviceState_LoraWan
{
    DEVICE_STATE_INIT,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_SEND,
    DEVICE_STATE_CYCLE,
    DEVICE_STATE_SLEEP
};

enum eDeviceState_Lora
{
    LORA_INIT,
    LORA_SEND,
    LORA_RECEIVE,
    MCU_SLEEP,
};


void enableAt(void);
void getDevParam(void);
void printDevParam(void);
void saveNetInfo(uint8_t *joinpayload, uint16_t size);
void saveUpCnt(void);
void saveDownCnt(void);
void getNetInfo(void);
void saveDr(void);
bool checkNetInfo(void);
void netInfoDisable(void);
extern bool checkUserAt(char * cmd, char * content);



#ifdef __cplusplus
}
#endif

#endif



