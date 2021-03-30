/**
 ******************************************************************************
 * @file    tremo_adc.h
 * @author  ASR Tremo Team
 * @version v1.4.0
 * @date    2021-03-05
 * @brief   Header file of ADC module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup ADC
 * @{
 */

#ifndef __TREMO_ADC_H
#define __TREMO_ADC_H

#ifdef __cplusplus
extern "C"{
#endif

#include "tremo_regs.h"
#include "stdbool.h"

	
/**
 * @brief ADC control register definition
 */
typedef enum {
    ADC_ENABLE = 0x1,  /*!< ADC enable*/
    ADC_DISABLE = 0x2, /*!< ADC disable*/
    ADC_START = 0x4,   /*!< ADC start*/
    ADC_STOP = 0x8     /*!< ADC stop*/
} adc_cr_t;

/**
 * @brief ADC configuration definition
 */
typedef enum {
    ADC_DMA_ENABLE = 0x1000,    /*!< ADC DMA enable*/
    ADC_OVERRUN_MODE = 0x80000, /*!< ADC overrun mode*/
    ADC_WAIT_MODE = 0x400000    /*!< ADC wait mode*/
} adc_cfg_t;

/**
 * @brief ADC clock division definition
 */
typedef enum {
    ADC_CLK_DIV_0 = 0x0,    /*!< ADC clock not division*/
    ADC_CLK_DIV_1 = 0x1,    /*!< ADC clock not division*/
    ADC_CLK_DIV_MAX = 0xfff /*!< ADC clock max division*/
} adc_clk_div_t;

/**
 * @brief ADC trigger source
 */
typedef enum {
    ADC_TRG_SOURCE_RSV_0 = 0x0,            /*!< ADC trigger source 0*/
    ADC_TRG_SOURCE_RSV_1 = 0x2000,         /*!< ADC trigger source 1*/
    ADC_TRG_SOURCE_RSV_2 = 0x4000,         /*!< ADC trigger source 2*/
    ADC_TRG_SOURCE_RSV_3 = 0x6000,         /*!< ADC trigger source 3*/
    ADC_TRG_SOURCE_RSV_4 = 0x8000,         /*!< ADC trigger source 4*/
    ADC_TRG_SOURCE_GP47_5 = 0xa000,        /*!< ADC trigger source 5*/
    ADC_TRG_SOURCE_GP31_6 = 0xc000,        /*!< ADC trigger source 6*/
    ADC_TRG_SOURCE_GP19_7 = 0xe000,        /*!< ADC trigger source 7*/
    ADC_TRG_SOURCE_GP10_8 = 0x10000,       /*!< ADC trigger source 8*/
    ADC_TRG_SOURCE_TIM1_TRGO_9 = 0x12000,  /*!< ADC trigger source 9*/
    ADC_TRG_SOURCE_TIM0_CH2_10 = 0x14000,  /*!< ADC trigger source 10*/
    ADC_TRG_SOURCE_TIM3_TRGO_11 = 0x16000, /*!< ADC trigger source 11*/
    ADC_TRG_SOURCE_TIM0_CH3_12 = 0x18000,  /*!< ADC trigger source 12*/
    ADC_TRG_SOURCE_TIM0_TRGO_13 = 0x1a000, /*!< ADC trigger source 13*/
    ADC_TRG_SOURCE_TIM2_CH1_14 = 0x1c000,  /*!< ADC trigger source 14*/
    ADC_TRG_SOURCE_RSV_15 = 0x1e000        /*!< ADC trigger source 15*/
} adc_trigger_source_t;

/**
 * @brief ADC trigger polarity
 */
typedef enum {
    ADC_TRG_POLARITY_SOFT = 0x0,         /*!< ADC soft trigger*/
    ADC_TRG_POLARITY_RISING = 0x20000,   /*!< ADC rising trigger*/
    ADC_TRG_POLARITY_FALLING = 0x40000,  /*!< ADC falling trigger*/
    ADC_TRG_POLARITY_BOTH_EDGE = 0x60000 /*!< ADC both edge trigger*/
} adc_trigger_polarity_t;

/**
 * @brief ADC converse mode
 */
typedef enum {
    ADC_CONV_MODE_SINGLE = 0x0,           /*!< ADC single converse*/
    ADC_CONV_MODE_CONTINUE = 0x100000,    /*!< ADC continue converse*/
    ADC_CONV_MODE_DISCONTINUE = 0x200000, /*!< ADC discontinue converse*/
    ADC_CONV_MODE_RSV = 0x300000          /*!< Reserved*/
} adc_conv_mode_t;

/**
 * @brief ADC sample channel
 */
typedef enum {
    ADC_SAMPLE_CHAN_0 = 0x0,  /*!< ADC sample channel reserved*/
    ADC_SAMPLE_CHAN_1 = 0x1,  /*!< ADC sample channel 1*/
    ADC_SAMPLE_CHAN_2 = 0x2,  /*!< ADC sample channel 2*/
    ADC_SAMPLE_CHAN_3 = 0x3,  /*!< ADC sample channel 3*/
    ADC_SAMPLE_CHAN_4 = 0x4,  /*!< ADC sample channel 4*/
    ADC_SAMPLE_CHAN_5 = 0x5,  /*!< ADC sample channel 5*/
    ADC_SAMPLE_CHAN_6 = 0x6,  /*!< ADC sample channel 6*/
    ADC_SAMPLE_CHAN_7 = 0x7,  /*!< ADC sample channel 7*/
    ADC_SAMPLE_CHAN_8 = 0x8,  /*!< ADC sample channel 8*/
    ADC_SAMPLE_CHAN_9 = 0x9,  /*!< ADC sample channel 9*/
    ADC_SAMPLE_CHAN_10 = 0xa, /*!< ADC sample channel 10*/
    ADC_SAMPLE_CHAN_11 = 0xb, /*!< ADC sample channel 11*/
    ADC_SAMPLE_CHAN_12 = 0xc, /*!< ADC sample channel 12*/
    ADC_SAMPLE_CHAN_13 = 0xd, /*!< ADC sample channel 13*/
    ADC_SAMPLE_CHAN_14 = 0xe, /*!< ADC sample channel 14*/
    ADC_SAMPLE_CHAN_15 = 0xf, /*!< ADC sample channel 15*/
} adc_sample_chan_t;

/**
 * @brief ADC sample sequence channel
 */
typedef enum {
    ADC_SAMPLE_SEQ_CHAN_0 = 0x0,  /*!< ADC sample sequence channel 0*/
    ADC_SAMPLE_SEQ_CHAN_1 = 0x1,  /*!< ADC sample sequence channel 1*/
    ADC_SAMPLE_SEQ_CHAN_2 = 0x2,  /*!< ADC sample sequence channel 2*/
    ADC_SAMPLE_SEQ_CHAN_3 = 0x3,  /*!< ADC sample sequence channel 3*/
    ADC_SAMPLE_SEQ_CHAN_4 = 0x4,  /*!< ADC sample sequence channel 4*/
    ADC_SAMPLE_SEQ_CHAN_5 = 0x5,  /*!< ADC sample sequence channel 5*/
    ADC_SAMPLE_SEQ_CHAN_6 = 0x6,  /*!< ADC sample sequence channel 6*/
    ADC_SAMPLE_SEQ_CHAN_7 = 0x7,  /*!< ADC sample sequence channel 7*/
    ADC_SAMPLE_SEQ_CHAN_8 = 0x8,  /*!< ADC sample sequence channel 8*/
    ADC_SAMPLE_SEQ_CHAN_9 = 0x9,  /*!< ADC sample sequence channel 9*/
    ADC_SAMPLE_SEQ_CHAN_10 = 0xa, /*!< ADC sample sequence channel 10*/
    ADC_SAMPLE_SEQ_CHAN_11 = 0xb, /*!< ADC sample sequence channel 11*/
    ADC_SAMPLE_SEQ_CHAN_12 = 0xc, /*!< ADC sample sequence channel 12*/
    ADC_SAMPLE_SEQ_CHAN_13 = 0xd, /*!< ADC sample sequence channel 13*/
    ADC_SAMPLE_SEQ_CHAN_14 = 0xe, /*!< ADC sample sequence channel 14*/
    ADC_SAMPLE_SEQ_CHAN_15 = 0xf, /*!< ADC sample sequence channel 15*/
} adc_sample_seq_chan_t;

/**
 * @brief ADC difference selection
 */
typedef enum {
    ADC_DIFF_SEL_CHAN_1 = 0x2,  /*!< ADC channel 1 difference selection*/
    ADC_DIFF_SEL_CHAN_2 = 0x4,  /*!< ADC channel 2 difference selection*/
    ADC_DIFF_SEL_CHAN_3 = 0x8,  /*!< ADC channel 3 difference selection*/
    ADC_DIFF_SEL_CHAN_4 = 0x10, /*!< ADC channel 4 difference selection*/
    ADC_DIFF_SEL_CHAN_5 = 0x20, /*!< ADC channel 5 difference selection*/
    ADC_DIFF_SEL_CHAN_6 = 0x40, /*!< ADC channel 6 difference selection*/
    ADC_DIFF_SEL_CHAN_7 = 0x80, /*!< ADC channel 7 difference selection*/
    ADC_DIFF_SEL_CHAN_8 = 0x100 /*!< ADC channel 8 difference selection*/
} adc_diff_sel_t;

/**
 * @brief ADC interrupt status definition
 */
typedef enum {
    ADC_ISR_EOC = 0x1,     /*ADC converse finish flag*/
    ADC_ISR_EOS = 0x2,     /*ADC sequence sample finish flag*/
    ADC_ISR_OVERRUN = 0x4, /*ADC overrun flag*/
} adc_isr_t;

/**
 * @brief ADC interrupt enable definition
 */
typedef enum {
    ADC_IER_EOC = 0x1,     /*ADC converse finish interrupt*/
    ADC_IER_EOS = 0x2,     /*ADC sequence sample finish interrupt*/
    ADC_IER_OVERRUN = 0x4, /*ADC overrun interrupt*/
} adc_ier_t;


void adc_init();

void adc_start(bool start_flag);
void adc_enable(bool enable_flag);
void adc_enable_dma(bool dma_flag);
void adc_retain_data_when_overrun(bool retain_flag);

void adc_enable_wait_mode(bool wait_flag);
void adc_config_clock_division(uint32_t clock_division);
void adc_config_trigger_source(adc_trigger_source_t trigger_source);
void adc_config_trigger_polarity(adc_trigger_polarity_t trigger_polarity);
void adc_config_conv_mode(adc_conv_mode_t conv_mode);
void adc_config_sample_sequence(uint8_t sample_seq_chan, uint8_t sample_chan);
void adc_config_sample_chan_diff(adc_diff_sel_t diff_sel, bool dif_flag);

bool adc_get_interrupt_status(adc_isr_t isr_flag);
void adc_clear_interrupt_status(adc_isr_t isr_flag);
void adc_config_interrupt(adc_ier_t interrupt, bool enable_flag);

uint16_t adc_get_data();

void adc_get_calibration_value(bool dif_flag, float *gain_value, float *dco_value);

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_ADC_H */

/**
 * @} 
 * @}
 */
