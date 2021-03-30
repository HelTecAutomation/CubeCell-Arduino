/**
 ******************************************************************************
 * @file    tremo_timer.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of timer module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup TIMER
 * @{
 */
 
#ifndef __TREMO_TIMER_H
#define __TREMO_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "tremo_regs.h"

#define TIMER_CHANNEL_0 0x0 /*!< Capture/compare channel 1 identifier*/
#define TIMER_CHANNEL_1 0x1 /*!< Capture/compare channel 2 identifier*/
#define TIMER_CHANNEL_2 0x2 /*!< Capture/compare channel 3 identifier*/
#define TIMER_CHANNEL_3 0x3 /*!< Capture/compare channel 4 identifier*/

#define TIMER_DCR_DBL_POSITION 8 /*!< Timer DCR register DBL position*/

#define TIMER_OK     (0)  /*!< status ok*/
#define TIMER_ERROR  (-1) /*!< status error*/

/**
 * @brief TIMER count mode
 */
typedef enum {
    TIMER_COUNTERMODE_UP             = 0xFFFF, /*!< Counter used as up-counter*/
    TIMER_COUNTERMODE_DOWN           = 0x0,    /*!< Counter used as down-counter*/
    TIMER_COUNTERMODE_CENTERALIGNED1 = 0x20,   /*!< Center-aligned mode 1*/
    TIMER_COUNTERMODE_CENTERALIGNED2 = 0x40,   /*!< Center-aligned mode 2*/
    TIMER_COUNTERMODE_CENTERALIGNED3 = 0x60    /*!< Center-aligned mode 3*/
} timer_count_mode_t;

/**
 * @brief TIMER sample clock division
 */
typedef enum {
    TIMER_CKD_FPCLK_DIV1 = 0x0,   /*!< Clock division: fDTS=fpclk   */
    TIMER_CKD_FPCLK_DIV2 = 0x100, /*!< Clock division: fDTS=fpclk/2 */
    TIMER_CKD_FPCLK_DIV4 = 0x200, /*!< Clock division: fDTS=fpclk/4 */
    TIMER_CKD_FPCLK_RESV = 0x300  /*!< RESERVED */
} timer_ckd_t;

/**
 * @brief TIMER initialization configuration definition
 */
typedef struct {
    uint32_t prescaler; /*!< Specifies the prescaler value used to divide the
                           TIMER clock. This parameter can be a number between
                           Min_Data = 0x0000 and Max_Data = 0xFFFF */

    timer_count_mode_t counter_mode; /*!< Specifies the counter mode*/

    uint32_t period; /*!< Specifies the period value to be loaded into the
                        active Auto-Reload Register at the next update event.
                        This parameter can be a number between
                        Min_Data = 0x0000 and Max_Data = 0xFFFF.  */

    timer_ckd_t clock_division; /*!< Specifies the clock division*/

    bool autoreload_preload; /*!< Specifies the auto-reload preload*/
    uint8_t RESV;
} timer_init_t;

/**
 * @brief TIMER cr1 configuration
 */
typedef enum {
    TIMER_CEN                = 0x1,  /*!< Counter enable*/
    TIMER_UDIS               = 0x2,  /*!< Update disable*/
    TIMER_URS                = 0x4,  /*!< Update Request Source*/
    TIMER_OPM                = 0x8,  /*!< One Pulse Mode*/
    TIMER_DIR                = 0x10, /*!< Count direction*/
    TIMER_AUTORELOAD_PRELOAD = 0x80  /*!< Auto-Reload Preload*/
} timer_cr1_t;

/**
 * @brief TIMER master Mode Selection
 */
typedef enum {
    TIMER_TRGO_RESET  = 0x0,  /*!< TIMERx_EGR.UG bit is used as trigger output (TRGO)*/
    TIMER_TRGO_ENABLE = 0x10, /*!< TIMERx_CR1.CEN bit is used as trigger output (TRGO)*/
    TIMER_TRGO_UPDATE = 0x20, /*!< Update event is used as trigger output (TRGO)*/
    TIMER_TRGO_OC1    = 0x30, /*!< Capture or a compare match 1 is used as trigger output (TRGO)*/
    TIMER_TRGO_OC0REF = 0x40, /*!< OC0REF signal is used as trigger output (TRGO)*/
    TIMER_TRGO_OC1REF = 0x50, /*!< OC1REF signal is used as trigger output (TRGO)*/
    TIMER_TRGO_OC2REF = 0x60, /*!< OC2REF signal is used as trigger output (TRGO)*/
    TIMER_TRGO_OC3REF = 0x70  /*!< OC3REF signal is used as trigger output (TRGO)*/
} timer_master_mode_t;

/**
 * @brief TIMER cr2 configuration
 */
typedef enum {
    TIMER_CCDS_UPD = 0x8,  /*!< Capture/Compare DMA Selection*/
    TIMER_TI0S_XOR = 0x80, /*!< TI0 Input Selection*/
} timer_cr2_t;

/**
 * @brief TIMER ETR Prescaler
 */
typedef enum {
    TIMER_ETRPRESCALER_DIV1 = 0x0,    /*!< No prescaler is used*/
    TIMER_ETRPRESCALER_DIV2 = 0x1000, /*!< ETR input source is divided by 2*/
    TIMER_ETRPRESCALER_DIV4 = 0x2000, /*!< ETR input source is divided by 4*/
    TIMER_ETRPRESCALER_DIV8 = 0x3000  /*!< ETR input source is divided by 8*/
} timer_etps_t;

/**
 * @brief TIMER External trigger filter
 */
