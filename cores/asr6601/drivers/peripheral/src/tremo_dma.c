#include <stdbool.h>
#include <stdio.h>
#include "tremo_cm4.h"
#include "tremo_dma.h"

dma_callback_func g_dma_callback_handler[TREMO_DMA_NUM][TREMO_DMA_CHAN_NUM]; /*!< DMA callback function handler*/

static uint32_t write32_bit_variate(uint32_t variate_value, uint8_t start_bit, uint8_t len, uint32_t src_val)
{
    uint32_t tmp, mask, val = 0;

    if ((start_bit < 32) && (len <= 32) && (src_val <= ((1 << len) - 1))) {
        tmp = variate_value;

        mask = (1 << len) - 1;
        mask = ~(mask << start_bit);

        val = tmp & mask;

        src_val = (src_val << start_bit);
        val     = val | src_val;
    } else {
    }

    return val;
}

static void write32_bit_reg(uint32_t reg, uint8_t start_bit, uint8_t len, uint32_t src_val)
{
    uint32_t tmp, mask, val;

    if ((start_bit < 32) && (len <= 32) && (src_val <= ((1 << len) - 1))) {
        tmp = TREMO_REG_RD(reg);

        mask = (1 << len) - 1;
        mask = ~(mask << start_bit);

        val = tmp & mask;

        src_val = (src_val << start_bit);
        val     = val | src_val;

        TREMO_REG_WR(reg, val);
    } else {
    }
}

static uint32_t read32_bit_reg(uint32_t reg, uint8_t start_bit, uint8_t len)
{
    uint32_t mask, val;

    if ((start_bit < 32) && (len <= 32)) {
        val = TREMO_REG_RD(reg);

        mask = (1 << len) - 1;
        mask = mask << start_bit;
        val  = val & mask;

        val = (val >> start_bit);

        return val;
    } else {
        return -1;
    }
}

uint8_t getDmaIrqCh(uint8_t i)
{
	if (TREMO_REG_RD(DMA_STATUS_BLOCK_L_REG(i)) & 0x01) {
		return 0;
	} else if (TREMO_REG_RD(DMA_STATUS_BLOCK_L_REG(i)) & 0x02) {
		return 1;
	} else if (TREMO_REG_RD(DMA_STATUS_BLOCK_L_REG(i)) & 0x04) {
		return 2;
	} else {
		return 3;
	}
}

void clearDmaIrqFlag(uint8_t i,int dma_ch)
{
	if (dma_ch == 0) {
		/*clear TFR int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_TFR_L_REG(i), 0x1);
		/*clear block int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_BLOCK_L_REG(i), 0x1);
	} else if (dma_ch == 1) {
		/*clear TFR int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_TFR_L_REG(i), 0x2);
		/*clear block int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_BLOCK_L_REG(i), 0x2);
	} else if (dma_ch == 2) {
		/*clear TFR int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_TFR_L_REG(i), 0x4);
		/*clear block int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_BLOCK_L_REG(i), 0x4);
	} else {
		/*clear TFR int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_TFR_L_REG(i), 0x8);
		/*clear block int,channelx*/
		TREMO_REG_WR(DMA_CLEAR_BLOCK_L_REG(i), 0x8);
	}
}

void dma0_IRQHandler(void)
{
	uint8_t dma_ch = getDmaIrqCh(0);

	if (g_dma_callback_handler[0][dma_ch]) {
		g_dma_callback_handler[0][dma_ch]();
	}
}

void dma1_IRQHandler(void)
{
	uint8_t dma_ch = getDmaIrqCh(1);
	clearDmaIrqFlag(1,dma_ch);
	if (g_dma_callback_handler[1][dma_ch]) {
		g_dma_callback_handler[1][dma_ch]();
	}
}

