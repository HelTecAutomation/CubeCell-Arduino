/**
 ******************************************************************************
 * @file    tremo_bstimer.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of bstimer module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup BSTIMER
 * @{
 */

#ifndef __TREMO_BSTIMER_H
#define __TREMO_BSTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "tremo_regs.h"

/**
 * @brief BSTIMER CR1 register definition
 */
typedef enum {
    BSTIMER_CR1_CEN  = 0x1, /*!< Counter enable*/
    BSTIMER_CR1_UDIS = 0x2, /*!< Update disable*/
    BSTIMER_CR1_URS  = 0x4, /*!< Update request source*/
    BSTIMER_CR1_OPM  = 0x8, /*!< One pulse mode*/
    BSTIMER_CR1_ARPE = 0x80 /*!< Auto-Reload preload*/
} bstimer_cr1_t;

/**
 * @brief BSTIMER master mode selection
 */
typedef enum {
    BSTIMER_MMS_RESET  = 0x0,  /*!< EGR.UG bit is used as trigger output*/
    BSTIMER_MMS_ENABLE = 0x10, /*!< CR1.CEN bit is used as trigger output*/
    BSTIMER_MMS_UPDATE = 0x20, /*!< Update event is used as trigger output*/
    BSTIMER_MMS_RESV   = 0x70  /*!< Auto-Reload preload*/
} bstimer_mms_t;

/**
 * @brief BSTIMER DIER register definition
 */
typedef enum {
    BSTIMER_DIER_UIE = 0x1,  /*!< Update interrupt*/
    BSTIMER_DIER_UDE = 0x100 /*!< DMA request is triggered by the update event*/
} bstimer_dier_t;

/**
 * @brief BSTIMER SR register definition
 */
typedef enum {
    BSTIMER_SR_UIF = 0x1 /*!< Update interrupt flag*/
} bstimer_sr_t;

/**
 * @brief BSTIMER EGR register definition
 */
typedef enum {
    BSTIMER_EGR_UG = 0x1 /*!< Reinitialize the counter and generates an update
                            of the registers*/
} bstimer_egr_t;

/**
 * @brief BSTIMER init configuration structure definition
 */
typedef struct {
    uint32_t prescaler; /*!< Specifies the prescaler value used to divide the
                           TIMER clock. This parameter can be a number between
                           Min_Data = 0x0000 and Max_Data = 0xFFFF */

    uint32_t period; /*!< Specifies the period value to be loaded into the
                        active Auto-Reload Register at the next update event.
                        This parameter can be a number between
                        Min_Data = 0x0000 and Max_Data = 0xFFFF.  */

    bstimer_mms_t bstimer_mms; /*!< Master mode selection*/

    bool autoreload_preload; /*!< Specifies the auto-reload preload*/
    uint8_t RESV[3];         /*!< Reserved*/
    bool prescaler_change;
} bstimer_init_t;

void bstimer_init(bstimer_t* BSTIMERx, bstimer_init_t* bstimer_init);
void bstimer_deinit(bstimer_t* BSTIMERx);

void bstimer_cmd(bstimer_t* BSTIMERx, bool state);

void bstimer_config_one_pulse(bstimer_t* BSTIMERx, uint8_t state);
void bstimer_config_overflow_update(bstimer_t* BSTIMERx, uint8_t state);
void bstimer_config_update_disable(bstimer_t* BSTIMERx, uint8_t state);

void bstimer_config_dma(bstimer_t* BSTIMERx, uint8_t state);
void bstimer_config_interrupt(bstimer_t* BSTIMERx, uint8_t state);

bool bstimer_get_status(bstimer_t* BSTIMERx, bstimer_sr_t status);

void bstimer_generate_event(bstimer_t* BSTIMERx, bstimer_egr_t egr_event, uint8_t state);

#ifdef __cplusplus
}
#endif
#endif // _TREMO_BSTIMER_H_

/**
 * @} 
 * @}
 */