typedef enum {
    TIMER_ETF_0  = 0x0,   /*!< No filter*/
    TIMER_ETF_1  = 0x100, /*!< Sample frequence is fpclk,filter length is 2*/
    TIMER_ETF_2  = 0x200, /*!< Sample frequence is fpclk,filter length is 4*/
    TIMER_ETF_3  = 0x300, /*!< Sample frequence is fpclk,filter length is 8*/
    TIMER_ETF_4  = 0x400, /*!< Sample frequence is fDTS/2,filter length is 6*/
    TIMER_ETF_5  = 0x500, /*!< Sample frequence is fDTS/2,filter length is 8*/
    TIMER_ETF_6  = 0x600, /*!< Sample frequence is fDTS/4,filter length is 6*/
    TIMER_ETF_7  = 0x700, /*!< Sample frequence is fDTS/4,filter length is 8*/
    TIMER_ETF_8  = 0x800, /*!< Sample frequence is fDTS/8,filter length is 6*/
    TIMER_ETF_9  = 0x900, /*!< Sample frequence is fDTS/8,filter length is 8*/
    TIMER_ETF_10 = 0xa00, /*!< Sample frequence is fDTS/16,filter length is 5*/
    TIMER_ETF_11 = 0xb00, /*!< Sample frequence is fDTS/16,filter length is 6*/
    TIMER_ETF_12 = 0xc00, /*!< Sample frequence is fDTS/16,filter length is 8*/
    TIMER_ETF_13 = 0xd00, /*!< Sample frequence is fDTS/32,filter length is 5*/
    TIMER_ETF_14 = 0xe00, /*!< Sample frequence is fDTS/32,filter length is 6*/
    TIMER_ETF_15 = 0xf00  /*!< Sample frequence is fDTS/32,filter length is 8*/
} timer_etf_t;

/**
 * @brief TIMER Trigger source selection
 */
typedef enum {
    TIMER_TS_ITR0    = 0x0,  /*!< Internal Trigger 0 (ITR0)*/
    TIMER_TS_ITR1    = 0x10, /*!< Internal Trigger 1 (ITR1)*/
    TIMER_TS_ITR2    = 0x20, /*!< Internal Trigger 2 (ITR2)*/
    TIMER_TS_TI0F_ED = 0x40, /*!< TI0 Edge Detector (TI0F_ED)*/
    TIMER_TS_TI0FP0  = 0x50, /*!< Filtered Timer Input 0 (TI0FP0)*/
    TIMER_TS_TI1FP1  = 0x60, /*!< Filtered Timer Input 1 (TI1FP1)*/
    TIMER_TS_ETRF    = 0x70  /*!< Filtered External Trigger input (ETRF)*/
} timer_ts_t;

/**
 * @brief TIMER Slave mode selection
 */
typedef enum {
    TIMER_SMS_DISABLE  = (uint16_t)0x0,  /*!< Slave mode disabled*/
    TIMER_SMS_ENCODER1 = (uint16_t)0x1,  /*!< encoder mode 1,counts up/down on TI0FP0 edge depending
                                            on TI1FP1 level*/
    TIMER_SMS_ENCODER2 = (uint16_t)0x2,  /*!< encoder mode 2,counts up/down on TI1FP1 edge depending
                                            on TI0FP0 level*/
    TIMER_SMS_ENCODER3 = (uint16_t)0x3,  /*!< encoder mode 3,counts up/down on both TI0FP0 and
                                            TI1FP1 edges depending on the level of the other input*/
    TIMER_SMS_RESET     = (uint16_t)0x4, /*!< Reset Mode*/
    TIMER_SMS_GATED     = (uint16_t)0x5, /*!< Gated Mode*/
    TIMER_SMS_TRIGGER   = (uint16_t)0x6, /*!< Trigger Mode*/
    TIMER_SMS_EXTERNAL1 = (uint16_t)0x7  /*!< External Clock Mode 1*/
} timer_sms_t;

/**
 * @brief TIMER smcr configuration
 */
typedef enum {
    TIMER_MSM_SYNC     = 0x80,   /*!< Master/slave mode is synchronized*/
    TIMER_ECE_ENABLE   = 0x4000, /*!< External Clock Mode 2 enable*/
    TIMER_ETP_INVERTED = 0x8000, /*!< Polarity for ETR source*/
} timer_smcr_t;

/**
 * @brief TIMER channel 1 and 3 input filter
 */
typedef enum {
    TIMER_IC1F_0  = 0x0,    /*!< No filter*/
    TIMER_IC1F_1  = 0x1000, /*!< Sample frequence is fpclk,filter length is 2*/
    TIMER_IC1F_2  = 0x2000, /*!< Sample frequence is fpclk,filter length is 4*/
    TIMER_IC1F_3  = 0x3000, /*!< Sample frequence is fpclk,filter length is 8*/
    TIMER_IC1F_4  = 0x4000, /*!< Sample frequence is fDTS/2,filter length is 6*/
    TIMER_IC1F_5  = 0x5000, /*!< Sample frequence is fDTS/2,filter length is 8*/
    TIMER_IC1F_6  = 0x6000, /*!< Sample frequence is fDTS/4,filter length is 6*/
    TIMER_IC1F_7  = 0x7000, /*!< Sample frequence is fDTS/4,filter length is 8*/
    TIMER_IC1F_8  = 0x8000, /*!< Sample frequence is fDTS/8,filter length is 6*/
    TIMER_IC1F_9  = 0x9000, /*!< Sample frequence is fDTS/8,filter length is 8*/
    TIMER_IC1F_10 = 0xa000, /*!< Sample frequence is fDTS/16,filter length is 5*/
    TIMER_IC1F_11 = 0xb000, /*!< Sample frequence is fDTS/16,filter length is 6*/
    TIMER_IC1F_12 = 0xc000, /*!< Sample frequence is fDTS/16,filter length is 8*/
    TIMER_IC1F_13 = 0xd000, /*!< Sample frequence is fDTS/32,filter length is 5*/
    TIMER_IC1F_14 = 0xe000, /*!< Sample frequence is fDTS/32,filter length is 6*/
    TIMER_IC1F_15 = 0xf000  /*!< Sample frequence is fDTS/32,filter length is 8*/
} timer_ic1f_t;

