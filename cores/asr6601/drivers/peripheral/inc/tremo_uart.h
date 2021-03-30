/**
 ******************************************************************************
 * @file    tremo_uart.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the I2C firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup UART
 * @{
 */

#ifndef __TREMO_UART_H
#define __TREMO_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "tremo_regs.h"

#define UART_BAUDRATE_110    (110)     /*!< baudrate 110 */
#define UART_BAUDRATE_300    (300)     /*!< baudrate 300 */
#define UART_BAUDRATE_600    (600)     /*!< baudrate 600 */
#define UART_BAUDRATE_1200   (1200)    /*!< baudrate 1200 */
#define UART_BAUDRATE_2400   (2400)    /*!< baudrate 2400 */
#define UART_BAUDRATE_4800   (4800)    /*!< baudrate 4800 */
#define UART_BAUDRATE_9600   (9600)    /*!< baudrate 9600 */
#define UART_BAUDRATE_14400  (14400)   /*!< baudrate 14400 */
#define UART_BAUDRATE_19200  (19200)   /*!< baudrate 19200 */
#define UART_BAUDRATE_38400  (38400)   /*!< baudrate 38400 */
#define UART_BAUDRATE_57600  (57600)   /*!< baudrate 57600 */
#define UART_BAUDRATE_115200 (115200)  /*!< baudrate 115200 */
#define UART_BAUDRATE_230400 (230400)  /*!< baudrate 230400 */
#define UART_BAUDRATE_460800 (460800)  /*!< baudrate 460800 */
#define UART_BAUDRATE_921600 (921600)  /*!< baudrate 921600 */

#define UART_FLAG_TX_FIFO_EMPTY (1 << 7)  /*!< TX FIFO is empty */
#define UART_FLAG_RX_FIFO_FULL  (1 << 6)  /*!< RX FIFO is full */
#define UART_FLAG_TX_FIFO_FULL  (1 << 5)  /*!< TX FIFO is full */
#define UART_FLAG_RX_FIFO_EMPTY (1 << 4)  /*!< RX FIFO is empty */
#define UART_FLAG_BUSY          (1 << 3)  /*!< Busy */

#define UART_INTERRUPT_RX_DONE       (1 << 4)  /*!< RX done */
#define UART_INTERRUPT_TX_DONE       (1 << 5)  /*!< TX done */
#define UART_INTERRUPT_RX_TIMEOUT    (1 << 6)  /*!< RX timeout */
#define UART_INTERRUPT_FRAME_ERROR   (1 << 7)  /*!< Frame error */
#define UART_INTERRUPT_PARITY_ERROR  (1 << 8)  /*!< Parity */
#define UART_INTERRUPT_BREAK_ERROR   (1 << 9)  /*!< Break error */
#define UART_INTERRUPT_OVERRUN_ERROR (1 << 10) /*!< Overrun error */

#define UART_IRDA_MODE_NORMAL 0  /*!< IRDA normal mode */
#define UART_IRDA_MODE_LP     1  /*!< IRDA low power mode */
#define UART_IRDA_LPBAUD16    ((uint32_t)1843200) /*!< IRDA low power baudrate */

/**
 * @brief UART data width
 */
typedef enum {
    UART_DATA_WIDTH_5 = UART_LCR_H_WLEN_5,  /*!< data width is 5 bits */
    UART_DATA_WIDTH_6 = UART_LCR_H_WLEN_6,  /*!< data width is 6 bits */
    UART_DATA_WIDTH_7 = UART_LCR_H_WLEN_7,  /*!< data width is 7 bits */
    UART_DATA_WIDTH_8 = UART_LCR_H_WLEN_8   /*!< data width is 8 bits */
} uart_data_width_t;

/**
 * @brief UART stop bits
 */
typedef enum { 
    UART_STOP_BITS_1 = UART_LCR_H_STOP_1,  /*!< stop bits is 1*/
    UART_STOP_BITS_2 = UART_LCR_H_STOP_2   /*!< stop bits is 2*/
} uart_stop_bits_t;

/**
 * @brief UART flow control
 */
typedef enum {
    UART_FLOW_CONTROL_DISABLED = UART_CR_FLOW_CTRL_NONE,    /*!< flow control is disabled */
    UART_FLOW_CONTROL_RTS      = UART_CR_FLOW_CTRL_RTS,     /*!< RTS is enabled */
    UART_FLOW_CONTROL_CTS      = UART_CR_FLOW_CTRL_CTS,     /*!< CTS is enabled */
    UART_FLOW_CONTROL_CTS_RTS  = UART_CR_FLOW_CTRL_CTS_RTS  /*!< Both RTS and CTS are enabled */
} uart_flow_control_t;

