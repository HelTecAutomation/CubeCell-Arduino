#include "tremo_adc.h"
#include "tremo_rcc.h"
#include "system_cm4.h"

void read_write_adc_afec(uint32_t addr, uint8_t bitBegin, uint8_t bitNum, uint32_t data)
{
    uint32_t readData;
    uint32_t bitSel;
    uint8_t i;

    bitSel = 1 << bitBegin;
    for (i = bitBegin + 1; i < (bitBegin + bitNum); i++)
    {
        bitSel |= (1 << i);
    }
    readData = TREMO_ANALOG_RD(addr);
    readData = (readData & (~bitSel)) | (data & bitSel);
    TREMO_ANALOG_WR(addr, readData);
}

/**
  * @brief  ADC initialization
  * @return None
  */
void adc_init()
{
    read_write_adc_afec(0x11, 6, 0x4, 0x0);
}

/**
  * @brief  ADC start
  * @param[in]  start_flag start or stop flag
  * @return None
  */
void adc_start(bool start_flag)
{
    if (start_flag == true)
    {
			  if (((ADC -> CR) & ADC_ENABLE) && (((ADC -> CR) & ADC_DISABLE) == 0x0))
        {
            ADC -> CR |= ADC_START;
				}
    }
		else
		{
			  if ((ADC -> CR) & ADC_START)
        {
            ADC -> CR |= ADC_STOP;
				}
		}
}

/**
  * @brief  ADC enable
  * @param[in]  enable_flag enable or disable flag
  * @return None
  */
void adc_enable(bool enable_flag)
{
    if (enable_flag == true)
    {
			  if ((ADC -> CR) == 0x0)
        {
            ADC -> CR |= ADC_ENABLE;
				}
    }
		else
		{
			  if (((ADC -> CR) & ADC_ENABLE) && (((ADC -> CR) & ADC_START) == 0x0))
        {
            ADC -> CR |= ADC_DISABLE;
				}
		}
}

/**
  * @brief  ADC enable dma
  * @param[in]  dma_flag enable or disable ADC DMA
  * @return None
  */
void adc_enable_dma(bool dma_flag)
{
    if (dma_flag == true)
    {
        ADC -> CFGR |= ADC_DMA_ENABLE;
    }
		else
		{
        ADC -> CFGR &= (uint32_t)(~(uint32_t)ADC_DMA_ENABLE);
		}
}

/**
  * @brief  ADC retain data when overrun
  * @param[in]  retain_flag retain or not retain data
  * @return None
  */
void adc_retain_data_when_overrun(bool retain_flag)
{
    if (retain_flag == true)
    {
        ADC -> CFGR &= (uint32_t)(~(uint32_t)ADC_OVERRUN_MODE);
    }
		else
		{
        ADC -> CFGR |= ADC_OVERRUN_MODE;
		}
}

/**
  * @brief  ADC enable wait mode
  * @param[in]  wait_flag wait or not wait converse
  * @return None
  */
void adc_enable_wait_mode(bool wait_flag)
{
    if (wait_flag == true)
    {
        ADC -> CFGR |= ADC_WAIT_MODE;
    }
		else
		{
        ADC -> CFGR &= (uint32_t)(~(uint32_t)ADC_WAIT_MODE);
		}
}

/**
  * @brief  ADC config clock division
  * @param[in]  clock_division ADC clock division
  * @return None
  */
void adc_config_clock_division(uint32_t clock_division)
{
    if (clock_division > (uint32_t)ADC_CLK_DIV_MAX)
    {
        return ;
    }
    ADC -> CFGR &= (uint32_t)(~(uint32_t)ADC_CLK_DIV_MAX);
    ADC -> CFGR |= clock_division;
}

/**
  * @brief  ADC config trigger source
  * @param[in]  trigger_source ADC trigger source
  * @return None
  */
void adc_config_trigger_source(adc_trigger_source_t trigger_source)
{
    ADC -> CFGR &= (uint32_t)(~(uint32_t)ADC_TRG_SOURCE_RSV_15);
    ADC -> CFGR |= trigger_source;
}

/**
  * @brief  ADC config trigger polarity
  * @param[in]  trigger_polarity ADC trigger polarity
  * @return None
  */
