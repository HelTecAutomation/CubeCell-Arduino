/**
 ******************************************************************************
 * @file    tremo_lpuart.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of LPUART module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup LPUART
 * @{
 */

#ifndef __TREMO_LPUART_H
#define __TREMO_LPUART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "tremo_regs.h"

#define LPUART_BAUDRATE_110  (110)  /*!< Baudrate 110*/
#define LPUART_BAUDRATE_300  (300)  /*!< Baudrate 300*/
#define LPUART_BAUDRATE_600  (600)  /*!< Baudrate 600*/
#define LPUART_BAUDRATE_1200 (1200) /*!< Baudrate 1200*/
#define LPUART_BAUDRATE_2400 (2400) /*!< Baudrate 2400*/
#define LPUART_BAUDRATE_4800 (4800) /*!< Baudrate 4800*/
#define LPUART_BAUDRATE_9600 (9600) /*!< Baudrate 9600*/

#define LPUART_BAUD_RATE_INT_MASK (0x3ffc00) /*!< Baudrate integer mask*/
#define LPUART_BAUD_RATE_INT_POS  (10)       /*!< Baudrate integer position*/
#define LPUART_BAUD_RATE_FRA_MASK (0x3c0)    /*!< Baudrate fraction mask*/
#define LPUART_BAUD_RATE_FRA_POS  (6)         /*!< Baudrate fraction position*/

/**
 * @brief LPUART data width
 */
typedef enum {
    LPUART_DATA_5BIT, /*!< Data width 5bit*/
    LPUART_DATA_6BIT, /*!< Data width 6bit*/
    LPUART_DATA_7BIT, /*!< Data width 7bit*/
    LPUART_DATA_8BIT  /*!< Data width 8bit*/
} lpuart_data_width_t;

/**
 * @brief LPUART parity
 */
typedef enum {
    LPUART_PARITY_EVEN    = 0x0, /*!< Even check*/
    LPUART_PARITY_ODD     = 0x4, /*!< ODD check*/
    LPUART_PARITY_STICK_0 = 0x8, /*!< Stick 0 check*/
    LPUART_PARITY_STICK_1 = 0xc, /*!< Stick 1 check*/
    LPUART_PARITY_NONE    = 0x1c /*!< None check*/
} lpuart_parity_t;

/**
 * @brief LPUART stop bits
 */
typedef enum {
    LPUART_STOP_1BIT = 0x0,  /*!< Stop 1bit*/
    LPUART_STOP_2BITS = 0x20 /*!< Stop 2bits*/
} lpuart_stop_bits_t;

/**
 * @brief LPUART CR0 register definition
 */
typedef enum {
    LPUART_CR0_LOW_LEVEL_WAKEUP = 0x400000,  /*!< rx low level wakeup*/
    LPUART_CR0_START_WAKEUP     = 0x800000,  /*!< start bit wakeup*/
    LPUART_CR0_RX_DONE_WAKEUP   = 0x1000000, /*!< receive done wakeup*/
    LPUART_CR0_RX_ENABLE        = 0x2000000, /*!< receive enable*/
    LPUART_CR0_RTS_ENABLE       = 0x4000000  /*!< rts enable*/
} lpuart_cr0_t;

/**
 * @brief LPUART interrupt definition
 */
typedef enum {
    LPUART_CR1_START_VALID_INT   = 0x1,  /*!< RX done interrupt*/
    LPUART_CR1_RX_DONE_INT       = 0x2,  /*!< RX done interrupt*/
    LPUART_CR1_START_INVALID_INT = 0x4,  /*!< RX start invalid interrupt*/
    LPUART_CR1_PARITY_ERR_INT    = 0x8,  /*!< RX parity error interrupt*/
    LPUART_CR1_STOP_ERR_INT      = 0x10, /*!< RX stop error interrupt*/
    LPUART_CR1_RX_OVERFLOW_INT   = 0x20, /*!< RX overflow interrupt*/
    LPUART_CR1_RX_NOT_EMPTY_INT  = 0x40, /*!< RX not empty interrupt*/
    LPUART_CR1_TX_EMPTY_INT      = 0x80, /*!< TX empty interrupt*/
    LPUART_CR1_TX_DONE_INT       = 0x100 /*!< TX done interrupt*/
} lpuart_it_t;

