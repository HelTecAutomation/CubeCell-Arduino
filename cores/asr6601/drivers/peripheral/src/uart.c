#include "uart.h"
#include "sx126x-board.h"

uart_t* serialArray[]={UART0,UART1,UART2,UART3};
dma_dev_t uartDmaConf[4]={{0},{0},{0},{0}};

#define UART_BUFF_SIZE 1024
#define UART_TX_QUEUE_SIZE (UART_BUFF_SIZE+1)
#define UART_RX_QUEUE_SIZE (UART_BUFF_SIZE+1)
#define UART_TX_BUF_SIZE 128

bool uart0started = false;
int uartStart(uint32_t baud,uint32_t conf,int rx, int tx)
{
	int uartnum = -1;
	if(rx==tx)
		return uartnum;

	if( tx>47 || rx>47)
		return uartnum;

	uint8_t rxmode;
	uint8_t txmode;
	uint8_t rx_temp = rx;
	uint8_t tx_temp = tx;

	while(rx_temp>15)
		rx_temp-=16;
	while(tx_temp>15)
		tx_temp-=16;

	if( rx_temp <2 && tx_temp<2)
	{
		uartnum = 0;
		if(rx_temp==0)
			rxmode=1;
		else
			rxmode=2;
		if(tx_temp==0)
			txmode=2;
		else
			txmode=1;
	}
	else if( (rx_temp==4||rx_temp==5) && (tx_temp==4||tx_temp==5) )
	{
		uartnum = 1;
		if(rx_temp==4)
			rxmode=1;
		else
			rxmode=2;
		if(tx_temp==4)
			txmode=2;
		else
			txmode=1;
	}
	else if( (rx_temp==8||rx_temp==9) && (tx_temp==8||tx_temp==9) )
	{
		uartnum = 2;
		if(rx_temp==8)
			rxmode=1;
		else
			rxmode=2;
		if(tx_temp==8)
			txmode=2;
		else
			txmode=1;
	}
	else if( (rx_temp==12||rx_temp==13) && (tx_temp==12||tx_temp==13) )
	{
		uartnum = 3;
		if(rx_temp==12)
			rxmode=1;
		else
			rxmode=2;
		if(tx_temp==12)
			txmode=2;
		else
			txmode=1;
	}
	
	if(uartnum<0)
		return uartnum;

	pinMode((uint8_t)rx,ANALOG);
	pinMode((uint8_t)tx,ANALOG);
	iomux((uint8_t)rx,rxmode);
	iomux((uint8_t)tx,txmode);

	uart_t* uartx = serialArray[uartnum];

	uart_config_t config;
	
	config.data_width   = (conf>>16) & 0xFF;
	config.parity       = (conf>>8) & 0xFF;
	config.stop_bits    = conf & 0xFF;
	config.mode         = UART_MODE_TXRX;
	config.flow_control = UART_FLOW_CONTROL_DISABLED;
	config.fifo_mode    = ENABLE;
	config.baudrate = baud;

	//enable uart clk;
	uint32_t peripheral = RCC_PERIPHERAL_UART0 - uartnum;
	rcc_enable_peripheral_clk(peripheral, true);

	//enable dma clk
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_DMA0, true);

	uart_init(uartx, &config);
	
	//set rx fifo 14;
	uint32_t uart_fifo_level;
	if(uartnum==0)
		uart_fifo_level = UART_IFLS_RX_1_8;
	else
		uart_fifo_level = UART_IFLS_RX_7_8;
	uart_set_rx_fifo_threshold(uartx, uart_fifo_level);

	uint8_t uart_irqn = 0;
	if(uartnum<3)
		uart_irqn = 6 + uartnum;
	else
		uart_irqn =4;

	//enable uart Rx interrupt
	uart_config_interrupt(uartx,UART_INTERRUPT_RX_DONE,ENABLE);
	NVIC_EnableIRQ(uart_irqn);
	NVIC_SetPriority(uart_irqn, 2);
	
	uart_cmd(uartx, ENABLE);
	//TREMO_REG_EN(uartx->CR, UART_CR_UART_EN, true);

	//init dma para
	dma_dev_t * dmatemp = &uartDmaConf[uartnum];
	if(dmatemp->started == false)
	{
		dmatemp->dma_buf = (uint8_t*) malloc(UART_TX_BUF_SIZE);
		dmatemp->dma_queue = (uint8_t*) malloc(UART_TX_QUEUE_SIZE);
		dmatemp->dma_busy = 0;
		dmatemp->dma_idx_w = 0;
		dmatemp->dma_idx_r = 0;
		dmatemp->rx_buf = (uint8_t*) malloc(UART_RX_QUEUE_SIZE);
		dmatemp->rx_r = 0;
		dmatemp->rx_w = 0;
		dmatemp->started = true;
	}
	if(uartnum == 0)
		uart0started = true;
	return uartnum;
}
void uartFlushTxOnly(int uart,bool txOnly)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	if(dmatemp->started==false)
		return;
	if(txOnly)
	{
		dmatemp->dma_idx_w = dmatemp->dma_idx_r = 0;
		dma_finalize(dmatemp);
		dmatemp->dma_busy = 0;
	}
}
void uartEnd(int uart,int rx, int tx)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	if(dmatemp->started==false)
		return;
	dmatemp->dma_idx_w = dmatemp->dma_idx_r = 0;
	dmatemp->started = false;
	dma_finalize(dmatemp);
	dmatemp->dma_busy = 0;
	free(dmatemp->dma_buf);
	free(dmatemp->dma_queue);
	free(dmatemp->rx_buf);
	dmatemp->dma_buf = NULL;
	dmatemp->dma_queue = NULL;
	dmatemp->rx_buf = NULL;
	//printf("%d,%d\r\n",malloc_usable_size(uartDmaConf[uart].dma_buf),malloc_usable_size(uartDmaConf[uart].dma_queue));
	pinMode((uint8_t)rx,ANALOG);
	pinMode((uint8_t)tx,ANALOG);

	uint8_t uart_irqn = 0;
	if(uart<3)
		uart_irqn = 6 + uart;
	else
		uart_irqn =4;
	uart_config_interrupt(serialArray[uart],UART_INTERRUPT_RX_DONE,DISABLE);
	NVIC_DisableIRQ(uart_irqn);

	uint32_t peripheral = RCC_PERIPHERAL_UART0 - uart;
	rcc_enable_peripheral_clk(peripheral, false);
	rcc_rst_peripheral(peripheral, true);
	rcc_rst_peripheral(peripheral, false);
	if(uart == 0)
		uart0started = false;
}
int uartAvailableForWrite(int uart)
{
	if(uartDmaConf[uart].started==false)
		return 0;
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	if( dmatemp->dma_idx_w == dmatemp->dma_idx_r )
		return UART_TX_QUEUE_SIZE;
	else if( dmatemp->dma_idx_w > dmatemp->dma_idx_r )
		return UART_TX_QUEUE_SIZE - dmatemp->dma_idx_w + dmatemp->dma_idx_r - 1;
	else
		return dmatemp->dma_idx_r - dmatemp->dma_idx_w - 1;
}