static void set_dma_mode(dma_dev_t* dma, dma_config_reg_t* config_reg)
{
    uint8_t handshake_src = 0;
    uint8_t handshake_dest = 0;
    uint32_t syscfg;

    if (config_reg == NULL) {
        return;
    }

    if (dma->mode != M2M_MODE) {
        if (dma->dma_num == 0) {
            syscfg = TREMO_REG_RD(&(SYSCFG -> CR0));
            syscfg &= ~(0x3f << (8 * (TREMO_DMA_CHAN_NUM - dma->ch - 1)));
            syscfg |= (dma->handshake) << (8 * (TREMO_DMA_CHAN_NUM - dma->ch - 1));
            TREMO_REG_WR(&(SYSCFG->CR0), syscfg);
        } else if (dma->dma_num == TREMO_DMA_NUM - 1) {
            syscfg = TREMO_REG_RD(&(SYSCFG -> CR1));
            syscfg &= ~(0x3f << (8 * (TREMO_DMA_CHAN_NUM - dma->ch - 1)));
            syscfg |= (dma->handshake) << (8 * (TREMO_DMA_CHAN_NUM - dma->ch - 1));
            TREMO_REG_WR(&(SYSCFG->CR1), syscfg);
        } else {
            return;
        }
    }

    TREMO_REG_WR(DMA_CLEAR_TFR_L_REG(dma->dma_num), 0x0F);
    TREMO_REG_WR(DMA_CLEAR_BLOCK_L_REG(dma->dma_num), 0x0F);
    TREMO_REG_WR(DMA_CLEAR_SRC_TRAN_L_REG(dma->dma_num), 0x0F);
    TREMO_REG_WR(DMA_CLEAR_DST_TRAN_L_REG(dma->dma_num), 0x0F);
    TREMO_REG_WR(DMA_CLEAR_ERR_L_REG(dma->dma_num), 0x0F);
    if (dma->ch == 0) {
        while (CHENREG_L(dma->dma_num) & 0x01)
            ;
        config_reg->DMA_SAR_L     = (uint32_t*)&DMA_SAR0_L(dma->dma_num);
        config_reg->DMA_DAR_L     = (uint32_t*)&DMA_DAR0_L(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP0_L(dma->dma_num);
        config_reg->DMA_LLP_H     = (uint32_t*)&DMA_LLP0_H(dma->dma_num);
        config_reg->DMA_CTL_L_REG = DMA_CTL0_L_REG(dma->dma_num);
        config_reg->DMA_CTL_H     = (uint32_t*)&DMA_CTL0_H(dma->dma_num);
        config_reg->DMA_CFG_L     = (uint32_t*)&DMA_CFG0_L(dma->dma_num);
        config_reg->DMA_CFG_H_REG = DMA_CFG0_H_REG(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP0_L(dma->dma_num);
    } else if (dma->ch == 1) {
        while (CHENREG_L(dma->dma_num) & 0x02)
            ;
        config_reg->DMA_SAR_L     = (uint32_t*)&DMA_SAR1_L(dma->dma_num);
        config_reg->DMA_DAR_L     = (uint32_t*)&DMA_DAR1_L(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP1_L(dma->dma_num);
        config_reg->DMA_LLP_H     = (uint32_t*)&DMA_LLP1_H(dma->dma_num);
        config_reg->DMA_CTL_L_REG = DMA_CTL1_L_REG(dma->dma_num);
        config_reg->DMA_CTL_H     = (uint32_t*)&DMA_CTL1_H(dma->dma_num);
        config_reg->DMA_CFG_L     = (uint32_t*)&DMA_CFG1_L(dma->dma_num);
        config_reg->DMA_CFG_H_REG = DMA_CFG1_H_REG(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP1_L(dma->dma_num);
    } else if (dma->ch == 2) {
        while (CHENREG_L(dma->dma_num) & 0x04)
            ;
        config_reg->DMA_SAR_L     = (uint32_t*)&DMA_SAR2_L(dma->dma_num);
        config_reg->DMA_DAR_L     = (uint32_t*)&DMA_DAR2_L(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP2_L(dma->dma_num);
        config_reg->DMA_LLP_H     = (uint32_t*)&DMA_LLP2_H(dma->dma_num);
        config_reg->DMA_CTL_L_REG = DMA_CTL2_L_REG(dma->dma_num);
        config_reg->DMA_CTL_H     = (uint32_t*)&DMA_CTL2_H(dma->dma_num);
        config_reg->DMA_CFG_L     = (uint32_t*)&DMA_CFG2_L(dma->dma_num);
        config_reg->DMA_CFG_H_REG = DMA_CFG2_H_REG(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP2_L(dma->dma_num);
    } else {
        while (CHENREG_L(dma->dma_num) & 0x08)
            ;
        config_reg->DMA_SAR_L     = (uint32_t*)&DMA_SAR3_L(dma->dma_num);
        config_reg->DMA_DAR_L     = (uint32_t*)&DMA_DAR3_L(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP3_L(dma->dma_num);
        config_reg->DMA_LLP_H     = (uint32_t*)&DMA_LLP3_H(dma->dma_num);
        config_reg->DMA_CTL_L_REG = DMA_CTL3_L_REG(dma->dma_num);
        config_reg->DMA_CTL_H     = (uint32_t*)&DMA_CTL3_H(dma->dma_num);
        config_reg->DMA_CFG_L     = (uint32_t*)&DMA_CFG3_L(dma->dma_num);
        config_reg->DMA_CFG_H_REG = DMA_CFG3_H_REG(dma->dma_num);
        config_reg->DMA_LLP_L     = (uint32_t*)&DMA_LLP3_L(dma->dma_num);
    }

    if (dma->mode == M2M_MODE) {
        /*[26:25]=01,  src AHB master 2*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 25, 2, 0x1);
        /*[24:23]=00,  dst AHB master 2*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 23, 2, 0x1);
        /*[22:20]=001, mem-->mem, DW_ahb_dmac flow ctrl*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 20, 3, (uint32_t)dma->mode);
        /*[10:9]=0,    src address increment*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 9, 2, 0x0);
        /*[8:7]=10,    dest address increment*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 7, 2, 0x0);
        handshake_src  = 0;
        handshake_dest = 0;
    } else if (dma->mode == M2P_MODE) {
        /*[26:25]=01,  src AHB master 2*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 25, 2, 0x1);
        /*[24:23]=00,  dst AHB master 1*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 23, 2, 0x0);
        /*[22:20]=001, mem-->peri, DW_ahb_dmac flow ctrl*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 20, 3, (uint32_t)dma->mode);
        /*[10:9]=0,    src address increment*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 9, 2, 0x0);
        /*[8:7]=10,    dest address no change*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 7, 2, 0x2);
        handshake_src  = 0;
        handshake_dest = dma->ch;
    } else if (dma->mode == P2M_MODE) {
        /*[26:25]=01,  src AHB master 1*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 25, 2, 0x0);
        /*[24:23]=00,  dst AHB master 2
        write32_bit_reg(config_reg.DMA_CTL_L_REG,23,2,0x1);*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 23, 2, 0x0);
        /*[22:20]=001, peri-->mem, DW_ahb_dmac flow ctrl*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 20, 3, (uint32_t)dma->mode);
        /*[10:9]=0,    src address no change*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 9, 2, 0x2);
        /*[8:7]=10,    dest address increment*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 7, 2, 0x0);
        handshake_src  = dma->ch;
        handshake_dest = 0;
    } else if (dma->mode == P2P_MODE) {
        /*[26:25]=01,  src AHB master 1*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 25, 2, 0x0);
        /*[24:23]=00,  dst AHB master 1*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 23, 2, 0x0);
        /*[22:20]=001, peri-->peri, DW_ahb_dmac flow ctrl*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 20, 3, (uint32_t)dma->mode);
        /*[10:9]=0,    src address no change*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 9, 2, 0x2);
        /*[8:7]=10,    dest address no change*/
        write32_bit_reg(config_reg->DMA_CTL_L_REG, 7, 2, 0x2);
        handshake_src  = dma->ch;
        handshake_dest = dma->ch;
    }

    /*DMA channelx CFG,config handshake signal with peri chip
    [31], reload DST, [19:18], interface polarity active high, [11]=1 src
    software,[10]=0 dest hardware*/
    *((volatile int*)config_reg->DMA_CFG_L) = 0x00000000;
    /*[14:11]=2,dest chx*/
    write32_bit_reg(config_reg->DMA_CFG_H_REG, 11, 4, handshake_dest);
    /*[10:7]=2,src chx*/
    write32_bit_reg(config_reg->DMA_CFG_H_REG, 7, 4, handshake_src);
    /*enable fifo mode*/
    write32_bit_reg(config_reg->DMA_CFG_H_REG, 1, 1, 0x0);
}

/**
 * @brief  Initialize a dma interface
 * @param  dma the configuration of the interface
 * @return
 */
void dma_init(dma_dev_t* dma)
{
    uint32_t temp_val           = 0;
    dma_config_reg_t config_reg = { 0 };

    if (dma == NULL) {
        return;
    }

    set_dma_mode(dma, &config_reg);

    /*DMA LL*/
    *((volatile int*)config_reg.DMA_LLP_L) = 0x00;
    *((volatile int*)config_reg.DMA_LLP_H) = 0x00;

    *((volatile int*)config_reg.DMA_CTL_H) = dma->block_size | (0x1 << 12);

#ifdef DMA_SCATTER_GATHER_ENABLE
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 17, 1, 0x1); /*source gather enable*/
    DMA_SGR0_L(dma->dma_num) = ((4 << 20) | 2);            /*source gather config count and interval*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 18, 1, 0x1); /*destination scatter enable*/
    DMA_DSR0_L(dma->dma_num) = ((4 << 20) | 2);            /*destination scatter config count and interval*/
#endif

    /*[16:14]=000, src_size*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 14, 3, dma->src_msize);
    /*[13:11]=000, dest_size*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 11, 3, dma->dest_msize);
    /*[6:4]=000,   source width*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 4, 3, dma->data_width);
    /*[3:1]=000,   destination width*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 1, 3, dma->data_width);
    /*[0]=1,       int enable*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 0, 1, 0x1);

    *((volatile uint32_t*)config_reg.DMA_SAR_L) = (uint32_t)dma->src;
    *((volatile uint32_t*)config_reg.DMA_DAR_L) = (uint32_t)dma->dest;

    if (dma->dma_num == 0) {
        NVIC_EnableIRQ(DMA0_IRQn);
        NVIC_SetPriority(DMA0_IRQn, 2);
    } else {
        NVIC_EnableIRQ(DMA1_IRQn);
        NVIC_SetPriority(DMA1_IRQn, 2);
    }

    if (dma->priv || dma->dma_num == 0) {
        if (dma->ch == 0) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0101);
        } else if (dma->ch == 1) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0202);
        } else if (dma->ch == 2) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0404);
        } else {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0808);
        }
        g_dma_callback_handler[dma->dma_num][dma->ch] = (dma_callback_func)(dma->priv);
    } else {
        if (dma->ch == 0) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0101));
        } else if (dma->ch == 1) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0202));
        } else if (dma->ch == 2) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0404));
        } else {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0808));
        }
    }
}

/**
 * @brief  Initialize lli of a dma interface
 * @param  dma the configuration of the interface
 * @param  dma_lli the lli list
 * @param  dma_lli_block_config_t the lli block configuration
 * @param  dma_lli_mode_t the lli mode
 * @return
 */
void dma_lli_init(dma_dev_t* dma, dma_lli_t* dma_lli, dma_lli_block_config_t* dma_lli_block, dma_lli_mode_t* lli_mode)
{
    uint32_t temp_val = 0;
    uint32_t temp_ctl_l, ctl_l, ctl_h;
    dma_config_reg_t config_reg = { 0 };

    if ((dma == NULL) || (dma_lli == NULL) || (dma_lli_block == NULL) || (lli_mode == NULL)
        || (lli_mode->block_num == 0)) {
        return;
    }

    set_dma_mode(dma, &config_reg);

    temp_val                               = (uint32_t)&dma_lli[0];
    temp_val                               = temp_val >> 2;
    *((volatile int*)config_reg.DMA_LLP_L) = (temp_val << 2);

    /* enable llp*/
    if ((lli_mode->src_lli_enable == true) && (lli_mode->dest_lli_enable == true)) {
        write32_bit_reg(config_reg.DMA_CTL_L_REG, 27, 2, 0x3); /*src and dest lli*/
    } else {
        if (lli_mode->src_lli_enable == true) {
            write32_bit_reg(config_reg.DMA_CTL_L_REG, 28, 1, 0x1); /*src lli*/
        } else if (lli_mode->dest_lli_enable == true) {
            write32_bit_reg(config_reg.DMA_CTL_L_REG, 27, 1, 0x1); /*dest lli*/
        } else {
            return;
        }
    }

    /*[0]=1,       int enable*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 0, 1, 0x1);

    /* *((volatile int *)config_reg.DMA_CTL_H) = dma->block_size;*/
    *((volatile int*)config_reg.DMA_CTL_H) = dma_lli_block[0].block_size | (0x1 << 12);

    /*[16:14]=000, src_size*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 14, 3, dma_lli_block[0].src_msize);
    /*[13:11]=000, dest_size*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 11, 3, dma_lli_block[0].dest_msize);
    /*[6:4]=000,   source width*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 4, 3, dma_lli_block[0].data_width);
    /*[3:1]=000,   destination width*/
    write32_bit_reg(config_reg.DMA_CTL_L_REG, 1, 3, dma_lli_block[0].data_width);

    ctl_l = (uint32_t)(TREMO_REG_RD(config_reg.DMA_CTL_L_REG));
    for (temp_val = 0; temp_val < lli_mode->block_num; temp_val++) {
        ctl_h                 = dma_lli_block[temp_val].block_size | (0x1 << 12);
        temp_ctl_l            = write32_bit_variate(ctl_l, 14, 3, dma_lli_block[temp_val].src_msize);
        ctl_l                 = temp_ctl_l;
        temp_ctl_l            = write32_bit_variate(ctl_l, 11, 3, dma_lli_block[temp_val].dest_msize);
        ctl_l                 = temp_ctl_l;
        temp_ctl_l            = write32_bit_variate(ctl_l, 4, 3, dma_lli_block[temp_val].data_width);
        ctl_l                 = temp_ctl_l;
        temp_ctl_l            = write32_bit_variate(ctl_l, 1, 3, dma_lli_block[temp_val].data_width);
        ctl_l                 = temp_ctl_l;
        dma_lli[temp_val].SAR = (uint32_t)dma_lli_block[temp_val].src;
        dma_lli[temp_val].DAR = (uint32_t)dma_lli_block[temp_val].dest;
        if ((lli_mode->block_num - 1) > temp_val) {
            dma_lli[temp_val].LLP = (uint32_t)(&dma_lli[temp_val + 1]);
        }
        dma_lli[temp_val].CTL_L = ctl_l;
        dma_lli[temp_val].CTL_H = ctl_h;
    }

    dma_lli[lli_mode->block_num - 1].LLP   = 0;
    dma_lli[lli_mode->block_num - 1].CTL_L = ctl_l & (~(3 << 27)); // disable src and dest lli

    *((volatile uint32_t*)config_reg.DMA_SAR_L) = (uint32_t)dma->src;
    *((volatile uint32_t*)config_reg.DMA_DAR_L) = (uint32_t)dma->dest;

    if (dma->dma_num == 0) {
        NVIC_EnableIRQ(DMA0_IRQn);
        NVIC_SetPriority(DMA0_IRQn, 2);
    } else {
        NVIC_EnableIRQ(DMA1_IRQn);
        NVIC_SetPriority(DMA1_IRQn, 2);
    }

    if (dma->priv || dma->dma_num == 0) {
        if (dma->ch == 0) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0101);
        } else if (dma->ch == 1) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0202);
        } else if (dma->ch == 2) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0404);
        } else {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val | 0X0808);
        }
        g_dma_callback_handler[dma->dma_num][dma->ch] = (dma_callback_func)(dma->priv);
    } else {
        if (dma->ch == 0) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0101));
        } else if (dma->ch == 1) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0202));
        } else if (dma->ch == 2) {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0404));
        } else {
            temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
            TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(uint32_t)0X0808));
        }
    }
}

