#ifndef __AT_COMMAND_H__
#define __AT_COMMAND_H__

#include "hw.h"
#include "low_power.h"
#include "lorawan_port.h"

#ifdef __cplusplus
extern "C" {
#endif

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


void Enable_AT(void);
void getDevParam(void);
void printDevParam(void);

#ifdef __cplusplus
}
#endif

#endif