int uart_tx_queue_push(uint8_t c,int uart)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	if(dmatemp->started==false)
		return 0;
	if( ( ( dmatemp->dma_idx_w + 1 ) % UART_TX_QUEUE_SIZE ) == dmatemp->dma_idx_r ){
		return 0;//queue is full , drop log
	}
	bool tempIrq = BoardDisableIrq( );
	dmatemp->dma_queue[dmatemp->dma_idx_w++] = c;
	dmatemp->dma_idx_w = dmatemp->dma_idx_w % UART_TX_QUEUE_SIZE;
	BoardEnableIrq( tempIrq );
	return 1;
}

void uart_tx_queue_pop(uint16_t * size,int uart)
{
    int i, len;
    dma_dev_t * dmatemp = &uartDmaConf[uart];
    if( dmatemp->dma_idx_w > dmatemp->dma_idx_r ) {
        len = dmatemp->dma_idx_w - dmatemp->dma_idx_r;
        if( len > UART_TX_BUF_SIZE ){
            len = UART_TX_BUF_SIZE;
        }
        for( i = 0; i< len; i++){
            dmatemp->dma_buf[i] = dmatemp->dma_queue[ dmatemp->dma_idx_r + i ];
        }
        dmatemp->dma_idx_r += len;
        *size = len;
        //printf("%d %d %d\r\n",uart,dmatemp->dma_idx_w,dmatemp->dma_idx_r);
    } else if( dmatemp->dma_idx_w < dmatemp->dma_idx_r ){
        len = UART_TX_QUEUE_SIZE - dmatemp->dma_idx_r;
        if( len > UART_TX_BUF_SIZE ){
            len = UART_TX_BUF_SIZE;
            for( i = 0; i< len; i++){
                dmatemp->dma_buf[i] = dmatemp->dma_queue[ dmatemp->dma_idx_r + i ];
            }
            dmatemp->dma_idx_r += len;
            *size = len;
        } else {
            for( i = 0; i < len; i++){
                dmatemp->dma_buf[i] = dmatemp->dma_queue[ dmatemp->dma_idx_r + i ];
            }
            dmatemp->dma_idx_r = 0;
            *size = len;
            for( ; dmatemp->dma_idx_r < dmatemp->dma_idx_w  && len < UART_TX_BUF_SIZE ; (dmatemp->dma_idx_r)++, len++) {
                dmatemp->dma_buf[ i + dmatemp->dma_idx_r ] = dmatemp->dma_queue[ dmatemp->dma_idx_r ];
            }
            *size += dmatemp->dma_idx_r;
        }
    }
}

