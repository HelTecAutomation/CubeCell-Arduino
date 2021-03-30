#include <string.h>
#include "tremo_rcc.h"
#include "tremo_timer.h"

/**
 * @brief  TIMERx initialize
 * @param  TIMERx select the TIMER peripheral
 * @param  timer_init timer configuration
 * @return None
 */
void timer_init(timer_gp_t* TIMERx, timer_init_t* timer_init)
{
    uint32_t tmpcr1;
    tmpcr1 = TIMERx->CR1;

    /* Select the Counter Mode */
    tmpcr1 &= ~TIMER_COUNTERMODE_CENTERALIGNED3;
    if ((timer_init->counter_mode == TIMER_COUNTERMODE_UP) || (timer_init->counter_mode == TIMER_COUNTERMODE_DOWN)) {
        tmpcr1 &= ~TIMER_DIR;
        if (timer_init->counter_mode == TIMER_COUNTERMODE_DOWN) {
            tmpcr1 |= TIMER_DIR;
        }
    } else {
        tmpcr1 |= timer_init->counter_mode;
    }

    /* Set the clock division */
    tmpcr1 &= ~TIMER_CKD_FPCLK_RESV;
    tmpcr1 |= timer_init->clock_division;

    /* Set the auto-reload preload */
    tmpcr1 &= ~TIMER_AUTORELOAD_PRELOAD;
    if (timer_init->autoreload_preload == true) {
        tmpcr1 |= TIMER_AUTORELOAD_PRELOAD;
    }

    TIMERx->CR1 = tmpcr1;

    /* Set the Autoreload value */
    TIMERx->ARR = (uint32_t)timer_init->period;

    /* Set the Prescaler value */
    TIMERx->PSC = timer_init->prescaler;
}

/**
 * @brief  TIMERx deinitialize
 * @param  TIMERx select the TIMER peripheral
 * @return None
 */
void timer_deinit(timer_gp_t* TIMERx)
{
    if (TIMERx == TIMER0) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_TIMER0, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER0, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER0, false);
    } else if (TIMERx == TIMER1) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_TIMER1, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER1, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER1, false);
    } else if (TIMERx == TIMER2) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_TIMER2, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER2, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER2, false);
    } else if (TIMERx == TIMER3) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_TIMER3, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER3, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_TIMER3, false);
    } else {
        return;
    }
}

/**
 * @brief  TIMERx enable or disable
 * @param  TIMERx select the TIMER peripheral
 * @param  state true or false
 * @return None
 */
void timer_cmd(timer_gp_t* TIMERx, bool state)
{
    if (state == true) {
        TIMERx->CR1 |= (uint32_t)TIMER_CEN;
    } else {
        TIMERx->CR1 &= (uint32_t) ~(uint32_t)TIMER_CEN;
    }
}

/**
 * @brief  TIMERx disable update event
 * @param  TIMERx select the TIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_disable_update(timer_gp_t* TIMERx, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->CR1 |= (uint32_t)TIMER_UDIS;
    } else {
        TIMERx->CR1 &= (uint32_t) ~(uint32_t)TIMER_UDIS;
    }
}

/**
 * @brief  TIMERx config update source
 * @param  TIMERx select the TIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_config_update_source(timer_gp_t* TIMERx, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->CR1 |= (uint32_t)TIMER_URS;
    } else {
        TIMERx->CR1 &= (uint32_t) ~(uint32_t)TIMER_URS;
    }
}

/**
 * @brief  TIMERx config master timer mode
 * @param  TIMERx select the TIMER peripheral
 * @param  master_mode master mode
 * @return None
 */
void timer_config_master_mode(timer_gp_t* TIMERx, timer_master_mode_t master_mode)
{
    TIMERx->CR2 &= (uint32_t) ~(uint32_t)TIMER_TRGO_OC3REF;
    TIMERx->CR2 |= (uint32_t)master_mode;
}

/**
 * @brief  TIMERx enable xor of channel 0
 * @param  TIMERx select the TIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_enable_xor(timer_gp_t* TIMERx, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->CR2 |= (uint32_t)TIMER_TI0S_XOR;
    } else {
        TIMERx->CR2 &= (uint32_t) ~(uint32_t)TIMER_TI0S_XOR;
    }
}

/**
 * @brief  TIMERx enable dma update request
 * @param  TIMERx select the TIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_enable_ccds_upd(timer_gp_t* TIMERx, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->CR2 |= (uint32_t)TIMER_CCDS_UPD;
    } else {
        TIMERx->CR2 &= (uint32_t) ~(uint32_t)TIMER_CCDS_UPD;
    }
}

/**
 * @brief  Configures the TIMERx External Trigger (ETR).
 * @param  TIMERx select the TIMER peripheral
 * @param  ext_trg_prescaler The external Trigger Prescaler.
 * @param  ext_trg_polarity The external Trigger Polarity.
 * @param  ext_trg_filter External Trigger Filter.
 * @return None
 */