/**
 * @brief UART parity
 */
typedef enum { 
    UART_PARITY_NO,   /*!< No parity */
    UART_PARITY_ODD,  /*!< Odd parity */
    UART_PARITY_EVEN  /*!< Even parity */
} uart_parity_t;

/**
 * @brief UART mode
 */
typedef enum {
    UART_MODE_TX   = UART_CR_UART_MODE_TX,   /*!< TX mode */
    UART_MODE_RX   = UART_CR_UART_MODE_RX,   /*!< RX mode */
    UART_MODE_TXRX = UART_CR_UART_MODE_TXRX  /*!< TX and RX mode */
} uart_mode_t;

/**
 * @brief UART TX fifo threshold
 */
typedef enum {
    UART_TX_FIFO_LEVEL_1_8 = UART_IFLS_TX_1_8,  /*!< 1/8 */
    UART_TX_FIFO_LEVEL_1_4 = UART_IFLS_TX_1_4,  /*!< 1/4 */
    UART_TX_FIFO_LEVEL_1_2 = UART_IFLS_TX_1_2,  /*!< 1/2 */
    UART_TX_FIFO_LEVEL_3_4 = UART_IFLS_TX_3_4,  /*!< 3/4 */
    UART_TX_FIFO_LEVEL_7_8 = UART_IFLS_TX_7_8   /*!< 7/8 */
} uart_tx_fifo_level_t;

/**
 * @brief UART RX fifo threshold
 */
typedef enum {
    UART_RX_FIFO_LEVEL_1_8 = UART_IFLS_RX_1_8,  /*!< 1/8 */
    UART_RX_FIFO_LEVEL_1_4 = UART_IFLS_RX_1_4,  /*!< 1/4 */
    UART_RX_FIFO_LEVEL_1_2 = UART_IFLS_RX_1_2,  /*!< 1/2 */
    UART_RX_FIFO_LEVEL_3_4 = UART_IFLS_RX_3_4,  /*!< 3/4 */
    UART_RX_FIFO_LEVEL_7_8 = UART_IFLS_RX_7_8   /*!< 7/8 */
} uart_rx_fifo_level_t;

/**
 * @brief UART DMA req
 */
typedef enum { 
    UART_DMA_REQ_TX = UART_DMACR_TX_EN_MASK,   /*!< DMA tx req */
    UART_DMA_REQ_RX = UART_DMACR_RX_EN_MASK    /*!< DMA rx req */
} uart_dma_req_t;

/**
 * @brief UART configuration
 */
typedef struct {
    uint32_t baudrate;                 /*!< Baudrate */
    uint32_t data_width;      /*!< Data width */
    uint32_t parity;              /*!< Parity */
    uint32_t stop_bits;        /*!< Stop bits */
    uint32_t flow_control;  /*!< Flow control policy */
    uint32_t mode;                  /*!< UART mode */
    uint8_t fifo_mode;                 /*!< FIFO mode */
} uart_config_t;

uint32_t calc_uart_baud(uint32_t uart_clk, uint32_t baud);

void uart_deinit(uart_t* uartx);
void uart_config_init(uart_config_t* config);
int32_t uart_init(uart_t* uartx, uart_config_t* config);

void uart_cmd(uart_t* uartx, bool newstate);
void uart_send_data(uart_t* uartx, uint8_t data);
uint8_t uart_receive_data(uart_t* uartx);

void uart_set_tx_fifo_threshold(uart_t* uartx, uint32_t uart_fifo_level);
void uart_set_rx_fifo_threshold(uart_t* uartx, uint32_t uart_fifo_level);

flag_status_t uart_get_flag_status(uart_t* uartx, uint32_t uart_flag);

void uart_config_interrupt(uart_t* uartx, uint32_t uart_interrupt, bool new_state);
void uart_clear_interrupt(uart_t* uartx, uint32_t uart_interrupt);
it_status_t uart_get_interrupt_status(uart_t* uartx, uint32_t uart_interrupt);

void uart_irda_config(uart_t* uartx, uint32_t uart_irda_mode);
void uart_irda_cmd(uart_t* uartx, bool new_state);

void uart_dma_config(uart_t* uartx, uart_dma_req_t dma_req, bool new_state);
void uart_dma_onerror_config(uart_t* uartx, bool new_state);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_UART_H

/**
 * @} 
 * @}
 */
