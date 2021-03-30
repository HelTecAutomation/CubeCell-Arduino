/**
 ******************************************************************************
 * @file    tremo_lptimer.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of LPTIMER module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup LPTIMER
 * @{
 */

#ifndef __TREMO_LPTIMER_H
#define __TREMO_LPTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "tremo_regs.h"

/**
 * @brief LPTIMER status definition
 */
typedef enum {
    LPTIMER_ISR_CMPM    = 0x1,  /*!< Compare match*/
    LPTIMER_ISR_ARRM    = 0x2,  /*!< Autoreload match*/
    LPTIMER_ISR_EXTTRIG = 0x4,  /*!< External trigger edge event*/
    LPTIMER_ISR_CMPOK   = 0x8,  /*!< Compare register update OK*/
    LPTIMER_ISR_ARROK   = 0x10, /*!< Autoreload register update OK*/
    LPTIMER_ISR_UP      = 0x20, /*!< Counter direction change down to up*/
    LPTIMER_ISR_DOWN    = 0x40, /*!< Counter direction change up to down*/
    LPTIMER_ISR_CFGROK  = 0x80, /*!< CFGR register operation status*/
    LPTIMER_ISR_CROK    = 0x100 /*!< CR register register operation status*/
} lptimer_status_t;

/**
 * @brief LPTIMER interrupt definition
 */
typedef enum {
    LPTIMER_IT_CMPM    = 0x1,  /*!< Compare match interrupt*/
    LPTIMER_IT_ARRM    = 0x2,  /*!< Autoreload match interrupt*/
    LPTIMER_IT_EXTTRIG = 0x4,  /*!< External trigger edge event interrupt*/
    LPTIMER_IT_CMPOK   = 0x8,  /*!< Compare register update OK interrupt*/
    LPTIMER_IT_ARROK   = 0x10, /*!< Autoreload register update OK interrupt*/
    LPTIMER_IT_UP      = 0x20, /*!< Counter direction change down to up interrupt*/
    LPTIMER_IT_DOWN    = 0x40  /*!< Counter direction change up to down interrupt*/
} lptimer_it_t;

/**
 * @brief LPTIMER clear status success flag definition
 */
typedef enum {
    LPTIMER_CSR_CMPM    = 0x1,  /*!< Compare match*/
    LPTIMER_CSR_ARRM    = 0x2,  /*!< Autoreload match*/
    LPTIMER_CSR_EXTTRIG = 0x4,  /*!< External trigger edge event*/
    LPTIMER_CSR_UP      = 0x10, /*!< Counter direction change down to up*/
    LPTIMER_CSR_DOWN    = 0x20  /*!< Counter direction change up to down*/
} lptimer_clear_status_flag_t;

/**
 * @brief LPTIMER external trigger polarity
 */
typedef enum {
    LPTIMER_TRIGEN_SOFTWARE       = 0x0,     /*!< Software trigger*/
    LPTIMER_TRIGEN_RISING         = 0x20000, /*!< Rising edge*/
    LPTIMER_TRIGEN_FALLING        = 0x40000, /*!< Falling edge*/
    LPTIMER_TRIGEN_RISING_FALLING = 0x60000  /*!< Risig and falling edge*/
} lptimer_trigpolari_t;

/**
 * @brief LPTIMER trigger source
 */
typedef enum {
    LPTIMER_TRIGSEL_0 = 0x0,    /*!< External trigger etr*/
    LPTIMER_TRIGSEL_1 = 0x2000, /*!< External trigger comp0*/
    LPTIMER_TRIGSEL_2 = 0x4000, /*!< External trigger comp1*/
    LPTIMER_TRIGSEL_3 = 0x6000, /*!< External trigger rtc cyc*/
    LPTIMER_TRIGSEL_4 = 0x8000, /*!< External trigger rtc alarm0*/
    LPTIMER_TRIGSEL_5 = 0xa000, /*!< External trigger rtc alarm1*/
    LPTIMER_TRIGSEL_6 = 0xc000, /*!< External trigger gpio*/
    LPTIMER_TRIGSEL_7 = 0xe000  /*!< External trigger gpio*/
} lptimer_trigsel_t;