void timer_config_etr(
    timer_gp_t* TIMERx, timer_etps_t ext_trg_prescaler, bool ext_trg_polarity, timer_etf_t ext_trg_filter)
{
    uint32_t tmpsmcr;

    tmpsmcr = TIMERx->SMCR;
    /* Reset the ETR Bits */
    tmpsmcr &= ~(TIMER_ETF_15 | TIMER_ETRPRESCALER_DIV8 | TIMER_ECE_ENABLE | TIMER_ETP_INVERTED);

    /* Set the Prescaler, the Filter value and the Polarity */
    tmpsmcr |= (uint32_t)(ext_trg_prescaler | ext_trg_filter);
    if (ext_trg_polarity == true) {
        tmpsmcr |= TIMER_ETP_INVERTED;
    }

    /* Write to TIMERx SMCR */
    TIMERx->SMCR = tmpsmcr;
}

/**
 * @brief  Config the External Clock2 mode enable or disable
 * @param  TIMERx select the TIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_config_external_clock2(timer_gp_t* TIMERx, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->SMCR |= (uint32_t)TIMER_ECE_ENABLE;
    } else {
        TIMERx->SMCR &= (uint32_t) ~(uint32_t)TIMER_ECE_ENABLE;
    }
}

/**
 * @brief  Config the Input Trigger source
 * @param  TIMERx select the TIMER peripheral
 * @param  input_trigger_source The Input Trigger source.
 * @return None
 */
void timer_config_itrx(timer_gp_t* TIMERx, timer_ts_t input_trigger_source)
{
    uint32_t tmpsmcr;

    /* Get the TIMERx SMCR register value */
    tmpsmcr = TIMERx->SMCR;
    /* Reset the TS Bits */
    tmpsmcr &= ~TIMER_TS_ETRF;
    /* Set the Input Trigger source */
    tmpsmcr |= input_trigger_source;
    /* Write to TIMERx SMCR */
    TIMERx->SMCR = tmpsmcr;
}

/**
 * @brief  Config the Slave Mode
 * @param  TIMERx select the TIMER peripheral
 * @param  slave_mode the slave mode
 * @return None
 */
void timer_config_slave_mode(timer_gp_t* TIMERx, timer_sms_t slave_mode)
{
    TIMERx->SMCR &= (uint32_t) ~(uint32_t)TIMER_SMS_EXTERNAL1;
    TIMERx->SMCR |= (uint32_t)slave_mode;
}

/**
 * @brief  Configure the channel as input.
 * @param  TIMERx select the TIM peripheral.
 * @param  ic_polarity specifies the input Polarity.
 *            @arg TIMER_CC0P_RISING: Channel 0 rising edge
 *            @arg TIMER_CC0P_FALLING: Channel 0 falling edge
 *            @arg TIMER_CC0P_BOTHEDGE: Channel 0 rising and falling edge
 *            @arg TIMER_CC1P_RISING: Channel 1 rising edge
 *            @arg TIMER_CC1P_FALLING: Channel 1 falling edge
 *            @arg TIMER_CC1P_BOTHEDGE: Channel 1 rising and falling edge
 *            @arg TIMER_CC2P_RISING: Channel 2 rising edge
 *            @arg TIMER_CC2P_FALLING: Channel 2 falling edge
 *            @arg TIMER_CC2P_BOTHEDGE: Channel 2 rising and falling edge
 *            @arg TIMER_CC3P_RISING: Channel 3 rising edge
 *            @arg TIMER_CC3P_FALLING: Channel 3 falling edge
 *            @arg TIMER_CC3P_BOTHEDGE: Channel 3 rising and falling edge
 * @param  ic_selection specifies the input to be used.
 *            @arg TIMER_CC0S_INPUT_SAME: Channel 0 or 2 iutput
 *            @arg TIMER_CC0S_INPUT_NEAR: Channel 0 or 2 iutput to the near channel
 *            @arg TIMER_CC0S_INPUT_TRC: Channel 0 or 2 iutput to TRC
 *            @arg TIMER_CC0S_INPUT_SAME: Channel 1 or 3 iutput
 *            @arg TIMER_CC0S_INPUT_NEAR: Channel 1 or 3 iutput to the near channel
 *            @arg TIMER_CC0S_INPUT_TRC: Channel 1 or 3 iutput to TRC
 * @param  ic_filter specifies the input capture filter.
 *            @arg timer_ic0f_t: Channel 0 or 2 iutput filter
 *            @arg timer_ic1f_t: Channel 1 or 3 iutput filter
 * @param  channel specifies the channel number.
 *            @arg TIMER_CHANNEL_0: Channel 0
 *            @arg TIMER_CHANNEL_1: Channel 1
 *            @arg TIMER_CHANNEL_2: Channel 2
 *            @arg TIMER_CHANNEL_3: Channel 3
 * @return None
 */
