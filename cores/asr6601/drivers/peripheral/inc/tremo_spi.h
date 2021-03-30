/**
 ******************************************************************************
 * @file    tremo_spi.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of SPI module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup SPI
 * @{
 */

#ifndef __TREMO_SPI_H
#define __TREMO_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tremo_regs.h"

#define SSP_ROLE_MASTER (0x0) /*!< SPI master*/
#define SSP_ROLE_SLAVE  (0x4) /*!< SPI slave*/

#define SSP_FRAME_FORMAT_SPI (0x0)      /*!< Frame format SPI*/
#define SSP_FRAME_FORMAT_TI  (0x1 << 4) /*!< Frame format TI*/
#define SSP_FRAME_FORMAT_MW  (0x2 << 4) /*!< Frame format MW*/

#define SSP_SLAVE_OUTPUT_DISABLE (1 << 3) /*!< Disable slave output*/

#define SPI_CLK_POLARITY_POS  (0x6)                         /*!< Clock polarity position*/
#define SPI_CLK_POLARITY_LOW  (0x0)                         /*!< Clock polarity low*/
#define SPI_CLK_POLARITY_HIGH (0x1 << SPI_CLK_POLARITY_POS) /*!< Clock polarity high*/

#define SPI_CLK_PHASE_POS   (0x7)                      /*!< Clock phase position*/
#define SPI_CLK_PHASE_1EDGE (0x0)                      /*!< Clock phase 1edge*/
#define SPI_CLK_PHASE_2EDGE (0x1 << SPI_CLK_PHASE_POS) /*!< Clock phase 2edge*/

#define SSP_DATA_SIZE_4BIT  (0x3) /*!< Data size 4bit*/
#define SSP_DATA_SIZE_8BIT  (0x7) /*!< Data size 8bit*/
#define SSP_DATA_SIZE_16BIT (0xF) /*!< Data size 16bit*/

#define SSP_FLAG_TX_FIFO_EMPTY     (0x1)    /*!< TX fifo empty flag*/
#define SSP_FLAG_TX_FIFO_NOT_FULL  (1 << 1) /*!< TX fifo not full flag*/
#define SSP_FLAG_RX_FIFO_NOT_EMPTY (1 << 2) /*!< RX fifo not empty flag*/
#define SSP_FLAG_RX_FIFO_FULL      (1 << 3) /*!< RX fifo full flag*/
#define SSP_FLAG_BUSY              (1 << 4) /*!< Busy flag*/

#define SSP_INTERRUPT_RX_FIFO_OVERRUN (1 << 0) /*!< RX fifo overrun interrupt*/
#define SSP_INTERRUPT_RX_TIMEOUT      (1 << 1) /*!< RX timeout interrupt*/
#define SSP_INTERRUPT_RX_FIFO_TRIGGER (1 << 2) /*!< RX fifo trigger interrupt*/
#define SSP_INTERRUPT_TX_FIFO_TRIGGER (1 << 3) /*!< TX fifo trigger interrupt*/
#define SSP_INTERRUPT_ALL             (0xf)    /*!< All interrupt*/

#define SSP_DMA_TX_EN (1 << 1) /*!< TX DMA enable*/
#define SSP_DMA_RX_EN (1)      /*!< RX DMA enable*/

/**
 * @brief SSP initialization configuration
 */
typedef struct {
    uint32_t ssp_sclk;     /*!< SSP sclk*/
    uint8_t ssp_role;      /*!< SSP role*/
    uint8_t ssp_format;    /*!< SSP format*/
    uint8_t ssp_clk_pol;   /*!< Clock polarity*/
    uint8_t ssp_clk_phase; /*!< Clock phase*/
    uint8_t ssp_data_size; /*!< Data size*/
    uint8_t ssp_dma_tx_en; /*!< TX DMA enable*/
    uint8_t ssp_dma_rx_en; /*!< RX DMA enable*/
} ssp_init_t;

/**
 * @brief  Clear interrupt
 * @param  SSPx SSP handler
 * @param  ssp_interrupt SSP interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_OVERRUN: RX fifo overrun interrupt
 *            @arg SSP_INTERRUPT_RX_TIMEOUT: RX timeout interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_TRIGGER: RX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_TX_FIFO_TRIGGER: TX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_ALL: All interrupt
 * @return
 */
__STATIC_INLINE void ssp_clear_interrupt(ssp_typedef_t* SSPx, uint8_t ssp_interrupt)
{
    SSPx->ICR |= ssp_interrupt;
}

/**
 * @brief  Get flag status
 * @param  SSPx SSP handler
 * @param  ssp_flag SSP flag
 *            @arg SSP_FLAG_TX_FIFO_EMPTY: TX fifo empty flag
 *            @arg SSP_FLAG_TX_FIFO_NOT_FULL: TX fifo not full flag
 *            @arg SSP_FLAG_RX_FIFO_NOT_EMPTY: RX fifo not empty flag
 *            @arg SSP_FLAG_RX_FIFO_FULL: RX fifo full flag
 *            @arg SSP_FLAG_BUSY: Busy flag
 * @retval SET set status
 * @retval RESET reset status
 */
__STATIC_INLINE it_status_t ssp_get_flag_status(ssp_typedef_t* SSPx, uint8_t ssp_flag)
{
	it_status_t res = RESET;
	if(SSPx->SR & ssp_flag)
		res = SET;
	return res;
}

/**
 * @brief  Get interrupt status
 * @param  SSPx SSP handler
 * @param  ssp_interrupt SSP interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_OVERRUN: RX fifo overrun interrupt
 *            @arg SSP_INTERRUPT_RX_TIMEOUT: RX timeout interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_TRIGGER: RX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_TX_FIFO_TRIGGER: TX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_ALL: All interrupt
 * @retval SET set status
 * @retval RESET reset status
 */
__STATIC_INLINE it_status_t ssp_get_interrupt_status(ssp_typedef_t* SSPx, uint8_t ssp_interrupt)
{
	it_status_t res = RESET;
	if(SSPx->MIS & ssp_interrupt)
		res = SET;
	return res;
}

/**
 * @brief  Get raw interrupt status
 * @param  SSPx SSP handler
 * @param  ssp_interrupt SSP interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_OVERRUN: RX fifo overrun interrupt
 *            @arg SSP_INTERRUPT_RX_TIMEOUT: RX timeout interrupt
 *            @arg SSP_INTERRUPT_RX_FIFO_TRIGGER: RX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_TX_FIFO_TRIGGER: TX fifo trigger interrupt
 *            @arg SSP_INTERRUPT_ALL: All interrupt
 * @retval SET set status
 * @retval RESET reset status
 */
__STATIC_INLINE it_status_t ssp_get_raw_interrupt_status(ssp_typedef_t* SSPx, uint8_t ssp_interrupt)
{
	it_status_t res = RESET;
	if(SSPx->RIS & ssp_interrupt)
		res = SET;
	return res;
}

void ssp_init_struct(ssp_init_t* init_struct);
void ssp_init(ssp_typedef_t* SSPx, ssp_init_t* init_struct);
void ssp_deinit(ssp_typedef_t* SSPx);

void ssp_config_interrupt(ssp_typedef_t* SSPx, uint8_t ssp_interrupt, uint8_t new_state);
void ssp_cmd(ssp_typedef_t* SSPx, uint8_t new_state);

void ssp_send_data(ssp_typedef_t* SSPx, uint8_t* tx_data, uint16_t len);
void ssp_receive_data(ssp_typedef_t* SSPx, uint8_t* rx_data, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_SPI_H */

/**
 * @} 
 * @}
 */
