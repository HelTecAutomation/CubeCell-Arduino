/**
 ******************************************************************************
 * @file    tremo_dma_handshake.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of DMA module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup DMA
 * @{
 */

#ifndef __TREMO_DMA_HANDSHAKE_H_
#define __TREMO_DMA_HANDSHAKE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DMA hardware handshake definition
 */
typedef enum {
    DMA_HANDSHAKE_LORAC_TX = 4, /*!< Lorac tx dma hand shake*/
    DMA_HANDSHAKE_LORAC_RX,     /*!< Lorac rx dma hand shake*/
    DMA_HANDSHAKE_DACCTRL,      /*!< Dacctrl dma hand shake*/
    DMA_HANDSHAKE_ADCCTRL,      /*!< Adcctrl dma hand shake*/
    DMA_HANDSHAKE_SCC = 9,      /*!< Scc dma hand shake*/
    DMA_HANDSHAKE_I2C_2_TX,     /*!< I2C2 tx dma hand shake*/
    DMA_HANDSHAKE_I2C_2_RX,     /*!< I2C2 rx dma hand shake*/
    DMA_HANDSHAKE_I2C_1_TX,     /*!< I2C1 tx dma hand shake*/
    DMA_HANDSHAKE_I2C_1_RX,     /*!< I2C1 rx dma hand shake*/
    DMA_HANDSHAKE_I2C_0_TX,     /*!< I2C0 tx dma hand shake*/
    DMA_HANDSHAKE_I2C_0_RX,     /*!< I2C0 rx dma hand shake*/
    DMA_HANDSHAKE_SSP_2_TX,     /*!< SSP2 tx dma hand shake*/
    DMA_HANDSHAKE_SSP_2_RX,     /*!< SSP2 rx dma hand shake*/
    DMA_HANDSHAKE_SSP_1_TX,     /*!< SSP1 tx dma hand shake*/
    DMA_HANDSHAKE_SSP_1_RX,     /*!< SSP1 rx dma hand shake*/
    DMA_HANDSHAKE_SSP_0_TX,     /*!< SSP0 tx dma hand shake*/
    DMA_HANDSHAKE_SSP_0_RX,     /*!< SSP0 rx dma hand shake*/
    DMA_HANDSHAKE_LPUART_TX,    /*!< Lpuart tx dma hand shake*/
    DMA_HANDSHAKE_LPUART_RX,    /*!< Lpuart rx dma hand shake*/
    DMA_HANDSHAKE_UART_3_TX,    /*!< Uart3 tx dma hand shake*/
    DMA_HANDSHAKE_UART_3_RX,    /*!< Uart3 rx dma hand shake*/
    DMA_HANDSHAKE_UART_2_TX,    /*!< Uart2 tx dma hand shake*/
    DMA_HANDSHAKE_UART_2_RX,    /*!< Uart2 rx dma hand shake*/
    DMA_HANDSHAKE_UART_1_TX,    /*!< Uart1 tx dma hand shake*/
    DMA_HANDSHAKE_UART_1_RX,    /*!< Uart1 rx dma hand shake*/
    DMA_HANDSHAKE_UART_0_TX,    /*!< Uart0 tx dma hand shake*/
    DMA_HANDSHAKE_UART_0_RX,    /*!< Uart0 rx dma hand shake*/
    DMA_HANDSHAKE_TIMER_0_CH3,  /*!< TIM0 channel3 dma hand shake*/
    DMA_HANDSHAKE_TIMER_0_CH2,  /*!< TIM0 channel2 dma hand shake*/
    DMA_HANDSHAKE_TIMER_0_CH1,  /*!< TIM0 channel1 dma hand shake*/
    DMA_HANDSHAKE_TIMER_0_CH0,  /*!< TIM0 channel0 dma hand shake*/
    DMA_HANDSHAKE_TIMER_0_TRG,  /*!< TIM0 trigger dma hand shake*/
    DMA_HANDSHAKE_TIMER_0_UP,   /*!< TIM0 update dma hand shake*/
    DMA_HANDSHAKE_TIMER_1_CH3,  /*!< TIM1 channel3 dma hand shake*/
    DMA_HANDSHAKE_TIMER_1_CH2,  /*!< TIM1 channel2 dma hand shake*/
    DMA_HANDSHAKE_TIMER_1_CH1,  /*!< TIM1 channel1 dma hand shake*/
    DMA_HANDSHAKE_TIMER_1_CH0,  /*!< TIM1 channel0 dma hand shake*/
    DMA_HANDSHAKE_TIMER_1_TRG,  /*!< TIM1 trigger dma hand shake*/
    DMA_HANDSHAKE_TIMER_1_UP,   /*!< TIM1 update dma hand shake*/
    DMA_HANDSHAKE_TIMER_2_CH1,  /*!< TIM2 channel1 dma hand shake*/
    DMA_HANDSHAKE_TIMER_2_CH0,  /*!< TIM2 channel0 dma hand shake*/
    DMA_HANDSHAKE_TIMER_2_TRG,  /*!< TIM2 trigger dma hand shake*/
    DMA_HANDSHAKE_TIMER_2_UP,   /*!< TIM2 update dma hand shake*/
    DMA_HANDSHAKE_TIMER_3_CH1,  /*!< TIM3 channel1 dma hand shake*/
    DMA_HANDSHAKE_TIMER_3_CH0,  /*!< TIM3 channel0 dma hand shake*/
    DMA_HANDSHAKE_TIMER_3_TRG,  /*!< TIM3 trigger dma hand shake*/
    DMA_HANDSHAKE_TIMER_3_UP,   /*!< TIM3 update dma hand shake*/
    DMA_HANDSHAKE_BSTIMER_1_UP, /*!< Bstimer1 update dma hand shake*/
    DMA_HANDSHAKE_BSTIMER_0_UP  /*!< Bstimer0 update dma hand shake*/
} dma_hand_shake_t;

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_DMA_HANDSHAKE_H_ */

/**
 * @}
 * @}
 */