void timer_config_ti(
    timer_gp_t* TIMERx, uint32_t ic_polarity, uint32_t ic_selection, uint32_t ic_filter, uint8_t channel)
{
    uint32_t tmpccmr;
    uint32_t tmpccer;

    if (channel == TIMER_CHANNEL_0) {
        /* Disable the Channel 0*/
        TIMERx->CCER &= ~TIMER_CCER_CC0E;
        tmpccmr = TIMERx->CCMR1;
        tmpccer = TIMERx->CCER;

        tmpccmr &= ~TIMER_CC0S_INPUT_TRC;
        tmpccmr |= ic_selection;
        /* Set the filter */
        tmpccmr &= ~TIMER_IC0F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC0P_BOTHEDGE;
        tmpccer |= ic_polarity | TIMER_CCER_CC0E;
    } else if (channel == TIMER_CHANNEL_1) {
        /* Disable the Channel 1*/
        TIMERx->CCER &= ~TIMER_CCER_CC1E;
        tmpccmr = TIMERx->CCMR1;
        tmpccer = TIMERx->CCER;

        tmpccmr &= ~TIMER_CC1S_INPUT_TRC;
        tmpccmr |= ic_selection;
        /* Set the filter */
        tmpccmr &= ~TIMER_IC1F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC1P_BOTHEDGE;
        tmpccer |= ic_polarity | TIMER_CCER_CC1E;
    } else if (channel == TIMER_CHANNEL_2) {
        /* Disable the Channel 2*/
        TIMERx->CCER &= ~TIMER_CCER_CC2E;
        tmpccmr = TIMERx->CCMR2;
        tmpccer = TIMERx->CCER;

        tmpccmr &= ~TIMER_CC0S_INPUT_TRC;
        tmpccmr |= ic_selection;
        /* Set the filter */
        tmpccmr &= ~TIMER_IC0F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC2P_BOTHEDGE;
        tmpccer |= ic_polarity | TIMER_CCER_CC2E;
    } else {
        /* Disable the Channel 3*/
        TIMERx->CCER &= ~TIMER_CCER_CC3E;
        tmpccmr = TIMERx->CCMR2;
        tmpccer = TIMERx->CCER;

        tmpccmr &= ~TIMER_CC1S_INPUT_TRC;
        tmpccmr |= ic_selection;
        /* Set the filter */
        tmpccmr &= ~TIMER_IC1F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC3P_BOTHEDGE;
        tmpccer |= ic_polarity | TIMER_CCER_CC3E;
    }
    /*Write to TIMERx CCMR1/CCMR2 and CCER registers*/
    if ((channel == TIMER_CHANNEL_0) || (channel == TIMER_CHANNEL_1)) {
        TIMERx->CCMR1 = tmpccmr;
    } else {
        TIMERx->CCMR2 = tmpccmr;
    }
    TIMERx->CCER = tmpccer;
}

/**
 * @brief  Configure the polarity and filter for TI.
 * @param  TIMERx select the TIM peripheral.
 * @param  ic_polarity specifies the input Polarity.
 *            @arg TIMER_CC0P_RISING: Channel 0 rising edge
 *            @arg TIMER_CC0P_FALLING: Channel 0 falling edge
 *            @arg TIMER_CC0P_BOTHEDGE: Channel 0 rising and falling edge
 *            @arg TIMER_CC1P_RISING: Channel 1 rising edge
 *            @arg TIMER_CC1P_FALLING: Channel 1 falling edge
 *            @arg TIMER_CC1P_BOTHEDGE: Channel 1 rising and falling edge
 *            @arg TIMER_CC2P_RISING: Channel 2 rising edge
 *            @arg TIMER_CC2P_FALLING: Channel 2 falling edge
 *            @arg TIMER_CC2P_BOTHEDGE: Channel 2 rising and falling edge
 *            @arg TIMER_CC3P_RISING: Channel 3 rising edge
 *            @arg TIMER_CC3P_FALLING: Channel 3 falling edge
 *            @arg TIMER_CC3P_BOTHEDGE: Channel 3 rising and falling edge
 * @param  ic_filter specifies the input capture filter.
 *            @arg timer_ic0f_t: Channel 0 or 2 iutput filter
 *            @arg timer_ic1f_t: Channel 1 or 3 iutput filter
 * @param  channel specifies the channel number.
 *            @arg TIMER_CHANNEL_0: Channel 0
 *            @arg TIMER_CHANNEL_1: Channel 1
 *            @arg TIMER_CHANNEL_2: Channel 2
 *            @arg TIMER_CHANNEL_3: Channel 3
 * @return None
 */
