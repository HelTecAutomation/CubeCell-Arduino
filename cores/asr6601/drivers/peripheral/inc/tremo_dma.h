/**
 ******************************************************************************
 * @file    tremo_dma.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of DMA module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup DMA
 * @{
 */

#ifndef __TREMO_DMA_H_
#define __TREMO_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tremo_rcc.h"
#include "tremo_regs.h"

#define DMA_BASE 0x40023000

/*CH0*/
#define DMA_SAR0_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x000))
#define DMA_SAR0_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x004))
#define DMA_DAR0_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x008))
#define DMA_DAR0_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x00C))

#define DMA_LLP0_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x010))
#define DMA_LLP0_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x014))
#define DMA_CTL0_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x018))
#define DMA_CTL0_L_REG(i) (DMA_BASE + i * 0x1000 + 0x018)
#define DMA_CTL0_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x01C))

#define DMA_SSTAT0_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x020))
#define DMA_SSTAT0_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x024))
#define DMA_DSTAT0_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x028))
#define DMA_DSTAT0_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x02C))

#define DMA_SSTATAR0_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x030))
#define DMA_SSTATAR0_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x034))
#define DMA_DSTATAR0_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x038))
#define DMA_DSTATAR0_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x03C))

#define DMA_CFG0_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x040))
#define DMA_CFG0_L_REG(i) (DMA_BASE + i * 0x1000 + 0x040)
#define DMA_CFG0_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x044))
#define DMA_CFG0_H_REG(i) (DMA_BASE + i * 0x1000 + 0x044)
#define DMA_SGR0_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x048))
#define DMA_SGR0_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x04C))
#define DMA_DSR0_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x050))
#define DMA_DSR0_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x054))

/*CH1*/
#define DMA_SAR1_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x058))
#define DMA_SAR1_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x05C))
#define DMA_DAR1_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x060))
#define DMA_DAR1_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x064))

#define DMA_LLP1_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x068))
#define DMA_LLP1_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x06C))
#define DMA_CTL1_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x070))
#define DMA_CTL1_L_REG(i) (DMA_BASE + i * 0x1000 + 0x070)
#define DMA_CTL1_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x074))

#define DMA_SSTAT1_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x078))
#define DMA_SSTAT1_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x07C))
#define DMA_DSTAT1_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x080))
#define DMA_DSTAT1_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x084))

#define DMA_SSTATAR1_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x088))
#define DMA_SSTATAR1_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x08C))
#define DMA_DSTATAR1_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x090))
#define DMA_DSTATAR1_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x094))

#define DMA_CFG1_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x098))
#define DMA_CFG1_L_REG(i) (DMA_BASE + i * 0x1000 + 0x098)
#define DMA_CFG1_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x09C))
#define DMA_CFG1_H_REG(i) (DMA_BASE + i * 0x1000 + 0x09C)
#define DMA_SGR1_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0A0))
#define DMA_SGR1_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0A4))
#define DMA_DSR1_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0A8))
#define DMA_DSR1_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0AC))

/*CH2*/
#define DMA_SAR2_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0B0))
#define DMA_SAR2_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0B4))
#define DMA_DAR2_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0B8))
#define DMA_DAR2_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0BC))

#define DMA_LLP2_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0C0))
#define DMA_LLP2_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0C4))
#define DMA_CTL2_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0C8))
#define DMA_CTL2_L_REG(i) (DMA_BASE + i * 0x1000 + 0x0C8)
#define DMA_CTL2_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0CC))

#define DMA_SSTAT2_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0D0))
#define DMA_SSTAT2_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0D4))
#define DMA_DSTAT2_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0D8))
#define DMA_DSTAT2_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0DC))

#define DMA_SSTATAR2_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0E0))
#define DMA_SSTATAR2_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0E4))
#define DMA_DSTATAR2_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0E8))
#define DMA_DSTATAR2_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0EC))

