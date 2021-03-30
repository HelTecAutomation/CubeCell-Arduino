#include <stdio.h>
#include <stdbool.h>
#include "tremo_rcc.h"
#include "tremo_uart.h"

uint32_t calc_uart_baud(uint32_t uart_clk, uint32_t baud)
{
    uint32_t int_div;
    uint32_t fac_div;
    uint32_t remainder;
    uint32_t temp;

    temp = 16 * baud;
    if ((0 == baud) || uart_clk < temp) {
        return 0;
    }

    int_div   = (uint32_t)(uart_clk / temp);
    remainder = uart_clk % temp;
    temp      = 8 * remainder / baud;
    fac_div   = (temp >> 1) + (temp & 1);

    temp = ((int_div << 16) | (fac_div & 0xFFFF));
    return temp;
}


/**
 * @brief Get the flag status of the specified UART flag
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param uart_flag The specified flag
 *          This parameter can be one of the following values:
 *           @arg UART_FLAG_TX_FIFO_EMPTY: TX FIFO is empty
 *           @arg UART_FLAG_RX_FIFO_FULL:  RX FIFO is full
 *           @arg UART_FLAG_TX_FIFO_FULL:  TX FIFO is full
 *           @arg UART_FLAG_RX_FIFO_EMPTY: RX FIFO is empty
 *           @arg UART_FLAG_BUSY:          Busy
 * @return SET or RESET 
 */
flag_status_t uart_get_flag_status(uart_t* uartx, uint32_t uart_flag)
{
    if (uartx->FR & uart_flag)
        return SET;
    else
        return RESET;
}

/**
 * @brief Send 8 bit data through UART
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param data The data to be sent
 * @retval None 
 */
void uart_send_data(uart_t* uartx, uint8_t data)
{
    /* wait till tx fifo is not full */
    while (uart_get_flag_status(uartx, UART_FLAG_TX_FIFO_FULL) == SET)
        ;
    uartx->DR = data;
}

/**
 * @brief Receive 8 bit data through UART
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @retval uint8_t Data received 
 */
uint8_t uart_receive_data(uart_t* uartx)
{
    /* wait till rx fifo is not empty */
    while (uart_get_flag_status(uartx, UART_FLAG_RX_FIFO_EMPTY) == SET)
        ;

    return uartx->DR & 0xFF;
}

/**
 * @brief Config the interrupt of the specified UART flag
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param uart_interrupt The specified interrupt
 *          This parameter can be one of the following values:
 *           @arg UART_INTERRUPT_RX_DONE:      RX done
 *           @arg UART_INTERRUPT_TX_DONE:      TX done
 *           @arg UART_INTERRUPT_RX_TIMEOUT:   RX timeout
 *           @arg UART_INTERRUPT_FRAME_ERROR:  Frame error
 *           @arg UART_INTERRUPT_PARITY_ERROR: Busy
 * @param new_state true or false
 * @retval None 
 */
void uart_config_interrupt(uart_t* uartx, uint32_t uart_interrupt, bool new_state)
{
    if (new_state == DISABLE) {
        uartx->IMSC &= ~(uart_interrupt);
    } else {
        uartx->IMSC |= (uart_interrupt);
    }
}

/**
 * @brief Deinitializes the UART peripheral registers to the reset values
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @retval None
 */
void uart_deinit(uart_t* uartx)
{
    uint32_t peripheral = RCC_PERIPHERAL_UART0;

    if (uartx == UART1)
        peripheral = RCC_PERIPHERAL_UART1;
    else if (uartx == UART2)
        peripheral = RCC_PERIPHERAL_UART2;
    else if (uartx == UART3)
        peripheral = RCC_PERIPHERAL_UART3;

    rcc_enable_peripheral_clk(peripheral, false);
    rcc_rst_peripheral(peripheral, true);
    rcc_rst_peripheral(peripheral, false);
}

/**
 * @brief Set the default value of the UART configuration
 * @param config UART configuration
 * @retval None
 */
void uart_config_init(uart_config_t* config)
{
    config->data_width   = UART_DATA_WIDTH_8;
    config->baudrate     = UART_BAUDRATE_115200;
    config->parity       = UART_PARITY_NO;
    config->stop_bits    = UART_STOP_BITS_1;
    config->mode         = UART_MODE_TXRX;
    config->flow_control = UART_FLOW_CONTROL_DISABLED;
    config->fifo_mode    = DISABLE;
}

