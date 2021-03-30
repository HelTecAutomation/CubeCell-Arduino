#include "tremo_i2s.h"
#include "tremo_rcc.h"

/**
 * @brief  Initialize configuration
 * @param  i2s_struct the configuration of I2S
 * @return
 */
void i2s_init_struct(i2s_init_t* i2s_struct)
{
    i2s_struct->i2s_role           = I2S_ROLE_MASTER;
    i2s_struct->i2s_word_size      = I2S_WORDSIZE_32bit;
    i2s_struct->i2s_fifo_threshold = I2S_FIFO_TRIGGERL_LEVEL_1;
}

/**
 * @brief  Calculate sclk devision
 * @param  word_size I2S word size
 * @return frequece devision
 */
uint8_t i2s_calculate_devision(uint8_t word_size)
{
    uint8_t freq_devision;

    if (word_size == I2S_WORDSIZE_DONT_CARE) {
        freq_devision = 21;
    } else if (word_size == I2S_WORDSIZE_12bit) {
        freq_devision = 13;
    } else if (word_size == I2S_WORDSIZE_16bit) {
        freq_devision = 17;
    } else if (word_size == I2S_WORDSIZE_20bit) {
        freq_devision = 21;
    } else if (word_size == I2S_WORDSIZE_24bit) {
        freq_devision = 25;
    } else if (word_size == I2S_WORDSIZE_32bit) {
        freq_devision = 33;
    } else {
        freq_devision = 21;
    }

    return freq_devision;
}

/**
 * @brief  Config interrupt
 * @param  I2Sx I2S handler
 * @param  i2s_interrupt I2S interrupt
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void i2s_config_interrupt(i2s_t* I2Sx, uint32_t i2s_interrupt, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->IMR &= ~i2s_interrupt;
    } else {
        I2Sx->IMR |= i2s_interrupt;
    }
}

/**
 * @brief  Clear interrupt
 * @param  I2Sx I2S handler
 * @param  i2s_interrupt I2S interrupt
 * @return
 */
void i2s_clear_interrupt(i2s_t* I2Sx, uint32_t i2s_interrupt)
{
    if (i2s_interrupt == I2S_INTERRUPT_TXFO)
        I2Sx->TOR; // read to clear interrupt
    else if (i2s_interrupt == I2S_INTERRUPT_RXFO)
        I2Sx->ROR;
}

/**
 * @brief  Get interrupt status
 * @param  I2Sx I2S handler
 * @param  i2s_interrupt I2S interrupt
 * @return true generate interrupt
 * @return false no interrupt
 */
bool i2s_get_interrupt_status(i2s_t* I2Sx, uint32_t i2s_interrupt)
{
    return (I2Sx->ISR & i2s_interrupt) ? true : false;
}

/**
 * @brief  Enalbe or disable I2S
 * @param  I2Sx I2S handler
 * @param  state true or false
 * @return
 */
void i2s_cmd(i2s_t* I2Sx, bool state)
{
    if (state == true) {
        I2Sx->IER |= ENABLE;
    } else {
        I2Sx->IER &= DISABLE;
    }
}

/**
 * @brief  Enalbe or disable I2S tx block
 * @param  I2Sx I2S handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void i2s_tx_block_cmd(i2s_t* I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->ITER |= ENABLE;
    } else {
        I2Sx->ITER &= DISABLE;
    }
}

/**
 * @brief  Enalbe or disable I2S rx block
 * @param  I2Sx I2S handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void i2s_rx_block_cmd(i2s_t* I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->IRER |= ENABLE;
    } else {
        I2Sx->IRER &= DISABLE;
    }
}

/**
 * @brief  Enalbe or disable I2S tx channel
 * @param  I2Sx I2S handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void i2s_tx_channel_cmd(i2s_t* I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->TER |= ENABLE;
    } else {
        I2Sx->TER &= DISABLE;
    }
}

/**
 * @brief  Enalbe or disable I2S rx channel
 * @param  I2Sx I2S handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void i2s_rx_channel_cmd(i2s_t* I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->RER |= ENABLE;
    } else {
        I2Sx->RER &= DISABLE;
    }
}

/**
 * @brief  Enalbe or disable master clock
 * @param  I2Sx I2S handler
 * @param  new_state ENABLE or DISABLE
 * @return
 */
void i2s_master_clock_cmd(i2s_t* I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->CER |= ENABLE;
    } else {
        I2Sx->CER &= DISABLE;
    }
}

/**
 * @brief  I2S initialize
 * @param  I2Sx I2S handler
 * @param  i2s_struct the configuration of I2S
 * @return
 */
void i2s_init(i2s_t* I2Sx, i2s_init_t* i2s_struct)
{
    i2s_cmd(I2Sx, true);

    I2Sx->RCR &= ~I2S_RX_WORDSIZE_MASK;
    I2Sx->RCR |= i2s_struct->i2s_word_size;

    I2Sx->TCR &= ~I2S_TX_WORDSIZE_MASK;
    I2Sx->TCR |= i2s_struct->i2s_word_size;

    I2Sx->RFCR &= ~I2S_FIFO_TRIGGER_LEVEL_MASK;
    I2Sx->RFCR |= i2s_struct->i2s_fifo_threshold;

    I2Sx->TFCR &= ~I2S_FIFO_TRIGGER_LEVEL_MASK;
    I2Sx->TFCR |= i2s_struct->i2s_fifo_threshold;
}

/**
 * @brief  I2S deinitialize
 * @param  I2Sx I2S handler
 * @return
 */
void i2s_deinit(i2s_t* I2Sx)
{
    if (I2Sx == I2S) {
        rcc_enable_peripheral_clk(RCC_PERIPHERAL_I2S, false);
        rcc_rst_peripheral(RCC_PERIPHERAL_I2S, true);
        rcc_rst_peripheral(RCC_PERIPHERAL_I2S, false);
    } else {
        return;
    }
}

/**
 * @brief  I2S initialize
 * @param  I2Sx I2S handler
 * @param  lr left or right channel
 * @return received data
 */
uint32_t i2s_receive_data(i2s_t* I2Sx, uint8_t lr)
{
    if (lr == I2S_LEFT_CHANNEL) // left channel
        return I2Sx->LRBR_LTHR;
    else // right channel
        return I2Sx->RRBR_RTHR;
}

/**
 * @brief  I2S initialize
 * @param  I2Sx I2S handler
 * @param  left_chan_data left channel data
 * @param  right_chan_data right channel data
 * @param  len right data length
 */
void i2s_send_data(i2s_t* I2Sx, uint16_t* left_chan_data, uint16_t* right_chan_data, uint32_t len)
{
    uint8_t word_size;

    word_size = I2Sx->TCR & I2S_TX_WORDSIZE_MASK;

    for (int i = 0; i < len; i++) {
        while (!i2s_get_interrupt_status(I2Sx, I2S_INTERRUPT_TXFE))
            ; // wait till tx fifo emptys
        if ((word_size > I2S_WORDSIZE_16bit) || (word_size == I2S_WORDSIZE_DONT_CARE)) {
            I2Sx->LRBR_LTHR = *((uint32_t*)left_chan_data + i);
            I2Sx->RRBR_RTHR = *((uint32_t*)right_chan_data + i);
        } else {
            I2Sx->LRBR_LTHR = *((uint16_t*)left_chan_data + i);
            I2Sx->RRBR_RTHR = *((uint16_t*)right_chan_data + i);
        }
    }
}