void timer_config_polarity_filter_ti(timer_gp_t* TIMERx, uint32_t ic_polarity, uint32_t ic_filter, uint8_t channel)
{
    uint32_t tmpccmr;
    uint32_t tmpccer;

    if (channel == TIMER_CHANNEL_0) {
        /* Disable the Channel 0*/
        TIMERx->CCER &= ~TIMER_CCER_CC0E;
        tmpccmr = TIMERx->CCMR1;
        tmpccer = TIMERx->CCER;

        /* Set the filter */
        tmpccmr &= ~TIMER_IC0F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC0P_BOTHEDGE;
        tmpccer |= ic_polarity;

        /*Write to TIMERx CCMR1*/
        TIMERx->CCMR1 = tmpccmr;
    } else if (channel == TIMER_CHANNEL_1) {
        /* Disable the Channel 1*/
        TIMERx->CCER &= ~TIMER_CCER_CC1E;
        tmpccmr = TIMERx->CCMR1;
        tmpccer = TIMERx->CCER;

        /* Set the filter */
        tmpccmr &= ~TIMER_IC1F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC1P_BOTHEDGE;
        tmpccer |= ic_polarity;

        /*Write to TIMERx CCMR1*/
        TIMERx->CCMR1 = tmpccmr;
    } else if (channel == TIMER_CHANNEL_2) {
        /* Disable the Channel 2*/
        TIMERx->CCER &= ~TIMER_CCER_CC2E;
        tmpccmr = TIMERx->CCMR2;
        tmpccer = TIMERx->CCER;

        /* Set the filter */
        tmpccmr &= ~TIMER_IC0F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC2P_BOTHEDGE;
        tmpccer |= ic_polarity;

        /*Write to TIMERx CCMR2*/
        TIMERx->CCMR2 = tmpccmr;
    } else {
        /* Disable the Channel 3*/
        TIMERx->CCER &= ~TIMER_CCER_CC3E;
        tmpccmr = TIMERx->CCMR2;
        tmpccer = TIMERx->CCER;

        /* Set the filter */
        tmpccmr &= ~TIMER_IC1F_15;
        tmpccmr |= ic_filter;
        /*Select the polarity and enable the channel*/
        tmpccer &= ~TIMER_CC3P_BOTHEDGE;
        tmpccer |= ic_polarity;

        /*Write to TIMERx CCMR2*/
        TIMERx->CCMR2 = tmpccmr;
    }
    /*Write to TIMERx CCER registers*/
    TIMERx->CCER = tmpccer;
}

/**
 * @brief  Slave Timer configuration function
 * @param  TIMERx select the TIMER peripheral
 * @param  slave_config Slave timer configuration
 * @retval TIMER_OK config successfully
 * @retval TIMER_ERROR config failed
 */
int32_t timer_config_slave(timer_gp_t* TIMERx, timer_slave_config_t* slave_config)
{
    uint32_t tmpsmcr;
    uint32_t tmpccmr1;
    uint32_t tmpccer;

    /* Configure the trigger prescaler, filter, and polarity */
    switch (slave_config->input_trigger) {
    case TIMER_TS_ETRF: {
        /* Configure the ETR Trigger source */
        timer_config_etr(
            TIMERx, slave_config->trigger_prescaler, slave_config->trigger_polarity, slave_config->trigger_filter);
        break;
    }

    case TIMER_TS_TI0F_ED: {
        if (slave_config->slave_mode == TIMER_SMS_GATED) {
            return TIMER_ERROR;
        }
        /* Disable the Channel 0*/
        TIMERx->CCER &= ~TIMER_CCER_CC0E;
        tmpccer  = TIMERx->CCER;
        tmpccmr1 = TIMERx->CCMR1;

        /* Set the filter */
        tmpccmr1 &= ~TIMER_IC0F_15;
        tmpccmr1 |= (slave_config->ic_filter).ic0f_filter;
        tmpccer |= TIMER_CCER_CC0E;

        /* Write to TIMx CCMR1 and CCER registers */
        TIMERx->CCMR1 = tmpccmr1;
        TIMERx->CCER  = tmpccer;
        break;
    }

    case TIMER_TS_TI0FP0: {
        /* Configure TI0 Filter and Polarity */
        timer_config_polarity_filter_ti(
            TIMERx, (uint32_t)((slave_config->ic_polarity).cc0p_polarity), (uint32_t)((slave_config->ic_filter).ic0f_filter), TIMER_CHANNEL_0);
        break;
    }

    case TIMER_TS_TI1FP1: {
        /* Configure TI1 Filter and Polarity */
        timer_config_polarity_filter_ti(
            TIMERx, (uint32_t)(slave_config->ic_polarity.cc1p_polarity), (uint32_t)(slave_config->ic_filter.ic1f_filter), TIMER_CHANNEL_1);
        break;
    }

    case TIMER_TS_ITR0:
    case TIMER_TS_ITR1:
    case TIMER_TS_ITR2: {
        /* Check the parameter */
        break;
    }

    default:
        break;
    }

    /* Get the TIMx SMCR register value */
    tmpsmcr = TIMERx->SMCR;

    /* Reset the Trigger Selection Bits */
    tmpsmcr &= ~TIMER_TS_ETRF;
    /* Set the Input Trigger source */
    tmpsmcr |= slave_config->input_trigger;
    /* Write to TIMERx SMCR */
    TIMERx->SMCR = tmpsmcr;

    /* Get the TIMx SMCR register value */
    tmpsmcr = TIMERx->SMCR;
    /* Reset the slave mode Bits */
    tmpsmcr &= ~TIMER_SMS_EXTERNAL1;
    /* Set the slave mode */
    tmpsmcr |= slave_config->slave_mode;

    /* Write to TIMERx SMCR */
    TIMERx->SMCR = tmpsmcr;

    return TIMER_OK;
}