/**
 * @brief TIMER channel 0 and 2 input filter
 */
typedef enum {
    TIMER_IC0F_0  = 0x0,  /*!< No filter*/
    TIMER_IC0F_1  = 0x10, /*!< Sample frequence is fpclk,filter length is 2*/
    TIMER_IC0F_2  = 0x20, /*!< Sample frequence is fpclk,filter length is 4*/
    TIMER_IC0F_3  = 0x30, /*!< Sample frequence is fpclk,filter length is 8*/
    TIMER_IC0F_4  = 0x40, /*!< Sample frequence is fDTS/2,filter length is 6*/
    TIMER_IC0F_5  = 0x50, /*!< Sample frequence is fDTS/2,filter length is 8*/
    TIMER_IC0F_6  = 0x60, /*!< Sample frequence is fDTS/4,filter length is 6*/
    TIMER_IC0F_7  = 0x70, /*!< Sample frequence is fDTS/4,filter length is 8*/
    TIMER_IC0F_8  = 0x80, /*!< Sample frequence is fDTS/8,filter length is 6*/
    TIMER_IC0F_9  = 0x90, /*!< Sample frequence is fDTS/8,filter length is 8*/
    TIMER_IC0F_10 = 0xa0, /*!< Sample frequence is fDTS/16,filter length is 5*/
    TIMER_IC0F_11 = 0xb0, /*!< Sample frequence is fDTS/16,filter length is 6*/
    TIMER_IC0F_12 = 0xc0, /*!< Sample frequence is fDTS/16,filter length is 8*/
    TIMER_IC0F_13 = 0xd0, /*!< Sample frequence is fDTS/32,filter length is 5*/
    TIMER_IC0F_14 = 0xe0, /*!< Sample frequence is fDTS/32,filter length is 6*/
    TIMER_IC0F_15 = 0xf0  /*!< Sample frequence is fDTS/32,filter length is 8*/
} timer_ic0f_t;

/**
 * @brief TIMER channel 0 input capture polarity
 */
typedef enum {
    TIMER_CC0P_RISING   = 0x0, /*!< Capture triggered by rising edge on timer input*/
    TIMER_CC0P_FALLING  = 0x2, /*!< Capture triggered by falling edge on timer input*/
    TIMER_CC0P_BOTHEDGE = 0xa  /*!< Capture triggered by both rising and falling
                                  edges on timer input*/
} timer_cc0p_input_t;

/**
 * @brief TIMER channel 1 input capture polarity
 */
typedef enum {
    TIMER_CC1P_RISING   = 0x0,  /*!< Capture triggered by rising edge on timer input*/
    TIMER_CC1P_FALLING  = 0x20, /*!< Capture triggered by falling edge on timer input*/
    TIMER_CC1P_BOTHEDGE = 0xa0  /*!< Capture triggered by both rising and falling
                                   edges on timer input*/
} timer_cc1p_input_t;

/**
 * @brief TIMER channel 2 input capture polarity
 */
typedef enum {
    TIMER_CC2P_RISING   = 0x0,   /*!< Capture triggered by rising edge on timer input*/
    TIMER_CC2P_FALLING  = 0x200, /*!< Capture triggered by falling edge on timer input*/
    TIMER_CC2P_BOTHEDGE = 0xa00  /*!< Capture triggered by both rising and
                                    falling edges on timer input*/
} timer_cc2p_input_t;

/**
 * @brief TIMER channel 3 input capture polarity
 */
typedef enum {
    TIMER_CC3P_RISING   = 0x0,    /*!< Capture triggered by rising edge on timer input*/
    TIMER_CC3P_FALLING  = 0x2000, /*!< Capture triggered by falling edge on timer input*/
    TIMER_CC3P_BOTHEDGE = 0xa000  /*!< Capture triggered by both rising and
                                     falling edges on timer input*/
} timer_cc3p_input_t;

/**
 * @brief TIMER channel polarity
 */
typedef union {
    timer_cc0p_input_t cc0p_polarity; /*!< Specifies the active edge of the input signal*/
    timer_cc1p_input_t cc1p_polarity; /*!< Specifies the active edge of the input signal*/
    timer_cc2p_input_t cc2p_polarity; /*!< Specifies the active edge of the input signal*/
    timer_cc3p_input_t cc3p_polarity; /*!< Specifies the active edge of the input signal*/
}timer_polarity_t;

/**
 * @brief TIMER channel filter
 */
typedef union {
    timer_ic0f_t ic0f_filter; /*!< Specifies the input capture filter of
                                     channel 0 or 2*/
    timer_ic1f_t ic1f_filter; /*!< Specifies the input capture filter of
                                     channel 1 or 3*/
}timer_filter_t;

/**
 * @brief TIMER Slave configuration Structure definition
 */
typedef struct {
    timer_sms_t slave_mode;         /*!< Slave mode selection*/
    timer_ts_t input_trigger;       /*!< Input trigger source*/
    bool trigger_polarity;          /*!< External input trigger polarity*/
    timer_etps_t trigger_prescaler; /*!< External input trigger prescaler*/
    timer_etf_t trigger_filter;     /*!< External input trigger filter*/
    timer_polarity_t ic_polarity;   /*!< Channel polarity*/
    timer_filter_t ic_filter;       /*!< Channel filter*/
} timer_slave_config_t;