/**
 * @brief LPTIMER clock prescaler
 */
typedef enum {
    LPTIMER_PRESC_1   = 0x0,   /*!< 1 prescaler*/
    LPTIMER_PRESC_2   = 0x200, /*!< 2 prescaler*/
    LPTIMER_PRESC_4   = 0x400, /*!< 4 prescaler*/
    LPTIMER_PRESC_8   = 0x600, /*!< 8 prescaler*/
    LPTIMER_PRESC_16  = 0x800, /*!< 16 prescaler*/
    LPTIMER_PRESC_32  = 0xa00, /*!< 32 prescaler*/
    LPTIMER_PRESC_64  = 0xc00, /*!< 64 prescaler*/
    LPTIMER_PRESC_128 = 0xe00  /*!< 128 prescaler*/
} lptimer_presc_t;

/**
 * @brief LPTIMER trigger filter configuration
 */
typedef enum {
    LPTIMER_TRGFLT_NO = 0x0,  /*!< No filter*/
    LPTIMER_TRGFLT_2  = 0x40, /*!< Filter length is 2*/
    LPTIMER_TRGFLT_4  = 0x80, /*!< Filter length is 4*/
    LPTIMER_TRGFLT_8  = 0xc0  /*!< Filter length is 8*/
} lptimer_trgflt_t;

/**
 * @brief LPTIMER external clock filter configuration
 */
typedef enum {
    LPTIMER_CKFLT_NO = 0x0,  /*!< No filter*/
    LPTIMER_CKFLT_2  = 0x8,  /*!< Filter length is 2*/
    LPTIMER_CKFLT_4  = 0x10, /*!< Filter length is 4*/
    LPTIMER_CKFLT_8  = 0x18  /*!< Filter length is 8*/
} lptimer_ckflt_t;

/**
 * @brief LPTIMER clock polarity
 */
typedef enum {
    LPTIMER_CKPOL_RISING  = 0x0, /*!< Count by rising edge*/
    LPTIMER_CKPOL_FALLING = 0x2, /*!< Count by falling edge*/
    LPTIMER_CKPOL_BOTH    = 0x4, /*!< Count by both rising and falling edge*/
    LPTIMER_CKPOL_RESV    = 0x6  /*!< reserved*/
} lptimer_ckpol_t;

/**
 * @brief LPTIMER CFGR register definition
 */
typedef enum {
    LPTIMER_CFGR_CKFLT_EN  = 0x20,      /*!< External clock filter enable*/
    LPTIMER_CFGR_TRGFLT_EN = 0x100,     /*!< Trigger filter enable*/
    LPTIMER_CFGR_TIMEOUT   = 0x80000,   /*!< Timeout mode enable*/
    LPTIMER_CFGR_WAVE      = 0x100000,  /*!< Waveform shape*/
    LPTIMER_CFGR_WAVPOL    = 0x200000,  /*!< Waveform shape polarity*/
    LPTIMER_CFGR_PRELOAD   = 0x400000,  /*!< ARR and CMP register update mode*/
    LPTIMER_CFGR_COUNTMODE = 0x800000,  /*!< Counter mode selection*/
    LPTIMER_CFGR_ENC       = 0x1000000, /*!< Encoder mode enable*/
} lptimer_cfgr_t;

/**
 * @brief LPTIMER wakeup definition
 */
