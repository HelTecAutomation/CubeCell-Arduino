#include "tremo_rcc.h"
#include "tremo_spi.h"

/**
 * @brief  SSP initialize
 * @param  init_struct initialization configuration
 * @return
 */
void ssp_init_struct(ssp_init_t* init_struct)
{
    init_struct->ssp_sclk      = 1000000; // 1M
    init_struct->ssp_role      = SSP_ROLE_MASTER;
    init_struct->ssp_format    = SSP_FRAME_FORMAT_SPI;
    init_struct->ssp_data_size = SSP_DATA_SIZE_8BIT;
    init_struct->ssp_clk_pol   = SPI_CLK_POLARITY_HIGH;
    init_struct->ssp_clk_phase = SPI_CLK_PHASE_2EDGE;
    init_struct->ssp_dma_tx_en = DISABLE;
    init_struct->ssp_dma_rx_en = DISABLE;
}

/**
 * @brief  Config interrupt
 * @param  SSPx SSP handler
 * @param  ssp_interrupt SSP interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_OVERRUN: RX fifo overrun interrupt
 *            @arg SSP_INTERRUPT_RX_TIMEOUT: RX timeout interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_TRIGGER: RX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_TX_FIFO_TRIGGER: TX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_ALL: All interrupt
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void ssp_config_interrupt(ssp_typedef_t* SSPx, uint8_t ssp_interrupt, uint8_t new_state)
{
    if (new_state == ENABLE)
        SSPx->IMSC |= ssp_interrupt;
    else
        SSPx->IMSC &= ~ssp_interrupt;
}

/**
 * @brief  Enable or disable SSP
 * @param  SSPx SSP handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void ssp_cmd(ssp_typedef_t* SSPx, uint8_t new_state)
{
    if (new_state == ENABLE)
        SSPx->CR1 |= (0x1 << 1);
    else
        SSPx->CR1 &= ~(0x1 << 1);
}

/**
 * @brief  SSP initialize
 * @param  SSPx SSP handler
 * @param  init_struct initialization configuration
 * @return
 */
void ssp_init(ssp_typedef_t* SSPx, ssp_init_t* init_struct)
{
    uint8_t scr;
    uint32_t ssp_clk_freq = 0;
    ssp_cmd(SSPx, DISABLE);
    ssp_config_interrupt(SSPx, SSP_INTERRUPT_ALL, DISABLE);
    ssp_clear_interrupt(SSPx, SSP_INTERRUPT_ALL);

    if (SSPx == SSP0) {
        ssp_clk_freq = rcc_get_clk_freq(RCC_PCLK0);
    } else {
        ssp_clk_freq = rcc_get_clk_freq(RCC_PCLK1);
    }

    /* set frame format */
    SSPx->CR0 &= ~(0x3 << 4); /* reset FRF to 0 */
    SSPx->CR0 |= init_struct->ssp_format;

    /* set sclk divider */
    scr = ((ssp_clk_freq / 2 / init_struct->ssp_sclk) - 1) ? ((ssp_clk_freq / 2 / init_struct->ssp_sclk) - 1) : 0;
    SSPx->CPSR &= ~0xff;       /* reset CPSR to 0 */
    SSPx->CPSR |= 0x2;         /* set CPSR to 2, shoule be even number between 2-254 */
    SSPx->CR0 &= ~(0xff << 8); /* reset SCR to 0 */
    SSPx->CR0 |= scr << 8;     /* set SCR to 0x7, serial clk = 16M/2/(1+7) = 1M */

    /* set sclk polarity & phase */
    SSPx->CR0 &= ~(0x3 << 6); /* reset SPI clk phase/polarity setting to mode 0 */
    SSPx->CR0 |= init_struct->ssp_clk_pol | init_struct->ssp_clk_phase;

    /* set data size */
    SSPx->CR0 &= ~(0xf); /* reset data size to 0 */
    SSPx->CR0 |= init_struct->ssp_data_size;

    if (init_struct->ssp_role == SSP_ROLE_MASTER) {
        SSPx->CR1 &= ~(0x1 << 2); /* reset master/slave select to 0, which is master mode */
    } else {
        SSPx->CR1 &= ~(0x1 << 2);    /* reset master/slave select to 0, which is master mode */
        SSPx->CR1 |= SSP_ROLE_SLAVE; /* set to slave role */
    }

    /* dma handshake config,
    should be enabled after dmac has been configured and ready */
    if (init_struct->ssp_dma_tx_en == ENABLE) {
        SSPx->DMA_CR |= SSP_DMA_TX_EN;
    } else {
        SSPx->DMA_CR &= ~SSP_DMA_TX_EN;
    }
    if (init_struct->ssp_dma_rx_en == ENABLE) {
        SSPx->DMA_CR |= SSP_DMA_RX_EN;
    } else {
        SSPx->DMA_CR &= ~SSP_DMA_RX_EN;
    }
}

/**
 * @brief  SSP deinitialize
 * @param  SSPx SSP handler
 * @return
 */
void ssp_deinit(ssp_typedef_t* SSPx)
{
    if (SSPx == SSP0) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_SSP0, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_SSP0, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_SSP0, false);
    } else if (SSPx == SSP1) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_SSP1, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_SSP1, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_SSP1, false);
    } else if (SSPx == SSP2) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_SSP2, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_SSP2, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_SSP2, false);
    } else {
        return;
    }
}

/**
 * @brief  SSP send data
 * @param  SSPx SSP handler
 * @param  tx_data data need to send
 * @param  len data data length
 * @return
 */
void ssp_send_data(ssp_typedef_t* SSPx, uint8_t* tx_data, uint16_t len)
{
    uint8_t data_size;

    data_size = (uint8_t)(SSPx->CR0 & 0xF);

    while (len--) {
        while (!(ssp_get_flag_status(SSPx, SSP_FLAG_TX_FIFO_NOT_FULL)))
            ; /* wait till tx fifo is not full, change to timeout mechanism */

        if (data_size > SSP_DATA_SIZE_8BIT) {
            SSPx->DR = *((uint16_t *) tx_data);
            tx_data = (uint8_t *)((uint16_t *)tx_data + 1);
        } else {
            SSPx->DR = *((uint8_t *) tx_data);
            tx_data = (uint8_t *)tx_data + 1;
        }
    }
}

/**
 * @brief  SSP receive data
 * @param  SSPx SSP handler
 * @param  rx_data received data
 * @param  len data data length
 * @return
 */
void ssp_receive_data(ssp_typedef_t* SSPx, uint8_t* rx_data, uint16_t len)
{
    uint8_t data_size;

    data_size = (uint8_t)(SSPx->CR0 & 0xF);

    while (len--) {
        while (!(ssp_get_flag_status(SSPx, SSP_FLAG_RX_FIFO_NOT_EMPTY)))
            ; /* wait till rx fifo is not empty, change to timeout mechanism */

        if (data_size > SSP_DATA_SIZE_8BIT) {
            *((uint16_t *) rx_data) = SSPx->DR;
            rx_data = (uint8_t *)((uint16_t *)rx_data + 1);
        } else {
            *((uint8_t *) rx_data) = SSPx->DR;
            rx_data = (uint8_t *)rx_data + 1;
        }
    }
}