/**
 * @brief TIMER dma definition
 */
typedef enum {
    TIMER_DIER_UDE   = 0x100,  /*!< DMA request is triggered by the update event*/
    TIMER_DIER_CC0DE = 0x200,  /*!< DMA request is triggered by the
                                  capture/compare macth 0 event*/
    TIMER_DIER_CC1DE = 0x400,  /*!< DMA request is triggered by the
                                  capture/compare macth 1 event event*/
    TIMER_DIER_CC2DE = 0x800,  /*!< DMA request is triggered by the
                                  capture/compare macth 2 event event*/
    TIMER_DIER_CC3DE = 0x1000, /*!< DMA request is triggered by the
                                  capture/compare macth 3 event event*/
    TIMER_DIER_TDE = 0x4000    /*!< DMA request is triggered by the trigger event*/
} timer_dma_t;

/**
 * @brief TIMER interrupt definition
 */
typedef enum {
    TIMER_DIER_UIE   = 0x1,  /*!< Update interrupt*/
    TIMER_DIER_CC0IE = 0x2,  /*!< Capture/Compare 0 interrupt*/
    TIMER_DIER_CC1IE = 0x4,  /*!< Capture/Compare 1 interrupt*/
    TIMER_DIER_CC2IE = 0x8,  /*!< Capture/Compare 2 interrupt*/
    TIMER_DIER_CC3IE = 0x10, /*!< Capture/Compare 3 interrupt*/
    TIMER_DIER_TIE   = 0x40, /*!< Trigger interrupt*/
} timer_interrupt_t;

/**
 * @brief TIMER Flag definition
 */
typedef enum {
    TIMER_SR_UIF   = 0x1,   /*!< Update interrupt flag*/
    TIMER_SR_CC0IF = 0x2,   /*!< Capture/Compare 0 interrupt flag*/
    TIMER_SR_CC1IF = 0x4,   /*!< Capture/Compare 1 interrupt flag*/
    TIMER_SR_CC2IF = 0x8,   /*!< Capture/Compare 2 interrupt flag*/
    TIMER_SR_CC3IF = 0x10,  /*!< Capture/Compare 3 interrupt flag*/
    TIMER_SR_TIF   = 0x40,  /*!< Trigger interrupt flag*/
    TIMER_SR_CC0OF = 0x200, /*!< Capture 0 overcapture flag*/
    TIMER_SR_CC1OF = 0x400, /*!< Capture 1 overcapture flag*/
    TIMER_SR_CC2OF = 0x800, /*!< Capture 2 overcapture flag*/
    TIMER_SR_CC3OF = 0x1000 /*!< Capture 3 overcapture flag*/
} timer_sr_t;

/**
 * @brief TIMER Event register
 */
typedef enum {
    TIMER_EGR_UG   = 0x1,  /*!< Reinitialize the counter and generates an update*/
    TIMER_EGR_CC0G = 0x2,  /*!< A capture/compare event is generated on channel 0*/
    TIMER_EGR_CC1G = 0x4,  /*!< A capture/compare event is generated on channel 1*/
    TIMER_EGR_CC2G = 0x8,  /*!< A capture/compare event is generated on channel 2*/
    TIMER_EGR_CC3G = 0x10, /*!< A capture/compare event is generated on channel 3*/
    TIMER_EGR_TG   = 0x40  /*!< A trigger event is generated*/
} timer_egr_t;

/**
 * @brief TIMER Channel 1 and 3 output compare and PWM modes
 */
typedef enum {
    TIMER_OC1M_TIMING          = 0x0,    /*!< Frozen*/
    TIMER_OC1M_ACTIVE          = 0x1000, /*!< Set channel to active level on match*/
    TIMER_OC1M_INACTIVE        = 0x2000, /*!< Set channel to inactive level on match*/
    TIMER_OC1M_TOGGLE          = 0x3000, /*!< Toggle*/
    TIMER_OC1M_FORCED_INACTIVE = 0x4000, /*!< Force active level*/
    TIMER_OC1M_FORCED_ACTIVE   = 0x5000, /*!< Force inactive level*/
    TIMER_OC1M_PWM1            = 0x6000, /*!< PWM mode 1*/
    TIMER_OC1M_PWM2            = 0x7000  /*!< PWM mode 2*/
} timer_oc1m_t;

/**
 * @brief TIMER Channel 0 and 2 output compare and PWM modes
 */
typedef enum {
    TIMER_OC0M_TIMING          = 0x0,  /*!< Frozen*/
    TIMER_OC0M_ACTIVE          = 0x10, /*!< Set channel to active level on match*/
    TIMER_OC0M_INACTIVE        = 0x20, /*!< Set channel to inactive level on match*/
    TIMER_OC0M_TOGGLE          = 0x30, /*!< Toggle*/
    TIMER_OC0M_FORCED_INACTIVE = 0x40, /*!< Force active level*/
    TIMER_OC0M_FORCED_ACTIVE   = 0x50, /*!< Force inactive level*/
    TIMER_OC0M_PWM1            = 0x60, /*!< PWM mode 1*/
    TIMER_OC0M_PWM2            = 0x70  /*!< PWM mode 2*/
} timer_oc0m_t;

/**
 * @brief TIMER Channel 1 and 3 capture and compare selection
 */
typedef enum {
    TIMER_CC1S_OUTPUT     = 0x0,   /*!< Output mode*/
    TIMER_CC1S_INPUT_SAME = 0x100, /*!< Iutput to the same channel*/
    TIMER_CC1S_INPUT_NEAR = 0x200, /*!< Iutput to the near channel*/
    TIMER_CC1S_INPUT_TRC  = 0x300  /*!< Iutput to TRC*/
} timer_cc1s_t;