#define DMA_CFG2_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0F0))
#define DMA_CFG2_L_REG(i) (DMA_BASE + i * 0x1000 + 0x0F0)
#define DMA_CFG2_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0F4))
#define DMA_CFG2_H_REG(i) (DMA_BASE + i * 0x1000 + 0x0F4)
#define DMA_SGR2_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0F8))
#define DMA_SGR2_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x0FC))
#define DMA_DSR2_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x100))
#define DMA_DSR2_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x104))

/*CH3*/
#define DMA_SAR3_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x108))
#define DMA_SAR3_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x10C))
#define DMA_DAR3_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x110))
#define DMA_DAR3_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x114))

#define DMA_LLP3_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x118))
#define DMA_LLP3_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x11C))
#define DMA_CTL3_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x120))
#define DMA_CTL3_L_REG(i) (DMA_BASE + i * 0x1000 + 0x120)
#define DMA_CTL3_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x124))

#define DMA_SSTAT3_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x128))
#define DMA_SSTAT3_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x12C))
#define DMA_DSTAT3_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x130))
#define DMA_DSTAT3_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x134))

#define DMA_SSTATAR3_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x138))
#define DMA_SSTATAR3_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x13C))
#define DMA_DSTATAR3_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x140))
#define DMA_DSTATAR3_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x144))

#define DMA_CFG3_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x148))
#define DMA_CFG3_L_REG(i) (DMA_BASE + i * 0x1000 + 0x148)
#define DMA_CFG3_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x14C))
#define DMA_CFG3_H_REG(i) (DMA_BASE + i * 0x1000 + 0x14C)
#define DMA_SGR3_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x150))
#define DMA_SGR3_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x154))
#define DMA_DSR3_L(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x158))
#define DMA_DSR3_H(i)     *((volatile int*)(DMA_BASE + i * 0x1000 + 0x15C))

#define DMACFGREG_L(i)         *((volatile int*)(DMA_BASE + i * 0x1000 + 0x398))
#define DMACFGREG_H(i)         *((volatile int*)(DMA_BASE + i * 0x1000 + 0x39C))
#define CHENREG_L(i)           *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3A0))
#define CHENREG_L_REG(i)       (DMA_BASE + i * 0x1000 + 0x3A0)
#define CHENREG_H(i)           *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3A4))
#define DMA_COMP_PARAMS_3_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3E0))
#define DMA_COMP_PARAMS_3_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3E4))
#define DMA_COMP_PARAMS_2_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3E8))
#define DMA_COMP_PARAMS_2_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3EC))
#define DMA_COMP_PARAMS_1_L(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3F0))
#define DMA_COMP_PARAMS_1_H(i) *((volatile int*)(DMA_BASE + i * 0x1000 + 0x3F4))

/*DMA interrupt registers*/
#define DMA_RAW_TFR_L_REG(i)   (DMA_BASE + i * 0x1000 + 0x2C0)
#define DMA_RAW_TFR_H_REG(i)   (DMA_BASE + i * 0x1000 + 0x2C4)
#define DMA_RAW_BLOCK_L_REG(i) (DMA_BASE + i * 0x1000 + 0x2C8)
#define DMA_RAW_BLOCK_H_REG(i) (DMA_BASE + i * 0x1000 + 0x2CC)

#define DMA_STATUS_TFR_L_REG(i)   (DMA_BASE + i * 0x1000 + 0x2E8)
#define DMA_STATUS_TFR_H_REG(i)   (DMA_BASE + i * 0x1000 + 0x2EC)
#define DMA_STATUS_BLOCK_L_REG(i) (DMA_BASE + i * 0x1000 + 0x2F0)
#define DMA_STATUS_BLOCK_H_REG(i) (DMA_BASE + i * 0x1000 + 0x2F4)

