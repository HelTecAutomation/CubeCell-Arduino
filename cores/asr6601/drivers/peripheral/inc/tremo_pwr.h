/**
 ******************************************************************************
 * @file    tremo_pwr.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the PWR firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup PWR
 * @{
 */

#ifndef __TREMO_PWR_H
#define __TREMO_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tremo_regs.h"

#define PWR_LP_MODE_STOP0   ((uint32_t)0x00000000)  /*!< STOP0 Mode */
#define PWR_LP_MODE_STOP1   ((uint32_t)0x00000001)  /*!< STOP1 Mode */
#define PWR_LP_MODE_STOP2   ((uint32_t)0x00000002)  /*!< STOP2 Mode */
#define PWR_LP_MODE_STOP3   ((uint32_t)0x00000003)  /*!< STOP3 Mode */
#define PWR_LP_MODE_STANDBY ((uint32_t)0x00000004)  /*!< STANDBY Mode */

// deepsleep
void pwr_deepsleep_wfi(uint32_t mode);
void pwr_deepsleep_wfe(uint32_t mode);

// sleep
void pwr_sleep_wfi(bool lowpower);
void pwr_sleep_wfe(bool lowpower);

// lprun
void pwr_enter_lprun_mode(void);
void pwr_exit_lprun_mode(void);

//lowpower xo32k
void pwr_xo32k_lpm_cmd(bool new_state);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_PWR_H

/**
 * @} 
 * @}
 */
