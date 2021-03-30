/**
 ******************************************************************************
 * @file    tremo_iwdg.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the IWDG firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup IWDG
 * @{
 */

#ifndef __TREMO_IWDG_H
#define __TREMO_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "tremo_regs.h"

#define IWDG_MAX_RELOAD (0x0FFF)  /*!< The max value of the reload value */

/**
 * @brief The prescaler of IWDG
 */
typedef enum {
    IWDG_PRESCALER_4   = IWDG_CR_PREDIV_4,     /*!< Used to set prescaler to 4 */
    IWDG_PRESCALER_8   = IWDG_CR_PREDIV_8,     /*!< Used to set prescaler to 8 */
    IWDG_PRESCALER_16  = IWDG_CR_PREDIV_16,    /*!< Used to set prescaler to 16 */
    IWDG_PRESCALER_32  = IWDG_CR_PREDIV_32,    /*!< Used to set prescaler to 32 */
    IWDG_PRESCALER_64  = IWDG_CR_PREDIV_64,    /*!< Used to set prescaler to 64 */
    IWDG_PRESCALER_128 = IWDG_CR_PREDIV_128,   /*!< Used to set prescaler to 128 */
    IWDG_PRESCALER_256 = IWDG_CR_PREDIV_256    /*!< Used to set prescaler to 256 */
} iwdg_prescaler_t;

void iwdg_deinit(void);
void iwdg_init(bool auto_reset);
void iwdg_set_prescaler(iwdg_prescaler_t prescaler);
void iwdg_set_reload(uint32_t value);
void iwdg_set_window_value(uint32_t value);
void iwdg_reload(void);
void iwdg_start(void);
void iwdg_stop(void);
void iwdg_config_interrupt(bool new_state);
void iwdg_clear_interrupt(void);
void innerWdtEnable(bool autofeed);
void feedInnerWdt();

extern uint8_t idwgIrqCnt;
extern bool autoFeedIdwg;
#ifdef __cplusplus
}
#endif
#endif //__TREMO_IWDG_H

/**
 * @} 
 * @}
 */
