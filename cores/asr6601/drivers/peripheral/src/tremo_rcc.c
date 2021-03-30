#include "tremo_rcc.h"
#include "lora_config.h"

/**
 * @brief Get the frequency of the specified clock
 * @param clk The clock type
 * @retval uint32_t The frequency of the clock(Hz)
 */
uint32_t rcc_get_clk_freq(rcc_clk_t clk)
{
    uint32_t freq        = 0;
    uint32_t sysclk_freq = 0;
    uint32_t tmp         = 0;

    tmp = RCC->CR0 & RCC_CR0_SYSCLK_SEL_MASK;
    switch (tmp) {
    case RCC_CR0_SYSCLK_SEL_RCO48M: {
        sysclk_freq = RCC_FREQ_48M;
        break;
    }
    case RCC_CR0_SYSCLK_SEL_RCO32K: {
        sysclk_freq = RCC_FREQ_32000;
        break;
    }
    case RCC_CR0_SYSCLK_SEL_XO32K: {
        sysclk_freq = RCC_FREQ_32768;
        break;
    }
    case RCC_CR0_SYSCLK_SEL_XO24M: {
        sysclk_freq = RCC_FREQ_24M;
        break;
    }
    case RCC_CR0_SYSCLK_SEL_XO32M: {
        sysclk_freq = RCC_FREQ_32M;
        break;
    }
	case RCC_CR0_SYSCLK_SEL_RCO4M: {
        sysclk_freq = RCC_FREQ_4M;
        break;
	}
	case RCC_CR0_SYSCLK_SEL_RCO48M_DIV2: 
	default: {
        sysclk_freq = RCC_FREQ_24M;
        break;
    }
    }

    switch (clk) {
    case RCC_HCLK: {
        tmp  = RCC->CR0 & RCC_CR0_HCLK_DIV_MASK;
        tmp  = tmp >> 8;
        freq = sysclk_freq >> tmp;
        break;
    }
    case RCC_PCLK0: {
        tmp  = RCC->CR0 & RCC_CR0_HCLK_DIV_MASK;
        tmp  = tmp >> 8;
        freq = sysclk_freq >> tmp;

        tmp = RCC->CR0 & RCC_CR0_PCLK0_DIV_MASK;
        tmp = tmp >> 5;
        freq >>= tmp;
        break;
    }
    case RCC_PCLK1: {
        tmp  = RCC->CR0 & RCC_CR0_HCLK_DIV_MASK;
        tmp  = tmp >> 8;
        freq = sysclk_freq >> tmp;

        tmp = RCC->CR0 & RCC_CR0_PCLK1_DIV_MASK;
        tmp = tmp >> 15;
        freq >>= tmp;
        break;
    }
    case RCC_SYS_CLK:
    default: {
        freq = sysclk_freq;
        break;
    }
    }

    return freq;
}

/**
 * @brief Enable/Disable the specified oscillator
 * @param osc The oscillator. 
 *          This parameter can be one of the following values:
 *           @arg RCC_OSC_RCO48M:  RCO48M(RCO48M is enabled by default)
 *           @arg RCC_OSC_RCO32K:  RCO32K
 *           @arg RCC_OSC_XO32K:   XO32K
 *           @arg RCC_OSC_XO24M:   XO24M
 *           @arg RCC_OSC_XO32M:   XO32M
 *           @arg RCC_OSC_RCO4M:   RCO4M
 * @param new_state true or false
 * @retval None
 */