/**
 * @brief Set the threshold of RX FIFO
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param uart_fifo_level The threshold
 *        - @ref uart_rx_fifo_level_t
 * @retval None 
 */
void uart_set_rx_fifo_threshold(uart_t* uartx, uint32_t uart_fifo_level)
{
    TREMO_REG_SET(uartx->IFLS, UART_IFLS_RX, uart_fifo_level);
}

/**
 * @brief Set the threshold of TX FIFO
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param uart_fifo_level The threshold
 *        - @ref uart_tx_fifo_level_t
 * @retval None 
 */
void uart_set_tx_fifo_threshold(uart_t* uartx, uint32_t uart_fifo_level)
{
    TREMO_REG_SET(uartx->IFLS, UART_IFLS_TX, uart_fifo_level);
}

/**
 * @brief Enable or disable the UART peripheral
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param new_state true or false
 * @retval None
 */
void uart_cmd(uart_t* uartx, bool new_state)
{
    TREMO_REG_EN(uartx->CR, UART_CR_UART_EN, new_state);
}

/**
 * @brief Get the interrupt status of the specified UART interrupt
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param uart_interrupt The specified interrupt
 *          This parameter can be one of the following values:
 *           @arg UART_INTERRUPT_RX_DONE:      RX done
 *           @arg UART_INTERRUPT_TX_DONE:      TX done
 *           @arg UART_INTERRUPT_RX_TIMEOUT:   RX timeout
 *           @arg UART_INTERRUPT_FRAME_ERROR:  Frame error
 *           @arg UART_INTERRUPT_PARITY_ERROR: Busy
 * @return SET or RESET 
 */
it_status_t uart_get_interrupt_status(uart_t* uartx, uint32_t uart_interrupt)
{
    if (uartx->MIS & uart_interrupt)
        return SET;
    else
        return RESET;
}

/**
 * @brief Clear the interrupt status of the specified UART interrupt
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param uart_interrupt The specified interrupt
 *          This parameter can be one of the following values:
 *           @arg UART_INTERRUPT_RX_DONE:      RX done
 *           @arg UART_INTERRUPT_TX_DONE:      TX done
 *           @arg UART_INTERRUPT_RX_TIMEOUT:   RX timeout
 *           @arg UART_INTERRUPT_FRAME_ERROR:  Frame error
 *           @arg UART_INTERRUPT_PARITY_ERROR: Busy
 * @retval None 
 */
void uart_clear_interrupt(uart_t* uartx, uint32_t uart_interrupt)
{
    uartx->ICR = uart_interrupt;
}

/**
 * @brief Init the UART peripheral registers according to the specified parameters
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param config UART configuration
 * @retval ERRNO_OK Init successfully
 * @retval ERRNO_ERROR Init failed
 */
int32_t uart_init(uart_t* uartx, uart_config_t* config)
{
    uint32_t uart_clk_freq = 0;
    uint32_t clk_src       = 0;

    // disable UART
    uartx->CR &= ~UART_CR_UART_EN;
    // flush fifo by setting FEN = 0
    uartx->LCR_H &= ~UART_LCR_H_FEN;
    uartx->IMSC = 0;

    if (uartx == UART0)
        clk_src = (uint32_t)rcc_get_uart0_clk_source() >> 15;
    else if (uartx == UART1)
        clk_src = (uint32_t)rcc_get_uart1_clk_source() >> 13;
    else if (uartx == UART2)
        clk_src = (uint32_t)rcc_get_uart2_clk_source() >> 11;
    else if (uartx == UART3)
        clk_src = (uint32_t)rcc_get_uart3_clk_source() >> 9;

    switch (clk_src) {
    case 1:
        uart_clk_freq = RCC_FREQ_4M;
        break;
    case 2:
        uart_clk_freq = RCC_FREQ_32768;
        break;
    case 3:
        uart_clk_freq = RCC_FREQ_24M;
        break;
    case 0:
    default: {
        uart_clk_freq = (uartx == UART0 || uartx == UART1) ? rcc_get_clk_freq(RCC_PCLK0) : rcc_get_clk_freq(RCC_PCLK1);
        break;
    }
    }

    if (uart_clk_freq < 16 * config->baudrate)
        return ERRNO_ERROR;

    // set baudrate
    uint32_t br_div = calc_uart_baud(uart_clk_freq, config->baudrate);
    uartx->IBRD     = br_div >> 16; /* baudrate divdier register must be placed
                                       before a LCR_H write */
    uartx->FBRD = br_div & 0x3f;

    // set LCR_H
    TREMO_REG_SET(uartx->LCR_H, UART_LCR_H_WLEN, config->data_width);
    TREMO_REG_SET(uartx->LCR_H, UART_LCR_H_STOP, config->stop_bits);
    TREMO_REG_EN(uartx->LCR_H, UART_LCR_H_FEN, config->fifo_mode);
    switch (config->parity) {
    case UART_PARITY_ODD:
        uartx->LCR_H |= UART_LCR_H_PEN;
        uartx->LCR_H &= ~UART_LCR_H_EPS_EVEN;
        break;
    case UART_PARITY_EVEN:
        uartx->LCR_H |= UART_LCR_H_PEN;
        uartx->LCR_H |= UART_LCR_H_EPS_EVEN;
        break;
    case UART_PARITY_NO:
        uartx->LCR_H &= ~UART_LCR_H_PEN;
        break;
    default:
        break;
    }

    TREMO_REG_SET(uartx->CR, UART_CR_UART_MODE, config->mode);
    TREMO_REG_SET(uartx->CR, UART_CR_FLOW_CTRL, config->flow_control);

    return ERRNO_OK;
}

