#include "tremo_rcc.h"
#include "tremo_dma.h"
#include "tremo_lptimer.h"

/**
 * @brief  Lptimer initialize
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  lptimer_init CFGR register configuration
 * @return None
 */
void lptimer_init(lptimer_t* LPTIMERx, lptimer_init_t* lptimer_init)
{
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    if (lptimer_init->count_by_external == true) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_COUNTMODE;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_COUNTMODE;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_PRESC_128;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR |= (uint32_t)(lptimer_init->prescaler);
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    if (lptimer_init->autoreload_preload == true) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_PRELOAD;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_PRELOAD;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    if (lptimer_init->wavpol_inverted == true) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_WAVPOL;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_WAVPOL;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer deinitialize
 * @param  LPTIMERx the LPTIMER peripheral
 * @return None
 */
void lptimer_deinit(lptimer_t* LPTIMERx)
{
    if (LPTIMERx == LPTIMER0) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_LPTIMER0, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_LPTIMER0, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_LPTIMER0, false);
    } else if (LPTIMERx == LPTIMER1) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_LPTIMER1, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_LPTIMER1, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_LPTIMER1, false);
    } else {
        return;
    }
}

/**
 * @brief  Lptimer enable or disable external clock filter
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_enable_clock_filter(lptimer_t* LPTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_CKFLT_EN;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_CKFLT_EN;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer enable or disable trigger filter
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_enable_trigger_filter(lptimer_t* LPTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_TRGFLT_EN;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_TRGFLT_EN;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config wakeup
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  wkup wakeup configuration
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_config_wakeup(lptimer_t* LPTIMERx, lptimer_wkup_t wkup, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->CFGR |= (uint32_t)wkup;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)wkup;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config timeout
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_config_timeout(lptimer_t* LPTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_TIMEOUT;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_TIMEOUT;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config wave
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_config_wave(lptimer_t* LPTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_WAVE;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_WAVE;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config encoder mode
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_config_encoder(lptimer_t* LPTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->CFGR |= (uint32_t)LPTIMER_CFGR_ENC;
    } else {
        LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CFGR_ENC;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config external trigger polarity
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  config_trigen trigger polarity configuration
 * @return None
 */
void lptimer_config_trigger_polarity(lptimer_t* LPTIMERx, lptimer_trigpolari_t config_trigpolari)
{
    LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_TRIGEN_RISING_FALLING;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR |= (uint32_t)config_trigpolari;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config trigger source
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  config_trigsel trigger source selection
 * @return None
 */
void lptimer_config_trigger_source(lptimer_t* LPTIMERx, lptimer_trigsel_t config_trigsel)
{
    LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_TRIGSEL_7;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR |= (uint32_t)config_trigsel;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config clock prescaler
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  config_presc clock prescaler configuration
 * @return None
 */
void lptimer_config_clock_prescaler(lptimer_t* LPTIMERx, lptimer_presc_t config_presc)
{
    LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_PRESC_128;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR |= (uint32_t)config_presc;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config trigger filter
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  config_trgflt trigger filter configuration
 * @return None
 */
void lptimer_config_trigger_filter(lptimer_t* LPTIMERx, lptimer_trgflt_t config_trgflt)
{
    LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_TRGFLT_8;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR |= (uint32_t)config_trgflt;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config external clock filter
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  config_ckflt clock filter configuration
 * @return None
 */
void lptimer_config_clock_filter(lptimer_t* LPTIMERx, lptimer_ckflt_t config_ckflt)
{
    LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CKFLT_8;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR |= (uint32_t)config_ckflt;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config clock polarity
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  config_ckpol clock filter configuration
 * @return None
 */
void lptimer_config_clock_polarity(lptimer_t* LPTIMERx, lptimer_ckpol_t config_ckpol)
{
    LPTIMERx->CFGR &= (uint32_t) ~(uint32_t)LPTIMER_CKPOL_RESV;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
    LPTIMERx->CFGR |= (uint32_t)config_ckpol;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CFGROK))
        ;
}

/**
 * @brief  Lptimer config enable or disable
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  state true or false
 * @return None
 */
void lptimer_cmd(lptimer_t* LPTIMERx, bool state)
{
    if (state == true) {
        LPTIMERx->CR |= (uint32_t)LPTIMER_CR_ENABLE;
    } else {
        LPTIMERx->CR &= (uint32_t) ~(uint32_t)LPTIMER_CR_ENABLE;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CROK))
        ;
}

/**
 * @brief  Lptimer config count mode
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  count_mode count mode
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_config_count_mode(lptimer_t* LPTIMERx, lptimer_mode_t count_mode, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->CR |= (uint32_t)count_mode;
    } else {
        LPTIMERx->CR &= (uint32_t) ~(uint32_t)count_mode;
    }
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CROK))
        ;
}

/**
 * @brief  Lptimer config interrupt
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  interrupt interrupt definition
 * @param  state ENABLE or DISABLE
 * @return None
 */
void lptimer_config_interrupt(lptimer_t* LPTIMERx, lptimer_it_t interrupt, uint8_t state)
{
    if (state == ENABLE) {
        LPTIMERx->IER |= (uint32_t)interrupt;
    } else {
        LPTIMERx->IER &= (uint32_t) ~(uint32_t)interrupt;
    }
}

/**
 * @brief  Lptimer clear interrupt status
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  interrupt interrupt configuration
 * @return None
 */
void lptimer_clear_interrupt(lptimer_t* LPTIMERx, lptimer_it_t interrupt)
{
    LPTIMERx->ICR |= (uint32_t)interrupt;
}

/**
 * @brief  Lptimer set ARR register
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  arr_value ARR register value
 * @return None
 */
void lptimer_set_arr_register(lptimer_t* LPTIMERx, uint16_t arr_value)
{
    LPTIMERx->ARR = (uint32_t)arr_value;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_ARROK))
        ;
}

/**
 * @brief  Lptimer set CMP register
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  cmp_value CMP register value
 * @return None
 */
void lptimer_set_cmp_register(lptimer_t* LPTIMERx, uint16_t cmp_value)
{
    LPTIMERx->CMP = (uint32_t)cmp_value;
    while (!lptimer_get_status(LPTIMERx, LPTIMER_ISR_CMPOK))
        ;
}

/**
 * @brief  Lptimer get status
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  status ISR register configuration
 * @return true or false
 */
bool lptimer_get_status(lptimer_t* LPTIMERx, lptimer_status_t status)
{
    if ((LPTIMERx->ISR & status) == status) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Lptimer get clear status success flag
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  status_flag status configuration
 * @retval true clear status successfully
 * @retval false clearing status
 */
bool lptimer_get_clear_status_flag(lptimer_t* LPTIMERx, lptimer_clear_status_flag_t status_flag)
{
    if ((LPTIMERx->CSR & status_flag) == status_flag) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Lptimer get interrupt status
 * @param  LPTIMERx the LPTIMER peripheral
 * @param  interrupt SR1 register configuration
 * @retval true generate interrupt
 * @retval false not interrupt
 */
bool lptimer_get_interrupt_status(lptimer_t* LPTIMERx, lptimer_it_t interrupt)
{
    if ((LPTIMERx->SR1 & interrupt) == interrupt) {
        return true;
    } else {
        return false;
    }
}
