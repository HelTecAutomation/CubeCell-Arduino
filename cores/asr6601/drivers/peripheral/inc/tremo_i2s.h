/**
 ******************************************************************************
 * @file    tremo_i2s.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of I2S module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup I2S
 * @{
 */

#ifndef __TREMO_I2S_H
#define __TREMO_I2S_H

#ifdef __cplusplus
extern "c" {
#endif

#include "tremo_regs.h"
#include "stdbool.h"

#define I2S_INTERRUPT_TXFO (1 << 5) /*!< TX fifo overflow interrupt status*/
#define I2S_INTERRUPT_TXFE (1 << 4) /*!< TX fifo empty interrupt status*/
#define I2S_INTERRUPT_RXFO (1 << 1) /*!< RX fifo overflow interrupt status*/
#define I2S_INTERRUPT_RXDA (1 << 0) /*!< RX available data interrupt status*/

#define I2S_MODE_LEFT_ALIGN  (0) /*!< Left alignment mode*/
#define I2S_MODE_RIGHT_ALIGN (1) /*!< Right alignment mode*/
#define I2S_MODE_PHILIPS     (2) /*!< PHILIPS mode*/

#define I2S_TX_BLOCK_EN (1) /*!< TX block enable*/
#define I2S_RX_BLOCK_EN (1) /*!< RX block enable*/
#define I2S_TX_CHAN_EN  (1) /*!< TX channel enable*/
#define I2S_RX_CHAN_EN  (1) /*!< RX channel enable*/

#define I2S_FIFO_TRIGGERL_LEVEL_1  (1)  /*!< FIFO trigger level 1*/
#define I2S_FIFO_TRIGGERL_LEVEL_2  (2)  /*!< FIFO trigger level 2*/
#define I2S_FIFO_TRIGGERL_LEVEL_3  (3)  /*!< FIFO trigger level 3*/
#define I2S_FIFO_TRIGGERL_LEVEL_4  (4)  /*!< FIFO trigger level 4*/

#define I2S_FIFO_TRIGGER_LEVEL_MASK (0xf) /*!< FIFO trigger level mask*/

#define I2S_WORDSIZE_DONT_CARE (0) /*!< Don't care word size*/
#define I2S_WORDSIZE_12bit     (1) /*!< 12bit word size*/
#define I2S_WORDSIZE_16bit     (2) /*!< 16bit word size*/
#define I2S_WORDSIZE_20bit     (3) /*!< 20bit word size*/
#define I2S_WORDSIZE_24bit     (4) /*!< 24bit word size*/
#define I2S_WORDSIZE_32bit     (5) /*!< 32bit word size*/

#define I2S_FIFO_DEPTH     (4) /*!< FIFO depth*/

#define I2S_RX_WORDSIZE_MASK (0x7) /*!< RX word size mask*/
#define I2S_TX_WORDSIZE_MASK (0x7) /*!< TX word size mask*/

#define I2S_ROLE_MASTER (1) /*!< I2S master*/
#define I2S_ROLE_SLAVE  (0) /*!< I2S slave*/

#define I2S_RIGHT_CHANNEL (1) /*!< I2S right channel*/
#define I2S_LEFT_CHANNEL  (0) /*!< I2S left channel*/

#define I2S_MCLK_SRC_FREQ17   (16934400) /*!< I2S 96KHz sample rate*/
#define I2S_MCLK_SRC_FREQ12   (12288000) /*!< I2S 44.1KHz sample rate*/
#define I2S_SAMPLE_RATE_44P1K (44100)    /*!< I2S 44.1KHz frequence*/
#define I2S_SAMPLE_RATE_96K   (96000)    /*!< I2S 96KHz frequence*/

#define I2S_MASTER_ENABLE_POS (14) /*!< I2S master enable position*/
#define I2S_WS_FREQ_POS       (15) /*!< I2S ws frequence position*/
#define I2S_WS_ENABLE_POS     (22) /*!< I2S ws enable position*/
#define I2S_WS_DELAY_POS      (23) /*!< I2S ws delay position*/

/**
 * @brief I2S initialization configuration
 */
typedef struct {
    uint8_t i2s_role;
    uint8_t i2s_word_size;
    uint8_t i2s_fifo_threshold;
} i2s_init_t;

void i2s_init_struct(i2s_init_t* i2s_struct);
void i2s_init(i2s_t* I2Sx, i2s_init_t* i2s_struct);
void i2s_deinit(i2s_t* I2Sx);

uint8_t i2s_calculate_devision(uint8_t word_size);
void i2s_master_clock_cmd(i2s_t* I2Sx, uint32_t new_state);

void i2s_config_interrupt(i2s_t* I2Sx, uint32_t i2s_interrupt, uint32_t new_state);
void i2s_clear_interrupt(i2s_t* I2Sx, uint32_t i2s_interrupt);
bool i2s_get_interrupt_status(i2s_t* I2Sx, uint32_t i2s_interrupt);

void i2s_cmd(i2s_t* I2Sx, bool state);
void i2s_tx_block_cmd(i2s_t* I2Sx, uint32_t new_state);
void i2s_rx_block_cmd(i2s_t* I2Sx, uint32_t new_state);
void i2s_tx_channel_cmd(i2s_t* I2Sx, uint32_t new_state);
void i2s_rx_channel_cmd(i2s_t* I2Sx, uint32_t new_state);

void i2s_send_data(i2s_t* I2Sx, uint16_t* left_chan_data, uint16_t* right_chan_data, uint32_t len);
uint32_t i2s_receive_data(i2s_t* I2Sx, uint8_t lr);

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_I2S_H */

/**
 * @} 
 * @}
 */
