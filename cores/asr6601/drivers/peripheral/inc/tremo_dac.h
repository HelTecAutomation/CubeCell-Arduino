/**
 ******************************************************************************
 * @file    tremo_dac.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of DAC module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup DAC
 * @{
 */
#ifndef __TREMO_DAC_H
#define __TREMO_DAC_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include "tremo_regs.h"

/**
 * @brief DAC trigger source
 */
typedef enum {
    DAC_TRIGGER_SRC_GPTIMER1_TRGO = DAC_CR_TRIG_SRC_SEL_GPTIMER1_TRGO, /*!< DAC trigger source is GPTIMER1_TRGO */
    DAC_TRIGGER_SRC_GPTIMER0_TRGO = DAC_CR_TRIG_SRC_SEL_GPTIMER0_TRGO, /*!< DAC trigger source is GPTIMER0_TRGO */
    DAC_TRIGGER_SRC_BSTIMER1_TRGO = DAC_CR_TRIG_SRC_SEL_BSTIMER1_TRGO, /*!< DAC trigger source is BSTIMER1_TRGO */
    DAC_TRIGGER_SRC_BSTIMER0_TRGO = DAC_CR_TRIG_SRC_SEL_BSTIMER0_TRGO, /*!< DAC trigger source is BSTIMER1_TRGO */
    DAC_TRIGGER_SRC_GPIO6         = DAC_CR_TRIG_SRC_SEL_GPIO6,         /*!< DAC trigger source is GPIO6 */
    DAC_TRIGGER_SRC_GPIO24        = DAC_CR_TRIG_SRC_SEL_GPIO24,        /*!< DAC trigger source is GPIO24 */
    DAC_TRIGGER_SRC_GPIO43        = DAC_CR_TRIG_SRC_SEL_GPIO43,        /*!< DAC trigger source is GPIO43 */
    DAC_TRIGGER_SRC_SOFTWARE      = DAC_CR_TRIG_SRC_SEL_SOFTWARE,      /*!< DAC trigger source is SOFTWARE */
    DAC_TRIGGER_SRC_NONE,                                              /*!< DAC trigger source is NONE */
} dac_trigger_src_t;   

/**
 * @brief DAC trigger type
 */
typedef enum {
    DAC_TRIGGER_TYPE_RISING_EDGE         = DAC_CR_TRIG_TYPE_SEL_RISING_EDGE,         /*!< DAC trigger type is rising edge */
    DAC_TRIGGER_TYPE_FALLING_EDGE        = DAC_CR_TRIG_TYPE_SEL_FALLING_EDGE,        /*!< DAC trigger type is falling edge */
    DAC_TRIGGER_TYPE_RISING_FALLING_EDGE = DAC_CR_TRIG_TYPE_SEL_RISING_FALLING_EDGE, /*!< DAC trigger type is both rising and falling edge */
} dac_trigger_type_t;  

/**
 * @brief DAC wave type
 */
typedef enum {
    DAC_WAVE_NONE     = DAC_CR_WAVE_SEL_NONE,     /*!< DAC wave type is none */
    DAC_WAVE_NOISE    = DAC_CR_WAVE_SEL_NOISE,    /*!< DAC wave type is noise wave */
    DAC_WAVE_TRIANGLE = DAC_CR_WAVE_SEL_TRIANGLE, /*!< DAC wave type is triangle wave */
} dac_wave_t; 

/**
 * @brief DAC wave level
 */
typedef enum {
    DAC_WAVE_LEVEL_1    = DAC_CR_MASK_AMP_SEL_1,    /*!< DAC wave level is 1 */
    DAC_WAVE_LEVEL_3    = DAC_CR_MASK_AMP_SEL_3,    /*!< DAC wave level is 3 */
    DAC_WAVE_LEVEL_7    = DAC_CR_MASK_AMP_SEL_7,    /*!< DAC wave level is 7 */
    DAC_WAVE_LEVEL_15   = DAC_CR_MASK_AMP_SEL_15,   /*!< DAC wave level is 15 */
    DAC_WAVE_LEVEL_31   = DAC_CR_MASK_AMP_SEL_31,   /*!< DAC wave level is 31 */
    DAC_WAVE_LEVEL_63   = DAC_CR_MASK_AMP_SEL_63,   /*!< DAC wave level is 63 */
    DAC_WAVE_LEVEL_127  = DAC_CR_MASK_AMP_SEL_127,  /*!< DAC wave level is 127 */
    DAC_WAVE_LEVEL_255  = DAC_CR_MASK_AMP_SEL_255,  /*!< DAC wave level is 255 */
    DAC_WAVE_LEVEL_511  = DAC_CR_MASK_AMP_SEL_511,  /*!< DAC wave level is 511 */
    DAC_WAVE_LEVEL_1023 = DAC_CR_MASK_AMP_SEL_1023, /*!< DAC wave level is 1023 */
} dac_wave_levle_t;

/**
 * @brief DAC interrupt
 */
typedef enum {
    DAC_INTR_UNDERFLOW = 0, /*!< DAC buffer underflow interrupt */
    DAC_INTR_EMPTY     = 1, /*!< DAC buffer empty interrupt */
} dac_intr_t;

/**
 * @brief DAC config
 */
typedef struct {
    dac_trigger_src_t trigger_src;   /*!< DAC trigger source */
    dac_trigger_type_t trigger_type; /*!< DAC trigger type */
    
    dac_wave_t wave_type;            /*!< DAC wave type */
    dac_wave_levle_t wave_level;     /*!< DAC wave level */
} dac_config_t;


void dac_deinit(void);
void dac_init(dac_config_t *config);

void dac_cmd(bool new_state);
void dac_software_trigger_cmd(bool new_state);
void dac_dma_cmd(bool new_state);

void dac_write_data(uint16_t data);
uint16_t dac_read_output_data(void);

void dac_config_interrupt(dac_intr_t intr_type, bool new_state);
it_status_t dac_get_interrupt_status(dac_intr_t intr_type);
void dac_clear_interrupt(dac_intr_t intr_type);

#ifdef __cplusplus
}
#endif	
#endif //__TREMO_DAC_H

/**
 * @} 
 * @}
 */
