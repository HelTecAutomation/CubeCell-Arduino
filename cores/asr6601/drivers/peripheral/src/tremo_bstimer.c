#include <string.h>
#include "tremo_bstimer.h"
#include "tremo_rcc.h"

/**
 * @brief  Bstimer initialize 
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  bstimer_init bstimer configuration
 * @return None
 */
void bstimer_init(bstimer_t *BSTIMERx, bstimer_init_t *bstimer_init)
{
    uint32_t tmpcr1;
    uint32_t tmpcr2;

    tmpcr1 = BSTIMERx->CR1;
    tmpcr2 = BSTIMERx->CR2;

    /* Select the master mode */
    tmpcr2 &= ~BSTIMER_MMS_RESV;
    tmpcr2 |= bstimer_init->bstimer_mms;

    /* Set the auto-reload preload */
    tmpcr1 &= ~BSTIMER_CR1_ARPE;
    if (bstimer_init->autoreload_preload == true) {
        tmpcr1 |= BSTIMER_CR1_ARPE;
    }

    BSTIMERx->CR1 = tmpcr1;
    BSTIMERx->CR2 = tmpcr2;

    /* Set the Autoreload value */
    BSTIMERx->ARR = (uint32_t)bstimer_init->period;

    /* Set the Prescaler value */
    BSTIMERx->PSC = bstimer_init->prescaler;
}

/**
 * @brief  Bstimer deinitialize 
 * @param  BSTIMERx the BSTIMER peripheral
 * @return None
 */
void bstimer_deinit(bstimer_t *BSTIMERx)
{
    if (BSTIMERx == BSTIMER0) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_BSTIMER0, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_BSTIMER0, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_BSTIMER0, false);
    } else if (BSTIMERx == BSTIMER1) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_BSTIMER1, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_BSTIMER1, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_BSTIMER1, false);
    } else {
        return;
    }
}

/**
 * @brief  Bstimer config one pulse mode 
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void bstimer_config_one_pulse(bstimer_t *BSTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        BSTIMERx->CR1 |= (uint32_t)BSTIMER_CR1_OPM;
    } else {
        BSTIMERx->CR1 &= (uint32_t) ~(uint32_t)BSTIMER_CR1_OPM;
    }
}

/**
 * @brief  Bstimer config counter overflow trigger update event 
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void bstimer_config_overflow_update(bstimer_t *BSTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        BSTIMERx->CR1 |= (uint32_t)BSTIMER_CR1_URS;
    } else {
        BSTIMERx->CR1 &= (uint32_t) ~(uint32_t)BSTIMER_CR1_URS;
    }
}

/**
 * @brief  Bstimer config update disable 
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void bstimer_config_update_disable(bstimer_t *BSTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        BSTIMERx->CR1 |= (uint32_t)BSTIMER_CR1_UDIS;
    } else {
        BSTIMERx->CR1 &= (uint32_t) ~(uint32_t)BSTIMER_CR1_UDIS;
    }
}

/**
 * @brief  Bstimer enable or disable 
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  state true or false
 * @return None
 */
void bstimer_cmd(bstimer_t *BSTIMERx, bool state)
{
    if (state == true) {
        BSTIMERx->CR1 |= (uint32_t)BSTIMER_CR1_CEN;
    } else {
        BSTIMERx->CR1 &= (uint32_t) ~(uint32_t)BSTIMER_CR1_CEN;
    }
}

/**
 * @brief  Bstimer config dma enable or disable
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void bstimer_config_dma(bstimer_t *BSTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        BSTIMERx->DIER |= (uint32_t)BSTIMER_DIER_UDE;
    } else {
        BSTIMERx->DIER &= (uint32_t) ~(uint32_t)BSTIMER_DIER_UDE;
    }
}

/**
 * @brief  Bstimer config interrupt enable or disable
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void bstimer_config_interrupt(bstimer_t *BSTIMERx, uint8_t state)
{
    if (state == ENABLE) {
        BSTIMERx->DIER |= (uint32_t)BSTIMER_DIER_UIE;
    } else {
        BSTIMERx->DIER &= (uint32_t) ~(uint32_t)BSTIMER_DIER_UIE;
    }
}

/**
 * @brief  Bstimer get SR register status
 * @param  BSTIMERx the BSTIMER peripheral
 * @param  status ISR register configuration
 * @retval true set status
 * @retval false reset status
 */
bool bstimer_get_status(bstimer_t *BSTIMERx, bstimer_sr_t status)
{
    if ((BSTIMERx->SR & status) == status) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Bstimer enable or disable event
 * @param  BSTIMERx select the BSTIMER peripheral
 * @param  egr_event EGR register event
 * @param  state ENABLE or DISABLE
 * @return None
 */
void bstimer_generate_event(bstimer_t *BSTIMERx, bstimer_egr_t egr_event, uint8_t state)
{
    if (state == ENABLE) {
        BSTIMERx->EGR |= (uint32_t)egr_event;
    } else {
        BSTIMERx->EGR &= (uint32_t) ~(uint32_t)egr_event;
    }
}