void uart_tx_done_callback()
{
	uint8_t ch = getDmaIrqCh(0);
	clearDmaIrqFlag(0,ch);

	uint16_t size = 0;
	dma_dev_t * dmatemp = &uartDmaConf[ch];
	if( dmatemp->dma_idx_w == dmatemp->dma_idx_r){
		dmatemp->dma_idx_w = dmatemp->dma_idx_r  = 0;
		dmatemp->dma_busy = 0;
		dma_finalize(dmatemp);
	}else{
		uart_tx_queue_pop(&size,ch);
		dmatemp->src        = (uint32_t)(dmatemp->dma_buf);
		dmatemp->block_size = size;
		dma_init(dmatemp);
		dma_ch_enable(dmatemp->dma_num, ch);
		uart_dma_config(serialArray[ch],UART_DMA_REQ_TX, true);
	}
}

bool uartBusy(int uart)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	if(dmatemp->dma_busy == 0)
	{
		return false;
	}
	return true;
}

uint32_t uartGetBaudRate(int uart)
{
	uint32_t uart_clk_freq = 0;
	uint32_t clk_src       = 0;
	uart_t* uartx = serialArray[uart];
	if (uartx == UART0)
		clk_src = (uint32_t)rcc_get_uart0_clk_source() >> 15;
	else if (uartx == UART1)
		clk_src = (uint32_t)rcc_get_uart1_clk_source() >> 13;
	else if (uartx == UART2)
		clk_src = (uint32_t)rcc_get_uart2_clk_source() >> 11;
	else if (uartx == UART3)
		clk_src = (uint32_t)rcc_get_uart3_clk_source() >> 9;

	switch (clk_src)
	{	
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
		default: 
			uart_clk_freq = (uartx == UART0 || uartx == UART1) ? rcc_get_clk_freq(RCC_PCLK0) : rcc_get_clk_freq(RCC_PCLK1);
			break;
	}

	float int_div = (float)(uartx->IBRD);
	float fac_div = (float)(uartx->FBRD & 0x3F);
	
	float temp = int_div +fac_div/64;
	uint32_t baud = uart_clk_freq / 16 / temp;
	return baud;
}

void uartTx(int uart)
{
    uint16_t size = 0;
    dma_dev_t * dmatemp = &uartDmaConf[uart];
    if(dmatemp->started==false)
        return ;
    if( !(dmatemp->dma_busy) ){
        bool tempIrq = BoardDisableIrq( );
        uart_tx_queue_pop(&size,uart);
        if( size > 0 ){
            dmatemp->dma_num    = 0;
            dmatemp->ch         = uart;
            dmatemp->mode       = M2P_MODE;
            dmatemp->src        = (uint32_t)(dmatemp->dma_buf);
            dmatemp->dest       = (uint32_t)(serialArray[uart]);
            dmatemp->priv       = NULL;
            dmatemp->data_width = 0;
            dmatemp->block_size = size;
            dmatemp->src_msize  = 1;
            dmatemp->dest_msize = 1;
            dmatemp->handshake  = DMA_HANDSHAKE_UART_0_TX-2*uart;
            dma_init(dmatemp);
            dma_ch_enable(dmatemp->dma_num, uart);
            uart_dma_config(serialArray[uart],UART_DMA_REQ_TX, true);
            dmatemp->dma_busy = 1;
        }
        BoardEnableIrq( tempIrq );
    }
}

