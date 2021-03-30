#include "tremo_rcc.h"
#include "tremo_crc.h"

/**
 * @brief Deinitializes the CRC registers to the reset values
 * @param None
 * @retval None
 */
void crc_deinit(void)
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_CRC, false);
    rcc_rst_peripheral(RCC_PERIPHERAL_CRC, true);
    rcc_rst_peripheral(RCC_PERIPHERAL_CRC, false);
}

/**
 * @brief Init the CRC according to the settings in the crc_config_t
 * @param config The configuration of the CRC
 * @retval None
 */
void crc_init(crc_config_t* config)
{
    uint32_t tmp = config->poly_size | config->reverse_in | (config->reverse_out ? CRC_CR_REVERSE_OUT_EN : 0);

    CRC->INIT = config->init_value;
    tmp |= CRC_CR_CALC_INIT;

    CRC->POLY = config->poly;

    CRC->CR = tmp;
}

/**
 * @brief Calculate the crc value of the 32-bit array
 * @param data The pointer to the 32-bit input data
 * @param size The size of the input data
 * @retval uint32_t the CRC value 
 */
uint32_t crc_calc32(uint32_t* data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        CRC->DR = data[i];
    }

    while (CRC->CR & CRC_CR_CALC_FLAG)
        ;

    return CRC->DR;
}

/**
 * @brief Calculate the crc value of the 16-bit array
 * @param data The pointer to the 16-bit input data
 * @param size The size of the input data
 * @retval uint32_t the CRC value 
 */
uint32_t crc_calc16(uint16_t* data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        *((uint16_t*)CRC_DR_ADDR) = data[i];
    }

    while (CRC->CR & CRC_CR_CALC_FLAG)
        ;

    return CRC->DR;
}

/**
 * @brief Calculate the crc value of the 8-bit array
 * @param data The pointer to the 8-bit input data
 * @param size The size of the input data
 * @retval uint32_t the CRC value 
 */
uint32_t crc_calc8(uint8_t* data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        *((uint8_t*)CRC_DR_ADDR) = data[i];
    }

    while (CRC->CR & CRC_CR_CALC_FLAG)
        ;

    return CRC->DR;
}
