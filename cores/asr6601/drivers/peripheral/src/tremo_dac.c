#include "tremo_rcc.h"
#include "tremo_dac.h"

/**
 * @brief Deinitializes the DAC registers to the reset values
 * @param None
 * @retval None
 */
void dac_deinit(void)
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_DAC, false);
    rcc_rst_peripheral(RCC_PERIPHERAL_DAC, true);
    rcc_rst_peripheral(RCC_PERIPHERAL_DAC, false);
}

/**
 * @brief Init the DAC peripheral registers according to the specified parameters
 * @param config DAC configuration
 * @retval None
 */
void dac_init(dac_config_t *config)
{
    if(config->trigger_src == DAC_TRIGGER_SRC_NONE) {
        TREMO_REG_EN(DAC->CR, DAC_CR_TRIG_EN_MASK, false);
    }else{
        TREMO_REG_SET(DAC->CR, DAC_CR_TRIG_SRC_SEL_MASK, config->trigger_src);
        TREMO_REG_EN(DAC->CR, DAC_CR_TRIG_EN_MASK, true);
    }
    TREMO_REG_SET(DAC->CR, DAC_CR_TRIG_TYPE_SEL_MASK, config->trigger_type);
    
    TREMO_REG_SET(DAC->CR, DAC_CR_WAVE_SEL_MASK, config->wave_type);
    TREMO_REG_SET(DAC->CR, DAC_CR_MASK_AMP_SEL_MASK, config->wave_level);
}

/**
 * @brief Enable or disable the DAC peripheral
 * @param new_state true or false
 * @retval None
 */
void dac_cmd(bool new_state)
{
    if(new_state) {
        TREMO_ANALOG_WR(0x11, ((TREMO_ANALOG_RD(0x11) & (~(3<<24)))));
        TREMO_ANALOG_WR(0x12, ((TREMO_ANALOG_RD(0x12) & (~(1<<8)))));
        TREMO_ANALOG_WR(0x27, ((TREMO_ANALOG_RD(0x27) & (~(3<<11))) | (3<<11)));
		
        TREMO_REG_EN(DAC->CR, DAC_CR_DAC_EN_MASK, true);
	}else{
        TREMO_REG_EN(DAC->CR, DAC_CR_DAC_EN_MASK, false);

        TREMO_ANALOG_WR(0x11, ((TREMO_ANALOG_RD(0x11) | ((3<<24)))));
        TREMO_ANALOG_WR(0x12, ((TREMO_ANALOG_RD(0x12) | ((1<<8)))));
        TREMO_ANALOG_WR(0x27, (TREMO_ANALOG_RD(0x27) & (~(3<<11))));		
	}
}

/**
 * @brief DAC software trigger
 * @param new_state true or false
 * @retval None
 */
void dac_software_trigger_cmd(bool new_state)
{
    TREMO_REG_EN(DAC->SWTRIGR, 0x1, new_state);
}

/**
 * @brief Enable or disable the DAC DMA function
 * @param new_state true or false
 * @retval None
 */
void dac_dma_cmd(bool new_state)
{
    TREMO_REG_EN(DAC->CR, DAC_CR_DMA_EN_MASK, new_state);
}

/**
 * @brief Write the 12-bit data to DAC
 * @param data The 12-bit data
 * @retval None 
 */
void dac_write_data(uint16_t data)
{
    DAC->DHR = data;
}

/**
 * @brief Read the output data of the DAC
 * @param None
 * @retval uint16_t The output data 
 */
uint16_t dac_read_output_data(void)
{
    return DAC->DOR;
}

/**
 * @brief Config the interrupt of the DAC
 * @param intr_type The specified interrupt
 *          This parameter can be one of the following values:
 *           @arg DAC_INTR_UNDERFLOW: Buffer underflow
 *           @arg DAC_INTR_EMPTY:     Buffer empty
 * @param new_state true or false
 * @retval None 
 */
void dac_config_interrupt(dac_intr_t intr_type, bool new_state)
{
    TREMO_REG_EN(DAC->CR, (1<<(intr_type+15)), new_state);
}

/**
 * @brief Get the interrupt status of the DAC
 * @param intr_type The specified interrupt
 *          This parameter can be one of the following values:
 *           @arg DAC_INTR_UNDERFLOW: Buffer underflow
 *           @arg DAC_INTR_EMPTY:     Buffer empty
 * @return SET or RESET  
 */
it_status_t dac_get_interrupt_status(dac_intr_t intr_type)
{
    if(DAC->SR&(1<<intr_type))
        return SET;
    else
        return RESET;
}

/**
 * @brief Clear the interrupt status of the specified DAC interrupt
 * @param intr_type The specified interrupt
 *          This parameter can be one of the following values:
 *           @arg DAC_INTR_UNDERFLOW: Buffer underflow
 *           @arg DAC_INTR_EMPTY:     Buffer empty
 * @retval None 
 */
void dac_clear_interrupt(dac_intr_t intr_type)
{
    DAC->SR = 1<<intr_type;
}