size_t uartWrite(int uart,const uint8_t *buffer, size_t size)
{
	size_t bufIndex,txsize;
	bufIndex = 0;
	txsize = 0;
	while(bufIndex < size)
	{
		txsize += uart_tx_queue_push(buffer[bufIndex],uart);
		bufIndex++;
	}
	uartTx(uart);
	return txsize;
}

void UART_IRQHandler(int uart)
{
	uart_t* uartx = serialArray[uart];
	if(uart_get_interrupt_status(uartx,UART_INTERRUPT_RX_DONE))
	{
		uart_clear_interrupt(uartx,UART_INTERRUPT_RX_DONE);
		dma_dev_t * dmatemp = &uartDmaConf[uart];
		while (uart_get_flag_status(uartx, UART_FLAG_RX_FIFO_EMPTY) != SET)
		{
			if( ((dmatemp->rx_w + 1) % UART_RX_QUEUE_SIZE) != dmatemp->rx_r )
			{
				dmatemp->rx_buf[dmatemp->rx_w++] = uartx->DR & 0xFF;
				dmatemp->rx_w = dmatemp->rx_w % UART_RX_QUEUE_SIZE;
			}
			else
			{
				uint8_t temp = uartx->DR & 0xFF;
			}
		}
	}
}

int uartAvailable(int uart)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	if(dmatemp->started==false)
		return 0;

	//get rx fifo which is not push to buff;
	while (uart_get_flag_status(UART0, UART_FLAG_RX_FIFO_EMPTY) != SET)
	{
		if( ((dmatemp->rx_w + 1) % UART_RX_QUEUE_SIZE) != dmatemp->rx_r )
		{
			dmatemp->rx_buf[dmatemp->rx_w ++] = UART0->DR & 0xFF;
			dmatemp->rx_w = dmatemp->rx_w % UART_RX_QUEUE_SIZE;
		}
		else
		{
			uint8_t temp = UART0->DR & 0xFF;
		}
	}

	if(dmatemp->rx_r == dmatemp->rx_w)
		return 0;
	if(dmatemp->rx_r < dmatemp->rx_w)
		return dmatemp->rx_w - dmatemp->rx_r;
	else
		return UART_RX_QUEUE_SIZE - dmatemp->rx_r + dmatemp->rx_w;
}

uint8_t uartRead(int uart)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	uint8_t data = dmatemp->rx_buf[dmatemp->rx_r++];
	dmatemp->rx_r = dmatemp->rx_r % UART_RX_QUEUE_SIZE;
	return data;
}

uint8_t uartPeek(int uart)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	return dmatemp->rx_buf[dmatemp->rx_r];
}

void uartFlush(int uart)
{
	dma_dev_t * dmatemp = &uartDmaConf[uart];
	if(dmatemp->started==false)
		return;
	//get rx fifo which is not push to buff;
	while (uart_get_flag_status(UART0, UART_FLAG_RX_FIFO_EMPTY) != SET)
	{
		if( ((dmatemp->rx_w + 1) % UART_RX_QUEUE_SIZE) != dmatemp->rx_r )
		{
			dmatemp->rx_buf[dmatemp->rx_w ++] = UART0->DR & 0xFF;
			dmatemp->rx_w = dmatemp->rx_w % UART_RX_QUEUE_SIZE;
		}
		else
		{
			uint8_t temp = UART0->DR & 0xFF;
		}
	}
	dmatemp->rx_w = dmatemp->rx_r = 0;
	uartFlushTxOnly(uart,1);
}

//used for uart tx
void DMA0_IRQHandler(void)
{
	uart_tx_done_callback();
}

extern void ReceiveCommand(void);
extern bool AtEnabled;

uartIrqHandler uart0irq = NULL;

void UART0_IRQHandler()
{
	if(uart0irq!=NULL)
		uart0irq();
	else
		UART_IRQHandler(0);
}

void UART1_IRQHandler()
{
	UART_IRQHandler(1);
}

void UART2_IRQHandler()
{
	UART_IRQHandler(2);
}

void UART3_IRQHandler()
{
	UART_IRQHandler(3);
}