#define DMA_MASK_TFR_L_REG(i)   (DMA_BASE + i * 0x1000 + 0x310)
#define DMA_MASK_TFR_H_REG(i)   (DMA_BASE + i * 0x1000 + 0x314)
#define DMA_MASK_BLOCK_L_REG(i) (DMA_BASE + i * 0x1000 + 0x318)
#define DMA_MASK_BLOCK_H_REG(i) (DMA_BASE + i * 0x1000 + 0x31C)

#define DMA_CLEAR_TFR_L_REG(i)      (DMA_BASE + i * 0x1000 + 0x338)
#define DMA_CLEAR_TFR_H_REG(i)      (DMA_BASE + i * 0x1000 + 0x33C)
#define DMA_CLEAR_BLOCK_L_REG(i)    (DMA_BASE + i * 0x1000 + 0x340)
#define DMA_CLEAR_BLOCK_H_REG(i)    (DMA_BASE + i * 0x1000 + 0x344)
#define DMA_CLEAR_SRC_TRAN_L_REG(i) (DMA_BASE + i * 0x1000 + 0x348)
#define DMA_CLEAR_SRC_TRAN_H_REG(i) (DMA_BASE + i * 0x1000 + 0x34c)
#define DMA_CLEAR_DST_TRAN_L_REG(i) (DMA_BASE + i * 0x1000 + 0x350)
#define DMA_CLEAR_DST_TRAN_H_REG(i) (DMA_BASE + i * 0x1000 + 0x354)
#define DMA_CLEAR_ERR_L_REG(i)      (DMA_BASE + i * 0x1000 + 0x358)
#define DMA_CLEAR_ERR_H_REG(i)      (DMA_BASE + i * 0x1000 + 0x35c)

#define DMA_STATUS_INT_REG(i) (DMA_BASE + i * 0x1000 + 0x360)

#define DMA_REQ_SRC_REG(i)     (DMA_BASE + i * 0x1000 + 0x368)
#define DMA_REQ_DST_REG(i)     (DMA_BASE + i * 0x1000 + 0x370)
#define DMA_SGL_REQ_SRC_REG(i) (DMA_BASE + i * 0x1000 + 0x378)
#define DMA_SGL_REQ_DST_REG(i) (DMA_BASE + i * 0x1000 + 0x380)

#define TREMO_DMA_NUM_0    0 /*!< The first DMA*/
#define TREMO_DMA_NUM_1    1 /*!< The second DMA*/
#define TREMO_DMA_CHAN_0   1 /*!< The DMA channel 0*/
#define TREMO_DMA_CHAN_1   1 /*!< The DMA channel 1*/
#define TREMO_DMA_CHAN_2   1 /*!< The DMA channel 2*/
#define TREMO_DMA_CHAN_3   1 /*!< The DMA channel 3*/
#define TREMO_DMA_NUM      2 /*!< DMA number*/
#define TREMO_DMA_CHAN_NUM 4 /*!< DMA channel number in per DMA*/

typedef void (*dma_callback_func)(); /*!< DMA callback function*/

/**
 * @brief DMA mode
 */
typedef enum {
    M2M_MODE, /*!< Memory to memory mode*/
    M2P_MODE, /*!< Memory to peripheral mode*/
    P2M_MODE, /*!< peripheral to memory mode*/
    P2P_MODE  /*!< peripheral to peripheral mode*/
} dma_mode_t;

/**
 * @brief DMA registers
 */
typedef struct {
    uint32_t* DMA_SAR_L;    /*!< SAR register lower 32bits*/
    uint32_t* DMA_DAR_L;    /*!< DAR register lower 32bits*/
    uint32_t* DMA_LLP_L;    /*!< LLP register lower 32bits*/
    uint32_t* DMA_LLP_H;    /*!< LLP register upper 32bits*/
    uint32_t* DMA_CTL_H;    /*!< CTL register upper 32bits*/
    uint32_t* DMA_CFG_L;    /*!< CFG register lower 32bits*/
    uint32_t DMA_CTL_L_REG; /*!< CTL register lower 32bits*/
    uint32_t DMA_CFG_H_REG; /*!< CFG register upper 32bits*/
} dma_config_reg_t;