/**
 * @brief  Config dma source or destination reload
 * @param  dma the configuration of the interface
 * @param  dma_reload dma reload configuration
 * @return
 */
void dma_config_reload(dma_dev_t* dma, dma_reload_t* dma_reload)
{
    uint32_t * DMA_CFG_L;

    if ((dma == NULL) || (dma_reload == NULL)) {
        return;
    }

    if (dma->ch == 0) {
        DMA_CFG_L = (uint32_t*)&DMA_CFG0_L(dma->dma_num);
    } else if (dma->ch == 1) {
        DMA_CFG_L = (uint32_t*)&DMA_CFG1_L(dma->dma_num);
    } else if (dma->ch == 2) {
        DMA_CFG_L = (uint32_t*)&DMA_CFG2_L(dma->dma_num);
    } else {
        DMA_CFG_L = (uint32_t*)&DMA_CFG3_L(dma->dma_num);
    }

    if (dma_reload->src_reload == true) {
        *((volatile int*)DMA_CFG_L) |= 0x40000000;
    }
    if (dma_reload->dest_reload == true) {
        *((volatile int*)DMA_CFG_L) |= 0x80000000;
    }
}

/**
 * @brief  Deinitialise a dma interface
 * @param  dma_num  the interface number which should be deinitialized
 *            @arg TREMO_DMA_NUM_0: DMA 0
 *            @arg TREMO_DMA_NUM_1: DMA 1
 * @return
 */