/**
 * @brief TIMER Channel 0 and 2 capture and compare selection
 */
typedef enum {
    TIMER_CC0S_OUTPUT     = 0x0, /*!< Output mode*/
    TIMER_CC0S_INPUT_SAME = 0x1, /*!< Iutput to the same channel*/
    TIMER_CC0S_INPUT_NEAR = 0x2, /*!< Iutput to the near channel*/
    TIMER_CC0S_INPUT_TRC  = 0x3  /*!< Iutput to TRC*/
} timer_cc0s_t;

/**
 * @brief TIMER ccmr output mode configuration
 */
typedef enum {
    TIMER_CCMR_OC0FE = 0x4,    /*!< Output fast state enable*/
    TIMER_CCMR_OC0PE = 0x8,    /*!< Output compare preload enable*/
    TIMER_CCMR_OC0CE = 0x80,   /*!< Output compare clear enable*/
    TIMER_CCMR_OC1FE = 0x400,  /*!< Output fast state enable*/
    TIMER_CCMR_OC1PE = 0x800,  /*!< Output compare preload enable*/
    TIMER_CCMR_OC1CE = 0x8000, /*!< Output compare clear enable*/
} timer_ccmr_output_t;

/**
 * @brief TIMER channel 1 and 3 input capture prescaler
 */
typedef enum {
    TIMER_IC1PSC_DIV1 = 0x0,   /*!< Clock division: 1*/
    TIMER_IC1PSC_DIV2 = 0x400, /*!< Clock division: 2*/
    TIMER_IC1PSC_DIV4 = 0x800, /*!< Clock division: 4*/
    TIMER_IC1PSC_DIV8 = 0xc00, /*!< Clock division: 8*/
} timer_ic1psc_t;

/**
 * @brief TIMER channel 0 and 2 input capture prescaler
 */
typedef enum {
    TIMER_IC0PSC_DIV1 = 0x0, /*!< Clock division: 1*/
    TIMER_IC0PSC_DIV2 = 0x4, /*!< Clock division: 2*/
    TIMER_IC0PSC_DIV4 = 0x8, /*!< Clock division: 4*/
    TIMER_IC0PSC_DIV8 = 0xc, /*!< Clock division: 8*/
} timer_ic0psc_t;

/**
 * @brief TIMER CCER register config
 */
typedef enum {
    TIMER_CCER_CC0E  = 0x1,    /*!< Capture compare 0 enabled*/
    TIMER_CCER_CC0P  = 0x2,    /*!< Capture/Compare output polarity*/
    TIMER_CCER_CC0NP = 0x8,    /*!< Input enable*/
    TIMER_CCER_CC1E  = 0x10,   /*!< Capture compare 1 enabled*/
    TIMER_CCER_CC1P  = 0x20,   /*!< Capture/Compare output polarity*/
    TIMER_CCER_CC1NP = 0x80,   /*!< Input enable*/
    TIMER_CCER_CC2E  = 0x100,  /*!< Capture compare 2 enabled*/
    TIMER_CCER_CC2P  = 0x200,  /*!< Capture/Compare output polarity*/
    TIMER_CCER_CC2NP = 0x800,  /*!< Input enable*/
    TIMER_CCER_CC3E  = 0x1000, /*!< Capture compare 3 enabled*/
    TIMER_CCER_CC3P  = 0x2000, /*!< Capture/Compare output polarity*/
    TIMER_CCER_CC3NP = 0x8000  /*!< Input enable*/
} timer_ccer_t;

/**
 * @brief TIMER channel selection
 */
typedef union {
    timer_cc0s_t cc0s_selection; /*!< Specifies the input of channel 0 or 2*/
    timer_cc1s_t cc1s_selection; /*!< Specifies the input of channel 1 or 3*/
}timer_selection_t;

/**
 * @brief TIMER channel prescaler
 */
typedef union {
    timer_ic0psc_t ic0psc_prescaler; /*!< Specifies the input capture
                                            prescaler of channel 0 or 2*/
    timer_ic1psc_t ic1psc_prescaler; /*!< Specifies the nput capture
                                            prescaler of channel 1 or 3*/
}timer_prescaler_t;

/**
 * @brief TIMER input capture configuration structure definition
 */
typedef struct {
    timer_polarity_t ic_polarity;   /*!< Channel polarity*/
    timer_selection_t ic_selection; /*!< Channel selection*/
    timer_prescaler_t ic_prescaler; /*!< Channel prescaler*/
    timer_filter_t ic_filter;       /*!< Channel filter*/
} timer_ic_init_t;

/**
 * @brief TIMER channel output mode
 */
typedef union {
    timer_oc0m_t oc0m_mode; /*!< Specifies the channel 0 or 2 output mode*/
    timer_oc1m_t oc1m_mode; /*!< Specifies the channel 1 or 3 output mode*/
}timer_oc_mode_t;

/**
 * @brief TIMER Output compare configuration structure definition
 */
typedef struct {
    timer_oc_mode_t oc_mode; /*!< Channel output mode.*/
    uint32_t pulse;          /*!< Specifies the pulse value to be loaded into the Capture
                                    Compare Register.*/

    bool high_level;         /*!< Specifies the output high level*/

    bool oc_fast;             /*!< Specifies output fast state enable.*/
} timer_oc_init_t;

/**
 * @brief TIMER dma read/write length
 */