void adc_config_trigger_polarity(adc_trigger_polarity_t trigger_polarity)
{
    ADC -> CFGR &= (uint32_t)(~(uint32_t)ADC_TRG_POLARITY_BOTH_EDGE);
    ADC -> CFGR |= trigger_polarity;
}

/**
  * @brief  ADC config converse mode
  * @param[in]  conv_mode ADC converse mode
  * @return None
  */
void adc_config_conv_mode(adc_conv_mode_t conv_mode)
{
    ADC -> CFGR &= (uint32_t)(~(uint32_t)ADC_CONV_MODE_RSV);
    ADC -> CFGR |= conv_mode;
}

/**
  * @brief  ADC config converse mode
  * @param[in]  sample_seq_chan ADC sample sequence channel
  * @param[in]  sample_chan ADC sample channel
  * @return None
  */
void adc_config_sample_sequence(uint8_t sample_seq_chan, uint8_t sample_chan)
{
    uint32_t * seqr_sel;
    uint32_t num;

    if (sample_seq_chan < ADC_SAMPLE_SEQ_CHAN_8)
    {
        seqr_sel = (uint32_t *)(&(ADC -> SEQR0));
        num = sample_seq_chan;
    }
    else
    {
        seqr_sel = (uint32_t *)(&(ADC -> SEQR1));
        num = sample_seq_chan - ADC_SAMPLE_SEQ_CHAN_8;
    }
    *seqr_sel &= (uint32_t)(~(uint32_t)(0xf << (4 * num)));
    *seqr_sel |= (sample_chan << (4 * num));
}

/**
  * @brief  ADC config sample channel difference
  * @param[in]  diff_sel ADC difference sample channel
  * @param[in]  dif_flag ADC sample channel difference or not
  * @return None
  */
void adc_config_sample_chan_diff(adc_diff_sel_t diff_sel, bool dif_flag)
{
    if (dif_flag == true)
    {
        ADC -> DIFFSEL |= diff_sel;
    }
		else
		{
        ADC -> DIFFSEL &= (uint32_t)(~(uint32_t)diff_sel);
		}
}

/**
  * @brief  ADC get interrupt status
  * @param[in]  isr_flag ADC interrupt status
  * @return bool
  */
bool adc_get_interrupt_status(adc_isr_t isr_flag)
{
    if ((ADC -> ISR & isr_flag) == isr_flag)
    {
        return true;
    }
		else
		{
        return false;
		}
}

/**
  * @brief  ADC clear interrupt status
  * @param[in]  isr_flag ADC interrupt status
  * @return None
  */
void adc_clear_interrupt_status(adc_isr_t isr_flag)
{
    if ((ADC -> ISR & isr_flag) == isr_flag)
    {
        ADC -> ISR |= (uint32_t)isr_flag;
    }
}

/**
  * @brief  ADC config interrupt
  * @param[in]  interrupt ADC interrupt
  * @param[in]  enable_flag ENABLE or DISABLE interrupt
  * @return None
  */
void adc_config_interrupt(adc_ier_t interrupt, bool enable_flag)
{
    if (enable_flag == true)
    {
        ADC -> IER |= (uint32_t)interrupt;
    }
    else
    {
        ADC -> IER &= (uint32_t)~(uint32_t)interrupt;
    }
}

/**
  * @brief  ADC get interrupt status
  * @param[in]  isr_flag ADC interrupt status
  * @return uint32_t
  */
uint16_t adc_get_data()
{
    return (uint16_t)((ADC -> DR) & 0x0fff);
}

/**
  * @brief  ADC get calibration value
  * @param[in]  dif_flag ADC sample channel difference or not
  * @param[out]  gain_value calibration gain value
  * @param[out]  dco_value calibration dco value
  * @return None
  */
void adc_get_calibration_value(bool dif_flag, float *gain_value, float *dco_value)
{
    if (dif_flag != true)
    {
        *gain_value = 1.000 + 0.002 * (((*(uint32_t *)0x10002030) & 0x1fe0) >> 5);
        *dco_value = -0.256 + 0.001 * (((*(uint32_t *)0x10002034) & 0x1ff0) >> 4);
    }
    else
    {
        *gain_value = 1.000 + 0.002 * (((*(uint32_t *)0x10002030) & 0x1fe000) >> 13);
        *dco_value = -0.256 + 0.001 * (((*(uint32_t *)0x10002034) & 0x3fe000) >> 13);
    }
}
