/**
 ******************************************************************************
 * @file    tremo_wdg.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the WDG firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup WDG
 * @{
 */

#ifndef __TREMO_WDG_H
#define __TREMO_WDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tremo_regs.h"

#define WDG_LOCK_TOKEN 0x1ACCE551
#define WDG_RESEN      (1 << 1)
#define WDG_INTEN      1

void wdg_deinit(void);
void wdg_start(uint32_t reload_value);
void wdg_reload(void);
void wdg_stop(void);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_WDG_H

/**
 * @} 
 * @}
 */