typedef enum {
    TIMER_DBL_1    = 0x0,    /*!< 1 transmission*/
    TIMER_DBL_2    = 0x100,  /*!< 2 transmissions*/
    TIMER_DBL_3    = 0x200,  /*!< 3 transmissions*/
    TIMER_DBL_4    = 0x300,  /*!< 4 transmissions*/
    TIMER_DBL_5    = 0x400,  /*!< 5 transmissions*/
    TIMER_DBL_6    = 0x500,  /*!< 6 transmissions*/
    TIMER_DBL_7    = 0x600,  /*!< 7 transmissions*/
    TIMER_DBL_8    = 0x700,  /*!< 8 transmissions*/
    TIMER_DBL_9    = 0x800,  /*!< 9 transmissions*/
    TIMER_DBL_10   = 0x900,  /*!< 10 transmissions*/
    TIMER_DBL_11   = 0xa00,  /*!< 11 transmissions*/
    TIMER_DBL_12   = 0xb00,  /*!< 12 transmissions*/
    TIMER_DBL_13   = 0xc00,  /*!< 13 transmissions*/
    TIMER_DBL_14   = 0xd00,  /*!< 14 transmissions*/
    TIMER_DBL_15   = 0xe00,  /*!< 15 transmissions*/
    TIMER_DBL_16   = 0xf00,  /*!< 16 transmissions*/
    TIMER_DBL_17   = 0x1000, /*!< 17 transmissions*/
    TIMER_DBL_18   = 0x1100, /*!< 18 transmissions*/
    TIMER_DBL_RESV = 0X1F00  /*!< reserved*/
} timer_dbl_t;

/**
 * @brief TIMER dma read/write base address
 */
typedef enum {
    TIMER_DBA_CR1   = 0x0,  /*!< TIM_CR1*/
    TIMER_DBA_CR2   = 0x1,  /*!< TIM_CR2*/
    TIMER_DBA_SMCR  = 0x2,  /*!< TIM_SMCR*/
    TIMER_DBA_DIER  = 0x3,  /*!< TIM_DIER*/
    TIMER_DBA_SR    = 0x4,  /*!< TIM_SR*/
    TIMER_DBA_EGR   = 0x5,  /*!< TIM_EGR*/
    TIMER_DBA_CCMR1 = 0x6,  /*!< TIM_CCMR1*/
    TIMER_DBA_CCMR2 = 0x7,  /*!< TIM_CCMR2*/
    TIMER_DBA_CCER  = 0x8,  /*!< TIM_CCER*/
    TIMER_DBA_CNT   = 0x9,  /*!< TIM_CNT*/
    TIMER_DBA_PSC   = 0xa,  /*!< TIM_PSC*/
    TIMER_DBA_ARR   = 0xb,  /*!< TIM_ARR*/
    TIMER_DBA_RESV1 = 0xc,  /*!< RESERVED*/
    TIMER_DBA_CCR0  = 0xd,  /*!< TIM_CCR0*/
    TIMER_DBA_CCR1  = 0xe,  /*!< TIM_CCR1*/
    TIMER_DBA_CCR2  = 0xf,  /*!< TIM_CCR2*/
    TIMER_DBA_CCR3  = 0x10, /*!< TIM_CCR3*/
    TIMER_DBA_DMAR  = 0x11, /*!< TIM_DMAR*/
    TIMER_DBA_OR    = 0x12, /*!< TIM_OR*/
    TIMER_DBA_RES2  = 0X1f  /*!< RESERVED*/
} timer_dba_t;

/**
 * @brief TIMER clear OCxREF structure definition
 */
typedef struct {
    timer_oc_init_t oc_init;    /*!< Output compare configuration*/
    bool trigger_polarity;      /*!< External input trigger polarity*/
    timer_etf_t trigger_filter; /*!< External input trigger filter*/
} timer_clear_ocxref_t;

/**
 * @brief TIMER timer0 TI0 OR remap
 */
typedef enum {
    TIMER_TIM0_TI0_IOM   = 0x0, /*!< TIM0 TI0 input connected to channel 0*/
    TIMER_TIM0_TI0_UART0 = 0x1, /*!< TIM0 TI0 input connected to uart0 rx*/
    TIMER_TIM0_TI0_UART1 = 0x2, /*!< TIM0 TI0 input connected to uart1 rx*/
    TIMER_TIM0_TI0_UART2 = 0x3, /*!< TIM0 TI0 input connected to uart2 rx*/
    TIMER_TIM0_TI0_UART3 = 0x4, /*!< TIM0 TI0 input connected to uart3 rx*/
    TIMER_TIM0_TI0_UART4 = 0x5, /*!< TIM0 TI0 input connected to uart4 rx*/
    TIMER_TIM0_TI0_RESV  = 0xf  /*!< RESERVED*/
} timer_tim0_ti0_or_t;

/**
 * @brief TIMER timer0 TI3 OR remap
 */
typedef enum {
    TIMER_TIM0_TI3_IOM   = 0x0,  /*!< TIM0 TI3 input connected to channel 3*/
    TIMER_TIM0_TI3_COMP0 = 0x10, /*!< TIM0 TI3 input connected to comparator0*/
    TIMER_TIM0_TI3_COMP1 = 0x20, /*!< TIM0 TI3 input connected to comparator1*/
    TIMER_TIM0_TI3_RESV  = 0x70, /*!< RESERVED*/
} timer_tim0_ti3_or_t;

/**
 * @brief TIMER timer0 etr OR remap
 */