/**
 * @brief  Initializes the TIMER input capture channels
 * @param  TIMERx select the TIMER peripheral
 * @param  ic_init TIMER Input Capture configuration structure
 * @param  channel TIMER Channel to configure
 *            @arg TIMER_CHANNEL_0: Channel 0
 *            @arg TIMER_CHANNEL_1: Channel 1
 *            @arg TIMER_CHANNEL_2: Channel 2
 *            @arg TIMER_CHANNEL_3: Channel 3
 * @return None
 */
void timer_config_channel_ic(timer_gp_t* TIMERx, timer_ic_init_t* ic_init, uint8_t channel)
{
    if (channel == TIMER_CHANNEL_0) {
        /* TI0 Configuration */
        timer_config_ti(TIMERx, (uint32_t)((ic_init->ic_polarity).cc0p_polarity), (uint32_t)((ic_init->ic_selection).cc0s_selection),
            (uint32_t)((ic_init->ic_filter).ic0f_filter), channel);

        TIMERx->CCMR1 &= ~TIMER_IC0PSC_DIV8;
        TIMERx->CCMR1 |= (ic_init->ic_prescaler).ic0psc_prescaler;
    } else if (channel == TIMER_CHANNEL_1) {
        /* TI0 Configuration */
        timer_config_ti(TIMERx, (uint32_t)((ic_init->ic_polarity).cc1p_polarity), (uint32_t)((ic_init->ic_selection).cc1s_selection),
            (uint32_t)((ic_init->ic_filter).ic1f_filter), channel);

        TIMERx->CCMR1 &= ~TIMER_IC1PSC_DIV8;
        TIMERx->CCMR1 |= (ic_init->ic_prescaler).ic0psc_prescaler;
    } else if (channel == TIMER_CHANNEL_2) {
        /* TI0 Configuration */
        timer_config_ti(TIMERx, (uint32_t)((ic_init->ic_polarity).cc2p_polarity), (uint32_t)((ic_init->ic_selection).cc0s_selection),
            (uint32_t)((ic_init->ic_filter).ic0f_filter), channel);

        TIMERx->CCMR2 &= ~TIMER_IC0PSC_DIV8;
        TIMERx->CCMR2 |= (ic_init->ic_prescaler).ic0psc_prescaler;
    } else {
        /* TI0 Configuration */
        timer_config_ti(TIMERx, (uint32_t)((ic_init->ic_polarity).cc3p_polarity), (uint32_t)((ic_init->ic_selection).cc1s_selection),
            (uint32_t)((ic_init->ic_filter).ic1f_filter), channel);

        TIMERx->CCMR2 &= ~TIMER_IC1PSC_DIV8;
        TIMERx->CCMR2 |= (ic_init->ic_prescaler).ic0psc_prescaler;
    }
}

/**
 * @brief  Timer output compare configuration
 * @param  TIMERx select the TIM peripheral
 * @param  oc_init the ouput configuration structure
 * @param  channel TIMER Channel to configure
 *            @arg TIMER_CHANNEL_0: Channel 0
 *            @arg TIMER_CHANNEL_1: Channel 1
 *            @arg TIMER_CHANNEL_2: Channel 2
 *            @arg TIMER_CHANNEL_3: Channel 3
 * @return None
 */