void dma_deinit(uint8_t dma_num)
{
    if (dma_num == 0) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_DMA0, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_DMA0, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_DMA0, false);
    } else if (dma_num == (TREMO_DMA_NUM - 1)) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_DMA1, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_DMA1, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_DMA1, false);
    } else {
        return;
    }
}

/**
 * @brief  Update src dest addr
 * @param  dma the configuration of the interface
 * @param  size number of bytes to transmit/recive,value is 0 to 4095
 * @return
 */
void dma_addr_update(dma_dev_t* dma, uint32_t size)
{
    if (dma->ch == 0) {
        while (CHENREG_L(dma->dma_num) & 0x01)
            ;
        DMA_SAR0_L(dma->dma_num) = (uint32_t)dma->src;
        DMA_DAR0_L(dma->dma_num) = (uint32_t)dma->dest;
        DMA_CTL0_H(dma->dma_num) = size;
    } else if (dma->ch == 1) {
        while (CHENREG_L(dma->dma_num) & 0x02)
            ;
        DMA_SAR1_L(dma->dma_num) = (uint32_t)dma->src;
        DMA_DAR1_L(dma->dma_num) = (uint32_t)dma->dest;
        DMA_CTL1_H(dma->dma_num) = size;
    } else if (dma->ch == 2) {
        while (CHENREG_L(dma->dma_num) & 0x04)
            ;
        DMA_SAR2_L(dma->dma_num) = (uint32_t)dma->src;
        DMA_DAR2_L(dma->dma_num) = (uint32_t)dma->dest;
        DMA_CTL2_H(dma->dma_num) = size;
    } else {
        while (CHENREG_L(dma->dma_num) & 0x08)
            ;
        DMA_SAR3_L(dma->dma_num) = (uint32_t)dma->src;
        DMA_DAR3_L(dma->dma_num) = (uint32_t)dma->dest;
        DMA_CTL3_H(dma->dma_num) = size;
    }
}