void rcc_enable_oscillator(rcc_oscillator_t osc, bool new_state)
{
    uint32_t value = 0;

    switch (osc) {
    case RCC_OSC_RCO48M: {
        value = TREMO_ANALOG_RD(0x06);
        if (new_state) {
            TREMO_ANALOG_WR(0x06, value & (~(1 << 5)));
            while (!(AFEC->RAW_SR & AFEC_RAW_SR_RCO24M_READY_MASK))
                ;
        } else {
            TREMO_ANALOG_WR(0x06, value | (1 << 5));
            while (AFEC->RAW_SR & AFEC_RAW_SR_RCO24M_READY_MASK)
                ;
        }
        break;
    }
    case RCC_OSC_RCO32K: {
        value = TREMO_ANALOG_RD(0x02);
        TREMO_ANALOG_WR(0x02, new_state ? (value & (~(1 << 15))) : (value | (1 << 15)));
        break;
    }
    case RCC_OSC_XO32K: {
        value = TREMO_ANALOG_RD(0x02);
        TREMO_ANALOG_WR(0x02, new_state ? (value & (~(1 << 13)) & (~(1 << 14))) : (value | (1 << 13) | (1 << 14)));
        break;
    }
    case RCC_OSC_XO24M: {
        value = TREMO_ANALOG_RD(0x06);
        TREMO_ANALOG_WR(0x06, new_state ? ((value | (1<<3)) & (~(1<<4))) : ((value & (~(1<<3))) | ((1<<4))));
        break;
    }
    case RCC_OSC_XO32M: {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_LORA, true);

        if (new_state) {
            if(!(LORAC->CR1 & (0x00000020))){
				LORAC->CR1 |= 1<<5;  //nreset
				LORAC->CR1 &= ~(1<<7); //por
			}
            
#ifdef CONFIG_LORA_USE_TCXO
            LORAC->CR1 |= 0x3;
#endif      
            LORAC->CR1 |= 1 << 2;
            while (!(LORAC->SR & 1 << 1))
                ;
        } else {
            LORAC->CR1 &= ~(1 << 2);
            while (LORAC->SR & 1 << 1)
                ;
        }

        break;
    }
    case RCC_OSC_RCO4M: {
        value = TREMO_ANALOG_RD(0x06);
        if (new_state) {
            TREMO_ANALOG_WR(0x06, value & (~(1 << 6)));
            while (!(AFEC->RAW_SR & AFEC_RAW_SR_RCO4M_READY_MASK))
                ;
        } else {
            TREMO_ANALOG_WR(0x06, value | (1 << 6));
            while (AFEC->RAW_SR & AFEC_RAW_SR_RCO4M_READY_MASK)
                ;
        }
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Set the source of the SYSCLK
 * @param clk_source The source of the SYSCLK
 *          This parameter can be one of the following values:
 *           @arg RCC_SYS_CLK_SOURCE_RCO48M_DIV2: RCO48M/2(default)
 *           @arg RCC_SYS_CLK_SOURCE_RCO48M:      RCO48M
 *           @arg RCC_SYS_CLK_SOURCE_RCO32K:      RCO32K
 *           @arg RCC_SYS_CLK_SOURCE_XO32K:       XO32K
 *           @arg RCC_SYS_CLK_SOURCE_XO24M:       XO24M
 *           @arg RCC_SYS_CLK_SOURCE_XO32M:       XO32M
 *           @arg RCC_SYS_CLK_SOURCE_RCO4M:       RCO4M
 * @retval None
 */
void rcc_set_sys_clk_source(rcc_sys_clk_source_t clk_source)
{
    TREMO_REG_SET(RCC->CR0, RCC_CR0_SYSCLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the SYSTICK
 * @param clk_source The source of the SYSTICK
 *          This parameter can be one of the following values:
 *           @arg RCC_SYSTICK_SOURCE_XO32K: XO32K
 *           @arg RCC_SYSTICK_SOURCE_RCO32K: RCO32K
 *           @arg RCC_SYSTICK_SOURCE_HCLK: HCLK
 * @retval None
 */
void rcc_set_systick_source(rcc_systick_source_t clk_source)
{
    if (clk_source == RCC_SYSTICK_SOURCE_HCLK)
        SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    else
        TREMO_REG_SET(RCC->CR0, RCC_CR0_STCLKEN_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the MCO CLK
 * @param clk_source The source of the MCO CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_MCO_CLK_SOURCE_RCO32K: RCO32K
 *           @arg RCC_MCO_CLK_SOURCE_XO32K:  XO32K
 *           @arg RCC_MCO_CLK_SOURCE_RCO4M:  RCO4M
 *           @arg RCC_MCO_CLK_SOURCE_XO24M:  XO24M
 *           @arg RCC_MCO_CLK_SOURCE_XO32M:  XO32M
 *           @arg RCC_MCO_CLK_SOURCE_RCO48M: RCO48M
 *           @arg RCC_MCO_CLK_SOURCE_SYSCLK: SYSCLK
 * @retval None
 */
void rcc_set_mco_clk_source(rcc_mco_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_MCO_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CR0, RCC_CR0_MCO_CLK_OUT_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_MCO_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR0, RCC_CR0_MCO_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the UART0 CLK
 * @param clk_source The source of the UART0 CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_UART0_CLK_SOURCE_PCLK0: PCLK0
 *           @arg RCC_UART0_CLK_SOURCE_RCO4M: RCO4M
 *           @arg RCC_UART0_CLK_SOURCE_XO32K: XO32K
 *           @arg RCC_UART0_CLK_SOURCE_XO24M: XO24M
 * @retval None
 */
void rcc_set_uart0_clk_source(rcc_uart0_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_UART0_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART0_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_UART0_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR2, RCC_CR2_UART0_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the UART1 CLK
 * @param clk_source The source of the UART1 CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_UART1_CLK_SOURCE_PCLK0: PCLK0
 *           @arg RCC_UART1_CLK_SOURCE_RCO4M: RCO4M
 *           @arg RCC_UART1_CLK_SOURCE_XO32K: XO32K
 *           @arg RCC_UART1_CLK_SOURCE_XO24M: XO24M
 * @retval None
 */
void rcc_set_uart1_clk_source(rcc_uart1_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_UART1_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART1_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_UART1_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR2, RCC_CR2_UART1_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the UART2 CLK
 * @param clk_source The source of the UART2 CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_UART2_CLK_SOURCE_PCLK1: PCLK1
 *           @arg RCC_UART2_CLK_SOURCE_RCO4M: RCO4M
 *           @arg RCC_UART2_CLK_SOURCE_XO32K: XO32K
 *           @arg RCC_UART2_CLK_SOURCE_XO24M: XO24M
 * @retval None
 */
void rcc_set_uart2_clk_source(rcc_uart2_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_UART2_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART2_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_UART2_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR2, RCC_CR2_UART2_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the UART3 CLK
 * @param clk_source The source of the UART3 CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_UART3_CLK_SOURCE_PCLK1: PCLK1
 *           @arg RCC_UART3_CLK_SOURCE_RCO4M: RCO4M
 *           @arg RCC_UART3_CLK_SOURCE_XO32K: XO32K
 *           @arg RCC_UART3_CLK_SOURCE_XO24M: XO24M
 * @retval None
 */
void rcc_set_uart3_clk_source(rcc_uart3_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_UART3_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART3_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_UART3_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR2, RCC_CR2_UART3_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the LPTIMER0 CLK
 * @param clk_source The source of the LPTIMER0 CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_LPTIMER0_CLK_SOURCE_PCLK0:  PCLK0
 *           @arg RCC_LPTIMER0_CLK_SOURCE_RCO4M:  RCO4M
 *           @arg RCC_LPTIMER0_CLK_SOURCE_XO32K:  XO32K
 *           @arg RCC_LPTIMER0_CLK_SOURCE_RCO32K: RCO32K
 *           @arg RCC_LPTIMER0_CLK_SOURCE_EXTCLK: external clk
 * @retval None
 */
void rcc_set_lptimer0_clk_source(rcc_lptimer0_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_LPTIMER0_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER0_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_LPTIMER0_CLK_EN_SYNC)
            ;
    }

    if (clk_source == RCC_LPTIMER0_CLK_SOURCE_EXTCLK) {
        TREMO_REG_EN(RCC->CR1, RCC_CR1_LPTIMER0_EXTCLK_SEL_MASK, true);
    } else {
        TREMO_REG_SET(RCC->CR1, RCC_CR1_LPTIMER0_CLK_SEL_MASK, clk_source);
    }
}

/**
 * @brief Set the source of the LPTIMER1 CLK
 * @param clk_source The source of the LPTIMER1 CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_LPTIMER1_CLK_SOURCE_PCLK0:  PCLK0
 *           @arg RCC_LPTIMER1_CLK_SOURCE_RCO4M:  RCO4M
 *           @arg RCC_LPTIMER1_CLK_SOURCE_XO32K:  XO32K
 *           @arg RCC_LPTIMER1_CLK_SOURCE_RCO32K: RCO32K
 *           @arg RCC_LPTIMER1_CLK_SOURCE_EXTCLK: external clk
 * @retval None
 */
void rcc_set_lptimer1_clk_source(rcc_lptimer1_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_LPTIMER1_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER1_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_LPTIMER1_CLK_EN_SYNC)
            ;
    }

    if (clk_source == RCC_LPTIMER1_CLK_SOURCE_EXTCLK) {
        TREMO_REG_EN(RCC->CR1, RCC_CR1_LPTIMER1_EXTCLK_SEL_MASK, true);
    } else {
        TREMO_REG_SET(RCC->CR1, RCC_CR1_LPTIMER1_CLK_SEL_MASK, clk_source);
    }
}

/**
 * @brief Set the source of the LCD CLK
 * @param clk_source The source of the LCD CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_LCD_CLK_SOURCE_XO32K:  XO32K
 *           @arg RCC_LCD_CLK_SOURCE_RCO32K: RCO32K
 *           @arg RCC_LCD_CLK_SOURCE_RCO4M:  RCO4M
 * @retval None
 */
void rcc_set_lcd_clk_source(rcc_lcd_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_LCD_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_LCD_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_LCD_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR1, RCC_CR1_LCD_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the LPUART CLK
 * @param clk_source The source of the LPUART CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_LPUART_CLK_SOURCE_XO32K:  XO32K
 *           @arg RCC_LPUART_CLK_SOURCE_RCO32K: RCO32K
 *           @arg RCC_LPUART_CLK_SOURCE_RCO4M:  RCO4M
 * @retval None
 */
void rcc_set_lpuart_clk_source(rcc_lpuart_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_LPUART_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_LPUART_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_LPUART_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR1, RCC_CR1_LPUART_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the RTC CLK
 * @param clk_source The source of the RTC CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_RTC_CLK_SOURCE_XO32K:  XO32K
 *           @arg RCC_RTC_CLK_SOURCE_RCO32K: RCO32K
 * @retval None
 */
void rcc_set_rtc_clk_source(rcc_rtc_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_RTC_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_RTC_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_RTC_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR1, RCC_CR1_RTC_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the IWDG CLK
 * @param clk_source The source of the IWDG CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_IWDG_CLK_SOURCE_XO32K:  XO32K
 *           @arg RCC_IWDG_CLK_SOURCE_RCO32K: RCO32K
 * @retval None
 */
void rcc_set_iwdg_clk_source(rcc_iwdg_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_IWDG_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_IWDG_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_IWDG_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR1, RCC_CR1_IWDG_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the ADC CLK
 * @param clk_source The source of the ADC CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_ADC_CLK_SOURCE_PCLK1:  PCLK1
 *           @arg RCC_ADC_CLK_SOURCE_SYSCLK: SYSCLK
 *           @arg RCC_ADC_CLK_SOURCE_RCO48M: RCO48M
 * @retval None
 */
void rcc_set_adc_clk_source(rcc_adc_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_ADC_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_ADC_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_ADC_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR2, RCC_CR2_ADC_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the QSPI CLK
 * @param clk_source The source of the QSPI CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_QSPI_CLK_SOURCE_HCLK:   HCLK
 *           @arg RCC_QSPI_CLK_SOURCE_SYSCLK: SYSCLK
 * @retval None
 */
void rcc_set_qspi_clk_source(rcc_qspi_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_QSPI_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_QSPI_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_QSPI_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR2, RCC_CR2_QSPI_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Set the source of the I2S CLK
 * @param clk_source The source of the I2S CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_I2S_CLK_SOURCE_PCLK0:   PCLK0
 *           @arg RCC_I2S_CLK_SOURCE_XO24M:   XO24M
 *           @arg RCC_I2S_CLK_SOURCE_XO32M:   XO32M
 *           @arg RCC_I2S_CLK_SOURCE_EXT_CLK: External CLK
 * @retval None
 */
void rcc_set_i2s_clk_source(rcc_i2s_clk_source_t clk_source)
{
    if (RCC->SR1 & RCC_SR1_I2S_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_I2S_CLK_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_I2S_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR2, RCC_CR2_I2S_CLK_SEL_MASK, clk_source);
}

/**
 * @brief Get the source of the SYSCLK
 * @param None
 * @return The source of the SYSCLK
 *         - @ref rcc_sys_clk_source_t
 */
rcc_sys_clk_source_t rcc_get_sys_clk_source(void)
{
    return (RCC->CR0 & RCC_CR0_SYSCLK_SEL_MASK);
}

/**
 * @brief Get the source of the SYSTICK
 * @param None
 * @return The source of the SYSTICK
 *         - @ref rcc_systick_source_t
 */
rcc_systick_source_t rcc_get_systick_source(void)
{
    if (SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk)
        return RCC_SYSTICK_SOURCE_HCLK;

    return (RCC->CR0 & RCC_CR0_STCLKEN_SEL_MASK);
}

/**
 * @brief Get the source of the MCO CLK
 * @param None
 * @return The source of the MCO CLK
 *         - @ref rcc_mco_clk_source_t
 */
rcc_mco_clk_source_t rcc_get_mco_clk_source(void)
{
    return (RCC->CR0 & RCC_CR0_MCO_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the UART0 CLK
 * @param None
 * @return The source of the UART0 CLK
 *         - @ref rcc_uart0_clk_source_t
 */
rcc_uart0_clk_source_t rcc_get_uart0_clk_source(void)
{
    return (RCC->CR2 & RCC_CR2_UART0_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the UART1 CLK
 * @param None
 * @return The source of the UART1 CLK
 *         - @ref rcc_uart1_clk_source_t
 */
rcc_uart1_clk_source_t rcc_get_uart1_clk_source(void)
{
    return (RCC->CR2 & RCC_CR2_UART1_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the UART2 CLK
 * @param None
 * @return The source of the UART2 CLK
 *         - @ref rcc_uart2_clk_source_t
 */
rcc_uart2_clk_source_t rcc_get_uart2_clk_source(void)
{
    return (RCC->CR2 & RCC_CR2_UART2_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the UART3 CLK
 * @param None
 * @return The source of the UART3 CLK
 *         - @ref rcc_uart3_clk_source_t
 */
rcc_uart3_clk_source_t rcc_get_uart3_clk_source(void)
{
    return (RCC->CR2 & RCC_CR2_UART3_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the LPTIMER0 CLK
 * @param None
 * @return The source of the LPTIMER0 CLK
 *         - @ref rcc_lptim_clk_source_t
 */
rcc_lptimer0_clk_source_t rcc_get_lptimer0_clk_source(void)
{
    if (RCC->CR1 & RCC_CR1_LPTIMER0_EXTCLK_SEL_MASK)
        return RCC_LPTIMER0_CLK_SOURCE_EXTCLK;

    return (RCC->CR1 & RCC_CR1_LPTIMER0_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the LPTIMER1 CLK
 * @param None
 * @return The source of the LPTIMER1 CLK
 *         - @ref rcc_lptim1_clk_source_t
 */
rcc_lptimer1_clk_source_t rcc_get_lptimer1_clk_source(void)
{
    if (RCC->CR1 & RCC_CR1_LPTIMER1_EXTCLK_SEL_MASK)
        return RCC_LPTIMER1_CLK_SOURCE_EXTCLK;

    return (RCC->CR1 & RCC_CR1_LPTIMER1_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the LCD CLK
 * @param None
 * @return The source of the LCD CLK
 *         - @ref rcc_lcd_clk_source_t
 */
rcc_lcd_clk_source_t rcc_get_lcd_clk_source(void)
{
    return (RCC->CR1 & RCC_CR1_LCD_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the LPUART CLK
 * @param None
 * @return The source of the LPUART CLK
 *         - @ref rcc_lpuart_clk_source_t
 */
rcc_lpuart_clk_source_t rcc_get_lpuart_clk_source(void)
{
    return (RCC->CR1 & RCC_CR1_LPUART_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the RTC CLK
 * @param None
 * @return The source of the RTC CLK
 *         - @ref rcc_rtc_clk_source_t
 */
rcc_rtc_clk_source_t rcc_get_rtc_clk_source(void)
{
    return (RCC->CR1 & RCC_CR1_RTC_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the IWDG CLK
 * @param None
 * @return The source of the IWDG CLK
 *         - @ref rcc_iwdg_clk_source_t
 */
rcc_iwdg_clk_source_t rcc_get_iwdg_clk_source(void)
{
    return (RCC->CR1 & RCC_CR1_IWDG_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the ADC CLK
 * @param None
 * @return The source of the ADC CLK
 *         - @ref rcc_adc_clk_source_t
 */
rcc_adc_clk_source_t rcc_get_adc_clk_source(void)
{
    return (RCC->CR2 & RCC_CR2_ADC_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the QSPI CLK
 * @param None
 * @return The source of the QSPI CLK
 *         - @ref rcc_qspi_clk_source_t
 */
rcc_qspi_clk_source_t rcc_get_qspi_clk_source(void)
{
    return (RCC->CR2 & RCC_CR2_QSPI_CLK_SEL_MASK);
}

/**
 * @brief Get the source of the I2S CLK
 * @param None
 * @return The source of the I2S CLK
 *         - @ref rcc_i2s_clk_source_t
 */
rcc_i2s_clk_source_t rcc_get_i2s_clk_source(void)
{
    return (RCC->CR2 & RCC_CR2_I2S_CLK_SEL_MASK);
}

/**
 * @brief Set the divider of the HCLK
 * @param hclk_div The divider of the HCLK
 *          This parameter can be one of the following values:
 *           @arg RCC_HCLK_DIV_1: divider 1
 *           @arg RCC_HCLK_DIV_2: divider 2
 *           @arg RCC_HCLK_DIV_4: divider 4
 *           @arg RCC_HCLK_DIV_8: divider 8
 *           @arg RCC_HCLK_DIV_16: divider 16
 *           @arg RCC_HCLK_DIV_32: divider 32
 *           @arg RCC_HCLK_DIV_64: divider 64
 *           @arg RCC_HCLK_DIV_128: divider 128
 *           @arg RCC_HCLK_DIV_256: divider 256
 *           @arg RCC_HCLK_DIV_512: divider 512
 * @retval None
 */
void rcc_set_hclk_div(rcc_hclk_div_t hclk_div)
{
    TREMO_REG_SET(RCC->CR0, RCC_CR0_HCLK_DIV_MASK, hclk_div);
}

/**
 * @brief Set the divider of the PCLK
 * @param pclk0_div The divider of the PCLK0
 *          This parameter can be one of the following values:
 *           @arg RCC_PCLK0_DIV_1:  divider 1
 *           @arg RCC_PCLK0_DIV_2:  divider 2
 *           @arg RCC_PCLK0_DIV_4:  divider 4
 *           @arg RCC_PCLK0_DIV_8:  divider 8
 *           @arg RCC_PCLK0_DIV_16: divider 16
 * @param pclk1_div The divider of the PCLK1
 *          This parameter can be one of the following values:
 *           @arg RCC_PCLK1_DIV_1:  divider 1
 *           @arg RCC_PCLK1_DIV_2:  divider 2
 *           @arg RCC_PCLK1_DIV_4:  divider 4
 *           @arg RCC_PCLK1_DIV_8:  divider 8
 *           @arg RCC_PCLK1_DIV_16: divider 16
 * @retval None
 */
void rcc_set_pclk_div(rcc_pclk0_div_t pclk0_div, rcc_pclk1_div_t pclk1_div)
{
    TREMO_REG_SET(RCC->CR0, (RCC_CR0_PCLK0_DIV_MASK | RCC_CR0_PCLK1_DIV_MASK), (pclk0_div | pclk1_div));
}

/**
 * @brief Set the divider of the MCO CLK
 * @param mco_clk_div The divider of the MCO CLK
 *          This parameter can be one of the following values:
 *           @arg RCC_MCO_CLK_DIV_1:  divider 1
 *           @arg RCC_MCO_CLK_DIV_2:  divider 2
 *           @arg RCC_MCO_CLK_DIV_4:  divider 4
 *           @arg RCC_MCO_CLK_DIV_8:  divider 8
 *           @arg RCC_MCO_CLK_DIV_16: divider 16
 * @retval None
 */
void rcc_set_mco_clk_div(rcc_mco_clk_div_t mco_clk_div)
{
    if (RCC->SR1 & RCC_SR1_MCO_CLK_EN_SYNC) {
        TREMO_REG_EN(RCC->CR0, RCC_CR0_MCO_CLK_OUT_EN_MASK, false);
        while (RCC->SR1 & RCC_SR1_MCO_CLK_EN_SYNC)
            ;
    }
    TREMO_REG_SET(RCC->CR0, RCC_CR0_MCO_CLK_DIV_MASK, mco_clk_div);
}

/**
 * @brief Enable/Disable the clock of the specified peripheral
 * @param peripheral The specified peripheral
 *          This parameter can be one of the following values:
 *           @arg RCC_PERIPHERAL_SAC:    SAC
 *           @arg RCC_PERIPHERAL_SEC:    SEC
 *           @arg RCC_PERIPHERAL_CRC:    CRC
 *           @arg RCC_PERIPHERAL_RTC:    RTC
 *           @arg RCC_PERIPHERAL_WDG:    WDG
 *           @arg RCC_PERIPHERAL_IWDG:   IWDG
 *           @arg RCC_PERIPHERAL_LPTIMER0:  LPTIMER0
 *           @arg RCC_PERIPHERAL_BSTIMER1:  BSTIMER1
 *           @arg RCC_PERIPHERAL_BSTIMER0:  BSTIMER0
 *           @arg RCC_PERIPHERAL_TIMER3:  TIMER3
 *           @arg RCC_PERIPHERAL_TIMER2:  TIMER2
 *           @arg RCC_PERIPHERAL_TIMER1:  TIMER1
 *           @arg RCC_PERIPHERAL_TIMER0:  TIMER0
 *           @arg RCC_PERIPHERAL_GPIOA:  GPIOA
 *           @arg RCC_PERIPHERAL_GPIOB:  GPIOB
 *           @arg RCC_PERIPHERAL_GPIOC:  GPIOC
 *           @arg RCC_PERIPHERAL_GPIOD:  GPIOD
 *           @arg RCC_PERIPHERAL_LORA:   LORA
 *           @arg RCC_PERIPHERAL_DAC:    DAC
 *           @arg RCC_PERIPHERAL_LCD:    LCD
 *           @arg RCC_PERIPHERAL_AFEC:   AFEC
 *           @arg RCC_PERIPHERAL_ADC:    ADC
 *           @arg RCC_PERIPHERAL_I2C2:   I2C2
 *           @arg RCC_PERIPHERAL_I2C1:   I2C1
 *           @arg RCC_PERIPHERAL_I2C0:   I2C0
 *           @arg RCC_PERIPHERAL_QSPI:   QSPI
 *           @arg RCC_PERIPHERAL_SSP2:   SSP2
 *           @arg RCC_PERIPHERAL_SSP1:   SSP1
 *           @arg RCC_PERIPHERAL_SSP0:   SSP0
 *           @arg RCC_PERIPHERAL_LPUART: LPUART 
 *           @arg RCC_PERIPHERAL_UART3:  UART3
 *           @arg RCC_PERIPHERAL_UART2:  UART2
 *           @arg RCC_PERIPHERAL_UART1:  UART1
 *           @arg RCC_PERIPHERAL_UART0:  UART0
 *           @arg RCC_PERIPHERAL_DMA1:   DMA1
 *           @arg RCC_PERIPHERAL_DMA0:   DMA0
 *           @arg RCC_PERIPHERAL_I2S:    I2S
 *           @arg RCC_PERIPHERAL_RNGC:   RNGC
 *           @arg RCC_PERIPHERAL_LPTIMER1: LPTIMER1
 *           @arg RCC_PERIPHERAL_SYSCFG: SYSCFG
 *           @arg RCC_PERIPHERAL_PWR:    PWR
 * @param new_state true or false
 * @retval None
 */
void rcc_enable_peripheral_clk(uint8_t peripheral, bool new_state)
{
    switch (peripheral) {
    case RCC_PERIPHERAL_UART0: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART0_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_UART1: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART1_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_UART2: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART2_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_UART3: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_UART3_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_LPUART: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_LPUART_CLK_EN_MASK, new_state);

        while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
            ;
        TREMO_REG_EN(RCC->CGR2, RCC_CGR2_LPUART_AON_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_SSP0: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_SSP0_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_SSP1: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_SSP1_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_SSP2: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_SSP2_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_QSPI: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_QSPI_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_I2C0: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_I2C0_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_I2C1: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_I2C1_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_I2C2: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_I2C2_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_ADC: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_ADC_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_DAC: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_DAC_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_AFEC: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_AFEC_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_LCD: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_LCD_CLK_EN_MASK, new_state);

        while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
            ;
        TREMO_REG_EN(RCC->CGR2, RCC_CGR2_LCD_AON_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_LORA: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_LORA_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_GPIOA: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_IOM0_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_GPIOB: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_IOM1_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_GPIOC: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_IOM2_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_GPIOD: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_IOM3_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_TIMER0: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_TIMER0_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_TIMER1: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_TIMER1_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_TIMER2: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_TIMER2_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_TIMER3: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_TIMER3_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_BSTIMER0: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_BSTIMER0_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_BSTIMER1: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_BSTIMER1_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_LPTIMER0: {
        if (new_state) {
            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER0_PCLK_EN_MASK, new_state);

            while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
                ;
            TREMO_REG_EN(RCC->CGR2, RCC_CGR2_LPTIMER0_AON_CLK_EN_MASK, new_state);

            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER0_CLK_EN_MASK, new_state);
        } else {
            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER0_CLK_EN_MASK, new_state);

            while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
                ;
            TREMO_REG_EN(RCC->CGR2, RCC_CGR2_LPTIMER0_AON_CLK_EN_MASK, new_state);

            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER0_PCLK_EN_MASK, new_state);
        }

        break;
    }
    case RCC_PERIPHERAL_LPTIMER1: {

        if (new_state) {
            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER1_PCLK_EN_MASK, new_state);

            while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
                ;
            TREMO_REG_EN(RCC->CGR2, RCC_CGR2_LPTIMER1_AON_CLK_EN_MASK, new_state);

            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER1_CLK_EN_MASK, new_state);
        } else {
            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER1_CLK_EN_MASK, new_state);

            while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
                ;
            TREMO_REG_EN(RCC->CGR2, RCC_CGR2_LPTIMER1_AON_CLK_EN_MASK, new_state);

            TREMO_REG_EN(RCC->CGR1, RCC_CGR1_LPTIMER1_PCLK_EN_MASK, new_state);
        }

        break;
    }
    case RCC_PERIPHERAL_IWDG: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_IWDG_CLK_EN_MASK, new_state);

        while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
            ;
        TREMO_REG_EN(RCC->CGR2, RCC_CGR2_IWDG_CLK_EN_MASK, new_state);

        break;
    }
    case RCC_PERIPHERAL_WDG: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_WDG_CLK_EN_MASK, new_state);
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_WDG_CNT_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_RTC: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_RTC_CLK_EN_MASK, new_state);

        while ((RCC->SR & RCC_SR_ALL_DONE) != RCC_SR_ALL_DONE)
            ;
        TREMO_REG_EN(RCC->CGR2, RCC_CGR2_RTC_AON_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_CRC: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_CRC_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_SEC: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_SEC_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_SAC: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_SAC_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_I2S: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_I2S_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_DMA0: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_DMAC0_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_DMA1: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_DMAC1_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_RNGC: {
        TREMO_REG_EN(RCC->CGR1, RCC_CGR1_RNGC_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_SYSCFG: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_SYSCFG_CLK_EN_MASK, new_state);
        break;
    }
    case RCC_PERIPHERAL_PWR: {
        TREMO_REG_EN(RCC->CGR0, RCC_CGR0_PWR_CLK_EN_MASK, new_state);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Enable/Disable the output of the mco clk
 * @param new_state true or false
 * @retval None
 */
void rcc_enable_mco_clk_output(bool new_state)
{
    TREMO_REG_EN(RCC->CR0, RCC_CR0_MCO_CLK_OUT_EN_MASK, new_state);
}

/**
 * @brief Reset the register of the specified peripheral to the reset value
 * @param peripheral The specified peripheral
 *          This parameter can be one of the following values:
 *           @arg RCC_PERIPHERAL_SAC:    SAC
 *           @arg RCC_PERIPHERAL_SEC:    SEC
 *           @arg RCC_PERIPHERAL_CRC:    CRC
 *           @arg RCC_PERIPHERAL_RTC:    RTC
 *           @arg RCC_PERIPHERAL_WDG:    WDG
 *           @arg RCC_PERIPHERAL_IWDG:   IWDG
 *           @arg RCC_PERIPHERAL_LPTIMER0:  LPTIMER0
 *           @arg RCC_PERIPHERAL_BSTIMER1:  BSTIMER1
 *           @arg RCC_PERIPHERAL_BSTIMER0:  BSTIMER0
 *           @arg RCC_PERIPHERAL_TIMER3:  TIMER3
 *           @arg RCC_PERIPHERAL_TIMER2:  TIMER2
 *           @arg RCC_PERIPHERAL_TIMER1:  TIMER1
 *           @arg RCC_PERIPHERAL_TIMER0:  TIMER0
 *           @arg RCC_PERIPHERAL_GPIOA:  GPIOA
 *           @arg RCC_PERIPHERAL_GPIOB:  GPIOB
 *           @arg RCC_PERIPHERAL_GPIOC:  GPIOC
 *           @arg RCC_PERIPHERAL_GPIOD:  GPIOD
 *           @arg RCC_PERIPHERAL_LORA:   LORA
 *           @arg RCC_PERIPHERAL_DAC:    DAC
 *           @arg RCC_PERIPHERAL_LCD:    LCD
 *           @arg RCC_PERIPHERAL_AFEC:   AFEC
 *           @arg RCC_PERIPHERAL_ADC:    ADC
 *           @arg RCC_PERIPHERAL_I2C2:   I2C2
 *           @arg RCC_PERIPHERAL_I2C1:   I2C1
 *           @arg RCC_PERIPHERAL_I2C0:   I2C0
 *           @arg RCC_PERIPHERAL_QSPI:   QSPI
 *           @arg RCC_PERIPHERAL_SSP2:   SSP2
 *           @arg RCC_PERIPHERAL_SSP1:   SSP1
 *           @arg RCC_PERIPHERAL_SSP0:   SSP0
 *           @arg RCC_PERIPHERAL_LPUART: LPUART 
 *           @arg RCC_PERIPHERAL_UART3:  UART3
 *           @arg RCC_PERIPHERAL_UART2:  UART2
 *           @arg RCC_PERIPHERAL_UART1:  UART1
 *           @arg RCC_PERIPHERAL_UART0:  UART0
 *           @arg RCC_PERIPHERAL_DMA1:   DMA1
 *           @arg RCC_PERIPHERAL_DMA0:   DMA0
 *           @arg RCC_PERIPHERAL_I2S:    I2S
 *           @arg RCC_PERIPHERAL_RNGC:   RNGC
 *           @arg RCC_PERIPHERAL_LPTIMER1: LPTIMER1
 * @param new_state true or false
 * @retval None
 */
void rcc_rst_peripheral(uint8_t peripheral, bool new_state)
{
    if (peripheral >= RCC_PERIPHERAL_SYSCFG)
        return;

    if (peripheral >= RCC_PERIPHERAL_DMA1) {
        TREMO_REG_EN(RCC->RST1, 1 << (peripheral - RCC_PERIPHERAL_DMA1), !new_state);
    } else {
        uint32_t pos = peripheral;

        if (peripheral == RCC_PERIPHERAL_GPIOB || 
              peripheral == RCC_PERIPHERAL_GPIOC || 
              peripheral == RCC_PERIPHERAL_GPIOD) 
            pos = RCC_PERIPHERAL_GPIOA;

        TREMO_REG_EN(RCC->RST0, 1 << pos, !new_state);
    }
}

/**
 * @brief Set the reset mask
 * @param reset_mask The rest mask which can be a mixed value(eg. RCC_RESET_MASK_CPU|RCC_RESET_MASK_WDG)
 *        - @ref rcc_reset_mask_t
 * @retval None
 */
void rcc_set_reset_mask(uint32_t reset_mask)
{
    TREMO_REG_SET(RCC->RST_CR, RCC_RST_CR_RESET_REQ_EN_MASK, reset_mask);
}

/**
 * @brief Get the reset mask
 * @param None
 * @return The reset mask
 *         - @ref rcc_reset_mask_t
 */
uint32_t rcc_get_reset_mask(void)
{
    return (RCC->RST_CR & RCC_RST_CR_RESET_REQ_EN_MASK);
}

/**
 * @brief Set the divider of the I2S MCLK
 * @param div divider of the I2S MCLK
 *          This parameter can be one of the following values:
 *           @arg 0:  Divider is 1
 *           @arg 1:  Divider is 1 
 *           @arg 2:  Divider is 2
 *           @arg 3:  Divider is 3 
 *           @arg N:  Divider is N 
 * @retval None
 */
void rcc_set_i2s_mclk_div(uint8_t div)
{
    TREMO_REG_SET(RCC->CR3, RCC_CR3_I2S_MCLK_DIV_MASK, div << 8);
}

/**
 * @brief Set the divider of the I2S SCLK
 * @param div divider of the I2S SCLK
 *          This parameter can be one of the following values:
 *           @arg 0:  Divider is 1
 *           @arg 1:  Divider is 1 
 *           @arg 2:  Divider is 2
 *           @arg 3:  Divider is 3 
 *           @arg N:  Divider is N 
 * @retval None
 */
void rcc_set_i2s_sclk_div(uint8_t div)
{
    TREMO_REG_SET(RCC->CR3, RCC_CR3_I2S_SCLK_DIV_MASK, div);
}