typedef enum {
    TIMER_TIM0_ETR_IOM    = 0x0,   /*!< TIM0 etr input connected to etr*/
    TIMER_TIM0_ETR_COMP0  = 0x80,  /*!< TIM0 etr input connected to comparator0*/
    TIMER_TIM0_ETR_COMP1  = 0x100, /*!< TIM0 etr input connected to comparator1*/
    TIMER_TIM0_ETR_XO32K  = 0x180, /*!< TIM0 etr input connected to XO32K*/
    TIMER_TIM0_ETR_RCO48M = 0x200, /*!< TIM0 etr input connected to RCO48M*/
    TIMER_TIM0_ETR_ADC0   = 0x280, /*!< TIM0 etr input connected to adcctrl awd0*/
    TIMER_TIM0_ETR_ADC1   = 0x300, /*!< TIM0 etr input connected to adcctrl awd1*/
    TIMER_TIM0_ETR_ADC2   = 0x380, /*!< TIM0 etr input connected to adcctrl awd2*/
    TIMER_TIM0_ETR_UART0  = 0x400, /*!< TIM0 etr input connected to uart0 rx*/
    TIMER_TIM0_ETR_UART1  = 0x480, /*!< TIM0 etr input connected to uart1 rx*/
    TIMER_TIM0_ETR_UART2  = 0x500, /*!< TIM0 etr input connected to uart2 rx*/
    TIMER_TIM0_ETR_UART3  = 0x580, /*!< TIM0 etr input connected to uart3 rx*/
    TIMER_TIM0_ETR_UART4  = 0x600, /*!< TIM0 etr input connected to uart4 rx*/
    TIMER_TIM0_ETR_RESV   = 0x780  /*!< RESERVED*/
} timer_tim0_etr_or_t;

/**
 * @brief TIMER timer1 TI2 OR remap
 */
typedef enum {
    TIMER_TIM1_TI2_IOM      = 0x0, /*!< TIM1 TI2 input connected to channel 2*/
    TIMER_TIM1_TI2_TIM3_CH1 = 0x1, /*!< TIM1 TI2 input connected to TIM3 channel 1*/
    TIMER_TIM1_TI2_RESV     = 0x3  /*!< RESERVED*/
} timer_tim1_ti2_or_t;

/**
 * @brief TIMER timer2 TI0 OR remap
 */
typedef enum {
    TIMER_TIM2_TI0_IOM     = 0x0, /*!< TIM2 TI0 input connected to channel 0*/
    TIMER_TIM2_TI0_XO24M   = 0x1, /*!< TIM2 TI0 input connected to XO24M*/
    TIMER_TIM2_TI0_XO32M   = 0x2, /*!< TIM2 TI0 input connected to XO32M*/
    TIMER_TIM2_TI0_RCO48M  = 0x3, /*!< TIM2 TI0 input connected to RCO48M*/
    TIMER_TIM2_TI0_XO32K   = 0x4, /*!< TIM2 TI0 input connected to XO32K*/
    TIMER_TIM2_TI0_RCO32K  = 0x5, /*!< TIM2 TI0 input connected to RCO32K*/
    TIMER_TIM2_TI0_MCO     = 0x6, /*!< TIM2 TI0 input connected to mco*/
    TIMER_TIM2_TI0_COMP0   = 0x7, /*!< TIM2 TI0 input connected to comparator0*/
    TIMER_TIM2_TI0_RCO4M   = 0x8, /*!< TIM2 TI0 input connected to RCO4M*/
    TIMER_TIM2_TI0_WAKEUP0 = 0x9, /*!< TIM2 TI0 input connected to rtc wakeup0*/
    TIMER_TIM2_TI0_WAKEUP1 = 0xa, /*!< TIM2 TI0 input connected to rtc wakeup1*/
    TIMER_TIM2_TI0_WAKEUP2 = 0xb, /*!< TIM2 TI0 input connected to rtc wakeup2*/
    TIMER_TIM2_TI0_RESV    = 0x1f /*!< RESERVED*/
} timer_tim2_ti0_or_t;

/**
 * @brief TIMER timer2 TI1 OR remap
 */
typedef enum {
    TIMER_TIM2_TI1_IOM   = 0x0,  /*!< TIM2 TI1 input connected to channel 1*/
    TIMER_TIM2_TI1_COMP1 = 0x20, /*!< TIM2 TI1 input connected to comparator1*/
    TIMER_TIM2_TI1_RESV  = 0x60  /*!< RESERVED*/
} timer_tim2_ti1_or_t;

/**
 * @brief TIMER timer2 etr OR remap
 */
typedef enum {
    TIMER_TIM2_ETR_IOM   = 0x0,   /*!< TIM2 etr input connected to etr*/
    TIMER_TIM2_ETR_COMP0 = 0x80,  /*!< TIM2 etr input connected to comparator0*/
    TIMER_TIM2_ETR_COMP1 = 0x100, /*!< TIM2 etr input connected to comparator1*/
    TIMER_TIM2_ETR_XO32K = 0x180, /*!< TIM2 etr input connected to XO32K*/
    TIMER_TIM2_ETR_RESV  = 0x380  /*!< RESERVED*/
} timer_tim2_etr_or_t;

/**
 * @brief TIMER timer3 TI0 OR remap
 */
typedef enum {
    TIMER_TIM3_TI0_IOM   = 0x0, /*!< TIM3 TI0 input connected to channel 0*/
    TIMER_TIM3_TI0_COMP0 = 0x1, /*!< TIM3 TI0 input connected to comparator0*/
    TIMER_TIM3_TI0_COMP1 = 0x2, /*!< TIM3 TI0 input connected to comparator1*/
    TIMER_TIM3_TI0_UART0 = 0x3, /*!< TIM3 TI0 input connected to uart0 rx*/
    TIMER_TIM3_TI0_UART1 = 0x4, /*!< TIM3 TI0 input connected to uart1 rx*/
    TIMER_TIM3_TI0_UART2 = 0x5, /*!< TIM3 TI0 input connected to uart2 rx*/
    TIMER_TIM3_TI0_UART3 = 0x6, /*!< TIM3 TI0 input connected to uart3 rx*/
    TIMER_TIM3_TI0_UART4 = 0x7  /*!< TIM3 TI0 input connected to uart4 rx*/
} timer_tim3_ti0_or_t;