/**
 * @brief Config UART IRDA Mode
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param uart_irda_mode IRDA mode
 *          This parameter can be one of the following values:
 *           @arg UART_IRDA_MODE_NORMAL: Normal mode
 *           @arg UART_IRDA_MODE_LP:     Low power mode
 * @retval None
 */
void uart_irda_config(uart_t* uartx, uint32_t uart_irda_mode)
{
    uint32_t uart_clk_freq = 0;
    uint32_t clk_src       = 0;

    if (uartx == UART0)
        clk_src = (uint32_t)rcc_get_uart0_clk_source();
    else if (uartx == UART1)
        clk_src = (uint32_t)rcc_get_uart1_clk_source();
    else if (uartx == UART2)
        clk_src = (uint32_t)rcc_get_uart2_clk_source();
    else if (uartx == UART3)
        clk_src = (uint32_t)rcc_get_uart3_clk_source();

    switch (clk_src) {
    case 1:
        uart_clk_freq = RCC_FREQ_4M;
        break;
    case 2:
        uart_clk_freq = RCC_FREQ_32768;
        break;
    case 3:
        uart_clk_freq = RCC_FREQ_24M;
        break;
    case 0:
    default: {
        uart_clk_freq = (uartx == UART0 || uartx == UART1) ? rcc_get_clk_freq(RCC_PCLK0) : rcc_get_clk_freq(RCC_PCLK1);
        break;
    }
    }
    if (uart_irda_mode == UART_IRDA_MODE_LP) {
        uartx->ILPR = (uart_clk_freq + UART_IRDA_LPBAUD16 / 2) / UART_IRDA_LPBAUD16;
        uartx->CR |= UART_CR_SIR_LPIRDA_EN;
    } else {
        uartx->CR &= ~UART_CR_SIR_LPIRDA_EN;
    }
}

/**
 * @brief Enable or disable the UART IRDA
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param new_state true or false
 * @retval None
 */
void uart_irda_cmd(uart_t* uartx, bool new_state)
{
    TREMO_REG_EN(uartx->CR, UART_CR_SIR_EN, new_state);
}

/**
 * @brief Enable or disable the UART DMA function
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param dma_req UART DMA request
 *        - @ref uart_dma_req_t
 * @param new_state true or false
 * @retval None
 */
void uart_dma_config(uart_t* uartx, uart_dma_req_t dma_req, bool new_state)
{
    TREMO_REG_EN(uartx->DMACR, dma_req, new_state);
}

/**
 * @brief Enable or disable the UART DMA on error function
 * @param uartx Select the UART peripheral number(UART0, UART1, UART2 and UART3)
 * @param new_state true or false
 * @retval None
 */
void uart_dma_onerror_config(uart_t* uartx, bool new_state)
{
    TREMO_REG_EN(uartx->DMACR, UART_DMACR_ONERR_EN_MASK, new_state);
}