typedef enum {
    LPTIMER_CFGR_CMPM_WKUP    = 0x2000000,  /*!< CMPM wakeup enable*/
    LPTIMER_CFGR_ARRM_WKUP    = 0x4000000,  /*!< ARRM wakeup enable*/
    LPTIMER_CFGR_EXTTRIG_WKUP = 0x8000000,  /*!< EXTTRIG wakeup enable*/
    LPTIMER_CFGR_UP_WKUP      = 0x10000000, /*!< UP wakeup enable*/
    LPTIMER_CFGR_DOWN_WKUP    = 0x20000000, /*!< DOWN wakeup enable*/
    LPTIMER_CFGR_OUT_WKUP     = 0x40000000  /*!< OUT wakeup enable*/
} lptimer_wkup_t;

/**
 * @brief LPTIMER enable definition
 */
typedef enum {
    LPTIMER_CR_ENABLE = 0x1, /*!< LPTIMER enable*/
} lptimer_enable_t;

/**
 * @brief LPTIMER count mode definition
 */
typedef enum {
    LPTIMER_MODE_SNGSTRT = 0x2, /*!< LPTIMER start in single mode*/
    LPTIMER_MODE_CNTSTRT = 0x4  /*!< LPTIMER start in continuous mode*/
} lptimer_mode_t;

/**
 * @brief LPTIMER initialization configuration definition
 */
typedef struct {
    bool sel_external_clock;   /*!< Select external clock*/
    bool count_by_external;    /*!< Count by external clock*/
    lptimer_presc_t prescaler; /*!< Specifies the clock prescaler*/
    bool autoreload_preload;   /*!< auto-reload preload*/
    bool wavpol_inverted;      /*!< Invert output wave polarity*/
} lptimer_init_t;

void lptimer_init(lptimer_t* LPTIMERx, lptimer_init_t* lptimer_init);
void lptimer_deinit(lptimer_t* LPTIMERx);

void lptimer_cmd(lptimer_t* LPTIMERx, bool state);

void lptimer_enable_clock_filter(lptimer_t* LPTIMERx, uint8_t state);
void lptimer_enable_trigger_filter(lptimer_t* LPTIMERx, uint8_t state);

void lptimer_config_wakeup(lptimer_t* LPTIMERx, lptimer_wkup_t wkup, uint8_t state);
void lptimer_config_timeout(lptimer_t* LPTIMERx, uint8_t state);
void lptimer_config_wave(lptimer_t* LPTIMERx, uint8_t state);
void lptimer_config_encoder(lptimer_t* LPTIMERx, uint8_t state);

void lptimer_config_trigger_polarity(lptimer_t* LPTIMERx, lptimer_trigpolari_t config_trigpolari);
void lptimer_config_trigger_source(lptimer_t* LPTIMERx, lptimer_trigsel_t config_trigsel);
void lptimer_config_clock_prescaler(lptimer_t* LPTIMERx, lptimer_presc_t config_presc);
void lptimer_config_trigger_filter(lptimer_t* LPTIMERx, lptimer_trgflt_t config_trgflt);
void lptimer_config_clock_filter(lptimer_t* LPTIMERx, lptimer_ckflt_t config_ckflt);
void lptimer_config_clock_polarity(lptimer_t* LPTIMERx, lptimer_ckpol_t config_ckpol);

void lptimer_config_count_mode(lptimer_t* LPTIMERx, lptimer_mode_t count_mode, uint8_t state);

void lptimer_config_interrupt(lptimer_t* LPTIMERx, lptimer_it_t interrupt, uint8_t state);
void lptimer_clear_interrupt(lptimer_t* LPTIMERx, lptimer_it_t interrupt);
bool lptimer_get_interrupt_status(lptimer_t* LPTIMERx, lptimer_it_t interrupt);

void lptimer_set_arr_register(lptimer_t* LPTIMERx, uint16_t arr_value);
void lptimer_set_cmp_register(lptimer_t* LPTIMERx, uint16_t cmp_value);

bool lptimer_get_status(lptimer_t* LPTIMERx, lptimer_status_t status);

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_LPTIMER_H */

/**
 * @} 
 * @}
 */
