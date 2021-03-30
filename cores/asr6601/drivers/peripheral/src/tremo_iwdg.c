#include "tremo_rcc.h"
#include "tremo_iwdg.h"

static void check_iwdg_sr_done()
{
    while ((IWDG->SR & 0xF) != 0xF)
        ;
}

/**
 * @brief Enable/Disable the IWDG interrupt
 * @param new_state The new state of the IWDG interrupt
 * @retval None
 */
void iwdg_config_interrupt(bool new_state)
{
    TREMO_REG_EN(IWDG->CR1, IWDG_CR1_RESET_REQ_INT_EN_MASK, new_state);
    NVIC_EnableIRQ(IWDG_IRQn);
    NVIC_SetPriority(IWDG_IRQn, 1);
}

/**
 * @brief Clear the IWDG interrupt status
 * @param None
 * @retval None
 */
void iwdg_clear_interrupt(void)
{
    IWDG->SR2 = IWDG_SR2_RESET_REQ_SR_MASK;
    while (!(IWDG->SR & IWDG_SR_WRITE_SR2_DONE))
        ;
}

/**
 * @brief Deinitializes the IWDG registers to the reset values
 * @param None
 * @retval None
 */
void iwdg_deinit(void)
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_IWDG, false);
    rcc_rst_peripheral(RCC_PERIPHERAL_IWDG, true);
    rcc_rst_peripheral(RCC_PERIPHERAL_IWDG, false);
}

/**
 * @brief Initializes the IWDG registers
 * @note The auto-reset function doesn't work in stop3 mode.
 * @param auto_reset If set to true, system will reset automatically when IWDG timeout
 * @retval None
 */
void iwdg_init(bool auto_reset)
{
    check_iwdg_sr_done();
    IWDG->CR &= ~IWDG_CR_START_MASK;

    check_iwdg_sr_done();
    IWDG->SR2 = IWDG_SR2_RESET_REQ_SR_MASK;
    check_iwdg_sr_done();

    TREMO_REG_EN(RCC->RST_CR, RCC_RST_CR_IWDG_RESET_REQ_EN_MASK, auto_reset);
    TREMO_REG_EN(IWDG->CR, IWDG_CR_RSTEN_MASK, auto_reset);
    check_iwdg_sr_done();
}

/**
 * @brief Set the prescaler of the IWDG
 * @param prescaler The prescaler.
 *          This parameter can be one of the following values:
 *           @arg IWDG_PRESCALER_4:   prescaler 4
 *           @arg IWDG_PRESCALER_8:   prescaler 8
 *           @arg IWDG_PRESCALER_16:  prescaler 16
 *           @arg IWDG_PRESCALER_32:  prescaler 32
 *           @arg IWDG_PRESCALER_64:  prescaler 64
 *           @arg IWDG_PRESCALER_128: prescaler 128
 *           @arg IWDG_PRESCALER_256: prescaler 256
 * @retval None
 */
void iwdg_set_prescaler(iwdg_prescaler_t prescaler)
{
    check_iwdg_sr_done();

    TREMO_REG_SET(IWDG->CR, IWDG_CR_PREDIV_MASK, prescaler);
}

/**
 * @brief Set the reload value of the IWDG
 * @param value The reload value between 0x0 and 0x0FFF
 * @retval None
 */
void iwdg_set_reload(uint32_t value)
{
    check_iwdg_sr_done();
    IWDG->MAX = value > IWDG_MAX_RELOAD ? IWDG_MAX_RELOAD : value;
}

/**
 * @brief Set the window value of the IWDG
 * @param value The window value between 0x0 and 0x0FFF
 * @retval None
 */
void iwdg_set_window_value(uint32_t value)
{
    check_iwdg_sr_done();

    IWDG->WIN = value > IWDG_MAX_RELOAD ? IWDG_MAX_RELOAD : value;
}

/**
 * @brief Reload the IWDG counter with the value in the reload register
 * @note This function is used to feed the watch dog
 * @param None
 * @retval None
 */
void iwdg_reload(void)
{
    if (IWDG->SR2 & IWDG_SR2_RESET_REQ_SR_MASK) {
        check_iwdg_sr_done();
        IWDG->SR2 = IWDG_SR2_RESET_REQ_SR_MASK;
    }

    check_iwdg_sr_done();
    IWDG->MAX = IWDG->MAX;
    idwgIrqCnt = 0;
}

/**
 * @brief Start the IWDG
 * @param None
 * @retval None
 */
void iwdg_start(void)
{
    check_iwdg_sr_done();

    IWDG->CR |= IWDG_CR_START_MASK | IWDG_CR_WKEN_MASK;

    while (!(IWDG->SR & IWDG_SR_WRITE_CR_DONE))
        ;
}

/**
 * @brief Stop the IWDG
 * @param None
 * @retval None
 */
void iwdg_stop(void)
{
    check_iwdg_sr_done();

    IWDG->CR &= ~IWDG_CR_START_MASK;

    while (!(IWDG->SR & IWDG_SR_WRITE_CR_DONE))
        ;
}

uint8_t idwgIrqCnt = 0;
bool autoFeedIdwg = false;
void innerWdtEnable(bool autofeed)
{
    rcc_enable_oscillator(RCC_OSC_XO32K, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_IWDG, true);

    iwdg_init(false);

    iwdg_set_prescaler(IWDG_PRESCALER_256);
    iwdg_set_reload(0x3FF); // 8s
    iwdg_start();
    iwdg_config_interrupt(true);
    autoFeedIdwg = autofeed;
}

void feedInnerWdt()
{
	iwdg_reload();
}