/**
 * @brief DMA configuration
 */
typedef struct {
    uint32_t src;           /*!< DMA source address*/
    uint32_t dest;          /*!< DMA destination address*/
    uint8_t data_width;     /*!< DMA data width,0:8bit,1:16bit,2:32bit*/
    uint8_t src_msize;      /*!< DMA source burst size*/
    uint8_t dest_msize;     /*!< DMA destination burst size*/
    uint8_t ch;             /*!< DMA channel*/
    uint16_t block_size;    /*!< DMA transfer block size*/
    uint8_t handshake;      /*!< DMA handshake*/
    uint8_t dma_num;        /*!< DMA number*/
    uint8_t  dma_busy;
    uint8_t * dma_queue;
    uint8_t * dma_buf;
    uint16_t  dma_idx_w;
    uint16_t  dma_idx_r;
    uint8_t * rx_buf;
    uint16_t rx_w;
    uint16_t rx_r;
    bool started;
    dma_mode_t mode;        /*!< DMA mode*/
    dma_callback_func priv; /*!< callback function*/
} dma_dev_t;

/**
 * @brief DMA LLI definition
 */
typedef struct {
    uint32_t SAR;   /*!< source address*/
    uint32_t DAR;   /*!< destination address*/
    uint32_t LLP;   /*!< linked list pointer*/
    uint32_t CTL_L; /*!< CTL register lower 32bit*/
    uint32_t CTL_H; /*!< CTL register upper 32bit*/
    uint32_t SSTAT; /*!< SSTAT register*/
    uint32_t DSTAT; /*!< DSTAT register*/
} dma_lli_t;

/**
 * @brief DMA LLI block configuration
 */
typedef struct {
    uint32_t src;        /*!< DMA source address*/
    uint32_t dest;       /*!< DMA destination address*/
    uint8_t data_width;  /*!< DMA data width,0:8bit,1:16bit,2:32bit*/
    uint8_t src_msize;   /*!< DMA source burst transaction length,0:1,1:4,2:8*/
    uint8_t dest_msize;  /*!< DMA destination burst transaction length,0:1,1:4,2:8*/
    uint8_t res[3];      /*!< reserved*/
    uint16_t block_size; /*!< DMA transfer block size*/
} dma_lli_block_config_t; 

/**
 * @brief DMA LLI mode configuration
 */
typedef struct {
    uint16_t block_num;   /*!< DMA LLI transfer block number*/
    bool src_lli_enable;  /*!< source use lli*/
    bool dest_lli_enable; /*!< destination use lli*/
} dma_lli_mode_t;

/**
 * @brief DMA reload configuration
 */
typedef struct {
    bool src_reload;  /*!< source reload*/
    bool dest_reload; /*!< destination reload*/
} dma_reload_t;


void dma_init(dma_dev_t* dma);
void dma_lli_init(dma_dev_t* dma, dma_lli_t* dma_lli, dma_lli_block_config_t* dma_lli_block, dma_lli_mode_t* lli_mode);
void dma_deinit(uint8_t dma_num);

void dma_config_reload(dma_dev_t* dma, dma_reload_t* dma_reload);

void dma_addr_update(dma_dev_t* dma, uint32_t size);

void dma_ch_enable(uint8_t dma_num, uint8_t ch);
void dma_ch_disable(uint8_t dma_num, uint8_t ch);

void dma_finalize(dma_dev_t* dma);
void dma1_IRQHandler(void);
void dma0_IRQHandler(void);
uint8_t getDmaIrqCh(uint8_t i);
void clearDmaIrqFlag(uint8_t i,int dma_ch);

#ifdef __cplusplus
}
#endif
#endif /* __TREMO_DMA_H_ */

/**
 * @} 
 * @}
 */