void timer_config_oc(timer_gp_t* TIMERx, timer_oc_init_t* oc_init, uint8_t channel)
{
    uint32_t tmpccmrx;
    uint32_t tmpccer;
    uint32_t tmpcr2;

    /* Get the TIMERx CR2 register value */
    tmpcr2 = TIMERx->CR2;
    if (channel == TIMER_CHANNEL_0) {
        /* Disable the Channel*/
        TIMERx->CCER &= ~TIMER_CCER_CC0E;

        /* Get the TIMERx CCER register value */
        tmpccer = TIMERx->CCER;
        tmpccer |= TIMER_CCER_CC0E;
        tmpccmrx = TIMERx->CCMR1;

        /* Reset the Output Compare mode and Capture/Compare selection Bits */
        tmpccmrx &= ~TIMER_OC0M_PWM2;
        tmpccmrx &= ~TIMER_CC0S_INPUT_TRC;

        /* Select the Output Compare Mode */
        tmpccmrx |= (oc_init->oc_mode).oc0m_mode;

        /* Set the output mode */
        tmpccer &= ~TIMER_CCER_CC0NP;
        /* Reset the Output Polarity level */
        tmpccer &= ~TIMER_CCER_CC0P;
        /* Set the Output Compare Polarity */
        if (oc_init->high_level != true) {
            tmpccer |= TIMER_CCER_CC0P;
        }
        /* Set the Capture Compare Register value */
        TIMERx->CCR0  = oc_init->pulse;
        TIMERx->CCMR1 = tmpccmrx;
    } else if (channel == TIMER_CHANNEL_1) {
        /* Disable the Channel*/
        TIMERx->CCER &= ~TIMER_CCER_CC1E;

        /* Get the TIMERx CCER register value */
        tmpccer = TIMERx->CCER;
        tmpccer |= TIMER_CCER_CC1E;
        tmpccmrx = TIMERx->CCMR1;

        /* Reset the Output Compare mode and Capture/Compare selection Bits */
        tmpccmrx &= ~TIMER_OC1M_PWM2;
        tmpccmrx &= ~TIMER_CC1S_INPUT_TRC;

        /* Select the Output Compare Mode */
        tmpccmrx |= (oc_init->oc_mode).oc1m_mode;

        /* Set the output mode */
        tmpccer &= ~TIMER_CCER_CC1NP;
        /* Reset the Output Polarity level */
        tmpccer &= ~TIMER_CCER_CC1P;
        /* Set the Output Compare Polarity */
        if (oc_init->high_level != true) {
            tmpccer |= TIMER_CCER_CC1P;
        }
        /* Set the Capture Compare Register value */
        TIMERx->CCR1  = oc_init->pulse;
        TIMERx->CCMR1 = tmpccmrx;
    } else if (channel == TIMER_CHANNEL_2) {
        /* Disable the Channel*/
        TIMERx->CCER &= ~TIMER_CCER_CC2E;

        /* Get the TIMERx CCER register value */
        tmpccer = TIMERx->CCER;
        tmpccer |= TIMER_CCER_CC2E;
        tmpccmrx = TIMERx->CCMR2;

        /* Reset the Output Compare mode and Capture/Compare selection Bits */
        tmpccmrx &= ~TIMER_OC0M_PWM2;
        tmpccmrx &= ~TIMER_CC0S_INPUT_TRC;

        /* Select the Output Compare Mode */
        tmpccmrx |= (oc_init->oc_mode).oc0m_mode;

        /* Set the output mode */
        tmpccer &= ~TIMER_CCER_CC2NP;
        /* Reset the Output Polarity level */
        tmpccer &= ~TIMER_CCER_CC2P;
        /* Set the Output Compare Polarity */
        if (oc_init->high_level != true) {
            tmpccer |= TIMER_CCER_CC2P;
        }
        /* Set the Capture Compare Register value */
        TIMERx->CCR2  = oc_init->pulse;
        TIMERx->CCMR2 = tmpccmrx;
    } else {
        /* Disable the Channel*/
        TIMERx->CCER &= ~TIMER_CCER_CC3E;

        /* Get the TIMERx CCER register value */
        tmpccer = TIMERx->CCER;
        tmpccer |= TIMER_CCER_CC3E;
        tmpccmrx = TIMERx->CCMR2;

        /* Reset the Output Compare mode and Capture/Compare selection Bits */
        tmpccmrx &= ~TIMER_OC1M_PWM2;
        tmpccmrx &= ~TIMER_CC1S_INPUT_TRC;

        /* Select the Output Compare Mode */
        tmpccmrx |= (oc_init->oc_mode).oc1m_mode;

        /* Set the output mode */
        tmpccer &= ~TIMER_CCER_CC3NP;
        /* Reset the Output Polarity level */
        tmpccer &= ~TIMER_CCER_CC3P;
        /* Set the Output Compare Polarity */
        if (oc_init->high_level != true) {
            tmpccer |= TIMER_CCER_CC3P;
        }
        /* Set the Capture Compare Register value */
        TIMERx->CCR3  = oc_init->pulse;
        TIMERx->CCMR2 = tmpccmrx;
    }

    /* Write to TIMERx CR2 */
    TIMERx->CR2 = tmpcr2;
    /* Write to TIMERx CCER */
    TIMERx->CCER = tmpccer;
}

/**
 * @brief  One pulse configuration
 * @param  TIMERx select the TIMER peripheral
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_config_one_pulse(timer_gp_t* TIMERx, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->CR1 |= (uint32_t)TIMER_OPM;
    } else {
        TIMERx->CR1 &= (uint32_t) ~(uint32_t)(TIMER_OPM);
    }
}

/**
 * @brief  Ts trigger source configuration
 * @param  TIMERx select the TIMER peripheral
 * @param  ts to select trigger source
 * @return None
 */
void timer_config_ts(timer_gp_t* TIMERx, timer_ts_t ts)
{
    TIMERx->SMCR &= (uint32_t) ~(uint32_t)TIMER_TS_ETRF;
    TIMERx->SMCR |= (uint32_t)ts;
}

/**
 * @brief  Etr high level clear output
 * @param  TIMERx select the TIMER peripheral
 * @param  clear_ocxref TIMER clear ocxref to configure
 * @param  channel TIMER Channel to configure
 *            @arg TIMER_CHANNEL_0: Channel 0
 *            @arg TIMER_CHANNEL_1: Channel 1
 *            @arg TIMER_CHANNEL_2: Channel 2
 *            @arg TIMER_CHANNEL_3: Channel 3
 * @retval TIMER_OK clear successfully
 * @retval TIMER_ERROR clear failed
 */
