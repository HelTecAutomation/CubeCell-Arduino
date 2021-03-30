/**
 ******************************************************************************
 * @file    tremo_delay.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains the delay functions
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup DELAY
 * @{
 */

#ifndef __TREMO_DELAY_H
#define __TREMO_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tremo_cm4.h"


void delay_init(void);
void delayMicroseconds(uint32_t nus);
void delay(uint32_t nms);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_DELAY_H

/**
 * @} 
 * @}
 */
