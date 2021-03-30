#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tremo_rcc.h"
#include "tremo_lpuart.h"

/**
 * @brief  LPUART receive data
 * @param  lpuart LPUART handler
 * @return received data
 */
uint8_t lpuart_receive_data(lpuart_t* lpuart)
{
    uint8_t data = 0;

    if (lpuart_get_rx_not_empty_status(lpuart) == true) {
        data = lpuart->DATA;
    }

    return data;
}

/**
 * @brief  LPUART send data
 * @param  lpuart LPUART handler
 * @param  data data need to send
 * @return
 */
void lpuart_send_data(lpuart_t* lpuart, uint8_t data)
{
    if (lpuart_get_tx_empty_status(lpuart) == true) {
        lpuart->DATA = data;
    }
}

/**
 * @brief  Config dma
 * @param  lpuart LPUART handler
 * @param  dma dma configuration
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void lpuart_config_dma(lpuart_t* lpuart, lpuart_dma_t dma, bool new_state)
{
    TREMO_REG_EN(lpuart->CR1, dma, new_state);
}

/**
 * @brief  Config interrupt
 * @param  lpuart LPUART handler
 * @param  interrupt interrupt configuration
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void lpuart_config_interrupt(lpuart_t* lpuart, lpuart_it_t interrupt, bool new_state)
{
    TREMO_REG_EN(lpuart->CR1, interrupt, new_state);
}

/**
 * @brief  Config rts
 * @param  lpuart LPUART handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void lpuart_config_rts(lpuart_t* lpuart, bool new_state)
{
    while (((lpuart->SR1) & (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        != (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        ;
    TREMO_REG_EN(lpuart->CR0, LPUART_CR0_RTS_ENABLE, new_state);
    while (((lpuart->SR1) & (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        != (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        ;
}

/**
 * @brief  Config rx
 * @param  lpuart LPUART handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void lpuart_config_rx(lpuart_t* lpuart, bool new_state)
{
    while (((lpuart->SR1) & (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        != (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        ;
    TREMO_REG_EN(lpuart->CR0, LPUART_CR0_RX_ENABLE, new_state);
    while (((lpuart->SR1) & (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        != (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        ;
}

/**
 * @brief  Config cts
 * @param  lpuart LPUART handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void lpuart_config_cts(lpuart_t* lpuart, bool new_state)
{
    TREMO_REG_EN(lpuart->CR1, LPUART_CR1_CTS_ENABLE, new_state);
}

/**
 * @brief  Config tx
 * @param  lpuart LPUART handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void lpuart_config_tx(lpuart_t* lpuart, bool new_state)
{
    TREMO_REG_EN(lpuart->CR1, LPUART_CR1_TX_ENABLE, new_state);
}

/**
 * @brief  Get rx status
 * @param  lpuart LPUART handler
 * @param  rx_status rx status
 * @retval true set status
 * @retval false reset status
 */
bool lpuart_get_rx_status(lpuart_t* lpuart, lpuart_rx_status_t rx_status)
{
    while (((lpuart->SR1) & LPUART_SR1_WRITE_SR0_STATE) != LPUART_SR1_WRITE_SR0_STATE)
        ;
    if (lpuart->SR0 & rx_status) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Clear rx status
 * @param  lpuart LPUART handler
 * @param  rx_status rx status
 * @return 
 */
void lpuart_clear_rx_status(lpuart_t* lpuart, lpuart_rx_status_t rx_status)
{
    while (((lpuart->SR1) & (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        != (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        ;
    lpuart->SR0 = rx_status;
    while (((lpuart->SR1) & (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        != (LPUART_SR1_WRITE_SR0_STATE | LPUART_SR1_WRITE_CR0_STATE))
        ;
}

/**
 * @brief  Get rx not empty status
 * @param  lpuart LPUART handler
 * @retval true set status
 * @retval false reset status
 */
bool lpuart_get_rx_not_empty_status(lpuart_t* lpuart)
{
    if (lpuart->SR1 & LPUART_SR1_RX_NOT_EMPTY_STATE) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Get tx empty status
 * @param  lpuart LPUART handler
 * @retval true set status
 * @retval false reset status
 */
bool lpuart_get_tx_empty_status(lpuart_t* lpuart)
{
    if (lpuart->SR1 & LPUART_SR1_TX_EMPTY_STATE) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Get tx done status
 * @param  lpuart LPUART handler
 * @retval true set status
 * @retval false reset status
 */
bool lpuart_get_tx_done_status(lpuart_t* lpuart)
{
    if (lpuart->SR1 & LPUART_SR1_TX_DONE_STATE) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  Clear tx done status
 * @param  lpuart LPUART handler
 * @return
 */
void lpuart_clear_tx_done_status(lpuart_t* lpuart)
{
    TREMO_REG_EN(lpuart->SR1, LPUART_SR1_TX_DONE_STATE, true);
}

/**
 * @brief  Initialize lpuart interface
 * @param  lpuart LPUART handler
 * @param  uart_init the interface configuration
 * @return
 */
void lpuart_init(lpuart_t* lpuart, lpuart_init_t* uart_init)
{
    uint16_t ibaud, fbaud;
    uint32_t tmp_value                      = 0;
    rcc_lpuart_clk_source_t lpuart_clk_freq = 0;
    uint32_t freq                           = 0;

    lpuart_clk_freq = rcc_get_lpuart_clk_source();
    if (lpuart_clk_freq == RCC_CR1_LPUART_CLK_SEL_XO32K) {
        freq = 32768;
    } else if (lpuart_clk_freq == RCC_CR1_LPUART_CLK_SEL_RCO32K) {
        freq = 32000;
    } else {
        freq = 4000000;
    }
    if (uart_init->low_level_wakeup == true) {
        tmp_value |= LPUART_CR0_LOW_LEVEL_WAKEUP;
    } else if (uart_init->start_wakeup == true) {
        tmp_value |= LPUART_CR0_START_WAKEUP;
    } else if (uart_init->rx_done_wakeup == true) {
        tmp_value |= LPUART_CR0_RX_DONE_WAKEUP;
    }

    // baudrate
    ibaud = freq / uart_init->baudrate;
    fbaud = ((freq % uart_init->baudrate) * 16 + uart_init->baudrate / 2) / uart_init->baudrate;

    tmp_value |= (ibaud << LPUART_BAUD_RATE_INT_POS) | (fbaud << LPUART_BAUD_RATE_FRA_POS) | (uart_init->stop_bits)
        | (uart_init->parity) | (uart_init->data_width);

    while (((lpuart->SR1) & 0x6) != 0x6)
        ;
    lpuart->CR0 = tmp_value;
}

/**
 * @brief  Deinitialize lpuart interface
 * @param  lpuart LPUART handler
 * @return
 */
void lpuart_deinit(lpuart_t* lpuart)
{
    if (lpuart == LPUART) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_LPUART, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_LPUART, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_LPUART, false);
    } else {
        return;
    }
}