/**
 * @brief LPUART dma definition
 */
typedef enum {
    LPUART_CR1_TX_DMA = 0x400, /*!< TX DMA*/
    LPUART_CR1_RX_DMA = 0x800  /*!< RX DMA*/
} lpuart_dma_t;

/**
 * @brief LPUART CR1 register definition
 */
typedef enum {
    LPUART_CR1_TX_ENABLE = 0x200,  /*!< TX enable*/
    LPUART_CR1_CTS_ENABLE = 0x1000 /*!< CTX enable*/
} lpuart_cr1_t;

/**
 * @brief LPUART SR0 rx status definition
 */
typedef enum {
    LPUART_SR0_START_VALID_STATE   = 0x1,  /*!< RX start valid*/
    LPUART_SR0_RX_DONE_STATE       = 0x2,  /*!< RX done*/
    LPUART_SR0_START_INVALID_STATE = 0x4,  /*!< RX start invalid*/
    LPUART_SR0_PARITY_ERR_STATE    = 0x8,  /*!< RX parity error*/
    LPUART_SR0_STOP_ERR_STATE      = 0x10, /*!< RX stop error*/
    LPUART_SR0_RX_OVERFLOW_STATE   = 0x20  /*!< RX overflow*/
} lpuart_rx_status_t;

/**
 * @brief LPUART SR1 register definition
 */
typedef enum {
    LPUART_SR1_WRITE_SR0_STATE    = 0x2,  /*!< Write SR0 register state*/
    LPUART_SR1_WRITE_CR0_STATE    = 0x4,  /*!< Write CR0 register state*/
    LPUART_SR1_RX_NOT_EMPTY_STATE = 0x8,  /*!< RX not empty*/
    LPUART_SR1_TX_EMPTY_STATE     = 0x10, /*!< TX empty*/
    LPUART_SR1_TX_DONE_STATE      = 0x20  /*!< TX done*/
} lpuart_sr1_t;

/**
 * @brief LPUART configuration
 */
typedef struct {
    uint32_t baudrate;              /*!< Baudrate*/
    lpuart_data_width_t data_width; /*!< Data width*/
    lpuart_parity_t parity;         /*!< Parity mode*/
    lpuart_stop_bits_t stop_bits;   /*!< Stop bit*/
    bool low_level_wakeup;          /*!< Level wakeup */
    bool start_wakeup;              /*!< Start wakeup*/
    bool rx_done_wakeup;            /*!< RX done wakeup*/
} lpuart_init_t;

void lpuart_init(lpuart_t* lpuart, lpuart_init_t* uart_init);
void lpuart_deinit(lpuart_t* lpuart);

uint8_t lpuart_receive_data(lpuart_t* lpuart);
void lpuart_send_data(lpuart_t* lpuart, uint8_t data);

void lpuart_config_dma(lpuart_t* lpuart, lpuart_dma_t dma, bool new_state);
void lpuart_config_interrupt(lpuart_t* lpuart, lpuart_it_t interrupt, bool new_state);

void lpuart_config_rts(lpuart_t* lpuart, bool new_state);
void lpuart_config_rx(lpuart_t* lpuart, bool new_state);
void lpuart_config_cts(lpuart_t* lpuart, bool new_state);
void lpuart_config_tx(lpuart_t* lpuart, bool new_state);

bool lpuart_get_rx_status(lpuart_t* lpuart, lpuart_rx_status_t rx_status);
void lpuart_clear_rx_status(lpuart_t* lpuart, lpuart_rx_status_t rx_status);
bool lpuart_get_rx_not_empty_status(lpuart_t* lpuart);
bool lpuart_get_tx_empty_status(lpuart_t* lpuart);
bool lpuart_get_tx_done_status(lpuart_t* lpuart);
void lpuart_clear_tx_done_status(lpuart_t* lpuart);

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_LPUART_H */

/**
 * @} 
 * @}
 */