/**
 * @brief TIMER timer3 etr OR remap
 */
typedef enum {
    TIMER_TIM3_ETR_IOM   = 0x0,  /*!< TIM3 etr input connected to channel 0*/
    TIMER_TIM3_ETR_COMP0 = 0x8,  /*!< TIM3 etr input connected to comparator0*/
    TIMER_TIM3_ETR_COMP1 = 0x10, /*!< TIM3 etr input connected to comparator1*/
    TIMER_TIM3_ETR_XO32K = 0x18, /*!< TIM3 etr input connected to XO32K*/
    TIMER_TIM3_ETR_UART0 = 0x20, /*!< TIM3 etr input connected to uart0 rx*/
    TIMER_TIM3_ETR_UART1 = 0x28, /*!< TIM3 etr input connected to uart1 rx*/
    TIMER_TIM3_ETR_UART2 = 0x30, /*!< TIM3 etr input connected to uart2 rx*/
    TIMER_TIM3_ETR_UART3 = 0x38, /*!< TIM3 etr input connected to uart3 rx*/
    TIMER_TIM3_ETR_UART4 = 0x40, /*!< TIM3 etr input connected to uart4 rx*/
    TIMER_TIM3_ETR_RESV  = 0x78  /*!< RESERVED*/
} timer_tim3_etr_or_t;

void timer_init(timer_gp_t* TIMERx, timer_init_t* timer_init);
void timer_deinit(timer_gp_t* TIMERx);

void timer_cmd(timer_gp_t* TIMERx, bool state);

void timer_disable_update(timer_gp_t* TIMERx, uint8_t state);
void timer_config_update_source(timer_gp_t* TIMERx, uint8_t state);
void timer_config_master_mode(timer_gp_t* TIMERx, timer_master_mode_t master_mode);
void timer_enable_xor(timer_gp_t* TIMERx, uint8_t state);
void timer_enable_ccds_upd(timer_gp_t* TIMERx, uint8_t state);
void timer_config_etr(
    timer_gp_t* TIMERx, timer_etps_t ext_trg_prescaler, bool ext_trg_polarity, timer_etf_t ext_trg_filter);
void timer_config_external_clock2(timer_gp_t* TIMERx, uint8_t state);
void timer_config_itrx(timer_gp_t* TIMERx, timer_ts_t input_trigger_source);
void timer_config_slave_mode(timer_gp_t* TIMERx, timer_sms_t slave_mode);
void timer_config_ti(
    timer_gp_t* TIMERx, uint32_t ic_polarity, uint32_t ic_selection, uint32_t ic_filter, uint8_t channel);
void timer_config_polarity_filter_ti(timer_gp_t* TIMERx, uint32_t ic_polarity, uint32_t ic_filter, uint8_t channel);
int32_t timer_config_slave(timer_gp_t* TIMERx, timer_slave_config_t* slave_config);
void timer_config_channel_ic(timer_gp_t* TIMERx, timer_ic_init_t* ic_init, uint8_t channel);
void timer_config_oc(timer_gp_t* TIMERx, timer_oc_init_t* oc_init, uint8_t channel);
void timer_config_one_pulse(timer_gp_t* TIMERx, uint8_t state);
void timer_config_ts(timer_gp_t* TIMERx, timer_ts_t ts);
int32_t timer_clear_ocxref(timer_gp_t* TIMERx, timer_clear_ocxref_t* clear_ocxref, uint8_t channel);
void timer_config_pwm(timer_gp_t* TIMERx, timer_oc_init_t* oc_init, timer_init_t* timerx_init, uint8_t channel);
void timer_generate_event(timer_gp_t* TIMERx, timer_egr_t egr_event, uint8_t state);

void timer_config_interrupt(timer_gp_t* TIMERx, timer_interrupt_t interrupt, uint8_t state);
void timer_get_status(timer_gp_t* TIMERx, timer_sr_t status, bool* state);
void timer_clear_status(timer_gp_t* TIMERx, timer_sr_t status);

void timer_config_dma(timer_gp_t* TIMERx, timer_dma_t dma, uint8_t state);
void timer_set_dma_rw_len(timer_gp_t* TIMERx, timer_dbl_t dbl_len);
void timer_set_dma_base_addr(timer_gp_t* TIMERx, timer_dba_t dba_addr);

void timer_config_or_tim0_ti0(timer_gp_t* TIMERx, timer_tim0_ti0_or_t tim0_ti0_or);
void timer_config_or_tim0_ti3(timer_gp_t* TIMERx, timer_tim0_ti3_or_t tim0_ti3_or);
void timer_config_or_tim0_etr(timer_gp_t* TIMERx, timer_tim0_etr_or_t tim0_etr_or);
void timer_config_or_tim1_ti2(timer_gp_t* TIMERx, timer_tim1_ti2_or_t tim1_ti2_or);
void timer_config_or_tim2_ti0(timer_gp_t* TIMERx, timer_tim2_ti0_or_t tim2_ti0_or);
void timer_config_or_tim2_ti1(timer_gp_t* TIMERx, timer_tim2_ti1_or_t tim2_ti1_or);
void timer_config_or_tim2_etr(timer_gp_t* TIMERx, timer_tim2_etr_or_t tim2_etr_or);
void timer_config_or_tim3_ti0(timer_gp_t* TIMERx, timer_tim3_ti0_or_t tim3_ti0_or);
void timer_config_or_tim3_etr(timer_gp_t* TIMERx, timer_tim3_etr_or_t tim3_etr_or);

#ifdef __cplusplus
}
#endif
#endif /* _TREMO_TIMER_H_ */

/**
 * @} 
 * @}
 */
