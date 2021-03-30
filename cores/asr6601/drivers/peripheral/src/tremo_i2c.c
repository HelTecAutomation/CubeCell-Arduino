#include "tremo_rcc.h"
#include "tremo_i2c.h"

static int8_t unit_reset(i2c_t* i2cx, uint32_t t_timeout)
{
    /* check unit busy */
    int32_t temp = t_timeout;
    while ((i2cx->SR & I2C_SR_UNIT_BUSY_MASK) && (temp--))
        ;

    if (temp) {
        i2cx->CR &= I2C_CR_UNIT_RESET_MASK; // clear rest of CR
        i2cx->CR |= I2C_CR_UNIT_RESET_MASK; // set RESET bit
		
        i2cx->SR = i2cx->SR;
        i2cx->CR &= ~I2C_CR_UNIT_RESET_MASK; // clear RESET bit
    } else {
        return ERRNO_ERROR;
    }

    return ERRNO_OK;
}

/**
 * @brief Deinitializes the I2C peripheral registers to the reset values
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @retval None
 */
void i2c_deinit(i2c_t* i2cx)
{
    uint32_t peripheral = RCC_PERIPHERAL_I2C0;

    if (i2cx == I2C1)
        peripheral = RCC_PERIPHERAL_I2C1;
    else if (i2cx == I2C2)
        peripheral = RCC_PERIPHERAL_I2C2;

    rcc_enable_peripheral_clk(peripheral, false);
    rcc_rst_peripheral(peripheral, true);
    rcc_rst_peripheral(peripheral, false);
}

/**
 * @brief Set the default value of the I2C configuration
 * @param config I2C configuration
 * @retval None
 */
void i2c_config_init(i2c_config_t* config)
{
    config->mode                  = I2C_MODE_MASTER;
    config->fifo_mode_en          = false;
    config->settings.master.speed = I2C_CR_BUS_MODE_FAST;
    config->freq                 = 100000;
}

/**
 * @brief Init the I2C peripheral registers according to the specified parameters
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param config I2C configuration
 * @retval None
 */
void i2c_init(i2c_t* i2cx, i2c_config_t* config)
{
    /* reset unit */
    unit_reset(i2cx, 1000);

    TREMO_REG_EN(i2cx->CR, I2C_CR_FIFO_EN_MASK, config->fifo_mode_en);

    uint32_t i2c_clk_freq = (i2cx == I2C0) ? rcc_get_clk_freq(RCC_PCLK0) : rcc_get_clk_freq(RCC_PCLK1);
    uint32_t flv;
    //uint32_t i2c_speed_standard = 100000;
    //uint32_t i2c_speed_fast     = 400000;
    //printf("%d\r\n",i2c_clk_freq);
    //slv                         = (i2c_clk_freq / i2c_speed_standard - 8) / 2;
    //flv                         = (i2c_clk_freq / i2c_speed_fast - 8) / 2 - 1;
	if(config->freq < 50000)
		config->freq = 50000;
	if(config->freq > 600000)
		config->freq = 600000;


	/*if(config->freq<200000)
		flv = ( i2c_clk_freq / config->freq )/2 - 20;
	else
		flv = ( i2c_clk_freq / config->freq )/2 - 20;
	*/
	
	flv = ( i2c_clk_freq / config->freq )/2 - 19 - round(config->freq/100000);
	i2cx->LCR = flv << 9; // set divider
    //i2cx->LCR = slv | flv << 9; // set divider
    i2cx->WCR = flv / 3;        // set wait count value to adjust clock for
	
    if (config->mode == I2C_MODE_MASTER) {
        TREMO_REG_SET(i2cx->CR, I2C_CR_BUS_MODE_MASK, config->settings.master.speed );
    }else{
        i2cx->SAR = config->settings.slave.slave_addr; // set unit address as slave
    }
}

/**
 * @brief Enable or disable the I2C peripheral
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param newstate true or false
 * @retval None
 */
void i2c_cmd(i2c_t* i2cx, bool newstate)
{
    TREMO_REG_EN(i2cx->CR, I2C_CR_TWSI_UNIT_EN_MASK, newstate);
    TREMO_REG_EN(i2cx->CR, I2C_CR_SCL_EN_MASK, newstate);
}

/**
 * @brief Enable or disable the I2C DMA function
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param newstate true or false
 * @retval None
 */
void i2c_dma_cmd(i2c_t* i2cx, bool newstate)
{
    TREMO_REG_EN(i2cx->CR, I2C_CR_DMA_EN_MASK, newstate);
}

/**
 * @brief Enable or disable the I2C specified interrupt
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param interrupt The I2C interrupts
 *        - @ref i2c_interrupt_t
 * @param new_state true or false
 * @retval None
 */
void i2c_config_interrupt(i2c_t* i2cx, i2c_interrupt_t interrupt, bool new_state)
{
    if (interrupt == I2C_INTR_MASTER_STOP_DET)
        TREMO_REG_EN(i2cx->CR, I2C_CR_MASTER_STOP_DET_EN_MASK, new_state);
    TREMO_REG_EN(i2cx->CR, (1 << interrupt), new_state);
}

/**
 * @brief Clear the I2C interrupt status
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param interrupt The I2C interrupts
 *        - @ref i2c_interrupt_t
 * @retval None
 */