int32_t timer_clear_ocxref(timer_gp_t* TIMERx, timer_clear_ocxref_t* clear_ocxref, uint8_t channel)
{
    if ((channel == 0) || (channel == 2)) {
        if (((clear_ocxref->oc_init).oc_mode.oc0m_mode != TIMER_OC0M_PWM1)
            && ((clear_ocxref->oc_init).oc_mode.oc0m_mode != TIMER_OC0M_PWM2)) {
            return TIMER_ERROR;
        }
        if (channel == 0) {
            TIMERx->CCMR1 |= TIMER_CCMR_OC0PE;
            TIMERx->CCMR1 |= TIMER_CCMR_OC0CE;
        } else {
            TIMERx->CCMR2 |= TIMER_CCMR_OC0PE;
            TIMERx->CCMR2 |= TIMER_CCMR_OC0CE;
        }
    } else {
        if (((clear_ocxref->oc_init).oc_mode.oc1m_mode != TIMER_OC1M_PWM1)
            && ((clear_ocxref->oc_init).oc_mode.oc1m_mode != TIMER_OC1M_PWM2)) {
            return TIMER_ERROR;
        }
        if (channel == 1) {
            TIMERx->CCMR1 |= TIMER_CCMR_OC1PE;
            TIMERx->CCMR1 |= TIMER_CCMR_OC1CE;
        } else {
            TIMERx->CCMR2 |= TIMER_CCMR_OC1PE;
            TIMERx->CCMR2 |= TIMER_CCMR_OC1CE;
        }
    }

    timer_config_oc(TIMERx, &(clear_ocxref->oc_init), channel);
    timer_config_etr(TIMERx, TIMER_ETRPRESCALER_DIV1, clear_ocxref->trigger_polarity, clear_ocxref->trigger_filter);

    return TIMER_OK;
}

/**
 * @brief  Timer PWM configuration
 * @param  TIMERx select the TIM peripheral
 * @param  oc_init the ouput configuration structure
 * @param  timerx_init TIMER initialization configuration
 * @param  channel TIMER Channel to configure
 *            @arg TIMER_CHANNEL_0: Channel 0
 *            @arg TIMER_CHANNEL_1: Channel 1
 *            @arg TIMER_CHANNEL_2: Channel 2
 *            @arg TIMER_CHANNEL_3: Channel 3
 * @return None
 */
void timer_config_pwm(timer_gp_t* TIMERx, timer_oc_init_t* oc_init, timer_init_t* timerx_init, uint8_t channel)
{
    if (((oc_init->oc_mode).oc0m_mode != TIMER_OC0M_PWM1) && ((oc_init->oc_mode).oc0m_mode != TIMER_OC0M_PWM2)) {
        return;
    }

    timer_config_oc(TIMERx, oc_init, channel);
    timer_init(TIMERx, timerx_init);
}

/**
 * @brief  Timer enable or disable dma
 * @param  TIMERx select the TIM peripheral
 * @param  dma DIER register channel event
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_config_dma(timer_gp_t* TIMERx, timer_dma_t dma, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->DIER |= (uint32_t)dma;
    } else {
        TIMERx->DIER &= (uint32_t) ~(uint32_t)dma;
    }
}

/**
 * @brief  Timer enable or disable interrupt
 * @param  TIMERx select the TIM peripheral
 * @param  interrupt DIER register channel event
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_config_interrupt(timer_gp_t* TIMERx, timer_interrupt_t interrupt, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->DIER |= (uint32_t)interrupt;
    } else {
        TIMERx->DIER &= (uint32_t) ~(uint32_t)interrupt;
    }
}

/**
 * @brief  Timer get status
 * @param  TIMERx select the TIM peripheral
 * @param  status SR register flag
 * @param  state true or false
 * @return None
 */
void timer_get_status(timer_gp_t* TIMERx, timer_sr_t status, bool* state)
{
    if (((TIMERx->SR) & status) == status) {
        *state = true;
    } else {
        *state = false;
    }
}

/**
 * @brief  Timer clear status
 * @param  TIMERx select the TIM peripheral
 * @param  status SR register flag
 * @return None
 */
void timer_clear_status(timer_gp_t* TIMERx, timer_sr_t status)
{
    TIMERx->SR &= (uint32_t) ~(uint32_t)status;
}

/**
 * @brief  Timer enable or disable event
 * @param  TIMERx select the TIM peripheral
 * @param  egr_event EGR register event
 * @param  state ENABLE or DISABLE
 * @return None
 */
void timer_generate_event(timer_gp_t* TIMERx, timer_egr_t egr_event, uint8_t state)
{
    if (state == ENABLE) {
        TIMERx->EGR |= (uint32_t)egr_event;
    } else {
        TIMERx->EGR &= (uint32_t) ~(uint32_t)egr_event;
    }
}