/**
 * @brief  Enable dma channel
 * @param  dma_num the dma interface number
 *            @arg TREMO_DMA_NUM_0: DMA 0
 *            @arg TREMO_DMA_NUM_1: DMA 1
 * @param  ch dma channel
 *            @arg TREMO_DMA_CHAN_0: DMA channel 0
 *            @arg TREMO_DMA_CHAN_1: DMA channel 1
 *            @arg TREMO_DMA_CHAN_2: DMA channel 2
 *            @arg TREMO_DMA_CHAN_3: DMA channel 3
 * @return
 */
void dma_ch_enable(uint8_t dma_num, uint8_t ch)
{
    if (!(DMACFGREG_L(dma_num) & 0x1)) {
        DMACFGREG_L(dma_num) = 0x1;
    }
    if (ch == 0) {
        while (CHENREG_L(dma_num) & 0x01)
            ;
        CHENREG_L(dma_num) = 0x0101;
        while ((CHENREG_L(dma_num) & 0x1) == 0) {
            ;
        }
    } else if (ch == 1) {
        while (CHENREG_L(dma_num) & 0x02)
            ;
        CHENREG_L(dma_num) = 0x0202;
    } else if (ch == 2) {
        while (CHENREG_L(dma_num) & 0x04)
            ;
        CHENREG_L(dma_num) = 0x0404;
    } else {
        while (CHENREG_L(dma_num) & 0x08)
            ;
        CHENREG_L(dma_num) = 0x0808;
    }
}

