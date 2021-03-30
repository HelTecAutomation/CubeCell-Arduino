#include "asr6601_spi.h"

uint8_t spi_bitOrder[3]={0,0,0};
int8_t getSpiNum(spi_t * spi);

spi_t * spiStartBus(int8_t spi_num, uint32_t freq, uint8_t dataMode, uint8_t bitOrder,int8_t sck,int8_t mosi,int8_t miso,int8_t ss)
{
	spi_num = -1;
	uint8_t sck_temp = sck;
	uint8_t miso_temp = miso;
	uint8_t mosi_temp = mosi;
	
	if( sck>47 || mosi>47 || miso>47 || sck<0 || mosi<0 || miso<0)
		return NULL;
		
	while(sck_temp>19)
		sck_temp-=20;
	while(miso_temp>19)
		miso_temp-=20;
	while(mosi_temp>19)
		mosi_temp-=20;

	if( sck_temp == 0 && miso_temp == 3 && mosi_temp == 2)
	{
		spi_num = 0;
	}
	else if( sck_temp == 4 && miso_temp == 7 && mosi_temp == 6)
	{
		spi_num = 1;
	}
	else if( sck_temp == 8 && miso_temp == 11 && mosi_temp == 10)
	{
		spi_num = 2;
	}
	
	if(spi_num<0)
		return NULL;

	pinMode((uint8_t)sck,ANALOG); 
	pinMode((uint8_t)mosi,ANALOG);
	pinMode((uint8_t)miso,ANALOG);
	
	iomux((uint8_t)sck,4);
	iomux((uint8_t)mosi,4);
	iomux((uint8_t)miso,4);
	iomux((uint8_t)ss,0);
	if(ss>=0)
	{
		pinMode((uint8_t)ss,OUTPUT);
		digitalWrite(ss, HIGH);
	}
	spi_t* _spi_bus_array[] = {SPI_0,SPI_1,SPI_2};
	spi_t* spi = _spi_bus_array[spi_num];
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_SSP0-spi_num, true);

	ssp_init_t init_struct;
	ssp_init_struct(&init_struct);
	spiSetBitOrder(spi,bitOrder);
	
	init_struct.ssp_sclk       = freq;
	switch (dataMode) {
		case SPI_MODE1:
			init_struct.ssp_clk_phase = SPI_CLK_PHASE_2EDGE;
			init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_LOW;
			break;
		case SPI_MODE2:
			init_struct.ssp_clk_phase = SPI_CLK_PHASE_1EDGE;
			init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_HIGH;
			break;
		case SPI_MODE3:
			init_struct.ssp_clk_phase = SPI_CLK_PHASE_2EDGE;
			init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_HIGH;
			break;
		case SPI_MODE0:
		default:
			init_struct.ssp_clk_phase = SPI_CLK_PHASE_1EDGE;
			init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_LOW;
			break;
	}
	ssp_init(spi, &init_struct);
	ssp_cmd(spi, true);
	return spi;
}

int spiSSEnable(spi_t * spi,int8_t ss)
{
	uint8_t spi_num = getSpiNum(spi);
	if(spi_num == -1)
		return 0;

	uint8_t ss_temp = ss;
	
	if( ss>47 || ss<0 )
		return -1;
		
	while(ss_temp>19)
		ss_temp-=20;

	if( ss_temp != (1 + 4*spi_num))
	{
		return -1;
	}
	pinMode((uint8_t)ss,ANALOG);
	iomux((uint8_t)ss,4);
	return 0;
}

void spiSSDisable(spi_t * spi,int8_t ss)
{
	iomux((uint8_t)ss,0);
	pinMode((uint8_t)ss,OUTPUT);
	digitalWrite(ss, HIGH);
}


void spiStopBus(spi_t * spi,int8_t sck,int8_t mosi,int8_t miso,int8_t ss)
{
	uint8_t spi_num = getSpiNum(spi);
	if(spi_num == -1)
		return;

	pinMode((uint8_t)sck,ANALOG);
	pinMode((uint8_t)mosi,ANALOG);
	pinMode((uint8_t)miso,ANALOG);
	pinMode((uint8_t)ss,ANALOG);

	uint32_t peripheral = RCC_PERIPHERAL_SSP0 - spi_num;
	rcc_enable_peripheral_clk(peripheral, false);
	rcc_rst_peripheral(peripheral, true);
	rcc_rst_peripheral(peripheral, false);
}

void spiWaitReady(spi_t * spi)
{
}

void spiWrite(spi_t * spi, uint32_t *data, uint8_t len)
{
	if(spi == NULL)
		return;
	uint32_t out[len];
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&out, (uint32_t)(4*len));
}

void spiTransfer(spi_t * spi, uint32_t *data, uint8_t len)
{
	if(spi == NULL)
		return;
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&data, (uint32_t)(4*len));
}

void spiWriteByte(spi_t * spi, uint8_t data)
{
	if(spi == NULL)
		return;
	uint8_t out;
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&out, 1);
}

uint8_t spiTransferByte(spi_t * spi, uint8_t data)
{
	if(spi == NULL)
		return 0;
	uint8_t out;
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&out, 1);
	return out;
}

void spiWriteWord(spi_t * spi, uint16_t data)
{
	if(spi == NULL)
		return;
	uint16_t out;
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&out, 2);
}

uint16_t spiTransferWord(spi_t * spi, uint16_t data)
{
	if(spi == NULL)
		return 0;
	uint16_t out;
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&out, 2);
	return out;
}

void spiWriteLong(spi_t * spi, uint32_t data)
{
	if(spi == NULL)
		return;
	uint32_t out;
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&out, 2);
}