/**
 * @brief  Timer set dma read/write length
 * @param  TIMERx select the TIM peripheral
 * @param  dbl_len dma read/write length
 * @return None
 */
void timer_set_dma_rw_len(timer_gp_t* TIMERx, timer_dbl_t dbl_len)
{
    TIMERx->DCR &= (uint32_t) ~(uint32_t)TIMER_DBL_RESV;
    TIMERx->DCR |= (uint32_t)dbl_len;
}

/**
 * @brief  Timer set dma read/write base address
 * @param  TIMERx select the TIM peripheral
 * @param  dba_addr dma read/write base address
 * @return None
 */
void timer_set_dma_base_addr(timer_gp_t* TIMERx, timer_dba_t dba_addr)
{
    TIMERx->DCR &= (uint32_t) ~(uint32_t)TIMER_DBA_RES2;
    TIMERx->DCR |= (uint32_t)dba_addr;
}

/**
 * @brief  Timer config OR remap of TIM0 TI0
 * @param  TIMERx select the TIMER peripheral
 * @param  tim0_ti0_or timer0 TI0 OR remap configuration
 * @return None
 */
void timer_config_or_tim0_ti0(timer_gp_t* TIMERx, timer_tim0_ti0_or_t tim0_ti0_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM0_TI0_RESV;
    TIMERx->OR |= (uint32_t)tim0_ti0_or;
}

/**
 * @brief  Timer config OR remap of TIM0 TI3
 * @param  TIMERx select the TIMER peripheral
 * @param  tim0_ti3_or timer0 TI3 OR remap configuration
 * @return None
 */
void timer_config_or_tim0_ti3(timer_gp_t* TIMERx, timer_tim0_ti3_or_t tim0_ti3_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM0_TI3_RESV;
    TIMERx->OR |= (uint32_t)tim0_ti3_or;
}

/**
 * @brief  Timer config OR remap of TIM0 etr
 * @param  TIMERx select the TIMER peripheral
 * @param  tim0_etr_or timer0 etr OR remap configuration
 * @return None
 */
void timer_config_or_tim0_etr(timer_gp_t* TIMERx, timer_tim0_etr_or_t tim0_etr_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM0_ETR_RESV;
    TIMERx->OR |= (uint32_t)tim0_etr_or;
}

/**
 * @brief  Timer config OR remap of TIM1 TI2
 * @param  TIMERx select the TIMER peripheral
 * @param  tim1_ti2_or timer1 TI2 OR remap configuration
 * @return None
 */
void timer_config_or_tim1_ti2(timer_gp_t* TIMERx, timer_tim1_ti2_or_t tim1_ti2_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM1_TI2_RESV;
    TIMERx->OR |= (uint32_t)tim1_ti2_or;
}

/**
 * @brief  Timer config OR remap of TIM2 TI0
 * @param  TIMERx select the TIMER peripheral
 * @param  tim2_ti0_or timer2 TI0 OR remap configuration
 * @return None
 */
void timer_config_or_tim2_ti0(timer_gp_t* TIMERx, timer_tim2_ti0_or_t tim2_ti0_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM2_TI0_RESV;
    TIMERx->OR |= (uint32_t)tim2_ti0_or;
}

/**
 * @brief  Timer config OR remap of TIM2 TI1
 * @param  TIMERx select the TIMER peripheral
 * @param  tim2_ti1_or timer2 TI1 OR remap configuration
 * @return None
 */
void timer_config_or_tim2_ti1(timer_gp_t* TIMERx, timer_tim2_ti1_or_t tim2_ti1_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM2_TI1_RESV;
    TIMERx->OR |= (uint32_t)tim2_ti1_or;
}

/**
 * @brief  Timer config OR remap of TIM2 etr
 * @param  TIMERx select the TIMER peripheral
 * @param  tim2_etr_or timer2 etr OR remap configuration
 * @return None
 */
void timer_config_or_tim2_etr(timer_gp_t* TIMERx, timer_tim2_etr_or_t tim2_etr_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM2_ETR_RESV;
    TIMERx->OR |= (uint32_t)tim2_etr_or;
}

/**
 * @brief  Timer config OR remap of TIM3 TI0
 * @param  TIMERx select the TIMER peripheral
 * @param  tim3_ti0_or timer3 TI0 OR remap configuration
 * @return None
 */
void timer_config_or_tim3_ti0(timer_gp_t* TIMERx, timer_tim3_ti0_or_t tim3_ti0_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM3_TI0_UART4;
    TIMERx->OR |= (uint32_t)tim3_ti0_or;
}

/**
 * @brief  Timer config OR remap of TIM3 etr
 * @param  TIMERx select the TIMER peripheral
 * @param  tim3_etr_or timer3 etr OR remap configuration
 * @return None
 */
void timer_config_or_tim3_etr(timer_gp_t* TIMERx, timer_tim3_etr_or_t tim3_etr_or)
{
    TIMERx->OR &= (uint32_t) ~(uint32_t)TIMER_TIM3_ETR_RESV;
    TIMERx->OR |= (uint32_t)tim3_etr_or;
}