/**
 * @brief  Disable dma channel
 * @param  dma_num the dma interface number
 *            @arg TREMO_DMA_NUM_0: DMA 0
 *            @arg TREMO_DMA_NUM_1: DMA 1
 * @param  ch dma channel
 *            @arg TREMO_DMA_CHAN_0: DMA channel 0
 *            @arg TREMO_DMA_CHAN_1: DMA channel 1
 *            @arg TREMO_DMA_CHAN_2: DMA channel 2
 *            @arg TREMO_DMA_CHAN_3: DMA channel 3
 * @return
 */
void dma_ch_disable(uint8_t dma_num, uint8_t ch)
{
    if (ch == 0) {
        CHENREG_L(dma_num) = 0x0100;
    } else if (ch == 1) {
        CHENREG_L(dma_num) = 0x0200;
    } else if (ch == 2) {
        CHENREG_L(dma_num) = 0x0400;
    } else {
        CHENREG_L(dma_num) = 0x0800;
    }
}

/**
 * @brief  Deinitialise a dma interface channel
 * @param  dma the configuration of the interface
 * @return
 */
void dma_finalize(dma_dev_t* dma)
{
    uint32_t temp_val = 0;
    if (dma->ch == 0) {
        temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
        TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(0X0001)));
        if (CHENREG_L(dma->dma_num) & 0x01) {
            write32_bit_reg(DMA_CFG0_L_REG(dma->dma_num), 8, 1, 0x01);
            while (read32_bit_reg(DMA_CFG0_L_REG(dma->dma_num), 9, 1) == 0)
                ;
            CHENREG_L(dma->dma_num) = 0X0100;
        }
        DMA_SAR0_L(dma->dma_num) = 0;
        DMA_DAR0_L(dma->dma_num) = 0;
    } else if (dma->ch == 1) {
        temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
        TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(0X0002)));
        if (CHENREG_L(dma->dma_num) & 0x02) {
            write32_bit_reg(DMA_CFG1_L_REG(dma->dma_num), 8, 1, 0x01);
            while (read32_bit_reg(DMA_CFG1_L_REG(dma->dma_num), 9, 1) == 0)
                ;
            CHENREG_L(dma->dma_num) = 0X0200;
        }
        DMA_SAR1_L(dma->dma_num) = 0;
        DMA_DAR1_L(dma->dma_num) = 0;
    } else if (dma->ch == 2) {
        temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
        TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(0X0004)));
        if (CHENREG_L(dma->dma_num) & 0x04) {
            write32_bit_reg(DMA_CFG2_L_REG(dma->dma_num), 8, 1, 0x01);
            while (read32_bit_reg(DMA_CFG2_L_REG(dma->dma_num), 9, 1) == 0)
                ;
            CHENREG_L(dma->dma_num) = 0X0400;
        }
        DMA_SAR2_L(dma->dma_num) = 0;
        DMA_DAR2_L(dma->dma_num) = 0;

    } else {
        temp_val = TREMO_REG_RD(DMA_MASK_BLOCK_L_REG(dma->dma_num));
        TREMO_REG_WR(DMA_MASK_BLOCK_L_REG(dma->dma_num), temp_val & (~(0X0008)));
        if (CHENREG_L(dma->dma_num) & 0x08) {
            write32_bit_reg(DMA_CFG3_L_REG(dma->dma_num), 8, 1, 0x01);
            while (read32_bit_reg(DMA_CFG3_L_REG(dma->dma_num), 9, 1) == 0)
                ;
            CHENREG_L(dma->dma_num) = 0X0800;
        }
        DMA_SAR3_L(dma->dma_num) = 0;
        DMA_DAR3_L(dma->dma_num) = 0;
    }

    g_dma_callback_handler[dma->dma_num][dma->ch] = NULL;

    if ((DMA_SAR0_L(dma->dma_num) == 0) && (DMA_SAR1_L(dma->dma_num) == 0) && (DMA_SAR2_L(dma->dma_num) == 0)
        && (DMA_SAR3_L(dma->dma_num) == 0)) {
        if (dma->dma_num == 0) {
            NVIC_DisableIRQ(DMA0_IRQn);
        } else {
            NVIC_DisableIRQ(DMA1_IRQn);
        }
        DMACFGREG_L(dma->dma_num) = 0x0; /*disable dma*/
    }
}