uint32_t spiTransferLong(spi_t * spi, uint32_t data)
{
	if(spi == NULL)
		return 0;
	uint32_t out;
	spiTransferBytes(spi, (uint8_t*)&data, (uint8_t*)&out, 2);
	return out;
}

uint8_t byteReverse(uint8_t byte)
{
	uint8_t temp = 0;
	temp |= (byte & 0x01)<<7;
	temp |= (byte & 0x02)<<5;
	temp |= (byte & 0x04)<<3;
	temp |= (byte & 0x08)<<1;
	temp |= (byte & 0x10)>>1;
	temp |= (byte & 0x20)>>3;
	temp |= (byte & 0x40)>>5;
	temp |= (byte & 0x80)>>7;
	return temp;
}
void spiTransferBytes(spi_t * spi, uint8_t * data, uint8_t * out, uint32_t size)
{
	if(spi == NULL)
		return;
		
	uint32_t starttime;
	uint32_t status;
	
	uint8_t spi_num = getSpiNum(spi);
	uint8_t bitOrder = spi_bitOrder[spi_num];

	uint8_t dataSend,dataRev;

	for (uint32_t i=0;i<size;i++)
	{
		dataSend = * data;
		if(bitOrder == LSBFIRST)
			dataSend = byteReverse(dataSend);
		spi->DR = dataSend;
		starttime = millis();
		while(1) 
		{
			status = spi->SR;
			if(((status & 0x01) == 0x01) && ((status & 0x10)==0)) break;
			if(millis() - starttime > SPI_TIMEOUT) return;
		}
		dataRev = spi->DR & 0xFF;
		if(bitOrder == LSBFIRST)
			dataRev = byteReverse(dataRev);
		*out = dataRev;
		data++;
		out++;
	}
}

uint8_t spiGetDataMode(spi_t * spi)
{
	if(spi == NULL)
		return 0;
	uint8_t mode = spi->CR0 & (0x3 << 6);
	uint8_t pol,phase;
	if(mode & SPI_CLK_PHASE_2EDGE)
		phase = 1;
	else
		phase = 0;

	if(mode & SPI_CLK_POLARITY_HIGH)
		pol = 1;
	else
		pol = 0;
	return (pol<<1) | phase;
}

void spiSetDataMode(spi_t * spi, uint8_t dataMode)
{
	if(spi == NULL)
		return;

	uint8_t ssp_clk_phase,ssp_clk_pol;
	switch (dataMode) {
		case SPI_MODE1:
			ssp_clk_phase = SPI_CLK_PHASE_2EDGE;
			ssp_clk_pol   = SPI_CLK_POLARITY_LOW;
			break;
		case SPI_MODE2:
			ssp_clk_phase = SPI_CLK_PHASE_1EDGE;
			ssp_clk_pol   = SPI_CLK_POLARITY_HIGH;
			break;
		case SPI_MODE3:
			ssp_clk_phase = SPI_CLK_PHASE_2EDGE;
			ssp_clk_pol   = SPI_CLK_POLARITY_HIGH;
			break;
		case SPI_MODE0:
		default:
			ssp_clk_phase = SPI_CLK_PHASE_1EDGE;
			ssp_clk_pol   = SPI_CLK_POLARITY_LOW;
			break;
	}
	/* set sclk polarity & phase */
	spi->CR0 &= ~(0x3 << 6); /* reset SPI clk phase/polarity setting to mode 0 */
	spi->CR0 |= ssp_clk_pol | ssp_clk_phase;
}

/*
 * Manual Lock Management
 * */
void spiTransaction(spi_t * spi, uint32_t freq, uint8_t dataMode, uint8_t bitOrder)
{
	if(spi == NULL)
		return;
	spiSetFrequency(spi,freq);
	spiSetDataMode(spi,dataMode);
	spiSetBitOrder(spi,bitOrder);
}

uint32_t spiSetFrequency(spi_t * spi,uint32_t freq)
{
	if(spi == NULL)
		return 0;

	uint32_t ssp_clk_freq;
	
	if (spi == SPI_0) {
		ssp_clk_freq = rcc_get_clk_freq(RCC_PCLK0);
	} else {
		ssp_clk_freq = rcc_get_clk_freq(RCC_PCLK1);
	}
	uint8_t scr = ((ssp_clk_freq / 2 / freq) - 1) ? ((ssp_clk_freq / 2 / freq) - 1) : 0;
	spi->CR0 &= ~(0xff << 8); /* reset SCR to 0 */
	spi->CR0 |= scr << 8;
	return 1;
}

uint32_t spiGetFrequency(spi_t * spi)
{
	if(spi == NULL)
		return 0;

	uint32_t ssp_clk_freq;
	if (spi == SPI_0) {
		ssp_clk_freq = rcc_get_clk_freq(RCC_PCLK0);
	} else {
		ssp_clk_freq = rcc_get_clk_freq(RCC_PCLK1);
	}
	uint8_t scr = (spi->CR0>>8) & 0xFF;
	return ssp_clk_freq / 2 / (scr+1);
}


uint8_t spiGetBitOrder(spi_t * spi)
{
	uint8_t spi_num = getSpiNum(spi);
	if(spi_num == -1)
		return -1;
	return spi_bitOrder[spi_num];
}

void spiSetBitOrder(spi_t * spi, uint8_t bitOrder)
{
	uint8_t spi_num = getSpiNum(spi);
	if(spi_num == -1)
		return;
	spi_bitOrder[spi_num] = bitOrder;
}
int8_t getSpiNum(spi_t * spi)
{
	if(spi==SPI_0)
		return 0;
	else if(spi==SPI_1)
		return 1;
	else if(spi==SPI_2)
		return 2;
	else
		return -1;
}