void i2c_clear_interrupt(i2c_t* i2cx, i2c_interrupt_t interrupt)
{
    i2cx->SR = 1 << interrupt;
}

/**
 * @brief Send the start request for I2C master
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param slave_addr The slave address
 * @param bit_rw The read/write bit
 *           @arg I2C_WRITE: write
 *           @arg I2C_READ:  read
 * @retval None
 */
void i2c_master_send_start(i2c_t* i2cx, uint8_t slave_addr, uint8_t bit_rw)
{
    uint8_t data = (slave_addr << 1) | bit_rw;

    if (IS_I2C_FIFO_MODE(i2cx->CR)) {
        i2cx->CR |= I2C_CR_TRANS_BEGIN_MASK;

        i2cx->WFIFO = data | I2C_WFIFO_CONTROL_START_MASK | I2C_WFIFO_CONTROL_TB_MASK;
    } else {
        i2cx->DBR = data;
        i2cx->CR &= ~(I2C_CR_MASTER_ABORT_MASK);
        i2cx->CR |= I2C_CR_START_MASK | I2C_CR_TRANS_BYTE_MASK;
    }
}

/**
 * @brief Send the stop request for I2C master
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @retval None
 */
void i2c_master_send_stop(i2c_t* i2cx)
{
    i2cx->CR &= ~(I2C_CR_START_MASK);
    i2cx->CR |= I2C_CR_MASTER_ABORT_MASK;
}

/**
 * @brief Send the stop request with data for I2C master
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @retval None
 */
void i2c_master_send_stop_with_data(i2c_t* i2cx, uint8_t data)
{
    if (IS_I2C_FIFO_MODE(i2cx->CR)) {
        i2cx->WFIFO = data | I2C_WFIFO_CONTROL_TB_MASK | I2C_WFIFO_CONTROL_STOP_MASK;
    } else {
        i2cx->DBR = data;

        i2cx->CR &= ~(I2C_CR_START_MASK);
        i2cx->CR |= I2C_CR_TRANS_BYTE_MASK;
        i2cx->CR |= I2C_CR_STOP_MASK;
    }
}

/**
 * @brief Write a byte to send
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param data The byte to be sent
 * @retval None
 */
void i2c_send_data(i2c_t* i2cx, uint8_t data)
{
    if (IS_I2C_FIFO_MODE(i2cx->CR)) {
        i2cx->WFIFO = data | I2C_WFIFO_CONTROL_TB_MASK;
    } else {
        i2cx->DBR = data;

        i2cx->CR &= ~(I2C_CR_START_MASK);
        i2cx->CR |= I2C_CR_TRANS_BYTE_MASK;
    }
}

/**
 * @brief Read a byte
 * @note Please call the i2c_set_receive_mode first
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2) 
 * @retval uint8_t The byte received
 */
uint8_t i2c_receive_data(i2c_t* i2cx)
{
	uint8_t data = 0;
    if (IS_I2C_FIFO_MODE(i2cx->CR)) {
        data = i2cx->RFIFO;
    } else {
        data = i2cx->DBR;
    }
	
	return data;
}

/**
 * @brief Config the receive mode and initial the receive transaction
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param ack The ACK bit setting in receive mode
 *        - @ref i2c_ack_t
 * @retval None
 */
void i2c_set_receive_mode(i2c_t* i2cx, i2c_ack_t ack)
{
    if (IS_I2C_FIFO_MODE(i2cx->CR)) {
        i2cx->WFIFO = I2C_WFIFO_CONTROL_TB_MASK | ((!ack) ? I2C_WFIFO_CONTROL_ACKNAK_MASK : 0);
    } else {
        TREMO_REG_EN(i2cx->CR, I2C_CR_ACKNAK_MASK, !ack);
        i2cx->CR &= ~(I2C_CR_START_MASK);
        i2cx->CR |= I2C_CR_TRANS_BYTE_MASK;
    }
}

/**
 * @brief Clear the flag status of the specified I2C
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param flag The specified flag
 *        - @ref i2c_flag_t
 * @retval None 
 */
void i2c_clear_flag_status(i2c_t* i2cx, i2c_flag_t flag)
{
    i2cx->SR = 1 << flag;
}

/**
 * @brief Get the flag status of the specified I2C flag
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param flag The specified flag
 *        - @ref i2c_flag_t
 * @retval SET or RESET 
 */
flag_status_t i2c_get_flag_status(i2c_t* i2cx, i2c_flag_t flag)
{
    if (flag == I2C_FLAG_RFIFO_EMPTY)
        return ((I2C0->RFIFO_STATUS & I2C_RFIFO_STATUS_SIZE_MASK) == 0 ? SET : RESET);

    if (i2cx->SR & (1 << flag))
        return SET;
    else
        return RESET;
}

/**
 * @brief Get the interrupt status of the specified I2C
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param intr The specified interrupt
 *        - @ref i2c_interrupt_t
 * @retval None 
 */
it_status_t i2c_get_interrupt_status(i2c_t* i2cx, i2c_interrupt_t intr)
{
    if (i2cx->SR & (1 << intr))
        return SET;
    else
        return RESET;
}
