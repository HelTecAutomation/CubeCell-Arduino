/**
 ******************************************************************************
 * @file    tremo_crc.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the CRC firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup CRC
 * @{
 */

#ifndef __TREMO_CRC_H
#define __TREMO_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "tremo_regs.h"

#define CRC_DR_ADDR (0x40022004)

/**
 * @brief The bits of the CRC polynomial value
 */
typedef enum {
    CRC_POLY_SIZE_7  = CRC_CR_POLY_SIZE_7,   /*!< 7bits polynomial size */
    CRC_POLY_SIZE_8  = CRC_CR_POLY_SIZE_8,   /*!< 8bits polynomial size */
    CRC_POLY_SIZE_16 = CRC_CR_POLY_SIZE_16,  /*!< 16bits polynomial size */
    CRC_POLY_SIZE_32 = CRC_CR_POLY_SIZE_32,  /*!< 32bits polynomial size */
} crc_poly_size_t;

/**
 * @brief The bit order reverse config of input data
 */
typedef enum {
    CRC_REVERSE_IN_NONE  = CRC_CR_REVERSE_IN_NONE,   /*!< The bit order of the input data is not reversed */
    CRC_REVERSE_IN_BYTE  = CRC_CR_REVERSE_IN_BYTE,   /*!< The bit order of the input data is reversed by byte */
    CRC_REVERSE_IN_HWORD = CRC_CR_REVERSE_IN_HWORD,  /*!< The bit order of the input data is reversed by half-word */
    CRC_REVERSE_IN_WORD  = CRC_CR_REVERSE_IN_WORD,   /*!< The bit order of the input data is reversed by word */
} crc_reverse_in_t;

/**
 * @brief The configuration of the CRC
 */
typedef struct {
    uint32_t init_value;          /*!< The init value of the CRC */
    crc_poly_size_t poly_size;    /*!< The bits of the CRC polynomial value */
    uint32_t poly;                /*!< The the CRC polynomial value */
    crc_reverse_in_t reverse_in;  /*!< The reverse config of the input data */
    bool reverse_out;             /*!< The reverse config of the output data */
} crc_config_t;

void crc_deinit(void);
void crc_init(crc_config_t* config);
uint32_t crc_calc32(uint32_t* data, uint32_t size);
uint32_t crc_calc16(uint16_t* data, uint32_t size);
uint32_t crc_calc8(uint8_t* data, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_CRC_H

/**
 * @} 
 * @}
 */
